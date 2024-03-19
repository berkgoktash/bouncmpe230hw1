#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // For using 'true' and 'false'
#include <ctype.h>   // For using 'isupper', 'islower'

// Item structure
typedef struct Item {
    char itemName[100]; // Name of the item
    int quantity;       // Quantity of the item
} Item;

// Subject structure
typedef struct Subject {
    char name[100];    // Name of the subject
    char location[100]; // Current location of the subject
    Item* inventory;   // Pointer to an array of items
} Subject;

// Location Structure
typedef struct {
    char* locationName;
    int subjectCount;
    char** subjectsPresent; // Array of subject names present at this location
} Location;


// Function to check if a string is valid based on given criteria
bool isValidString(const char *str) {
    while (*str) {
        if (!islower(*str) && !isupper(*str) && *str != '_') {
            // If the current character is not an uppercase letter,
            // not a lowercase letter, and not an underscore, return false
            return false;
        }
        str++; // Move to the next character
    }
    // If we reach here, all characters met the criteria
    return true;
}

Subject* createSubject(const char* name) {
    Subject* newSubject = (Subject*)malloc(sizeof(Subject));
    newSubject->name = strdup(name); // Copy the name
    newSubject->inventorySize = 0;
    newSubject->inventory = NULL; // Initially, no items
    newSubject->location = strdup("NOWHERE"); // Initially, at nowhere
    return newSubject;
}

Item* findItem(Subject* subject, const char* itemName) {
    for (int i = 0; i < subject->inventorySize; ++i) {
        if (strcmp(subject->inventory[i].itemName, itemName) == 0) {
            // Item found
            return &(subject->inventory[i]);
        }
    }
    // Item not found
    return NULL;
}

void addItemToInventory(Subject* subject, const char* itemName, int quantity) {
    Item* item = findItem(subject, itemName);
    
    if (item != NULL) {
        // Item exists, update quantity
        item->quantity += quantity;
    } 
    else {
        // Item doesn't exist, add new item
        // Reallocate inventory to accommodate new item
        subject->inventorySize++;
        subject->inventory = realloc(subject->inventory, subject->inventorySize * sizeof(Item));
        
        // Initialize the new item
        subject->inventory[subject->inventorySize - 1].itemName = strdup(itemName);
        subject->inventory[subject->inventorySize - 1].quantity = quantity;
    }
}


