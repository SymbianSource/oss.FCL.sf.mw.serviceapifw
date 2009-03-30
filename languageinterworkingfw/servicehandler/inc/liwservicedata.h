/*
* Copyright (c) 2003-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:       Declares Service Data and Service Provider Metadata classes
*
*/







#ifndef LIW_SERVICEDATA_H
#define LIW_SERVICEDATA_H

// INCLUDES
#include <e32base.h>

// CONSTANTS

// MACROS

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CLiwServiceData;
class CLiwXmlHandler;
class CLiwServiceHandlerImpl;
class CDesC8Array;

class CLiwMap;
class CLiwGenericParamList;
class TLiwVariant;

/**
* CLiwServiceData models the service provider data/information.
* A Service provider can obtain its metadata through the interfaces
* of this class. The Service metadata is a name-value pair defined 
* by the provider as part of its registration information (Refer \c REGISTRY_INFO).
*
* One of the key metadata value is the capability set which are expected 
* from the service consumer by the provider. The required capability set
* is specifed in the provider registration information (Refer \c REGISTRY_INFO).
*
* ServiceHandler queries for those capabilities using \c CLiwServiceData::GetCapabilitiesL
*
* @lib ServiceHandler.lib
*
* @see CLiwServiceHandler
* @see CLiwMetaData
* @see TCapability
*/
class CLiwServiceData : public CBase
{
public:

	
	/**
    * Returns the service metadata.
    *
    * @return the service metadata
    */
    CLiwGenericParamList* GetMetaData() const;
	
	// Destructor
	virtual ~CLiwServiceData();
													
private:
	
	/*
	 * Creates and returns an instance of \c CLiwServiceData
	 * This is private and not available for external clients.
	 *
	 * @return the instance of created instance of \c CLiwServiceData
	 */
	static CLiwServiceData* NewL();
	
	/*
	 * Creates and returns an instance of \c CLiwServiceData
	 * The created instance is left in the cleanupstack.
	 * This is private and not available for external clients.
	 *
	 * @return the instance of created instance of \c CLiwServiceData
	 */
	static CLiwServiceData* NewLC();
	
	/*
	 * Default private constructor.
	 * Creates the metadata associated with the service data
	 */
	CLiwServiceData();
	
	/*
	 * Second phase constructor
	 */
	void ConstructL();

	/*
	 * Adds a metadata name-value pair. If the key already exists,
	 * then the value will be added to the list of values associated
	 * with the metadata key 
	 *
	 * @param aKey the metadata key to be inserted
	 * @param aValue the value associated with the metadata key to be inserted
	 */
	void AddMetaDataL(const TDesC8& aKey, const TDesC8& aValue);
	void AddMetaDataL(const TDesC8& aKey, const TLiwVariant& aValue);
	
	/*
	 * Removes all the metadata key-value pairs.
	 * This is called from service handler implementation if and only
	 * if there is an error in parsing. Since, the FW uses SAX parser
	 * and if the XML error happens after the metadata entries are 
	 * created, the entries should be cleaned up.
	 */
	void CleanUpMetaData();
	
	
	
	CLiwGenericParamList* iDataList;
	
	friend class CLiwXmlHandler;
	friend class CLiwServiceHandlerImpl;
};

#endif 

// End of File


