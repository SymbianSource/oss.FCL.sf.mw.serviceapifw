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
* Description:       Defines security manager client side session and sub-session classes
 *
*/







#include <coemain.h>
#include <bautils.h>
#include <s32file.h>
#include <eikenv.h>
#include <rtsecmgrscript.h>
#include "rtsecmgrmsg.h"
#include "rtsecmgrdef.h"
#include "rtsecmgrclient.h"
#include "rtsecmgrtracer.h"

#ifdef _DEBUG
_LIT(KServerStartFailed, "Security manager server starting failed");
#endif

// ---------------------------------------------------------------------------
// Defintiion of default private constructor
// ---------------------------------------------------------------------------
//
RSecMgrSession::RSecMgrSession()
	{	
	}

// ---------------------------------------------------------------------------
// Connects to the runtime security manager server
//
// This function attemtps to kick start security manager server if
// it is not running already. The number of attempts is currently 2.
// The number of message slot is defaulted to 4.
// ---------------------------------------------------------------------------
//
/*TInt RSecMgrSession::Connect()
	{
	RTSecMgrTraceFunction("RSecMgrSession::Connect()") ;
	TInt retry(KSecSrvClientTryCount); // Try this twice
	TInt err(KErrNone);
	while (retry>KErrNone)
		{
		// Try to create a Server session
		err = CreateSession ( KSecServerProcessName, Version (),
				KDefaultMessageSlots);

		if ( err != KErrNotFound && err != KErrServerTerminated)
			{
			// KErrNone or unrecoverable error
			if ( err != KErrNone)
				{
#ifdef _DEBUG
				RDebug::Print(KServerStartFailed);
#endif
				}
			retry = 0;
			}
		else
			{
			// Return code was KErrNotFound or KErrServerTerminated.
			// Try to start a new security manager server instance
			err = StartSecManagerServer ();
			if ( err != KErrNone && err != KErrAlreadyExists)
				{
				// Unrecoverable error
#ifdef _DEBUG
				RDebug::Print(KServerStartFailed);
#endif
				retry = 0;
				}
			}

		retry--;
		}
	return (err);
	}*/

TInt RSecMgrSession::Connect()
    {
    RTSecMgrTraceFunction("RSecMgrSession::Connect()") ;
    TInt err(KErrNone);
        // Try to create a Server session
    err = CreateSession ( KSecServerProcessName, Version (),
            KDefaultMessageSlots);

    if ( err != KErrNotFound && err != KErrServerTerminated)
        {
        // KErrNone or unrecoverable error
        if ( err != KErrNone)
            {
#ifdef _DEBUG
            RDebug::Print(KServerStartFailed);
#endif
            }
        
        }
    else
        {
        // Return code was KErrNotFound or KErrServerTerminated.
        // Try to start a new security manager server instance
        TInt retry(KSecSrvClientTryCount);
        while(retry > 0)
            {
            err = StartSecManagerServer ();
            if ( err != KErrNone && err != KErrAlreadyExists)
                {
                // Unrecoverable error
                #ifdef _DEBUG
                            RDebug::Print(KServerStartFailed);
                #endif
                retry = 0;
                }
            else
                {
                err = CreateSession ( KSecServerProcessName, Version (),
                                                KDefaultMessageSlots);
                if(err != KErrNotFound && err != KErrServerTerminated)
                    {
                    if ( err != KErrNone)
                                {
                    #ifdef _DEBUG
                                RDebug::Print(KServerStartFailed);
                    #endif
                                }
                    retry = 0;
                    }
                }
            retry--;
            }
        }       
    return (err);
    }

// ---------------------------------------------------------------------------
// Starts runtime security manager server
//
// ---------------------------------------------------------------------------
//
TInt RSecMgrSession::StartSecManagerServer() const
	{
	RTSecMgrTraceFunction("RSecMgrSession::StartSecManagerServer()") ;
	RProcess server;
	const TUidType serverUid( KNullUid, KSecMgrServerUid2, KNullUid);
	TInt err = server.Create ( ServerLocation (), 
			KNullDesC, 
			serverUid, 
			EOwnerProcess);

	// Return error code if we the process couldn't be created
	if ( KErrNone == err)
		{
		// Rendezvous is used to detect server start
		TRequestStatus status;
		server.Rendezvous ( status);
		if ( status != KRequestPending)
			{
			// Log Abort Error
#ifdef _DEBUG
			RDebug::Print(KServerStartFailed);
#endif			
			server.Kill ( 0); // Abort startup
			}
		else
			{
			server.Resume (); // Logon OK - start the server
			}
		User::WaitForRequest (status); // Wait for start or death

		if ( server.ExitType ()== EExitPanic)
			{
#ifdef _DEBUG
			RDebug::Print(KServerStartFailed);
#endif
			err = KErrGeneral;
			}
		else
			{
			err = status.Int ();
			}

		// We can close the handle now
		server.Close ();
		}
	return err;
	}

