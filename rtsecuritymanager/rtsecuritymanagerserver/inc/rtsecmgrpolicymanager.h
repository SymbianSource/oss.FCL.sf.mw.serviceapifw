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
* Description:       Definition of policy manager class
 *
*/






#ifndef C_RTSECMGRPOLICYMANAGER_H
#define C_RTSECMGRPOLICYMANAGER_H

#include <e32capability.h>
#include <rtsecmgrutility.h>
#include <rtsecmgrscript.h>
#include "rtsecmgrdef.h"
#include "rtsecmgrpolicy.h"
#include "rtsecmgrdata.h"
#include "rtsecmgrstore.h"

/*
 * Manages set of security and trust policies. 
 * 
 * CPolicyManager provides management functionalities to
 *  - register a policy
 *  - un-register a policy
 *  - restore policies from the persistent storage
 *
 * CPolicyManager interacts with CSecMgrStore which abstracts the
 * underlying persistent storage from policy users
 * 
 * @see CSecMgrStore
 * @see CPolicy
 * 
 * @exe rtsecmgrserver.exe
 */
NONSHARABLE_CLASS(CPolicyManager) : public CBase
	{
public:

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPolicyManager instance
	 * 
	 * @param aSecMgrDB CSecMgrStore* pointer to security manager storage type
	 * 
	 * @return CPolicyManager* pointer to an instance of CPolicyManager
	 */
	static CPolicyManager* NewL(CSecMgrStore* aSecMgrDB);

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPolicyManager instance and leaves the created
	 * instance on the cleanupstack
	 * 
	 * @param aSecMgrDB CSecMgrStore* pointer to security manager storage type
	 * 
	 * @return CPolicyManager* pointer to an instance of CPolicyManager
	 */
	static CPolicyManager* NewLC(CSecMgrStore* aSecMgrDB);

	/**
	 * Destructor
	 * 
	 * Cleanups the internal cache for policy data
	 */
	inline ~CPolicyManager();

	/**
	 * Registers a new policy data. 
	 * 
	 * This method in turn updates the security manager
	 * persistent store with new policy data
	 * 
	 * @param aPolicy const CPolicy& Reference to policy data
	 * 
	 */
	void RegisterPolicyL(const CPolicy& aPolicy);

	/**
	 * Un-registers an existing policy data 
	 * 
	 * This method in turn removes the policy data from the 
	 * security manager persistent store
	 * 
	 * @param aPolicyID TPolicyID Policy identifier
	 * 
	 * @return KErrNone if unregistration is successful; Otherwise one of
	 * system wide error codes
	 */
	TInt UnRegisterPolicy(TPolicyID aPolicyID);

	/**
	 * Restores policy data from security manager persistent store.
	 * 
	 * The internal cache of policy data maintained by this class
	 * gets updated
	 * 
	 */
	inline void RestorePoliciesL();

	/**
	 * Gets the policy data associated with the policy
	 * identifier
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * 
	 * @return CPolicy * pointer to an instance of CPolicy associated
	 * with policy identifier; Returns NULL if policy identifier is not valid
	 */
	CPolicy* Policy(TPolicyID aPolicyID) const;

	/**
	 * Checks if a policy data is stored with a policy identifier
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * 
	 * @return TBool ETrue if policy associated with policy identifier
	 * exists; Otherwise EFalse
	 * 
	 */
	TBool HasPolicy(TPolicyID aPolicyID) const;

	/**
	 * Gets the number of registered policy data
	 * 
	 * @return TInt number of registered policy data
	 */
	inline TInt Count() const;
private:

	/**
	 * Overloaded constructor
	 * 
	 */
	inline CPolicyManager(CSecMgrStore* aSecMgrDB);
private:

	//Array of policy data
	RPolicies iPolicies;

	//Reference to security manager persistent storage
	CSecMgrStore* iSecMgrDB;
	};

#include "rtsecmgrpolicymanager.inl"

#endif //C_RTSECMGRPOLICYMANAGER_H

