/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:      
*
*/





#include <e32base.h>
#include "rtsecmgrserver.h"
#include "rtsecmgrserverdef.h"

TInt E32Main(); // Process entry point

static void RunServerL()
	{
	// Naming the server process and thread after the startup helps to debug panics
	// No error checking as names are not critical for operation
	User::RenameProcess (KSecServerProcessName);
	User::RenameThread (KSecSrvMainThreadName);

	// Set process priority
	RProcess svrProcess;
	svrProcess.SetPriority (EPriorityHigh);
	svrProcess.Close ();

	// Create and install the active scheduler we need
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL (scheduler);

	CActiveScheduler::Install ( scheduler);
	CRTSecMgrServer* server=  NULL;
	server = CRTSecMgrServer::NewLC (CActive::EPriorityStandard);

	// Initialisation complete, now signal the client
	RProcess::Rendezvous (KErrNone);
	CActiveScheduler::Start ();
	// Cleanup
	CleanupStack::PopAndDestroy (server);
	CleanupStack::PopAndDestroy (scheduler);
	}

/*
 * Process main function
 * 
 */
TInt E32Main()
	{
	__UHEAP_MARK;
	TInt i = User::CountAllocCells();
	TInt err(KErrNone);

	// Start server.
	CTrapCleanup* cleanup = CTrapCleanup::New ();
	err = KErrNoMemory;

	if ( cleanup)
		{
		TRAP(err, RunServerL());
		
		delete cleanup;
		cleanup = NULL;
		}
	i = User::CountAllocCells();
	__UHEAP_MARKEND;

	return err;
	}

//  End of File  

