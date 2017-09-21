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
	buf_out->n = min (buf_in->n, buf_out->n);
	memcpy (buf_out, buf_in, buf_out->n);
	return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int wrcb (
   buffer_t *restrict buf_out,
   buffer_t const *restrict buf_in,
   void *restrict arg) {
	TODO (?)
	buf_out->n = min (buf_in->n, buf_out->n);
	memcpy (buf_out, buf_in, buf_out->n);
	return 0;
}

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpadded"
typedef struct {
	io_t *restrict io;
	fd_t rd;
	fd_t wr;
} rw_io_t;
	#pragma GCC diagnostic pop

void *rw_io_wrapper (void *restrict _arg) {
	rw_io_t *restrict arg = (rw_io_t *restrict) _arg;
	error_check (rw_io (arg->io, arg->rd, arg->wr) != 0)
		return NULL;
	return NULL;
}

int main (void) {
	size_t nargv = 3;
	thserver_t argvs[3];
	size_t in_bufsz = 10;
	size_t in_nbuf  = 3;
	size_t out_bufsz = 8;
	size_t out_nbuf  = 3;
	pthread_t io_thread;
	rw_io_t rw_io_arg;

	io_t io;
	error_check (alloc_io (&io, in_bufsz, in_nbuf, out_bufsz, out_nbuf) != 0)
		return EXIT_FAILURE;

	argvs[0].cb = rdcb;
	argvs[0].input_esz = in_bufsz;
	argvs[0].input_n   = in_nbuf;
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
	argvs[2].output_esz = out_bufsz;
	argvs[2].output_n   = out_nbuf;

	rw_io (&io, STDIN_FILENO, STDOUT_FILENO);
	error_check (pthread_create (
		&io_thread, NULL, rw_io_wrapper, &rw_io_arg) != 0)
		return EXIT_FAILURE;

	error_check (exec_pipeline (argvs, nargv, io.in, io.out) != 0) return EXIT_FAILURE;

	error_check (pthread_join (io_thread, NULL) != 0) return EXIT_FAILURE;

	error_check (free_io (&io) != 0) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}