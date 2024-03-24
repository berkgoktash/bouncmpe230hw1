#ifndef PARSE_H
#define PARSE_H

#include "entities.h"
#include "actions.h"
#include "conditions.h"

#define MAX_INPUT_LENGTH 1024
#define MAX_WORDS 300
#define MAX_LINES 128

Location** allLocations = NULL; // Define and initialize the global locations array
int allLocationsCount = 0; // Define and initialize the global count of locations
Subject** allSubjects = NULL; // Define and initialize the global subjects array
int allSubjectsCount = 0; // Define and initialize the global count of subjects


char input[MAX_INPUT_LENGTH];
char* words[MAX_WORDS];
int numWords;

const char* allKeywords[] = {
    "sell", "buy", "go", "to", "from", "and", "at", "has", "if",
    "less", "more", "than", "exit", "where", "total", "who",
    "NOBODY", "NOTHING", "NOWHERE"
} ;
int numAllKeywords = 19;

const char* targetKeywords[] = {
    "sell", "buy", "go", "to", "from", "at", "has", "if",
    "less", "more", "than"
} ;
int numTargetKeywords = 11;

const char* actionKeywords[] = {
    "sell", "buy", "go"
} ;
int numActionKeywords = 3;


// Function to check if a string is any keyword
bool isAnyKeyword(const char* str) {
    for (int i = 0; i < numAllKeywords; ++i) {
        if (strcmp(allKeywords[i], str) == 0) {
            return true; // Found a match
        }
    }
    return false; // No match found
}

// Function to check if a string is target keyword
bool isTargetKeyword(const char* str) {
    for (int i = 0; i < numTargetKeywords; ++i) {
        if (strcmp(targetKeywords[i], str) == 0) {
            return true; // Found a match
        }
    }
    return false; // No match found
}

// Function to check if a string is action keyword
bool isActionKeyword(const char* str) {
    for (int i = 0; i < numTargetKeywords; ++i) {
        if (strcmp(actionKeywords[i], str) == 0) {
            return true; // Found a match
        }
    }
    return false; // No match found
}

char* nextKeyword(int start) {
    for (int i = start; i < numWords; ++i) {
        if (isTargetKeyword(words[i])) return words[i];
    }
    return NULL;
}

// Function to check if the string is numeric
bool validQuantity(const char* str) {
    // Check for a NULL pointer or an empty string
    if (str == NULL || *str == '\0') return false;

    for (const char* p = str; *p; p++) {
        if (!isdigit((unsigned char)*p)) {
            return false; // Found a non-digit character
        }
    }
    // If the code gets here, the string is all digits
    return true;
}




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


bool globalReader(Sentence** sentences, int* sentenceCount, int* currentIndex, int sentenceType) {
    if (sentenceType == -1) return false; // Invalid sentence

    if (sentenceType == 2) return true; // End of sentence


    sentences = realloc(sentences, (*sentenceCount + 1) * sizeof(Sentence*));

    if (sentenceType == 0) { // Call action reader
        Sentence* newSentence = malloc(sizeof(Sentence));
        sentences[(*sentenceCount)++] = newSentence;
        newSentence->type = 0;
        return globalReader(sentences, sentenceCount, currentIndex, actionReader());
    }

    if (sentenceType == 1) { // Call condition reader
        Sentence* newSentence = malloc(sizeof(Sentence));
        sentences[(*sentenceCount)++] = newSentence;
        newSentence->type = 1;
        return globalReader(sentences, sentenceCount, currentIndex, conditionReader());
    } 

}

