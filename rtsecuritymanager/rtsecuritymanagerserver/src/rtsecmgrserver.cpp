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





#include <e32svr.h>
#include <e32uid.h>

#include "rtsecmgrserver.h"
#include "rtsecmgrsession.h"
#include "rtsecmgrsubsession.h"
#include "rtsecmgrcommondef.h"
#include "rtsecmgrstore.h"
#include "rtsecmgrpolicymanager.h"
#include "rtsecmgrscriptmanager.h"

CRTSecMgrServer* CRTSecMgrServer::NewL(CActive::TPriority aActiveObjectPriority)
	{
	CRTSecMgrServer* self=new (ELeave) CRTSecMgrServer(aActiveObjectPriority);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CRTSecMgrServer* CRTSecMgrServer::NewLC(CActive::TPriority aActiveObjectPriority)
	{
	CRTSecMgrServer* self=new (ELeave) CRTSecMgrServer(aActiveObjectPriority);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CRTSecMgrServer::ConstructL()
	{
	StartL(KSecServerProcessName);
	iContainerIndex = CObjectConIx::NewL();
	
	iSecMgrDb = CSecMgrStore::NewL();	
	iPolicyMgr = CPolicyManager::NewL(iSecMgrDb);
	iPolicyMgr->RestorePoliciesL();
	
	iScriptMgr = CScriptManager::NewL(iSecMgrDb,iPolicyMgr);
	
	// Start the shutDown timer
	iShutDown.ConstructL();

	iShutDown.Start ();
	}

CRTSecMgrServer::~CRTSecMgrServer()
	{
	if(iPolicyMgr)
	{
		delete iPolicyMgr;			
	}
	
	if(iScriptMgr)
	{
		delete iScriptMgr;
	}
	
	if(iSecMgrDb)
	{
		delete iSecMgrDb;
	}
			
	Delete(iContainerIndex); 
	
	iShutDown.Cancel();
	}

CSession2* CRTSecMgrServer::NewSessionL(const TVersion &aVersion,const RMessage2& /*aMessage*/) const
	{
	  // Check that the version is OK
	TVersion v(KRTSecMgrServMajorVersionNumber,KRTSecMgrServMinorVersionNumber,KRTSecMgrServBuildVersionNumber);
	if (!User::QueryVersionSupported(v,aVersion))
		User::Leave(KErrNotSupported);
	
	return CRTSecMgrSession::NewL();
	}
	

    
