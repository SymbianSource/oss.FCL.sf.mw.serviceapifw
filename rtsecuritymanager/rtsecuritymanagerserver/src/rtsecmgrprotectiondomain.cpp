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





#include "rtsecmgrprotectiondomain.h"
#include "rtsecmgrserverdef.h"

#include <e32debug.h>
#include <centralrepository.h>

CProtectionDomain* CProtectionDomain::NewL()
	{
	CProtectionDomain* self = CProtectionDomain::NewLC ();
	CleanupStack::Pop (self);
	return self;
	}

CProtectionDomain* CProtectionDomain::NewLC()
	{
	CProtectionDomain* self = new (ELeave) CProtectionDomain();
	CleanupStack::PushL (self);
	self->ConstructL ();
	return self;
	}

CProtectionDomain* CProtectionDomain::NewL(RStoreReadStream& aSource)
	{
	CProtectionDomain* self = CProtectionDomain::NewLC (aSource);
	CleanupStack::Pop (self);
	return self;
	}

CProtectionDomain* CProtectionDomain::NewLC(RStoreReadStream& aSource)
	{
	CProtectionDomain* self = CProtectionDomain::NewLC ();
	self->InternalizeL (aSource);
	return self;
	}

CProtectionDomain::~CProtectionDomain()
	{
	if ( iDomain)
		{
		delete iDomain;
		iDomain = NULL;
		}

	if ( iPermSet)
		{
		delete iPermSet;
		iPermSet = NULL;
		}
	}

void CProtectionDomain::ExternalizeL(RStoreWriteStream& aSink) const
	{
	//externalise policy info data
	aSink.WriteInt32L (iDomain->Des().MaxLength ());
	aSink << *iDomain;
	iPermSet->ExternalizeL (aSink);
	}

void CProtectionDomain::InternalizeL(RStoreReadStream& aSource)
	{
	TInt32 len = aSource.ReadInt32L ();
	if ( iDomain)
		{
		delete iDomain;
		iDomain = NULL;
		}

	iDomain = HBufC::NewL (aSource, len);

	if ( iPermSet)
		{
		delete iPermSet;
		iPermSet = NULL;
		}

	iPermSet = CPermissionSet::NewL ();
	iPermSet->InternalizeL (aSource);
	}
