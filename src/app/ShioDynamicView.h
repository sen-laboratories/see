/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <GroupView.h>

class ShioDynamicView : public BGroupView
{
    public:
                    ShioDynamicView(const BMessage *props);
        virtual	   ~ShioDynamicView();

    protected:
        status_t    Populate(const BMessage *props);
        BView*      CreateDataView(const char* name, type_code typeCode, const void* data);
};
