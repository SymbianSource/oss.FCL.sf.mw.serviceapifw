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
*				 and populates the service provider metadata information. This class
*				 uses Symbian SAX parser to load the metadata information.
*
*/







#include "LiwXmlHandler.h"
#include <f32file.h>
#include "data_caging_path_literals.hrh"
#include <e32capability.h>

#include "LiwServiceData.h"
#include <LiwVariant.h>

_LIT8(KMetaDataName, "meta");


/*
 * Creates and returns an instance of \c CLiwXmlHandler
 */
CLiwXmlHandler* CLiwXmlHandler::NewL()
{
	CLiwXmlHandler* ptrToThis = CLiwXmlHandler::NewLC();
	CleanupStack::Pop(ptrToThis);
	
	return ptrToThis;
}

/*
 * Creates and returns an instance of \c CLiwXmlHandler
 * Leaves the created instance in the cleanupstack.
 *
 * @return an instance of \c CLiwXmlHandler
 */
CLiwXmlHandler* CLiwXmlHandler::NewLC()
{
	CLiwXmlHandler* ptrToThis = new (ELeave) CLiwXmlHandler();
	CleanupStack::PushL(ptrToThis);
	
	ptrToThis->ConstructL();
	
	return ptrToThis;
}


/*
 * Destructor. Deletes the handler to XML parser
 */	
CLiwXmlHandler::~CLiwXmlHandler()
{
	if(iXmlHandler)
		delete iXmlHandler;
	
	if(iBuffer)
		delete iBuffer;
}

/*
 * Default constructor.
 */	
CLiwXmlHandler::CLiwXmlHandler()
{
	
}

/*
 * Instantiates the SAX parser instance and registers itself
 * as the callback to handle SAX events.
 */
void CLiwXmlHandler::ConstructL()
{
	TInt parserFeature = EErrorOnUnrecognisedTags | EXmlVersion_1_1;
	iXmlHandler = CSenXmlReader::NewL(parserFeature);
	iXmlHandler->SetContentHandler(*this);
}

/*
 * Parses the inline XML content passed in the buffer. The parser
 * is kick started to recevie SAX events. This method also accepts the
 * reference to \c CLiwServiceData to store the parsed metadata name-value
 * pairs.
 */
TInt CLiwXmlHandler::LoadServiceData(const TDesC8& aXmlBuffer,CLiwServiceData* aServiceData)
{
	iServiceData = aServiceData;
	
	TRAPD(err,iXmlHandler->ParseL(aXmlBuffer));
	
	TInt retVal = ESrvDataLoadFailed;
		
	if(!err)
	{
		retVal = ESrvDataLoadSuccess;
	}
	
	return retVal;
}

/*
 * Parses the XML content present in an external file. The parser
 * is kick started to recevie SAX events. This method also accepts the
 * reference to \c CLiwServiceData to store the parsed metadata name-value
 * pairs.
 */
 #include <bautils.h>
TInt CLiwXmlHandler::LoadServiceData(const TDesC& aFileToParse,CLiwServiceData* aServiceData)
{
	TInt retVal = ESrvDataLoadFailed;
	
	iServiceData = aServiceData;
	
	RFs fsSession;
    User::LeaveIfError( fsSession.Connect() );
    
    TFileName resFile;
    resFile.Append(aFileToParse);
    
    if(!BaflUtils::FileExists(fsSession,resFile))
    {
    	fsSession.Close();
    	return ESrvDataFileNotFnd;
    }
    	
    
	TRAPD(err,iXmlHandler->ParseL(fsSession,resFile));
	
	fsSession.Close();
	if(!err)
	{
		retVal = ESrvDataLoadSuccess;
	}
	
	return retVal;	
}

/**
* Receive notification of the beginning of a document.
* @return KErrNone or some of the system-wide Symbian error codes.
*/		
TInt CLiwXmlHandler::StartDocument()
{
	return KErrNone;
}

/**
* Receive notification of the end of a document.
* @return KErrNone or some of the system-wide Symbian error codes.
*/
TInt CLiwXmlHandler::EndDocument()
{
	return KErrNone;
}

/**
* The important parsed elements are <metadata/> and its sub-element
* <capability/>.
* The other sub-elements (other than <capability/>) are considered
* as service provider specific metadata name-value pairs.
*
* @return KErrNone or some of the system-wide Symbian error codes.
*/
TInt CLiwXmlHandler::StartElement(	const TDesC8& /*aURI*/, 
									const TDesC8& aLocalName, 
									const TDesC8& /*aName*/, 
									const RAttributeArray& /* apAttrs */)
{
	if(iBuffer)
	{
		delete iBuffer;
		iBuffer=NULL;
	}
		
	startBuf = aLocalName;
	return KErrNone;
}


/**
* The end of the elements such as <metadata/> and its sub-element
* <capability/> are processed. The flags set for the processing of
* <metadata> and it sub-elements like <capability/> are toggled off.
*
* @return KErrNone or some of the system-wide Symbian error codes.
*/
TInt CLiwXmlHandler::EndElement(	const TDesC8& /*aURI*/, 
								const TDesC8& aLocalName, 
								const TDesC8& /*aName*/)
{
	if(0!=aLocalName.CompareF(KMetaDataName) && 0==aLocalName.CompareF(startBuf))
		iServiceData->AddMetaDataL(aLocalName,*iBuffer);
	
	delete iBuffer;
	iBuffer = NULL;

	return KErrNone;
}

/**
* The metadata can be defined in the following XML format:
*
*
* <metadata>
*	<metadata_keyname1>metadata_keyvalue1</metadata_keyname1>
*	<metadata_keyname2>metadata_keyvalue2</metadata_keyname2>
* </metadata>
*
* A realistic example below
*
*
* <metadata>
* 	<capability>CapabilityReadDeviceData</capability>
* 	<capability>CapabilityWriteDeviceData</capability>
* 	<capability>CapabilityLocation</capability>
* </metadata>
*
* This function obtains the character data defined within the
* child elements of <metadata>. The child element name under <metadata>
* is taken as metadata key and the character data under it as metadata
* value and added to the internal metadata key-value pair entries.
*
* There could be multiple metadata keys having different values as
* shown above
*
* Right now it supports only capability metadata information
*
*/										
TInt CLiwXmlHandler::Characters(const TDesC8& aBuf, 
		   							   const TInt /* aStart */, 
									   const TInt /* aLength */)
{
	if(iBuffer)
	{		
		delete iBuffer;
		iBuffer=NULL;
	}
		
	
	iBuffer = aBuf.Alloc();
	
	return KErrNone;
}

/**
* In case of parsing errors due to non-well formed XML content,
* file not being present etc are handled here. In case of XML content
* error, the metadata stored so far will be cleaned up.
*
* @return KErrNone or some of the system-wide Symbian error codes.
*/

TInt CLiwXmlHandler::Error(TInt /*aErrorCode*/)
{
	return ESrvDataLoadFailed;
}
