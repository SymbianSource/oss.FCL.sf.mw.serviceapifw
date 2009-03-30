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
* Description:       Defines CTrustInfo class
 *
*/






#include <rtsecmgrtrustinfo.h>

// ---------------------------------------------------------------------------
// Defintiion of default private constructor
// ---------------------------------------------------------------------------
//
CTrustInfo::CTrustInfo()
{	
}

// ---------------------------------------------------------------------------
// Definition of second phase constructor
//
// Constructs a CTrustInfo instance
// ---------------------------------------------------------------------------
//
EXPORT_C CTrustInfo* CTrustInfo::NewL()
{
	CTrustInfo* self = CTrustInfo::NewLC();
	CleanupStack::Pop(self);
	return self;
}

// ---------------------------------------------------------------------------
// Definition of second phase constructor
//
// Constructs a CTrustInfo instance and leaves the created instance
// on the cleanupstack
// ---------------------------------------------------------------------------
//
EXPORT_C CTrustInfo* CTrustInfo::NewLC()
{
	CTrustInfo* self = new (ELeave) CTrustInfo();
	CleanupStack::PushL(self);
	return self;	
}

// ---------------------------------------------------------------------------
// Destructor
//
// ---------------------------------------------------------------------------
//
EXPORT_C CTrustInfo::~CTrustInfo()
{
}

