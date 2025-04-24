/*
 * @version 0.1.0
 * @copyright 2025 sun0day 
 * @description Linked list 
 */

#ifndef TANC_LLIST_H
#define TANC_LLIST_H

#define LinkedList(type) \
  struct List_##type { \
    type data; \
    struct List_##type *next; \
  };

#endif
