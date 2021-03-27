#!/usr/bin/env python3

# NAME: Belle Lerdwoaratwee
# EMAIL: bellelee@g.ucla.edu
# ID: 105375663

import sys
import csv

# SUPERBLOCK, total blocks, total i-nodes, block size, i-node size, blocks/group, i-nodes/group, 
# first non-reserved i-node
class superblock:
   def __init__(self, row):
      self.totBlocks = int(row[1])
      self.totInodes = int(row[2])      
      self.blockSize = int(row[3])
      self.inodeSize = int(row[4])
      self.blocksPerGroup = int(row[5])
      self.inodesPerGroup = int(row[6])
      self.firstNonResInode = int(row[7])

# GROUP, group number, total blocks in group, total i-nodes in group, free blocks num, free inodes num
# free block bitmap block #, free inode bitmap block #, first block of inodes block #
class group:
   def __init__(self, row):
       self.groupNum = int(row[1])
       self.totBlocks = int(row[2])
       self.totInodes = int(row[3])
       self.totfreeBlocks = int(row[4])
       self.totFreeInodes = int(row[5])
       self.bfreeBlockNum = int(row[6])
       self.ifreeBlockNum = int(row[7])
       self.firstInodeBlockNum = int(row[8])

# INODE, inode #, file type, mode, owner, group, link count, time of last I-node change, 
# modification time, last access time, file size, number of (512 byte) blocks of disk space
class inode:
   def __init__(self, row):
      self.inodeNum = int(row[1])
      self.filetype = row[2]
      self.mode = int(row[3])
      self.owner = int(row[4])
      self.group = int(row[5])
      self.linkCount = int(row[6])
      self.changeTime = row[7]
      self.modTime = row[8]
      self.accessTime = row[9]
      self.filesize = int(row[10])
      self.numBlocks = int(row[11])
      self.blocks = {} # map block number to its level of indirection and offset
      if self.filetype == "d" or self.filetype == "f":
         for i in range(12,24):
            self.blocks[ int(row[i]) ] = ["BLOCK", i - 12]
         self.blocks[ int(row[24]) ] = ["INDIRECT BLOCK", 12]
         self.blocks[ int(row[25]) ] =  ["DOUBLE INDIRECT BLOCK", 268]
         self.blocks[ int(row[26]) ] =  ["TRIPLE INDIRECT BLOCK", 65804]

# DIRENT, parent inode number, logical byte offset, referenced file inode number, entry length, name length, name 
class dirent:
   def __init__(self, row):
      self.parentInodeNum = int(row[1])
      self.logiByteOffset = int(row[2])
      self.inodeNum = int(row[3])
      self.entryLen = int(row[4])
      self.nameLen = int(row[5])
      self.name = row[6]

# INDIRECT, owning file inode number, level of indirection for the block being scanned, logical block offset
# block number of the (1, 2, 3) indirect block being scanned, block number of the referenced block
class indirect:
   def __init__(self, row):
      self.inodeNum = int(row[1])
      self.indirLevel = int(row[2])
      self.logiBlockOffset = int(row[3])
      self.indirBlockNum = int(row[4])
      self.refBlockNum = int(row[5])

def auditBlocks():
   # maps block number to [number of occurrences, level of indirection, inode number, and offset]
   global exitStatus
   referenced = {} # all the blocks that are referenced
   
   for inode in inodes:
      # unused
      if inode.inodeNum == 0:
         continue
       
      # check all blocks in inodes
      for bNum in inode.blocks:
         if bNum == 0:
            continue
         my_bitmap[bNum] = 1 
         # invalid
         if bNum < 0 or bNum > sb.totBlocks:
            print(f"INVALID {inode.blocks[bNum][0]} {bNum} IN INODE {inode.inodeNum} AT OFFSET {inode.blocks[bNum][1]}")
            exitStatus = 2
         # reserved
         elif bNum > 0 and bNum < firstLegalBlock:
            print(f"RESERVED {inode.blocks[bNum][0]} {bNum} IN INODE {inode.inodeNum} AT OFFSET {inode.blocks[bNum][1]}")
            exitStatus = 2
         # block is marked as free when it is allocated
         elif bNum in bfree:
            print(f"ALLOCATED BLOCK {bNum} ON FREELIST")
            exitStatus = 2
         # store duplicate block
         elif bNum in referenced:
            referenced[bNum].append([ inode.blocks[bNum][0], inode.inodeNum, inode.blocks[bNum][1] ])
            exitStatus = 2
         # add valid block to referenced
         else:
            referenced[bNum] = [ [inode.blocks[bNum][0], inode.inodeNum, inode.blocks[bNum][1]] ] 
   
   # duplicates
   for block in referenced:
      if len(referenced[block]) > 1:
         for ref in referenced[block]:
            print(f"DUPLICATE {ref[0]} {block} IN INODE {ref[1]} AT OFFSET {ref[2]}")
            exitStatus = 2
   
   # unreferenced blocks
   for i in range(firstLegalBlock, sb.totBlocks):
      if my_bitmap[i] == 0 and i not in bfree:
         print(f"UNREFERENCED BLOCK {i}")
   
