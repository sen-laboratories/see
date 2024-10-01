/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <Mime.h>
#include <Window.h>
#include "ShojiView.h"

class ShojiWindow : public BWindow
{
    public:
                            ShojiWindow(entry_ref *ref);
        virtual			    ~ShojiWindow();

    protected:
        status_t            MapAttributesToMessage(const entry_ref *ref, const BMessage *mimeAttrInfo, BMessage* outAttrMsg);
        ShojiView*           GetViewTemplateForType(const char* mimeType);
        void                ShowUserError(const char* title, const char* message, status_t errorCode);
        status_t            GetMimeTypeForRef(const entry_ref* ref, char* mimeType);
};
