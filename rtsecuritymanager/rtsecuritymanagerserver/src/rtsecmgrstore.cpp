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
* Description:       Definition of security manager store class
 *
*/






#include <s32file.h>
#include <bautils.h>
#include <eikenv.h>
#include <f32file.h>
#include <pathinfo.h>
#include "rtsecmgrstore.h"
#include "rtsecmgrdata.h"

const TReal DEFAULT_VERSION(1.0);
CSecMgrStore* CSecMgrStore::NewL()
	{
	CSecMgrStore* self = CSecMgrStore::NewLC ();
	CleanupStack::Pop (self);
	return self;
	}

CSecMgrStore* CSecMgrStore::NewLC()
	{
	CSecMgrStore* self = new (ELeave)CSecMgrStore();
	CleanupStack::PushL (self);
	self->ConstructL ();
	return self;
	}

void CSecMgrStore::StorePolicyL(const CPolicy& aPolicy)
	{
	__UHEAP_MARK;

	HBufC *policyFile = HBufC::NewLC(KMaxName);
	TPtr ptr(policyFile->Des());
	GetPolicyFile (ptr, aPolicy.PolicyID ());

	CFileStore* store = CPermanentFileStore::ReplaceLC (iFsSession, ptr,
			EFileWrite);
	store->SetTypeL (KPermanentFileStoreLayoutUid);

	// Construct the output stream.
	RStoreWriteStream outstream;
	TStreamId id = outstream.CreateLC (*store);
	//Write version of the policy
	outstream.WriteReal32L (TReal(DEFAULT_VERSION));

	TInt aliasCnt(aPolicy.AliasGroup().Count());
	outstream.WriteInt32L (aliasCnt);
	RAliasGroup aliasGroups = aPolicy.AliasGroup();
	for (TInt i(0); i!=aliasCnt;++i)
		{
		CPermission* alias = aliasGroups[i];
		alias->ExternalizeL (outstream);
		}
	
	TInt domainCnt(aPolicy.ProtectionDomain().Count ());
	outstream.WriteInt32L (domainCnt);
	RProtectionDomains domains = aPolicy.ProtectionDomain ();
	for (TInt i(0); i!=domainCnt;++i)
		{
		CProtectionDomain* domain = domains[i];
		domain->ExternalizeL (outstream);
		}

	// Commit changes to the stream
	outstream.CommitL ();

	CleanupStack::PopAndDestroy (&outstream);

	// Set this stream id as the root
	store->SetRootL (id);

	// Commit changes to the store
	store->CommitL ();

	CleanupStack::PopAndDestroy (store);
	CleanupStack::PopAndDestroy (policyFile);
	__UHEAP_MARKEND;
	}


TInt CSecMgrStore::RemovePolicy(TPolicyID aPolicyID)
	{
	HBufC *policyFile = HBufC::NewLC(KMaxName);
	TPtr ptr(policyFile->Des());
	TInt ret(KErrNone);
	
	ret = GetPolicyFile(ptr, aPolicyID);
	
	if(KErrNone==ret)
		ret  = iFsSession.Delete (ptr);
	CleanupStack::PopAndDestroy(policyFile);
	return ret;
	}

void CSecMgrStore::RestorePoliciesL(RPolicies& aPolicy)
	{
	//Fetch all the individual policy files from "epoc32\winscw\c\private\<SECMGR_UID>\policy" directory
	//Iteratively internalise the policy files, with the policyID (PolicyID is = to filename)
	HBufC *privateDir = HBufC::NewLC(KMaxName);
	TPtr privateDirptr(privateDir->Des());
	GetPolicyPath (privateDirptr);

	// create search string
	HBufC* searchBuf = privateDirptr.AllocLC ();
	TPtr searchPtr(searchBuf->Des());

	CDir* dir=  NULL;
	iFsSession.GetDir (searchPtr, KEntryAttNormal,
			ESortByName | EAscending, dir);

	if ( dir)
		{
		CleanupStack::PushL (dir);
		for (TInt i=0; i!=dir->Count ();++i)
			{
			TEntry entry = dir->operator[] (i);
			HBufC *fileName = HBufC::NewLC(KMaxName);
			TPtr ptr(fileName->Des());
			ptr = entry.iName;

			HBufC *policyFile = HBufC::NewLC(KMaxName);
			TPtr policyFileptr(policyFile->Des());
			policyFileptr.Copy(privateDirptr) ;
			policyFileptr.Append (ptr);

			CFileStore* store = CPermanentFileStore::OpenLC (iFsSession,
					policyFileptr, EFileShareAny);

			RStoreReadStream instream;
			instream.OpenLC (*store, store->Root ());

			TLex16 lexer(ptr);
			TPolicyID pID;
			lexer.Val (pID);

			CPolicy* policy = CPolicy::NewL (pID, instream);
			aPolicy.Append (policy);

			CleanupStack::PopAndDestroy (&instream); //instream
			CleanupStack::PopAndDestroy (store); //store
			CleanupStack::PopAndDestroy (policyFile);
			CleanupStack::PopAndDestroy (fileName);
			}

		CleanupStack::PopAndDestroy (dir);
		}

	CleanupStack::PopAndDestroy (searchBuf);
	CleanupStack::PopAndDestroy (privateDir);
	}

