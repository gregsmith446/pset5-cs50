// Implements a spell-checker

#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "dictionary.h"

// Undefine any definitions
#undef calculate
#undef getrusage

// Default dictionary
#define DICTIONARY "dictionaries/large"

// Prototype for returning number of seconds between b and a
// double data type is an double-precision, 64-bit floating-point data type. It accommodates 15 to 16 digits.
double calculate(const struct rusage *b, const struct rusage *a);

int main(int argc, char *argv[])
{
    // Check for correct number of cmd line args
    if (argc != 2 && argc != 3)
    {
        printf("Usage: speller [dictionary] text\n");
        return 1;
    }

    // Structures for holding timing data
    struct rusage before, after;

    // Benchmarks --> how the process will be timed
    double time_load = 0.0, time_check = 0.0, time_size = 0.0, time_unload = 0.0;

    // Determine dictionary to use
    // if user supplies one in argv1, use that, if not use the default dictionary, "dictionaries/large", defined above
    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    // Load dictionary
    // get the time before loading
    getrusage(RUSAGE_SELF, &before);

    // if loaded is true
    bool loaded = load(dictionary);

    // get the time after loading
    getrusage(RUSAGE_SELF, &after);

    // Exit if dictionary not loaded
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }

    // Calculate time to load dictionary
    // take the 2 times from above, run it through calculate, then assign it to time_load
    time_load = calculate(&before, &after);

    // Try to open text file (to spell check)
    // the text file will be in argv[1] or argv[2] depending on if the user supplies a dictionary in cmd line
    // if the argc is 3 long, the user supplied a dictionary, so open argv[2] for the text file
    // else, if the user did not provide dictionary, argv[1] is the text
    char *text = (argc == 3) ? argv[2] : argv[1];  //char *text = (Condition) ? value_if_true : value_if_false;
    // open the text file stream for reading
    FILE *file = fopen(text, "r");
    //if there is no file at pointer location
    if (file == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    // Prepare to report misspellings
    printf("\nMISSPELLED WORDS\n\n");

    // Prepare to spell-check
    // index counter starts at 0 as do misspellings and words
    int index = 0, misspellings = 0, words = 0;
    // word is equal to 45 + 1, LENGTH defined in dictionary.h
    char word[LENGTH + 1];

    // Spell-check each word in text, aka, strategically loop through supplied text file to see if in dictionary
    // fgetc() reads the next character from stream and returns it as an unsigned char cast to an int, or *EOF\* on end of file or error.
    // intc set = to the stream for the supplied file
    // As long as c doesn't = the EndOfFile, c will stay = to the stream of the file
    for (int c = fgetc(file); c != EOF; c = fgetc(file))
    {
        // Allow only alphabetical characters and apostrophes
        // if c is a alphabetical char or c is an apostrophe ' that is not at index 0
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Append character to word[] array
            word[index] = c; // add the valid char the the word[] array
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while ((c = fgetc(file)) != EOF && isalpha(c));

                // Prepare for new word
                index = 0;
            }
        }

        // Ignore words with numbers using isdigit() (like MS Word can)
        else if (isdigit(c))
        {
            // Consume remainder of alphanumeric string
            while ((c = fgetc(file)) != EOF && isalnum(c));

            // Prepare for new word
            index = 0;
        }

        // At this point in checking, we have found a whole word
        else if (index > 0)
        {
            // In that case, terminate current word by adding '\0' to the end
            word[index] = '\0';

            // Update counter
            words++;

            // Check the just terminated word's spelling against the dictionary
            getrusage(RUSAGE_SELF, &before);
            bool misspelled = !check(word);
            getrusage(RUSAGE_SELF, &after);

            // Update benchmark
            time_check += calculate(&before, &after);

            // Print word if misspelled
            if (misspelled)
            {
                printf("%s\n", word);
                misspellings++;
            }

            // Prepare for next word by resetting index to 0
            index = 0;
        }
    }

    // Check whether there was an error reading the file, close the file, tell the user, unload() memory, & return 1;
    if (ferror(file))
    {
        fclose(file);
        printf("Error reading %s.\n", text);
        unload();
        return 1;
    }

    // If no error, the process is complete at this point
    // Close text file
    fclose(file);

    // Determine dictionary's size
    getrusage(RUSAGE_SELF, &before);
    unsigned int n = size();
    getrusage(RUSAGE_SELF, &after);

    // Calculate time to determine dictionary's size
    time_size = calculate(&before, &after);

    // Unload dictionary
    getrusage(RUSAGE_SELF, &before);
    bool unloaded = unload();
    getrusage(RUSAGE_SELF, &after);

    // Abort if dictionary not unloaded
    if (!unloaded)
    {
        printf("Could not unload %s.\n", dictionary);
        return 1;
    }

    // Calculate time to unload dictionary
    time_unload = calculate(&before, &after);

    // Report benchmarks
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", time_load);
    printf("TIME IN check:        %.2f\n", time_check);
    printf("TIME IN size:         %.2f\n", time_size);
    printf("TIME IN unload:       %.2f\n", time_unload);
    printf("TIME IN TOTAL:        %.2f\n\n",
           time_load + time_check + time_size + time_unload);

    // Success
    return 0;
}

// Returns number of seconds between b and a
double calculate(const struct rusage *b, const struct rusage *a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}
