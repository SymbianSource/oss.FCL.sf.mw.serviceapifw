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
* Description:       Definition of security protection domain inline functions
 *
*/







/**
 * Default private constructor
 * 
 */
inline CProtectionDomain::CProtectionDomain() :
	iDomain(NULL)
	{
	}

/**
 * Two phased constructor
 * 
 */
inline void CProtectionDomain::ConstructL()
	{
	iPermSet = CPermissionSet::NewL ();
	}

/**
 * Gets underlying permission set instance
 * 
 * @return const CPermissionSet& permission set instance
 * 
 */
inline const CPermissionSet& CProtectionDomain::PermSet() const
	{
	return *iPermSet;
	}

/**
 * Gets domain name
 * 
 * @return const TDesC& domain name
 * 
 */
inline const TDesC& CProtectionDomain::DomainName() const
	{
	if ( iDomain)
		return *iDomain;
	else
		return KNullDesC ();
	}

/**
 * Sets domain name
 * 
 * @param aDomainName const TDesC& input domain name string
 */
inline void CProtectionDomain::SetDomainName(const TDesC& aDomain)
	{
	if ( iDomain)
		{
		delete iDomain;
		iDomain = NULL;
		}

	iDomain = aDomain.AllocL ();
	}

/**
 * Sets permission set
 * 
 * @param aPermissionSet CPermissionSet* input permission set
 */
inline void CProtectionDomain::SetCapInfo(CPermissionSet* aPermissionSet)
	{
	if ( iPermSet)
		{
		delete iPermSet;
		}

	iPermSet = aPermissionSet;
	}