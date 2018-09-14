// Recursion.h -- function declarations for the EE312 Recursion project

#ifndef _Recursion_h
#define _Recursion_h 1

int minIt(int[], int);
int minRec1(int[], int);
int minRec2(int[], int);
double sqrtIt(double, double, double);
double sqrtRec(double, double, double);
int strCompare(char*, char*);
int strCompare2(char*, char*);
int solveMazeRec(int, int);
void solveMazeIt(int, int);


/* this struct is also defined in the project file, please do not change the struct in either file, or you'll break the main program
* (i.e., the two struct definitions must match each other EXACTLY)
*/
struct Martian {
	int pennies;
	int nicks;
	int dodeks;
};

Martian change(int);
Martian changeIt(int);

Martian change(int, int, int);
Martian changeIt(int, int, int);

#endif /* _Recursion_h */
