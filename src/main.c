#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

#include <threll.h>

typedef struct {
	int x;
} input_t;

typedef struct {
	double x;
} output_t;

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int cb (
	buffer_t *restrict buf_out,
	buffer_t const *restrict buf_in,
	void *restrict unused) {
	/*output->x = (double) (input->x);*/
	buf_out->n = min (buf_in->n, buf_out->n);
	memcpy (buf_out, buf_in, buf_out->n);
	return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int rdcb (
	buffer_t *restrict buf_out,
	buffer_t const *restrict buf_in,
	void *restrict arg) {
	TODO (?)
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int wrcb (
   buffer_t *restrict buf_out,
   buffer_t const *restrict buf_in,
   void *restrict arg) {
	TODO (?)
}

int main (void) {
	size_t nargv = 3;
	thserver_t argvs[3];
	argvs[0].cb = rdcb;
	argvs[0].input_esz = 0;
	argvs[0].input_n   = 0;
	argvs[0].output_esz = sizeof (int);
	argvs[0].output_n   = 2;

	argvs[1].cb = cb;
	argvs[1].input_esz = argvs[0].output_esz;
	argvs[1].input_n   = argvs[0].output_n;
	argvs[1].output_esz = sizeof (double);
	argvs[1].output_n   = 2;

	argvs[2].cb = wrcb;
	argvs[2].input_esz = argvs[1].output_esz;
	argvs[2].input_n   = argvs[1].input_n;
	argvs[2].output_esz = 0;
	argvs[2].output_n   = 0;

	if (exec_pipeline (argvs, nargv) != 0) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}