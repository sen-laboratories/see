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
#include "ShioView.h"
#include "ShioGenericFormView.h"
#include "ShioTemplateView.h"

ShioWindow::ShioWindow(entry_ref* ref) : BWindow(
    BRect(BPoint(320.0, 128.0), BSize(480.0, 576.0)),
    "Shio Entity Viewer",
    B_DOCUMENT_WINDOW,
    B_WILL_ACCEPT_FIRST_CLICK)
{
	SetLayout(new BGroupLayout(B_VERTICAL, 0));
    status_t result;

    // get MIME type for view template lookup
    char mimeTypeStr[B_MIME_TYPE_LENGTH];
    BMimeType mimeType;

    result = GetMimeTypeForRef(ref, mimeTypeStr);
    if (result == B_OK) {
        mimeType.SetTo(mimeTypeStr);
        result = mimeType.InitCheck();
    }
    if (result != B_OK) {
        ShowUserError("MIME type lookup error", "Could not identify MIME type of file.", result);
        Close();
    }
    // store MIME type attribute info to check for displayable or editable flags
    BMessage mimeAttrInfo;
    result = mimeType.GetAttrInfo(&mimeAttrInfo);
    if (result != B_OK) {
        ShowUserError("MIME attrInfo lookup error", "Could not identify MIME type attributeInfo for filetype.", result);
        Close();
    }
    BMimeType superType;
    BMessage superTypeAttrInfo;
    result = mimeType.GetSupertype(&superType);
    superType.GetAttrInfo(&superTypeAttrInfo);

    mimeAttrInfo.Append(superTypeAttrInfo);

    // look up suitable template by mimeType
    ShioView *entityView = GetViewTemplateForType(mimeType.Type());

    BMessage attrMsg;
    result = MapAttributesToMessage(ref, &mimeAttrInfo, &attrMsg);
    if (result != B_OK) {
        ShowUserError("Error opening view", "Could not map data for display.", result);
        Close();
    }

    entityView->Populate(&mimeAttrInfo, &attrMsg);
    AddChild(entityView->GetView());
}

ShioWindow::~ShioWindow()
{
}

status_t ShioWindow::MapAttributesToMessage(const entry_ref *ref, const BMessage *mimeAttrInfo, BMessage* outAttrMsg)
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
        // always omit known internal / system attributes
        // todo: maintain a BStringList and check with ContainsString() here, e.g. MediaThumbnail etc.
		if (relationAttr.StartsWith("BEOS:") || relationAttr.StartsWith("be:") || relationAttr.StartsWith("_trk/")) {
            continue;
        }
        result = node.GetAttrInfo(attrName, &attrInfo);
	    if (result != B_OK) {
		    ShowUserError("Error opening file", "Encountered an error reading attribute info from file!", result);
		    return result;
        }
        // omit items that should not be displayed
        if (! mimeAttrInfo->FindBool("attr:viewable", attrCount)) {
            continue;   // skip attribute as it should not be displayed
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

    outAttrMsg->PrintToStream();    // DEBUG
    return B_OK;
}

// look up suitable view based on MIME type, fall back to generic form view if not available.
ShioView* ShioWindow::GetViewTemplateForType(const char* mimeType) {
    ShioView* view = new ShioTemplateView(mimeType);

    BString message;
    if (view->IsValid()) {
        message.Append("Open template view for type ");
    } else {
        message.Append("Open generic view for type ");
        delete view;
        view = new ShioGenericFormView();
    }
    message.Append(mimeType);
    ShowUserError("Debug", message.String(), B_OK);

    return view;
}

status_t ShioWindow::GetMimeTypeForRef(const entry_ref* ref, char* mimeType) {
    BNode sourceNode(ref);
    status_t result;
    if ((result = sourceNode.InitCheck()) != B_OK) {
        return result;
    }
    BNodeInfo sourceInfo(&sourceNode);
    if ((result = sourceInfo.InitCheck()) != B_OK) {
        return result;
    }
    if ((result = sourceInfo.GetType(mimeType)) != B_OK) {
        return result;
    }

    return B_OK;
}

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