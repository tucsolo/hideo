#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* Server TCP pre-threaded with dynamic pool size
  accept() on main, worker thread with condition and mutex
*/


int				listensd, nthreads;
socklen_t		addrlen;
pthread_mutex_t	mtx;


struct Thread{
  int connsock;
  pthread_t		thread_tid;		/* thread ID */
  long			thread_count;	/* # connections handled */
};

struct Thread	*tptr;		/* array of Thread structures; calloc'ed */

void create_pthread(pthread_t *, void *, void *, int);

void lock(pthread_mutex_t *);
void unlock(pthread_mutex_t *);
void thread_make(int );
void millisleep(int milliseconds);

#endif