/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:       Defines CRTSecManager and CRTSecMgrClientProxy classes
 *
*/






#include <coemain.h>
#include <rtsecmanager.h>
#include <rtsecmgrscriptsession.h>
#include "rtsecmgrclient.h"
#include "rtsecmgrtracer.h"

/**
 * Proxy delegate class for runtime security manager client-side
 * session handle.
 * 
 * @lib rtsecmgrclient.lib
 */
class CRTSecMgrClientProxy : public CBase
	{
public:
	CRTSecMgrClientProxy()
		{
		}

	void ConstructL()
		{
		User::LeaveIfError (iSecSession.Connect ());
		}

	static CRTSecMgrClientProxy* NewL()
		{
		CRTSecMgrClientProxy* self = CRTSecMgrClientProxy::NewLC ();
		CleanupStack::Pop (self);
		return self;
		}

	static CRTSecMgrClientProxy* NewLC()
		{
		CRTSecMgrClientProxy* self = new (ELeave) CRTSecMgrClientProxy();
		CleanupStack::PushL (self);
		self->ConstructL ();
		return self;
		}

	virtual ~CRTSecMgrClientProxy()
		{
		iSecSession.Close ();
		}

	RSecMgrSession& Session()
		{
		return iSecSession;
		}
	RSecMgrSession* operator ->()
	{
		return &iSecSession;
	}
private:
	RSecMgrSession iSecSession;
	};

// ---------------------------------------------------------------------------
// Defintiion of default private constructor
// ---------------------------------------------------------------------------
//
CRTSecManager::CRTSecManager()
	{
	}

// ---------------------------------------------------------------------------
// Definition of second phase constructor
//
// Instantiates client proxy object, in turn creating a client-side
// session
// ---------------------------------------------------------------------------
//
void CRTSecManager::ConstructL()
	{
	iClientProxy = CRTSecMgrClientProxy::NewL ();

#ifdef _DEBUG
	if(CCoeEnv::Static())
	{
	CCoeEnv::Static()->DisableExitChecks(ETrue); 
	}	
#endif
	}

