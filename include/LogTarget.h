#pragma once

#ifndef SPACESHOOTER3D_LOGTARGET_H
#define SPACESHOOTER3D_LOGTARGET_H

// Local Includes
#include "defines.h"

// Prototypes

class ILogTarget
{
public:
    ILogTarget() = default;
    virtual ~ILogTarget() = default;

    /**
     *
     * This function is responsible for writing a message.
     *
     * @param _pcMessage is the message to be written.
     * @return Returns void.
     *
     */
    virtual void Write(const Int8* _pcMessage) = 0;
};

#endif // SPACESHOOTER3D_LOGTARGET_H
