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
* Description:       Definition of security protection domain
 *
*/






#ifndef C_RTSECMGRPROTECTIONDOMAIN_H
#define C_RTSECMGRPROTECTIONDOMAIN_H

#include <e32capability.h>
#include <rtsecmgrutility.h>
#include <rtsecmgrscript.h>
#include "rtsecmgrdef.h"

/* 
 * Models a protection domain. A protection domain is part of a security policy.
 * A security policy could have more than one protection domain.
 *
 * Every protection domain has a name, list of unconditionally allowed permissions
 * and set of user grantable permissions.
 *
 * @see CPermissionSet
 * 
 * @exe rtsecmgrserver.exe
 * 
 */
NONSHARABLE_CLASS(CProtectionDomain) : public CBase
	{
public:

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CProtectionDomain instance
	 * 
	 * @return CProtectionDomain* pointer to an instance of CProtectionDomain
	 */
	static CProtectionDomain* NewL();

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CProtectionDomain instance and leaves the created instance
	 * on the cleanupstack
	 * 
	 * @return CProtectionDomain* pointer to an instance of CProtectionDomain
	 */
	static CProtectionDomain* NewLC();

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CProtectionDomain instance from the input
	 * read source
	 * 
	 * @param aSource RStoreReadStream& Input readable source
	 * 
	 * @return CProtectionDomain* pointer to an instance of CProtectionDomain
	 */
	static CProtectionDomain* NewL(RStoreReadStream& aSource);

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CProtectionDomain instance from the input
	 * read source and leaves the created instance on the cleanup
	 * stack
	 *  
	 * @param aSource RStoreReadStream& Input readable source
	 * 
	 * @return CProtectionDomain* pointer to an instance of CProtectionDomain
	 */
	static CProtectionDomain* NewLC(RStoreReadStream& aSource);
	
	/**
	 * Destructor
	 * 
	 * Performs clean-up of domain name descriptor and
	 * permission set
	 * 
	 */
	~CProtectionDomain();

	/**
	 * Gets underlying permission set instance
	 * 
	 * @return const CPermissionSet& permission set instance
	 * 
	 */
	inline const CPermissionSet& PermSet() const;

	/**
	 * Gets domain name
	 * 
	 * @return const TDesC& domain name
	 * 
	 */
	inline const TDesC& DomainName() const;

	/**
	 * Sets domain name
	 * 
	 * @param aDomainName const TDesC& input domain name string
	 */
	inline void SetDomainName(const TDesC& aDomainName);

	/**
	 * Sets permission set
	 * 
	 * @param aPermissionSet CPermissionSet* input permission set
	 */
	inline void SetCapInfo(CPermissionSet* aPermissionSet);

	/**
	 * Externalises CProtectionDomain data to writable output stream
	 * 
	 * @param aSink RStoreWriteStream& writable output stream
	 */
	void ExternalizeL(RStoreWriteStream& aSink) const;

	/**
	 * Internalises CProtectionDomain data from readble input stream
	 * 
	 * @param aSource RStoreReadStream& readble input stream
	 */
	void InternalizeL(RStoreReadStream& aSource);

private:
	/**
	 * Default private constructor
	 * 
	 */
	inline CProtectionDomain();

	/**
	 * Two phased constructor
	 * 
	 */
	inline void ConstructL();

private:
	/*
	 * Domain name
	 * 
	 * Own.
	 */
	HBufC* iDomain;
	
	/*
	 * Permissionset instance
	 * 
	 * Own.
	 */
	CPermissionSet* iPermSet;
	};

#include "rtsecmgrprotectiondomain.inl"
	
#endif //C_RTSECMGRPROTECTIONDOMAIN_H

