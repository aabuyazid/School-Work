/*
 * Project6.c
 * Recursion
 * Fall 2017
 * My Name
 * My UT EID
 * My TA
 * My Section Time
 *
 *
 */


#include <stdio.h>
#include <stdint.h>
#include "MazeParams.h"
#include "Recursion.h"

void adjacentCell(int row, int col, int dir, int* trow, int* tcol);
int isOK(int row, int col);

/* return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 */

int minIt(int x[], int n) {
	return 0;
}

/* return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 * solve the problem recursively and 
 * use an "n-1" type of decomposition
 */
int minRec1(int x[], int n) {

    if(n == 1) {
        return x[0];
    }
    if(x[n - 1] < x[n - 2]) {
        x[n - 2] = x[n - 1];
    }
	return minRec1(x, n - 1);

}

/*
 * return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 * n may be either odd or even
 * solve the problem recursively and 
 * use an "n / 2" type of decomposition
 */
int minRec2(int x[], int n) {

    if(n == 1){
        return x[0];
    }
    int remainder = n % 2;
    if(minRec2(x, n/2) < minRec2(&x[n/2], n/2 + remainder)) {
        return minRec2(x, n/2);
    }
    else {
        return minRec2(&x[n/2], n/2 + remainder);
    }

}


/*
 * calculate and return the square root of x.
 * The other two parameters are estimates of the square root.
 * low_guess is smaller than the actual square root, and high_guess 
 * is larger than the actual square root.
 * Searching for the square root can be done much like searching
 * for a name in a phone book.
 *
 * Since you can't calculate a square root exactly, for this problem
 * you're required to calculate the answer to 15 decimal digits of
 * accuracy.
 */
double sqrtIt(double x, double low_guess, double high_guess) {
	return 0;
}

/*
 * calculate and return the square root of x.
 * The other two parameters are estimates of the square root.
 * low_guess is smaller than the actual square root, and high_guess 
 * is larger than the actual square root.
 * Searching for the square root can be done much like searching
 * for a name in a phone book.
 *
 * Since you can't calculate a square root exactly, for this problem
 * you're required to calculate the answer to 15 decimal digits of
 * accuracy.
 */
double sqrtRec(double x, double low_guess, double high_guess) {
    double avg = (low_guess + high_guess)/2;
    if((high_guess - low_guess)/avg <= 1E-15) {
        return avg;
    }
    if(avg * avg > x) {
        return sqrtRec(x, low_guess, avg);
    }
    if(avg * avg < x) {
        return sqrtRec(x, avg, high_guess);
    }
}


/*
 * using only recursion, write a string comparison function
 * return -1 if str1 is less than str2
 * return 0 if the two strings are equal
 * return +1 if str1 is greater than str2
 * when comparing strings, use the ASCII value to compare each character
 * (i.e., that means 'A' is less than 'a' since it's ASCII value is smaller)
 * The string str1 is less than str2 if either
 * str1[0] < str2[0]
 * or there exists a k such that str1[k] < str2[k] and 
 *   for all j < k str1[j] == str2[j]
 *   and k is less than the length of str1 and str2
 */

int strCompare(char* str1, char* str2) {
    if(*str1 == NULL && *str2 == NULL) {
        return 0;
    }
	else if(*str1 == NULL) {
        return -1;
    }
    else if(*str2 == NULL){
        return 1;
    }
    if(*str1 < *str2) {
        return -1;
    }
    else if(*str1 > *str2) {
        return 1;
    }
    else {
        return strCompare(&str1[1], &str2[1]);
    }
}

/*
 * if c is not a letter return -1
 * if c is a letter, return the position of c in the alphabet 
 * where 'A' (and 'a') are position 1 and 'Z' (and 'z') are position 26
 *
 * This code is rather ugly as I'm exploiting some detailed knowledge of the ASCII table
 */
int whatLetter(char c) {
	if (c < 'A') { return -1; }
	if (c > 'z') { return -1; }
	if (c > 'Z' && c < 'a') { return -1; }
	return c & ~32 - 64;
}

/*
 * same as before, only this time 
 * ignore anything that is not a letter
 * ignore the case (upper case, lower case)
 * So, strCompare2("Hello", "hello") should return 0 (they're the same)
 * strCompare2("The plane!", "theater") should return 1 since "theplane" is larger than "theater"
 * once again, you can only use recursion, no loops
 */
