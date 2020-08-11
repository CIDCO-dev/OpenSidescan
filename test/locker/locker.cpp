/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/* 
 * File:   locker.cpp
 * Author: Jordan McManus
 */

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/file.h>
#include <string>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "USAGE: locker [filename]" << std::endl;
        return 0;
    }

    std::string file(argv[argc - 1]);

    int fd = open(file.c_str(), O_RDWR | O_NOATIME);

    if (fd == -1) {
        std::cout << "Couldn't open file: " << file << std::endl;
        return 0;
    }

    struct flock flockStructForTest;
    memset(&flockStructForTest, 0, sizeof (flockStructForTest));

    flockStructForTest.l_type = F_WRLCK;
    if (fcntl(fd, F_SETLKW, &flockStructForTest) == -1) // F_SETLKW waits until lock obtained
    {
        std::cout << "Couldn't lock file" << std::endl;
        return 0;
    }
    /*Done Obtaining lock*/


    //sleep for 180 seconds, then release lock
    sleep(180);

    /*Unlock file*/
    flockStructForTest.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &flockStructForTest); // Release the lock
    close(fd);
    /*Done Unlock file*/

    return 0;
}

