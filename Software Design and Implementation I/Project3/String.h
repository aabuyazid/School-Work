#ifndef STRING_H
#define STRING_H 1

#include <stdint.h>

typedef 
struct UTString {
	uint32_t length;	// number of characters before the null terminator
	uint32_t capacity;	// length of the longest string that can be stored
	char* string;		// pointer to the actual string (contains signature after string)
} UTString;
bool isOurs(UTString* s);
UTString* utstrdup(const char* src);
uint32_t utstrlen(const UTString* src);
UTString* utstrcat(UTString* s, const char* suffix); // append the suffix to s
UTString* utstrcpy(UTString* dst, const char* src); // overwrite dst with src
void utstrfree(UTString* self);
UTString* utstrrealloc(UTString* s, uint32_t new_capacity);

#endif /* STRING_H */
