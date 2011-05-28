#ifndef _qlist_h
#define _qlist_h

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef NFD
#include <unistd.h>
#include <errno.h>
#endif

struct qitem{
  struct qitem *next;
};

typedef struct Qlist{
  struct qitem *first, *last;
  unsigned int size;
  pthread_mutex_t mutex;
#ifndef NFD
  struct fds{
    int r;
    int w;
  }fd;
#endif
}Qlist;


Qlist* Qlist_create();

void Qlist_enq(Qlist *q, void *item);

void* Qlist_deq(Qlist *q);

void Qlist_unshift(Qlist *q, void *item);

unsigned int Qlist_size(Qlist *q);

void Qlist_destroy(Qlist *q);

#endif