void CSecMgrStore::RegisterScriptL(TExecutableID aExecID, const CScript& aScript)
	{
	HBufC *scriptFile = HBufC::NewLC(KMaxName);
	TPtr ptr(scriptFile->Des());
	User::LeaveIfError(GetScriptFile(ptr,aExecID));

	CFileStore* store = CPermanentFileStore::ReplaceLC (iFsSession, ptr,
			EFileWrite);

	// Must say what kind of file store.
	store->SetTypeL (KPermanentFileStoreLayoutUid);

	// Construct the output stream.
	RStoreWriteStream outstream;
	TStreamId id = outstream.CreateLC (*store);

	aScript.ExternalizeL (outstream);

	// Commit changes to the stream
	outstream.CommitL ();

	// Cleanup the stream object
	CleanupStack::PopAndDestroy (&outstream);

	// Set this stream id as the root
	store->SetRootL (id);

	// Commit changes to the store
	store->CommitL ();

	CleanupStack::PopAndDestroy (store);
	CleanupStack::PopAndDestroy (scriptFile);

	}

void CSecMgrStore::UnRegisterScriptL(TExecutableID aExecID)
	{
	HBufC *scriptFile = HBufC::NewLC(KMaxName);
	TPtr ptr(scriptFile->Des());
	User::LeaveIfError (GetScriptFile (ptr,aExecID));

	if ( BaflUtils::FileExists (iFsSession, ptr))
		iFsSession.Delete (ptr);
	else
		{
		User::Leave(ErrInvalidScriptID);
		}
	CleanupStack::PopAndDestroy (scriptFile);
	}

TInt CSecMgrStore::ReadScriptInfo(TExecutableID aExecID, CScript& aScriptInfo)
	{
	__UHEAP_MARK;

	HBufC *scriptFile = HBufC::NewLC(KMaxName);
	TPtr ptr(scriptFile->Des());
	TInt ret(GetScriptFile (ptr, aExecID));

	if ( KErrNone==ret)
		{
		if ( !BaflUtils::FileExists(iFsSession, ptr))
			{
			return ErrInvalidScriptID;
			}

		CFileStore* store=  NULL;
		store=CPermanentFileStore::OpenLC (iFsSession, ptr,
				EFileShareAny);

		if ( KErrNone==ret)
			{
			RStoreReadStream instream;
			instream.OpenLC (*store, store->Root ());

			aScriptInfo.InternalizeL (instream);

			CleanupStack::PopAndDestroy (&instream);
			CleanupStack::PopAndDestroy (store);
			}
		return ret;
		}
	CleanupStack::PopAndDestroy (scriptFile);
	__UHEAP_MARKEND;

	return ret;
	}

TInt CSecMgrStore::GetScriptFile(TExecutableID aExecID, TDes& aScriptFile)
	{
	TInt ret(GetScriptFile (aScriptFile,aExecID));

	if ( KErrNone==ret)
		{
		if ( !BaflUtils::FileExists(iFsSession, aScriptFile))
			{
			return ErrInvalidScriptID;
			}
		}

	return ret;
	}

void CSecMgrStore::WriteScriptInfoL(TExecutableID aExecID, CScript& aScriptInfo)
	{
	__UHEAP_MARK;

	HBufC *scriptFile = HBufC::NewLC(KMaxName);
	TPtr ptr(scriptFile->Des());
	User::LeaveIfError (GetScriptFile (ptr,aExecID));

	CFileStore* store = CPermanentFileStore::ReplaceLC (iFsSession, ptr,
			EFileWrite);

	// Must say what kind of file store.
	store->SetTypeL (KPermanentFileStoreLayoutUid);

	// Construct the output stream.
	RStoreWriteStream outstream;
	TStreamId id = outstream.CreateLC (*store);

	aScriptInfo.ExternalizeL (outstream);

	// Commit changes to the stream
	outstream.CommitL ();

	// Cleanup the stream object
	CleanupStack::PopAndDestroy (&outstream);

	// Set this stream id as the root
	store->SetRootL (id);

	// Commit changes to the store
	store->CommitL ();

	CleanupStack::PopAndDestroy (store);
	CleanupStack::PopAndDestroy (scriptFile);
	__UHEAP_MARKEND;
	}

