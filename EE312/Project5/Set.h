#ifndef _Set_h
#define _Set_h 1

struct Set {
	int* elements; 
	int len;
//	int capacity; // NOTE: amortized doubling does not improve the time complexity for these problems
	/* in fact, you can safely ignore capacity entirely. I'm including it here just in case some
	 * students feel more comfortable tracking the array size and the number of elements independently
	 * REPEATNG: You do not need to to implement amortized doubling in order to achieve the time-complexity
	 * bounds for this problem. You can achieve all the time complexity bounds even when
	 * capacity is ignored (i.e., your design can assume capacity == len).
	 */
};

/* creation, assignment and destruction functions */
void createSingletonSet(Set* self, int x); // construct a set containing only the element x
void createEmptySet(Set* self); // construct a set containing no elements
void createCopySet(Set* self, const Set* other); // construct a new set that has the same elements as other
void assignSet(Set* self, const Set* other); // replace the elements in this set with the elements of other
void destroySet(Set* self);

/* predicate functions */
bool isMemberSet(const Set* self, int x); // return true iff x is an element in the set
bool isEmptySet(const Set* self); // return true iff the set contains zero elements
bool isEqualToSet(const Set* self, const Set* other); // return true iff this and other
	                                       	// contain the same elements
bool isSubsetOf(const Set* self, const Set* other);	// return true if all of the elements
	                                        	// self are also elements of other
	                                        	// NOTE: for any set, x, x is a subset
	                                        	// of itself.
	/* printing */
void displaySet(const Set* self);

/* scalar manipulation functions */
void insertSet(Set* self, int x); // insert x into this set.  does nothing if
	                   	//   x is already in the set
void removeSet(Set* self, int x); // remove x from this set.  does nothing if 
	                  	//   x is not in the set

/* manipulations with sets */
void unionInSet(Set* self, const Set* other); // replace the elements of self with self union other
void intersectFromSet(Set* self, const Set* other); // replace the elements of self with self intersection other
void subtractFromSet(Set* self, const Set* other); // replace the elements of self with self minus other

#endif /* _Set_h */
