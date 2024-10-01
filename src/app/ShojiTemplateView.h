/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#pragma once

#include <View.h>
#include "ShojiView.h"

#define Shoji_TEMPLATE_VIEW_TYPE "application/x.vnd-sen-labs.Shoji.templateView"

class ShojiTemplateView : public ShojiView
{
    public:
                    ShojiTemplateView(const char* mimeType);
        virtual	   ~ShojiTemplateView();
        // from ShojiView
        BView*      GetView();
        bool        IsValid();
        status_t    Populate(const BMessage *mimeAttrInfo, const BMessage* attrs);

    protected:
        status_t    LookupView(const char* mimeType, BView* view);
        void        PopulateControl(BView *view, const void** data);

    private:
        BView*      fTemplateView;
};
