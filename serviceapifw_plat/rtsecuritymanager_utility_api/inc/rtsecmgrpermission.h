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
* Description:       Defines permission data structure
 *
*/







#ifndef _C_PERMISSION_H_
#define _C_PERMISSION_H_

#include <e32debug.h>
#include <s32strm.h>
#include <s32mem.h>
#include <rtsecmgrcommondef.h>

typedef TUint32 TPermissionData;
const TPermissionData LOWBIT = 0x0001;

/**
 * Represents permission data structure.
 * 
 * A permission data is modelled as 32 bit unsigned integer type
 * with the following layout :
 * 
 * - With leftmost 20 bits representing one of symbian native capability value
 * - Bits 25 to 28 represent various user prompt conditions like oneshot, 
 * 	 session or blanket
 * - Bits 29-32 represent default condition value
 * - Bits 21-24 are reserved
 * 
 * Various accessor and modifier methods are provided to access underlying
 * native capability value, the default condition and the conditions set
 * for this permission data.
 * 
 * @lib rtsecmgrutil.lib
 */
NONSHARABLE_CLASS(CPermission) : public CBase
	{
public:
	/**
	 * Default constructor
	 * 
	 */
	IMPORT_C static CPermission* NewL();
	
	/**
	 * destructor
	 * 
	 */
	IMPORT_C ~CPermission();
	
	/**
	 * Default constructor
	 * 
	 */
	IMPORT_C static CPermission* NewLC();
	
	
	/**
	 * Copy constructor
	 * 
	 * @param aPermission TPermission permission source from which copy is made
	 */
	IMPORT_C static CPermission* NewL(const CPermission& aPermission);
	
	/**
	 * Copy constructor
	 * 
	 * @param aPermission TPermission permission source from which copy is made
	 */
	IMPORT_C static CPermission* NewLC(const CPermission& aPermission);
	
	/**
	 * Overloaded constructor taking capability
	 * 
	 * @param aCapability TCapability capability to be set
	 */
	IMPORT_C static CPermission* NewL(const RCapabilityArray& aCapabilities);
	
	/**
	 * Overloaded constructor taking capability
	 * 
	 * @param aCapability TCapability capability to be set
	 */
	IMPORT_C static CPermission* NewLC(const RCapabilityArray& aCapabilities);
	
	/**
	 * Overloaded constructor taking capability and default condition
	 * 
	 * @param aCapability TCapability capability to be set
	 * @param aDefaultOption TUserPromptOption Default user prompt option to be set
	 */
	IMPORT_C static CPermission* NewL(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt);
	
	/**
	 * Overloaded constructor taking capability and default condition
	 * 
	 * @param aCapability TCapability capability to be set
	 * @param aDefaultOption TUserPromptOption Default user prompt option to be set
	 */
	IMPORT_C static CPermission* NewLC(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt);
	
	/**
	 * Overloaded constructor taking capability, default condition and conditions of
	 * permission
	 * 
	 * @param aCapability TCapability capability to be set
	 * @param aDefaultOption TUserPromptOption Default user prompt option to be set
	 * @param aCondition TUserPromptOption Conditions to be set
	 */
	IMPORT_C static CPermission* NewL(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt,
			TUserPromptOption aCondition);

	/**
	 * Overloaded constructor taking capability, default condition and conditions of
	 * permission
	 * 
	 * @param aCapability TCapability capability to be set
	 * @param aDefaultOption TUserPromptOption Default user prompt option to be set
	 * @param aCondition TUserPromptOption Conditions to be set
	 */
	IMPORT_C static CPermission* NewLC(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt,
			TUserPromptOption aCondition);

	/**
	 * Returns default prompt option
	 * 
	 * @return TUserPromptOption default condition
	 */
	IMPORT_C TUserPromptOption Default() const;
	
	/**
	 * Returns condition
	 * 
	 * @return TUserPromptOption condition
	 */
	IMPORT_C TUserPromptOption Condition() const;
	
	/**
	 * Returns capability value
	 * 
	 * @return TCapability capability value
	 */
	IMPORT_C TCapability Capability() const;

	/**
	 * Returns capability of the permission
	 * 
	 * @param aCapabilities RCapabilityArray& contains the capabilities
	 */
	IMPORT_C void Capabilitilites(RCapabilityArray& aCapabilities) const;
	
	/**
	 * Sets default condition value
	 * 
	 * @param aDefaultOption TUserPromptOption default condition value
	 */
	IMPORT_C void SetDefault(TUserPromptOption aDefaultOption);
	
	/**
	 * Sets condition value
	 * 
	 * @param aCondition TUserPromptOption condition value
	 */
	IMPORT_C void SetCondition(TUserPromptOption aCondition);

	/**
	 * Sets the name of the permission
	 *
	 * @param aName const TDesC& value
	 */
	IMPORT_C void SetPermName(const TDesC& aName);
	
	/**
	 * Returns Name of the permission
	 *
	 * @return const TDesC&
	 */
	IMPORT_C const TDesC& PermName() const; 
	
	/**
	 * Returns iPermissionData of the permission
	 *
	 * @return TPermission iPermissionData value
	 */
	IMPORT_C TPermissionData PermissionData() const; 
	
	// ---------------------------------------------------------------------------
	// Adds a capability to permission data
	// ---------------------------------------------------------------------------
	//
	IMPORT_C void AppendCapPermData(TCapability aCap);
	
	// ---------------------------------------------------------------------------
	// Adds an unconditional capability
	// ---------------------------------------------------------------------------
	//
	IMPORT_C void SetPermissionData(TPermissionData aPermData);

	/**
	 * Internalizes permission data from stream
	 * 
	 * @param aInStream RReadStream input source stream
	 */
	IMPORT_C void InternalizeL(RReadStream& aInStream);
	
	/**
	 * Externalizes permission data to stream
	 * 
	 * @param aOutStream RWriteStream output stream
	 */
	IMPORT_C void ExternalizeL(RWriteStream& aOutStream) const;
 
private:
	/*
	 * Default private constructor
	 */
	inline CPermission() : iPermName(NULL), iPermissionData(0)
		{}
	
	/*
	 * private copy constructor
	 */
	CPermission(const CPermission& aPermission);
	

	CPermission(const RCapabilityArray& aCapabilities);

	CPermission(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt);

	CPermission(const RCapabilityArray& aCapabilities, TUserPromptOption aDefOpt,TUserPromptOption aCondition);

private:
	//layout of TPermissionData
	//__________________________________________________________________________
	//|                 |              |               |       		            |
	//|   Default       | Condition	   | RESERVED      |   CAPABILITY BITS      |
	//|_________________|______________|_______________|________________________|
	//
	//  32,31,30,29,      28,27,26,25,    24,23,22,21,    20,19,18,...4,3,2,1
	//
	//underlying permission data structure
	
		
	TPermissionData iPermissionData;
	
		
	/*
	 * alias group name
	 */
	HBufC* iPermName;
	
	};

#endif //_T_PERMISSION_H_

