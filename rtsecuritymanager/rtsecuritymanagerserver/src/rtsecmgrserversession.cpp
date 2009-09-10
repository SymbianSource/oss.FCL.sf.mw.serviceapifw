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
#include "rtsecmgrdata.h"
#include "rtsecmgrpolicyparser.h"
#include "rtsecmgrsession.h"
#include "rtsecmgrsubsession.h"

/**
 * Called by client/server framework after 
 * session has been successfully created.
 * In effect, a second-phase constructor.
 * creates:
 * 
 *  1. the object index
 *  2. the object container for this session.
 *
 */
void CRTSecMgrSession::CreateL()
	{
	if(!iSecMgrServer)
		iSecMgrServer = dynamic_cast<CRTSecMgrServer*> (const_cast<CServer2*> (Server()));
		
	iSecMgrServer->AddSession ();

	iSubSessionObjectIndex = CObjectIx::NewL ();
	iContainer = iSecMgrServer->NewContainerL ();
	}

void CRTSecMgrSession::CloseSession()
	{
	//delete the object index (this stores the objects for this session)
	Delete (iSubSessionObjectIndex);
	//delete the object container (this provides unique ids for the objects of this session )

	iSecMgrServer->RemoveFromContainerIx (iContainer);
	iSecMgrServer->RemoveSession ();
	}

/**
 * A simple utility function to return the appropriate 
 * CRTSecMgrSubSession object given a client's subsession handle.
 */
CRTSecMgrSubSession* CRTSecMgrSession::SubSessionFromHandle(
		const RMessage2& aMessage, TInt aHandle)
	{
	CRTSecMgrSubSession* subSession = (CRTSecMgrSubSession*)iSubSessionObjectIndex->At(aHandle);

	if ( !subSession)
		{
		PanicClient (aMessage, EBadSubsessionHandle);
		}
	return subSession;
	}

void CRTSecMgrSession::DispatchMessageL(const RMessage2& aMessage)
	{
	// First check for session-relative requests
	switch (aMessage.Function ())
		{
		case ESetPolicy:
			SetPolicy (aMessage);
			return;
		case EUpdatePolicy:
			UpdatePolicy (aMessage);
			return;
		case EUnsetPolicy:
			UnsetPolicy (aMessage);
			return;
		case ERegisterScript:
			RegisterScript (aMessage);
			return;
		case ERegisterScriptWithHash:
			RegisterScript (aMessage, ETrue);
			return;
		case EUnRegisterScript:
			UnregisterScript (aMessage);
			return;
		case EGetScriptSession:
			GetScriptSessionL (aMessage);
			return;
		case EGetTrustedUnRegScriptSession:
			GetTrustedUnRegScriptSessionL (aMessage);
			return;
		case ESecServCloseSession:
			CloseSession ();
			return;
			}

	// All other function codes must be subsession relative.
	// We need to find the appropriate server side subsession
	// i.e. the CRTSecMgrSubSession object. 
	// The handle value is passed as the 4th aregument.
	CRTSecMgrSubSession* subSession = SubSessionFromHandle (aMessage,
			aMessage.Int3 ());
	switch (aMessage.Function ())
		{
		case ECloseScriptSession:
			DeleteSubSession (aMessage.Int3 ());
			return;
		case EUpdatePermanentGrant:
			subSession->UpdatePermGrantL (aMessage);
			return;
		case EGetScriptFile:
			subSession->GetScriptFile (aMessage);
			return;
		case EUpdatePermanentGrantProvider:
		    subSession->UpdatePermGrantProviderL (aMessage);
		    return;
		default:
			PanicClient (aMessage, EBadRequest);
			return;
		}
	}
TBool CRTSecMgrSession::IsScriptOpenWithPolicy(TPolicyID aPolicyID)
	{
	for (TInt i(0); i!=iSubSessionObjectIndex->Count ();++i)
		{
		CRTSecMgrSubSession
				* subsession = ((CRTSecMgrSubSession*)iSubSessionObjectIndex->operator[](i));
		if ( subsession)
			{
			if ( aPolicyID==subsession->Script().PolicyID ())
				{
				return ETrue;
				}
			}
		else
			{
			continue;
			}

		}
	return EFalse;
	}

TBool CRTSecMgrSession::IsScriptSessionOpen(TExecutableID aScriptID,CRTSecMgrSubSession* aCurrentSession)
	{
	for (TInt i(0); i!=iSubSessionObjectIndex->Count ();++i)
		{
		CRTSecMgrSubSession
				* subsession = ((CRTSecMgrSubSession*)iSubSessionObjectIndex->operator[](i));
		if ( subsession && ( subsession != aCurrentSession))
			{
			if ( aScriptID==subsession->ScriptID ())
				{
				return ETrue;
				}
			}
		else
			{
			continue;
			}

		}
	return EFalse;
	}
