#ifndef INC_8307_ERSHOV_LAB2_P1_VIRTUALMEMORY_H
#define INC_8307_ERSHOV_LAB2_P1_VIRTUALMEMORY_H


#include <sysinfoapi.h>
#include <memoryapi.h>
#include <iostream>
#include <map>
#include <bitset>

void getSystemInfo();

void getVirtualMemoryStatus();

void getVirtualMemoryStatusByAddressStarter();

void getVirtualMemoryStatusByAddress(DWORD64 memoryAddress);

void manualReverseCommit();

void autoReverseCommit();

void writeToAddress();

void protectMemoryByAddress();

#endif //INC_8307_ERSHOV_LAB2_P1_VIRTUALMEMORY_H