TInt CSecMgrStore::ReadCounter(TExecutableID& aExecID)
	{
	HBufC *configFile = HBufC::NewLC(KMaxName);
	TPtr ptr(configFile->Des());
	TInt ret(GetConfigFile (ptr));

	if ( KErrNone==ret)
		{
		if ( !BaflUtils::FileExists(iFsSession,*configFile))
			{
			aExecID = (TExecutableID)KCRUIdSecMgr.iUid;
			BaflUtils::EnsurePathExistsL (iFsSession, *configFile);
			CleanupStack::PopAndDestroy (configFile);
			return WriteCounter (aExecID);
			}

		CFileStore* store = CPermanentFileStore::OpenLC (iFsSession,
				*configFile, EFileRead);

		RStoreReadStream instream;
		instream.OpenLC (*store, store->Root ());

		aExecID = instream.ReadInt32L ();

		CleanupStack::PopAndDestroy (&instream);
		CleanupStack::PopAndDestroy (store);
		}
	CleanupStack::PopAndDestroy (configFile);
	return ret;
	}

TInt CSecMgrStore::WriteCounter(TExecutableID aExecID)
	{
	HBufC *configFile = HBufC::NewLC(KMaxName);
	TPtr ptr(configFile->Des());
	TInt ret(GetConfigFile (ptr));

	if ( KErrNone==ret)
		{
		CFileStore* store = CPermanentFileStore::ReplaceLC (iFsSession,
				*configFile, EFileWrite);

		// Must say what kind of file store.
		store->SetTypeL (KPermanentFileStoreLayoutUid);

		// Construct the output stream.
		RStoreWriteStream outstream;
		TStreamId id = outstream.CreateLC (*store);

		outstream.WriteInt32L (aExecID);

		// Commit changes to the stream
		outstream.CommitL ();

		// Cleanup the stream object
		CleanupStack::PopAndDestroy (&outstream);

		// Set this stream id as the root
		store->SetRootL (id);

		// Commit changes to the store
		store->CommitL ();

		CleanupStack::PopAndDestroy (store);
		}
	CleanupStack::PopAndDestroy (configFile);
	return ret;
	}

TInt CSecMgrStore::GetPrivatePath(TDes& aPrivatePath)
	{
	TInt ret(KErrNone);
	aPrivatePath.Append (KCDrive);

	HBufC *privateDir = HBufC::NewLC(KMaxName);
	TPtr ptr(privateDir->Des());
	ret = iFsSession.PrivatePath (ptr);

	if ( KErrNone==ret)
		{
		aPrivatePath.Append (ptr);
		}
	CleanupStack::PopAndDestroy (privateDir);
	return ret;
	}

TInt CSecMgrStore::GetConfigFile(TDes& aConfigFile)
	{
	TInt ret(KErrNone);

	aConfigFile.Append (KCDrive);

	HBufC *privateDir = HBufC::NewLC(KMaxName);
	TPtr ptr(privateDir->Des());
	ret = iFsSession.PrivatePath (ptr);

	if ( KErrNone==ret)
		{
		aConfigFile.Append (ptr);

		ret = iFsSession.MkDirAll (ptr);

		if ( KErrAlreadyExists==ret)
			ret=KErrNone;

		aConfigFile.Append (KConfigFile);
		}
	CleanupStack::PopAndDestroy (privateDir);
	return ret;
	}

TInt CSecMgrStore::GetPolicyPath(TDes& aFile)
	{
	TInt ret(GetPrivatePath (aFile));

	if ( KErrNone==ret)
		{
		aFile.Append (KPolicyDir);
		aFile.Append (KDirSeparator);

		TRAP (ret, BaflUtils::EnsurePathExistsL (iFsSession, aFile));
		}

	return ret;
	}

TInt CSecMgrStore::GetScriptPath(TDes& aFile)
	{
	TInt ret(GetPrivatePath (aFile));

	if ( KErrNone==ret)
		{
		aFile.Append (KScriptDir);
		aFile.Append (KDirSeparator);

		TRAP (ret, BaflUtils::EnsurePathExistsL (iFsSession, aFile));

		if ( KErrAlreadyExists==ret)
			ret = KErrNone;
		}

	return ret;
	}

TInt CSecMgrStore::GetScriptFile(TDes& aFile, TExecutableID aExecID)
	{
	TInt ret(GetScriptPath (aFile));

	if ( (KErrNone==ret) || (KErrAlreadyExists==ret))
		{
		if ( KAnonymousScript!=aExecID)
			{
			HBufC *fSuffix = HBufC::NewLC(KMaxName);
			TPtr fSuffixPtr(fSuffix->Des());
			fSuffixPtr.Num (aExecID);

			aFile.Append (fSuffixPtr);
			aFile.Append (KDatExtn);
			CleanupStack::PopAndDestroy (fSuffix);
			}
		}

	if ( KErrAlreadyExists==ret)
		ret=KErrNone;

	return ret;
	}

