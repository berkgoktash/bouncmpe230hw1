#ifndef PARSE_H
#define PARSE_H

#include "actions.h"
#include "conditions.h"
#include "questions.h"

// Some constraints
#define MAX_INPUT_LENGTH 1024
#define MAX_WORDS 400
#define MAX_LINES 128

int actionReader(Sentence* sentence, int* currentIndex);
int conditionReader(Sentence* sentence, int* currentIndex);

Location** allLocations = NULL; // Define and initialize the global locations array
int allLocationsCount = 0; // Define and initialize the global count of locations
Subject** allSubjects = NULL; // Define and initialize the global subjects array
int allSubjectsCount = 0; // Define and initialize the global count of subjects

// Some variables for parsing input
int start = 0;
int* currentIndex = &start;
int count = 0;
int* sentenceCount = &count;
Sentence** sentences = NULL;

char input[MAX_INPUT_LENGTH];
char* words[MAX_WORDS];
int numWords;

const char* allKeywords[] = { // Array of all possible keywords
    "sell", "buy", "go", "to", "from", "and", "at", "has", "if",
    "less", "more", "than", "exit", "where", "total", "who",
    "NOBODY", "NOTHING", "NOWHERE"
} ;
int numAllKeywords = 19;

const char* targetKeywords[] = { // Array of valid keywords in a sentence
    "sell", "buy", "go", "to", "from", "at", "has", "if",
    "less", "more", "than"
} ;
int numTargetKeywords = 11;

const char* actionKeywords[] = { // Array of action keywords
    "sell", "buy", "go"
} ;
int numActionKeywords = 3;

const char* questionKeywords[] = { // Array of question keywords
    "total", "where", "who"
} ;
int numQuestionKeywords = 3;


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
    for (int i = 0; i < numActionKeywords; ++i) {
        if (strcmp(actionKeywords[i], str) == 0) {
            return true; // Found a match
        }
    }
    return false; // No match found
}

// Function to check if a string is question keyword
bool isQuestionKeyword(const char* str) {
    for (int i = 0; i < numQuestionKeywords; ++i) {
        if (strcmp(questionKeywords[i], str) == 0) {
            return true; // Found a match
        }
    }
    return false; // No match found
}

// Function to get next keyword
char* findNextKeyword(int start) {
    for (int i = start; i < numWords; ++i) {
        if (isTargetKeyword(words[i])) return words[i];
    }
    return NULL;
}

