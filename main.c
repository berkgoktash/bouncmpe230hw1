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

        (*currentIndex) = 0;
        (*sentenceCount) = 0;
        sentences = NULL;
    
        

        if (globalReader(0)) {
            printf("TRUE\n");
            Sentence* sentence1 = sentences[0];
            Subject* subject1 = sentence1->subjects[0];
            Subject* subject2 = sentence1->subjects[1];
            Subject* subject3 = sentence1->other;
            printf("Subject1 : %s\n", subject1->name);
            printf("Subject2 : %s\n", subject2->name);
            printf("Subject3 : %s\n", subject3->name);
            printf("Verb : %s\n", sentence1->verb);
            printf("Item1 : %s\n", sentence1->items[0]);
            printf("Item2 : %s\n", sentence1->items[1]);

            Sentence* sentence2 = sentences[1];
            Subject* subject4 = sentence2->subjects[0];
            Subject* subject5 = sentence2->subjects[1];
            printf("Subject4 : %s\n", subject4->name);
            printf("Subject5 : %s\n", subject5->name);
            printf("Verb : %s\n", sentence2->verb);
            printf("Location : %s\n", sentence2->location);


        }
        else {
            printf("INVALID\n");

        }
        /*
        else { 
            printf("%lu\n", sizeof(sentences));
            Sentence* sentence1 = sentences[0];
            printf("%s\n", "INVALID"); 
            Subject* subject1 = sentence1->subjects[0];
            Subject* subject2 = sentence1->subjects[1];
            printf("Subject1 : %s\n", subject1->name);
            printf("Subject2 : %s\n", subject2->name);

        } */




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