TInt CSecMgrStore::GetPolicyFile(TDes& aFile, TPolicyID aPolicyID)
	{
	TInt ret(GetPolicyPath (aFile));

	if ( (KErrNone==ret) || (KErrAlreadyExists==ret))
		{
		HBufC *fPolicyName = HBufC::NewLC(KMaxName);
		TPtr policyNamePtr(fPolicyName->Des());
		policyNamePtr.Num (aPolicyID);

		aFile.Append (policyNamePtr);
		aFile.Append (KDatExtn);
		CleanupStack::PopAndDestroy (fPolicyName);
		}

	return ret;
	}


TInt CSecMgrStore::BackupFile(TPolicyID aPolicyID)
{
	HBufC *policyFile = HBufC::NewLC(KMaxName);
	TPtr policyFilePtr(policyFile->Des());
	
	HBufC *tempPolicyFile = HBufC::NewLC(KMaxName);
	TPtr tempPolicyFilePtr(tempPolicyFile->Des());
	
	TInt ret(GetPolicyFile(policyFilePtr, aPolicyID));

	ret= GetPolicyPath (tempPolicyFilePtr);

	if ( (KErrNone==ret) || (KErrAlreadyExists==ret))
	{
		HBufC *fPolicyName = HBufC::NewLC(KMaxName);
		TPtr policyNamePtr(fPolicyName->Des());
		policyNamePtr.Num (aPolicyID);

		tempPolicyFilePtr.Append (policyNamePtr);
		tempPolicyFilePtr.Append (_L("_temp"));
		tempPolicyFilePtr.Append (KDatExtn);
		
		CleanupStack::PopAndDestroy(fPolicyName);
	}
	
	CFileMan *fm = CFileMan::NewL(iFsSession);
	TInt backupResult = fm->Copy(policyFilePtr, tempPolicyFilePtr, CFileMan::EOverWrite);
	delete fm;
	
	CleanupStack::PopAndDestroy(tempPolicyFile);
	CleanupStack::PopAndDestroy(policyFile);
	
	return backupResult;	
}

TInt CSecMgrStore::RemoveTempPolicy(TPolicyID aPolicyID)
{
	HBufC *tempPolicyFile = HBufC::NewLC(KMaxName);
	TPtr tempPolicyFilePtr(tempPolicyFile->Des());
	
	TInt ret= GetPolicyPath (tempPolicyFilePtr);

	if ( (KErrNone==ret) || (KErrAlreadyExists==ret))
	{
		HBufC *fPolicyName = HBufC::NewLC(KMaxName);
		TPtr policyNamePtr(fPolicyName->Des());
		policyNamePtr.Num (aPolicyID);

		tempPolicyFilePtr.Append (policyNamePtr);
		tempPolicyFilePtr.Append (_L("_temp"));
		tempPolicyFilePtr.Append (KDatExtn);
		
		CleanupStack::PopAndDestroy(fPolicyName);
	}
	
	CFileMan *fm = CFileMan::NewL(iFsSession);
	TInt rmTempResult = fm->Delete(tempPolicyFilePtr);
	delete fm;
	
	CleanupStack::PopAndDestroy(tempPolicyFile);
		
	return rmTempResult;	
}

TInt CSecMgrStore::RestoreTempPolicy(TPolicyID aPolicyID)
{
	HBufC *policyFile = HBufC::NewLC(KMaxName);
	TPtr policyFilePtr(policyFile->Des());
	
	HBufC *tempPolicyFile = HBufC::NewLC(KMaxName);
	TPtr tempPolicyFilePtr(tempPolicyFile->Des());
	
	TInt ret(GetPolicyFile(policyFilePtr, aPolicyID));

	ret= GetPolicyPath (tempPolicyFilePtr);

	if ( (KErrNone==ret) || (KErrAlreadyExists==ret))
	{
		HBufC *fPolicyName = HBufC::NewLC(KMaxName);
		TPtr policyNamePtr(fPolicyName->Des());
		policyNamePtr.Num (aPolicyID);

		tempPolicyFilePtr.Append (policyNamePtr);
		tempPolicyFilePtr.Append (_L("_temp"));
		tempPolicyFilePtr.Append (KDatExtn);
		
		CleanupStack::PopAndDestroy(fPolicyName);
	}
	
	CFileMan *fm = CFileMan::NewL(iFsSession);
	TInt restoreResult = fm->Copy(tempPolicyFilePtr, policyFilePtr, CFileMan::EOverWrite);
	delete fm;
	
	CleanupStack::PopAndDestroy(tempPolicyFile);
	CleanupStack::PopAndDestroy(policyFile);
	return restoreResult;
}
