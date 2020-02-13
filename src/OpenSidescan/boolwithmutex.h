/*
* Copyright 2019 © Centre Interdisciplinaire de développement en Cartographie des Océans (CIDCO), Tous droits réservés
*/

 /*
 * \author Christian Bouchard
 */

#ifndef BOOLWITHMUTEX_H
#define BOOLWITHMUTEX_H

#include <QMutex>
#include <QMutexLocker>

// Based on https://stackoverflow.com/questions/8971168/how-to-use-qmutex

class BoolWithMutex
{
public:
    BoolWithMutex( bool stateStart )
    : state( stateStart )
    {
    }

    bool getValue() {
        QMutexLocker ml(&mutex);
        return state;
    }

    bool setValue( bool stateIn ) {
        QMutexLocker ml(&mutex);
        state = stateIn;
        return state;
    }

private:
    QMutex mutex;
    bool state;
};

#endif // BOOLWITHMUTEX_H
