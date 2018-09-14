/*
 * Name:
 * EID:
 * Project 7, CRM part 2
 * EE 312, Spring 2017
 */
#include <iostream>
#include <cassert>
#include "String.h"
#include "CustomerDB.h"

using namespace std;

const int default_capacity = 1;

Customer::Customer(String name) { // done, please do not edit
	this->bottles = 0;
	this->rattles = 0;
	this->diapers = 0;
	this->name = name;
}

CustomerDB::CustomerDB(void) { // done, please do not edit
	this->capacity = default_capacity;
	this->data = new Customer[this->capacity];
	this->length = 0;
}

int CustomerDB::size(void) { // done, please do not edit
	return this->length;
}

CustomerDB::~CustomerDB(void) { // done, please do not edit
	delete[] this->data;
}

void CustomerDB::clear(void) { // done, please do not edit
	delete[] this->data;
	this->capacity = default_capacity;
	this->data = new Customer[this->capacity];
	this->length = 0;
}

Customer& CustomerDB::operator[](int k) { // done, please do not edit
	assert(k >= 0 && k < this->length);
	return this->data[k];
}

/* assumes that there the customer is already in the data base */
Customer& CustomerDB::operator[](String name) {
	Customer data[this->length];
	int index;
	for(index = 0; index < this->length; index++) {
		if(this->data[index].name == name) {
			return this->data[index];
		}
		data[index] = this->data[index];
	}
	if(this->length == this->capacity) {
		this->capacity *= 2;
		delete[] this->data;
		this->data = new Customer[this->capacity];
		for(index = 0; index < this->length; index++) {
			this->data[index] = data[index];
		}
	}
	this->data[index] = Customer(name);
	this->length++;
	return this->data[index];
}

bool CustomerDB::isMember(String name) {
	for(int index = 0; index < this->length; index++) {
		if(this->data[index].name == name) {
			return true;
		}
	}
	return false;
}
