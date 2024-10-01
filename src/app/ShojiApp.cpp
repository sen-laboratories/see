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

#include "ShojiApp.h"
#include "ShojiWindow.h"

const char* kApplicationSignature = "application/x-vnd.sen-labs.Shoji";

ShojiApp::ShojiApp() : BApplication(kApplicationSignature)
{
}

ShojiApp::~ShojiApp()
{
}

void ShojiApp::ArgvReceived(int32 argc, char **argv)
{
    if (argc < 1) {
        // todo: show empty splash screen and allow drag&drop
        return;
    }

    BMessage refsMsg(B_REFS_RECEIVED);
    BEntry entry(argv[1]);
    entry_ref ref;

    entry.GetRef(&ref);
    refsMsg.AddRef("refs", &ref);

    RefsReceived(&refsMsg);
}

void ShojiApp::RefsReceived(BMessage *message)
{
    entry_ref ref;

    if (message->FindRef("refs", &ref) != B_OK) {
        BAlert* alert = new BAlert("Error launching SHOI",
            "Failed to resolve source file.",
            "Oh no.");
        alert->SetFlags(alert->Flags() | B_WARNING_ALERT | B_CLOSE_ON_ESCAPE);

        return;
    }

    (new ShojiWindow(new entry_ref(ref.device, ref.directory, ref.name)))->Show();
}

int main()
{
	ShojiApp* app = new ShojiApp();
    if (app->InitCheck() != B_OK) {
        return 1;
    }
	app->Run();

	delete app;
	return 0;
}
