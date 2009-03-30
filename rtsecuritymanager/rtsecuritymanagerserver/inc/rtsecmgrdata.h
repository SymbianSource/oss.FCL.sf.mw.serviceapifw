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
* Description:       Defines security manager server side data strucutures
 *
*/






#ifndef _RTSECMGRDATA_H
#define _RTSECMGRDATA_H

#include "rtsecmgrprotectiondomain.h"
#include "rtsecmgrpolicy.h"
#include <rtsecmgrtrustinfo.h>

typedef RPointerArray<CProtectionDomain> RProtectionDomains;
typedef RPointerArray<CTrustInfo> RTrustArray;
typedef RPointerArray<CPolicy> RPolicies;

#endif  //_RTSECMGRDATA_H

// End of file
