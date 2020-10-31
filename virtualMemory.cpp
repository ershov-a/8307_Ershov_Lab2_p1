#include "virtualMemory.h"

std::map<int, std::string> wProcessorArchitectures{
        {0,      "x86"},
        {5,      "ARM"},
        {6,      "Intel Itanium-based"},
        {9,      "x64 (AMD or Intel)"},
        {12,     "ARM 64"},
        {0xffff, "Unknown architecture"},
};

/*
 * An obsolete member that is retained for compatibility.
 * Source:  https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-system_info
 */
std::map<int, std::string> dwProcessorTypes{
        {386,  "Intel 386"},
        {486,  "Intel 486"},
        {586,  "Intel Pentium"},
        {2200, "Intel IA64"},
        {8664, "AMD x8664"},
};

// Source: https://docs.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
std::map<DWORD, std::string> PROTECT_MODES{
        {PAGE_EXECUTE,           "PAGE_EXECUTE"},
        {PAGE_EXECUTE_READ,      "PAGE_EXECUTE_READ"},
        {PAGE_EXECUTE_READWRITE, "PAGE_EXECUTE_READWRITE"},
        {PAGE_EXECUTE_WRITECOPY, "PAGE_EXECUTE_WRITECOPY"},
        {PAGE_NOACCESS,          "PAGE_NOACCESS"},
        {PAGE_READONLY,          "PAGE_READONLY"},
        {PAGE_READWRITE,         "PAGE_READWRITE"},
        {PAGE_WRITECOPY,         "PAGE_WRITECOPY"},
        {PAGE_GUARD,             "PAGE_GUARD"},
        {PAGE_NOCACHE,           "PAGE_NOCACHE"},
        {PAGE_WRITECOMBINE,      "PAGE_WRITECOMBINE"},
};

void getSystemInfo() {

    SYSTEM_INFO systemInfo;

    // Retrieve system information
    GetSystemInfo(&systemInfo);

    // Output retrieved information
    std::cout << "--- System info ---" << std::endl;
    std::cout << "Processor Architecture: " << wProcessorArchitectures[systemInfo.wProcessorArchitecture] << std::endl;
    std::cout << "Number of processor cores: " << systemInfo.dwNumberOfProcessors << std::endl;
    std::cout << "(Obsolete) Processors Type: " << dwProcessorTypes[systemInfo.dwProcessorType] << std::endl;
    /*
     * If wProcessorArchitecture is PROCESSOR_ARCHITECTURE_INTEL, wProcessorLevel is defined by the CPU vendor.
     * If wProcessorArchitecture is PROCESSOR_ARCHITECTURE_IA64, wProcessorLevel is set to 1.
     */
    std::cout << "Processor vendor: " << systemInfo.wProcessorLevel << std::endl;
    std::cout << "Page size: " << systemInfo.dwPageSize << std::endl;
    std::cout << "Minimum application address: " << systemInfo.lpMinimumApplicationAddress << std::endl;
    std::cout << "Maximum application address: " << systemInfo.lpMaximumApplicationAddress << std::endl;
    /*
     * Number of active cores in bitmap format (max 31, I used 8 only)
     * Number of active cores in bitmap format (max 31, I used 8 only)
     */
    std::cout << "Active cores: " << std::bitset<8>(systemInfo.dwActiveProcessorMask) << std::endl;

}

// Source: https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-memorystatusex
void getVirtualMemoryStatus() {
    // Divide each value in bytes by the divider to get kilobytes
    int divider = 1024;
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);

    // Retrieve memory status
    GlobalMemoryStatusEx(&memoryStatus);

    // Output retrivied information
    std::cout << "Memory usage " << memoryStatus.dwMemoryLoad << "%" << std::endl;
    std::cout << "Total memory " << memoryStatus.ullTotalPhys / divider << " KB" << std::endl;
    std::cout << "Available memory " << memoryStatus.ullAvailPhys / divider << " KB" << std::endl;
    std::cout << "Total memory for page file " << memoryStatus.ullTotalPageFile / divider << " KB" << std::endl;
    std::cout << "Available memory for page file " << memoryStatus.ullAvailPageFile / divider << " KB" << std::endl;
    std::cout << "Total virtual memory " << memoryStatus.ullTotalVirtual / divider << " KB" << std::endl;
    std::cout << "Available virtual memory " << memoryStatus.ullAvailVirtual / divider << " KB" << std::endl;

    // It's always zero
    std::cout << "Available extended memory " << memoryStatus.ullAvailExtendedVirtual / divider << " KB" << std::endl;
}

