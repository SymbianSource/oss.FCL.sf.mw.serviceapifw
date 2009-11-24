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
* Description:       Wrapper to XML parser interface. Loads the inline and external XML metadata
*				 				 and populates the service provider metadata information. This class
*				 				 uses Symbian SAX parser to load the metadata information.
*
*/







#ifndef LIW_XMLHANDLER_H
#define LIW_XMLHANDLER_H

#include <SenXmlReader.h>
#include <MSenContentHandlerClient.h>
#include <xml/contenthandler.h>
#include <e32capability.h>

// CONSTANTS
const TInt KMaxBufLen = 255;

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CLiwServiceData;
class CLiwList;

/**
* Wrapper class for XML parser. This class is used to parse inline
* and file-based external metadata definitions. SAX (Simple API for XML)
* parser is used to parse the XML content.
*
* @lib ServiceHandler.lib
*
* @see MSenContentHandlerClient
* @see CSenXmlReader
* @see CLiwServiceData
**/
class CLiwXmlHandler : public CBase, public MSenContentHandlerClient
{
public:
	
	/*
	 * Creates and returns an instance of \c CLiwXmlHandler
	 *
	 * @return an instance of \c CLiwXmlHandler
	 */
	static CLiwXmlHandler* NewL();
	
	/*
	 * Creates and returns an instance of \c CLiwXmlHandler
	 * Leaves the created instance in the cleanupstack.
	 *
	 * @return an instance of \c CLiwXmlHandler
	 */
	static CLiwXmlHandler* NewLC();
	
	/*
	 * Destructor
	 */
	virtual ~CLiwXmlHandler();
	
	/**
	* Receive notification of the beginning of a document.
	* @return KErrNone or some of the system-wide Symbian error codes.
	*/		
	virtual TInt StartDocument();

	/**
	* Receive notification of the end of a document.
	* @return KErrNone or some of the system-wide Symbian error codes.
	*/
	virtual TInt EndDocument();
	
	/**
	* Receive notification of the start of an element.
	* @return KErrNone or some of the system-wide Symbian error codes.
	*/
	virtual TInt StartElement(	const TDesC8& /*aURI*/, 
										const TDesC8& /*aLocalName*/, 
										const TDesC8& /*aName*/, 
										const RAttributeArray& /* apAttrs */);

	/**
	* Receive notification of the end of an element.
	* @return KErrNone or some of the system-wide Symbian error codes.
	*/
	virtual TInt EndElement(	const TDesC8& /*aURI*/, 
									const TDesC8& /*aLocalName*/, 
									const TDesC8& /*aName*/);
	/**
	* Receive notification of the character data found in the XML document.
	* @return KErrNone or some of the system-wide Symbian error codes.
	*/
	virtual TInt Characters(const TDesC8& /*aBuf*/, 
	   							   const TInt /*aStart*/, 
								   const TInt /*aLength*/);
	
	/**
	 * Called when there is an error in parsing.
	 * @return KErrNone or some of the system-wide Symbian error codes.
	*/
	inline virtual TInt Error(TInt /*aErrorCode*/);
		
	/**
    * Loads service data from an external file specified 
    * in aFileToParse parameter.
    *
    * @param aFileToParse a valid well defined XML file having metadata definition 
    * @param aServiceData service data to be populated with service provider information. 
    *  					  The caller should pass a properly instantiated pointer to CLiwServiceData instance
    *
    * @return KErrNone if the loading of service data is successful
    		  KSrvDataLoadFailed if the loading of XML file failed
    */				
	TInt LoadServiceData(const TDesC& aFileToParse, CLiwServiceData* aServiceData);
	
	/**
    * Loads service data from a XML data buffer. The passed XML data buffer
    * should be well defined and well formatted valid XML content.
    *
    * @param aXmlBuffer a valid well defined and well formatted XML data content
    * @param aServiceData service data to be populated with service provider information. 
    *  					  The caller should pass a properly instantiated pointer to CLiwServiceData instance
    *
    * @return KErrNone if the loading of service data is successful
    		  KSrvDataLoadFailed if the loading of XML buffer content failed
    */
	TInt LoadServiceData(const TDesC8& aXmlBuffer, CLiwServiceData* aServiceData);
	
	/*
     * Enumeration IDs corresponding to XML parsing status
     */ 
	enum TLiwXmlParseStatus
	{
		ESrvDataLoadSuccess=0, 
		ESrvDataLoadFailed,	//General failures other than below
		ESrvDataFileNotFnd, 
		ESrvDataInvalidFile, //Parse error
		ESrvDataCapNotSpecified	
	};
												
private:

	//private default constructor
	CLiwXmlHandler();
	
	//second phase constructor
	void ConstructL();
	
	//the SAX parser instance
	CSenXmlReader* iXmlHandler;	
	
	//the service data instance to be populated with metadata information
	CLiwServiceData* iServiceData;
	
	HBufC8* iBuffer;
	TBuf8<KMaxBufLen> startBuf;
};

#endif 

// End of File


