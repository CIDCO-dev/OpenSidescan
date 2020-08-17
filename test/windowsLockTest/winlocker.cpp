/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/* 
 * File:   winlocker.cpp
 * Author: Jordan McManus
 */

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    
    if (argc != 2) {
        std::cout << "USAGE: locker [filename]" << std::endl;
        return 0;
    }
    
    std::string file(argv[argc - 1]);
    
    std::cout << "Locking file: " << file << std::endl;
    
    LPCSTR lpFileName = file.c_str();
    
    HANDLE hFile = CreateFileA(lpFileName,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL, // no security attributes
            OPEN_EXISTING,
            0,
            NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Couldn't get handle on file: " << file << std::endl;
        return 0;
    }
    
    OVERLAPPED sOverlapped;
    sOverlapped.Offset = 0;
    sOverlapped.OffsetHigh = 0;
    
    BOOL fSuccess = FALSE;
    
    fSuccess = LockFileEx(hFile, // exclusive access,
            LOCKFILE_EXCLUSIVE_LOCK |
            LOCKFILE_FAIL_IMMEDIATELY,
            0, // reserved, must be zero
            MAXDWORD, // number of bytes to lock: lock the entire file
            MAXDWORD,
            &sOverlapped); // contains the file offset
    
    if (!fSuccess) {
        CloseHandle(hFile);
        std::cout << "Couldn't get lock on file: " << file << std::endl;
        return 0;
    }
    
    Sleep(5000);
    
    std::cout << "Unlocking file: " << file << std::endl;
    
    // Unlock the file.
    fSuccess = UnlockFileEx(hFile,
            0, // reserved, must be zero
            MAXDWORD, // num. of bytes to unlock
            MAXDWORD,
            &sOverlapped); // contains the file offset
    
    if (!fSuccess) {
        CloseHandle(hFile);
        std::cout << "Couldn't unlock on file: " << file << std::endl;
        return 0;
    }
    
    
    CloseHandle(hFile);
    return 0;
}

