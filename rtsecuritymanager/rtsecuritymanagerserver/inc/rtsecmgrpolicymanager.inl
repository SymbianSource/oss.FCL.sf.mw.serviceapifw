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
* Description:       Definition of policy manager inline functions
 *
*/







inline CPolicyManager::CPolicyManager(CSecMgrStore* aSecMgrDB) :
	iSecMgrDB(aSecMgrDB)
	{
	}

inline CPolicyManager::~CPolicyManager()
	{
	iPolicies.ResetAndDestroy ();
	}

inline void CPolicyManager::RestorePoliciesL()
	{
	if ( iSecMgrDB)
		iSecMgrDB->RestorePoliciesL (iPolicies);
	}

inline TInt CPolicyManager::Count() const
	{
	return iPolicies.Count ();
	}