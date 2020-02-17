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
    BoolWithMutex( bool value )
    : value( value )
    {
    }

    bool getValue() {
        QMutexLocker ml(&mutex);
        return value;
    }

    bool setValue( bool valueIn ) {
        QMutexLocker ml(&mutex);
        value = valueIn;
        return value;
    }

private:
    QMutex mutex;
    bool value;
};

#endif // BOOLWITHMUTEX_H
