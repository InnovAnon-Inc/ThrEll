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

static int cb (void *_input, void *_output) {
	input_t *input = (input_t *) _input;
	output_t *output = (output_t *) _output;
	output->x = (double) (input->x);
	return 0;
}



int main () {
	input_t intmp;
	output_t outtmp;
	caq_t inq;
	caq_t outq;



	/* TODO inq a few items */

	if (thserver (&inq, &outq, &intmp, &outtmp, cb) != 0) return EXIT_FAILURE;

	/* TODO deq a few items */

	return EXIT_SUCCESS;
}