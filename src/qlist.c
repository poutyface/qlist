#include "qlist.h"

Qlist* qlist_create()
{
  int rc;
  Qlist *q = (Qlist*)calloc(1, sizeof(Qlist));
  if(!q)
    return NULL;

  rc = pthread_mutex_init(&q->mutex, NULL);
  if(rc != 0)
    return NULL;

#ifndef NFD
  rc = pipe((int*)&q->fd);
  if(rc != 0)
    return NULL;
#endif  

  return q;
}


void qlist_destroy(Qlist *q)
{
#ifndef NFD
  close(q->fd.r);
  close(q->fd.w);
#endif
  pthread_mutex_destroy(&q->mutex);
  free(q);
}


#ifndef NFD
static void qlist_write_fd(Qlist *q)
{
  int rc;
  unsigned char m = '.';
  while(1){
    rc = write(q->fd.w, &m, sizeof(m));
    if(rc != 1) continue;
    break;
  }
}


static void qlist_read_fd(Qlist *q)
{
  int rc;
  unsigned char m;
  while(1){
    rc = read(q->fd.r, &m, sizeof(m));
    if(rc != 1) continue;
    break;
  }
}
#endif


void qlist_enq(Qlist *q, void *item)
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

#ifndef NFD
  qlist_write_fd(q);
#endif

  pthread_mutex_unlock(&q->mutex);
}


void* qlist_deq(Qlist *q)
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
#ifndef NFD
    qlist_read_fd(q);
#endif
  }

  pthread_mutex_unlock(&q->mutex);

  return (void*)tmp;
}


void qlist_unshift(Qlist *q, void *item)
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

#ifndef NFD
  qlist_write_fd(q);
#endif

  pthread_mutex_unlock(&q->mutex);
}


unsigned int qlist_size(Qlist *q)
{
  unsigned int size;
  pthread_mutex_lock(&q->mutex);
  size = q->size;
  pthread_mutex_unlock(&q->mutex);
  return size;
}