void getVirtualMemoryStatusByAddressStarter(){
    getVirtualMemoryStatusByAddress(0);
}

void getVirtualMemoryStatusByAddress(DWORD64 memoryAddress){
    MEMORY_BASIC_INFORMATION memoryBasicInformation;
    SYSTEM_INFO systemInfo;

    // Retrieve system information to get addresses range
    GetSystemInfo(&systemInfo);
    if (memoryAddress == 0){
        std::cout << "Enter memory address in range from ";
        std::cout << systemInfo.lpMinimumApplicationAddress << " to " << systemInfo.lpMaximumApplicationAddress;
        std::cout << "\n> 0x";

        std::cin >> std::hex >> memoryAddress;
    }

    // Retrieve memory information at entered address
    VirtualQuery((LPCVOID)memoryAddress, &memoryBasicInformation, sizeof(memoryBasicInformation));

    // Output information
    std::cout << "--- Memory information ---" << std::endl;
    std::cout << "Base Address: " << std::hex << memoryBasicInformation.BaseAddress << std::endl;
    std::cout << "Allocation Base Address: " << std::hex << memoryBasicInformation.AllocationBase << std::endl;
    std::cout << "Region Size: " << memoryBasicInformation.RegionSize << std::endl;
    std::cout << "Protect Mode: " << std::hex << memoryBasicInformation.Protect << std::endl;
    for (auto flag : PROTECT_MODES) {
        if (memoryBasicInformation.Protect & flag.first) {
            std::cout << "  " << flag.second << "\n";
        }
    }

    // Types and values source: https://docs.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-memory_basic_information
    std::cout << "Page State: " << std::hex << memoryBasicInformation.State << std::endl;
    if (memoryBasicInformation.State & MEM_COMMIT) std::cout << "  MEM_COMMIT" << std::endl;
    if (memoryBasicInformation.State & MEM_FREE) std::cout << "  MEM_FREE" << std::endl;
    if (memoryBasicInformation.State & MEM_RESERVE) std::cout << "  MEM_RESERVE" << std::endl;

    std::cout << "Page Type: " << std::hex << memoryBasicInformation.Type << std::endl;
    if (memoryBasicInformation.Type & MEM_IMAGE) std::cout << "  MEM_IMAGE" << std::endl;
    if (memoryBasicInformation.Type & MEM_MAPPED) std::cout << "  MEM_MAPPED" << std::endl;
    if (memoryBasicInformation.Type & MEM_PRIVATE) std::cout << "  MEM_PRIVATE" << std::endl;

    std::cout << std::dec;
}

void reserveMemory(){
    LPVOID startAddress;
    std::cout << "Enter start address (enter 0 for automatic): 0x";
    std::cin >> std::hex >> startAddress;
    SIZE_T dwSize;
    std::cout << "Enter size in bytes: ";
    std::cin >> std::dec >> dwSize;

    // Try to reserve memory
    LPVOID allocatedMemoryPtr = VirtualAlloc(startAddress, dwSize, MEM_RESERVE, PAGE_READWRITE);

    // If reserve was successful, output information
    if (allocatedMemoryPtr != nullptr) {
        std::cout << "\n*** Memory reserved successfully ***" << std::endl;
        getVirtualMemoryStatusByAddress((DWORD64)allocatedMemoryPtr);
    } else {
        std::cout << "\n*** Can not reserve memory ***" << std::endl;
        return;
    }

    // Release memory
    VirtualFree(allocatedMemoryPtr, 0, MEM_RELEASE);
    std::cout << "\n*** Memory released ***" << std::endl;
    getVirtualMemoryStatusByAddress((DWORD64)allocatedMemoryPtr);
}

