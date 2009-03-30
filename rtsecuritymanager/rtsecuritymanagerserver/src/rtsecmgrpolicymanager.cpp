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
* Description:       Definition of policy manager class
 *
*/






#include <e32debug.h>
#include "rtsecmgrpolicymanager.h"
#include "rtsecmgrserverdef.h"

CPolicyManager* CPolicyManager::NewL(CSecMgrStore* aSecMgrDB)
	{
	CPolicyManager* self = CPolicyManager::NewLC(aSecMgrDB);
	CleanupStack::Pop(self);
	return self;
	}

CPolicyManager* CPolicyManager::NewLC(CSecMgrStore* aSecMgrDB)
	{
	CPolicyManager* self = new (ELeave) CPolicyManager(aSecMgrDB);
	CleanupStack::PushL (self);
	return self;
	}

void CPolicyManager::RegisterPolicyL(const CPolicy& aPolicy)
	{
	//Remove the policy if it already exists
	for (TInt idx(0); idx!=iPolicies.Count ();++idx)
		{
		if ( aPolicy.PolicyID ()==iPolicies[idx]->PolicyID ())
			{
			delete iPolicies[idx];
			iPolicies.Remove (idx);
			break;
			}
		}

	//update the cache
	iPolicies.AppendL (&aPolicy);

	iSecMgrDB->StorePolicyL (aPolicy);
	}

TInt CPolicyManager::UnRegisterPolicy(TPolicyID aPolicyID)
	{
	TBool exists(EFalse);
	TInt ret(KErrNone);
	
	for(TInt idx(0);idx<iPolicies.Count();++idx)
		{
		if(aPolicyID==iPolicies[idx]->PolicyID())
			{
			if(KErrNone==iSecMgrDB->RemovePolicy(aPolicyID))
			{
			delete iPolicies[idx];
			iPolicies.Remove(idx);
			exists=ETrue;
			}
			}
		}

		//In case if policyID does not exist in the cache...
		if(!exists)
			ret = iSecMgrDB->RemovePolicy(aPolicyID);
	

	return ret;
	}

CPolicy* CPolicyManager::Policy(TPolicyID aPolicyID) const
	{
	for (TInt idx(0); idx!=iPolicies.Count ();++idx)
		{
		if ( aPolicyID==iPolicies[idx]->PolicyID ())
			{
			return iPolicies[idx];
			}
		}

	return NULL;
	}

TBool CPolicyManager::HasPolicy(TPolicyID aPolicyID) const
	{
	for (TInt idx(0); idx!=iPolicies.Count ();++idx)
		{
		if ( aPolicyID==iPolicies[idx]->PolicyID ())
			{
			return ETrue;
			}
		}

	return EFalse;
	}

