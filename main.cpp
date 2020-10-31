#include <iostream>
#include <sstream>
#include "virtualMemory.h"


void menuPrint();
int menuSelect(int smallestItemNumber, int biggestItemNumber);

// Array of pointers to functions
void (*functionsList[])() = {
    getSystemInfo,
    getVirtualMemoryStatus,
    getVirtualMemoryStatusByAddressStarter,
    reserveMemory,
    reserveCommitMemory,
    writeToAddress,
    protectMemoryByAddress
};

int main(){
    int smallestItem = 0, biggestItem = 7;
    menuPrint();
    int val = menuSelect(smallestItem , biggestItem);

    while(val != 0){
        if (val != -1){
            (*functionsList[val-1])();
        }

        // Clearing input to avoid input buffering
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        menuPrint();
        val = menuSelect(smallestItem , biggestItem);
    }
    return 0;
}

// Print menu items
void menuPrint(){
    std::cout << "\n --- Main menu --- \n";
    std::cout << "1. Retrieve system information (GetSystemInfo)\n";
    std::cout << "2. Retrieve memory information (GlobalMemoryStatusEx)\n";
    std::cout << "3. Retrieve virtual memory status by address (VirtualQuery)\n";
    std::cout << "4. Reserve memory (by address, automatic), show info, decommit after\n";
    std::cout << "5. Reserve and commit memory (by address, automatic), show info, decommit and release after\n";
    std::cout << "6. Write data to memory\n";
    std::cout << "7. Protect memory by address\n";
    std::cout << "0. Exit";
    std::cout << "\n";
}

// Select menu item
int menuSelect(int smallestItemNumber, int biggestItemNumber){
    std::string selectedItemString;
    std::cout << "Enter selected item:\n";
    std::cin >> selectedItemString;

    // Try to get integer number from entered string
    std::stringstream stringStream;
    stringStream << selectedItemString;

    int selectedItem;
    stringStream >> selectedItem;

    if (stringStream.fail()){
        // Entered string is incorrect
        std::cout << "Entered string is incorrect" << std::endl;
        std::cout << "Try again" << std::endl;
        selectedItem = -1;
    } else if ((selectedItem < smallestItemNumber) || (selectedItem > biggestItemNumber)) {
        // Entered string is correct, but number is out of range [smallestItemNumber; biggestItemNumber]
        std::cout << "There is no item with number " << selectedItem << std::endl;
        std::cout << "Try again" << std::endl;
        selectedItem = -1;
    }
    return selectedItem;
}
