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

static int cb (void *_input, void *_output) {
	input_t *input = (input_t *) _input;
	output_t *output = (output_t *) _output;
	if (input == NULL) {
		puts ("unexpected NULL input");
		return -1;
	}
	if (output == NULL) {
		puts ("unexpected NULL output");
		return -2;
	}
	output->x = (double) (input->x);
	return 0;
}

static int rdcb (void *unused, void *_output) {
	/*int *output = (int *) _output;*/
	int *output = &(((input_t *) _output)->x);
	ssize_t rd = read (STDIN_FILENO, output, sizeof (int));
	if (rd != sizeof (int)) return -1;
	/*printf ("read:%d\n", *output);*/
	return 0;
}

static int wrcb (void *_input, void *unused) {
	/*double *input = (double *) _input;*/
	double *input = &(((output_t *) _input)->x);
	ssize_t wr = write (STDOUT_FILENO, input, sizeof (double));
	if (wr != sizeof (double)) return -2;
	/*fflush (stdout);
	printf ("write:%g\n", *input);*/
	return 0;
}

int main () {
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
	argvs[1].output_n   = 3;

	argvs[2].cb = wrcb;
	argvs[2].input_esz = argvs[1].output_esz;
	argvs[2].input_n   = argvs[1].input_n;
	argvs[2].output_esz = 0;
	argvs[2].output_n   = 0;

	if (exec_pipeline (argvs, nargv) != 0) return EXIT_FAILURE;

	return EXIT_SUCCESS;
}