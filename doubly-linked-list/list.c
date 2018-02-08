#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

struct list {
  Node head;
};


Node createNode(int data, Node prev, Node next) {
  Node n = malloc(sizeof(*n));
  assert(n && "Failed to allocate space for a new node");

  n->data = (int) data;
  assert(n->data == data && "Failed to assign provided data to the new node");

  n->prev = prev;
  assert(n->prev == prev && "Failed to assign provided prev pointer to the new node");

  n->next = next;
  assert(n->next == next && "Failed to assign provided next pointer to the new node");

  return n;
}

void _free(Node node) {
  free((void *) node);
}

List ListConstructor() {
  List l = malloc(sizeof(*l));
  assert(l && "Failed allocate space for the list");

  Node n = createNode(NULL, NULL, NULL);
  n->next = n->prev = n;

  l->head = n;
  assert(l->head && "Failed to set sentinel node as head");

  return l;
}

void ListDestructor(List l) {
  ListTraverse(l, _free, TRAVERSE_NORMAL);
  free(l->head);
  free(l);
}

void ListInsert(List l, int i) {
  assert(l && "Provide list where to insert item");

  Node n = createNode(i, l->head, l->head->next);
  assert(n && "Failed to create new list node");

  l->head->next->prev = n;
  assert(l->head->next->prev == n && "Failed to link new node as previous of the last element");

  l->head->next = n;
  assert(l->head->next == n && "Failed to link head to the new node as the next element");
}

void ListDelete(List l, int i) {
  Node n = ListSearch(l, i);
  assert(l->head->next != l->head && "List is empty, underflow");
  assert(n && "Node should be present in list to be eligible for delete");

  n->prev->next = n->next;
  n->next->prev = n->prev;

  free(n);
}

Node ListSearch(List l, int i) {
  assert(l && "Provide list to perform search on");
  assert(l->head && "There should be head in the list you want to search");
  Node cursor = l->head;

  while ((cursor = cursor->next) != l->head) {
    if (cursor->data == i) {
      break;
    }
  }

  return cursor == l->head ? NULL : cursor;
}

void ListTraverse(List l, void (*f)(Node), int direction) {
  assert(l && "Provide list you want to traverse");
  assert(l->head && "List should have head element");
  assert(f && "Provide function you want to pass each element to");
  assert((direction == TRAVERSE_NORMAL || direction == TRAVERSE_REVERSE) && "Provide traverse function with legit direction to traverse list in");
  
  Node cursor = l->head;

  while ((cursor = direction == TRAVERSE_NORMAL ? cursor->next : cursor->prev) != l->head) {
    f(cursor);
  }
}

