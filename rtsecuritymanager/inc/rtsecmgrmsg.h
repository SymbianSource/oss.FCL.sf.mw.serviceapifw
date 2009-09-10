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

NONSHARABLE_CLASS(CRTPermGrantMessage) : public CBase
    {
    public:
        /**
         * Destructor.
         */
        IMPORT_C virtual ~CRTPermGrantMessage();
        
        /**
         * Two-phased constructor.
         * 
         * Creates an instance of CRTPermGrantMessage.
         */
        IMPORT_C static CRTPermGrantMessage* NewL();
        
        /**
         * Two-phased constructor. 
         * 
         * Creates an instance of CRTPermGrantMessage
         * and leaves it on cleanupstack
         */
        IMPORT_C static CRTPermGrantMessage* NewLC();
                
        /**
         * Two-phased constructor. 
         * 
         * Creates an instance of CRTPermGrantMessage
         * from the input source
         * 
         * @param aBuf input source data
         * 
         * @return CRTPermGrantMessage* created instance of CRTPermGrantMessage 
         * 
         */
        IMPORT_C static CRTPermGrantMessage* NewL(const TDesC8& aBuf);
        
        /**
         * Two-phased constructor. 
         * 
         * Creates an instance of CRTPermGrantMessage
         * from the input source and leaves it on cleanup stack
         * 
         * @param aBuf input source data
         * 
         * @return CRTPermGrantMessage* created instance of CRTPermGrantMessage 
         * 
         */
        IMPORT_C static CRTPermGrantMessage* NewLC(const TDesC8& aBuf);
        
        /**
         * Two-phased constructor. 
         * 
         * Creates an instance of CRTPermGrantMessage
         * 
         * @param aAllowedProviders RProviderArray Allowed service providers
         * @param aDeniedProviders RProviderArray Denied service providers
         * @param aScriptId TExecutableID script identifier
         * @return CRTPermGrantMessage* created instance of CRTPermGrantMessage 
         * 
         */
        IMPORT_C static CRTPermGrantMessage* NewL(RProviderArray aAllowedProviders, RProviderArray aDeniedProviders,TExecutableID aScriptId);
        
        /**
         * Two-phased constructor. 
         * 
         * Creates an instance of CRTPermGrantMessage and leaves it on the cleanup stack
         * 
         * @param aAllowedProviders RProviderArray Allowed service providers
         * @param aDeniedProviders RProviderArray Denied service providers
         * @param aScriptId TExecutableID script identifier
         * @return CRTPermGrantMessage* created instance of CRTPermGrantMessage 
         * 
         */
        IMPORT_C static CRTPermGrantMessage* NewLC(RProviderArray aAllowedProviders, RProviderArray aDeniedProviders,TExecutableID aScriptId);
        
        /**
         * Get Method.
         * 
         * Gets the list of Allowed providers
         * 
         * @param aAllowedProviders RProviderArray In/Out parameter which will contain the list of allowed providers
         */
        IMPORT_C void AllowedProviders(RProviderArray& aAllowedProviders);
        
        /**
         * Get Method.
         * 
         * Gets the list of Allowed providers
         * 
         * @param aDeniedProviders RProviderArray In/Out parameter which will contain the list of denied providers
         */
        IMPORT_C void DeniedProviders(RProviderArray& aDeniedProviders);
        
        /**
         * Get Method
         * 
         * Gets the script Identifier.
         * 
         * @return TExecutableID the script identifier
         */
        IMPORT_C TExecutableID ScriptID();
        
        /**
         * Set Method.
         * 
         * Sets the list of Allowed providers
         * 
         * @param aAllowedProviders RProviderArray input parameter which contains the list of allowed providers
         */
        IMPORT_C void setAllowedProviders(RProviderArray aAllowedProviders);
        
        /**
         * Set Method.
         * 
         * Sets the list of Denied providers
         * 
         * @param aDeniedProviders RProviderArray input parameter which contains the list of denied providers
         */
        IMPORT_C void setDeniedProviders(RProviderArray aDeniedProviders);
        
        /**
         * Set Method
         * 
         * Sets the script Identifier.
         * 
         * @param TExecutableID the script identifier
         */
        IMPORT_C void setScriptID(TExecutableID aScriptId);
        
        /*
         * Creates an HBufC8 representation of CRTPermGrantMessage
         * 
         * @return HBufC8* buffer representation of CRTPermGrantMessage
         */
        IMPORT_C HBufC8* PackMessageL();
        
    private:
        CRTPermGrantMessage();
        CRTPermGrantMessage(RProviderArray aAllowedProviders,RProviderArray aDeniedProviders,TExecutableID aScriptId);
        void ConstructL(const TDesC8& aBuf);
        void InternalizeL(RReadStream& aSink);
        void ExternalizeL(RWriteStream& aSource);
    private:
        RProviderArray iAllowedProviders;
        RProviderArray iDeniedProviders;
        TExecutableID iScriptId;
    };

#endif // RTSECMGRMSG_H

