#include <stdio.h>
#include <string>
#include <stdio.h> // declarations of FILE and printf
#include <stdlib.h> // includes declaration of exit()
#include "String.h"
// #include "MemHeap.h"

#include "CustomerDB.h"
/*
struct CheckMemoryLeaks {
	~CheckMemoryLeaks(void) {
		if (! isSaneHeap()) {
			printf("oh goodness! you've corrupted the heap, naughty naughty\n");
			return;
		}
		if (! isEmptyHeap()) {
			printf("Uh Oh! you have a memory leak somewhere, better find it\n");
			return;
		}
		printf("The heap is all clean, good work!\n");
	}
} MemoryChecker;
*/
FILE* input_stream;
String inventory_cmd_name = String("Inventory");
String purchase_cmd_name = String("Purchase");
String summarize_cmd_name = String("Summarize");
String quit_cmd_name = String("Quit");

void readNum(int& x) {
	int val;
	fscanf(input_stream, "%d", &val);
	x = val;
}

void readString(String& x) {
	char buff[BUFSIZ];
	fscanf(input_stream, "%s", buff);
	x = String(buff);
}

void displayTailAndClose() {
	char* rval;
	char buff[BUFSIZ];
	
	rval = fgets(buff, BUFSIZ, input_stream);
	while (rval != 0) {
		printf(buff);
		rval = fgets(buff, BUFSIZ, input_stream);
	}
	fclose(input_stream);
}

void readInput(const char* file_name) {
	String next_command;
	bool all_done = false;
	
	/* open the file */
	input_stream = fopen(file_name, "r");
	if (input_stream == 0) {
		printf("sorry, I could not open %s for reading\n", file_name);
		exit(-1);
	}
	
	/* main loop, read commands from input file */
	do {
		readString(next_command);
		if (next_command == inventory_cmd_name) {
			processInventory();
		} else if (next_command == purchase_cmd_name) {
			processPurchase();
		} else if (next_command == summarize_cmd_name) {
			processSummarize();
		} else if (next_command == quit_cmd_name) {
			all_done = true;
		} else {
			printf("Encountered an error in input file\n");
			printf("The offending line has been ignored\n");
		}
	} while (! all_done);
	
	displayTailAndClose();
}

int main(void) {
	readInput("test1.txt");
	reset();
	readInput("test2.txt");
	reset();
	readInput("test3.txt");
	reset();
    readInput("test4.txt");
    reset();
    readInput("test5.txt");
    reset();
    readInput("test6.txt");
    reset();
    readInput("test7.txt");
    reset();
    return 0;
}
