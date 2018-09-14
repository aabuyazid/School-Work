// String.h
#ifndef _String_h
#define _String_h 1

struct String {
	int len; // number of chars in the string (does not include the zero)
	char* ptr; // pointer to an array on the heap (where the chars are stored)
};

/* two creation functions, the first promotes an ordinary C string to our Strings
 * the second creates a new duplicate of an existing String */
String StringCreate(const char* c_string);
String StringDup(String* s);

/* a more powerful creation function that creates a new string
 * by concatenating pre and suff
 * the newly created string will consist of all the characters from pre
 * followed by all the characters in suff */
String StringConcat(String* pre, String* suff);

void StringDestroy(String* s); // our cleanup function

/* display the String s on the standard output console */
void StringPrint(String* s);

/* MUTATOR: change the String dst so that it is an identical copy of src 
 * dst must already exist (i.e., dst must have been previously created/initialized) */
void StringReAssign(String* dst, String* src); 

/* PREDICATE: return true iff s1 represents exactly the same string as s2 */
bool StringIsEqualTo(String* s1, String* s2);

/* PREDICATE: return true iff s1 comes before s2 in dictionary order
 * NOTE: the comparison is based on the ordering in the asii table, so it is
 * case sensitive ('Z' < 'a') and is defined for punctuation, although the behavior
 * for comparing strings with punctuation is rather arbitrary */
bool StringIsLessThan(String* s1, String* s2);


/* MUTATOR
 * append suffix to s
 * the string s is increased in size by appending suffix to the end (suffix is not changed) */
void StringAppend(String* s, String* suffix);

/* ACCESSOR
 * return the number of characters in s
 * Use this macro like a function StringSize that returns the length of a String.
 */
#define StringSize(s) ((*s).len)


#endif /* _String_h */
