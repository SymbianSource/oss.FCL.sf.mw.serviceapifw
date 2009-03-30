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
* Description:       Implementation file for security manager server, session and 
 * 				  sub-session classes 
 *
*/






#include <f32file.h>
#include <e32debug.h>
#include <rtsecmgrcommondef.h>
#include "rtsecmgrserver.h"
#include "rtsecmgrsession.h"
#include "rtsecmgrsubsession.h"
#include "rtsecmgrpolicyparser.h"
#include "rtsecmgrstore.h"
#include "rtsecmgrprotectiondomain.h"
#include "rtsecmgrpolicymanager.h"
#include "rtsecmgrscriptmanager.h"
#include "rtsecmgrmsg.h"

_LIT(KUntrusted, "UnTrusted");

TInt CRTSecMgrServer::GetCapabilityInfo(TPolicyID aPolicyID,
		TExecutableID aExecID, CScript& aScript)
	{
	if ( KAnonymousScript==aExecID)
		{
		CPolicy* policy = iPolicyMgr->Policy (aPolicyID);
		if ( policy)
			{
			CProtectionDomain* domain = policy->ProtectionDomain (KUntrusted);

			if ( domain)
				{
				const CPermissionSet& permissionSet = domain->PermSet ();
				aScript.SetPermissionSet (permissionSet);
				return KErrNone;
				}
			}
		}

	return KErrNotFound;
	}

void CRTSecMgrSession::SetPolicy(const RMessage2& aMessage, TBool aIsUpdate)
	{
	if(!aMessage.HasCapability(ECapabilityWriteDeviceData , ""))
	    {
	    TPckgBuf<TInt> pkg(ErrAccessDenied);
	    aMessage.Write(EMsgArgZero , pkg);
	    return;
	    }
	RFile secPolicyFile;
	TPolicyID pID(ErrInvalidPolicyID);

	if ( KErrNone==secPolicyFile.AdoptFromClient (aMessage, EMsgArgOne,
			EMsgArgTwo))
		{
		CPolicyParser* policyParser = CPolicyParser::NewLC ();
		TInt ret(KErrNone);

		RProtectionDomains policyInfo;
		RAliasGroup aliasInfo;
		ret=policyParser->GetPolicyInfo (secPolicyFile, policyInfo, aliasInfo);

		if (KErrNone==ret)
			{
			if(aIsUpdate)
				{
				TPckgBuf<TInt> pIDPckg;
				aMessage.Read(0, pIDPckg);
				pID = pIDPckg();
				}
			else
				{
				if ( KErrNone==iSecMgrServer->ReadCounter (pID))
					{
					while (iSecMgrServer->IsValidPolicy(pID))
						{
						--pID ;
						}
					}
				else
					{
					pID = ErrServerReadConfig;
					}
				}
			TRAPD (err, iSecMgrServer->AddPolicyL (pID, policyInfo, aliasInfo));
			
			if ( KErrNone!=err)
				{
				pID = ErrSetPolicyFailed;
				}						
				
			}
		else
			{
			pID = ErrInvalidPolicyFormat;
			policyInfo.ResetAndDestroy ();
			}
		CleanupStack::PopAndDestroy (policyParser);
		}
	else
		{
		pID = ErrFileSessionNotShared;
		}

	secPolicyFile.Close();
	
	TPckgBuf<TInt> pkg(pID);
	aMessage.Write (EMsgArgZero, pkg);
	}

