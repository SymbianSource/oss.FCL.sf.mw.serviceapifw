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
* Description:      
*
*/







#ifndef _RTSECMGRMSG_H
#define _RTSECMGRMSG_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <s32strm.h>
#include <s32mem.h>
#include <rtsecmgrcommondef.h>

// Maximum size expected for iHashMarker in CRTSecMgrRegisterScriptMsg
const TInt KMaxHashValueDesLen = 255;

// Maximum total size expected for a CRTSecMgrRegisterScriptMsg object
const TInt KMaxMsgLength = 520;

// CLASS DECLARATION

/**
 *  Message type to pass script object across
 *  client server boundary
 * 
 */
NONSHARABLE_CLASS(CRTSecMgrRegisterScriptMsg) : public CBase
	{
public:

	/**
	 * Destructor.
	 */
	IMPORT_C ~CRTSecMgrRegisterScriptMsg();

	/**
	 * Two-phased constructor.
	 * 
	 * Creates an instance of CRTSecMgrRegisterScriptMsg.
	 * 
	 * 
	 * @param aPolicyID policy identifier of script
	 * @param aHashValue hash value of script
	 * 
	 * @return CRTSecMgrRegisterScriptMsg* created instance of CRTSecMgrRegisterScriptMsg
	 * 
	 */
	IMPORT_C static CRTSecMgrRegisterScriptMsg* NewL(TPolicyID aPolicyID, const TDesC& aHashValue);

	/**
	 * Two-phased constructor. 
	 * 
	 * Creates an instance of CRTSecMgrRegisterScriptMsg
	 * and leaves it on cleanupstack
	 * 
	 * @param aPolicyID policy identifier of script
	 * @param aHashValue hash value of script
	 * 
	 * @return CRTSecMgrRegisterScriptMsg* created instance of CRTSecMgrRegisterScriptMsg
	 * 
	 */
	IMPORT_C static CRTSecMgrRegisterScriptMsg* NewLC(TPolicyID aPolicyID, const TDesC& aHashValue);

	/**
	 * Two-phased constructor. 
	 * 
	 * Creates an instance of CRTSecMgrRegisterScriptMsg
	 * from the input source
	 * 
	 * @param aStreamData input source data
	 * 
	 * @return CRTSecMgrRegisterScriptMsg* created instance of CRTSecMgrRegisterScriptMsg 
	 * 
	 */
	IMPORT_C static CRTSecMgrRegisterScriptMsg* NewLC(const TDesC8& aStreamData);

	/*
	 * Creates an HBufC8 representation of CRTSecMgrRegisterScriptMsg
	 * 
	 * @return HBufC8* buffer representation of CRTSecMgrRegisterScriptMsg
	 */
	IMPORT_C HBufC8* PackMsgL() const;

	/*
	 * Gets the hash value of the script
	 * 
	 * @return const TDesC8& hash value of script
	 */
	inline const TDesC& HashValue() const
		{
		if(iHashMarker)
		return *iHashMarker;
		else
		return KNullDesC;
		}

	/*
	 * Gets the policy identifier
	 * 
	 * @return TPolicyID policy identifier
	 */
	inline TPolicyID PolicyID() const
		{
		return iPolicyID;
		}

protected:
	// Writes ’this’ to the stream
	void ExternalizeL(RWriteStream& aStream) const;
	// Initializes ’this’ from stream
	void InternalizeL(RReadStream& aStream);

private:

	/**
	 * Constructor for performing 1st stage construction
	 */
	CRTSecMgrRegisterScriptMsg()
		{}

	/**
	 * Constructor for performing 1st stage construction
	 */
	CRTSecMgrRegisterScriptMsg(TPolicyID aPolicyID);

	/**
	 * Second stage construction
	 */
	void ConstructL(const TDesC& aHashValue);

private:
	/*
	 * Policy identifier
	 * 
	 */
	TPolicyID iPolicyID;

	/*
	 * Hash value of the script
	 * 
	 */
	HBufC* iHashMarker;
	};

#endif // RTSECMGRMSG_H

