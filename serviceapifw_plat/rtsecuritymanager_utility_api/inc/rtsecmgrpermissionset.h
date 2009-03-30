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
* Description:       Defines permission set class
 *
*/







#ifndef _CPERMISSIONSET_H_
#define _CPERMISSIONSET_H_

#include <e32debug.h>
#include <s32strm.h>
#include <s32mem.h>
#include <rtsecmgrcommondef.h>
#include <rtsecmgrpermission.h>

//Forward declaration
class CPermissionSet;

typedef RPointerArray<CPermission> RPermissions;
typedef TUint32 TUnConditionalPermission;
typedef RArray<TCapability> RUnConditionals;
typedef RPointerArray<CPermissionSet> RPermissionSet;

/**
 * CPermissionSet represents list of user grantable permissions
 * and list of unconditional permissions.
 * 
 * CPermissionSet provides accessor and modifier methods to access
 * and modify CPermissionSet object state
 * 
 * @lib rtsecmgrutil.lib
 */
NONSHARABLE_CLASS(CPermissionSet) : public CBase
	{
public:
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPermissionSet instance
	 *
	 * @return pointer to an instance of CPermissionSet
	 */
	IMPORT_C static CPermissionSet* NewL();
	
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPermissionSet instance and leaves the created instance
	 * on the cleanupstack
	 *
	 * @return pointer to an instance of CPermissionSet
	 */
	IMPORT_C static CPermissionSet* NewLC();
	
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPermissionSet instance from another permissionset
	 * object
	 *
	 * @param aPermSet CPermissionSet source permissionset object
	 * 
	 * @return pointer to an instance of CPermissionSet
	 */
	IMPORT_C static CPermissionSet* NewL(const CPermissionSet& aPermSet);
	
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPermissionSet instance from another permissionset
	 * object and leaves the created instance on the cleanupstack
	 *
	 * @param aPermSet CPermissionSet source permissionset object
	 * 
	 * @return pointer to an instance of CPermissionSet
	 */
	IMPORT_C static CPermissionSet* NewLC(const CPermissionSet& aPermSet);
	
	/**
	 * Destructor
	 * 
	 */
	IMPORT_C ~CPermissionSet();
	
	/**
	 * Externalizes permissionset data to stream
	 * 
	 * @param aOutStream RWriteStream output stream
	 */
	IMPORT_C void ExternalizeL(RWriteStream& aSink) const;
	
	/**
	 * Internalizes permissionset data from stream
	 * 
	 * @param aInStream RReadStream input source stream
	 */
	IMPORT_C void InternalizeL(RReadStream& aSource);

	/*
	 * Adds an unconditional capability to the permissionset
	 * 
	 * @param aCapability TCapability unconditional capability to be added
	 */
	IMPORT_C void AppendUncondCap(TCapability aCapability);
	
	/*
	 * Adds a list of unconditional capabilities to the permissionset
	 * 
	 * @param TUnConditionalPermission aUnConditionals - list of TCapability that are to be added
	 */
	IMPORT_C void AppendUncondCapabilities(TUnConditionalPermission aUnConditionals);
	
	/*
	 * Adds a permission to the permissionset
	 * 
	 * @param aPermission CPermission permission data to be added
	 * 
	 */
	IMPORT_C void AppendPermission(CPermission& aPermission);

	/*
	 * Gets the list of user-grantable permissions
	 * 
	 * @return RPermissions non-modifiable reference to the list of user-grantable permissions
	 * 
	 */
	IMPORT_C const RPermissions& Permissions() const;
	
	/*
	 * Gets the list of unconditional permissions
	 * 
	 * @return TUnConditionalPermission unconditional permission
	 *  
	 */
	IMPORT_C TUnConditionalPermission UnconditionalCaps() const;
	
	/*
	 * Gets the list of unconditional permissions as array of
	 * TCapability
	 * 
	 * @return RUnConditionals list of unconditional capabilities
	 * 
	 * @see TCapability
	 * 
	 */
	IMPORT_C void UnconditionalCaps(RUnConditionals& aUnConditionals) const;
	
	/*
	 * Gets list of user-grantable permissions. This overloaded version
	 * returns modifiable list of user-grantable permissions
	 * 
	 * @return RPermissions list of user-grantable permissions
	 *  
	 */
	IMPORT_C RPermissions& Permissions();

private:
	/*
	 * Default private constructor
	 */
	inline CPermissionSet()
		{}
	
	/*
	 * private copy constructor
	 */
	CPermissionSet(const CPermissionSet& aPermSet);
	
	/* 
	 * Private second phase constructor
	 */
	 void ConstructL(const CPermissionSet& aPermSet);
	 
private:
	
	//unconditional permissions
	TUnConditionalPermission iUncondCapSet;
	
	//user-grantable permissions
	RPermissions iPermissions;
	};
#endif //_CPERMISSIONSET_H_

