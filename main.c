/*
Project1 - Favor for the Ringmaster (An interpreter using C for Tolkien)
Bora Aydemir - Berk Göktaş
2021400054 - 2022400039
*/

#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {

    for (int line = 0; line < MAX_LINES; line++) {
        printf(">> "); // Expect an input
        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
            break;
        }

        numWords = parseInput(input, words); // Parse the input and determine the word count

        if (numWords == 1 && strcmp(words[0], "exit") == 0) break; // Quit program when "exit" is entered

        (*currentIndex) = 0;
        (*sentenceCount) = 0;
        sentences = NULL;

        if (strcmp(words[numWords - 1], "?") == 0) { // Call question reader if input is a question
            questionReader();
        }
        else {
            if (globalReader(0)) { // Call global reader if input is a sentence
                processSentences();
                printf("OK\n");
            }
            else {
                printf("INVALID\n");
            }
        }

    }
    
    return 0;
}
