// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dictionary.h"

//2CHECK
// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    printf("%s\n", word);
    return false;
}

//1LOAD (open for reading ie/ use the same concepts from recover)
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // char *dictionary is address of the starting point of the dictionary file
    // FILE *dict is the address of the pointer of the file to open for reading
    // fopen() opens the file for reading
    FILE *dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        printf("Unable to open %s", dictionary);
        return false;
    }
    else
    {
        printf("Opened the file");
        return true;
    }

    fclose(dict);
}

//3SIZE
// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

//4UNLOAD
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    return false;
}
