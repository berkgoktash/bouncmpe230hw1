#include "entities.h"
#include "actions.h"
#include "parse.h"
#include "questions.h"
#include "conditions.h"

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

        // Call the function to read and check words after parsing
        readAndCheckWords(words, numWords);
        

        int start = 0;
        int* currentIndex = &start;
        int count = 0;
        int* sentenceCount = &count;
        Sentence** sentences;
        /*
        globalreader(sentences, 0, ...)
        false -> invalid
        true -> realizeactions
        */


        //Subject* s1 = findOrCreateSubject(words[0]);
        //Subject* s2 = findOrCreateSubject(words[1]);
        //Subject* s3 = findOrCreateSubject(words[2]);
        
        
        /*
        Subject** goers = malloc(2*sizeof(Subject*));
        goers[0] = s1;
        goers[1] = s2;

        goTo(goers, 2, words[2]);
        goTo(goers, 2, words[3]);
        whoAt(words[2]);
        if (at(goers, 2, words[3])) printf("they are at %s\n", s1->currentLocation->locationName);
        */

        
        /*
        Subject** buyers = malloc(2*sizeof(Subject*));
        buyers[0] = s1;
        buyers[1] = s2;

        const char** items = malloc(2*sizeof(char*));
        items[0] = words[2];
        items[1] = words[3];
        buy(buyers, 2, items, 2, 5);
        totalInventory(s1);
        if (has(buyers, 2, items, 2, 5)) printf("true\n");

        Subject** buyer2 = malloc(1*sizeof(Subject*));
        buyer2[0] = s2;
        buyFrom(buyer2, 1, s1, items, 2, 5);
        totalInventory(s1);
        totalInventory(s2);
        if (hasLessThan(buyers, 2, items, 2, 5)) printf("true\n");
        */

    }
    


    return 0;
}
