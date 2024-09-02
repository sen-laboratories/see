/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <View.h>
#include "ShioView.h"

#define SHIO_TEMPLATE_VIEW_TYPE "application/x.vnd-sen-labs.shio.templateView"

class ShioTemplateView : public ShioView
{
    public:
                    ShioTemplateView(const char* mimeType);
        virtual	   ~ShioTemplateView();
        // from ShioView
        BView*      GetView();
        bool        IsValid();
        status_t    Populate(const BMessage *mimeAttrInfo, const BMessage* attrs);

    protected:
        status_t    LookupView(const char* mimeType, BView* view);
        void        PopulateControl(BView *view, const void** data);

    private:
        BView*      fTemplateView;
};
