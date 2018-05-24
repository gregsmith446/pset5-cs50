// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// create a node data type to hold each word in our dictionary
// allocate space for each node to hold the max of 45 letters and \0
// each node has a pointer to the next node
// last node has a pointer to NULL
// after initiating the node data type, call the node to create one
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;


#define HASH_SIZE 200000
// Pointer array for nodes called hash table with ~10,001 spots
node *hash_table[HASH_SIZE];

// counter for total words in dictionary

int total_words = 0;

// HASH FUNCTION
// Input = a word from the dictionary
// Output = the length of the word multiplied by some exponents % HASH_SIZE
// From (http://www.partow.net/programming/hashfunctions/#StringHashing)
   int hash_index(const char *word)
{
    unsigned int length = strlen(word);
    unsigned int hash = 1315423911;
    unsigned int i    = 0;

   for (i = 0; i < length; ++word, ++i)
   {
      hash ^= ((hash << 5) + (*word) + (hash >> 2));
   }

   return hash % HASH_SIZE;
}

//2CHECK
// Returns true if word is in dictionary, else reuturns false
// VALGRIND --> bool has 3 exit points (returns)
// At all 3 exit points, memory has to be be freed to stop memory leaks
bool check(const char *word)
{
    // initialize a char* to hold a lowercase version of all words from text
    char *lower_case;
    // malloc space for holding the lowercase words w/ +1 being '\0'
    lower_case = malloc((strlen(word) * sizeof(char)) + 1);
    // copy the now lower_case word to word
    strcpy(lower_case, word);

    // loop through all the words
        for (int i = 0; i < strlen(word); i++) {
            lower_case[i] = tolower(lower_case[i]);
        }

    //Get the index in the now lowercase word in array and set = to head
    int head = hash_index(lower_case);

    // if the head node is equal to NULL,
    // this means the word is not in the dictionary
    if (hash_table[head] == NULL)
    {
        if(lower_case)
        {
            free(lower_case);
        }
        return false;
    }

    node *position = hash_table[head];

    while (position != NULL)
    {
        char *dictionary_word = position->word;

        if (strcmp(lower_case, dictionary_word) == 0)
        {
            if(lower_case)
                {
                    free(lower_case);
                }
            return true;
        }

        position = position->next;
    }

    if (lower_case)
    {
        free(lower_case);
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // char *dictionary is address of the starting point of the dictionary file
    // FILE *file is the address of the pointer of the file stream to open for reading
    // fopen() opens the file for reading

    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    //Will hold the words pulled from dictionary file
    char word[LENGTH + 1];

    // read the dictionary contents word by word, creating a new node for each word
    while (fscanf(file, "%s", word) != EOF) //scan 1 word at a time until end of file
    {
        // allocate space for a new node size of node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            total_words = 0;
            return false;
        }
        // copy the word into the new node
        else
        {
            strcpy(new_node->word, word);
        }

        total_words++;

        // Place word in the list by inserting at the head
        int head = hash_index(word);
        new_node->next = hash_table[head];
        hash_table[head] = new_node;
    }

    fclose(file);

    return true;
}

//3SIZE
// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return total_words;
}

//4UNLOAD
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Go to each index in the array
    for (int i = 0; i < HASH_SIZE; i++)
    {
        // Set current node to head of list
        node *current_node = hash_table[i];
        // Walk through the list, free nodes until reaching end
        while (current_node != NULL)
        {
            node *next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }
    }
    return true;
}
