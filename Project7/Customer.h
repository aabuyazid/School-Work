#ifndef _Customer_h
#define _Customer_h

#include "String.h"

/*
 * Customer is still defined as a struct, i.e., all the elements are public
 * That might not be the best design, but it's adequate for now. Note that I've
 * added a pair of constructors.
 */
struct Customer {
	String name;
	int bottles;
	int diapers;
	int rattles;

	Customer(String);
	Customer(void) { name = "__invalid__"; } 
};

#endif /* _Customer_h */
