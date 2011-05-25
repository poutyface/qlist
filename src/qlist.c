#include "qlist.h"

Qlist* Qlist_create()
{
  int rc;
  Qlist *q = (Qlist*)calloc(1, sizeof(Qlist));
  if(!q)
    return NULL;

  rc = pthread_mutex_init(&q->mutex, NULL);
  if(rc != 0)
    return NULL;

  return q;
}


void Qlist_destroy(Qlist *q)
{
  pthread_mutex_destroy(&q->mutex);
  free(q);
}


void Qlist_enq(Qlist *q, void *item)
{
  pthread_mutex_lock(&q->mutex);

  if(q->first == NULL){
    q->first = q->last = (struct qitem*)item;
  }
  else{
    q->last->next = (struct qitem*)item;
    q->last = q->last->next;
  }

  q->last->next = NULL;
  q->size++;

  pthread_mutex_unlock(&q->mutex);
}


void* Qlist_deq(Qlist *q)
{
  struct qitem *tmp;

  pthread_mutex_lock(&q->mutex);

  if(!q->first){
    tmp = NULL;
  }
  else{
    tmp = q->first;
    q->first = q->first->next;
    q->size--;
  }

  pthread_mutex_unlock(&q->mutex);

  return (void*)tmp;
}


void Qlist_unshift(Qlist *q, void *item)
{
  struct qitem *tmp;

  pthread_mutex_lock(&q->mutex);

  if(q->first == NULL){
    q->first = q->last = (struct qitem*)item;
    q->last->next = NULL;
  }
  else{
    tmp = q->first;
    q->first = item;
    q->first->next = tmp;
  }

  q->size++;
  pthread_mutex_unlock(&q->mutex);
}


unsigned int Qlist_size(Qlist *q)
{
  unsigned int size;
  pthread_mutex_lock(&q->mutex);
  size = q->size;
  pthread_mutex_unlock(&q->mutex);
  return size;
}





