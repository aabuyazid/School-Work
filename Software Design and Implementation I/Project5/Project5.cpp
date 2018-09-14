/*
 * Set ADT
 * Project5.c 
 *
 * Arkan Abuyazid
 * Monday 9am
 * Spring 2018
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Set.h"
#define SIZE 1

/*
 * Several of the required functions have been written already
 * You may use these functions as-is or you may modify them in any way. 
 * Please NOTE, you are responsible for ensuring that the functions work correctly in 
 * your project solution. Just because the function works correctly in my solution does 
 * NOT mean that the function will work correctly in yours. 
 */

/*
 * Design NOTES:
 *
 * The design provided in this starter kit assumes
 * (1) empty sets will be represented with length == 0 and elements == nullptr (i.e., address 0)
 * (2) amortized doubling is not used, and capacity is ignored/unused. Functions should assume that 
 * the amount of storage available in the elements[] array is equal to length
 */



/* done for you already */
void destroySet(Set* self) {
	free(self->elements);
}

/* done for you already */
void createEmptySet(Set* self) {
	self->len = 0;
	self->elements = 0;
}

/* done for you already */
void createSingletonSet(Set* self, int x) {
	self->elements = (int*) malloc(sizeof(int) * SIZE);
	self->elements[0] = x;
	self->len = 1;
}

/* done for you already */
void createCopySet(Set* self, const Set* other) {
	self->elements = (int*) malloc(other->len * sizeof(int));
	for (int k = 0; k < other->len; k += 1) {
		self->elements[k] = other->elements[k];
	}
	self->len = other->len;	
}

/* done for you already */
void assignSet(Set* self, const Set* other) {
	if (self == other) { return; }

	destroySet(self);
	createCopySet(self, other);
}

/* return true if x is an element of self */
bool isMemberSet(const Set* self, int x) {
    /* uses binary search */
    int start = 0;
    int end = self->len - 1;
    while(start <= end) {
        int mid = (start + end)/2;
        if(self->elements[mid] == x) return true;
        if(self->elements[mid] > x) {
            end = mid - 1;
        }
        if(self->elements[mid] < x) {
            start = mid + 1;
        }
    }
    return false;
}

/*
 * add x as a new member to this set. 
 * If x is already a member, then self should not be changed
 * Be sure to restore the design invariant property that elemnts[] remains sorted
 * (yes, you can assume it is sorted when the function is called, that's what an invariant is all about)
 */
void insertSet(Set* self, int x) {
    if (self->len == 0) {     /* if self is an empty set */
        createSingletonSet(self, x);
        return;
    }
    if (isMemberSet(self, x)) {
        return;
    }
    /* local int array */
    int elementsCopy[self->len + 1];
    int indexCopy = 0;
    int index;
    /* inserted dictates which loop the algorithm will run */
    bool inserted = false;
    for (index = 0; index < self->len - 1; index++) {
        if (!inserted) {
            if (index == 0 && self->elements[index] > x) {
                elementsCopy[indexCopy] = x;
                elementsCopy[indexCopy + 1] = self->elements[index];
                indexCopy += 2;
                inserted = true;
                continue;
            }
            else if (self->elements[index] < x && self->elements[index + 1] > x) {
                elementsCopy[indexCopy] = self->elements[index];
                elementsCopy[indexCopy + 1] = x;
                indexCopy += 2;
                inserted = true;
                continue;
            }
            else {
                elementsCopy[indexCopy] = self->elements[index];
                indexCopy++;
            }
        }
        /* occurs when the element has been inserted */
        else {
            elementsCopy[indexCopy] = self->elements[index];
            indexCopy++;
        }
    }
    if(index == self->len - 1) {
        if (!inserted) {
            if (self->elements[index] < x) {
                elementsCopy[indexCopy] = self->elements[index];
                elementsCopy[indexCopy + 1] = x;
                indexCopy += 2;
            } else {
                elementsCopy[indexCopy] = x;
                elementsCopy[indexCopy + 1] = self->elements[index];
                indexCopy += 2;
            }
        } else {
            elementsCopy[indexCopy] = self->elements[index];
            indexCopy++;
        }
    }
    free(self->elements);
    self->len = indexCopy;
    self->elements = (int*)malloc(sizeof(int) * self->len);
    for(int i = 0; i < self->len; i++) {
        self->elements[i] = elementsCopy[i];
    }
    //self->elements = elementsCopy;
}



/*
 * don't forget: it is OK to try to remove an element
 * that is NOT in the set.  
 * If 'x' is not in the set 'self', then
 * removeSet should do nothing (it's not an error)
 * Otherwise, ('x' IS in the set), remove x. Be sure to update self->length
 * It is not necessary (nor recommended) to call malloc -- if removing an element means the 
 * array on the heap is "too big", that's almost certainly OK, and reallocating a smaller array 
 * is almost definitely NOT worth the trouble
 */
void removeSet(Set* self, int x) {
    if(!isMemberSet(self, x)) {
        return;
    }
    bool removed = false;
    int index = 0;
    while (index < self->len) {
        if(!removed) {
            if (self->elements[index] == x){
                removed = true;
            }
            index++;
            continue;
        }
        /* if already removed, move the rest of the elements forward */
        else {
            self->elements[index - 1] = self->elements[index];
            index++;
        }
    }
    self->len--;
}

/* done for you already */
void displaySet(const Set* self) {
	int k;

	printf("{");

	if (self->len == 0) {
		printf("}");
	}
	else {
		for (k = 0; k < self->len; k += 1) {
			if (k < self->len - 1) {
				printf("%d,", self->elements[k]);
			} else {
				printf("%d}", self->elements[k]);
			}
		}
	}
}

