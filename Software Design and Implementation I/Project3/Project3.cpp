/*
 * Name: Arkan Abuyazid
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "String.h"

#define SIGNATURE (~0xdeadbeef)

/*
 * Helper macro to find the signature of a UTString
 * Treat this like a function that takes a UTString*
 * Accesses a uint32_t from where the signature should be located
 */
#define CHECK(s) (*(uint32_t*) ((s)->string + (s)->length + 1))
#define MAX 50

/*
 * Checks if a UTString is valid or not.
 */
bool isOurs(UTString* s) {
    if (CHECK(s) == SIGNATURE) { return true; }
    else { return false; }
}

/* 
 * Allocate a utstring on the heap. 
 * Initialize the string correctly by copying src.
 * Return a pointer to the UTString.
 */
UTString* utstrdup(const char* src) {

	UTString* stringAddress;
    char* srcCopy;
    uint32_t length = 0;
    uint32_t capacity = 0;
    uint32_t index;
    while(*(src + length) != 0){
        length++;
    }
    capacity = length + 5;
    srcCopy = (char*)malloc(sizeof(char) * capacity);
    for(index = 0; index <= length; index++){
        *(srcCopy + index) = *(src + index);
    }
    stringAddress = (UTString*)malloc(sizeof(UTString));
    stringAddress->string = srcCopy;
    stringAddress->length = length;
    stringAddress->capacity = capacity;
    CHECK(stringAddress) = SIGNATURE;

	return stringAddress;
}

/*
 * Returns the length of s.
 *  s must be a valid UTString.
 */
uint32_t utstrlen(const UTString* s) {
    assert(CHECK(s) == SIGNATURE);
    return (s->length);
}

/*
 * Append the string suffix to the UTString s.
 *  s must be a valid UTString.
 * Do not allocate any additional storage: only append as many characters
 *  as will actually fit in s.
 * Update the length of s.
 * Return s with the above changes. */
UTString* utstrcat(UTString* s, const char* suffix) {
    uint32_t indexUT, indexSuffix, capacity;
    assert(CHECK(s) == SIGNATURE);
    capacity = s->capacity;
    indexUT = s->length;
    indexSuffix = 0;
    while((indexUT < (capacity - 5)) && (*(suffix + indexSuffix) != 0)) {
        *(s->string + indexUT) = *(suffix + indexSuffix);
        indexSuffix++;
        indexUT++;
    }
    *(s->string + indexUT) = 0;
    s->length = indexUT;
    CHECK(s) = SIGNATURE;
	return s;
}

/* 
 * Copy src into dst.
 *  dst must be a valid UTString. 
 *  src is an ordinary string.
 * Do not allocate any additional storage: only copy as many characters
 *  as will actually fit in dst.
 * Update the length of dst. 
 * Return dst with the above changes.
 */
UTString* utstrcpy(UTString* dst, const char* src) {
    assert(CHECK(dst) == SIGNATURE);
    uint32_t index;
    for(index = 0; ((index < (dst->capacity - 5)) && (*src + index) != 0); index++) {
        *(dst->string + index) = *(src + index);
    }
    *(dst->string + index) = 0;
    dst->length = index;
    CHECK(dst) = SIGNATURE;
	return dst;
}

/*
 * Free all memory associated with the given UTString. 
 */
void utstrfree(UTString* self) {
    assert(CHECK(self) == SIGNATURE);
    free(self->string);
    free(self);
}

/* 
 * Make s have at least as much capacity as new_capacity.
 *  s must be a valid UTString.
 * If s has enough capacity already, do nothing.
 * If s does not have enough capacity, then allocate enough capacity, 
 *  copy the old string and the null terminator, free the old string,
 *  and update all relevant metadata. 
 * Return s with the above changes.
 */
UTString* utstrrealloc(UTString* s, uint32_t new_capacity) {
    assert(CHECK(s) == SIGNATURE);
    char stringCopy[s->length];
    uint32_t index;
    if(s->capacity < (new_capacity + 4)) {
        for(index = 0; index <= s->length; index++){
            stringCopy[index] = *(s->string + index);
        }
        s->string = (char*)realloc(s->string, sizeof(char) * (new_capacity + 5));
        for(index = 0; index <= s->length; index++){
            *(s->string + index) = stringCopy[index];
        }
        CHECK(s) = SIGNATURE;
    }
    s->capacity = (new_capacity + 5);
	return s;
}
