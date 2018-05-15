# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

"a word invented by the president of the National Puzzlers' League as a synonym for the disease known as silicosis."
"...an artificial long word said to mean a lung disease caused by inhaling very fine ash and sand dust."
It is the longest word in the English language published in a dictionary.

## According to its man page, what does `getrusage` do?

getrusage - get information about resource utilization/getrusage/get resource usage

getrusage() returns resource usage measures for who, which can be one of the following:

1.) RUSAGE_SELF
2.) RUSAGE_CHILDREN
3.) RUSAGE_THREAD

## Per that same man page, how many members are in a variable of type `struct rusage`?

There are 16 members in the variable of type 'struct rusage'

struct rusage {
    struct timeval ru_utime; /* user CPU time used */
    struct timeval ru_stime; /* system CPU time used */
    long   ru_maxrss;        /* maximum resident set size */
    long   ru_ixrss;         /* integral shared memory size */
    long   ru_idrss;         /* integral unshared data size */
    long   ru_isrss;         /* integral unshared stack size */
    long   ru_minflt;        /* page reclaims (soft page faults) */
    long   ru_majflt;        /* page faults (hard page faults) */
    long   ru_nswap;         /* swaps */
    long   ru_inblock;       /* block input operations */
    long   ru_oublock;       /* block output operations */
    long   ru_msgsnd;        /* IPC messages sent */
    long   ru_msgrcv;        /* IPC messages received */
    long   ru_nsignals;      /* signals received */
    long   ru_nvcsw;         /* voluntary context switches */
    long   ru_nivcsw;        /* involuntary context switches */
};


## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

Passing by reference is preferrable with 'calculate' because passing by value would take more time and memory.
Passing structs by value means that copies of the structs are being made.
With these copies we risk a stack overflow, because the structs copied become too large.
In 'calculate', we would be passing a double, which can hold 15-16 digits.
Considering the downside to passing by value, passing by reference here is just the better option.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file.
## In other words, convince us that you indeed understand how that function's `for` loop works.

'main' reads chars from the given text file via a for loop that contains 'if' statementsm 'else if' statements, and 'while' loops as well.

The loop adds valid chars from the text file to a word array until the word is complete, terminating the string with a '\0'.
It then checks to see if the just terminated word is in the dictionary (spell checks it + printf() if misspelled)
After it checks the word, it then repeats the process to look for other valid words, checking each of their spelling,
until the end of the file has been reached.

This is a breakdown of the logic for finding 'valid' words
1.) If the char is alphabetical or an apostrophe not at index 0, the char is added to the word array
2.) Else if the char is numerical, the whole word is ignored and skipped
3.) Else if there is a space or a form of punctuation, the program recognizes the end of the word and
terminates it by adding '\0' to the word array.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use
`fscanf` with a format string like `"%s"` to read whole words at a time?
## Put another way, what problems might arise by relying on `fscanf` alone?

I think 'fgetc' was used because it is the better option in this situation.
The program has more control for reading each char and that gives us the ability to look for the correct
input (letters and apostrophes, but not numbers).

'fscanf' doesn't read chars, but reads whole words at a time until coming across a space, newline, or tab.
That could complicate reading words at the end of a sentence.  Those words would have a period or comma included,
and when checked would come back as misspelled.  You would need to adjust the code to account for this and
it would be quite difficult.

Also, using 'fscanf' could cause a memory overflow in a case where the text file being read had a really long word
without spaces, a newline, or tab to end it, whereas this possible vulnerability is prevented with 'fgetc'.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

"Constants are treated just like regular variables except that their values cannot be modified after their definition." (tutorialpoint.com)

Declaring the parameters 'check' or 'load' as const is essential here because using const prevents changes to the word
from the text file and the content of the dictionary being pointed to.  We only want to change where to point to the data
for each spell check but not change it.
