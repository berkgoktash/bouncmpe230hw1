#ifndef QUESTIONS_H
#define QUESTIONS_H

#include "entities.h"

// Function prototypes for questions
void sumItem(Subject** owners, int numOwners, const char* itemName);
void where(Subject* visitor);
void whoAt(const char* locationName);
void totalInventory(Subject* owner);

void sumItem(Subject** owners, int numOwners, const char* itemName) {
    int total = 0;
    for (int i = 0; i < numOwners; ++i) {
        if (findItem(owners[i], itemName) != NULL) {
            total += findItem(owners[i], itemName)->quantity;
        }
    }
    printf("%d\n", total);

}
void where(Subject* visitor) {
    if (!visitor->currentLocation) {
        printf("NOWHERE\n");
    }
    else {
        printf("%s\n", visitor->currentLocation->locationName);
    }
    return;
}

void whoAt(const char* locationName) {
    Location* destLocation = findOrCreateLocation(locationName);
    if (!destLocation->subjectsPresent) {
        printf("NOBODY\n");
    }
    else {
        for (int i = 0; i < destLocation->subjectCount; ++i) {
            printf("%s", destLocation->subjectsPresent[i]->name);
            if (i != destLocation->subjectCount - 1) printf(" and ");
        }
        printf("\n");
    }
    return;
}

void totalInventory(Subject* owner) {
    int hasItems = 0; // Flag to track if any items with quantity > 0 exist
    int written = 0;

    for (int i = 0; i < owner->inventorySize; ++i) {
        if (owner->inventory[i].quantity > 0) {
            hasItems = 1; // Found at least one item with quantity > 0
        }
    }

    if (hasItems == 0) {
        printf("NOTHING\n"); // No items with quantity > 0 found
    }
    else {
        for (int i = 0; i < owner->inventorySize; ++i) {
            if (owner->inventory[i].quantity > 0) {
                if (written != 0) {
                    printf(" and ");
                }
                printf("%d %s", owner->inventory[i].quantity, owner->inventory[i].itemName);
                written = 1;
            }
        }
        printf("\n");
    }

}



#endif // QUESTIONS_H