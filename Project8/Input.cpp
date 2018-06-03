#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Parse.h"


TokenType next_token_type = INVALID;
int32_t token_number_value = 0;

FILE* input_source = stdin;

static const uint32_t buff_size = 4096;
static char buff[buff_size];
static uint32_t cursor = 0;
static const uint32_t token_max_size = 128;
static char token[token_max_size];

void set_input(const char* file_name) {
	FILE* p = fopen(file_name, "r");
	if (!p) {
		fprintf(stderr, "sorry, I coulnd't open the file %s for reading\n", file_name);
		return;
	}
	input_source = p;
	cursor = 0;
	buff[0] = 0;
}

void skip_line(void) {
	cursor = 0;
	buff[0] = 0;
}


static bool IS_NUMBER(char c) {
	return c >= '0' && c <='9';
}

static bool IS_LETTER(char c) {
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static bool IS_SPACE(char c) {
	return c == ' ' || c == '\t' || c == '\n';
}

static bool token_has_been_peeked = false;

void read_next_token(void) {
	if (token_has_been_peeked) {
		token_has_been_peeked = false;
		return;
	}

	uint32_t k = 0;
	do {
		if (buff[cursor] == 0) {
			cursor = 0;
			const char* res = fgets(buff, buff_size, input_source);
			if (res == 0) {
				*token = 0;
				next_token_type = END;
				return;
			}
		}
		bool is_quoted_string = false;
		while (buff[cursor] != 0) {
			if (IS_SPACE(buff[cursor]) && !is_quoted_string) {
				cursor += 1;
				break;
			}
			if (buff[cursor] == '\"') { // a " character is next
				is_quoted_string = !is_quoted_string;
				cursor += 1;
			} else if (buff[cursor] == '\\') { // a \ character is next
				switch(buff[cursor+1]) {
				case 'n':
					token[k] = '\n'; k += 1;
					break;
				case 't':
					token[k] = '\t'; k += 1;
					break;
				case '\\':
					token[k] = '\\'; k += 1;
					break;
				default:
					break;
				}
				cursor += 2;
			} else {
				token[k] = buff[cursor];
				k += 1;
				cursor += 1;
			}
		}
		token[k] = 0;
	} while (k == 0);

	while (buff[cursor] != 0 && IS_SPACE(buff[cursor])) {
		cursor += 1;
	}

	/* now, set the various parsing flags */
	if (IS_NUMBER(token[0])) {
		next_token_type = NUMBER;
		token_number_value = atoi(token);
	} else if (! IS_LETTER(token[0])) {
		next_token_type = SYMBOL;
	} else {
		next_token_type = NAME;
	}
}


const char* next_token(void) {
	return token;
}

const char* peek_next_token(void) {
	read_next_token();
	token_has_been_peeked = true;
	return next_token();
}
