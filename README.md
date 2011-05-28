qlist is a queue with FD. 

When I want a queue in C, there are not proper queue libraries.
Then I always write and rewrite a queue for some system's suit. It's boring me!


You know, Queue performs enqueue and dequeue. It's a queue. And ...

Sometimes, you want pushback(unshift) queue items on reciever side. 
qlist is able to this.

Queue is used to command queue in a thread system. 
You always check whether queue has items or you will implement monitor or condition signal/wait.
But I wish that queue is file descriptor(FD). if queue is FD, I am able to use 
select, poll or epoll. WAIT! that is able to use pipe, mkfifo, socketpair or message queue(IPC).
But these are not able to pushback easily.
qlist has FD (backend is pipe) and is able to pushback. 
But you remove this feature by compile option(make OPTS='-DNFD'), it works like a normal queue.

Example
=======
~~~~
// you have to set *struct qitem* to your queue item structe. 
typedef struct Item{
 struct qitem item;     // <- *THIS*
 int number;            
}Item;


int main()
{
  Qlist *q;
  int rc;
  Item item1, item2, item3;
  Item *item;

  item1.number = 1;
  item2.number = 2;
  item3.number = 3;

  // create queue 
  q = Qlist_create();
  
  // enqueue items
  Qlist_enq(q, &item1);
  Qlist_enq(q, &item2);
  Qlist_enq(q, &item3);
  // pushback(unshift) item
  Qlist_unshift(q, &item3);

  // check queue size
  rc = Qlist_size(q);  // rc == 4

  // dequeue items
  item = Qlist_deq(q);  // item == &item3  it was unshifted item
  item = Qlist_deq(q);  // item == &item1
  item = Qlist_deq(q);  // item == &item2
  item = Qlist_deq(q);  // item == &item3
  item = Qlist_deq(q);  // item == NULL
  
  // wait by calling select
  {
    int i, n;
    fd_set fds;
    
    Qlist_enq(q, &item1);
    Qlist_enq(q, &item2);
    Qlist_enq(q, &item3);

    for(i=0; i<3; ++i){
      FD_ZERO(&fds);
      FD_SET(q->fd.r, &fds);     // q->fd[0] is qlist's reader FD.
      n = select(q->fd.r+1, &fds, NULL, NULL, NULL);
      if(n > 0)
        Qlist_deq(q);
    }
  }

  // delete queue
  Qlist_destroy(q);

  return 0;
}
~~~~

learn more ... to see test/function_test.c


