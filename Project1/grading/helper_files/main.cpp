#include <stdio.h>
#include "Helpers.h"

char dictionary[1000000];
char article[100000];

void spellCheck(char[], char[]);

int main(int argc, char *argv[])
{
    int ret;
    ret = setvbuf(stdout, NULL, _IONBF, 0);


    FILE* dict_file;
    FILE* article_file;
    int bytes_read;
    char* p;

    dict_file = fopen( "dictionary.txt", "r");
    if (dict_file == 0)
    {
        printf("unable to open dictionary file %s\n", argv[1]);
        return -1;
    }

    article_file = fopen("article.txt", "r");
    if (article_file == 0)
    {
        printf("ERROR: Unable to open file %s\n", argv[2]);
        return -1;
    }

    /* read dictionary */
    p = dictionary;
    p = fgets(p, 100, dict_file);
    while (p != 0)
    {
        while (*p != '\0')
        {
            p += 1;
        }
        p = fgets(p, 100, dict_file);
    }

    /* read article */
    p = article;
    bytes_read = fread(p, 1, 1000, article_file);
    p += bytes_read;
    while (bytes_read != 0)
    {
        bytes_read = fread(p, 1, 1000, article_file);
        p += bytes_read;
    }
    *p = 0;

    spellCheck(article, dictionary);

    return 0;
}
