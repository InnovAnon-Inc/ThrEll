#ifndef _THRELL_H_
#define _THRELL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

#include <pthread.h>

#include <glitter.h>
#include <pipe.h>
#include <io.h>

/* exec_pipeline, exec_ring */

int ring (void)
__attribute__ ((const)) ;

/* ---------- */

/* ring, command, pipeline */

/*
typedef struct {
	int (*cb) (fd_t *input, fd_t *output, void *);
	void *arg;
	size_t input_esz;
	size_t input_n;
	size_t output_esz;
	size_t output_n;
} thclosure_t;
int exec_pipeline (thclosure_t *argvs, size_t nargv) ;
*/

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpadded"
typedef struct {
	worker_io_cb_t cb;
	size_t input_esz;
	size_t output_esz;
	size_t input_n;
	size_t output_n;
} thserver_t;
	#pragma GCC diagnostic pop

int exec_pipeline (
	thserver_t *restrict argvs, size_t nargv,
	pipe_t *restrict rd, pipe_t *restrict wr)
__attribute__ ((nonnull (1, 3, 4), nothrow, warn_unused_result)) ;

typedef __attribute__ ((warn_unused_result))
int (*pipeline_cb_t) (
	pipe_t *restrict,
	pipe_t *restrict,
	pipe_t *restrict,
	bool, bool,
	pipe_t *restrict,
	pipe_t *restrict,
	void *restrict);

typedef __attribute__ ((nonnull (1, 2, 4), /*nothrow,*/ warn_unused_result))
int (*threll_cb_t) (
   void *restrict dest,
   void const *restrict src, size_t srcsz, size_t *restrict destsz) ;

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wpadded"
typedef struct {
	threll_cb_t cb;
	void *restrict arg;
	pthread_t cpid;
	size_t input_esz;
	size_t input_n;
	size_t output_esz;
	size_t output_n;
} pipeline_t;
	#pragma GCC diagnostic pop

int threll (
   fd_t in, fd_t out,
   pipeline_t cmds[],
   size_t ncmd)
__attribute__ ((nonnull (3), nothrow, warn_unused_result)) ;


int pipeline (
	pipeline_t cmds[], size_t ncmd,
	pipe_t *restrict rd, pipe_t *restrict wr)
__attribute__ ((nonnull (1, 3, 4), nothrow, warn_unused_result)) ;

int thserver (
	pipe_t *restrict inq, pipe_t *restrict outq,
	worker_io_cb_t cb)
__attribute__ ((nonnull (1, 2, 3), nothrow, warn_unused_result)) ;

/* ---------- */

/* fork a process
 * child invokes cb (args)
 * parent does nothing, creating a zombie */
/*int zombify (int (*cb) (void *), void *args) ;*/

/* fork a process, then fork a process
 * grandchild invokes cb (args)
 * child does nothing, but dies, so grandchild is adopted by the init orphanage
 * parent waits for child to die */
/*int background (int (*cb) (void *), void *args) ;*/

/* ---------- */

/* fork a process
 * child invokes cb (args)
 * parent waits for child */
/*int fork_and_wait (int (*cb) (void *), void *cb_args) ;*/

/* fork a process
 * child invokes childcb (childargs)
 * parent invokes parencb (parenargs), then waits for child */
/*int fork_and_wait2 (
	int (*childcb)  (void *), void *childcb_args,
	int (*parentcb) (void *), void *parentcb_args) ;*/

/* ---------- */

int ezthork (
	void *(*childcb)  (void *),        void *childcb_args,
	int (*parentcb) (pthread_t, void *), void *parentcb_args) ;

#ifdef __cplusplus
}
#endif

#endif /* _THRELL_H_ */
