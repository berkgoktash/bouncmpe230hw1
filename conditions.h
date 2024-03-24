#ifndef CONDITIONS_H
#define CONDITIONS_H

#include "entities.h"

// Function prototypes for conditions
bool has(Subject** owners, int numOwners, const char** itemNames, int numItems, int* quantities);
bool hasMoreThan(Subject** owners, int numOwners, const char** itemNames, int numItems, int* quantities);
bool hasLessThan(Subject** owners, int numOwners, const char** itemNames, int numItems, int* quantities);
bool at(Subject** visitors, int numVisitors, const char* locationName);

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

bool at(Subject** visitors, int numVisitors, const char* locationName) {
    for (int i = 0; i < numVisitors; ++i) {
        if (strcmp(visitors[i]->currentLocation->locationName, locationName) != 0) return false;
    }
    return true;
}


#endif // CONDITIONS_H