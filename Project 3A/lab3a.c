// NAME:Belle Lerdworatawee
// EMAIL: bellelee@g.ucla.edu
// ID: 105375663

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "ext2_fs.h"

#define EXT2_SUPER_MAGIC 0xEF53
#define SUPERBLOCK_OFFSET 1024

unsigned int block_size;
int fd = 0;
struct ext2_super_block sb;
struct ext2_group_desc gd;

// converts block number to its offset
unsigned int calc_offset(unsigned int block_num) {
  return SUPERBLOCK_OFFSET + (block_num-1) * block_size;
}

// print directory entries
unsigned int print_dirent(unsigned int inode_no, struct ext2_inode *inode, 
                          unsigned int block_num, unsigned int base_offset) {
  void* block = malloc(block_size);
  unsigned int iter = 0;

  pread(fd, block, block_size, calc_offset(block_num));
  struct ext2_dir_entry* entry = (struct ext2_dir_entry*) block;   
  // loop through directory entry
  while (iter < inode->i_size && entry->file_type) {
    if (entry->inode) {
      char filename[EXT2_NAME_LEN + 1];
      memcpy(filename, entry->name, entry->name_len);
      filename[entry->name_len] = '\0';
      fprintf(stdout, "DIRENT,%u,%u,%u,%u,%u,'%s'\n",
              inode_no,
              base_offset + iter,
              entry->inode,
              entry->rec_len,
              entry->name_len,
              filename
              );
    }
    iter += entry->rec_len;
    entry = (void*)entry + entry->rec_len;
  }
  free(block);
  return iter;
}

