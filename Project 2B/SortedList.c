#include <sched.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "SortedList.h"

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
   if (list == NULL || element == NULL || list->key != NULL)
    return;

  SortedList_t* curr = list->next;
  // printf("Entered the insert function\n");
  // traverse the list
  while (curr != list && (strcmp(curr->key, element->key) < 0)) {
    curr = curr->next;
    // printf("Looping through the insert function\n");
  }

  if (opt_yield & INSERT_YIELD)
    sched_yield();

  element->next = curr;
  element->prev = curr->prev;
  curr->prev->next = element;
  curr->prev = element;

  return;
}

/* ret = 0 = success || ret = 1 = failure*/
int SortedList_delete(SortedListElement_t *element) {
  // check for corrupted pointers
  if ((element->prev->next != element) || (element->next->prev != element))
    return 1;

  if (opt_yield & DELETE_YIELD)
    sched_yield();

  // proceed with deletion
  element->next->prev = element->prev;
  element->prev->next = element->next;
  element->next = NULL;
  element->prev = NULL;
  return 0;
}

/* ret = element || ret = NULL if not found*/
SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
  SortedList_t* curr = list->next;

  // iterate through the list
  while ((curr->key != key) && (curr->key != NULL)) {
    if (opt_yield & LOOKUP_YIELD)
      sched_yield();
    curr = curr->next;
  }

  // now curr will be on the node that has the key or no key found
  if (curr->key == key) 
    return curr;
 
  return NULL;
}

/*does NOT include head*/
/* ret = len(list) || ret = -1 is list is corrupted*/
int SortedList_length(SortedList_t *list) {
  int count = 0;
  SortedList_t* curr = list->next;
  SortedList_t* prev = list;
  
  while (curr != list) {
    if (opt_yield & LOOKUP_YIELD)
      sched_yield();

    prev = curr;
    curr = curr->next;

    // check for corruption
    if ((curr->prev != prev) || (prev->next != curr))
      return -1;
 
    count++;
  }
  return count;   
}
