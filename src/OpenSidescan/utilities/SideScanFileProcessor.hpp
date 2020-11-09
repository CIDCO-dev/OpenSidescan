/*
 * Copyright 2020 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
 */

/* 
 * File:   SideScanFileProcessor.hpp
 * Author: Jordan McManus
 */

#ifndef SIDESCANFILEPROCESSOR_HPP
#define SIDESCANFILEPROCESSOR_HPP

class SideScanFileProcessor {
public:
    
    virtual void processFile(SidescanFile * f) = 0;

    virtual void reportProgress(std::string progress) = 0;

};

#endif /* SIDESCANFILEPROCESSOR_HPP */

