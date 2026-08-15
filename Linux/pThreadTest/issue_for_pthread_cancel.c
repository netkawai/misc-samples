#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

/* nanosleep 
   check the behavior of pthread_cond_signal

   Before creating a thread, initialize pthread_cond_t and pthrad_mutex_t
   and then create a signaling thread

   thread1 has 8 seconds interval before calling pthread_cond_signal
   However, the relase of wating the condition in thread2 does not happen.
   Moreover, if main thread calls pthread_cond_signal, the deadlock happened in linux 2.6.x series.
   I don't know exact reasons......    Oct 2009
   
   On 64bit(x86_64) environment (gcc 5.4.0) , it works properly.

   On 32bit (gcc 4.7.0) , it still does not work. 
   (Linux version 3.4.73-64.112.amzn1.i686)
   why? Nov 2017
*/


static pthread_mutex_t	mutex;
static pthread_cond_t	cond;
static unsigned int		count = 0;

void *thread1(void *dummy);
void *thread2(void *dummy);

int main (void)
{

    int							i, rtn;
    pthread_t					th1, th2;
    int err;
	struct timespec		req, res;

	req.tv_sec  = 10;
	req.tv_nsec = 0;


	/* init mutex */
	rtn = pthread_mutex_init(&mutex, NULL);
	if (rtn != 0) {
		fprintf(stderr, "pthread_mutex_init() failed for %d.\n", rtn);
		exit(EXIT_FAILURE);
	}


	/* init cond */
	rtn = pthread_cond_init(&cond, NULL);
	if (rtn != 0) {
		fprintf(stderr, "pthread_cond_init() failed for %d.\n", rtn);
		exit(EXIT_FAILURE);
	}

	/* create threads */
	rtn = pthread_create(&th1, NULL, thread1, (void *)NULL);
	if (rtn != 0) {
		fprintf(stderr, "pthread_create() - thread#1 failed for %d.\n", rtn);
		exit(EXIT_FAILURE);
	}


	/* create threads */
	rtn = pthread_create(&th2, NULL, thread2, (void *)NULL);
	if (rtn != 0) {
		fprintf(stderr, "pthread_create() - thread#2 failed for %d.\n", rtn);
		exit(EXIT_FAILURE);
	}

	//	nanosleep(&req,&res);


	/* join */

	pthread_join(th1, NULL);
	printf("joined thread1 \n");	
#if 0 // 2nd signaling
	pthread_cond_signal(&cond);
#endif
	// force cancel
	pthread_cancel(th2);
	printf("cancled thread2 \n");	
    err = pthread_join(th2, NULL);
	printf("joined thread2 \n");	
	err = pthread_cond_destroy(&cond);
	printf("pthread_cond_destory %d\n",err);
	err = pthread_mutex_destroy(&mutex);
	printf("pthread_mutex_destroy %d\n",err);

    exit(EXIT_SUCCESS);

}


void *thread1(void *dummy)
{

	int					ret;
	struct timespec		req, res;

	req.tv_sec  = 1;
	req.tv_nsec = 0;

	while(1) {

		putchar('0');

		pthread_mutex_lock(&mutex);
		count++;
		pthread_mutex_unlock(&mutex);

		if (count == 8){
			pthread_cond_signal(&cond);
			break;
		}
		ret = nanosleep(&req, &res);
		while (ret != 0 && errno == EINTR)
			ret = nanosleep(&req, &res);

		if (ret != 0) {
			perror("nanosleep()");
			return (void *) NULL;
		}

		fflush(stdout);
	}

	pthread_exit(0);
    putchar('1');
	return (void *) NULL;

}


void *thread2(void *dummy)
{

	struct timespec		req;

	req.tv_sec  = 0;
	req.tv_nsec = 10*1000*1000;

    pthread_cleanup_push(pthread_mutex_unlock,&mutex);
	printf("set mutex_unlock in cleanup");
	printf("fall into the waiting...");
	while (1) {
		pthread_mutex_lock(&mutex);
		pthread_cond_timedwait(&cond, &mutex,&req);
		pthread_mutex_unlock(&mutex);		
	}

	pthread_cleanup_pop(0);

  pthread_exit(0);
  return (void *) NULL;
}
