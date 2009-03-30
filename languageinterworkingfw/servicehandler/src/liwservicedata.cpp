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
* Description:       Class modelling service provider data. The important
*				 service provider data that are part of this class
*				 is metadata information.
*
*/







#include "LiwServiceData.h"
#include <LiwVariant.h>
#include <LiwGenericParam.h>

#include <escapeUtils.h>

#include <BADESCA.H> 

/**
* Creates and returns an instance of \c CLiwServiceData
*
* @return an instance of \c CLiwServiceData
*/
CLiwServiceData* CLiwServiceData::NewL()
{
	CLiwServiceData* srvData = CLiwServiceData::NewLC();
	CleanupStack::Pop(srvData);
	return srvData;
}

/**
* Creates and returns an instance of \c CLiwServiceData.
* Leaves the created instance in the cleanupstack. 
*
* @return an instance of \c CLiwServiceData
*/
CLiwServiceData* CLiwServiceData::NewLC()
{
	CLiwServiceData* srvData = new (ELeave) CLiwServiceData();
	CleanupStack::PushL( srvData );
	srvData->ConstructL();
	return srvData;
}

/**
* Default constructor
*
*/
CLiwServiceData::CLiwServiceData():iDataList(NULL)
{
}

/**
* Instantiates metadata instance
*
*/
void CLiwServiceData::ConstructL()
{
	iDataList = CLiwGenericParamList::NewL();
}

/**
* Destructor. Cleans up the metadata instance
*
*/
CLiwServiceData::~CLiwServiceData()
{
	if(iDataList)
	{
		iDataList->Reset();
		delete iDataList;
	}
}

CLiwGenericParamList* CLiwServiceData::GetMetaData() const
{
	if(iDataList)
		return iDataList;
	else
		return NULL;
}

void CLiwServiceData::AddMetaDataL(const TDesC8& aKey, const TLiwVariant& aValue)
{
	TInt idx(0);
	const TLiwGenericParam* constParam = iDataList->FindFirst(idx,aKey);
	
	if(constParam)
	{
			TLiwGenericParam* param = const_cast<TLiwGenericParam*>(constParam);
			
			//list exists already..fetch the value list and append aValue to it
			CLiwList* pValues = const_cast<CLiwList*>(param->Value().AsList());
			pValues->AppendL(aValue);
	}
	else
	{
	 		//key does not exist so far..
			CLiwList* pValues = CLiwDefaultList::NewLC();
			pValues->AppendL(aValue);
			
			iDataList->AppendL(TLiwGenericParam(aKey,TLiwVariant(pValues)));
			CleanupStack::Pop(pValues);
			pValues->DecRef();
	}
}

/*
 * Adds a metadata name-value pair if not already added. If the key already
 * exists, then the new metadata value (if does not exist already) will be added
 * to the list of values associated with a metadata key.
 *
 * @param aKey		the metadata key to be inserted
 * @param aValue  the metadata value corresponding to the key to be added
 *
 */
void CLiwServiceData::AddMetaDataL(const TDesC8& aKey, const TDesC8& aValue)
{
	HBufC *buff = EscapeUtils::ConvertToUnicodeFromUtf8L(aValue);
	CleanupStack::PushL(buff);
	TPtrC ptr = buff->Des();
    TLiwVariant tempVar(ptr);
    tempVar.PushL();
    
	this->AddMetaDataL(aKey,tempVar);
	
	CleanupStack::Pop(&tempVar);
	tempVar.Reset();
	CleanupStack::PopAndDestroy(buff);	
}

/*
 * Removes all the metadata key-value pairs.
 * This is called from service handler implementation if and only
 * if there is an error in parsing. Since, the FW uses SAX parser
 * and if the XML error happens after the metadata entries are 
 * created, the entries should be cleaned up.
 *
 */
void CLiwServiceData::CleanUpMetaData()
{
	iDataList->Reset();
}