int actionReader(Sentence* sentence, int* currentIndex) { // Read Sentences
    bool foundVerb = false;
    int i = 0;
    while (*currentIndex < numWords) {
        sentence->subjects = realloc(sentence->subjects, (sentence->subjectCount + 1) * sizeof(Subject*));
        if (i % 2 == 0) { // Search for a subject
            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) ) {
                Subject* currentSubject = findOrCreateSubject(words[*currentIndex]);
                sentence->subjects[sentence->subjectCount] = currentSubject;
                sentence->subjectCount++;
                i++;
                (*currentIndex)++;
            }
            else {
                return -1;
            }
        }
        else { // Search for an action verb or "and"
            if (strcmp(words[*currentIndex], "and") == 0) {
                i++;
                (*currentIndex)++;
            }
            else if (strcmp(words[*currentIndex], "sell") == 0) {
                (*currentIndex)++;
                i = 0;
                char* nextKeyword = nextKeyword(*currentIndex);
                if (!nextKeyword && strcmp(words[*currentIndex], "to") == 0) { // Sell-to block
                    strcpy(sentence->verb, "sellTo");
                    while(*currentIndex < numWords) {
                        if (i % 3 == 0) { // Search for a quantity
                            if (validQuantity(words[*currentIndex])) {
                                sentence->quantities = realloc(sentenceCount, (sentence->quantityCount + 1) * sizeof(int));
                                sentence->quantities[sentence->quantityCount] = atoi(words[*currentIndex]);
                                sentence->quantityCount++;
                                i++;
                                (*currentIndex)++;
                            }
                            else {
                                return -1;
                            }
                        }
                        else if (i % 3 == 1) { // Search for item
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex])) {
                                char* currentItem = words[*currentIndex];
                                sentence->items[sentence->itemCount] = currentItem;
                                sentence->itemCount++
                                i++;
                                (*currentIndex)++;
                            }
                            else {
                                return -1;
                            }
                        }
                        else { // Search for "and" or "to"
                            if (strcmp(words[*currentIndex], "and") == 0) {
                                i++;
                                (*currentIndex)++;
                            }
                            else if (strcmp(words[*currentIndex], "to" ) == 0 && currentIndex + 1 < numWords) {
                                i++;
                                (*currentIndex)++;
                                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) ) {
                                    Subject* otherSubject = findOrCreateSubject(words[*currentIndex]);
                                    sentence->other = otherSubject;
                                }
                                else { return -1; }
                        
                                if (*currentIndex + 1 < numWords) {
                                    i++;
                                    (*currentIndex)++;
                                    if (strcmp(words[*currentIndex], "and" && currentIndex + 1 < numWords ) == 0) {
                                        i++;
                                        (*currentIndex)++;
                                        return 0;
                                    }
                                    else if (strcmp(words[*currentIndex], "if" && currentIndex + 1 < numWords ) == 0) {
                                        i++;
                                        (*currentIndex)++;
                                        return 1;
                                    }
                                    else { return -1; }

                                }
                                else { return 2; }
                            }
                            else {
                                return -1;
                            }
                        }
                    }
                }

            
                else { // End of current or whole sentence, Sell block
                    strcpy(sentence->verb, "sell");
                    while(*currentIndex < numWords) {
                        if (i % 3 == 0) { // Search for a quantity
                            if (validQuantity(words[*currentIndex])) {
                                sentence->quantities = realloc(sentenceCount, (sentence->quantityCount + 1) * sizeof(int));
                                sentence->quantities[sentence->quantityCount] = atoi(words[*currentIndex]);
                                sentence->quantityCount++;
                                i++;
                                (*currentIndex)++;
                            }
                            else {
                                if (sentence->quantityCount == 0) { // If there is no quantity at first
                                    return -1;
                                }
                                else {
                                    return 0;
                                }
                            }
                        }
                        else if (i % 3 == 1) { // Search for item
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex])) {
                                char* currentItem = words[*currentIndex];
                                sentence->items[sentence->itemCount] = currentItem;
                                sentence->itemCount++
                                i++;
                                (*currentIndex)++;
                            }
                            else {
                                return -1;
                            }
                        }
                        else { // Search for "and" or "if"
                            if (strcmp(words[*currentIndex], "and") == 0 && currentIndex + 1 < numWords) {
                                i++;
                                (*currentIndex)++;
                            }
                            else if (strcmp(words[*currentIndex], "if" ) == 0 && currentIndex + 1 < numWords) {
                                i++
                                (*currentIndex)++;
                                return 1; // Next is the condition sentence
                            }
                            else {
                                return -1;
                            }
                        }
                        if (sentence->quantityCount != sentence->itemCount) {
                            foundVerb = false;
                        }
                        else {
                            foundVerb = true;
                        }
                    }
                }
            }

            else if (strcmp(words[*currentIndex], "buy") == 0) {
            
                char* nextKeyword = nextKeyword(*currentIndex);
                if (!nextKeyword && strcmp(words[*currentIndex], "from") == 0) { // Buy-from block

                }
                else { // End of current or whole sentence, Buy block
                
                }
            }
            
            else if (strcmp(words[*currentIndex], "go") == 0) {

            }
            else {
                return -1;
            }

        }
    }

    if (!foundVerb) {
        return -1;
    }
    return 2;
} 



#endif // PARSE_H