int main(int argc, char* argv[]) {
  int ret = 0;
  
  if (argc != 2) {
    fprintf(stderr, "Error the wrong number of arguments was provided\n");
    exit(1);
  }

  if ((fd = open(argv[1], O_RDONLY)) < 0) {
    fprintf(stderr, "Error the provided argument cannot be opened\n");
    exit(1);
  };

  // save superblock into a struct
  if ((ret = pread(fd, &sb, sizeof(sb), SUPERBLOCK_OFFSET)) < 0) {
    fprintf(stderr, "Error with reading file for super block contents\n");
    exit(2);
  }
  if (sb.s_magic != EXT2_SUPER_MAGIC) {
    fprintf(stderr, "Error the image file is corrupted, superblock is not at offset 1024\n");
    exit(2);
  }
  block_size = EXT2_MIN_BLOCK_SIZE << sb.s_log_block_size;

  // print super block
  fprintf(stdout, "SUPERBLOCK,%u,%u,%u,%u,%u,%u,%u\n",
          sb.s_blocks_count,
          sb.s_inodes_count,
          block_size,
          sb.s_inode_size,
          sb.s_blocks_per_group,
          sb.s_inodes_per_group, 
          sb.s_first_ino
          );

  // save group descriptor into a struct 
  if ((ret = pread(fd, &gd, sizeof(gd), SUPERBLOCK_OFFSET + block_size)) < 0) {
    fprintf(stderr, "Error with reading file for group descriptor\n");
    exit(2);
  }
  unsigned int total_g_blocks = (sb.s_blocks_count < sb.s_blocks_per_group) ? sb.s_blocks_count : sb.s_blocks_per_group;
  unsigned int total_g_inodes = (sb.s_inodes_count < sb.s_inodes_per_group) ? sb.s_inodes_count : sb.s_inodes_per_group;
  
  // print group descriptor
  fprintf(stdout, "GROUP,%u,%u,%u,%u,%u,%u,%u,%u\n",
          sb.s_block_group_nr,
          total_g_blocks,
          total_g_inodes,
          gd.bg_free_blocks_count,
          gd.bg_free_inodes_count,
          gd.bg_block_bitmap,
          gd.bg_inode_bitmap,
          gd.bg_inode_table
  ); 

  // read block bitmap
  unsigned char block_bitmap[block_size];
  if ((ret = pread(fd, block_bitmap, block_size, calc_offset(gd.bg_block_bitmap))) < 0) {
    fprintf(stderr, "Error with reading file for block bitmap\n");
    exit(2);
  }
  for (unsigned int block_no = 1; block_no <= total_g_blocks; block_no++) {
    int index = (block_no - 1) / 8;
    int offset = (block_no - 1) % 8;
    int allocated = (block_bitmap[index] & (1 << offset)); 
    if (!allocated)
      fprintf(stdout, "BFREE,%u\n", block_no);
  }

  // read inode bitmap
  unsigned char inode_bitmap[block_size];
  if ((ret = pread(fd, inode_bitmap, block_size, calc_offset(gd.bg_inode_bitmap))) < 0) {
    fprintf(stderr, "Error with reading file for inode bitmap\n");
    exit(2);
  }
  for (unsigned int inode_no = 1; inode_no <= total_g_inodes; inode_no++) {
    int index = (inode_no - 1) / 8;
    int offset = (inode_no - 1) % 8;
    int allocated = (inode_bitmap[index] & (1 << offset));
    if (!allocated)
      fprintf(stdout, "IFREE,%u\n", inode_no);
  }

  // loop through inodes
  int table_offset = calc_offset(gd.bg_inode_table);
  for (unsigned int inode_no = 1; inode_no <= total_g_inodes; inode_no++) {
    struct ext2_inode inode;
    int inode_offset = table_offset + (inode_no-1) * sb.s_inode_size;
    if ((ret = pread(fd, &inode, sizeof(inode), inode_offset)) < 0) {
      fprintf(stderr, "Error with reading inode\n");
      exit(2);
    } 
    
    // check if inode is allocated
    if (!inode.i_mode || !inode.i_links_count)
      continue;   
 
    char file_type = '?';
    if (S_ISREG(inode.i_mode))
      file_type = 'f';
    else if (S_ISDIR(inode.i_mode))
      file_type = 'd';
    else if (S_ISLNK(inode.i_mode))
      file_type = 's';
    
    char ctime[18], mtime[18], atime[18];
    time_t c = inode.i_ctime, m = inode.i_mtime, a = inode.i_atime;
    strftime(ctime, sizeof(ctime), "%m/%d/%y %H:%M:%S", gmtime(&c));
    strftime(mtime, sizeof(mtime), "%m/%d/%y %H:%M:%S", gmtime(&m));
    strftime(atime, sizeof(atime), "%m/%d/%y %H:%M:%S", gmtime(&a));

    // print first 12 inode info
    fprintf(stdout, "INODE,%u,%c,%o,%u,%u,%u,%s,%s,%s,%u,%u",
            inode_no,
            file_type,
            inode.i_mode & 0x0FFF,
            inode.i_uid,
            inode.i_gid,
            inode.i_links_count,
            ctime,
            mtime,
            atime,
            inode.i_size,
            inode.i_blocks
            );
    // print last 15 blocks
    if (file_type == 'f' || file_type == 'd' || (file_type == 's' && inode.i_size > 60)) {
      for (int i = 0; i < EXT2_N_BLOCKS; i++)
        fprintf(stdout, ",%u", inode.i_block[i]);
    }
    fprintf(stdout, "\n");

    // directory inodes
    unsigned int base_offset = 0;
    if (file_type == 'd') {
      for (int i = 0; i < EXT2_NDIR_BLOCKS; i++) {
        if (inode.i_block[i] != 0) {
          unsigned int add = print_dirent(inode_no, &inode, inode.i_block[i], base_offset);
          base_offset += add;
        }
      }
    }
 
    // directory and file inodes
    if (file_type == 'd' || file_type == 'f') {
      unsigned int total_entries = block_size / sizeof(__u32); // according to gdb, entries are __32 
      if (inode.i_block[12]) {
        __u32 block[block_size];
        pread(fd, block, block_size, calc_offset(inode.i_block[12]));
 
        // scan level 1 block
        for (__u32 j = 0; j < total_entries; j++) {
          if (block[j]) {
            if (file_type == 'd') {
              unsigned int add = print_dirent(inode_no, &inode, block[j], base_offset);
              base_offset += add;
            }
            fprintf(stdout, "INDIRECT,%u,1,%u,%u,%u\n",
                    inode_no,
                    j + 12, // + 12 for the first 12 entries
                    inode.i_block[12],
                    block[j]
                    );
          }
        }
      }
      if (inode.i_block[13]) {
        __u32 block2[block_size];
        pread(fd, block2, block_size, calc_offset(inode.i_block[13]));

        // scan level 2 block
        for (__u32 j = 0; j < total_entries; j++) {
          if (block2[j]) {
            fprintf(stdout, "INDIRECT,%u,2,%u,%u,%u\n",
                    inode_no,
                    j + 256 + 12, // + 12 for first 12 entries, + 256 for i_block[12] blocks
                    inode.i_block[13],
                    block2[j]
                    );

            __u32 block1[block_size];
            pread(fd, block1, block_size, calc_offset(block2[j]));
 
            // scan level 1 block pointed to by level 2 entries
            for (__u32 k = 0; k < total_entries; k++) {
              if (block1[k]) {
                if (file_type == 'd') {
                  unsigned int add = print_dirent(inode_no, &inode, block1[k], base_offset);
                  base_offset += add;
                };
                fprintf(stdout, "INDIRECT,%u,1,%u,%u,%u\n",
                        inode_no,
                        k + 256 + 12, // + 12 for the first 12 entries, + 256 for i_block[12] blocks
                        block2[j],
                        block1[k]
                        );
              }
            }           
          }
        }
      }
      if (inode.i_block[14]) {
        __u32 block3[block_size];
        pread(fd, block3, block_size, calc_offset(inode.i_block[14]));
        
        // scan level 3 block
        for (__u32 j = 0; j < total_entries; j++) {
          if (block3[j]) {
            fprintf(stdout, "INDIRECT,%u,3,%u,%u,%u\n",
                    inode_no,
                    j + 256*256 + 256 + 12, // + 12 for first 12 entries, + 256 for i_block[12] blocks, rest for i_block[13] blocks
                    inode.i_block[14],
                    block3[j]
                    );

            __u32 block2[block_size];
            pread(fd, block2, block_size, calc_offset(block3[j]));

            // scan level 2 block pointed to by level 3 entries
            for (__u32 k = 0; k < total_entries; k++) {
              if (block2[k]) {
                fprintf(stdout, "INDIRECT,%u,2,%u,%u,%u\n",
                        inode_no,
                        k + 256*256 + 256 + 12, // + 12 for first 12 entries, + 256 for i_block[12] blocks, rest for i_block[13] blocks
                        block3[j],
                        block2[k]
                        );
             
                __u32 block1[block_size];
                pread(fd, block1, block_size, calc_offset(block2[k]));

                // scan level 1 block pointed to by level 2 entries
                for (__u32 m = 0; m < total_entries; m++) {
                  if (block1[m]) {
                    if (file_type == 'd') {
                      unsigned int add = print_dirent(inode_no, &inode, block1[m], base_offset);
                      base_offset += add;
                    }
                    fprintf(stdout, "INDIRECT,%u,1,%u,%u,%u\n",
                            inode_no,
                            m + 256*256 + 256 + 12, // + 12 for the first 12 entries, + 256 for i_block[12] blocks
                            block2[k],
                            block1[m]
                            );
                  }
                }  
              }
            }
          }
        }
      }
    }
  }
 
  exit(0);
}
