/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <GroupView.h>

#include "ShioView.h"

class ShioGenericFormView : public ShioView
{
    public:
                    ShioGenericFormView();
        virtual	   ~ShioGenericFormView();
        // from ShioView
        BView*      GetView();
        bool        IsValid();
        status_t    Populate(const BMessage *mimeAttrInfo, const BMessage* attrs);

    protected:
        BView*      CreateDataView(const char* name, type_code typeCode, bool editable, const void* data);

    private:
        BGroupView* fView;
};