int strCompare2(char* str1, char* str2) {

    if(*str1 == NULL && *str2 == NULL) {
        return 0;
    }
    else if((*str1 == NULL && whatLetter(*str2) == -1)
            || (whatLetter(*str1) == - 1 && *str2 == NULL)){
        return 0;
    }
    else if(*str1 == NULL) {
        return -1;
    }
    else if(*str2 == NULL){
        return 1;
    }
    if(whatLetter(*str1) == -1 && whatLetter(*str2) == -1) {
        return strCompare2(&str1[1], &str2[1]);
    }
    else if(whatLetter(*str1) == -1) {
        return strCompare2(&str1[1], str2);
    }
    else if(whatLetter(*str2) == -1) {
        return  strCompare2(str1, &str2[1]);
    }
    if(whatLetter(*str1) < whatLetter(*str2)) {
        return -1;
    }
    else if (whatLetter(*str1) > whatLetter(*str2)) {
        return 1;
    }
    else {
        return strCompare2(&str1[1], &str2[1]);
    }

}



/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the 
 * value is 1).  Each 0 represents an open space.  
 *
 * Write an recursive solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 *
 * There is a relatively easy recursive solution to this problem, the trick is
 * to think of the solution in the following terms:
 *   "In which direction(s) can I go and find a way out of this maze?"
 * In some cases, you may find yourself in a spot in the maze that there's
 * no way out (at least, not without backtracking). In that case, you'd return "false"
 * since the maze has no solution. In most cases, there's one or more ways out
 * from where you are now. Your key question is simply, "what is the first step I need to take"
 *
 * If you considered going "north", and you had a function that could tell you whether
 * it was possible to get out of the maze starting at the square to the north of your 
 * current position, then you could use this function to determine if you can get out by
 * going north first. Similarly, you could consider going south, east or west, and (recursively)
 * determine if the maze can be solved from any of those locations.
 *
 * With that hint, the base case should be pretty obvious.  In fact,
 * I'd suggest you consider the possibility that the base case is "Yup, I'm already at 
 * the exit!"
 *
 * There is one tricky part to this decomposition.  You need to make certain 
 * that the problem is getting smaller.  The "bigness" or "smallness" of this 
 * problem is the number of squares that have not yet been tested.  Each time
 * you test an adjacent square (making a recursive call to decide if there is a 
 * path to the exit through that square), you'll be reducing  the number of squares 
 * that have not yet been tested.  Eventually, you must have tested all the 
 * squares and so you'll have to have found a way to the exit.
 *
 * The easy way to deal with the tricky part is to leave "bread crumbs" behind.
 * Before you recursively check any (or all) of your neighbors to see if you 
 * can find the exit from there -- drop a bread crumb in your current square.
 * Then, don't ever check to see if you can find the exit using a square
 * with a breadcrumb in it (backtracking into that square would be silly).
 *
 * If you're trying to see if you can find an exit from some square, and all 
 * the adjacent squares are either walls, or have bread crumbs in them, then
 * you already know the answer -- "you can't get to the exit from here". 
 * Pick up your bread crumb and return false.
 * 
 * You can set the value of the current square to "2" to indicate that a bread
 * crumb has been dropped.  Set the square back to 0 when you want to pick 
 * the bread crumb back up.
 * be sure not to change any of the squares that are 1 (the walls).
 *
 * for partial credit, you can leave all your bread crumbs on the floor.
 * for full credit, you must pick up all the bread crumbs EXCEPT those
 * along a path to the exit.
 */

