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
* Description:       Defines security policy data structure 
 *
*/






#include <e32debug.h>
#include "rtsecmgrpolicy.h"
#include "rtsecmgrserverdef.h"

CPolicy::CPolicy(TPolicyID aPolicyID) :
	iPolicyID(aPolicyID)
	{
	}

void CPolicy::ConstructL(const RProtectionDomains& aProtectionDomains, RAliasGroup& aAliasGroup)
	{
	iAccessPolicy.Reset();
	iAliasGroup.Reset();
	iAccessPolicy = aProtectionDomains;
	iAliasGroup = aAliasGroup;
	}

CPolicy* CPolicy::NewL(TPolicyID aPolicyID,
		const RProtectionDomains& aProtectionDomains, RAliasGroup& aAliasGroup)
	{
	CPolicy* self = CPolicy::NewLC (aPolicyID, aProtectionDomains, aAliasGroup);
	CleanupStack::Pop (self);
	return self;
	}

CPolicy* CPolicy::NewLC(TPolicyID aPolicyID,
		const RProtectionDomains& aProtectionDomains, RAliasGroup& aAliasGroup)
	{
	CPolicy* self = new (ELeave) CPolicy(aPolicyID);
	CleanupStack::PushL (self);
	self->ConstructL (aProtectionDomains, aAliasGroup);
	return self;
	}

CPolicy* CPolicy::NewL(TPolicyID aPolicyID, RStoreReadStream& aSource)
	{
	CPolicy* self = CPolicy::NewLC (aPolicyID, aSource);
	CleanupStack::Pop (self);
	return self;
	}

CPolicy* CPolicy::NewLC(TPolicyID aPolicyID, RStoreReadStream& aSource)
	{
	CPolicy* self = new (ELeave) CPolicy(aPolicyID);
	CleanupStack::PushL (self);
	self->InternalizeL (aSource);
	return self;
	}

void CPolicy::InternalizeL(RStoreReadStream& aSource)
	{
	//Read version of the policy
	TReal version(aSource.ReadReal32L());
	TInt aliasCnt(aSource.ReadInt32L());
	
	for (TInt i(0); i<aliasCnt;++i)
		{
		CPermission* alias = CPermission::NewL ();
		alias->InternalizeL(aSource);
		iAliasGroup.Append (alias);
		}
		
	TInt domainCnt(aSource.ReadInt32L ());
	for (TInt i(0); i<domainCnt;++i)
		{
		CProtectionDomain* domain = CProtectionDomain::NewL (aSource);
		iAccessPolicy.Append (domain);
		}
	}

CProtectionDomain* CPolicy::ProtectionDomain(const TDesC& aDomainName) const
	{
	for (TInt idx(0); idx<iAccessPolicy.Count ();++idx)
		{
		if ( KErrNone ==iAccessPolicy[idx]->DomainName().CompareF (aDomainName))
			{
			return iAccessPolicy[idx];
			}
		}

	return NULL;
	}

CPermission* CPolicy::AliasGroup(const TDesC& aAliasName) const
	{
	for (TInt idx(0); idx<iAliasGroup.Count ();++idx)
		{
		if ( KErrNone ==iAliasGroup[idx]->PermName().CompareF (aAliasName))
			{
			return iAliasGroup[idx];
			}
		}

	return NULL;
	}
