#include "day.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "aux.h"

static const size_t digitnames_max = 9;
static const char *digitnames[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

static AUX_RESULT_T str_is_digit(const char *p, int *digit) {
	if (*p >= '0' && *p <= '9') {
		if (digit != NULL) {
			*digit = *p - '0';
		}
		return AUX_OK;
	}

	for (size_t i = 0; i < digitnames_max; ++i) {
		if (strncmp(digitnames[i], p, strlen(digitnames[i])) == 0) {
			if (digit != NULL) {
				*digit = (i + 1);
			}
			return AUX_OK;
		}
	}

	return AUX_ERROR;
}

AUX_RESULT_T get_day_result(void) {
	struct line_iter_t iter;
	line_iter_init(&iter, "res/day01.txt");

	int digit_sum = 0;
	while (line_iter_next(&iter)) {
		char digits[2] = {0};
		for (char *p = iter.line; *p != '\0'; ++p) {
			int digit = 0;
			if (str_is_digit(p, &digit)) {
				if (digits[0] == 0) {
					digits[0] = digit;
				}
				digits[1] = digit;
			}
		}

		digit_sum += (digits[0] * 10 + digits[1]);
		printf("digits={%d, %d}\n", digits[0], digits[1]);
	}
	line_iter_fini(&iter);

	printf("digit_sum: %d\n", digit_sum);

	return AUX_OK;
}

