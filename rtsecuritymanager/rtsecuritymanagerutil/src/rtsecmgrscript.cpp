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





#include <rtsecmgrutility.h>
#include <rtsecmgrscript.h>
#include <s32mem.H>

const TReal DEFAULT_VERSION(1.0);
// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CScript::~CScript()
	{
	delete iPermissionSet;
	if(iHashMark)
		delete iHashMark;
	}

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CScript* CScript::NewL(TPolicyID aPolicyID,TExecutableID aScriptID)
	{
	CScript* self = CScript::NewLC(aPolicyID,aScriptID);
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CScript* CScript::NewLC(TPolicyID aPolicyID,TExecutableID aScriptID)
	{
	CScript* self = new (ELeave) CScript(aPolicyID,aScriptID);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// ---------------------------------------------------------------------------
// Two-phased constructor
// ---------------------------------------------------------------------------
//
void CScript::ConstructL()
	{
	iPermissionSet = CPermissionSet::NewL ();
	}

// ---------------------------------------------------------------------------
// Overloaded assignment operator
// ---------------------------------------------------------------------------
//
EXPORT_C const CScript& CScript::operator=(const CScript& aRhs)
	{
	if(iPermissionSet )
		{
		delete iPermissionSet;
		iPermissionSet = NULL;
		}

	iPermissionSet = CPermissionSet::NewL(*aRhs.iPermissionSet);
	iScriptID = aRhs.iScriptID;
	iPolicyID = aRhs.iPolicyID;
	iPermGrant = aRhs.iPermGrant;
	iPermDenied = aRhs.iPermDenied;

	if(iHashMark)
		{
			delete iHashMark;
			iHashMark = NULL;
		}
	
	if(aRhs.iHashMark)
		{
		iHashMark = aRhs.iHashMark->AllocLC();
		CleanupStack::Pop(iHashMark);
		}
	return *this;
	}

// ---------------------------------------------------------------------------
// Gets script identifier
// ---------------------------------------------------------------------------
//
EXPORT_C TExecutableID CScript::ScriptID() const
	{
	return iScriptID;
	}

// ---------------------------------------------------------------------------
// Gets policy identifier
// ---------------------------------------------------------------------------
//
EXPORT_C TPolicyID CScript::PolicyID() const
	{
	return iPolicyID;
	}

// ---------------------------------------------------------------------------
// Sets permission set data of the script
// ---------------------------------------------------------------------------
//
EXPORT_C void CScript::SetPermissionSet(const CPermissionSet& aPermSet)
	{
	if ( iPermissionSet)
		{
		delete iPermissionSet;
		iPermissionSet=NULL;
		}

	iPermissionSet = CPermissionSet::NewL (aPermSet);
	}

// ---------------------------------------------------------------------------
// Gets permission set data of the script
// ---------------------------------------------------------------------------
//
EXPORT_C const CPermissionSet& CScript::PermissionSet() const
	{
	return *iPermissionSet;
	}
// ---------------------------------------------------------------------------
// Gets permission set data of the script
// ---------------------------------------------------------------------------
//
EXPORT_C CPermissionSet& CScript::PermissionSet()
	{
	return *iPermissionSet;
	}

// ---------------------------------------------------------------------------
// Gets permanently granted permissions of the script
// ---------------------------------------------------------------------------
//
EXPORT_C TPermGrant CScript::PermGranted() const
	{
	return iPermGrant;
	}

// ---------------------------------------------------------------------------
// Gets permanently denied permissions of the script
// ---------------------------------------------------------------------------
//
EXPORT_C TPermGrant CScript::PermDenied() const
	{
	return iPermDenied;
	}

// ---------------------------------------------------------------------------
// ExternalizeLs script data to stream
// ---------------------------------------------------------------------------
//
EXPORT_C void CScript::ExternalizeL(RWriteStream& aSink) const
	{
	aSink.WriteReal32L(DEFAULT_VERSION);
	iPermissionSet->ExternalizeL (aSink);
	aSink.WriteInt32L (iScriptID);
	aSink.WriteInt32L (iPolicyID);
	aSink.WriteUint32L (iPermGrant);
	aSink.WriteUint32L (iPermDenied);
	if(iHashMark)
		{
			aSink.WriteUint32L(iHashMark->Length());
			aSink.WriteL(*iHashMark,iHashMark->Length());
		}
	else
		aSink.WriteInt32L(0);
	}

// ---------------------------------------------------------------------------
// InternalizeLs script data from stream
// ---------------------------------------------------------------------------
//
EXPORT_C void CScript::InternalizeL(RReadStream& aSource)
	{
	if ( iPermissionSet)
		{
		delete iPermissionSet;
		iPermissionSet=NULL;
		iPermissionSet = CPermissionSet::NewL ();
		}
	TReal version(aSource.ReadReal32L());
	iPermissionSet->InternalizeL (aSource);
	iScriptID = aSource.ReadInt32L ();
	iPolicyID = aSource.ReadInt32L ();
	iPermGrant = aSource.ReadUint32L ();
	iPermDenied = aSource.ReadUint32L ();
	TInt hashMarkLen(aSource.ReadUint32L());
	if(iHashMark)
		{
			delete iHashMark;
			iHashMark = NULL;
		}
		
	if(hashMarkLen)	
		{
		iHashMark = HBufC::NewL(hashMarkLen);
		TPtr ptr(iHashMark->Des());
		aSource.ReadL(ptr,hashMarkLen);
		}
	}

// ---------------------------------------------------------------------------
// Sets permanently granted permissions of the script
// ---------------------------------------------------------------------------
//
EXPORT_C void CScript::SetPermGranted(TPermGrant aPermGrant)
	{
	iPermGrant = aPermGrant;
	}

// ---------------------------------------------------------------------------
// Sets permanently denied permissions of the script
// ---------------------------------------------------------------------------
//
EXPORT_C void CScript::SetPermDenied(TPermGrant aPermDenied)
	{
	iPermDenied = aPermDenied;
	}