// Function to find question keyword
char* findQuestionKeyword() {
    for (int i = 0; i < numWords; ++i) {
        if (isQuestionKeyword(words[i])) return words[i];
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

// Function to check if a subject already exists in the Sentence
bool subjectExists(Sentence* sentence, char* subjectName) {
    for (int i = 0; i < sentence->subjectCount; i++) {
        if (strcmp(sentence->subjects[i]->name, subjectName) == 0) {
            return true; // Found a subject with the same name
        }
    }
    return false; // No matching subject found
}

// Function to check if an item already exists in the Sentence
bool itemExists(Sentence* sentence, char* newItem) {
    for (int i = 0; i < sentence->itemCount; i++) {
        if (strcmp(sentence->items[i], newItem) == 0) {
            return true; // Found an item with the same name
        }
    }
    return false; // No matching item found
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


void questionReader() { // Function to parse a question
    char* questionKeyword = findQuestionKeyword();
    if (questionKeyword != NULL && strcmp(questionKeyword, "where") == 0) {  // Where block
        if (numWords == 3 && isValidString(words[0]) && !isAnyKeyword(words[0])) {
            Subject* askedSubject = findOrCreateSubject(words[0]);
            where(askedSubject);
        }
        else { printf("INVALID\n"); }
    }

    else if (questionKeyword != NULL && strcmp(questionKeyword, "who") == 0) {  // Who-at block
        if (numWords == 4 && strcmp(words[0], "who") == 0 && strcmp(words[1], "at") == 0 && isValidString(words[2]) && !isAnyKeyword(words[2])) {
            whoAt(words[2]);
        }
        else { printf("INVALID\n"); }
    }

    else if (questionKeyword != NULL && strcmp(questionKeyword, "total") == 0) {  // Total block
        if (numWords == 3) { // Total-inventory block
            if (isValidString(words[0]) && !isAnyKeyword(words[0])) {
                Subject* askedSubject = findOrCreateSubject(words[0]);
                totalInventory(askedSubject);
            }
            else { printf("INVALID\n"); }

        }
        else if (numWords > 3 && strcmp(words[numWords - 3], "total") == 0 && isValidString(words[numWords - 2]) && !isAnyKeyword(words[numWords - 2])) { // Sum-item block
            Subject** askedSubjects = NULL;
            int askedSubjectsCount = 0;
            char* itemName = words[numWords - 2];

            for (int i = 0; i < numWords - 3; i++) {
                bool askedBefore = false;
                if (i % 2 == 0) { // Search for a subject
                    askedSubjects = realloc(askedSubjects, (askedSubjectsCount + 1) * sizeof(Subject*));
                    for (int j = 0; j < askedSubjectsCount; j++) {
                        if (strcmp(askedSubjects[j]->name, words[i]) == 0) {
                            askedBefore = true;
                        }
                    }

                    if (isValidString(words[i]) && !isAnyKeyword(words[i]) && !askedBefore) {
                        Subject* currentSubject = findOrCreateSubject(words[i]);
                        askedSubjects[askedSubjectsCount] = currentSubject;
                        askedSubjectsCount++;
                    }
                    else {
                        printf("INVALID\n");
                        return;
                    }
                }
                else { // Search for "and"
                    if (strcmp(words[i], "and") != 0) {
                        printf("INVALID\n");
                        return;
                    }
                }
            }
            sumItem(askedSubjects, askedSubjectsCount, itemName);
            free(askedSubjects);
        }
        else { printf("INVALID\n"); }
    }

    else { printf("INVALID\n"); }
}


bool globalReader(int sentenceType) { // Function to read sentences
    if (sentenceType == -1) {
        return false; // Invalid sentence
    } 

    if (sentenceType == 2) {
        return true; // End of sentence
    }


    sentences = realloc(sentences, (*sentenceCount + 1) * sizeof(Sentence*));

    Sentence* newSentence = malloc(sizeof(Sentence));
    newSentence->subjects = NULL;
    newSentence->subjectCount = 0;
    newSentence->items = NULL;
    newSentence->itemCount = 0;
    newSentence->quantities = NULL;
    newSentence->quantityCount = 0;
    newSentence->location = NULL;
    newSentence->other = NULL;
    newSentence->verb = NULL;

    if (sentenceType == 0) { // Call action reader
        newSentence->type = 0;
        sentences[(*sentenceCount)++] = newSentence;
        return globalReader(actionReader(newSentence, currentIndex));
    }

    
    if (sentenceType == 1) { // Call condition reader
        newSentence->type = 1;
        sentences[(*sentenceCount)++] = newSentence;
        return globalReader(conditionReader(newSentence, currentIndex));
    }
    
    return true; 
}

int actionReader(Sentence* sentence, int* currentIndex) { // Function to parse action
    bool foundVerb = false;
    int i = 0;
    while (*currentIndex < numWords) {
        sentence->subjects = realloc(sentence->subjects, (sentence->subjectCount + 1) * sizeof(Subject*));
        if (i % 2 == 0) { // Search for a subject
            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !subjectExists(sentence, words[*currentIndex]) ) {
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
                char* nextKeyword = findNextKeyword(*currentIndex);
                if (nextKeyword != NULL && strcmp(nextKeyword, "to") == 0) { // Sell-to block
                    sentence->verb = strdup("sellTo");
                    while(*currentIndex < numWords) {
                        if (i % 3 == 0) { // Search for a quantity
                            if (validQuantity(words[*currentIndex])) {
                                sentence->quantities = realloc(sentence->quantities, (sentence->quantityCount + 1) * sizeof(int));
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
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !itemExists(sentence, words[*currentIndex])) {
                                char* currentItem = words[*currentIndex];
                                sentence->items = realloc(sentence->items, (sentence->itemCount + 1) * sizeof(char*));
                                sentence->items[sentence->itemCount] = currentItem;
                                sentence->itemCount++;
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
                            else if (strcmp(words[*currentIndex], "to" ) == 0 && *currentIndex + 1 < numWords) {
                                i++;
                                (*currentIndex)++;
                                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !subjectExists(sentence, words[*currentIndex])) {
                                    Subject* otherSubject = findOrCreateSubject(words[*currentIndex]);
                                    sentence->other = otherSubject;
                                }
                                else { return -1; }
                        
                                if (*currentIndex + 1 < numWords) {
                                    i++;
                                    (*currentIndex)++;
                                    if (strcmp(words[*currentIndex], "and") == 0 && *currentIndex + 1 < numWords ) {
                                        i++;
                                        (*currentIndex)++;
                                        return 0;
                                    }
                                    else if (strcmp(words[*currentIndex], "if") == 0 && *currentIndex + 1 < numWords ) {
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
                    sentence->verb = strdup("sell");
                    while(*currentIndex < numWords) {
                        if (i % 3 == 0) { // Search for a quantity
                            if (validQuantity(words[*currentIndex])) {
                                sentence->quantities = realloc(sentence->quantities, (sentence->quantityCount + 1) * sizeof(int));
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
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !itemExists(sentence, words[*currentIndex])) {
                                char* currentItem = words[*currentIndex];
                                sentence->items = realloc(sentence->items, (sentence->itemCount + 1) * sizeof(char*));
                                sentence->items[sentence->itemCount] = currentItem;
                                sentence->itemCount++;
                                i++;
                                (*currentIndex)++;
                            }
                            else {
                                return -1;
                            }
                        }
                        else { // Search for "and" or "if"
                            if (strcmp(words[*currentIndex], "and") == 0 && *currentIndex + 1 < numWords) {
                                i++;
                                (*currentIndex)++;
                            }
                            else if (strcmp(words[*currentIndex], "if" ) == 0 && *currentIndex + 1 < numWords) {
                                i++;
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
                (*currentIndex)++;
                i = 0;
                char* nextKeyword = findNextKeyword(*currentIndex);
                if (nextKeyword != NULL && strcmp(nextKeyword, "from") == 0) { // Buy-from block
                    sentence->verb = strdup("buyFrom");
                    while(*currentIndex < numWords) {
                        if (i % 3 == 0) { // Search for a quantity
                            if (validQuantity(words[*currentIndex])) {
                                sentence->quantities = realloc(sentence->quantities, (sentence->quantityCount + 1) * sizeof(int));
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
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !itemExists(sentence, words[*currentIndex])) {
                                char* currentItem = words[*currentIndex];
                                sentence->items = realloc(sentence->items, (sentence->itemCount + 1) * sizeof(char*));
                                sentence->items[sentence->itemCount] = currentItem;
                                sentence->itemCount++;
                                i++;
                                (*currentIndex)++;
                            }

                            else {
                                return -1;
                            }
                        }
                        else { // Search for "and" or "from"
                            if (strcmp(words[*currentIndex], "and") == 0) {
                                i++;
                                (*currentIndex)++;
                            }
                            else if (strcmp(words[*currentIndex], "from" ) == 0 && *currentIndex + 1 < numWords) {
                                i++;
                                (*currentIndex)++;
                                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !subjectExists(sentence, words[*currentIndex]) ) {
                                    Subject* otherSubject = findOrCreateSubject(words[*currentIndex]);
                                    sentence->other = otherSubject;
                                }
                                else { return -1; }
                        
                                if (*currentIndex + 1 < numWords) {
                                    i++;
                                    (*currentIndex)++;
                                    if (strcmp(words[*currentIndex], "and") == 0 && *currentIndex + 1 < numWords ) {
                                        i++;
                                        (*currentIndex)++;
                                        return 0;
                                    }
                                    else if (strcmp(words[*currentIndex], "if") == 0 && *currentIndex + 1 < numWords ) {
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
                else { // End of current or whole sentence, Buy block
                    sentence->verb = strdup("buy");
                    while(*currentIndex < numWords) {
                        if (i % 3 == 0) { // Search for a quantity
                            if (validQuantity(words[*currentIndex])) {
                                sentence->quantities = realloc(sentence->quantities, (sentence->quantityCount + 1) * sizeof(int));
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
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !itemExists(sentence, words[*currentIndex])) {
                                char* currentItem = words[*currentIndex];
                                sentence->items = realloc(sentence->items, (sentence->itemCount + 1) * sizeof(char*));
                                sentence->items[sentence->itemCount] = currentItem;
                                sentence->itemCount++;
                                i++;
                                (*currentIndex)++;
                            }
                            else {
                                return -1;
                            }
                        }
                        else { // Search for "and" or "if"
                            if (strcmp(words[*currentIndex], "and") == 0 && *currentIndex + 1 < numWords) {
                                i++;
                                (*currentIndex)++;
                            }
                            else if (strcmp(words[*currentIndex], "if" ) == 0 && *currentIndex + 1 < numWords) {
                                i++;
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
            
            else if (strcmp(words[*currentIndex], "go" ) == 0 && *currentIndex + 1 < numWords) {
                (*currentIndex)++;
                if (strcmp(words[*currentIndex], "to") == 0) {
                    if (*currentIndex + 1 < numWords) { // Search for a location
                        (*currentIndex)++;
                        if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) ) {     
                            char* currentLocation = strdup(words[*currentIndex]);                        
                            sentence->location = currentLocation;
                            sentence->verb = strdup("goTo");  
                            foundVerb = true;
                            
                            if (*currentIndex + 1 < numWords) { // Search for a new sentence
                                (*currentIndex)++;
                                if (strcmp(words[*currentIndex], "and") == 0 && *currentIndex + 1 < numWords) {
                                    (*currentIndex)++;
                                    return 0;
                                }
                                else if (strcmp(words[*currentIndex], "if") == 0 && *currentIndex + 1 < numWords) {
                                    (*currentIndex)++;
                                    return 1;
                                }
                                else { return -1; }
                            }
                            else { 
                                return 2; 
                            }

                        }
                        else { return -1; }
                        
                    }
                    else { return -1; }
                }
                else { return -1; }
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

int conditionReader(Sentence* sentence, int* currentIndex) { // Function to parse condition
    bool foundVerb = false;
    int i = 0;
    while (*currentIndex < numWords) {
        sentence->subjects = realloc(sentence->subjects, (sentence->subjectCount + 1) * sizeof(Subject*));
        if (i % 2 == 0) { // Search for a subject
            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !subjectExists(sentence, words[*currentIndex])) {
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
        else { // Search for a condition verb or "and"
            if (strcmp(words[*currentIndex], "and") == 0) {
                i++;
                (*currentIndex)++;
            }
            else if (strcmp(words[*currentIndex], "has") == 0 && *currentIndex + 1 < numWords) {
                (*currentIndex)++;
                i = 0;
                char* nextKeyword = findNextKeyword(*currentIndex);
                if (nextKeyword != NULL && strcmp(words[*currentIndex], "less") == 0 && *currentIndex + 1 < numWords) { // Has less than block
                    (*currentIndex)++;
                    if (strcmp(words[*currentIndex], "than") == 0 && *currentIndex + 1 < numWords) {
                        sentence->verb = strdup("hasLessThan");
                        (*currentIndex)++;        
                        while(*currentIndex < numWords) {
                            if (i % 3 == 0) { // Search for a quantity
                                if (validQuantity(words[*currentIndex])) {
                                    sentence->quantities = realloc(sentence->quantities, (sentence->quantityCount + 1) * sizeof(int));
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
                                        char* nextKeyword = findNextKeyword(*currentIndex);
                                        if(nextKeyword != NULL && isActionKeyword(nextKeyword)){
                                            return 0;
                                        }
                                        else if(nextKeyword != NULL && (strcmp(nextKeyword,"at") == 0 || strcmp(nextKeyword,"has") == 0)){
                                            return 1;
                                        }
                                        else {return -1;}
                                    }
                                }
                            }
                            else if (i % 3 == 1) { // Search for item
                                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !itemExists(sentence, words[*currentIndex])) {
                                    char* currentItem = words[*currentIndex];
                                    sentence->items = realloc(sentence->items, (sentence->itemCount + 1) * sizeof(char*));
                                    sentence->items[sentence->itemCount] = currentItem;
                                    sentence->itemCount++;
                                    i++;
                                    (*currentIndex)++;
                                }
                                else {
                                    return -1;
                                }
                            }
                            else { // Search for "and"
                                if (strcmp(words[*currentIndex], "and") == 0 && *currentIndex + 1 < numWords) {
                                    i++;
                                    (*currentIndex)++;
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
                    else {return -1;}
                }
                else if (nextKeyword != NULL && strcmp(words[*currentIndex], "more") == 0 && *currentIndex + 1 < numWords) { // Has more than block
                    (*currentIndex)++;
                    if (strcmp(words[*currentIndex], "than") == 0 && *currentIndex + 1 < numWords) {
                        sentence->verb = strdup("hasMoreThan");
                        (*currentIndex)++;                      
                        while(*currentIndex < numWords) {
                            if (i % 3 == 0) { // Search for a quantity
                                if (validQuantity(words[*currentIndex])) {
                                    sentence->quantities = realloc(sentence->quantities, (sentence->quantityCount + 1) * sizeof(int));
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
                                        char* nextKeyword = findNextKeyword(*currentIndex);
                                        if(nextKeyword != NULL && isActionKeyword(nextKeyword)){
                                            return 0;
                                        }
                                        else if(nextKeyword != NULL && (strcmp(nextKeyword,"at") == 0 || strcmp(nextKeyword,"has") == 0)){
                                            return 1;
                                        }
                                        else { return -1; }
                                    }
                                }
                            }
                            else if (i % 3 == 1) { // Search for item
                                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !itemExists(sentence, words[*currentIndex])) {
                                    char* currentItem = words[*currentIndex];
                                    sentence->items = realloc(sentence->items, (sentence->itemCount + 1) * sizeof(char*));
                                    sentence->items[sentence->itemCount] = currentItem;
                                    sentence->itemCount++;
                                    i++;
                                    (*currentIndex)++;
                                }
                                else {
                                    return -1;
                                }
                            }
                            else { // Search for "and"
                                if (strcmp(words[*currentIndex], "and") == 0 && *currentIndex + 1 < numWords) {
                                    i++;
                                    (*currentIndex)++;
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
                    else { return -1; }
                }

                else { // End of current or whole sentence, has block
                    sentence->verb = strdup("has");
                    while(*currentIndex < numWords) {
                        if (i % 3 == 0) { // Search for a quantity
                            if (validQuantity(words[*currentIndex])) {
                                sentence->quantities = realloc(sentence->quantities, (sentence->quantityCount + 1) * sizeof(int));
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
                                    char* nextKeyword = findNextKeyword(*currentIndex);
                                    if(nextKeyword != NULL && isActionKeyword(nextKeyword)){
                                        return 0;
                                    }
                                    else if(nextKeyword != NULL && (strcmp(nextKeyword,"at") == 0 || strcmp(nextKeyword,"has") == 0)){
                                        return 1;
                                    }
                                    else { return -1; }
                                }
                            }
                        }
                        else if (i % 3 == 1) { // Search for item
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) && !itemExists(sentence, words[*currentIndex])) {
                                char* currentItem = words[*currentIndex];
                                sentence->items = realloc(sentence->items, (sentence->itemCount + 1) * sizeof(char*));
                                sentence->items[sentence->itemCount] = currentItem;
                                sentence->itemCount++;
                                i++;
                                (*currentIndex)++;
                            }
                            else { return -1; }
                        }
                        else { // Search for "and"
                            if (strcmp(words[*currentIndex], "and") == 0 && *currentIndex + 1 < numWords) {
                                i++;
                                (*currentIndex)++;
                            }
                            else { return -1; }
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
            else if (strcmp(words[*currentIndex], "at" ) == 0 && *currentIndex + 1 < numWords) { // At block
                (*currentIndex)++;
                    // Search for a location
                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) ) {    
                    char* currentLocation = strdup(words[*currentIndex]);                        
                    sentence->location = currentLocation;
                    sentence->verb = strdup("at");  
                    foundVerb = true;
        
                    if (*currentIndex + 1 < numWords) { // Search for a new sentence
                        (*currentIndex)++;
                        if (strcmp(words[*currentIndex], "and") == 0 && *currentIndex + 1 < numWords) {
                            char* nextKeyword = findNextKeyword(*currentIndex);
                            if(nextKeyword != NULL && isActionKeyword(nextKeyword)){
                                (*currentIndex)++;
                                return 0;
                            }
                            else if(nextKeyword != NULL && (strcmp(nextKeyword,"at") == 0 || strcmp(nextKeyword,"has") == 0)){
                                (*currentIndex)++;
                                return 1;
                            }
                            else { return -1; }
                        }
                        else { return -1; }
                    }
                    else { return 2; }
                }
                else { return -1; }
            }
            else { return -1; }
        }
    }

    if (!foundVerb) {
        return -1;
    }
    return 2;
} 

// Function to check given condition is true or false
bool checkCondition(Sentence* condition) {
    if (strcmp(condition->verb, "has") == 0) {
        return has(condition->subjects, condition->subjectCount, (const char**)condition->items, condition->itemCount, condition->quantities);
    }
    else if (strcmp(condition->verb, "hasMoreThan") == 0) {
        return hasMoreThan(condition->subjects, condition->subjectCount, (const char**)condition->items, condition->itemCount, condition->quantities);
    }
    else if (strcmp(condition->verb, "hasLessThan") == 0) {
        return hasLessThan(condition->subjects, condition->subjectCount, (const char**)condition->items, condition->itemCount, condition->quantities);
    }
    else if (strcmp(condition->verb, "at") == 0) {
        return at(condition->subjects, condition->subjectCount, condition->location);
    }
    printf("Condition does not have a verb.\n");
    return false;
}

// Function to execute action if possible
void executeAction(Sentence* action) {
    if (strcmp(action->verb, "buy") == 0) {
        buy(action->subjects, action->subjectCount, (const char**)action->items, action->itemCount, action->quantities);
    }
    else if (strcmp(action->verb, "buyFrom") == 0) {
        buyFrom(action->subjects, action->subjectCount, action->other, (const char**)action->items, action->itemCount, action->quantities);
    }
    else if (strcmp(action->verb, "sell") == 0) {
        sell(action->subjects, action->subjectCount, (const char**)action->items, action->itemCount, action->quantities);
    }
    else if (strcmp(action->verb, "sellTo") == 0) {
        sellTo(action->subjects, action->subjectCount, action->other, (const char**)action->items, action->itemCount, action->quantities);
    }
    else if (strcmp(action->verb, "goTo") == 0) {
        goTo(action->subjects, action->subjectCount, action->location);
    }
}

// Function to determine which actions will be executed according to the conditions
void processSentences() {
    bool conditionsMet = true; // Assume conditions are met until proven otherwise
    for (int i = 0; i < *sentenceCount; i++) {
        if (sentences[i]->type == 0) {
            // Buffer actions until we meet a condition or end of the sequence
            int actionStartIndex = i;
            int actionEndIndex = i;
            while (actionEndIndex + 1 < *sentenceCount && sentences[actionEndIndex + 1]->type == 0) {
                actionEndIndex++;
            }
            
            // Check conditions after this sequence of actions
            conditionsMet = true;
            for (int j = actionEndIndex + 1; j < *sentenceCount && sentences[j]->type == 1; j++) {
                if (!checkCondition(sentences[j])) {
                    conditionsMet = false; // Any condition is false, actions should not be executed
                    break;
                }
            }
            // Execute actions if all conditions met
            if (conditionsMet) {
                for (int j = actionStartIndex; j <= actionEndIndex; j++) {
                    executeAction(sentences[j]);
                }
            }
        
            
            // Skip to the end of the action sequence
            i = actionEndIndex;
        }
    }
}


void freeSentence(Sentence* sentence) {
    if (sentence != NULL) {

        // Free subjects array of pointers
        if (sentence->subjects != NULL) {
            free(sentence->subjects); // Free the pointer array itself
            sentence->subjects = NULL; // Nullify after free to avoid double free
        }

        // Free items array of pointers
        if (sentence->items != NULL) {
            free(sentence->items); // Free the pointer array itself
            sentence->items = NULL; // Nullify after free to avoid double free
        }
        

        // Free the quantities array
        if (sentence->quantities != NULL) {
            free(sentence->quantities);
            sentence->quantities = NULL;
        }

        // Free the location string
        if (sentence->location != NULL) {
            free(sentence->location);
            sentence->location = NULL;
        }

        // Free the verb string
        if (sentence->verb != NULL) {
            free(sentence->verb);
            sentence->verb = NULL;
        }

        // Finally, free the sentence struct itself
        free(sentence);
        sentence = NULL;
    }
}


void resetSentencesArray() {
    if (sentences != NULL) {
        for (int i = 0; i < *sentenceCount; i++) {
            if (sentences[i] != NULL) {
                freeSentence(sentences[i]);  // Free the resources held by the Sentence
                sentences[i] = NULL;        // Reset the pointer to NULL after freeing
            }
        }
        free(sentences);
    }
}

#endif // PARSE_H