// ---------------------------------------------------------------------------
// Returns runtime security manager server location
//
// ---------------------------------------------------------------------------
//
TFullName RSecMgrSession::ServerLocation() const
	{
	TFullName fullPathAndName;
	fullPathAndName.Append ( KSecMgrServerExeName);
	return fullPathAndName;
	}

// ---------------------------------------------------------------------------
// Returns the earliest version number of the security manager server
//
// ---------------------------------------------------------------------------
//
TVersion RSecMgrSession::Version(void) const
	{
	return (TVersion(KRTSecMgrServMajorVersionNumber,KRTSecMgrServMinorVersionNumber,KRTSecMgrServBuildVersionNumber));
	}

// ---------------------------------------------------------------------------
// A request to close the session.
//
// It makes a call to the server, which deletes the object container and object index
// for this session, before calling Close() on the base class.
// ---------------------------------------------------------------------------
//
void RSecMgrSession::Close()
	{
	if(iHandle)
		{		
		SendReceive (ESecServCloseSession);
		RSessionBase::Close();
		}
	}

//
// Registers the runtime security policy with security manager. This method
// packs the message parameters required for registering the policy. 
// 
// The various parameters required for SetPolicy operation are :
// 	
//  Operation Code  : ESetPolicy
// 	IPC Argument[0] : Policy Identifier (as inOut parameter)
//  IPC Argument[1] : SecurityPolicy FileHandle
//  IPC Argument[2] : SecurityPolicy FileSession object
//  IPC Argument[3] : none
// 
TInt RSecMgrSession::SetPolicy(const RFile& aSecPolicy)
	{
	TPckgBuf<TInt> pckgPId;
	TIpcArgs args(&pckgPId);

	TInt ret = aSecPolicy.TransferToServer (args, EMsgArgOne,
			EMsgArgTwo);

	if ( KErrNone==ret)
		{
		ret = SendReceive (ESetPolicy, args);

		if ( KErrNone==ret)
			return pckgPId (); // Extract the policyID returned from the server. 
		}

	return ret;
	}

TPolicyID RSecMgrSession::SetPolicy(const TDesC8& aPolicyBuffer)
	{	
	TInt ret(ErrInvalidParameters);
	if(0==aPolicyBuffer.CompareC(KNullDesC8))
		{
		return ret;
		}
		
		TFileName tempDirPath;
		TFileName tempPath;
	
		{
		RFs fileSession;
		if ( KErrNone==fileSession.Connect ())
			{
			fileSession.PrivatePath (tempDirPath);
			BaflUtils::EnsurePathExistsL (fileSession, tempDirPath);

			RFile secPolicyFile;
			secPolicyFile.Temp (fileSession, tempDirPath, tempPath, EFileWrite);
			secPolicyFile.Write(aPolicyBuffer);
			secPolicyFile.Close();			
			}		
		fileSession.Close();
		}
		
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if ( KErrNone==fileSession.ShareProtected ())
			{
			RFile secPolicyFile;
					
			if(KErrNone == secPolicyFile.Open(fileSession,tempPath,EFileRead))
				{
				ret = SetPolicy (secPolicyFile);
			
				secPolicyFile.Close();
					
				}
			
			fileSession.Delete (tempPath);
			}
		
		CleanupStack::PopAndDestroy (&fileSession);//fileSession
		}

	fileSession.Close();
	
	return ret;
	}

//
// UnRegisters a registered security policy. Runtimes should call this function
// to de-register the already registered security policy.
// 
// The various parameters required for UnSetPolicy operation are :
// 	
// Operation Code  : EUnsetPolicy
// IPC Argument[0] : Policy Identifier to un-register
// IPC Argument[1] : Successcode (as inOut parameter)
// IPC Argument[2] : none
// IPC Argument[3] : none
// 
TInt RSecMgrSession::UnSetPolicy(TPolicyID aPolicyID)
	{
	if ( aPolicyID<=KErrNone)
		return ErrInvalidPolicyID;

	TPckgBuf<TInt> sucess(KErrNone);
	TIpcArgs args(aPolicyID, &sucess);

	TInt ret = SendReceive (EUnsetPolicy, args);

	if ( KErrNone==ret)
		return sucess (); // Extract the value returned from the server. 

	return ret;
	}