int solveMazeRec(int row, int col) {
	if(row == MATRIX_SIZE - 1){
        maze[row][col] = 2;
        return 1;
    }
    if(isOK(row - 1, col)) {
        if(maze[row - 1][col] != 2) {
            maze[row][col] = 2;
            int path = solveMazeRec(row - 1, col);
            if(!path) {
                maze[row][col] = 0;
            }
            else {
                return path;
            }
        }
    }
    if(isOK(row + 1, col)) {
        if(maze[row + 1][col] != 2) {
            maze[row][col] = 2;
            int path = solveMazeRec(row + 1, col);
            if(!path) {
                maze[row][col] = 0;
            }
            else {
                return path;
            }
        }
    }
    if(isOK(row, col - 1)) {
        if(maze[row][col - 1] != 2) {
            maze[row][col] = 2;
            int path = solveMazeRec(row, col - 1);
            if(!path) {
                maze[row][col] = 0;
            }
            else {
                return path;
            }
        }
    }
    if(isOK(row, col + 1)) {
        if(maze[row][col + 1] != 2) {
            maze[row][col] = 2;
            int path = solveMazeRec(row, col + 1);
            if(!path) {
                maze[row][col] = 0;
            }
            else {
                return path;
            }
        }
    }
    return 0;
}


/**********************
 * adjacentCell and isOK are functions provided to help you write
 * solveMazeIt()
 */

/*
 * OK, we're currently at maze[row][col] and we're considering moving
 * in direction dir.  
 * Set trow and tcol (local variables inside the calling function)
 * to the row and column that we would move to IF we moved in
 * that direction
 *
 * For example, there are two good ways to use this function.
 * 1. to actually move one step in a direction use:
 *       adjacentCell(row, col, dir, &row, &col);
 *    That will set row and col to new values.  The new values will
 *    be one square away from the old values.
 *
 * 2. to set trow and tcol to a square that is adjacent to row and col use:
 *       adjacentCell(row, col, dir, &trow, &tcol);
 *    That will not change row and col, but will change trow and tcol.
 *    This is useful if you aren't sure if you can actually move in this 
 *    direction yet (e.g., maze[trow][tcol] may be a wall!).  So, you set
 *    trow and tcol, and then check to see if it's OK to move there
 */
void adjacentCell(int row, int col, int dir, int* trow, int* tcol) {
	*trow = row;
	*tcol = col;
	switch(dir) {
	case 0: // UP
		*trow = *trow - 1;
		break;
	case 1: // RIGHT
		*tcol = *tcol + 1;
		break;
	case 2: // DOWN
		*trow = *trow + 1;
		break;
	case 3: // LEFT
		*tcol = *tcol - 1;
		break;
	}
}


/* 
 * return false if there is a wall in the square for row and col
 * return true if it's not a wall.
 */
int isOK(int row, int col) {
	return (row > 0 && row < MATRIX_SIZE
		&& col > 0 && col < MATRIX_SIZE
		&& maze[row][col] != 1);
}

/*
 * return the value of the direction that is one turn to the right
 */
int turnRight(int dir) {
	return (dir + 1) % 4;
}

/*
 * return the value of the direction that is one turn to the left
 */
int turnLeft(int dir) {
	return (dir + 3) % 4;
}

/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the value
 * is 1).  Each 0 represents an open space.  
 *
 * write an iterative solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 * The maze can be solved by following the right hand wall.  In fact, there
 * is exactly one path between any two points in the maze (well, between any two
 * points that are not walls).
 *
 * The algorithm is not too bad, although it is certainly not trivial
 * Initially, you'll be headed DOWN (direction 2)
 * Each iteration do one of the following.  Note that only one of two cases
 * can possibly apply (the properties of the maze guarantee that).
 *    case 1: you can move in the current direction
 *       In this case, you should take one step in the current direction
 *       and then turn right.  
 *    case 2: you cannot move in the current direction
 *       If there's a wall directly in front of you, then to follow the right
 *       hand wall, you'd need to turn left (placing your hand on the wall that
 *       used to be directly in front of you).  So, if you discover this case
 *       then turn left.  Don't move, just turn left.
 * If you were walking down a straight corridor (with walls on both sides)
 * then you'd alternate case 1 and case 2.  On the first iteration, you'd
 * take a step, and then turn right (case 1).  This causes you to walk
 * one position down the hallway but then turn to face the wall.
 * On the next iteration, you'll be facing the wall, so you can't walk
 * forward and therefore (case 2) you'll turn left.  On the third iteration
 * you'll be able to walk down the hallway again.
 *
 * If you follow those two cases, then you'll eventually find your way out
 * of the maze.  To confirm that you're making it out, leave a "bread crumb" 
 * behind each square along the path to the exit.
 * For partial credit, place a bread crumb in every square you visit.
 * For full credit, pick up all the breadcrumbs when you backtrack.  
 * Backtracking is when you go back to a square you've already been before.  
 * If the square you're about to step into has a breadcrumb, then pick up 
 * the bread crumb in the square you're at now.
 * You indicate "bread crumbs" by setting the square equal to "2"
 */
