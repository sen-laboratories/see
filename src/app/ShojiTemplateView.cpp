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

#include "ShojiTemplateView.h"

/*
 * generic dynamic view to use when no MIME specific view is available for the source file.
 * Builds a view containing fields and controls according to the attribute type of the source file attributes.
 */
ShojiTemplateView::ShojiTemplateView(const char* mimeType) : ShojiView()
{
    status_t result = LookupView(mimeType, fTemplateView);
    if (result != B_OK) {
        fTemplateView = NULL;
/*
        BString error("");
        if (result == B_ENTRY_NOT_FOUND) {
            error.Append("No template view found for type ");
        } else if (result == B_FILE_ERROR) {
            error.Append("Could not read template for type ");
            error.Append(mimeType).Append(":\n").Append(strerror(result));
        } else {
            error.Append("Could not instantiate view for type ");
            error.Append(mimeType).Append(":\n").Append(strerror(result));
        }
        error.Append("\nFalling back to generic form view.");

        BAlert alert("Error setting up view", error.String(), "OK");
        alert.SetFlags(alert.Flags() | B_WARNING_ALERT | B_CLOSE_ON_ESCAPE);
        alert.Go();
        */
    }
}

ShojiTemplateView::~ShojiTemplateView()
{
}

BView* ShojiTemplateView::GetView()
{
    return fTemplateView;
}

bool ShojiTemplateView::IsValid() {
    return fTemplateView != NULL;
}

status_t ShojiTemplateView::LookupView(const char* mimeType, BView* view) {
    BPath path;
    status_t status;

	if (find_directory(B_USER_SETTINGS_DIRECTORY, &path) != B_OK)
    {
		path.SetTo("./templates");
    } else
    {
        path.Append("Shoji/templates");
    }
	BDirectory templatesDir(path.Path());
	if (! templatesDir.Contains(path.Path())) {
		status = templatesDir.CreateDirectory(path.Path(), NULL);
        if (status != B_OK) {
            return status;
        }
        return B_ENTRY_NOT_FOUND;
    }

    // now search for template corresponding to the mimetype
    path.Append(mimeType);
    templatesDir.SetTo(path.Path());

	if (templatesDir.InitCheck() != B_OK) {
        return status;
    }

    BFile templateFile(path.Path(), B_READ_ONLY);
    BMessage archivedViewMsg;
    status = archivedViewMsg.Unflatten(&templateFile);
    if (status != B_OK) {
        return status;
    }

    fTemplateView = new BView(&archivedViewMsg);
    return B_OK;
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

        BView*    view = fTemplateView->FindView(name);
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
