/*
 * Copyright 2024, Gregor B. Rosenauer <gregor.rosenauer@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include <Alert.h>
#include <Directory.h>
#include <Errors.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>
#include <TextControl.h>
#include <cstdio>

#include "ShojiTemplateView.h"

/*
 * custom MIME-specific template view for the provided source file type.
 * Builds a view from an archived view serving as a template and populates view controls
 * named after source file attributes according to type and attribute value.
 */
ShojiTemplateView::ShojiTemplateView(const char* mimeType) : ShojiView()
{
    fType = mimeType;
}

ShojiTemplateView::~ShojiTemplateView()
{
}

const char* ShojiTemplateView::GetType() {
    return fType;
}

status_t ShojiTemplateView::Initialize() {
    return LookupView(fType, fView);
}

status_t ShojiTemplateView::LookupView(const char* mimeType, BView* view) {
    BPath path;
    BDirectory templatesDir;
    status_t status;

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) != B_OK)
    {
        templatesDir.SetTo(".");
        if (! templatesDir.Contains("templates", B_DIRECTORY_NODE)) {
            return B_ENTRY_NOT_FOUND;
        }
        templatesDir.SetTo("templates");
        printf("could not access user settings dir, falling back to current dir.\n");
    } else {
        path.Append(SHOJI_TEMPLATE_PATH);
        status = create_directory(path.Path(), B_READ_WRITE);
        if (status != B_OK) {
            printf("failed to create settings dir %s: %s\n", path.Path(), strerror(status));
            return status;
        }
        templatesDir.SetTo(path.Path());
        printf("Created settings dir.\n");
    }
    if ((status = templatesDir.InitCheck()) != B_OK) {
        return status;
    }

    // now search for template corresponding to the mimetype
    if (! templatesDir.Contains(mimeType, B_DIRECTORY_NODE)) {
        printf("no view template found for MIME type %s.\n", mimeType);
        return B_ENTRY_NOT_FOUND;
    }
    status = templatesDir.SetTo(mimeType);
	if (status != B_OK) {
        return status;
    }
    // view template is stored in mime type dir as archived view
    // this directory will contain other assets later, like DataProviders.
    if (templatesDir.Contains(SHOJI_TEMPLATE_NAME)) {
        return B_ENTRY_NOT_FOUND;
    }
    BFile templateFile(&templatesDir, SHOJI_TEMPLATE_NAME, B_READ_ONLY);
    if ((status = templateFile.InitCheck()) != B_OK) {
        return status;
    }
    BMessage archivedViewMsg;
    status = archivedViewMsg.Unflatten(&templateFile);
    if (status != B_OK) {
        return status;
    }

    fView = new BView(&archivedViewMsg);
    return (fView != NULL ? B_OK : B_ERROR);
}

status_t ShojiTemplateView::Populate(const BMessage *mimeAttrInfo, const BMessage *props)
{
    const void* data;
    char*       name;
    ssize_t     size;
    type_code   type;
    status_t    result;

    int32 count = props->CountNames(B_ANY_TYPE);
    int32 countFieldItems;

    for (int32 i = 0; i < count; i++) {
        status_t result = props->GetInfo(B_ANY_TYPE, i, &name, &type, &countFieldItems);
		if (result != B_OK)
			continue;

        if (countFieldItems != 1) {
            BString error("Could not process message data ");
            error.Append(name).Append("\nMultiple items per field not supported.");

            BAlert alert("Error setting up view", error.String(), "OK");
            alert.SetFlags(alert.Flags() | B_STOP_ALERT | B_CLOSE_ON_ESCAPE);
            alert.Go();
            return B_ERROR;
        }

        result = props->FindData(name, type, &data, &size);
        if (result != B_OK) {
            BString error("Could not read data ");
            error.Append(name);
            BAlert alert("Error setting up view", error.String(), "OK");
            alert.SetFlags(alert.Flags() | B_STOP_ALERT | B_CLOSE_ON_ESCAPE);
            alert.Go();
            return B_ERROR;
        }

        bool editable = props->FindBool("attr:editable");

        BView*    view = fView->FindView(name);
        // sanity check
        if (BControl* control = dynamic_cast<BControl*>(view)) {
            control->SetEnabled(editable);
        }
        PopulateControl(view, &data);
    }

    return B_OK;
}

void ShojiTemplateView::PopulateControl(BView *view, const void** data) {
    if (BTextControl* tc = dynamic_cast<BTextControl*>(view)) {
        tc->SetText(reinterpret_cast<const char*>(data));
    }
}
