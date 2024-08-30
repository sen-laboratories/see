/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <Window.h>

class ShioWindow : public BWindow
{
    public:
                            ShioWindow(entry_ref *ref);
        virtual			    ~ShioWindow();

    protected:
        status_t            ProcessFile(const entry_ref *ref, BMessage* outAttrMsg);
        status_t            GetViewTemplateForType(const char* mimeType, BView* outView);
        status_t            SetupView(const BMessage* attrMsg, const BView* entityView);
};
