#include <stdio.h>
#include <assert.h>

#define SIZE 26

int are_anagrams(char[], char[]);
void string_length_sum(char word[], int* length, int* sum);

int main() {
    printf("running test 'iceman' and 'cinema'...\n");
    char iceman[] = "iceman";
    char cinema[] = "cinema";

    assert(are_anagrams(iceman, cinema));
    printf("passed!\n");

    printf("running test 'raganam' and 'anagram'...\n");
    char raganam[] = "raganam";
    char anagram[] = "anagram";

    assert(are_anagrams(raganam, anagram));
    printf("passed!\n");
    
    printf("running test 'listen' and 'silent'...\n");
    char listen[] = "listen";
    char silent[] = "silent";

    assert(are_anagrams(listen, silent));
    printf("passed!\n");

    printf("running test 'mole' and 'meal'...\n");
    char mole[] = "mole";
    char meal[] = "meal";

    assert(!are_anagrams(mole, meal));
    printf("passed!\n");

    printf("running test 'happy' and 'sad'...\n");
    char happy[] = "happy";
    char sad[] = "sad";

    assert(!are_anagrams(happy, sad));
    printf("passed!\n");
}

int are_anagrams(char first_word[], char second_word[]) {
    int firstLength = 0;
    int secondLength = 0;
    int sum1 = 0;
    int sum2 = 0;
    int* pFirstLength = 0;
    int* pSecondLength = 0;
    int* pSum1 = 0;
    int* pSum2 = 0;
    pFirstLength = &firstLength;
    pSecondLength = &secondLength;
    pSum1 = &sum1;
    pSum2 = &sum2;
    string_length_sum(first_word, pFirstLength, pSum1);
    string_length_sum(second_word, pSecondLength, pSum2);
    if(firstLength == secondLength && sum1 == sum2){
        return 1;
    }
    return 0;
}

void string_length_sum(char word[], int* pLength, int* pSum){
    int distance = 0;
    int index = 0;
    int total = 0;
    while(word[index] != '\0'){
        distance++;
        total += word[index];
        index++;
    }
    *pLength = distance;
    *pSum = total;
}