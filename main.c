#include "parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {

    for (int line = 0; line < MAX_LINES; line++) {
        printf(">> ");
        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
            break;
        }

        numWords = parseInput(input, words);

        if (numWords == 1 && strcmp(words[0], "exit") == 0) break;

        (*currentIndex) = 0;
        (*sentenceCount) = 0;
        sentences = NULL;

        if (strcmp(words[numWords - 1], "?") == 0) {
            questionReader();
        }
        else {
            if (globalReader(0)) {
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
