#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORDS 200
#define MAX_LINES 128

// Function to parse the input and place each word into an array
int parseInput(char* input, char* words[]) {
    int i = 0;
    char* token = strtok(input, " \n");
    while (token != NULL && i < MAX_WORDS) {
        words[i++] = token;
        token = strtok(NULL, " \n");
    }
    return i; // Return the number of words
}

// Function to read all words from the array and perform some check
void readAndCheckWords(char* words[], int numWords) {
    for (int i = 0; i < numWords; i++) {
        // For demonstration, simply print each word
        printf("Word %d: %s\n", i + 1, words[i]);

        // Here, you can add any checks you want to perform on each word
        // For example, checking if a word is "true" (just as an example)
        if (strcmp(words[i], "true") == 0) {
            printf("The word '%s' is found at position %d\n", words[i], i + 1);
        }
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    char* words[MAX_WORDS];
    int numWords;

    for (int line = 0; line < MAX_LINES; line++) {
        printf(">> ");
        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
            break;
        }

        if (strncmp(input, "exit", 4) == 0 && (input[4] == '\n' || input[4] == '\0')) {
            break;
        }

        numWords = parseInput(input, words);

        // Call the function to read and check words after parsing
        readAndCheckWords(words, numWords);
    }

    return 0;
}