void reserveCommitMemory(){
    LPVOID startAddress;
    std::cout << "Enter start address (enter 0 for automatic): 0x";
    std::cin >> std::hex >> startAddress;
    SIZE_T dwSize;
    std::cout << "Enter size in bytes: ";
    std::cin >> std::dec >> dwSize;

    // Try to reserve and commit memory
    LPVOID allocatedMemoryPtr = VirtualAlloc(startAddress, dwSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    // If reserve and commit was successful, output memory information
    if (allocatedMemoryPtr != nullptr) {
        std::cout << "\n*** Memory allocated successfully ***" << std::endl;
        getVirtualMemoryStatusByAddress((DWORD64)allocatedMemoryPtr);
    } else {
        std::cout << "\n*** Can not allocated memory ***" << std::endl;
        return;
    }

    // Release memory
    VirtualFree(allocatedMemoryPtr, 0, MEM_RELEASE);
    std::cout << "\n*** Memory released ***" << std::endl;
    getVirtualMemoryStatusByAddress((DWORD64)allocatedMemoryPtr);
}

/*
 * Returns the last Win32 error, in string format. Returns an empty string if there is no error.
 * Source: https://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror
 * */
std::string GetLastErrorAsString()
{
    //Get the error message, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0)
        return std::string(); //No error message has been recorded

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);

    //Free the buffer.
    LocalFree(messageBuffer);

    return message;
}

void writeToAddress() {

    LPVOID startAddress;
    std::cout << "Enter start address (enter 0 for automatic): 0x";
    std::cin >> std::hex >> startAddress;

    void *allocAddr = VirtualAlloc(startAddress, sizeof(int), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    /*
     * Check for success of memory reserve&commit
     * If error, print error no and human readable error
     * */
    if (!allocAddr) {
        std::cout << "Error while allocating: " << GetLastError() << std::endl;
        std::cout << GetLastErrorAsString();
        return;
    }

    // Print information about allocated memory and address
    MEMORY_BASIC_INFORMATION memInfo;
    VirtualQuery(allocAddr, &memInfo, sizeof(memInfo));
    std::cout << "Allocated " << memInfo.RegionSize << "B at " << std::hex << allocAddr << std::dec << std::endl;

    // Get and record data to memory
    std::cout << "Enter data: ";
    std::cin >> reinterpret_cast<char *>(allocAddr);
    std::cout << "Recorded data: " << reinterpret_cast<char *>(allocAddr) << std::endl;
}

void protectMemoryByAddress(){
    LPVOID startAddress;
    LPVOID memoryPointer;
    std::cout << "Enter start address: 0x";
    std::cin >> std::hex >> startAddress;

    DWORD flOldProtect;

    // Protect memory by address
    if (startAddress) {
        VirtualProtect(startAddress, sizeof(int), PAGE_NOACCESS, &flOldProtect);
        std::cout << "\n*** Memory protected ***" << std::endl;
        getVirtualMemoryStatusByAddress((DWORD64)startAddress);

    } else {
        // If memory is not allocated, reserve and commit it, protect after
        memoryPointer = VirtualAlloc(startAddress, sizeof(int), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (memoryPointer != nullptr) {
            std::cout << "\n*** Memory allocated ***" << std::endl;
            getVirtualMemoryStatusByAddress((DWORD64)memoryPointer);
        }

        VirtualProtect(memoryPointer, sizeof(int), PAGE_NOACCESS, &flOldProtect);

        std::cout << "\n*** Memory protected***" << std::endl;
        getVirtualMemoryStatusByAddress((DWORD64)memoryPointer);

        VirtualFree(memoryPointer, 0, MEM_RELEASE);
    }
}