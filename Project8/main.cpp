#include <iostream>
#include "Parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>
#include "Text.h"
#include "Symbols.h"


using namespace std;

void input(void) {
    read_next_token();
    if(next_token_type == NAME || next_token_type == SYMBOL) {
        string next = next_token();
        if(next == "//") {
            skip_line();
        }
        else if(next_token_type == NAME) {
            if(next == "text") {
                cmdText();
            }
            else if(next == "var") {
                cmdVar();
            }
            else if(next == "set") {
                cmdSet();
            }
            else if(next == "output") {
                cmdOut();
            }
        }
    }
}

void run(void) {
    next_token_type = NUMBER;
    while(next_token_type != END) {
        input();
    }
    destroyTable();
}

int main(void){
    //set_input("test4.blip");
    //run();
	set_input("test_grader.blip");
	run();
}
