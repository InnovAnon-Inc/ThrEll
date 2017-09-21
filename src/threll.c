#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdbool.h>

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

#include <threll.h>






















	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpadded"
typedef struct {
   io_t *restrict io;
   fd_t in, out;
} io_thread_cb_t;
	#pragma GCC diagnostic pop

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static void *io_thread_cb (void *restrict _arg) {
   io_thread_cb_t *restrict arg = (io_thread_cb_t *restrict) _arg;
   error_check (rw_io (arg->io, arg->in, arg->out) != 0) return NULL;
   return NULL;
}

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpadded"
typedef struct {
   io_t *restrict io;
   threll_cb_t cb;
} worker_thread_cb_t;
	#pragma GCC diagnostic pop

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int worker_thread_cb_cb (
   buffer_t *restrict buf_out,
   buffer_t const *restrict buf_in,
   void *restrict _arg) {
   threll_cb_t *restrict arg = (threll_cb_t *restrict) _arg;

   TODO (init buf_out->n to out_bufsz below)
   error_check ((*arg) (buf_out->buf, buf_in->buf,
      buf_in->n, &(buf_out->n)) != 0)
      return -1;
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static void *worker_thread_cb (void *restrict _arg) {
   /*io_t *restrict arg = (io_t *restrict) _arg;*/
   worker_thread_cb_t *restrict arg =
      (worker_thread_cb_t *restrict) _arg;
   error_check (worker_io (arg->io, worker_thread_cb_cb, &(arg->cb)) != 0)
      return NULL;
   return NULL;
}

__attribute__ ((nonnull (3), nothrow, warn_unused_result))
int threll (
   fd_t in, fd_t out,
   pipeline_t cmds[],
   size_t ncmd) {
   io_t dest/*, src*/;
   io_thread_cb_t io_thread_cb_arg;
   worker_thread_cb_t *restrict worker_thread_cb_arg;
   pthread_t io_thread;
   pthread_t *restrict worker_thread;
   buffer_t *restrict buf_in;
   buffer_t *restrict buf_out;
   size_t in_bufsz = cmds[0].input_esz;
   size_t in_nbuf  = cmds[0].input_n;
   size_t out_bufsz = cmds[ncmd - 1].output_esz;
   size_t out_nbuf  = cmds[ncmd - 1].output_n;
   io_t *restrict mid;
   pipe_t *restrict pipes;
   size_t i;

   error_check (alloc_io (&dest, /*&src,*/
      in_bufsz, in_nbuf, out_bufsz, out_nbuf) != 0) return -1;

	mid = malloc (ncmd * sizeof (io_t));
	error_check (mid == NULL) {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
      (void) free_io (&dest);
	#pragma GCC diagnostic pop
      return -2;
	}
	pipes = malloc ((ncmd - 1) * sizeof (pipe_t));
	error_check (pipes == NULL) {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
      (void) free_io (&dest);
	#pragma GCC diagnostic pop
      return -2;
	}

   io_thread_cb_arg.io = &dest;
   io_thread_cb_arg.in  = in;
   io_thread_cb_arg.out = out;
   error_check (pthread_create (&io_thread, NULL, io_thread_cb, &io_thread_cb_arg) != 0) {
	   free (mid);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
      (void) free_io (&dest);
	#pragma GCC diagnostic pop
      return -2;
   }


	init_io (mid + 0, dest.in, pipes + 0);
	for (i = 1; i != ncmd - 1; i++)
		init_io (mid + i, pipes + i - 1, pipes + i);
	init_io (mid + i, pipes + i - 1, dest.out);

	for (i = 0; i != ncmd - 1; i++)
		error_check (alloc_pipe (pipes + i, cmds[i].output_esz, cmds[i].output_n) != 0) {
			return -3;
		}
		/*alloc_pipe (pipes + i, cmds[i + 1].input_esz, cmds[i + 1].input_n);*/

	worker_thread = malloc (ncmd * sizeof (pthread_t));
	error_check (worker_thread == NULL) {
		return -3;
	}
	worker_thread_cb_arg = malloc (ncmd * sizeof (worker_thread_cb_t));
	error_check (worker_thread_cb_arg == NULL) {
		return -4;
	}
	for (i = 0; i != ncmd; i++) {
		worker_thread_cb_arg[i].io = mid + i;
		worker_thread_cb_arg[i].cb = cmds[i].cb;
		error_check (pthread_create (worker_thread + i, NULL, worker_thread_cb, /*&src*/ /*&dest*/ worker_thread_cb_arg + i) != 0) {
			TODO (kill io thread)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
			(void) free_io (&dest);
	#pragma GCC diagnostic pop
			return -3;
		}
   }

   error_check (pthread_join (io_thread, NULL) != 0) {
      TODO (kill worker thread)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
      (void) free_io (&dest);
	#pragma GCC diagnostic pop
      return -4;
   }

   /* join all workers */
   for (i = 0; i != ncmd; i++)
		error_check (pthread_join (worker_thread[i], NULL) != 0) {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-result"
		(void) free_io (&dest);
	#pragma GCC diagnostic pop
		return -5;
		}

	free (mid);
	free (pipes);
	free (worker_thread);
	free (worker_thread_cb_arg);
   error_check (free_io (&dest/*, &src*/) != 0) return -6;

   return 0;
}
