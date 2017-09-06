#include <string.h>
#include <stdlib.h>

#include <pthread.h>

#include <threll.h>

typedef struct {
	int x;
} input_t;

typedef struct {
	double x;
} output_t;

int cb (void *_input, void *_output) {
	input_t *input = (input_t *) _input;
	output_t *output = (output_t *) _output;
	output->x = (double) (input->x);
	return 0;
}

typedef int (*thservercb) (void *, void *) ;

pthread_mutex_t inq_mutex;
pthread_mutex_t outq_mutex;

int thserver (
	caq_t *inq, caq_t *outq,
	void *intmp, void *outtmp,
	thservercb cb) {
	while (! isempty (inq)) { /* while true ? */
		pthread_mutex_lock (&inq_mutex);
		memcpy (intmp, dequeue (inq), inq->esz);
		pthread_mutex_unlock (&inq_mutex);

		if (cb (intmp, outtmp) != 0) return -1;

		pthread_mutex_lock (&outq_mutex);
		enqueue (outq, outtmp);
		pthread_mutex_unlock (&outq_mutex);
	}
	return 0;
}

int main () {
	input_t intmp;
	output_t outtmp;
	caq_t inq;
	caq_t outq;

	pthread_mutex_init(&inq_mutex, NULL);
	pthread_mutex_init(&outq_mutex, NULL);

	/* TODO inq a few items */

	if (thserver (&inq, &outq, &intmp, &outtmp, cb) != 0) return EXIT_FAILURE;

	/* TODO deq a few items */

	return EXIT_SUCCESS;
}