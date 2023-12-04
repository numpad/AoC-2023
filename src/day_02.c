#include "day.h"

#include <assert.h>
#include <string.h>
#include "aux.h"

typedef struct {
	int max_amount;
	const char *color;
} constraint_t;

static void scan_reveal(const char *line, int *count, char *color, char *term) {
	sscanf(line, "%d %31[a-z]%c", count, color, term);
}

AUX_RESULT_T get_day_result(void) {
	struct line_iter_t iter;
	line_iter_init(&iter, "res/day02.txt");

	int power_of_sets = 0;
	while (line_iter_next(&iter)) {
		if (iter.line[0] == '\0') {
			continue;
		}

		const char *remaining_line = iter.line;
		// scan game id
		int game_id = 0;
		const int scan_count = sscanf(remaining_line, "Game %d:", &game_id);
		if (scan_count == 0) {
			continue;
		}
		remaining_line = strchr(remaining_line, ':');
		assert(remaining_line != NULL);
		remaining_line += 1;

		printf("[%03d] %s\n", game_id, strchr(iter.line, ':'));
		assert(game_id > 0);

		constraint_t constraints[] = {
			{0, "red"},
			{0, "green"},
			{0, "blue"},
			{0, NULL},
		};

		// scan set
		int game_possible = 1;
		do {
			char term;
			int count;
			char color[8] = {0};
			scan_reveal(remaining_line, &count, color, &term);
			assert(term == ',' || term == ';');

			remaining_line = strchr(remaining_line, term);
			if (remaining_line == NULL) {
				term = ';';
			}
			if (remaining_line != NULL && (term == ',' || term == ';')) {
				remaining_line += 1;
			}

			for (constraint_t *c = constraints; c->color != NULL; ++c) {
				if (strcmp(c->color, color) == 0) {
					c->max_amount = AUX_MAX(c->max_amount, count);
				}
			}

			// printf("Got %dx %s (%c)\n", count, color, term);
		} while (remaining_line != NULL);


		int set_power = 1;
		for (constraint_t *c = constraints; c->color != NULL; ++c) {
			printf("%s: %dx\n", c->color, c->max_amount);
			set_power *= c->max_amount;
		}
		power_of_sets += set_power;
	}
	line_iter_fini(&iter);
	
	printf("Power of Sets: %d\n", power_of_sets);

	return AUX_OK;
}

