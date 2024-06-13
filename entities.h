#ifndef ENTITIES_H
#define ENTITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // For using 'true' and 'false'
#include <ctype.h>   // For using 'isupper', 'islower'

typedef struct Subject Subject; // Forward declaration
typedef struct Location Location; // Forward declaration

// Item structure
typedef struct Item {
    char* itemName; // Name of the item
    int quantity;   // Quantity of the item
} Item;

// Subject structure
struct Subject {
    char* name;    // Name of the subject
    Location* currentLocation; // Current location of the subject
    int inventorySize;
    //Item* inventory;   // Pointer to an array of items
    Item** inventory;   // Pointer to an array of items
} ;

// Location Structure
struct Location {
    char* locationName;
    int subjectCount;
    Subject** subjectsPresent; // Array of pointers to Subjects
} ;

typedef struct Sentence {
    int type; // 0 if action, 1 if condition
    Subject** subjects;
    int subjectCount;
    char** items;
    int* quantities;
    int itemCount;
    int quantityCount;
    char* location;  
    Subject* other;
    char* verb;
} Sentence;

extern Location** allLocations; // Declare the global locations array
extern int allLocationsCount; // Declare the global count of locations
extern Subject** allSubjects; // Declare the global subjects array
extern int allSubjectsCount; // Declare the global count of subjects



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

// Function to find a subject from global subjects list or create a new one if not exists
Subject* findOrCreateSubject(const char* name) {
    // Try to find the subject by name
    for (int i = 0; i < allSubjectsCount; ++i) {
        if (strcmp(allSubjects[i]->name, name) == 0) {
            // Subject found
            return allSubjects[i];
        }
    }

    // Subject not found, create a new one
    // Increase the subjects array size by 1
    Subject** temp = realloc(allSubjects, (allSubjectsCount + 1) * sizeof(Subject*));
    if (!temp) {
        // Handle realloc failure
        printf("Failed to allocate memory for new subject.\n");
        return NULL;
    }
    allSubjects = temp;

    // Allocate memory for the new Subject
    Subject* newSubject = malloc(sizeof(Subject));
    if (!newSubject) {
        // Handle malloc failure
        printf("Failed to allocate memory for new subject.\n");
        return NULL;
    }

    // Initialize the new Subject
    newSubject->name = strdup(name); // Copy the name
    newSubject->currentLocation = NULL;
    newSubject->inventory = NULL;
    newSubject->inventorySize = 0;

    // Add the new Subject to the global list
    allSubjects[allSubjectsCount] = newSubject;
    allSubjectsCount++;

    return newSubject;
}

// Function to find an item from a subject's inventory
Item* findItem(Subject* subject, const char* itemName) {
    for (int i = 0; i < subject->inventorySize; ++i) {
        if (strcmp(subject->inventory[i]->itemName, itemName) == 0) {
            // Item found
            return subject->inventory[i];
        }
    }
    // Item not found
    return NULL;
}

// Function to add an item to a subject's inventory
void addItemToInventory(Subject* subject, const char* itemName, int quantity) {
    Item* item = findItem(subject, itemName);
    if (item != NULL) {
        // Item exists, update quantity
        item->quantity += quantity;
    } 
    else {
        // Item doesn't exist, add new item
        // Reallocate inventory to accommodate new item
        subject->inventory = realloc(subject->inventory, (subject->inventorySize + 1) * sizeof(Item*));
        // Initialize the new item
        Item* newItem = malloc(sizeof(Item));
        newItem->itemName = strdup(itemName);
        newItem->quantity = quantity;
        subject->inventory[subject->inventorySize] = newItem;
        subject->inventorySize++;
    }
}

// Function to find a location from global locations list or create a new one if not exists
Location* findOrCreateLocation(const char* locationName) {
    for (int i = 0; i < allLocationsCount; ++i) {
        if (strcmp(allLocations[i]->locationName, locationName) == 0) {
            return allLocations[i];
        }
    }

    // Location not found, create a new one
    allLocationsCount++;
    allLocations = realloc(allLocations, allLocationsCount * sizeof(Location*));
    if (!allLocations) {
        // Handle realloc failure
        return NULL; // Simple error handling for demonstration
    }

    // Initialize the new Location
    Location* newLocation = malloc(sizeof(Location));
    if (!newLocation) {
        // Handle malloc failure
        printf("Failed to allocate memory for new location.\n");
        return NULL;
    }
    newLocation->locationName = strdup(locationName);
    newLocation->subjectCount = 0;
    newLocation->subjectsPresent = NULL; // Start with no subjects
    allLocations[allLocationsCount - 1] = newLocation;

    return newLocation;
}

// Function to add a subject to a location
void addSubjectToLocation(Subject* subject, Location* location) {
    location->subjectsPresent = realloc(location->subjectsPresent, (location->subjectCount + 1) * sizeof(Subject*));
    if (!location->subjectsPresent) {
        // Handle realloc failure
        exit(1); 
    }

    location->subjectsPresent[location->subjectCount] = subject;
    location->subjectCount++;
}

// Function to remove a subject from a location
void removeSubjectFromLocation(Subject* subject) {
    Location* location = subject->currentLocation;
    if (!location) return; // Subject is not in a location

    int foundIndex = -1;
    for (int i = 0; i < location->subjectCount; ++i) {
        if (location->subjectsPresent[i] == subject) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex != -1) {
        // Allocate a new array with one less element
        Subject** newSubjectsPresent = NULL;
        if (location->subjectCount > 1) { // Check to prevent malloc(0)
            newSubjectsPresent = malloc((location->subjectCount - 1) * sizeof(Subject*));
            if (!newSubjectsPresent) {
                // Handle memory allocation failure (e.g., log and exit or throw an error)
                return;
            }

            // Copy elements over, skipping the foundIndex
            for (int i = 0, j = 0; i < location->subjectCount; ++i) {
                if (i != foundIndex) {
                    newSubjectsPresent[j++] = location->subjectsPresent[i];
                }
            }
        }

        // Free the old array and replace it with the new one
        free(location->subjectsPresent);
        location->subjectsPresent = newSubjectsPresent;
        location->subjectCount--;
    }
}


#endif // ENTITIES_H