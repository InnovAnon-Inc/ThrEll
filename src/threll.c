#include <stdbool.h>

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

/*#include <ezfork.h>*/

#include "threll.h"

/*
pipe_t stdpipe;
fd_t stdinput;
fd_t stdoutput;
*/


/*
static int ezfork_parentcb_wait (pid_t cpid, void *unused) {
	pid_t wpid;
	int status;

	do wpid = waitpid (cpid, &status, WUNTRACED);
	while (! WIFEXITED (status) && ! WIFSIGNALED (status));
	if (status == -1) return -1;
	return 0;
}

int fork_and_wait (int (*cb) (void *), void *cb_args) {
	return ezfork (cb, cb_args, ezfork_parentcb_wait, NULL);
}
*/
/*typedef thclosure_t parentcb_t;*/

/*
static int ezfork_parentcb_wait2 (pid_t cpid, void *args) {
	pid_t wpid;
	int status;
	parentcb_t *cb = (parentcb_t *) args;

	if (cb->cb (cb->arg) != 0) {
		/ * TODO cleanup child * /
	   return -2;
   }

	do wpid = waitpid (cpid, &status, WUNTRACED);
	while (! WIFEXITED (status) && ! WIFSIGNALED (status));
	if (status == -1) {
		return -1;
	}
	return 0;
}

int fork_and_wait2 (
	int (*childcb)  (void *), void *childcb_args,
	int (*parentcb) (void *), void *parentcb_args) {
	parentcb_t cb;

	cb.cb = parentcb;
	cb.arg = parentcb_args;
	if (ezfork (
		childcb, childcb_args,
		ezfork_parentcb_wait2, (void *) &cb) != 0) {
			return -1;
		}
	return 0;
}
*/
/*
static int do_nothing (pid_t cpid, void *unused) {
	return 0;
}

int zombify (int (*childcb)  (void *), void *childcb_args) {
	if (ezfork (childcb, childcb_args, do_nothing, NULL) != 0) {
		return -1;
	}
	return 0;
}

static int zombify_wrapper (void *arg) {
	closure_t *closure = (closure_t *) arg;

	if (zombify (closure->cb, closure->arg) != 0) {
		return -1;
	}
	return 0;
}
*/

/*typedef thclosure_t background_t;*/

/* typedef apply_closure_t; ==> closure with a closure in it */

/*
static int backgroundcb (void *args) {
	background_t *tmp = (background_t *) args;
	return zombify (tmp->cb, tmp->args);
}
*/
/* TODO pthread_detach () */
/*
int background (int (*cb) (void *), void *args) {
	closure_t tmp;
	tmp.cb = cb;
	tmp.arg = args;

	if (fork_and_wait (zombify_wrapper, &tmp) != 0) {
		return -1;
	}
	return 0;
}
*/

/* ------------------------------------------ */

int threll_close (fd_t *fd) {
	pipe_t *pipe;
	if (fd == NULL) return 0;
	pipe = fd->io;
	if (IS_FD_RD (fd) && pipe->nreader == 0)
		return -1;
	if (IS_FD_WR (fd) && pipe->nwriter == 0)
		return -2;
	if (IS_FD_RD (fd))
		pipe->nreader--;
	if (IS_FD_WR (fd))
		pipe->nwriter--;
	if (pipe->nreader != 0) {
		free (fd);
		return 0;
	}
	if (pipe->nwriter != 0)
		return -3;

	free_queue (&(pipe->io));
	free (pipe);
	free (fd);
	return 0;
}
int threll_pipe (fd_t *input, fd_t *output, size_t esz, size_t n) {
	pipe_t *pipe = malloc (sizeof (pipe_t));
	if (pipe == NULL) return -1;
	pipe->nreader = 1;
	pipe->nwriter = 1;
	/* TODO init pipe->io */
	alloc_queue (&(pipe->io), esz, n);
	input->io = pipe;
	input->type = FD_RD;
	output->io = pipe;
	output->type = FD_WR;
	pthread_mutex_init(&(pipe->mutex), NULL);
	return 0;
}
/*
void threll_cp (fd_t *dest, fd_t *src) {
	dest->io = src->io;
	dest->type = src->type;
	if (IS_FD_RD (dest->type))
		dest->io->nreader++;
	if (IS_FD_WR (dest->type))
		dest->io->nwriter++;
}
*/
typedef struct {
	fd_t *input;
	fd_t *wr;
	fd_t *rd;
	bool last;
	pthread_t cpid;
} parentcb2_t;

