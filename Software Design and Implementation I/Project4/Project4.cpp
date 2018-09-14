// CRM <Project4.cpp>
// EE 312 Project 4 submission by
// Arkan Abuyazid
// ata757
// Slip days used: <0>
// Spring 2018

#include <stdio.h>
#include <assert.h>
#include "MyString.h"
#include "Invent.h"

#define MAX_CUSTOMERS 1000
#define BOTTLES -5
#define DIAPERS -10
#define RATTLES -15
Customer customers[MAX_CUSTOMERS];
String customerName;
int num_customers = 0;
int num_bottles = 0;
int num_diapers = 0;
int num_rattles = 0;

/* you'll probably need several more global variables */
/* and of course, you have a few functions to write */


/* clear the inventory and reset the customer database to empty */
void reset(void) {

    int index;

    /* clears out customers */
    for (index = 0; index < num_customers; index++) {
        StringDestroy(&customers[index].name);
        customers[index].bottles = 0;
        customers[index].diapers = 0;
        customers[index].rattles = 0;
    }

    /* resets the inventory */

    num_customers = 0;
    num_bottles = 0;
    num_diapers = 0;
    num_rattles = 0;

}

/* based on what item is desired, this function searches for the customer
 * who purchased the most said item and returns that customer */

Customer* largestItemCustSrch(int item) {
    int index, indexMax, currentNum;
    int max = 0;
    for (index = 0; index < num_customers; index++) {
        /* determining which product to search for */
        if (item == BOTTLES) {
            currentNum = customers[index].bottles;
        }
        if (item == DIAPERS) {
            currentNum = customers[index].diapers;
        }
        if (item == RATTLES) {
            currentNum = customers[index].rattles;
        }
        if (currentNum > max) {
            max = currentNum;
            indexMax = index;
        }
    }

    return &customers[indexMax];

}
/*
 * Creates a new customer struct
 */
void customerCreate(Customer* newCustomer) {

    newCustomer->name = StringDup(&customerName);
    newCustomer->bottles = 0;
    newCustomer->diapers = 0;
    newCustomer->rattles = 0;

}

/*
 * Checks in the customer array whether that customer has already bought something
 * If yes, then their information is updated
 * If not, then creates a new customer struct for them
 * Prints out an error statement if the amount they ask for exceeds the current stock
 */
void customerPurchase(String item, int number) {

    String bottles = StringCreate("Bottles");
    String diapers = StringCreate("Diapers");
    String rattles = StringCreate("Rattles");
    int index;
    int newPurchased = 0;
    int existence = 0;
    for (index = 0; index < num_customers; index++) {
        if (StringIsEqualTo(&customers[index].name, &customerName)) {
            if (StringIsEqualTo(&item, &bottles)) {
                if (num_bottles >= number) {
                    customers[index].bottles += number;
                    num_bottles -= number;
                }
                else {
                    printf("Sorry ");
                    StringPrint(&customers[index].name);
                    printf(", we only have %d Bottles\n", num_bottles);
                }
            }
            else if (StringIsEqualTo(&item, &diapers)) {
                if (num_diapers >= number) {
                    customers[index].diapers += number;
                    num_diapers -= number;
                }
                else {
                    printf("Sorry ");
                    StringPrint(&customers[index].name);
                    printf(", we only have %d Diapers\n", num_diapers);
                }
            }

            else if (StringIsEqualTo(&item, &rattles)) {
                if (num_rattles >= number) {
                    customers[index].rattles += number;
                    num_rattles -= number;
                }
                else {
                    printf("Sorry ");
                    StringPrint(&customers[index].name);
                    printf(", we only have %d Rattles\n", num_rattles);
                }
            }
            existence = 1;
            break;
        }
    }
    if(!existence) {
        Customer newCustomer;
        customerCreate(&newCustomer);
        if (StringIsEqualTo(&item, &bottles)) {
            if (num_bottles >= number) {
                newCustomer.bottles = number;
                num_bottles -= number;
                newPurchased = 1;
            }
            else {
                printf("Sorry ");
                StringPrint(&newCustomer.name);
                printf(", we only have %d Bottles\n", num_bottles);
            }
        }
        else if (StringIsEqualTo(&item, &diapers)) {
            if (num_diapers >= number) {
                newCustomer.diapers = number;
                num_diapers -= number;
                newPurchased = 1;
            }
            else {
                printf("Sorry ");
                StringPrint(&newCustomer.name);
                printf(", we only have %d Diapers\n", num_diapers);
            }
        }
        else if (StringIsEqualTo(&item, &rattles)) {
            if (num_rattles >= number) {
                newCustomer.rattles = number;
                num_rattles -= number;
                newPurchased = 1;
            }
            else {
                printf("Sorry ");
                StringPrint(&newCustomer.name);
                printf(", we only have %d Rattles\n", num_rattles);
            }
        }
        if (newPurchased) {
            customers[num_customers] = newCustomer;
            num_customers++;
        }
    }
    StringDestroy(&bottles);
    StringDestroy(&diapers);
    StringDestroy(&rattles);

}
/*
 * Prints out the current amount of each item in the inventory
 * Also prints out who has purchased the most amount of each item
 * Will show if a certain item has not been purchased
 * Only accepts command syntax:
 * Summarize
 */
void processSummarize() {
    Customer* max_bottles = largestItemCustSrch(BOTTLES);
    Customer* max_diapers = largestItemCustSrch(DIAPERS);
    Customer* max_rattles = largestItemCustSrch(RATTLES);
    printf("There are %d Bottles, %d Diapers and %d Rattles in inventory\n", num_bottles, num_diapers, num_rattles);
    printf("we have had a total of %d different customers\n", num_customers);
    if (max_bottles->bottles == 0) {
        printf("no one has purchased any Bottles");
    }
    else {
        StringPrint((&max_bottles->name));
        printf(" has purchased the most Bottles (%d)\n", max_bottles->bottles);
    }
    if (max_diapers->diapers == 0) {
        printf("no one has purchased any Diapers");
    }
    else {
        StringPrint((&max_diapers->name));
        printf(" has purchased the most Diapers (%d)\n", max_diapers->diapers);
    }
    if (max_rattles->rattles == 0) {
        printf("no one has purchased any Rattles");
    }
    else {
        StringPrint((&max_rattles->name));
        printf(" has purchased the most Rattles (%d)\n", max_rattles->rattles);
    }
}

/*
 * Reads the customer's name, the item he or she wants to purchase,
 * and the amount they want to buy
 * Based on the inputs, the transaction will either go through or fail
 * Can only read the command syntax:
 * Purchase <name> <type> #
 */
void processPurchase() {

    String item;
    int number;
    readString(&customerName);
    readString(&item);
    readNum(&number);
    customerPurchase(item, number);
    StringDestroy(&item);
    StringDestroy(&customerName);


}

/*
 * Increments the amount of the given item based on the command.
 * The amount cannot be negative
 * Can only read the following command syntax:
 * Inventory <type> #
 */
void processInventory() {

    String type;
    int number;
    String bottles = StringCreate("Bottles");
    String diapers = StringCreate("Diapers");
    String rattles = StringCreate("Rattles");
    readString(&type);
    readNum(&number);
    if (StringIsEqualTo(&type, &bottles)) {
        num_bottles += number;
    }
    else if (StringIsEqualTo(&type, &diapers)) {
        num_diapers += number;
    }
    else if (StringIsEqualTo(&type, &rattles)) {
        num_rattles += number;
    }
    StringDestroy(&bottles);
    StringDestroy(&diapers);
    StringDestroy(&rattles);
    StringDestroy(&type);

}
