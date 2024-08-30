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
        status_t            MapAttributesToMessage(const entry_ref *ref, BMessage* outAttrMsg);
        BView*              GetViewTemplateForType(const char* mimeType);
        status_t            SetupView(const BView* entityView, const BMessage* attrMsg);
        void                ShowUserError(const char* title, const char* message, status_t errorCode);
};
