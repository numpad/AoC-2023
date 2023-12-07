#include "day.h"
#include "aux.h"

#include <string.h>

#define MAX_NUMBERS_WINNING 128
#define MAX_NUMBERS_MY      MAX_NUMBERS_WINNING

AUX_RESULT_T get_day_result(void) {
	struct line_iter_t iter;
	line_iter_init(&iter, "res/day03.txt");
	iter.flags = ITER_FLAGS_OMITEMPTY;

	int elf_points = 0;
	while (line_iter_next(&iter)) {
		// Card ID
		int card_id;
		sscanf(iter.line, "Card %d:", &card_id);

		// Read Numbers into respective array
		int winning_numbers[MAX_NUMBERS_WINNING];
		int winning_numbers_len = 0;
		int my_numbers[MAX_NUMBERS_MY];
		int my_numbers_len = 0;
		const char *numbers_begin = strchr(iter.line, ':') + 1;
		const char *my_numbers_begin = strchr(numbers_begin, '|');

		int count = 0;
		int chars_read;
		for (int i = 0; i < strlen(numbers_begin); i += chars_read) {
			const char *number_begin = &numbers_begin[i];

			int number;
			sscanf(number_begin, "%d%n", &number, &chars_read);
			if (number_begin < my_numbers_begin && number_begin + chars_read > my_numbers_begin) {
				continue;
			}

			if (number_begin > my_numbers_begin) {
				my_numbers[my_numbers_len++] = number;
			} else {
				winning_numbers[winning_numbers_len++] = number;
			}
		}

		// check for wins
		int card_points = 0;
		for (int i = 0; i < winning_numbers_len; ++i) {
			const int winning_number = winning_numbers[i];

			for (int j = 0; j < my_numbers_len; ++j) {
				const int my_number = my_numbers[j];

				if (winning_number == my_number) {
					card_points += (card_points == 0 ? 1 : card_points);
				}
			}
		}
		elf_points += card_points;
	}

	printf("Elf has %d points!\n", elf_points);
	
	line_iter_fini(&iter);
	return AUX_OK;
}

