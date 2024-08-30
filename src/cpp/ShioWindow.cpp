/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 *
 * derived from QPDF example 'pdf-bookmarks.cc'
 * see project site at https://qpdf.sourceforge.io
 */

#include <Alert.h>
#include <Entry.h>
#include <Errors.h>
#include <Path.h>

#include "ShioWindow.h"

ShioWindow::ShioWindow(entry_ref* ref) : BWindow(
    BRect(BPoint(200.0, 128.0), BSize(240.0, 320.0)),
    "Shio Entity Viewer",
    B_DOCUMENT_WINDOW,
    B_WILL_ACCEPT_FIRST_CLICK)
{
}

ShioWindow::~ShioWindow()
{
}

status_t ShioWindow::ProcessFile(const entry_ref *ref, BMessage* outAttrMsg) { return B_OK; }
status_t ShioWindow::GetViewTemplateForType(const char* mimeType, BView* outView) { return B_OK; }
status_t ShioWindow::SetupView(const BMessage* attrMsg, const BView* entityView) { return B_OK; }
