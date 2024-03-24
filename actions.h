#ifndef ACTIONS_H
#define ACTIONS_H

#include "entities.h" // Include the entities definitions

// Function prototypes for actions
bool buy(Subject** buyers, int numBuyers, const char** itemNames, int numItems, int* quantities);
bool buyFrom(Subject** buyers, int numBuyers, Subject* seller, const char** itemNames, int numItems, int* quantities);
bool sell(Subject** sellers, int numSellers, const char** itemNames, int numItems, int* quantities);
bool sellTo(Subject** sellers, int numSellers, Subject* buyer, const char** itemNames, int numItems, int* quantities);

void goTo(Subject** travellers, int numTravelllers, const char* locationName);


bool buy(Subject** buyers, int numBuyers, const char** itemNames, int numItems, int* quantities) {
    
    for (int i = 0; i < numBuyers; ++i) {
        for (int j = 0; j < numItems; ++j) {
            const char* itemName = itemNames[j];
            int quantity = quantities[j];
            addItemToInventory(buyers[i], itemName, quantity);
        }
    }
    return true;
}


bool buyFrom(Subject** buyers, int numBuyers, Subject* seller, const char** itemNames, int numItems, int* quantities) {

    for (int i = 0; i < numItems; ++i) {
        const char* itemName = itemNames[i];
        int quantity = quantities[i];
        Item* item = findItem(seller, itemName);
        if (!item || item->quantity < quantity) {
            return false; // Seller doesn't have enough items or item not found
        }
    }

    // Deduct the items from the seller's inventory directly
    for (int i = 0; i < numItems; ++i) {
        const char* itemName = itemNames[i];
        int quantity = quantities[i];
        Item* item = findItem(seller, itemName);
        item->quantity -= quantity;
    }

    for (int i = 0; i < numBuyers; ++i) {
        for (int j = 0; j < numItems; ++j) {
            const char* itemName = itemNames[j];
            int quantity = quantities[j];
            addItemToInventory(buyers[i], itemName, quantity);
        }
    }
    return true;
}



bool sell(Subject** sellers, int numSellers, const char** itemNames, int numItems, int* quantities) {
    
    // First, ensure all sellers have enough of the item
    for (int i = 0; i < numSellers; ++i) {
        for (int j = 0; j < numItems; ++j) {
            const char* itemName = itemNames[j];
            int quantity = quantities[j];
            Item* item = findItem(sellers[i], itemName);
            if (!item || item->quantity < quantity) {
                return false; // At least one seller doesn't have enough of the item
            }
        }
    }
    for (int i = 0; i < numSellers; ++i) {
        for (int j = 0; j < numItems; ++j) {
            const char* itemName = itemNames[j];
            int quantity = quantities[j];
            Item* item = findItem(sellers[i], itemName);
            item->quantity -= quantity; // Update the seller's inventory
        }
    }
    // Assuming selling to an infinite source doesn't require adding items anywhere
    return true;
}



bool sellTo(Subject** sellers, int numSellers, Subject* buyer, const char** itemNames, int numItems, int* quantities) {

    for (int i = 0; i < numSellers; ++i) {
        for (int j = 0; j < numItems; ++j) {
            const char* itemName = itemNames[j];
            int quantity = quantities[j];
            Item* item = findItem(sellers[i], itemName);
            if (!item || item->quantity < quantity) {
                return false; // At least one seller doesn't have enough of the item
            }
        }
    }
    // Proceed with the transaction
    for (int i = 0; i < numSellers; ++i) {
        for (int j = 0; j < numItems; ++j) {
            const char* itemName = itemNames[j];
            int quantity = quantities[j];
            Item* item = findItem(sellers[i], itemName);
            item->quantity -= quantity; // Update the seller's inventory
            addItemToInventory(buyer, itemName, quantity); // Add to the buyer's inventory
        }
    }
    return true;
}



void goTo(Subject** travellers, int numTravellers, const char* locationName) {
    Location* destLocation = findOrCreateLocation(locationName);

    for (int i = 0; i < numTravellers; ++i) {
        // Remove each traveller from their current location
        removeSubjectFromLocation(travellers[i]);

        // Update traveller's location and add them to the new location
        travellers[i]->currentLocation = destLocation; // Update Subject's location pointer
        addSubjectToLocation(travellers[i], destLocation);
    }
}



#endif // ACTIONS_H