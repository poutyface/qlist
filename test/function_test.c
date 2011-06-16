#include "test_helper.h"
#include "qlist.h"

typedef struct Item{
  struct qitem item;
  int number;
}Item;


int main()
{
  Qlist *q;
  int rc = 0;
  Item item1, item2, item3;
  Item *item;

  item1.number = 1;
  item2.number = 2;
  item3.number = 3;


  q = qlist_create();
  check(q != NULL, "qlist_create == NULL");
  
  // test qlist_enq
  qlist_enq(q, &item1);
  qlist_enq(q, &item2);
  qlist_enq(q, &item3);

  // test qlist_size
  rc = qlist_size(q);
  check(rc == 3, "qlist_size != 3");

  // test qlist_deq
  item = qlist_deq(q);
  check(item == &item1, "qlist_deq item != &item1");
  item = qlist_deq(q);
  check(item == &item2, "qlist_deq item != &item2");
  item = qlist_deq(q);
  check(item == &item3, "qlist_deq item != &item3");
  item = qlist_deq(q);
  check(item == NULL, "qlist_deq item != NULL");
  rc = qlist_size(q);
  check(rc == 0, "qlist_size != 0");

  // test qlist_unshift
  qlist_enq(q, &item1);
  qlist_unshift(q, &item2);
  rc = qlist_size(q);
  check(rc == 2, "qlist_size != 2");
  item = qlist_deq(q);
  check(item == &item2, "qlist_deq item != &item2");
  item = qlist_deq(q);
  check(item == &item1, "qlist_deq item != &item1");
  item = qlist_deq(q);
  check(item == NULL, "qlist_deq item != NULL");
  rc = qlist_size(q);
  check(rc == 0, "qlist_size != 0");

#ifndef NFD
  {
    int i, n;
    fd_set fds;
    
    qlist_enq(q, &item1);
    qlist_enq(q, &item2);
    qlist_enq(q, &item3);

    for(i=0; i<3; ++i){
      FD_ZERO(&fds);
      FD_SET(q->fd.r, &fds);
      n = select(q->fd.r + 1, &fds, NULL, NULL, NULL);
      if(n > 0)
        qlist_deq(q);
      else
        goto error;
    }
  }
#endif

  qlist_destroy(q);
  return 0;

 error:
  return -1;
}
