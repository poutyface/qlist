#ifndef _qlist_h
#define _qlist_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct qitem{
  struct qitem *next;
};

typedef struct Qlist{
  struct qitem *first, *last;
  unsigned int size;
  pthread_mutex_t mutex;
}Qlist;


Qlist* Qlist_create();

void Qlist_enq(Qlist *q, void *item);

void* Qlist_deq(Qlist *q);

void Qlist_unshift(Qlist *q, void *item);

unsigned int Qlist_size(Qlist *q);

void Qlist_destroy(Qlist *q);

#endif