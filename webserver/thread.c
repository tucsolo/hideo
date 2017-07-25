#include "utils.h"
#include "thread.h"
#include "libhttp.h"
#include <time.h>


void millisleep(int milliseconds)
{
	usleep(milliseconds * 1000);
}

void thread_make(int i)
{
	void *thread_main(void *);
	pthread_attr_t attr;
	int err;
	tptr[i].thread_count = i;


	if ((err = pthread_attr_init(&attr)) != 0) {
		fprintf(stderr, "Error in pthread_attr_init: %d : %s\n", err, strerror(err));
		exit(EXIT_FAILURE);
	}

	if ((err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0) {
		fprintf(stderr, "Error in pthread_attr_setdetachstate: %d : %s\n", err, strerror(err));
		exit(EXIT_FAILURE);
	}

	if ((pthread_create(&tptr[i].thread_tid, &attr, &thread_main, &tptr[i])) != 0) {
		err_exit("Error on pthread_create", errno);
	}

	if ((err = pthread_attr_destroy(&attr)) != 0) {
		fprintf(stderr, "Error in pthread_attr_destroy: %d : %s\n", err, strerror(err));
		exit(EXIT_FAILURE);
	}

	return;			/* main thread returns */
}


void *thread_main(void *arg)
{
	int connsd, retval = 0;
	struct Thread *thread_data = (struct Thread *) arg;
	struct conndata *cdata;
	int j = 0;

	thread_data->thread_tid = pthread_self() / 256;
	printf("Thread created [%u]\n", (unsigned int) thread_data->thread_tid);
	fflush(stdout);

	while (1) {

		if (pthread_mutex_lock(&mtx) < 0)
			pthread_exit(NULL);

		if ((connsd = accept(listensd, (struct sockaddr *) NULL, NULL)) < 0)
			perror("error in accept\n");

		fprintf(stdout, "...connection accepted!\n");

		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 80000;

		if (setsockopt(connsd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0)
			unix_error("setsockopt failed\n");

		if (setsockopt(connsd, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) < 0)
			unix_error("setsockopt failed\n");

		if (pthread_mutex_unlock(&mtx) < 0)
			pthread_exit(NULL);

		cdata = create_conndata();
		cdata->socketint = connsd;
		cdata->process_id = thread_data->thread_tid;

		/******************************
			Serve request to the client
		*******************************/
		retval = 1;

		while (1) {
			printf("thread.c cicle: %d\n", j++);
			/*To be implemented: thread sleep until next client request */
			retval = serve_request(cdata);
			printf("retval: %d\n", retval);
			if (retval == ERROR) {
				Free(cdata);
				close(cdata->socketint);
				break;
			}

		}
	}

	return NULL;
}
