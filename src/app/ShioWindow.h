/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <Mime.h>
#include <Window.h>
#include "ShioView.h"

class ShioWindow : public BWindow
{
    public:
                            ShioWindow(entry_ref *ref);
        virtual			    ~ShioWindow();

    protected:
        status_t            MapAttributesToMessage(const entry_ref *ref, const BMessage *mimeAttrInfo, BMessage* outAttrMsg);
        ShioView*           GetViewTemplateForType(const char* mimeType);
        void                ShowUserError(const char* title, const char* message, status_t errorCode);
        status_t            GetMimeTypeForRef(const entry_ref* ref, char* mimeType);
};
