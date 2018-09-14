#ifndef _Parse_h
#define _Parse_h 1

#include <stdint.h>
enum TokenType {
	NUMBER,
	SYMBOL,
	KEYWORD, // not currently used or supported by Parse.h/Input.cpp
	NAME,
	STRING,  // not currently used or supported by Parse.h/Input.cpp

	END, // special marker for end of input

	INVALID // this token type should never appear and represents a parsing error
};

/* The next_token_type global variable identifies the type of the most recently read
 * token from the input. You read a token using read_next_token. You then use next_token_type
 * to discover what type of token you just read.
 */
extern TokenType next_token_type;

/* if (and only if) next_token_type is equal to NUMBER, then token_number_value
 * will be set to the numeric value of the token. Note that only integer values are
 * supported (in fact, only positive integers are supported presently)
 */
extern int32_t token_number_value;

/* the next_token function returns a pointer to a null-terminated string that repesents
 * the actual raw token. When next_token_type is equal to NUMBER, the next_token string
 * is unecessary, since token_number_value is available. Nevertheless, in that case, next_token()
 * returns a string representation of the number. When next_token_type is either SYMBOL
 * or NAME, then you must use next_token to get the actual token's value
 *
 * NOTE: the token "//" is an actual token in Blip with type "SYMBOL". You must parse
 * this token and the comment that it precedes.
 */
const char* next_token(void);

/* the skip_line function forces the input to ignore the remainder of the current line of
 * input. In ordinary usage, read_next_token() ignores spaces, tabs and line breaks. However
 * when parsing a // comment, you must ignore any token or tokens that remain on the current
 * line. The skip_line is provided for this task
 */
void skip_line(void);

/* set_input allows you to open a text file to read tokens from. By default, read_next_token
 * reads tokens from the standard input (e.g., the keyboard). The argument provided to set_input
 * must be the name of a readable file containing a valid Blip program.
 */
void set_input(const char*);

/* read_next_token() is forces the Input.cpp infrastruture to advance to the next input token
 * This function reads characters from the input source until a complete token has been read
 * (and may read additional characters as well), and forms a token from those characters.
 * read_next_token does not return a value. Programmers must use the other functions and
 * the global variables to determine which token was read and whether there was an error
 */
void read_next_token(void);

/*
 * the peek_next_token() function returns the text of the next token without "reading" the token
 * In other words, the peek function let's you look at the next token that will be read when
 * read_next_token is called, without actually read (and removing) that token from the input
 */
const char* peek_next_token(void);


#endif /* _Parse_h */
