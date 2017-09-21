#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdbool.h>

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

/*#include <ezfork.h>*/

#include <threll.h>

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

typedef struct {
	pipe_t *restrict input;
	pipe_t *restrict wr;
	pipe_t *restrict rd;
	bool last;
	pthread_t cpid;
} parentcb2_t;


__attribute__ ((nonnull (2), nothrow, warn_unused_result))
static int parentcb (pthread_t cpid, void *restrict cbargs) {
	parentcb2_t *restrict args = (parentcb2_t *restrict ) cbargs;
	pipe_t *restrict input = args->input;
	pipe_t *restrict wr = args->wr;
	bool last = args->last;
	pipe_t *restrict rd = args->rd;
	args->cpid = cpid;

	/* TODO
	threll_close (input);
	threll_close (wr);
	if (last) threll_close (rd);*/
	return 0;
}




typedef struct {
	bool first, last;
	pipe_t *restrict input;
	pipe_t *restrict rd;
	pipe_t *restrict wr;
	pipeline_t *restrict cmd;
} childcommon_t;

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static void *childcommon (void *restrict tmp) {
	childcommon_t *restrict arg = (childcommon_t *restrict ) tmp;
	bool first = arg->first;
	bool last = arg->last;
	pipe_t *restrict input = arg->input;
	pipe_t *restrict rd = arg->rd;
	pipe_t *restrict wr = arg->wr;
	pipeline_t *restrict cmd = arg->cmd;
	pipeline_cb_t cb;
	void *restrict cbarg;
	int err;
	cb = cmd->cb;
	cbarg = cmd->arg;
	err = cb (input, rd, wr, first, last, cbarg);
	error_check (err != 0) {
		/*return -1;*/
		return NULL;
	}
	/*if (rd != &stdinput)*/
	error_check (rd != NULL)
		threll_close (rd);
	/*if (wr != &stdoutput)*/
	error_check (wr != NULL)
		threll_close (wr);
	/*return 0;*/
	return NULL;
}

__attribute__ ((nonnull (1, 3), warn_unused_result))
int ezthork (
	void *(*childcb)  (void *),        void *childcb_args,
	int (*_parentcb) (pthread_t, void *), void *parentcb_args) {
	pthread_t pid;

	if (pthread_create (&pid, NULL, childcb, childcb_args) != 0) return -1;
	if (_parentcb (pid, parentcb_args) != 0)
		   return -3;
	return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int command (
	pipeline_t *restrict cmd,
	pipe_t *restrict *restrict input,
	bool first, bool last) {
	childcommon_t *restrict cargs;
	parentcb2_t pargs;

	pipe_t *restrict pipettes = malloc (2 * sizeof (pipe_t));
	error_check (pipettes == NULL) {
		return -1;
	}
	cargs = malloc (sizeof (childcommon_t));
	error_check (cargs == NULL) {
		free (pipettes);
		return -2;
	}

	/* TODO a buffer array queue that will allow for
	 * input_{esz,n} != output_{esz,n} */
	/*if (cmd->input_esz != cmd->output_esz)
		return -2;
	if (cmd->input_n != cmd->output_n)
		return -3;
	threll_pipe (pipettes + 0, pipettes + 1, cmd->input_esz, cmd->input_n);*/
	TODO (threll_pipe() is deprecated)
	error_check (threll_pipe (pipettes + 0, pipettes + 1, cmd->output_esz, cmd->output_n) != 0) {
		free (cargs);
		free (pipettes);
		return -3;
	}
	/*(void) pipe (pipettes);*/

	cargs->first = first;
	cargs->last = last;
	cargs->input = *input;
	/*cargs.rd = pipettes[0];
	cargs.wr = pipettes[1];*/
	cargs->rd = pipettes + 0;
	cargs->wr = pipettes + 1;
	cargs->cmd = cmd;

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
	/* *input = pargs.rd;*/

	error_check (ezthork (childcommon, cargs, parentcb, &pargs) != 0) {
		free (cargs);
		free (pipettes);
		return -4;
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
__attribute__ ((nonnull (1), warn_unused_result))
int pipeline (pipeline_t cmds[], size_t ncmd) {
	/*caq_t *input = STDIN_FILENO;*/
	/*fd_t *input = &stdinput;*/
	pipe_t *restrict input = NULL;
	bool first = true;
	size_t i;

	for (i = 0; i != ncmd - 1; i++) {
		error_check (command (cmds + i, &input, first, false) != 0)
			return -1;
		first = false;
	}
	error_check (command (cmds + i, &input, first, true) != 0)
		return -2;
	for (i = 0; i != ncmd; i++) {
		pthread_t cpid = cmds[i].cpid;
		error_check (pthread_join (cpid, NULL) != 0)
			return -3;
		/* free queue */
		/*
		if (cmds[i].stdin != io)
			free (cmds[i].stdin);
		*/
	}
	return 0;
}

__attribute__ ((const))
int ring (void) {
	return -2;
}

/* zombify (): fork and don't wait */
/* bg (): fork_and_wait (zombify (cb)) */
/* pipeline (): fork_and_wait (fork_and_wait (fork_and_wait (..., pcb), pcb), pcb) */
/* ring (): */
/* web (): */



typedef struct {
	/*thclosure_t *argv;*/
	worker_io_cb_t argv;
} exec_pipelinecb_t;

__attribute__ ((nothrow, warn_unused_result))
static int exec_pipelinecb (
	pipe_t *restrict input,
	pipe_t *restrict rd,
	pipe_t *restrict wr,
	bool first, bool last, void *cbargs) {
	exec_pipelinecb_t *restrict args =
		(exec_pipelinecb_t *restrict) cbargs;
	worker_io_cb_t argv = args->argv;
	io_t io;

	pipe_t *restrict cmdinput = NULL;
	pipe_t *restrict cmdoutput = NULL;

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

	io.in  = cmdinput;
	io.out = cmdoutput;
	/*execvp (argv[0], argv);*/
	error_check (worker_io (&io, argv, NULL) != 0)
		return -1;
	/*return -1;*/
	/*return closure->cb (closure->arg);*/
	return 0;
}

__attribute__ ((nonnull (1), warn_unused_result))
int exec_pipeline (thserver_t argvs[], size_t nargv) {
	pipeline_t *restrict cmds = malloc (nargv * sizeof (pipeline_t)
	+ nargv * sizeof (exec_pipelinecb_t));
	exec_pipelinecb_t *restrict tmps;
	size_t i;
	error_check (cmds == NULL) return -1;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic error "-Wstrict-aliasing"
	tmps = (exec_pipelinecb_t *restrict) (cmds + nargv);
	#pragma GCC diagnostic pop
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
	error_check (pipeline (cmds, nargv) != 0) {
		/*puts ("exec_pipeline failed");*/
		return -2;
	}
	/*puts ("exec_pipeline success");*/
	free (cmds);
	return 0;
}
