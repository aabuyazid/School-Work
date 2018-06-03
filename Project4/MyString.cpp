// String.c
#include <stdio.h>
#include <stdlib.h>

#include "MyString.h"

/* private functions, internal to the development of the String abstract type */

/* copy 'len' characters from 'src' to 'dst'. If there is a zero in the src array
 * it is copied (i.e., we don't assume null-terminated strings */
// true C aficionados would use the standard library function 'memcpy' instead of this function
static void copyChars(char dst[], const char src[], int len) {
	int k = 0;
	while (k < len) {
		dst[k] = src[k];
		k = k + 1;
	}
}


/* public functions, declarations of these functions are included in String.h */

/*
 * c_string is a normal string (using C conventions)
 * initialize a String object and return it
 */
String StringCreate(const char* c_string) {
	String retVal;

	/* Step 1: determine the length of the source string */
	int k = 0;
	while (c_string[k] != 0) {
		k = k + 1;
	}

	/* Step 2: populate our struct's components */
	retVal.len = k;
	retVal.ptr = (char*) malloc(retVal.len * sizeof(char)); 

	/* Step 3: don't forget! to copy the string into the buffer */
	copyChars(retVal.ptr, c_string, retVal.len);

	return retVal;
}

/*
 * initialize a String object that is an exact duplicate of src and return that string
 * (NOTE: this is a deep copy)
 */
String StringDup(String* src) {
	String retVal;

	retVal.len = (*src).len;
	retVal.ptr = (char*) malloc(retVal.len * sizeof(char)); 

	/* Step 3: don't forget! to copy the string into the buffer */
	copyChars(retVal.ptr, (*src).ptr, retVal.len);

	return retVal;
}


void StringDestroy(String* s) {
	free((*s).ptr);
}

void StringPrint(String* s) {
	int k;
	for (k = 0; k < (*s).len; k += 1) {
		putchar((*s).ptr[k]);
	}
}


/*
 * Replace old_str with a copy of new_str
 * This function differs from StringDup in that here we assume old_str is already
 * a String (has previously been initialized). The StringDup function would be appropriate
 * (instead of this function) if you have an uninitialized String struct */
void StringReAssign(String* old_str, String* new_str) {
	if (old_str == new_str) { // huh? don't know why someone would do this
		/* old_str and new_str are already the same, so, just get out of town */
		return;
	}

	StringDestroy(old_str);
	*old_str = StringDup(new_str);
}

/*
 * replace the contents of s with the concatenation of s and suffix
 * NOTE: we must reallocate a new array on the heap to have sufficient storage to hold
 * thew new and longer string that results from concatenating */
void StringAppend(String* s, String* suffix) {
	int new_len = (*s).len + (*suffix).len;
	char* new_ptr = (char*) malloc(new_len * sizeof(char));

	copyChars(new_ptr, (*s).ptr, (*s).len);
	copyChars(new_ptr + (*s).len, (*suffix).ptr, (*suffix).len);

	free((*s).ptr);
	(*s).len = new_len;
	(*s).ptr = new_ptr;
}

/*
 * return a newly created string that is initialized as a copy of the concatenation of a and b */
String StringConcat(String* a, String* b) {
	int j; // indexes into a source string (a or b)
	int k; // indexes into the result string
	String retVal;
	retVal.len = (*a).len + (*b).len;
	retVal.ptr = (char*) malloc(retVal.len * sizeof(char)); // plus one byte for the terminating zero
	
	j = 0;
	k = 0;
	while (j < (*a).len) {
		retVal.ptr[k] = (*a).ptr[j];
		j += 1;
		k += 1;
	}

	j = 0;
	while (j < (*b).len) {
		retVal.ptr[k] = (*b).ptr[j];
		j += 1;
		k += 1;
	}

	return retVal;
}


bool StringIsEqualTo(String* s1, String* s2) {
	int k = 0;

	if ((*s1).len != (*s2).len) { return false; } // lengths must be the same, otherwise, don't bother

	while (k < (*s1).len) {
		if ((*s1).ptr[k] != (*s2).ptr[k]) { // kth character differs, so strings are not the same, quit now
			return false;
		}
		k = k + 1;
	}
	// if we finish the loop, then all characters are equal
	return true;
}

bool StringIsLessThan(String* s1, String* s2) {
	int k = 0;

	while (k != (*s1).len 
		&& k != (*s2).len) {
		if ((*s1).ptr[k] < (*s2).ptr[k]) { // the next character in s1 < the character from s2, so return true
			return true; 
		} else if ((*s1).ptr[k] > (*s2).ptr[k]) { // s1 > s2, return false
			return false;
		} else { // the characters must be the same, so just keep going
			k = k + 1;
		}
	}

	if ((*s1).len == (*s2).len) { // strings are equal (s1 not less than s2)
		return false; 
	} else if ((*s1).len > (*s2).len) { /* s2 is a prefix of s1, i.e., s2 < s1 */
		return false; 
	} else { /* s1 is a prefix of s2, so s1 < s2 */
		return true; 
	}
}


