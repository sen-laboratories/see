/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <Application.h>

class ShioApp : public BApplication
{
    public:
                            ShioApp();
        virtual			    ~ShioApp();
        virtual void        RefsReceived(BMessage* message);
        virtual void        ArgvReceived(int32 argc, char **argv);
};
