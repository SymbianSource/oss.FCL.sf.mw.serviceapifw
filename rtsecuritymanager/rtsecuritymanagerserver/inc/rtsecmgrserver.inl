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





/*
 *  Constructor
 */
inline CRTSecMgrServer::CRTSecMgrServer(CActive::TPriority aActiveObjectPriority) :
  	//CServer2(aActiveObjectPriority)
	CPolicyServer( aActiveObjectPriority, KSecMgrSrvPolicy)
	{
	iSessionCount = 0;
	}

/**
 A utility function to panic the server.
 */
inline void CRTSecMgrServer::PanicServer(TSecMgrServPanic aPanic)
	{
	_LIT (KTxtSecMgrServer, "SecurityMgrServer");
	User::Panic (KTxtSecMgrServer, aPanic);
	}

inline CObjectCon* CRTSecMgrServer::NewContainerL()
	{
	return iContainerIndex->CreateL ();
	}

inline void CRTSecMgrServer::AddSession()
    {
    iShutDown.Cancel();
    ++iSessionCount;
    }

inline void CRTSecMgrServer::RemoveSession()
    {
    if (--iSessionCount == 0)
    	if(!iShutDown.IsActive())
        iShutDown.Start();    
    }

inline void CRTSecMgrServer::RemoveFromContainerIx(CObjectCon *aContainer)
    {
    iContainerIndex->Remove(aContainer);   
    }

inline TInt CRTSecMgrServer::GetScriptFile(TExecutableID aExecID,
		TDes& aScriptFile)
	{
	return iScriptMgr->GetScriptFile (aExecID, aScriptFile);
	}

inline TInt CRTSecMgrServer::GetCapabilityInfo(CScript& aScriptInfo)
	{
	return iScriptMgr->ScriptInfo (aScriptInfo);
	}

inline void CRTSecMgrServer::UpdatePermGrantL(TExecutableID aExecID,
		TPermGrant aPermGrant, TPermGrant aPermDenied)
	{
	iScriptMgr->UpdatePermGrantL (aExecID, aPermGrant, aPermDenied);
	}

inline TInt CRTSecMgrServer::ReadCounter(TExecutableID& aExecID)
	{
	return iSecMgrDb->ReadCounter (aExecID);
	}

inline void CRTSecMgrServer::AddPolicyL(TPolicyID aPolicyID,
		const RProtectionDomains& aPolicyInfo, RAliasGroup& aAliasInfo)
	{
	CPolicy* policy = CPolicy::NewLC (aPolicyID, aPolicyInfo, aAliasInfo/*,RTrustArray()*/);
	iPolicyMgr->RegisterPolicyL (*policy);
	CleanupStack::Pop (policy);
	}

inline TInt CRTSecMgrServer::RemovePolicy(TPolicyID aPolicyID)
	{
	return iPolicyMgr->UnRegisterPolicy (aPolicyID);
	}

inline TExecutableID CRTSecMgrServer::RegisterScript(TPolicyID aPolicyID/*,const CTrustInfo& aTrustInfo*/)
	{
	return iScriptMgr->RegisterScript (aPolicyID);
	}

inline TExecutableID CRTSecMgrServer::RegisterScript(TPolicyID aPolicyID, const  TDesC& aHashValue)
	{
	return iScriptMgr->RegisterScript (aPolicyID, aHashValue);
	}

inline void CRTSecMgrServer::UnRegisterScriptL(TExecutableID aExecID,
		TPolicyID aPolicyID)
	{
	iScriptMgr->UnRegisterScriptL (aExecID, aPolicyID);
	}

inline TBool CRTSecMgrServer::IsValidPolicy(TPolicyID aPolicyID) const
	{
	return iPolicyMgr->HasPolicy (aPolicyID);
	}

inline TInt CRTSecMgrServer::BackupFile(TPolicyID aPolicyID)
	{
	return iSecMgrDb->BackupFile(aPolicyID);
	}
	
inline TInt CRTSecMgrServer::RemoveTempPolicy(TPolicyID aPolicyID)
	{	
	return iSecMgrDb->RemoveTempPolicy(aPolicyID);
	}
	
inline TInt CRTSecMgrServer::RestoreTempPolicy(TPolicyID aPolicyID)
	{	
	return iSecMgrDb->RestoreTempPolicy(aPolicyID);
	}

inline CPolicy* CRTSecMgrServer::ReturnPolicyInfo(TPolicyID aPolicyID)
	{	
	return iPolicyMgr->Policy(aPolicyID);
	}
	
inline void CRTSecMgrServer::StorePolicyL(const CPolicy& aPolicy)
	{	
	return iSecMgrDb->StorePolicyL(aPolicy);
	}
