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
* Description:       Defines permission data structure
 *
*/






#include <s32mem.h>
#include <rtsecmgrutility.h>

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission* CPermission::NewL()
	{
	CPermission* self = CPermission::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission* CPermission::NewLC()
	{
	CPermission* self = new (ELeave) CPermission();
	CleanupStack::PushL(self);
	return self;
	}

CPermission::CPermission(const CPermission& aPermission)
	{
	if ( iPermName)
		{
		delete iPermName;
		iPermName = NULL;
		}

	iPermName = aPermission.PermName().Alloc();
	iPermissionData = aPermission.PermissionData();
	}
CPermission::CPermission(const RCapabilityArray& aCapabilities) :iPermName(NULL)
	{
	for(TInt i(0);i!=aCapabilities.Count();++i)
		{
		TUint32 capBit = LOWBIT << (aCapabilities[i]);
		iPermissionData = KDefaultNullBit;
		iPermissionData |= capBit;
		}	
	}

CPermission::CPermission(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt) :iPermName(NULL)
	{
	iPermissionData = KDefaultNullBit;
	for(TInt i(0);i!=aCapabilities.Count();++i)
		{
		TUint32 capBit = LOWBIT << (aCapabilities[i]);
		iPermissionData |= capBit;

		if ( aDefOpt&RTUserPrompt_OneShot)
			iPermissionData |= DEFAULT_ONESHOT;
		else
			if ( aDefOpt&RTUserPrompt_Session)
				iPermissionData |= DEFAULT_SESSION;
			else
				if ( aDefOpt&RTUserPrompt_Permanent)
					iPermissionData |= DEFAULT_BLANKET;
		}
	}

CPermission::CPermission(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt,TUserPromptOption aCondition) :iPermName(NULL)
	{
	iPermissionData = KDefaultNullBit;
	for(TInt i(0);i!=aCapabilities.Count();++i)
		{
		TUint32 capBit = LOWBIT << (aCapabilities[i]);
		iPermissionData |= capBit;
		iPermissionData |= aCondition;

		if ( aDefOpt&RTUserPrompt_OneShot)
			iPermissionData |= DEFAULT_ONESHOT;
		else
			if ( aDefOpt&RTUserPrompt_Session)
				iPermissionData |= DEFAULT_SESSION;
			else
				if ( aDefOpt&RTUserPrompt_Permanent)
					iPermissionData |= DEFAULT_BLANKET;
		}
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission::~CPermission()
	{
	if(iPermName)
		{
		delete iPermName;
		iPermName = NULL;
		}
	}

// ---------------------------------------------------------------------------
// Overloaded constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission* CPermission::NewL(const CPermission& aPermission)
	{
	CPermission* self = CPermission::NewLC(aPermission);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CPermission* CPermission::NewLC(const CPermission& aPermission)
	{
	CPermission* self = new (ELeave) CPermission(aPermission);
	CleanupStack::PushL(self);
	return self;
	}
// ---------------------------------------------------------------------------
// Overloaded constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission* CPermission::NewL(const RCapabilityArray& aCapabilities)
	{
	CPermission* self = CPermission::NewLC(aCapabilities);
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Overloaded constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission* CPermission::NewLC(const RCapabilityArray& aCapabilities)
	{
	CPermission* self = new (ELeave) CPermission(aCapabilities);
	CleanupStack::PushL(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Overloaded constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission* CPermission::NewL(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt)
	{
	CPermission* self = CPermission::NewLC(aCapabilities, aDefOpt);
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Overloaded constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission* CPermission::NewLC(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt)
	{
	CPermission* self = new (ELeave) CPermission(aCapabilities ,aDefOpt);
	CleanupStack::PushL(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Overloaded constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission* CPermission::NewL(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt,
		TUserPromptOption aCondition)
	{
	CPermission* self = CPermission::NewLC(aCapabilities, aDefOpt, aCondition);
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Overloaded constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermission* CPermission::NewLC(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt,
		TUserPromptOption aCondition)
	{
	CPermission* self = new (ELeave) CPermission(aCapabilities, aDefOpt, aCondition);
	CleanupStack::PushL(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Returns default condition
// ---------------------------------------------------------------------------
//
EXPORT_C TUserPromptOption CPermission::Default() const
	{
	TUint32 ret(iPermissionData & DEFAULT_ALL);
	if(DEFAULT_ONESHOT&ret)
		{
		return RTUserPrompt_OneShot;
		}
	else if(DEFAULT_SESSION&ret)
		{
		return RTUserPrompt_Session;
		}
	else if(DEFAULT_BLANKET&ret)
		{
		return RTUserPrompt_Permanent;
		}
	return (TUserPromptOption)ret;
	}

// ---------------------------------------------------------------------------
// Returns condition set of the permission
// ---------------------------------------------------------------------------
//
EXPORT_C TUserPromptOption CPermission::Condition() const
	{
	return (TUserPromptOption)(iPermissionData & CONDITION_ALL);
	}

// ---------------------------------------------------------------------------
// Returns capability of the permission
// ---------------------------------------------------------------------------
//
EXPORT_C TCapability CPermission::Capability() const
	{
	TCapability retValue = (TCapability)(iPermissionData & CAPABILITY_ALL);

	for(TInt i(ECapabilityTCB);i<=ECapabilityUserEnvironment;++i)
		{
		TUint32 temp = LOWBIT << i;
		if(retValue & temp)
			{
			return (TCapability)i;
			}
		}
	return ECapability_None;
	}

// ---------------------------------------------------------------------------
// Returns capability of the permission
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermission::Capabilitilites(RCapabilityArray& aCapabilities) const
	{
	TCapability retValue = (TCapability)(iPermissionData & CAPABILITY_ALL);

	for(TInt i(ECapabilityTCB) ;i <= ECapabilityUserEnvironment ;i++)
		{
		TUint32 temp = LOWBIT << i;
		if(retValue & temp)
			{
			aCapabilities.Append((TCapability)i);
			}
		}
	}

// ---------------------------------------------------------------------------
// Sets default condition of the permission
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermission::SetDefault(TUserPromptOption aDefOpt)
	{
	if ( RTUserPrompt_OneShot==aDefOpt)
		{
		iPermissionData |= DEFAULT_ONESHOT;
		iPermissionData |= CONDITION_ONESHOT;
		}
	else
		if ( RTUserPrompt_Session==aDefOpt)
			{
			iPermissionData |= DEFAULT_SESSION;
			iPermissionData |= CONDITION_SESSION;
			}
		else
			if ( RTUserPrompt_Permanent==aDefOpt)
				{
				iPermissionData |= DEFAULT_BLANKET;
				iPermissionData |= CONDITION_BLANKET;
				}
	}

// ---------------------------------------------------------------------------
// Sets condition of the permission
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermission::SetCondition(TUserPromptOption aCondition)
	{
	iPermissionData |= aCondition;
	}

// ---------------------------------------------------------------------------
// Sets name of the permission
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermission::SetPermName(const TDesC& aPermName)
	{
	if ( iPermName)
		{
		delete iPermName;
		iPermName = NULL;
		}

	iPermName = aPermName.AllocL ();
	}
	
// ---------------------------------------------------------------------------
// Returns name of the permission
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CPermission::PermName() const
	{
	if ( iPermName)
		return *iPermName;
	else
		return KNullDesC ();
	}

// ---------------------------------------------------------------------------
// Adds an unconditional capability
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermission::AppendCapPermData(TCapability aCap)
	{
	if ( (aCap>=ECapabilityTCB)&&(aCap<=ECapabilityUserEnvironment))
		{
		iPermissionData = iPermissionData | (LOWBIT << aCap);
		}
	}

// ---------------------------------------------------------------------------
// Sets the permission data
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermission::SetPermissionData(TPermissionData aPermData)
	{
		iPermissionData = aPermData;
	}

// ---------------------------------------------------------------------------
// Returns PermissionData of the permission
// ---------------------------------------------------------------------------
//
EXPORT_C TPermissionData CPermission::PermissionData() const
	{
	return iPermissionData;
	}

// ---------------------------------------------------------------------------
// Internalizes permission data from stream
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermission::InternalizeL(RReadStream& aInStream)
	{
	TInt32 len = aInStream.ReadInt32L ();
	if ( iPermName)
		{
		delete iPermName;
		iPermName = NULL;
		}

	iPermName = HBufC::NewL (aInStream, len);

	iPermissionData = aInStream.ReadUint32L ();
	}

// ---------------------------------------------------------------------------
// ExternalizeLs permission data to stream
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermission::ExternalizeL(RWriteStream& aOutStream) const
	{
	if(iPermName)
		{
		aOutStream.WriteInt32L (iPermName->Des().MaxLength ());
		aOutStream << *iPermName;
		}
	else
		{
		aOutStream.WriteInt32L (0);
		aOutStream << KNullDesC;
		}	
	
	aOutStream.WriteUint32L (iPermissionData);
	}

// ---------------------------------------------------------------------------
// Copy constructor
// ---------------------------------------------------------------------------
//
CPermissionSet::CPermissionSet(const CPermissionSet& aPermSet)
	{
	iUncondCapSet = aPermSet.iUncondCapSet;
	}

void CPermissionSet::ConstructL(const CPermissionSet& aPermSet)
	{
	iPermissions.ResetAndDestroy ();
	//iPermissions.Close();

	for (TInt idx(0); idx!=aPermSet.iPermissions.Count ();++idx)
		{
		CPermission *temp = CPermission::NewL(*(aPermSet.iPermissions[idx]));
		iPermissions.Append (temp);
		}

	}

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermissionSet* CPermissionSet::NewL()
	{
	CPermissionSet* self = CPermissionSet::NewLC();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermissionSet* CPermissionSet::NewLC()
	{
	CPermissionSet* self = new (ELeave) CPermissionSet();
	CleanupStack::PushL(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermissionSet* CPermissionSet::NewL(const CPermissionSet& aPermSet)
	{
	CPermissionSet* self = CPermissionSet::NewLC(aPermSet);
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermissionSet* CPermissionSet::NewLC(const CPermissionSet& aPermSet)
	{
	CPermissionSet* self = new (ELeave) CPermissionSet(aPermSet);
	CleanupStack::PushL(self);
	self->ConstructL(aPermSet);
	return self;
	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPermissionSet::~CPermissionSet()
{
	iPermissions.ResetAndDestroy();
}

// ---------------------------------------------------------------------------
// Adds an unconditional capability
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermissionSet::AppendUncondCap(TCapability aCap)
	{
	if ( (aCap>=ECapabilityTCB)&&(aCap<=ECapabilityUserEnvironment))
		{
		iUncondCapSet = iUncondCapSet | (LOWBIT << aCap);
		}
	}

// ---------------------------------------------------------------------------
// Adds an unconditional capability
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermissionSet::AppendUncondCapabilities(TUnConditionalPermission aUnConditionals)
	{
		iUncondCapSet = aUnConditionals;
	}

// ---------------------------------------------------------------------------
// Returns unconditional capabilities of thhe permissionset
// ---------------------------------------------------------------------------
//
EXPORT_C TUnConditionalPermission CPermissionSet::UnconditionalCaps() const
	{
	return iUncondCapSet;
	}
// ---------------------------------------------------------------------------
// Returns unconditional capabilities of thhe permissionset
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermissionSet::UnconditionalCaps(RUnConditionals& aUnConditionals) const
	{
	for (TInt i(ECapabilityTCB); i!=ECapability_Limit;++i)
		{
		TUint32 capBitToCheck(KDefaultEnableBit);
		capBitToCheck = capBitToCheck << i;

		if ( capBitToCheck&iUncondCapSet)
			{
			aUnConditionals.Append ((TCapability)i);
			}
		}
	}

// ---------------------------------------------------------------------------
// ExternalizeLs permissionset data to stream
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermissionSet::ExternalizeL(RWriteStream& aSink) const
	{
	aSink.WriteInt32L (iPermissions.Count ());
	for (TInt idx(0); idx!=iPermissions.Count ();++idx)
		{
		iPermissions[idx]->ExternalizeL (aSink);
		}

	aSink.WriteUint32L (iUncondCapSet);
	}

// ---------------------------------------------------------------------------
// InternalizeL permissionset data from stream
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermissionSet::InternalizeL(RReadStream& aSource)
	{
	iPermissions.Reset ();
	TInt cnt(aSource.ReadInt32L ());

	for (TInt idx(0); idx!=cnt;++idx)
		{
		CPermission* permission = CPermission::NewL();
		permission->InternalizeL (aSource);
		iPermissions.Append (permission);
		}

	iUncondCapSet = aSource.ReadUint32L ();
	}

// ---------------------------------------------------------------------------
// Adds a permission data to the permissionset
// ---------------------------------------------------------------------------
//
EXPORT_C void CPermissionSet::AppendPermission(CPermission& aPermission)
	{
	iPermissions.Append (&aPermission);
	}

// ---------------------------------------------------------------------------
// Returns permission data
// ---------------------------------------------------------------------------
//
EXPORT_C const RPermissions& CPermissionSet::Permissions() const
	{
	return iPermissions;
	}

// ---------------------------------------------------------------------------
// Returns permission data
// ---------------------------------------------------------------------------
//
EXPORT_C RPermissions& CPermissionSet::Permissions()
	{
	return iPermissions;
	}