//
// Updates an already registered security policy. Runtimes should call this function
// to update their policy.
// 
// The various parameters required for UpdatePolicy operation are :
// 	
// Operation Code  : EUpdatePolicy
// IPC Argument[0] : Policy Identifier
// IPC Argument[1] : SecurityPolicy FileHandle
// IPC Argument[2] : SecurityPolicy FileSession object
// IPC Argument[3] : none
// 
TPolicyID RSecMgrSession::UpdatePolicy(TPolicyID aPolicyID,
		const RFile& aSecPolicy)
	{
	if ( aPolicyID<=KErrNone)
		{
		return ErrInvalidPolicyID;
		}

	TPckgBuf<TInt> pckgPID(aPolicyID);
	TIpcArgs args(&pckgPID);

	TInt ret = aSecPolicy.TransferToServer (args, EMsgArgOne,
			EMsgArgTwo);

	if ( KErrNone==ret)
		{
		ret = SendReceive (EUpdatePolicy, args);

		if ( KErrNone==ret)
			ret = pckgPID ();
		}

	return ret;
	}

TPolicyID RSecMgrSession::UpdatePolicy(TPolicyID aPolicyID,
		const TDesC8& aPolicyBuffer)
	{
	TInt ret(ErrInvalidParameters);
	if(0==aPolicyBuffer.CompareC(KNullDesC8))
		{
		return ret;
		}
		
		TFileName tempDirPath;
		TFileName tempPath;
	
		{
		RFs fileSession;
		if ( KErrNone==fileSession.Connect ())
			{
			fileSession.PrivatePath (tempDirPath);
			BaflUtils::EnsurePathExistsL (fileSession, tempDirPath);

			RFile secPolicyFile;
			secPolicyFile.Temp (fileSession, tempDirPath, tempPath, EFileWrite);
			secPolicyFile.Write(aPolicyBuffer);
			secPolicyFile.Close();			
			}		
		fileSession.Close();
		}
		
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if ( KErrNone==fileSession.ShareProtected ())
			{
			RFile secPolicyFile;
					
			if(KErrNone == secPolicyFile.Open(fileSession,tempPath,EFileRead))
				{
				
				ret = UpdatePolicy (aPolicyID, secPolicyFile);
			
				secPolicyFile.Close();
			
				}

			fileSession.Delete (tempPath);
			}
		
		CleanupStack::PopAndDestroy (&fileSession);//fileSession
		}

	fileSession.Close();
	
	return ret;
	}

//
// Registers a script/executable. Runtimes should specify the trust information
// of the script to be registered. 
// 
// Operation Code  : ERegisterScript
// IPC Argument[0] : Policy Identifier
// IPC Argument[1] : Script Identifier (as inOut Parameter)
// IPC Argument[2] : none
// IPC Argument[3] : none 
// 
TExecutableID RSecMgrSession::RegisterScript(TPolicyID aPolicyID, const CTrustInfo& /*aTrustInfo*/)
	{
	if ( aPolicyID<KErrNone)
		return ErrInvalidPolicyID;

	TPckgBuf<TInt> scriptID(KAnonymousScript);
	TIpcArgs args(aPolicyID, &scriptID);

	TInt result = SendReceive (ERegisterScript, args);

	if ( KErrNone==result)
		result=scriptID ();

	return result;
	}

//
// Registers a script/executable. Runtimes should specify the trust information
// of the script to be registered. 
// 
// Operation Code  : ERegisterScript
// IPC Argument[0] : Policy Identifier
// IPC Argument[1] : Script Identifier (as inOut Parameter)
// IPC Argument[2] : Hash value of script
// IPC Argument[3] : none
// 
TExecutableID RSecMgrSession::RegisterScript(TPolicyID aPolicyID,
		const TDesC& aHashMarker, const CTrustInfo& /*aTrustInfo*/)
	{
	__UHEAP_MARK;

	if(!(aHashMarker.Compare(KNullDesC)))
		return ErrInvalidParameters;
	CRTSecMgrRegisterScriptMsg* scriptMsg = CRTSecMgrRegisterScriptMsg::NewL (
			aPolicyID, aHashMarker);
 
	HBufC8* dataDes(NULL);
	TRAPD(ret, dataDes = scriptMsg->PackMsgL());
	if ( dataDes)
		{
		TExecutableID scriptID(KAnonymousScript);
		TPckgBuf<TInt> scriptIDBuf(scriptID);
		TIpcArgs args(dataDes, &scriptIDBuf);
		
		ret = SendReceive (ERegisterScriptWithHash, args);
		delete dataDes;
		
		if(KErrNone==ret)
			ret = scriptIDBuf();
		}

	delete scriptMsg;

	__UHEAP_MARKEND;

	return ret;
	}

