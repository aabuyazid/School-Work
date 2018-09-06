// StringADT demonstration file main.cpp
#include <stdio.h>
#include <string.h> // declares the built-in C string library functions, like strcat
#include "String.h" // declares our new improved String functions, like utstrcat
#include <stdlib.h>
#define DEBUG 1

#define CHECK(s) (*(uint32_t*) ((s)->string + (s)->length + 1))

void testStage1(void) {
	char p[12];
	const char* q = "Hello World";
	UTString* s; 
	UTString* t; 
	
	printf("this test should print Hello World three times\n");

	for (unsigned k = 0; k < 12; k += 1) {
		p[k] = q[k];
	}
	s = utstrdup(p);
	printf("%s\n", s->string);

	q = "you goofed!";
	for (unsigned k = 0; k < 12; k += 1) {
		p[k] = q[k];
	}
	printf("%s\n", s->string);
	
	t = utstrdup(s->string);
	utstrfree(s);
	printf("%s\n", t->string);
	utstrfree(t);
}	

void testStage2(void) {
	char c_str1[20] = "hello";
	UTString* ut_str1;
	UTString* ut_str2;

	printf("Starting stage 2 tests\n");
	strcat(c_str1, " world");
	printf("%s\n", c_str1); // nothing exciting, prints "hello world"

	ut_str1 = utstrdup("hello ");
	ut_str1 = utstrrealloc(ut_str1, 20);

	utstrcat(ut_str1, c_str1);
	printf("%s\n", ut_str1->string); // slightly more exciting, prints "hello hello world"
	utstrcat(ut_str1, " world");
	printf("%s\n", ut_str1->string); // exciting, should print "hello hello world wo", 'cause there's not enough room for the second world

	ut_str2 = utstrdup("");
	ut_str2 = utstrrealloc(ut_str2, 11);
	utstrcpy(ut_str2, ut_str1->string + 6);
	printf("%s\n", ut_str2->string); // back to "hello world"

	ut_str2 = utstrrealloc(ut_str2, 23);
	utstrcat(ut_str2, " ");
	utstrcat(ut_str2, ut_str1->string);
	printf("%s\n", ut_str2->string); // now should be "hello world hello hello"
 
	utstrfree(ut_str1);
	utstrfree(ut_str2);
}

#define BIG 1000000
void testStage3(void) {
	int k;
	UTString* ut_str1 = utstrdup("");
	ut_str1 = utstrrealloc(ut_str1, BIG); // big, big string

	printf("attempting stage 3 test. This shouldn't take long...\n");
	printf("(no really, it shouldn't take long, if it does, you fail this test)\n");
	fflush(stdout);

	for (k = 0; k < BIG; k += 1) {
		utstrcat(ut_str1, "*");
	}
	if (ut_str1->string[BIG-1] != '*') {
		printf("stage3 fails for not copying all the characters\n");
	} else if (strlen(ut_str1->string) != BIG) {
		printf("Hmmm, stage3 has something wrong\n");
	} else {
		printf("grats, stage 3 passed (unless it took a long time to print this message)\n");
	}
	utstrfree(ut_str1);
}

#define BAD_SIGNATURE (~0xbaadbeef)

/*
 * This is an important test that likely sounds strange: 
 * Each of the following lines should crash the program by failling an assert. 
 * Try each, one at a time, to make sure your program acts as expected.
 * If your program does not crash, you are doing something wrong and will lose points. 
 */
void testStage4(void) {
     char p[20] = "Hello World!";
	 UTString* utstr1 = utstrdup("Hello World");
     *(uint32_t*)( utstr1->string + utstr1->length + 1 ) = BAD_SIGNATURE;
   //printf("crashing with utstrlen\n\n\n"); utstrlen(utstr1);
//printf("Crashing with utstrcpy\n\n\n"); utstrcpy(utstr1, p);
  // printf("crashing with utstrcat\n\n\n"); utstrcat(utstr1, p);
//	 printf("crashing with utstrfree\n\n\n"); utstrfree(utstr1);
   //printf("crashing with utstrrealloc\n\n\n"); utstrrealloc(utstr1, 40);
    utstrfree(utstr1);
}

void testStage5(void) {
    char test[] = "    suh dude.......   ";
    char catTest[] = "what you gonna do with that ;;;;;;;;;;;;;;; dessert?";
    char copyThis[] = "";
    UTString* utTest = utstrdup(test);
    printf("%s\n", utTest->string);
    printf("The length of this string is %d\n", utTest->length);
    printf("It's capacity is %d\n", utTest->capacity);
    printf("Testing memory reallocation\n");
    printf("Increasing capacity by 100\n");
    utstrrealloc(utTest, 100);
    printf("Testing concatenation\n");
    utstrcat(utTest, catTest);
    printf("%s\n", utTest->string);
    printf("This new string's length is %d\n", utTest->length);
    printf("It's new capacity is now %d\n", utTest->capacity);
    printf("Testing utstrp function for length 0 string\n");
    /*
    utstrcpy(utTest, copyThis);
    printf("%s\n", utTest->string);
    printf("The length is now %d and the capacity is %d\n", utTest->length, utTest->capacity);
    printf("Successful");
     */
    printf("Appending string with length 0\n");
    utstrcat(utTest, copyThis);
    printf("%s\n", utTest->string);
    printf("New length %d; New Capacity: %d\n", utTest->length, utTest->capacity);
    utstrfree(utTest);
}


int main(void) {
	testStage1();
	testStage2();
	testStage3();
	//testStage4();
    testStage5();
	return 0;
}

/*
void printString(char* s) {
    int index = 0;
    while(*(s + index) != 0){
        printf("%c", *(s + index));
        index++;
    }
    printf("\n");
}

int main(void) {
    if(DEBUG) {
        char string1[] = "hello my friends";
        UTString* testCase1 = (utstrdup(string1));
        printf("%p\n", testCase1);
        printString(testCase1->string);
        printf("%d\n", testCase1->capacity);
        printf("%d\n", utstrlen(testCase1));
        printf("%d\n", CHECK(testCase1));
        if(isOurs(testCase1)) {
            printf("Suh");
        } else {
            printf("homie");
        }
    }

}
*/