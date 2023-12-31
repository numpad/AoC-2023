#include "aux.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>

static void reset_line(struct line_iter_t *);

//
// public api
//

void print_results(long result_a, long result_b) {
	printf("[* ] First Star : %ld\n[**] Second Star: %ld\n", result_a, result_b);
}

int aux_sum(int *arr, size_t n) {
	int sum = 0;
	for (size_t i = 0; i < n; ++i) {
		sum += arr[i];
	}
	return sum;
}

AUX_RESULT_T line_iter_init(struct line_iter_t *it, const char *filename) {
	assert(it != NULL);
	assert(filename != NULL);

	// init
	it->content = NULL;
	it->content_len = 0;
	it->current_line = NULL;
	it->line = NULL;
	it->flags = ITER_FLAGS_NONE;
	it->column_nr = -1;
	it->line_nr = -1;

	// try opening file
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		return AUX_ERROR;
	}

	// get size
	fseek(fp, 0, SEEK_END);
	it->content_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	it->content = malloc((it->content_len + 1) * sizeof(char));
	it->content[it->content_len] = '\0';
	it->current_line = it->content;

	// read into memory
	const size_t bytes_read = fread(it->content, sizeof(char), it->content_len, fp);
	assert(bytes_read == it->content_len);
	
	// cleanup
	fclose(fp);

	return AUX_OK;
}

AUX_RESULT_T line_iter_fini(struct line_iter_t *it) {
	assert(it != NULL);
	assert(it->content != NULL);
	assert(it->content_len > 0);

	// cleanup
	free(it->content);
	if (it->line != NULL) {
		free(it->line);
		it->line = NULL;
	}

	// reset state
	it->content = NULL;
	it->content_len = 0;
	it->current_line = NULL;
	it->column_nr = -1;
	it->line_nr = -1;

	return AUX_OK;
}

AUX_RESULT_T line_iter_next(struct line_iter_t *it) {
	assert(it != NULL);
	assert(it->content_len > 0);
	assert(it->current_line != NULL);

	reset_line(it);

	// end reached
	if (it->current_line >= it->content + it->content_len) {
		return AUX_ERROR;
	}

	// line ends with newline (or 0/eof)
	char *line_end = strchr(it->current_line, '\n');
	if (it->flags & ITER_FLAGS_CHARSTEP) {
		line_end = it->current_line;
	}
	ptrdiff_t len = (line_end - it->current_line);
	if (it->flags & ITER_FLAGS_CHARSTEP) {
		len = 1;
		if (it->current_line + len == it->content + it->content_len) {
			return AUX_ERROR;
		}
	}
	assert(it->current_line + len < it->content + it->content_len);
	
	// copy line, replace newline with \0
	it->line = malloc(len * sizeof(char) + 1); // TODO: realloc only when len increased. or just refactor into getter
	it->line[len] = '\0';
	memcpy(it->line, it->current_line, len);

	// increase line number
	if (it->flags & ITER_FLAGS_CHARSTEP) {
		++it->column_nr;
		it->line_nr += (it->line[0] == '\n' || it->current_line == it->content);
		if (it->line[0] == '\n') {
			it->column_nr = -1;
		}
	} else {
		it->column_nr = 0;
		++it->line_nr;
	}

	// go to next line
	it->current_line = line_end + 1;
	assert(it->current_line <= it->content + it->content_len);

	// FLAG: skip empty line/char
	if ((it->flags & ITER_FLAGS_OMITEMPTY)) {
		if (len == 0 || ((it->flags & ITER_FLAGS_CHARSTEP) && len == 1 && it->line[0] == '\n')) {
			return line_iter_next(it);
		}
	}

	assert(it->line != NULL); // line can always be dereferenced while iterating
	return AUX_OK;
}

//
// private
//

static void reset_line(struct line_iter_t *it) {
	// line may be NULL
	free(it->line);
	it->line = NULL;
}
