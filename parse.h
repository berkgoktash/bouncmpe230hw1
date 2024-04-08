#ifndef PARSE_H
#define PARSE_H

#include "entities.h"
#include "actions.h"
#include "conditions.h"
#include "questions.h"

#define MAX_INPUT_LENGTH 1024
#define MAX_WORDS 300
#define MAX_LINES 128

int actionReader(Sentence* sentence, int* currentIndex);

Location** allLocations = NULL; // Define and initialize the global locations array
int allLocationsCount = 0; // Define and initialize the global count of locations
Subject** allSubjects = NULL; // Define and initialize the global subjects array
int allSubjectsCount = 0; // Define and initialize the global count of subjects

int start = 0;
int* currentIndex = &start;
int count = 0;
int* sentenceCount = &count;
Sentence** sentences = NULL;

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

const char* questionKeywords[] = {
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

char* findNextKeyword(int start) {
    for (int i = start; i < numWords; ++i) {
        if (isTargetKeyword(words[i])) return words[i];
    }
    return NULL;
}

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

void questionReader() {
    char* questionKeyword = findQuestionKeyword();
    if (questionKeyword != NULL && strcmp(questionKeyword, "where") == 0) {  // Where block
        if (numWords == 3 && isValidString(words[0]) && !isAnyKeyword(words[0])) {
            Subject* askedSubject = findOrCreateSubject(words[0]);
            where(askedSubject);
        }
        else { printf("INVALID\n"); }
    }

    else if (questionKeyword != NULL && strcmp(questionKeyword, "who") == 0) {  // Who-at block
        if (numWords == 4 && strcmp(words[0], "who") == 0 && strcmp(words[1], "at") && isValidString(words[2]) && !isAnyKeyword(words[2])) {
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
                if (i % 2 == 0) { // Search for a subject
                    askedSubjects = realloc(askedSubjects, (askedSubjectsCount + 1) * sizeof(Subject*));
                    if (isValidString(words[i]) && !isAnyKeyword(words[i])) {
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
        }
        else { printf("INVALID\n"); }
    }

    else { printf("INVALID\n"); }
}


bool globalReader(int sentenceType) {
    if (sentenceType == -1) {
        return false; // Invalid sentence
    } 

    if (sentenceType == 2) {
        printf("END OF SENTENCE\n");
        return true; // End of sentence
    }


    sentences = realloc(sentences, (*sentenceCount + 1) * sizeof(Sentence*));

    if (sentenceType == 0) { // Call action reader
        Sentence* newSentence = malloc(sizeof(Sentence));
        newSentence->subjects = NULL;
        newSentence->items = NULL;
        newSentence->quantities = NULL;
        sentences[(*sentenceCount)++] = newSentence;
        newSentence->type = 0;
        return globalReader(actionReader(newSentence, currentIndex));
    }

    /*
    if (sentenceType == 1) { // Call condition reader
        Sentence* newSentence = malloc(sizeof(Sentence));
        sentences[(*sentenceCount)++] = newSentence;
        newSentence->type = 1;
        return globalReader(sentences, sentenceCount, currentIndex, conditionReader());
    }
    */
    return true; 
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
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex])) {
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
                                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) ) {
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
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex])) {
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
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex])) {
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
                                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex]) ) {
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
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex])) {
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

int conditionReader(Sentence* sentence, int* currentIndex) { // Read Sentences
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
                                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex])) {
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
                                if (strcmp(words[*currentIndex], "and") == 0) {
                                    i++;
                                    (*currentIndex)++;
                                }
                                else {
                                    return -1;
                                }
                            }
                        }
                    }
                    else {return -1;}
                }
                else if (nextKeyword != NULL && strcmp(words[*currentIndex], "more") == 0 && *currentIndex + 1 < numWords) { // Has more than block
                    (*currentIndex)++;
                    if (strcmp(words[*currentIndex], "than") == 0 && *currentIndex + 1 < numWords) {
                        sentence->verb = strdup("hasMoreThan");                    
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
                                if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex])) {
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
                                if (strcmp(words[*currentIndex], "and") == 0) {
                                    i++;
                                    (*currentIndex)++;
                                }
                                else {
                                    return -1;
                                }
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
                            if (isValidString(words[*currentIndex]) && !isAnyKeyword(words[*currentIndex])) {
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
                if (*currentIndex + 1 < numWords) { // Search for a location
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
                                    return 0;
                                }
                                else if(nextKeyword != NULL && (strcmp(nextKeyword,"at") == 0 || strcmp(nextKeyword,"has") == 0)){
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
            else { return -1; }
        }
    }

    if (!foundVerb) {
        return -1;
    }
    return 2;
} 



#endif // PARSE_H