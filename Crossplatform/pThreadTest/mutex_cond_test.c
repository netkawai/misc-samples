#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

static pthread_mutex_t	mutex = NULL;
static pthread_cond_t	cond = NULL;
static unsigned int		count = 0;

/* nanosleep pthread_cond_signalの挙動がハッキリしないので作成した
   プログラム。スレッド作成前に、conditionに使用するpthread_cond_tと
   ptrehad_mutex_tの初期化を行い、通知スレッドを先に作成した。
   thread1は8秒経つまでpthread_cond_siginalは行われない

   その後、thread1からcond_signalを送信すると、一度thread2のcondition 待ちが
   解除されるはずだが、linuxでは解除されず、さらに一度解除した後、mainスレッドから
   再度解除を試みるが、linuxでは上手くいかない。

   I don't know the behavior of nanosleep and pthread_cond_signal.
   Before creating a thread, it initializes pthread_cond_t and pthread_mutex_t for
   condition, and  thread_1 will notify a signal(condition) after 8 seconds with pthread_cond_signal.
   
   after thread_1 send the signal, thread_2 will be released from waiting of condition, in 32bit linux,
   it will not be released. Moreover, if main thread trys to release the condition also, it will not be released.

   Intel MacOS X(32bit) is success. but MacOS X use the long mode with compatibility mode to support 64bit program
   Linux(x86_64) is success, only 32bit(i386) version is stuck up. I don't know why.
   
*/

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
#if 0　//  second notification
	pthread_cond_signal(&cond);
#endif
	// force to call cancel(Android does not have it)
	pthread_cancel(th2);
  err = pthread_join(th2, NULL);

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
	return (void *) NULL;

}


void *thread2(void *dummy)
{

	struct timespec		req;

	req.tv_sec  = 0;
	req.tv_nsec = 10*1000*1000;

  pthread_cleanup_push(pthread_mutex_unlock,&mutex);

	while (1) {

//	putchar('1');

		pthread_mutex_lock(&mutex);
		pthread_cond_timedwait(&cond, &mutex,&req);
		pthread_mutex_unlock(&mutex);
//		break;
		
//	putchar('4');

	}

	pthread_cleanup_pop(0);

  pthread_exit(0);
  return (void *) NULL;
}
