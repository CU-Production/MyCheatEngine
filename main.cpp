#include <windows.h>
#include <TlHelp32.h>
#include <iostream>

DWORD findPidByName(const char* name) {
    HANDLE h;
    PROCESSENTRY32 singleProcess;
    h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    singleProcess.dwSize = sizeof(PROCESSENTRY32);

    do {
        if (strcmp(singleProcess.szExeFile, name) == 0) {
            DWORD pid = singleProcess.th32ProcessID;
            CloseHandle(h);
            return pid;
        }
    } while (Process32Next(h, &singleProcess));

    CloseHandle(h);

    return 0;
}

int main() {
    DWORD pid = findPidByName("victim.exe");

    if (pid == 0) {
        std::cout << "couldn't find victim.exe process" << std::endl;
    } else {
        std::cout << "Found the victim process!" << std::endl;

        HANDLE handleToProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

        if (!handleToProcess) {
            std::cout << "couldn't open victim.exe process" << std::endl;
        } else {
            std::cout << "Enter memory address:" << std::endl;
            void* address;
            std::cin >> address;

            int data = 0;
            ReadProcessMemory(handleToProcess, address, &data, sizeof(data), NULL);

            std::cout << "data old value: " << data << std::endl;

            int newData = 54321;
            if (WriteProcessMemory(handleToProcess, address, &newData, sizeof(newData), NULL)) {
                std::cout << "Successfully written the data!" << std::endl;
            }
        }
    }

    // std::cin.ignore();
    // std::cin.get();

    return 0;
}
