/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <Application.h>

class ShojiApp : public BApplication
{
    public:
                            ShojiApp();
        virtual			    ~ShojiApp();
        virtual void        RefsReceived(BMessage* message);
        virtual void        ArgvReceived(int32 argc, char **argv);
};
