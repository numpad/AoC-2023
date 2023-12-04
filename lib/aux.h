#ifndef AUX_H
#define AUX_H

#include <stdio.h>

#define AUX_RESULT_T int
#define AUX_OK    1
#define AUX_ERROR 0



//
// generic utils
//

#define AUX_MAX(a, b) ((a) > (b) ? (a) : (b))
#define AUX_MIN(a, b) ((a) < (b) ? (a) : (b))

void print_results(long result_a, long result_b);
int aux_sum(int *arr, size_t n);

//
// iterator
//

struct line_iter_t {
	// stays
	char *content;
	size_t content_len;

	// changes each iteration
	char *current_line; // ptr to current line start
	char *line; // copy of current_line, newline replaced with null terminator
};


// initialize & cleanup
AUX_RESULT_T line_iter_init(struct line_iter_t *, const char *filename);
AUX_RESULT_T line_iter_fini(struct line_iter_t *);

// iterate
AUX_RESULT_T line_iter_next(struct line_iter_t *);

#endif

