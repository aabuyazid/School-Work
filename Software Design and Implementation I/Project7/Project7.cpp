/*
 * Name:
 * EID:
 * Project 7, CRM part 2
 * EE 312, Spring 2017
 */

#include <stdio.h>
#include <iostream>
#include "String.h"
#include "Customer.h"
#include "CustomerDB.h"

void readString(String&);
void readNum(int&);

CustomerDB database;

int num_bottles = 0;
int num_rattles = 0;
int num_diapers = 0;

/* clear the inventory and reset the customer database to empty */
void reset(void) {
	database.clear();
	num_bottles = 0;
	num_rattles = 0;
	num_diapers = 0;
}

/*
 * selectInventItem is a convenience function that allows you
 * to obtain a pointer to the inventory record using the item type name
 * word must be "Bottles", "Diapers" or "Rattles"
 * for example the expression *selectInventItem("Bottles") returns the 
 * current global variable for the number of bottls in the inventory
 */
int* selectInventItem(String word) {
	if (word == "Bottles") {
		return &num_bottles;
	} else if (word == "Diapers") {
		return &num_diapers;
	} else if (word == "Rattles") {
		return &num_rattles;
	}
	
	/* NOT REACHED */
	return 0;
}

/*
 * this overload of selectInventItem is similar to selectInventItem above, however
 * this overload takes a Customer as a second argument and selects the data member
 * of that Customer that matches "word". The function returns a pointer to one of the three data
 * members from the specified customer.
 */
int* selectInventItem(String word, Customer& cust) {
	if (word == "Bottles") {
		return &cust.bottles;
	} else if (word == "Diapers") {
		return &cust.diapers;
	} else if (word == "Rattles") {
		return &cust.rattles;
	}
	
	/* NOT REACHED */
	return 0;
}

/*
 * findMax searches through the CustomerDB "database" and returns the Customer
 * who has purchased the most items of the specified type.
 * type must be one of "Bottles", "Rattles" or "Diapers".
 * 
 * Note: if two or more Customers are tied for having purchased the most of that item type
 * then findMax returns the first Customer in the CustomerDB who has purchased that maximal
 * quantity.
 *
 * Note: in the special case (invalid case) where there are zero Customers in the 
 * CustomerDB, fundMax returns a null pointer (0)
 */
Customer* findMax(String type) {
	Customer* result = 0;
	int max = 0;
	for (int k = 0; k < database.size(); k += 1) {
		Customer& cust = database[k];
		int *p = selectInventItem(type, cust); 
		if (*p > max) {
			max = *p;
			result = &cust;
		}
	}
	
	return result;
}



void processPurchase() {
    String name;
    String type;
    int number;
    readString(name);
    readString(type);
    readNum(number);
    if(number == 0) {
        return;
    }
    if(*selectInventItem(type) - number >= 0) {
        *selectInventItem(type, database[name]) += number;
        *selectInventItem(type) -= number;
    }
    else {
        std::cout << "Sorry " << name.c_str() << ", we only have " << *selectInventItem(type) << " " << type.c_str() << '\n';
    }
    return;
}

void printMax(String type) {
    Customer* customer = findMax(type);
    if(customer != NULL) {
        std::cout << customer->name.c_str() << " has purchased the most "
                  << type.c_str() << " (" << *selectInventItem(type, *customer) << ")\n";
    }
    else {
        std::cout << "no one has purchased any " << type.c_str() << '\n';
    }
}

void processSummarize() {
    std::cout << "There are " << num_bottles << " Bottles, "
              << num_diapers << " Diapers and " << num_rattles << " Rattles in inventory" << '\n';
    std::cout << "we have had a total of " << database.size() << " different customers" << '\n';
    printMax("Bottles");
    printMax("Diapers");
    printMax("Rattles");
}

void processInventory() {
    String type;
    int number;
    readString(type);
    readNum(number);
    *selectInventItem(type) += number;
}
