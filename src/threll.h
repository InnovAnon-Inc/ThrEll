#ifndef _THRELL_H_
#define _THRELL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <sys/types.h>

#include <pthread.h>
#include <semaphore.h>

#include <caq.h>

/* exec_pipeline, exec_ring */

/* ---------- */

/* ring, command, pipeline */

typedef int (*thservercb) (void *, void *) ;

#define FD_RD (1)
#define FD_WR (2)
#define IS_FD_RD(F) ((F)->type & FD_RD)
#define IS_FD_WR(F) ((F)->type & FD_WR)

typedef struct {
	caq_t io;
	size_t nreader;
	size_t nwriter;
	pthread_mutex_t mutex;
	sem_t empty, full;
} pipe_t;

typedef struct {
	pipe_t *io;
	int type;
} fd_t;
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
typedef struct {
	thservercb cb;
	size_t input_esz;
	size_t output_esz;
	size_t input_n;
	size_t output_n;
} thserver_t;

int exec_pipeline (thserver_t *argvs, size_t nargv) ;

int threll_close (fd_t *fd) ;
int threll_pipe (fd_t *input, fd_t *output, size_t esz, size_t n) ;
void threll_cp (fd_t *dest, fd_t *src) ;

typedef struct {
	int (*cb) (fd_t *, fd_t *, fd_t *, bool, bool, void *);
	void *arg;
	pthread_t cpid;
	size_t input_esz;
	size_t input_n;
	size_t output_esz;
	size_t output_n;
} pipeline_t;

int pipeline (pipeline_t cmds[], size_t ncmd) ;

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

#ifdef __cplusplus
}
#endif

#endif /* _THRELL_H_ */
