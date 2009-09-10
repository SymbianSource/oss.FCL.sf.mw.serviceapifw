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
* Description:       Models script data structure
 *
*/






#ifndef _CSCRIPT_H_
#define _CSCRIPT_H_

#include <rtsecmgrcommondef.h>
#include <rtsecmgrutility.h>

/**
 * Type holding data to be used while prompting. 
 * 
 * The default or custom (runtime over-ridden) prompt handler requires the permission
 * type to prompt and the user-selection to be returned to security 
 * manager for access permission check
 * 
 * @lib rtsecmgrclient.lib
 */
NONSHARABLE_CLASS(CScript) : public CBase
	{
public:
	
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CScript instance 
	 * 
	 * @param aPolicyID TPolicyID policy identifier of the script
	 * @param aScriptID TExecutableID script identifier
	 * 
	 * @return pointer to an instance of CScript
	 */
	IMPORT_C static CScript* NewL(TPolicyID aPolicyID,TExecutableID aScriptID);

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CScript instance and leaves the created instance
	 * on cleanupstack 
	 * 
	 * @param aPolicyID TPolicyID policy identifier of the script
	 * @param aScriptID TExecutableID script identifier
	 * 
	 * @return pointer to an instance of CScript
	 */
	IMPORT_C static CScript* NewLC(TPolicyID aPolicyID,TExecutableID aScriptID);

	/*
	 * Destructor
	 */
	IMPORT_C ~CScript();
	
	/*
	 * overloaded assignment operator
	 * 
	 * @param aRhs CScript source script object specified on the right
	 * hand side of the assignment operator
	 * 
	 * @return CScript returns the reference of this script object
	 */
	IMPORT_C const CScript& operator=(const CScript& aRhs);

	/*
	 * Gets the policy identifier of the script
	 * 
	 * @return TPolicyID policy identifier of the script
	 */
	IMPORT_C TPolicyID PolicyID() const;
	
	/*
	 * Gets the script identifier
	 * 
	 * @return TExecutableID identifier of the script
	 */
	IMPORT_C TExecutableID ScriptID() const;
	
	/*
	 * Gets the permissionset of the script. The permissionset
	 * contains the list of user-grantable permissions and the list
	 * of unconditional permissions
	 * 
	 * @return CPermissionSet permissionset of the script
	 */
	IMPORT_C const CPermissionSet& PermissionSet() const;

	/*
	 * Gets the permissionset of the script. The permissionset
	 * contains the list of user-grantable permissions and the list
	 * of unconditional permissions. This overloaded version returns
	 * a modifiable reference to permissionset object.
	 * 
	 * @return CPermissionSet modifiable reference to permissionset of the script
	 */
	IMPORT_C CPermissionSet& PermissionSet();
	
	/*
	 * Gets the permanently granted permissions
	 * 
	 * @return TPermGrant permanently granted permission
	 * 
	 */
	IMPORT_C TPermGrant PermGranted() const;
	
	/*
	 * Gets the permanently denied permissions
	 * 
	 * @return TPermGrant permanently denied permission
	 * 
	 */
	IMPORT_C TPermGrant PermDenied() const;

	/*
     * Gets the permanently granted providers
     * 
     * @param aAllowedProviders RProviderArray IN/OUT param which will co
     * 
     */
    IMPORT_C void PermGranted(RProviderArray& aAllowedProviders) ;
    
    /*
     * Gets the permanently denied permissions
     * 
     * @return TPermGrant permanently denied permission
     * 
     */
    IMPORT_C void PermDenied(RProviderArray& aDeniedProviders) ;
    
	/*
	 * Sets the permissionset of the script
	 * 
	 * @param aPermissionSet CPermissionSet source permissionset to be set
	 */
	IMPORT_C void SetPermissionSet(const CPermissionSet& aPermissionSet);

	/**
	 * Externalizes script data to stream
	 * 
	 * @param aOutStream RWriteStream output stream
	 */
	IMPORT_C void ExternalizeL(RWriteStream& aSink) const;
	
	/**
	 * Internalizes script data from stream
	 * 
	 * @param aInStream RReadStream input source stream
	 */
	IMPORT_C void InternalizeL(RReadStream& aSource);

	/*
	 * Sets permanently granted permission
	 * 
	 * @param aPermGrant TPermGrant permanently granted permission to be set
	 * 
	 */
	IMPORT_C void SetPermGranted(TPermGrant aPermGrant);
	
	/*
	 * Sets permanently denied permission
	 * 
	 * @param aPermDenied TPermGrant permanently denied permission to be set
	 * 
	 */
	IMPORT_C void SetPermDenied(TPermGrant aPermDenied);
	
	/*
     * Sets permanently granted permission
     * 
     * @param aPermGrant TPermGrant permanently granted permission to be set
     * 
     */
    IMPORT_C void SetPermGranted(RProviderArray aPermGrantProviders);
    
    /*
     * Sets permanently denied permission
     * 
     * @param aPermDenied TPermGrant permanently denied permission to be set
     * 
     */
    IMPORT_C void SetPermDenied(RProviderArray aPermDeniedProviders);
    
	/*
	 * Sets hash value of the script
	 * 
	 * @param aHashMark TDesC hash value of the script
	 * 
	 */
	inline void SetHashMarkL(const TDesC& aHashMark);
	
	/*
	 * Compares the HashValue of the script with the one passed as argument.
	 * 
	 * @return TBool hash values match or not.
	 * 
	 */
	inline TBool HashMatch(const TDesC& aHashValue = KNullDesC) const;

	/*
	 * returns hash value of the script
	 * 
	 * @return TDesC hash value of the script
	 * 
	 */
	inline TDesC Hash();
	
private:
	/*
	 * default private constructor
	 * 
	 */
	inline CScript():iPermGrant(KDefaultNullBit),iPermDenied(KDefaultNullBit)
		{}

	/*
	 * overloaded constructor
	 * 
	 * @param aPolicyID TPolicyID policy identifier of the script
	 * @param aScriptID TExecutableID script identifier
	 * 
	 */
	inline CScript(TPolicyID aPolicyID,TExecutableID aScriptID):iPolicyID(aPolicyID),iPermGrant(KDefaultNullBit),iPermDenied(KDefaultNullBit),iScriptID(aScriptID)
		{}

	/*
	 * copy constructor
	 * 
	 * @param aRhs source script object
	 */
	CScript(const CScript& aRhs);

	/*
	 * Two-phased constructor
	 * 
	 */
	void ConstructL();
 

private:
	//permission set 
	CPermissionSet* iPermissionSet;
	
	//script identifier
	TExecutableID iScriptID;
	
	//policy identifier of the script
	TPolicyID iPolicyID;
	
	//permanently granted permission	
	TPermGrant iPermGrant;
	
	//permanently denied permission
	TPermGrant iPermDenied; //separate bit-patterns for permanently allowed & denied
	
	//permanently allowed providers
	RProviderArray iAllowedProviders;
	
	//permanently denied providers
	RProviderArray iDeniedProviders;
	
	//hash value of the script
	HBufC* iHashMark;	
	
	// Reserved for future additions
	TAny* iReserved;
	};

/*
 * Sets hash value of the script
 * 
 * @param aHashMark TDesC hash value of the script
 * 
 */
inline void CScript::SetHashMarkL(const TDesC& aHashMark)
	{
	if ( iHashMark)
		{
		delete iHashMark;
		}
	iHashMark = aHashMark.AllocLC ();
	CleanupStack::Pop ();
	}

/*
 * 
 * 
 * @return TBool Etrue
 * 
 */
inline TBool CScript::HashMatch(const TDesC& aHashValue) const
	{
	TBool matches(EFalse);
	if(iHashMark)
		{
		if(0==aHashValue.CompareF(*iHashMark))	
			{
			return !matches;
			}
		}
	return matches;
	}

/*
 *
 * @return TDesC hash value of the script.
 *
 */

inline TDesC CScript::Hash()
	{
	if(iHashMark)
		return *iHashMark;
	else
		return KNullDesC;
	}

#endif //_CSCRIPT_H_

// End of file