static int parentcb (pthread_t cpid, void *cbargs) {
	parentcb2_t *args = (parentcb2_t *) cbargs;
	fd_t *input = args->input;
	fd_t *wr = args->wr;
	bool last = args->last;
	fd_t *rd = args->rd;
	args->cpid = cpid;

	threll_close (input);
	threll_close (wr);
	if (last) threll_close (rd);
	return 0;
}


/* client code */
/*
typedef struct {
	bool first, last;
	caq_t *input, *rd, *wr;
	void *args;
} command_t;
*/



typedef struct {
	bool first, last;
	fd_t *input, *rd, *wr;
	pipeline_t *cmd;
} childcommon_t;

static void *childcommon (void *tmp) {
	childcommon_t *arg = (childcommon_t *) tmp;
	bool first = arg->first;
	bool last = arg->last;
	fd_t *input = arg->input;
	fd_t *rd = arg->rd;
	fd_t *wr = arg->wr;
	pipeline_t *cmd = arg->cmd;
	if (cmd->cb (input, rd, wr, first, last, cmd->arg) != 0) {
		/*return -1;*/
		return NULL;
	}
	/*if (rd != &stdinput)*/
	if (rd != NULL)
		threll_close (rd);
	/*if (wr != &stdoutput)*/
	if (wr != NULL)
		threll_close (wr);
	/*return 0;*/
	return NULL;
}
/*
typedef struct {
	int rd; / * ret val * /
	caq_t *input;
	bool first;
	bool last;
	closure_t args;
} command_t;*/

int ezthork (
	void *(*childcb)  (void *),        void *childcb_args,
	int (*parentcb) (pthread_t, void *), void *parentcb_args) {
	pthread_t pid;

	pthread_create (&pid, NULL, childcb, childcb_args);
	if (parentcb (pid, parentcb_args) != 0)
		   return -3;
	return 0;
}

static int command (pipeline_t *cmd, fd_t **input, bool first, bool last) {
	childcommon_t cargs;
	parentcb2_t pargs;

	fd_t *pipettes = malloc (2 * sizeof (fd_t));
	if (pipettes == NULL) {
		return -1;
	}

	/* TODO a buffer array queue that will allow for
	 * input_{esz,n} != output_{esz,n} */
	/*if (cmd->input_esz != cmd->output_esz)
		return -2;
	if (cmd->input_n != cmd->output_n)
		return -3;
	threll_pipe (pipettes + 0, pipettes + 1, cmd->input_esz, cmd->input_n);*/
	threll_pipe (pipettes + 0, pipettes + 1, cmd->output_esz, cmd->output_n);
	/*(void) pipe (pipettes);*/

	cargs.first = first;
	cargs.last = last;
	cargs.input = *input;
	/*cargs.rd = pipettes[0];
	cargs.wr = pipettes[1];*/
	cargs.rd = pipettes + 0;
	cargs.wr = pipettes + 1;
	cargs.cmd = cmd;

	pargs.input = *input;
	/*pargs.wr = pipettes[1];
	pargs.rd = pipettes[0];*/
	pargs.wr = pipettes + 1;
	pargs.rd = pipettes + 0;
	/*threll_cp (pargs.wr, pipettes + 1);
	threll_cp (pargs.rd, pipettes + 0);*/
	pargs.last = last;

	/* *input = pipettes[0];*/
	*input = pipettes + 0;
	/*threll_cp (*input, pipetts + 0);*/

	if (ezthork (childcommon, &cargs, parentcb, &pargs) != 0) {
		return -1;
	}
	cmd->cpid = pargs.cpid;
	/*
	if (fork_and_wait2 (
		childcommon, &cargs,
		parentcb, &pargs) != 0) {
			puts ("command failed");
			return -1;
	}
	*/

	return 0;
}

/* TODO add void * param to cmds' siggy, and void * arg... closure-style */

/* nargv is non-zero */
int pipeline (pipeline_t cmds[], size_t ncmd) {
	/*caq_t *input = STDIN_FILENO;*/
	/*fd_t *input = &stdinput;*/
	fd_t *input = NULL;
	bool first = true;
	size_t i;

	for (i = 0; i != ncmd - 1; i++) {
		if (command (cmds + i, &input, first, false) != 0)
			return -1;
		first = false;
	}
	if (command (cmds + i, &input, first, true) != 0)
		return -2;
	for (i = 0; i != ncmd; i++) {
		pthread_t cpid = cmds[i].cpid;
		if (pthread_join (cpid, NULL) != 0)
			return -1;
		/* free queue */
		/*
		if (cmds[i].stdin != io)
			free (cmds[i].stdin);
		*/
	}
	return 0;
}

