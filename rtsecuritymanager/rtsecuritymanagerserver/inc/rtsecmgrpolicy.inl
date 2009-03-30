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
* Description:       Defines security policy data structure inline functions
 *
*/






/**
 * Gets policy identifier of the security and trust policy
 * data
 * 
 * @return TPolicyID policy identifier
 * 
 */
inline TPolicyID CPolicy::PolicyID() const
	{
	return iPolicyID;
	}

/**
 * Gets the list of protection domains associated with
 * the policy identifier
 *
 * @return const RProtectionDomains& protection domain list
 * 
 */
inline const RProtectionDomains& CPolicy::ProtectionDomain() const
	{
	return iAccessPolicy;
	}

/**
 * virtual destructor
 *  
 */
inline CPolicy::~CPolicy()
	{
	iAccessPolicy.ResetAndDestroy ();
	iAliasGroup.ResetAndDestroy();
	}

/**
 * Gets the list of alias group associated with
 * the policy identifier
 *
 * @return const RAliasGroup& alias group list
 * 
 */
inline const RAliasGroup& CPolicy::AliasGroup() const
	{
	return iAliasGroup;
	}