void CRTSecMgrSession::UpdatePolicy(const RMessage2& aMessage)
	{
	if(!aMessage.HasCapability(ECapabilityWriteDeviceData , ""))
        {
        TPckgBuf<TInt> pkg(ErrAccessDenied);
        aMessage.Write(EMsgArgZero , pkg);
        return;
        }
	TPckgBuf<TInt> pIDPckg;
	aMessage.Read (0, pIDPckg);

	TPolicyID pID(pIDPckg ());

		//Check if this ID already exists
	if ( !iSecMgrServer->IsValidPolicy(pID))
		{
		TPckgBuf<TInt> pkg(ErrUpdatePolicyFailed);
		aMessage.Write (EMsgArgZero, pkg);
		RFile secPolicyFile;
		//just to close the secPolicyFile. Else the temmporary file cannot be deleted
		secPolicyFile.AdoptFromClient (aMessage, EMsgArgOne,EMsgArgTwo);
		secPolicyFile.Close();
		return;
		}
	else
		{
		if ( IsScriptOpenWithPolicy (pID))
			{
			TPckgBuf<TInt> pkg(ErrUpdatePolicyFailed);
			aMessage.Write (EMsgArgZero, pkg);
			return;
			}
		}

	//back up the file before update (file with this policy Id)
	TInt backupResult = iSecMgrServer->BackupFile(pID);
	
	if(KErrNone == backupResult)
		{
			SetPolicy (aMessage, ETrue);
			
			aMessage.Read(0, pIDPckg);
			TInt resultSetPolicy = pIDPckg();		
			
			if(resultSetPolicy < KErrNone)
				{
					//means that the policy updation is NOT successful due to invalid policy file
					//Hence retain the previous file by restoring the temp file
					TInt restoreResult = iSecMgrServer->RestoreTempPolicy(pID);
					
					if(KErrNone != restoreResult)
					{
						// file backup not created due to errors
						TPckgBuf<TInt> pkg(ErrRestoreTempFailed);
						aMessage.Write (EMsgArgZero, pkg);
						return;
					}
				}

			//Backup file is no longer useful. 
			//Hence removing this temporary file using the method below
			TInt rmTempResult = iSecMgrServer->RemoveTempPolicy(pID);	
			if(KErrNone != rmTempResult)
			{
				//temporary file not removed
				
			}	
	}
	else
	{
		// file backup not created due to errors
		TPckgBuf<TInt> pkg(ErrBackupNotCreated);
		aMessage.Write (EMsgArgZero, pkg);
		return;
		
	}	
	
	}

void CRTSecMgrSession::UnsetPolicy(const RMessage2& aMessage)
	{
	if(!aMessage.HasCapability(ECapabilityWriteDeviceData , ""))
        {
        TPckgBuf<TInt> pkg(ErrAccessDenied);
        aMessage.Write(EMsgArgZero , pkg);
        return;
        }
	TPolicyID pID = aMessage.Int0 ();

	TInt result = ErrUnSetPolicyFailed;

	if ( !IsScriptOpenWithPolicy(pID))
		{
		result = iSecMgrServer->RemovePolicy (pID);
		}
	if( KErrNone>result )
		result = ErrUnSetPolicyFailed;
	
	TPckgBuf<TInt> retVal(result);
	aMessage.Write (EMsgArgOne, retVal);
	}

void CRTSecMgrSession::RegisterScript(const RMessage2& aMessage, TBool aIsHashed)
	{
	if( !aMessage.HasCapability(ECapabilityWriteDeviceData , ""))
        {
        TPckgBuf<TInt> pkg(ErrAccessDenied);
        aMessage.Write(EMsgArgZero , pkg);
        return;
        }
	if ( aIsHashed)
		{
		HBufC8* desData = HBufC8::NewLC (KMaxMsgLength);
		TPtr8 readPtr(desData->Des ());
		aMessage.ReadL (0, readPtr);
		CRTSecMgrRegisterScriptMsg
				* scriptMsg = CRTSecMgrRegisterScriptMsg::NewLC (*desData);

		TExecutableID scriptID = iSecMgrServer->RegisterScript (scriptMsg->PolicyID(),scriptMsg->HashValue());
		
		TPckgBuf<TInt> exeIDPkg(scriptID);
		aMessage.Write (EMsgArgOne, exeIDPkg);

		CleanupStack::PopAndDestroy (scriptMsg);
		CleanupStack::PopAndDestroy (desData);		
		}
	else
		{
		TPolicyID policyID = aMessage.Int0 ();

		TExecutableID scriptID = iSecMgrServer->RegisterScript (policyID);

		if ( KErrNone>scriptID)
			scriptID = ErrRegisterScriptFailed;

		TPckgBuf<TInt> exeIDPkg(scriptID);
		aMessage.Write (EMsgArgOne, exeIDPkg);
		}

	}

void CRTSecMgrSession::UnregisterScript(const RMessage2& aMessage)
	{
	if(!aMessage.HasCapability(ECapabilityWriteDeviceData , ""))
        {
        TPckgBuf<TInt> pkg(ErrAccessDenied);
        aMessage.Write(EMsgArgZero , pkg);
        return;
        }
	TExecutableID scriptID(aMessage.Int0 ());
	TPolicyID policyID(aMessage.Int1 ());

	TInt result = KErrNone;

	if ( !IsScriptSessionOpen(scriptID))
		{
		TRAP (result, iSecMgrServer->UnRegisterScriptL (scriptID, policyID));
		if(KErrNone>result)
				result = ErrUnRegisterScriptFailed;	
		}
	else
		{
		result = ErrUnRegisterScriptFailed;
		}

	TPckgBuf<TInt> errCode(result);
	aMessage.Write (EMsgArgTwo, errCode);
	}

