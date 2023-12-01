#include "day.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "aux.h"

AUX_RESULT_T get_day_result(void) {
	struct line_iter_t iter;
	line_iter_init(&iter, "res/day01.txt");

	int digit_sum = 0;
	while (line_iter_next(&iter)) {
		char digits[2] = {0};
		for (char *p = iter.line; *p != '\0'; ++p) {
			if (*p >= '0' && *p <= '9') {
				if (digits[0] == 0) {
					digits[0] = *p - '0';
				}
				digits[1] = *p - '0';
			}
		}

		digit_sum += (digits[0] * 10 + digits[1]);
		printf("digits={%d, %d}\n", digits[0], digits[1]);
	}
	line_iter_fini(&iter);

	printf("digit_sum: %d\n", digit_sum);

	return AUX_OK;
}