int ring () {
	return -2;
}

/* zombify (): fork and don't wait */
/* bg (): fork_and_wait (zombify (cb)) */
/* pipeline (): fork_and_wait (fork_and_wait (fork_and_wait (..., pcb), pcb), pcb) */
/* ring (): */
/* web (): */





int thserver (
	fd_t *inq, fd_t *outq,
	thservercb cb) {
	/* while inq is open */
	while (true) { /* while ! isempty (inq) ? ... + mutex */
		/*
		pthread_mutex_lock (&(inq->io->mutex));
		memcpy (intmp, dequeue (inq), inq->esz);
		pthread_mutex_unlock (&(inq->io->mutex));

		if (cb (intmp, outtmp) != 0) return -1;

		pthread_mutex_lock (&(outq->io->mutex));
		enqueue (outq, outtmp);
		pthread_mutex_unlock (&(outq->io->mutex));
		*/
		void *intmp;
		void *outtmp;
		/* if can't dequeue, then block til ready */
		if (inq != NULL) pthread_mutex_lock (&(inq->io->mutex));
		if (outq != NULL) pthread_mutex_lock (&(outq->io->mutex));
		if (inq != NULL) intmp  = dequeue (&(inq->io->io));
		else intmp = NULL;
		if (outq != NULL) outtmp = enqueue (&(outq->io->io));
		else outtmp = NULL;
		if (cb (intmp, outtmp) != 0) return -1;
		if (outq != NULL) pthread_mutex_unlock (&(outq->io->mutex));
		if (inq != NULL) pthread_mutex_unlock (&(inq->io->mutex));
	}
	return 0;
}

typedef struct {
	/*thclosure_t *argv;*/
	thservercb argv;
} exec_pipelinecb_t;

static int exec_pipelinecb (fd_t *input, fd_t *rd, fd_t *wr,
	bool first, bool last, void *cbargs) {
	exec_pipelinecb_t *args = (exec_pipelinecb_t *) cbargs;
	thservercb argv = args->argv;

	fd_t *cmdinput = NULL;
	fd_t *cmdoutput = NULL;

	/*cb ();*/
	/*if (first && ! last && input == STDIN_FILENO)*/ /* first command */
	if (first && ! last && input == NULL)
		/*dup2 (wr, STDOUT_FILENO);*/
		cmdoutput = wr;
	/*else if (! first && ! last && input != STDIN_FILENO) {*/ /* middle command */
	else if (! first && ! last && input != NULL) {
		/*dup2 (input, STDIN_FILENO);
		dup2 (wr, STDOUT_FILENO);*/
		cmdinput = input;
		cmdoutput = wr;
	} else /* last command */
		/*dup2 (input, STDIN_FILENO);*/
		cmdinput = input;

	/*execvp (argv[0], argv);*/
	thserver (cmdinput, cmdoutput, argv);
	return -1;
	/*return closure->cb (closure->arg);*/
}



int exec_pipeline (thserver_t *argvs, size_t nargv) {
	pipeline_t *cmds = malloc (nargv * sizeof (pipeline_t)
	+ nargv * sizeof (exec_pipelinecb_t));
	exec_pipelinecb_t *tmps = (exec_pipelinecb_t *) (cmds + nargv);
	size_t i;
	for (i = 0; i != nargv; i++) {
		cmds[i].cb = exec_pipelinecb;
		cmds[i].arg = tmps + i;
		tmps[i].argv = argvs[i].cb;

		cmds[i].input_esz = argvs[i].input_esz;
		cmds[i].input_n = argvs[i].input_n;
		cmds[i].output_esz = argvs[i].output_esz;
		cmds[i].output_n = argvs[i].output_n;
	}
	/*puts ("exec_pipeline ()");*/
	if (pipeline (cmds, nargv) != 0) {
		/*puts ("exec_pipeline failed");*/
		return -1;
	}
	/*puts ("exec_pipeline success");*/
	return 0;
}











/*
typedef int (*cmd_t) (void *, void *);
typedef struct {
	cmd_t cmd;
	size_t insz, outsz;
} cmd_meta_t;
typedef struct {
	cmd_t *cmds;
	size_t ncmd;
	caq_t *caqs;
} pipeline_t;

void setup_two_threads () {

}
*/

/*
?Asz ABsz BCsz CDsz D?sz ?
? -> A -> B -> C -> D -> ?
NULL ABq  BCq  CDq  NULL
 */
