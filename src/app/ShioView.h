/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <View.h>

class ShioView
{
    public:
        virtual BView*      GetView() { return NULL; }
        virtual bool        IsValid() { return false; }
        virtual status_t    Populate(const BMessage *mimeAttrInfo, const BMessage* attrs) { return B_ERROR; }
};
