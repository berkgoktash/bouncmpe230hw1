#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "entities.h"

// Function prototypes for conditions
bool has(Subject** owners, int numOwners, const char** itemNames, int numItems, int* quantities);
bool hasMoreThan(Subject** owners, int numOwners, const char** itemNames, int numItems, int* quantities);
bool hasLessThan(Subject** owners, int numOwners, const char** itemNames, int numItems, int* quantities);
bool at(Subject** visitors, int numVisitors, const char* locationName);

// Function to check if owners have a specific amount of item(s)
bool has(Subject** owners, int numOwners, const char** itemNames, int numItems, int* quantities) {
    int ownerItemCount;
    for (int i = 0; i < numOwners; ++i) {
        for (int j = 0; j < numItems; ++j) {
            const char* itemName = itemNames[j];
            int quantity = quantities[j];
            Item* item = findItem(owners[i], itemName);
            if (!item) {
                ownerItemCount = 0;
            }
            else {
                ownerItemCount = item->quantity;
            }

            if (ownerItemCount != quantity) return false;
        }
    }
    return true;
}

// Function to check if owners have more than a specific amount of item(s)
bool hasMoreThan(Subject** owners, int numOwners, const char** itemNames, int numItems, int* quantities) {
    int ownerItemCount;
    for (int i = 0; i < numOwners; ++i) {
        for (int j = 0; j < numItems; ++j) {
            const char* itemName = itemNames[j];
            int quantity = quantities[j];
            Item* item = findItem(owners[i], itemName);
            if (!item) {
                ownerItemCount = 0;
            }
            else {
                ownerItemCount = item->quantity;
            }

            if (ownerItemCount <= quantity) return false;
        }
    }
    return true;
}

// Function to check if owners have less than a specific amount of item(s)
bool hasLessThan(Subject** owners, int numOwners, const char** itemNames, int numItems, int* quantities) {
    int ownerItemCount;
    for (int i = 0; i < numOwners; ++i) {
        for (int j = 0; j < numItems; ++j) {
            const char* itemName = itemNames[j];
            int quantity = quantities[j];
            Item* item = findItem(owners[i], itemName);
            if (!item) {
                ownerItemCount = 0;
            }
            else {
                ownerItemCount = item->quantity;
            }

            if (ownerItemCount >= quantity) return false;
        }
    }
    return true;
}

// Function to check if subjects are at a specific location
bool at(Subject** visitors, int numVisitors, const char* locationName) {
    for (int i = 0; i < numVisitors; ++i) {
        if (visitors[i]->currentLocation == NULL) return false;
        if (strcmp(visitors[i]->currentLocation->locationName, locationName) != 0) return false;
    }
    return true;
}


#endif // CONDITIONS_H