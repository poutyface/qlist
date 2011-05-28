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


  q = Qlist_create();
  check(q != NULL, "Qlist_create == NULL");
  
  // test Qlist_enq
  Qlist_enq(q, &item1);
  Qlist_enq(q, &item2);
  Qlist_enq(q, &item3);

  // test Qlist_size
  rc = Qlist_size(q);
  check(rc == 3, "Qlist_size != 3");

  // test Qlist_deq
  item = Qlist_deq(q);
  check(item == &item1, "Qlist_deq item != &item1");
  item = Qlist_deq(q);
  check(item == &item2, "Qlist_deq item != &item2");
  item = Qlist_deq(q);
  check(item == &item3, "Qlist_deq item != &item3");
  item = Qlist_deq(q);
  check(item == NULL, "Qlist_deq item != NULL");
  rc = Qlist_size(q);
  check(rc == 0, "Qlist_size != 0");

  // test Qlist_unshift
  Qlist_enq(q, &item1);
  Qlist_unshift(q, &item2);
  rc = Qlist_size(q);
  check(rc == 2, "Qlist_size != 2");
  item = Qlist_deq(q);
  check(item == &item2, "Qlist_deq item != &item2");
  item = Qlist_deq(q);
  check(item == &item1, "Qlist_deq item != &item1");
  item = Qlist_deq(q);
  check(item == NULL, "Qlist_deq item != NULL");
  rc = Qlist_size(q);
  check(rc == 0, "Qlist_size != 0");

#ifndef NFD
  {
    int i, n;
    fd_set fds;
    
    Qlist_enq(q, &item1);
    Qlist_enq(q, &item2);
    Qlist_enq(q, &item3);

    for(i=0; i<3; ++i){
      FD_ZERO(&fds);
      FD_SET(q->fd.r, &fds);
      n = select(q->fd.r + 1, &fds, NULL, NULL, NULL);
      if(n > 0)
        Qlist_deq(q);
      else
        goto error;
    }
  }
#endif

  Qlist_destroy(q);
  return 0;

 error:
  return -1;
}