// ---------------------------------------------------------------------------
// Definition of second phase constructor
//
// Constructs a CRTSecManager instance
// ---------------------------------------------------------------------------
//
EXPORT_C CRTSecManager* CRTSecManager::NewL()
	{
	RTSecMgrCreateTraceFile() ;
	CRTSecManager* self = CRTSecManager::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Definition of second phase constructor
//
// Constructs a CRTSecManager instance and leaves the created instance
// on the cleanupstack
// ---------------------------------------------------------------------------
//
EXPORT_C CRTSecManager* CRTSecManager::NewLC()
	{
	CRTSecManager* self = new (ELeave) CRTSecManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
//
// Closes client-side session in turn closing the peer server
// side session
// ---------------------------------------------------------------------------
//
EXPORT_C CRTSecManager::~CRTSecManager()
	{
	delete iClientProxy;
	}

// ---------------------------------------------------------------------------
// Definition of SetPolicy
//
// This method delegates the actual handling to the client-side session
// handle class
// ---------------------------------------------------------------------------
EXPORT_C TPolicyID CRTSecManager::SetPolicy(const RFile& aSecPolicy)
	{
	RTSecMgrTraceFunction("CRTSecManager::SetPolicy(const RFile& aSecPolicy)") ;
	return (*iClientProxy)->SetPolicy(aSecPolicy);
	}

// ---------------------------------------------------------------------------
// Definition of SetPolicy
//
// This method delegates the actual handling to the client-side session
// handle class
// ---------------------------------------------------------------------------
EXPORT_C TPolicyID CRTSecManager::SetPolicy(const TDesC8& aPolicyBuffer)
	{
	RTSecMgrTraceFunction("CRTSecManager::SetPolicy(const TDesC8& aPolicyBuffer)") ;
	return (*iClientProxy)->SetPolicy(aPolicyBuffer);
	}

// ---------------------------------------------------------------------------
// Definition of UnSetPolicy
//
// This method delegates the actual handling to the client-side session
// handle class
// ---------------------------------------------------------------------------
EXPORT_C TInt CRTSecManager::UnSetPolicy(TPolicyID aPolicyID)
	{
	RTSecMgrTraceFunction("CRTSecManager::UnSetPolicy(TPolicyID aPolicyID)") ;
	return (*iClientProxy)->UnSetPolicy(aPolicyID);
	}

// ---------------------------------------------------------------------------
// Definition of UpdatePolicy
//
// This method delegates the actual handling to the client-side session
// handle class
// ---------------------------------------------------------------------------
EXPORT_C TPolicyID CRTSecManager::UpdatePolicy(TPolicyID aPolicyID,const RFile& aSecPolicy)
	{
	RTSecMgrTraceFunction("CRTSecManager::UpdatePolicy(TPolicyID aPolicyID,const RFile& aSecPolicy)") ;
	return (*iClientProxy)->UpdatePolicy(aPolicyID,aSecPolicy);
	}

// ---------------------------------------------------------------------------
// Definition of UpdatePolicy
//
// This method delegates the actual handling to the client-side session
// handle class
// ---------------------------------------------------------------------------
EXPORT_C TPolicyID CRTSecManager::UpdatePolicy(TPolicyID aPolicyID,const TDesC8& aPolicyBuffer)
	{
	RTSecMgrTraceFunction("CRTSecManager::UpdatePolicy(TPolicyID aPolicyID,const TDesC8& aPolicyBuffer)") ;
	return (*iClientProxy)->UpdatePolicy(aPolicyID,aPolicyBuffer);
	}

// ---------------------------------------------------------------------------
// Definition of RegisterScript
//
// This method delegates the actual handling to the client-side session
// handle class
// ---------------------------------------------------------------------------
EXPORT_C TExecutableID CRTSecManager::RegisterScript(TPolicyID aPolicyID, const CTrustInfo& aTrustInfo)
	{
	RTSecMgrTraceFunction("CRTSecManager::RegisterScript(TPolicyID aPolicyID, const CTrustInfo& aTrustInfo)") ;
	return (*iClientProxy)->RegisterScript(aPolicyID,aTrustInfo);
	}

// ---------------------------------------------------------------------------
// Definition of RegisterScript with script having hash value
//
// This method delegates the actual handling to the client-side session
// handle class
// ---------------------------------------------------------------------------
EXPORT_C TExecutableID CRTSecManager::RegisterScript(TPolicyID aPolicyID, const TDesC& aHashMarker, const CTrustInfo& aTrustInfo)
	{
    RTSecMgrTraceFunction("CRTSecManager::RegisterScript(TPolicyID aPolicyID, const TDesC& aHashMarker, const CTrustInfo& aTrustInfo)") ;
	return (*iClientProxy)->RegisterScript(aPolicyID,aHashMarker,aTrustInfo);
	}

// ---------------------------------------------------------------------------
// Definition of UnRegisterScript
//
// This method delegates the actual handling to the client-side session
// handle class
// ---------------------------------------------------------------------------
EXPORT_C TInt CRTSecManager::UnRegisterScript(TExecutableID aExeID, TPolicyID aPolicyID)
	{
    RTSecMgrTraceFunction("CRTSecManager::UnRegisterScript(TExecutableID aExeID, TPolicyID aPolicyID)") ;
	return (*iClientProxy)->UnRegisterScript(aExeID,aPolicyID);
	}

// ---------------------------------------------------------------------------
// Definition of GetScriptSession
//
// This method starts a client-side sub-session handle, modelling script
// session. This in turn creates a server-side peer sub-session handle
// ---------------------------------------------------------------------------
/*EXPORT_C CRTSecMgrScriptSession* CRTSecManager::GetScriptSession(TPolicyID aPolicyID,
		TExecutableID aExecID,
		MSecMgrPromptHandler* aPromptHdlr,const TDesC& aHashValue)
	{
	if(aExecID<=KAnonymousScript)
		return NULL;

	CRTSecMgrScriptSession* scriptSession = CRTSecMgrScriptSession::NewLC(aPromptHdlr);

	if(KErrNone==scriptSession->Open(iClientProxy->Session(),aPolicyID,aExecID))
		{
		CleanupStack::Pop(scriptSession);
		return scriptSession;
		}

	if(scriptSession)
		CleanupStack::PopAndDestroy(scriptSession);

	return NULL;
	}*/


// ---------------------------------------------------------------------------
// Definition of GetScriptSession
//
// This method starts a client-side sub-session handle, modelling script
// session. This in turn creates a server-side peer sub-session handle
// ---------------------------------------------------------------------------
EXPORT_C CRTSecMgrScriptSession* CRTSecManager::GetScriptSessionL(TPolicyID aPolicyID,
		const CTrustInfo& aTrustInfo,
		MSecMgrPromptHandler* aPromptHdlr)
	{
    RTSecMgrTraceFunction("CRTSecManager::GetScriptSession(TPolicyID aPolicyID,\
		                   const CTrustInfo& aTrustInfo,\
		                   MSecMgrPromptHandler* aPromptHdlr)") ;
	CRTSecMgrScriptSession* scriptSession = CRTSecMgrScriptSession::NewLC(aPromptHdlr);

	if(KErrNone==scriptSession->Open(iClientProxy->Session(),aPolicyID,KAnonymousScript,aTrustInfo))
		{
		CleanupStack::Pop(scriptSession);
		return scriptSession;
		}

	if(scriptSession)
		CleanupStack::PopAndDestroy(scriptSession);

	return NULL;
	}

// ---------------------------------------------------------------------------
// Definition of GetScriptSession
//
// This method starts a client-side sub-session handle, modelling script
// session. This in turn creates a server-side peer sub-session handle
// ---------------------------------------------------------------------------
EXPORT_C CRTSecMgrScriptSession* CRTSecManager::GetScriptSessionL(TPolicyID aPolicyID,
		TExecutableID aExecID,
		MSecMgrPromptHandler* aPromptHdlr,
		const TDesC& aHashValue)
	{
	RTSecMgrTraceFunction("CRTSecManager::GetScriptSession(TPolicyID aPolicyID,\
		                   TExecutableID aExecID,MSecMgrPromptHandler* aPromptHdlr,\
		                   const TDesC& aHashValue)") ;
	if(aExecID<=KAnonymousScript)
		return NULL;
	
	CRTSecMgrScriptSession* scriptSession = CRTSecMgrScriptSession::NewLC(aPromptHdlr);

	if(KErrNone==scriptSession->Open(iClientProxy->Session(),aPolicyID,aExecID,aHashValue))
		{
		CleanupStack::Pop(scriptSession);
		return scriptSession;
		}

	if(scriptSession)
		CleanupStack::PopAndDestroy(scriptSession);

	return NULL;
	}