void CRTSecMgrSession::GetScriptSessionL(const RMessage2& aMessage)
	{
	if(!aMessage.HasCapability(ECapabilityWriteDeviceData , ""))
        {
        User::Leave(ErrAccessDenied);
        }
	TExecutableID scriptID = (TExecutableID)aMessage.Int0();
	TPolicyID policyID = (TPolicyID)aMessage.Int1();

	CScript* script = CScript::NewLC (policyID, scriptID);

	User::LeaveIfError (iSecMgrServer->GetCapabilityInfo (*script));

	if(script->PolicyID() != policyID || !iSecMgrServer->IsValidPolicy(policyID))
	{
		User::Leave(ErrInvalidPolicyID);
	}
	
	CleanupStack::Pop (script);

	CRTSecMgrSubSession* counter = CRTSecMgrSubSession::NewL (this, script, iSecMgrServer);
	CleanupStack::PushL (counter);

	// add the CCountSubSession object to 
	// this subsession's object container
	// to gererate a unique id
	iContainer->AddL (counter);

	// Add the object to object index; this returns
	// a unique handle so that we can find the object
	// again laterit later.
	TInt handle=iSubSessionObjectIndex->AddL (counter);

	// Write the handle value back to the client.
	// NB It's not obvious but the handle value must be passed
	// back as the 4th parameter (i.e. parameter number 3 on
	// a scale of 0 to 3). 
	// The arguments that are passed across are actually
	// set up by RSubSessionBase::DoCreateSubSession().
	// If you pass your own arguments into a call
	// to RSubSessionBase::CreateSubSession(), which calls DoCreateSubSession, 
	// then only the first three are picked up - the 4th is reserved for the
	// the subsession handle.
	TPckgBuf<TInt> handlePckg(handle);
	aMessage.Write (EMsgArgThree, handlePckg);

	CleanupStack::Pop (counter);
	}

void CRTSecMgrSession::GetTrustedUnRegScriptSessionL(const RMessage2& aMessage)
	{
	if(!aMessage.HasCapability(ECapabilityWriteDeviceData , ""))
        {
        User::Leave(ErrAccessDenied);
        }
	//0th parameter - ScriptID
	//1st parameter - PolicyID	
	TExecutableID scriptID = (TExecutableID)aMessage.Int0(); //typically this is KAnonymousScript
	TPolicyID policyID = (TExecutableID)aMessage.Int1();

	CScript* script = CScript::NewLC (policyID, scriptID);

	User::LeaveIfError (iSecMgrServer->GetCapabilityInfo(policyID, scriptID, *script));

	CleanupStack::Pop (script);

	CRTSecMgrSubSession* counter = CRTSecMgrSubSession::NewL (this, script, iSecMgrServer);
	CleanupStack::PushL (counter);

	iContainer->AddL (counter);
	TInt handle=iSubSessionObjectIndex->AddL (counter);
	TPckgBuf<TInt> handlePckg(handle);
	aMessage.Write (EMsgArgThree, handlePckg);

	CleanupStack::Pop (counter);
	}

void CRTSecMgrSubSession::GetScriptFile(const RMessage2& aMessage)
	{
	RFile scriptFile;

	if ( KErrNone==scriptFile.AdoptFromClient (aMessage, EMsgArgOne,
			EMsgArgTwo))
		{
		RFileWriteStream rfws(scriptFile);
		iScript->ExternalizeL (rfws);
		rfws.Close ();
		scriptFile.Close ();
		}
	}

void CRTSecMgrSubSession::UpdatePermGrantL(const RMessage2& aMessage)
	{
	if ( !iSession->IsScriptSessionOpen(aMessage.Int0(),this))
		{
		iSecMgrServer->UpdatePermGrantL (aMessage.Int0 (), aMessage.Int1 (),
				aMessage.Int2 ());
		}
	else
		{
		TPckgBuf<TInt> pkg((TInt)ErrUpdatePermGrantFailed);
		aMessage.Write (EMsgArgZero, pkg);
		}
	}
