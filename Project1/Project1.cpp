/*
 * Project1.cpp
 * 
 * Name: Arkan Abuyazid
 * EE312 Spring 2016
 * SpellCheck
 */


#include <stdio.h> // provides declarations for printf and putchar
#include <stdint.h> // provides declarations for int32_t uint32_t and the other (new) standard C types

#define DEBUG 0
#define SIZE 100


/* All of your code must be in this file. Please no #includes other than standard system headers (ie.., stdio.h, stdint.h)
 *
 * Many students find it helpful to declare global variables (often arrays). You are welcome to use
 * globals if you find them helpful. Global variables are by no means necessary for this project.
 */

/* You must write this function (spellCheck). Do not change the way the function is declared (i.e., it has
 * exactly two parameters, each parameter is a standard (mundane) C string (see SpellCheck.pdf).
 * You are expected to use reasonable programming style. I *insist* that you indent
 * reasonably and consistently in your code. I strongly encourage you to avoid big functions
 * So, plan on implementing spellCheck by writing two or three other "support functions" that
 * help make the actual spell checking easier for you.
 * There are no explicit restrictions on using functions from the C standard library. However,
 * for this project you should avoid using functionality from the C++ standard library. You will
 * almost certainly find it easiest to just write everything you need from scratch!
 */

void spellCheck(char article[], char dictionary[]);
void printWord(char* word);
void differentiateDictionary(char dictionary[]);
void compareArticleDictionary(char dictionary[], int acceptable);
int differentiateArticle(char article[]);

int indexArticle = 0;
int indexDictionary = 0;
int indexWordArticle = 0;
int indexWordDictionary = 0;
char outputArticle[SIZE];
char outputDictionary[SIZE];

void printWord(char* word){

    int index = 0;
    while (*(word + index) != 0){
        printf("%c", *(word + index));
        index++;
    }
    printf("\n");
}

int differentiateArticle(char article[]) {

    int acceptable = 0;
    indexWordArticle = 0;              //index for output word

    while (article[indexArticle] != 0) {

        if ((article[indexArticle] < 'A') || ((article[indexArticle] > 'Z') && (article[indexArticle] < 'a')) || (article[indexArticle] >'z')){      //if the character isn't a letter
            indexArticle++;
            if (indexWordArticle == 0) {
                continue;
            } else {
                outputArticle[indexWordArticle] = 0;
                if(indexWordArticle >= 2) {
                    if (DEBUG) {printWord(outputArticle);}
                    acceptable = 1;
                }
                break;
            }
        }


        outputArticle[indexWordArticle] = article[indexArticle];
        indexArticle++;
        indexWordArticle++;

    }

    if((article[indexArticle] == 0) && (indexWordArticle >= 2)){
        outputArticle[indexWordArticle] = 0;
        acceptable = 1;
    }
    return acceptable;

}

void differentiateDictionary(char dictionary[]){

    indexWordDictionary = 0;

    while(dictionary[indexDictionary] != 0){

        if(dictionary[indexDictionary] == '\n'){
            outputDictionary[indexWordDictionary] = 0;
            if(DEBUG){
                printWord(outputDictionary);
            }
            indexDictionary++;
            break;
        }

        outputDictionary[indexWordDictionary] = dictionary[indexDictionary];

        indexWordDictionary++;
        indexDictionary++;
    }

    if(dictionary[indexDictionary] == 0) {
        outputDictionary[indexWordDictionary] = 0;
    }

    return;

}

void compareArticleDictionary(char dictionary[], int acceptable){

    int i;
    if(DEBUG){printf("%d", acceptable);}
    if(acceptable == 0){
        return;
    }
    while(dictionary[indexDictionary] != 0){
        differentiateDictionary(dictionary);
        for(i = 0; i <= indexWordDictionary; i++){
            if((outputDictionary[i] != outputArticle[i]) && (outputDictionary[i] != (outputArticle[i] + 32)) && (outputDictionary[i] != (outputArticle[i] - 32))){
                break;
            }
            if((outputDictionary[i] == outputArticle[i]) && (outputDictionary[i] == 0)){
                indexDictionary = 0;
                return;
            }
        }
    }
    printWord(outputArticle);
    indexDictionary = 0;
    return;
}

void spellCheck(char article[], char dictionary[]) {
    while(article[indexArticle] != 0) {
        compareArticleDictionary(dictionary, differentiateArticle(article));
    }
    indexArticle = 0;
    indexDictionary = 0;

}
