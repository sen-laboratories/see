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
#include <fs_attr.h>
#include <MimeType.h>
#include <NodeInfo.h>
#include <Path.h>

#include "ShioWindow.h"

ShioWindow::ShioWindow(entry_ref* ref) : BWindow(
    BRect(BPoint(200.0, 128.0), BSize(240.0, 320.0)),
    "Shio Entity Viewer",
    B_DOCUMENT_WINDOW,
    B_WILL_ACCEPT_FIRST_CLICK)
{
    BMessage attrMsg;
    status_t result;

    result = MapAttributesToMessage(ref, &attrMsg);
    if (result != B_OK) {
        Close();
    }

    // get MIME type for view template lookup
    BMimeType mimeType;
    result = BMimeType::GuessMimeType(ref, &mimeType);
    if (result != B_OK) {
        ShowUserError("File identification error", "Could not identify MIME type of file.", result);
        Close();
    }

    BView *entityView = GetViewTemplateForType(mimeType.Type());
}

ShioWindow::~ShioWindow()
{
}

status_t ShioWindow::MapAttributesToMessage(const entry_ref *ref, BMessage* outAttrMsg)
{
    status_t result;

    BNode node(ref);
    result = node.InitCheck();

    if (result != B_OK) {
        return result;
    }

	char *attrName = new char[B_ATTR_NAME_LENGTH];
	int32 attrCount = 0;
	attr_info attrInfo;

	while ((result = node.GetNextAttrName(attrName)) == B_OK) {
		BString relationAttr(attrName);
        // omit known internal / system attributes
		if (relationAttr.StartsWith("BEOS:") || relationAttr.StartsWith("be:") || relationAttr.StartsWith("_trk/")) {
            continue;
        }
        result = node.GetAttrInfo(attrName, &attrInfo);
	    if (result != B_OK) {
		    ShowUserError("Error opening file", "Encountered an error reading attribute info from file!", result);
		    return result;
        }
		const void *data[attrInfo.size];
		ssize_t bytesRead = node.ReadAttr(attrName, attrInfo.type, 0, data, attrInfo.size);

		if (bytesRead <= 0) {
			ShowUserError("Error opening file", "failed to read attribute value from file.", result);
			return result;
		}

		// now add to message as typed field
		outAttrMsg->AddData(attrName, attrInfo.type, data, bytesRead);
		attrCount++;
	} // while

	if (result != B_ENTRY_NOT_FOUND) {
		ShowUserError("Error opening file", "failed to read file attributes.", result);
		return result;
	}

    outAttrMsg->PrintToStream();
    return B_OK;
}

BView* ShioWindow::GetViewTemplateForType(const char* mimeType) { return NULL; }

status_t ShioWindow::SetupView(const BView* entityView, const BMessage* attrMsg) { return B_OK; }

void ShioWindow::ShowUserError(const char *title, const char* message, status_t errorCode)
{
    BString msg(message);
    msg.Append("\nDetail: ").Append(strerror(errorCode));

    BAlert* alert = new BAlert(
        title,
        msg.String(),
        "Oh no.");

    alert->SetFlags(alert->Flags() | B_WARNING_ALERT | B_CLOSE_ON_ESCAPE);
    alert->Go();
}