void solveMazeIt(int row, int col) {
	int dir = 2; // 0 is up, 1 is right, 2 is down, 3 is left.
	maze[row][col] = 2; // drop a bread crumb in the starting square
	while (row < MATRIX_SIZE - 1) { // the exit is the only open square 
				// in the last row

		/* the rest of this loop is yours */

	}
}


/*
 * using recursion, with no loops or globals, write a function that calculates the optimal
 * (fewest total coins) change for a given amount of money. Return a Martian struct that indicates
 * this optimal collection of coins.
 */
Martian change(int cents) {
    Martian martian1 = {0, 0, 0};
    Martian martian2;
    if(cents <= 0) {
        return martian1;
    }
    if(cents < 5) {
        martian2 = change(cents - 1);
        martian2.pennies++;
        return martian2;
    }
    if(cents == 5) {
        martian1.nicks++;
        return martian1;
    }
    if(cents == 12) {
        martian1.dodeks++;
        return martian1;
    }
    if(cents > 5) {
        martian1 = change(cents - 5);
        martian2 = change(cents - 12);
        int sumCoin1 = martian1.dodeks + martian1.nicks + martian1.pennies;
        int sumCoin2 = martian2.dodeks + martian2.nicks + martian2.pennies;
        if(sumCoin1 <= sumCoin2 || (sumCoin1 != 0 && sumCoin2 == 0)) {
            martian1.nicks++;
            return martian1;
        }
        if(sumCoin2 < sumCoin1 || (sumCoin2 != 0 && sumCoin1 == 0)){
            martian2.dodeks++;
            return martian2;
        }
    }
}

/*
 * Same as before, except the more general version where the coins have values
 * a cents and b cents, with a and b being algebraic. For the original problem, 
 * a is the constant 5 (nick_value = 5 cents) and b is the constant 12 
 * (dodek_value = 12 cents). 
 * If you've really mastered thinking recursively, then this version of the 
 * martian change problem is just as easy as the concrete version 
 */
Martian change(int cents, int nick_val, int dodek_val) {
    Martian martian1 = {0, 0, 0};
    Martian martian2;
    if(cents <= 0) {
        return martian1;
    }
    if(cents < nick_val) {
        martian2 = change(cents - 1, nick_val, dodek_val);
        martian2.pennies++;
        return martian2;
    }
    if(cents == nick_val) {
        martian1.nicks++;
        return martian1;
    }
    if(cents == dodek_val) {
        martian1.dodeks++;
        return martian1;
    }
    if(cents > nick_val) {
        martian1 = change(cents - nick_val, nick_val, dodek_val);
        martian2 = change(cents - dodek_val, nick_val, dodek_val);
        int sumCoin1 = martian1.dodeks + martian1.nicks + martian1.pennies;
        int sumCoin2 = martian2.dodeks + martian2.nicks + martian2.pennies;
        if(sumCoin1 <= sumCoin2 || (sumCoin1 != 0 && sumCoin2 == 0)) {
            martian1.nicks++;
            return martian1;
        }
        if(sumCoin2 < sumCoin1 || (sumCoin2 != 0 && sumCoin1 == 0)){
            martian2.dodeks++;
            return martian2;
        }
    }
}

/* 
 * without using recursion, solve the Martian change problem described above.
 * it's not too bad for the specific case of nick_value = 5 and dodek_value = 12
 */
Martian changeIt(int cents) {
	return Martian{}; // delete this line, it's broken. Then write the function properly!
}

/*
 * However, I don't have a solution for the general case. In fact, I consider 
 * the general problem to "challenging" (i.e, an exercise in futility). 
 * But, if you like banging your head against the wall for zero credit
 * this is the problem for you!
 */
Martian changeIt(int cents, int nick_value, int dodek_value) {
	return Martian{}; // delete this line, it's broken. Then write the function properly!
}