//
// De-Registers a script/executable. Runtimes should pass the previously registered
// script identifier corresponding to the script to be de-registered.
// 
// Operation Code  : EUnRegisterScript
// IPC Argument[0] : Script Identifier
// IPC Argument[1] : Policy Identifier
// IPC Argument[2] : Success code (as inOut parameter)
// IPC Argument[3] : none 
//  
TInt RSecMgrSession::UnRegisterScript(TExecutableID aExeID, TPolicyID aPolicyID)
	{
	if (aExeID<=KErrNone) 
		return ErrInvalidScriptID;
	if (aPolicyID<=KErrNone)
		return ErrInvalidPolicyID;

	TPckgBuf<TInt> errCode(KErrNone);
	TIpcArgs args(aExeID, aPolicyID, &errCode);

	TInt result = SendReceive (EUnRegisterScript, args);

	if ( KErrNone==result)
		return errCode ();

	return result;
	}

RSecMgrSubSession::RSecMgrSubSession()
	{	
	
	}
//
// Opens client-side sub-session for a registered script. The script session is modelled as a
// client side sub-session with a peer server side sub-session.
// 
TInt RSecMgrSubSession::Open(const RSessionBase& aSession,
		CScript& aScriptInfo, TPolicyID aPolicyID, const TDesC& aHashValue)
	{
	TIpcArgs args(aScriptInfo.ScriptID (), aPolicyID);

	TInt errCode(KErrNone);
	errCode = iFs.Connect();
	if(errCode == KErrNone)
    	{
    	if ( KAnonymousScript==aScriptInfo.ScriptID ())
    		errCode = CreateSubSession (aSession, EGetTrustedUnRegScriptSession,
    				args);
    	else
    		errCode = CreateSubSession (aSession, EGetScriptSession, args);
    
    	if ( errCode==KErrNone)
    		{
    		// Retrieve the RFs and RFile handles from the server
    		TPckgBuf<TInt> fh; // sub-session (RFile) handle
    		TIpcArgs args(&fh);
    
    		RFile file;
    		CleanupClosePushL(file);
    
    		if ( KErrNone==errCode)
    			{
    			iFs.ShareProtected ();
    
    			TFileName tempDirPath;
    			TFileName tempPath;
    
    			iFs.PrivatePath (tempDirPath);
    			BaflUtils::EnsurePathExistsL (iFs, tempDirPath);
    
    			errCode = file.Temp (iFs, tempDirPath, tempPath, EFileWrite);
    
    			if ( KErrNone==errCode)
    				{
    				file.TransferToServer (args, EMsgArgOne, EMsgArgTwo);
    				errCode = SendReceive (EGetScriptFile, args);
    
    				if ( KErrNone==errCode)
    					{
    					RFileReadStream rfs(file);
    					CleanupClosePushL(rfs);
    					aScriptInfo.InternalizeL (rfs);
    					TBufC<KMaxPath> hashValue(aScriptInfo.Hash());
    					if(0 != hashValue.Compare(KNullDesC))
    						{
    						if(!aScriptInfo.HashMatch(aHashValue))
    							{
    							//hash check failed
    							errCode = KErrNotFound;
    							}						
    						}
    					
    					CleanupStack::PopAndDestroy(&rfs);
    					}
    				}
    			iFs.Delete (tempPath);
    			}
    
    		CleanupStack::PopAndDestroy(&file);
    		}
    	}
	return errCode;
	}

//
// Opens client-side sub-session for an un-registered trusted script. The script session is modelled as a
// client side sub-session with a peer server side sub-session.
// 
TInt RSecMgrSubSession::Open(const RSessionBase& aSession,
		CScript& aScriptInfo, TPolicyID aPolicyID, const CTrustInfo& /*aTrustInfo*/)
	{	
	return Open (aSession, aScriptInfo, aPolicyID);
	}

//
// Updates the blanket permission data of the script
// 
TInt RSecMgrSubSession::UpdatePermGrant(TExecutableID aScriptID,
		TPermGrant aPermGrant, TPermGrant aPermDenied) const
	{
	TIpcArgs args(aScriptID, (TInt)aPermGrant, (TInt)aPermDenied);
	return SendReceive (EUpdatePermanentGrant, args);
	}

//
// Close the subsession.
//
void RSecMgrSubSession::Close()
	{
	iFs.Close();
	RSubSessionBase::CloseSubSession (ECloseScriptSession);
	}