def auditInodes():
   global exitStatus
   
   # allocated
   for curr in inodes:
      if curr.inodeNum != 0 and curr.inodeNum in ifree:
         print(f"ALLOCATED INODE {curr.inodeNum} ON FREELIST")
         exitStatus = 2
      
   # unallocated
   for inodeNum in range(sb.firstNonResInode, sb.totInodes):
       if inodeNum not in allocatedInodes and inodeNum not in ifree:
           print(f"UNALLOCATED INODE {inodeNum} NOT ON FREELIST")
           exitStatus = 2

def auditDir():
   global exitStatus
   inodeParent = {}
   linkCount = {}
   
   for dirent in dirs:
      # invalid
      if dirent.inodeNum > sb.totInodes:
         print(f"DIRECTORY INODE {dirent.parentInodeNum} NAME {dirent.name} INVALID INODE {dirent.inodeNum}")
         exitStatus = 2
      # unallocated
      elif dirent.inodeNum not in allocatedInodes:
         print(f"DIRECTORY INODE {dirent.parentInodeNum} NAME {dirent.name} UNALLOCATED INODE {dirent.inodeNum}")
         exitStatus = 2
      # valid and we add to its link count
      else:
         linkCount[dirent.inodeNum] = linkCount.get(dirent.inodeNum, 0) + 1

   for inode in inodes:
      # inode has mismatch in links
      if inode.inodeNum in linkCount and linkCount[inode.inodeNum] != inode.linkCount:
         print(f"INODE {inode.inodeNum} HAS {linkCount[inode.inodeNum]} LINKS BUT LINKCOUNT IS {inode.linkCount}")
         exitStatus = 2
      # inode is referenced 0 times
      elif inode.inodeNum not in linkCount and inode.linkCount > 0:
         print(f"INODE {inode.inodeNum} HAS 0 LINKS BUT LINKCOUNT IS {inode.linkCount}")
         exitStatus = 2
   
   # inode 2 is always the root directory
   # this always has 2 links because it still has . (itself) and .. (still itself)
   inodeParent[2] = 2
   # find parent inodes for each inode
   for dirent in dirs:
      if dirent.inodeNum <= sb.totInodes and dirent.inodeNum in allocatedInodes:
         if dirent.name != "'.'" and dirent.name != "'..'":
            inodeParent[dirent.inodeNum] = dirent.parentInodeNum
   
   # check . and ..
   for dirent in dirs:
      if dirent.name == "'..'" and inodeParent[dirent.parentInodeNum] != dirent.inodeNum:
            print(f"DIRECTORY INODE {dirent.parentInodeNum} NAME '..' LINK TO INODE {dirent.inodeNum} SHOULD BE {inodeParent[dirent.parentInodeNum]}")
            exitStatus = 2
      elif dirent.name == "'.'" and dirent.inodeNum != dirent.parentInodeNum:
            print(f"DIRECTORY INODE {dirent.parentInodeNum} NAME '.' LINK TO INODE {currDirent.inodeNum} SHOULD BE {currDirent.parentInodeNum}")
            exitStatus = 2

# global variables from csv
sb = None
gp = None
bfree = set()
ifree = set()
inodes = []
dirs = []
indirs = []

# global variables for record keeping
exitStatus = 0
allocatedInodes = set()
firstLegalBlock = 0
my_bitmap = {} # 0 = unreferenced, 1 = used 
parent = {}
inodeRefCount = {}

if __name__ == "__main__":
   if len(sys.argv) != 2:
      sys.stderr.write("Usage: ./lab3b file.csv\n")
      sys.exit(1)

   try:
      csvfile = open(sys.argv[1], 'r')
   except:
      sys.stderr.write("Error with opening csv file\n")
      sys.exit(1)

   rows = csv.reader(csvfile)
   
   for row in rows:
      if len(row) < 1:
         sys.stderr.write("Error the csv file has an empty line\n")
         sys.exit(2)         

      # extract data
      type = row[0]
      if type == "SUPERBLOCK":
         sb = superblock(row)
      elif type == "GROUP":
         gp = group(row)
      elif type == "BFREE":
         bfree.add(int(row[1]))
      elif type == "IFREE":
         ifree.add(int(row[1]))
      elif type == "INODE":
         inodes.append(inode(row))
      elif type == "DIRENT":
         dirs.append(dirent(row))
      elif type == "INDIRECT":
         indirs.append(indirect(row))
      else:
         sys.stderr.write("Error the csv file has an invalid line\n")
         sys.exit(2)
   
   # create set of inodes that are allocated and audit inodes
   for entry in inodes:
      if entry.inodeNum != 0:
         allocatedInodes.add(entry.inodeNum)
   auditInodes()
   
   # collect all blocks and audit blocks
   firstLegalBlock = int( (gp.totInodes*sb.inodeSize)/sb.blockSize + gp.firstInodeBlockNum )
   for i in range(firstLegalBlock, sb.totBlocks):
      my_bitmap[i] = 0
   for entry in indirs:
      my_bitmap[entry.refBlockNum] = 1
   auditBlocks()
   
   # audit direct entries
   auditDir()
   
   exit(exitStatus)  
