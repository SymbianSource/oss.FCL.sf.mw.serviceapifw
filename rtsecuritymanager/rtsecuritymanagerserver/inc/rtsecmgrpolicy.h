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






#ifndef C_RTSECMGRPOLICY_H
#define C_RTSECMGRPOLICY_H

#include <e32capability.h>
#include <rtsecmgrutility.h>
#include <rtsecmgrscript.h>
#include "rtsecmgrdef.h"
#include "rtsecmgrprotectiondomain.h"

class CProtectionDomain;

typedef RPointerArray<CProtectionDomain> RProtectionDomains;
typedef RPointerArray<CPermission> RAliasGroup;

/*
 * Represents a security and trust policy mapping. This class maps
 * the policyID and its associated security and trust policies.
 *
 * A CPolicy instance has list of protection domains specified
 * in the security access policy and list of trust information. The 
 * security access policy and trust policy are associated with a policy
 * identifier
 * 
 * @see CProtectionDomain
 * @see CPermission
 * 
 * @exe rtsecmgrserver.exe 
 */
NONSHARABLE_CLASS(CPolicy) : public CBase
	{
public:

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPolicy instance from an input serialization
	 * source
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * @param aSource 	RStoreReadStream& input source
	 * 
	 * @return CPolicy * pointer to an instance of CPolicy
	 */
	static CPolicy* NewL(TPolicyID aPolicyID, RStoreReadStream& aSource);
	
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPolicy instance from an input serialization
	 * source and leaves the created instance on the cleanupstack
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * @param aSource 	RStoreReadStream& input source
	 * 
	 * @return CPolicy * pointer to an instance of CPolicy
	 */
	static CPolicy* NewLC(TPolicyID aPolicyID, RStoreReadStream& aSource);

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPolicy instance and initializes the policy identifier
	 * and the security access policy domain data
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * @param aProtectionDomains RProtectionDomains& input security protection domains
	 * @param aAliasGroup RAliasGroup& input capability grouping
	 * 
	 * @return CPolicy * pointer to an instance of CPolicy
	 */
	static CPolicy* NewL(TPolicyID aPolicyID, const RProtectionDomains& aProtectionDomains, RAliasGroup& aAliasGroup);

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPolicy instance and initializes the policy identifier
	 * and the security access policy domain data. This method leaves the
	 * created instance on the cleanupstack.
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * @param aProtectionDomains RProtectionDomains& input security protection domains
	 * @param aAliasGroup RAliasGroup& input capability grouping
	 * 
	 * @return CPolicy * pointer to an instance of CPolicy
	 */
	static CPolicy* NewLC(TPolicyID aPolicyID, const RProtectionDomains& aProtectionDomains, RAliasGroup& aAliasGroup);

	/**
	 * Internalizes CPolicy data members from the input readable source
	 * 
	 * @param aSource RStoreReadStream& Input readable source
	 * 
	 */
	void InternalizeL(RStoreReadStream& aSource);

	/**
	 * Gets a protection domain instance when a valid domain 
	 * name is passed.
	 * 
	 * @param aDomainName const TDesC& name of the protection domain
	 * 
	 * @return CProtectionDomain* valid protection domain instance; NULL if
	 * domain name is invalid or does not exist
	 * 
	 */
	CProtectionDomain* ProtectionDomain(const TDesC& aDomainName) const;		

	/**
	 * Gets policy identifier of the security and trust policy
	 * data
	 * 
	 * @return TPolicyID policy identifier
	 * 
	 */
	inline TPolicyID PolicyID() const;
	
	/**
	 * Gets the list of protection domains associated with
	 * the policy identifier
	 *
	 * @return const RProtectionDomains& protection domain list
	 * 
	 */
	inline const RProtectionDomains& ProtectionDomain() const;

	/**
	 * virtual destructor
	 *  
	 */
	inline ~CPolicy();
	
	/**
	 * Gets a alias group instance when a valid alias group
	 * name is passed.
	 * 
	 * @param aAliasName const TDesC& name of the alias group
	 * 
	 * @return CPermission* valid alias group instance; NULL if
	 * alias group name is invalid or does not exist
	 * 
	 */
	CPermission* AliasGroup(const TDesC& aAliasName) const;		

	/**
	 * Gets the list of alias groups associated with
	 * the policy identifier
	 *
	 * @return const RAliasGroup& alias group list
	 * 
	 */
	inline const RAliasGroup& AliasGroup() const;

private:
	/*
	 * private default constructor
	 */
	CPolicy();
	
	/*
	 * Overloaded constructor to initialise policy identifier
	 * 
	 */
	CPolicy(TPolicyID aPolicyID);
	
	/*
	 * Two-phase constructor
	 * 
	 * Initializes policy identifier, security protection domains and alias groupings
	 * 
	 */
	void ConstructL(const RProtectionDomains& aAccessPolicy, RAliasGroup& aAliasGroup);

private:	
	/*
	 * Policy identifier
	 */
	TPolicyID iPolicyID;
	
	/*
	 * Array of protection domains
	 * 
	 * Own.
	 */
	RProtectionDomains iAccessPolicy;
	
	/*
	 * Array of alias groupings
	 * 
	 * Own.
	 */
	RAliasGroup iAliasGroup;
	};

#include "rtsecmgrpolicy.inl"
	
#endif //C_RTSECMGRPOLICY_H
	
