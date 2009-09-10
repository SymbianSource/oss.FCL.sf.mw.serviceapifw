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





#include "rtsecmgrscriptmanager.h"
#include "rtsecmgrserverdef.h"

#include "rtsecmgrstore.h"
#include "rtsecmgrpolicy.h"
#include "rtsecmgrpolicymanager.h"
#include "rtsecmgrscriptmanager.h"

#include <e32debug.h>
//#include <centralrepository.h>

_LIT(KDefaultTrustPolicy,"UnTrusted");

CScriptManager* CScriptManager::NewL(CSecMgrStore* aSecMgrDB,CPolicyManager* aPolicyMgr)
{	
	CScriptManager* pThis = CScriptManager::NewLC(aSecMgrDB,aPolicyMgr);
	CleanupStack::Pop(pThis);
	return pThis;
}

CScriptManager* CScriptManager::NewLC(CSecMgrStore* aSecMgrDB,CPolicyManager* aPolicyMgr)
{	
	CScriptManager* pThis = new (ELeave) CScriptManager(aSecMgrDB,aPolicyMgr);
	CleanupStack::PushL(pThis);
	pThis->ConstructL();
	return pThis;
}

CScriptManager::~CScriptManager()
{
	if(iSecMgrDB)
	{
		iSecMgrDB->WriteCounter(iID);	
	}
	
	iScripts.ResetAndDestroy();
}
void CScriptManager::ConstructL()	
{
	iSecMgrDB->ReadCounter(iID);
}

TExecutableID CScriptManager::RegisterScript(TPolicyID aPolicyID/*,const CTrustInfo& aTrustPolicy*/)
{	
	TInt ret(ErrPolicyIDDoesNotExist);
	
	CPolicy* policy = iPolicyMgr->Policy(aPolicyID);
	
	if(policy)
		{
		CProtectionDomain* domain = policy->ProtectionDomain(KDefaultTrustPolicy);
		
		if(domain)
			{
				TExecutableID execID(GetID());
				CScript* script = CScript::NewL(aPolicyID,execID);
				script->SetPermissionSet(domain->PermSet());
				iScripts.Append(script);
				
				TRAPD(err,iSecMgrDB->RegisterScriptL(execID,*script));
				
				if(KErrNone==err)
				{
					return execID;
				}
				else
				{
					return ret;
				}
			}			
		
		}
	return ret;	
	
}

TExecutableID CScriptManager::RegisterScript(TPolicyID aPolicyID,const TDesC& aHashValue/*,const CTrustInfo& aTrustPolicy*/)
	{	
		TInt ret(ErrPolicyIDDoesNotExist);
		
		CPolicy* policy = iPolicyMgr->Policy(aPolicyID);
		
		if(policy)
			{
			CProtectionDomain* domain = policy->ProtectionDomain(KDefaultTrustPolicy);
			
			if(domain)
				{
					TExecutableID execID(GetID());
					CScript* script = CScript::NewLC(aPolicyID,execID);
					script->SetHashMarkL(aHashValue);
					script->SetPermissionSet(domain->PermSet());
					iScripts.Append(script);
					
					TRAPD(err,iSecMgrDB->RegisterScriptL(execID, *script));
					
					if(KErrNone==err)
					{
					ret=execID;
					}
					CleanupStack::Pop(script);
				}			
			
			}
		return ret;	
		
	}


void CScriptManager::UnRegisterScriptL(TExecutableID aExecID,TPolicyID aPolicyID)
{	
	TInt ret;
	CScript *scriptInfo = CScript::NewL(KInvalidPolicyID ,aExecID);
	ret = ScriptInfo(*scriptInfo);
	
	
	if( (KErrNone<=ret) && (scriptInfo->PolicyID() != aPolicyID))
	{
		ret = ErrInvalidPolicyID;	
	}
	
	delete scriptInfo;
	scriptInfo = NULL;
	
	if(ret == KErrNone)
	{
		for(TInt i=0; i!=iScripts.Count();++i)
		{
			if(iScripts[i]->ScriptID()==aExecID)
			{
				delete iScripts[i];
				iScripts.Remove(i);	//remove from the cache first		
				break;
			}		 
		}		
		iSecMgrDB->UnRegisterScriptL(aExecID);
	}
	else
		User::Leave(ret);
}

TInt CScriptManager::ScriptInfo(CScript& aScriptInfo)
{	
	//Check if script info is already present in the cache (iScriptStore)
	//If yes, 	
	for(TInt i=0; i!=iScripts.Count();++i)
		{
		if(iScripts[i]->ScriptID()==aScriptInfo.ScriptID())
			{
				{
				aScriptInfo = *iScripts[i];				
				return KErrNone;	
				}	
			}
		}
	
	//Read the script file
	TInt ret = iSecMgrDB->ReadScriptInfo(aScriptInfo.ScriptID(),aScriptInfo);
	
	if(KErrNone==ret)
	{
		iScripts.AppendL(&aScriptInfo);		
	}
	
	return ret;
}

TInt CScriptManager::GetScriptFile(TExecutableID aExecID,TDes& aScriptFile)
{
	return iSecMgrDB->GetScriptFile(aExecID,aScriptFile);
}

void CScriptManager::UpdatePermGrantL(TExecutableID aExecID,TPermGrant aPermGrant,TPermGrant aPermDenied)
{	
	for(TInt i(0);i!=iScripts.Count();++i)
	{
	if(iScripts[i]->ScriptID()==aExecID)
		{
			iScripts[i]->SetPermGranted(aPermGrant);
			iScripts[i]->SetPermDenied(aPermDenied);
			iSecMgrDB->WriteScriptInfoL(aExecID,*iScripts[i]);			
			return;
		}	
	}
}

void CScriptManager::UpdatePermGrantL(TExecutableID aExecID,RProviderArray aAllowedProviders,
                                        RProviderArray aDeniedProviders)
{   
    for(TInt i(0);i!=iScripts.Count();++i)
    {
    if(iScripts[i]->ScriptID()==aExecID)
        {
            iScripts[i]->SetPermGranted(aAllowedProviders);
            iScripts[i]->SetPermDenied(aDeniedProviders);
            iSecMgrDB->WriteScriptInfoL(aExecID,*iScripts[i]);          
            return;
        }   
    }
}