/* return true if self and other have exactly the same elements */
bool isEqualToSet(const Set* self, const Set* other) {

    /* if the sets' lengths aren't the same,
     * then its obvious they aren't the equal */
    if(self->len != other->len) {
        return false;
    }
    int index = 0;
    while (index < self->len) {
        if(self->elements[index] != other->elements[index]) {
            return false;
        }
        index++;
    }
    return true;

}

/* return true if every element of self is also an element of other */
bool isSubsetOf(const Set* self, const Set* other) {
    if(isEmptySet(self)) {
        return true;
    }
    if(self->len > other->len) {
        return false;
    }
    else if(self->len == other->len) {
        return isEqualToSet(self, other);
    }
    else {
        int indexOther = 0;
        int indexSelf = 0;
        /* checks both arrays until on has been completely checked */
        while(indexSelf < self->len && indexOther < other->len - 1) {
            if(self->elements[indexSelf] == other->elements[indexOther]) {
                indexSelf++;
                indexOther++;
            }
            else if(self->elements[indexSelf] > other->elements[indexOther]
                    && self->elements[indexSelf] < other->elements[indexOther + 1]) {
                return false;
            }
            else if(self->elements[indexSelf] < other->elements[indexOther]) {
                indexSelf++;
            }
            else if(self->elements[indexSelf] > other->elements[indexOther]) {
                indexOther++;
            }
        }
        /* requires more analysis if other set has not been completed */
        if(indexOther == other->len - 1){
            while(indexSelf < self->len) {
                if(self->elements[indexSelf] <= other->elements[indexOther]) {
                    indexSelf++;
                }
                else if(self->elements[indexSelf] > other->elements[indexOther]) {
                    return false;
                }
            }
        }
        return true;
    }
}

/* done for you */
bool isEmptySet(const Set* self) {
	return self->len == 0;
}

/* remove all elements from self that are not also elements of other */
void intersectFromSet(Set* self, const Set* other) {
    if(isEmptySet(other)) {
        createEmptySet(self);
        return;
    }
    int elementsSelf[self->len];
    int indexSelf = 0;
    int indexOther = 0;
    int index = 0;
    while (indexSelf < self->len && indexOther < other->len) {
        if(self->elements[indexSelf] == other->elements[indexOther]){
            elementsSelf[index] = self->elements[indexSelf];
            index++;
            indexSelf++;
            indexOther++;
        }
        else if(self->elements[indexSelf] < other->elements[indexOther]) {
            indexSelf++;
        }
        else if(self->elements[indexSelf] > other->elements[indexOther]) {
            indexOther++;
        }
    }
    self->len = index;
    //free(self->elements);
    //self->elements = (int*)realloc(self->elements, sizeof(int) * self->len);
    for(int i = 0; i < self->len; i++) {
        self->elements[i] = elementsSelf[i];
    }
    //self->elements = elementsSelf;
}

/* remove all elements from self that are also elements of other */
void subtractFromSet(Set* self, const Set* other) {
    if(other->len == 0){
        return;
    }
    /* max number of elements will be the current length of the set */
    int elementsSelf[self->len];
    int indexSelf = 0;
    int indexOther = 0;
    int index = 0;
    while (indexSelf < self->len && indexOther < other->len) {
        /* if the sets have a common element, don't add it */
        if(self->elements[indexSelf] == other->elements[indexOther]) {
            indexSelf++;
            indexOther++;
        }
        else if(self->elements[indexSelf] < other->elements[indexOther]) {
            elementsSelf[index] = self->elements[indexSelf];
            index++;
            indexSelf++;
        }
        else if(self->elements[indexSelf] > other->elements[indexOther]) {
            indexOther++;
        }
    }
    if(indexOther == other->len) {
        while(indexSelf < self->len) {
            elementsSelf[index] = self->elements[indexSelf];
            index++;
            indexSelf++;
        }
    }
    //free(self->elements);
    self->len = index;
    //self->elements = (int*)realloc(self->elements, sizeof(int) * self->len);
    for(int i = 0; i < self->len; i++) {
        self->elements[i] = elementsSelf[i];
    }
}

/* add all elements of other to self (obviously, without creating duplicate elements) */
void unionInSet(Set* self, const Set* other) {
    if (isEmptySet(other)) {
        return;
    }
    int elementsSelf[self->len + other->len];
    int indexSelf = 0;
    int indexOther = 0;
    int index = 0;
    /* analyzes the elements in both sets until one has been exhausted */
    while(indexOther < other->len && indexSelf < self->len) {
        if(other->elements[indexOther] < self->elements[indexSelf]) {
            elementsSelf[index] = other->elements[indexOther];
            index++;
            indexOther++;
        }
        else if(other->elements[indexOther] == self->elements[indexSelf]) {
            elementsSelf[index] = other->elements[indexOther];
            index++;
            indexOther++;
            indexSelf++;
        }
        else if(other->elements[indexOther] > self->elements[indexSelf]) {
            elementsSelf[index] = self->elements[indexSelf];
            index++;
            indexSelf++;
        }
    }
    /* one will finish the other the following lines deal with those situations */
    if(indexSelf == self->len){
        while(indexOther < other->len) {
            elementsSelf[index] = other->elements[indexOther];
            index++;
            indexOther++;
        }
    }
    else if(indexOther == other->len) {
        while(indexSelf < self->len) {
            elementsSelf[index] = self->elements[indexSelf];
            index++;
            indexSelf++;
        }
    }
    free(self->elements);
    self->len = index;
    self->elements = (int*)malloc(sizeof(int) * self->len);
    for(int i = 0; i < self->len; i++) {
        self->elements[i] = elementsSelf[i];
    }
}
