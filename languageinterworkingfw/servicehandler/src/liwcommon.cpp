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
* Description:       Implementation for common LIW classes to be used both
*                consumer and provider side.
*
*/






#include "liwcommon.h"
#include <eikenv.h>
const TInt KInitialMaxProviders = 0xff;

CLiwCriteriaItem::CLiwCriteriaItem()
    {
    }



EXPORT_C CLiwCriteriaItem* CLiwCriteriaItem::NewL()
    {
    CLiwCriteriaItem* item = new (ELeave) CLiwCriteriaItem();
    CleanupStack::PushL( item );
    item->ConstructL();
    CleanupStack::Pop(item); // item
    return item;
    }



EXPORT_C CLiwCriteriaItem* CLiwCriteriaItem::NewLC()
    {
    CLiwCriteriaItem* item = new (ELeave) CLiwCriteriaItem();
    CleanupStack::PushL( item );
    item->ConstructL();
    return item;
    }



EXPORT_C CLiwCriteriaItem* CLiwCriteriaItem::NewL(
    TInt aCriteriaId,
    TInt aServiceCmd,
    const TDesC8& aContentType)
    {
    CLiwCriteriaItem* item = new (ELeave) CLiwCriteriaItem();
    CleanupStack::PushL( item );
    item->ConstructL(aCriteriaId, aServiceCmd, aContentType);
    CleanupStack::Pop(item); // item
    return item;
    }



EXPORT_C CLiwCriteriaItem* CLiwCriteriaItem::NewLC(
    TInt aCriteriaId,
    TInt aServiceCmd,
    const TDesC8& aContentType)
    {
    CLiwCriteriaItem* item = new (ELeave) CLiwCriteriaItem();
    CleanupStack::PushL( item );
    item->ConstructL(aCriteriaId, aServiceCmd, aContentType);
    return item;    
    }

EXPORT_C CLiwCriteriaItem* CLiwCriteriaItem::NewL(       
                                    TInt aCriteriaId,
                                    const TDesC8& aServiceCmd,
                                    const TDesC8& aContentType)
    {
    CLiwCriteriaItem* item = new (ELeave) CLiwCriteriaItem();
    CleanupStack::PushL( item );
    item->ConstructL(aCriteriaId, aServiceCmd, aContentType);
    CleanupStack::Pop(item); // item
    return item;
    }

EXPORT_C CLiwCriteriaItem* CLiwCriteriaItem::NewLC(
                                TInt aCriteriaId,
                                const TDesC8& aServiceCmd,
                                const TDesC8& aContentType)
    {
    CLiwCriteriaItem* item = new (ELeave) CLiwCriteriaItem();
    CleanupStack::PushL( item );
    item->ConstructL(aCriteriaId, aServiceCmd, aContentType);
    return item;    
    }


EXPORT_C CLiwCriteriaItem::~CLiwCriteriaItem()
    {
    delete iReserved.iData.iBufC8; 
    delete iContentType;
    iOptions.Reset();
    
    iMetadataOptions.Reset();
    }


void CLiwCriteriaItem::ConstructL()
    {
    iMaxProviders = KInitialMaxProviders;
    }



void CLiwCriteriaItem::ConstructL(TInt aCriteriaId, TInt aServiceCmd,
    const TDesC8& aContentType)
    {
    iMaxProviders = KInitialMaxProviders;
	iCriteriaId = aCriteriaId;
    iServiceCmd = aServiceCmd;
    SetContentTypeL( aContentType );
    }

void CLiwCriteriaItem::ConstructL(TInt aCriteriaId, const TDesC8& aServiceCmd,
                                  const TDesC8& aContentType)
    {
    iMaxProviders = KInitialMaxProviders;
    iCriteriaId = aCriteriaId;
    SetServiceCmdL( aServiceCmd );
    SetContentTypeL( aContentType );
    }

EXPORT_C void CLiwCriteriaItem::SetId(TInt aId)
    {
    iCriteriaId = aId;      
    }



EXPORT_C void CLiwCriteriaItem::SetServiceClass(const TUid& aServiceUid)
    {
    iServiceClass = aServiceUid;
    }
    


EXPORT_C void CLiwCriteriaItem::SetServiceCmd(TInt aServiceCmd)
    {
    iServiceCmd = aServiceCmd;
    }

EXPORT_C void CLiwCriteriaItem::SetServiceCmdL(const TDesC8& aServiceCmd)
    {
    iServiceCmd = KLiwCmdAsStr;
    delete iReserved.iData.iBufC8;
    iReserved.iData.iBufC8 = NULL;
    iReserved.iData.iBufC8 = HBufC8::NewL(aServiceCmd.Length());
    TPtr8 content(iReserved.iData.iBufC8->Des());
    content.SetLength(aServiceCmd.Length());
    content.Copy(aServiceCmd);
    }

EXPORT_C void CLiwCriteriaItem::SetContentTypeL(const TDesC8& aContentType)
    {
    delete iContentType;
    iContentType = NULL;
    iContentType = HBufC8::NewL(aContentType.Length());
    TPtr8 content(iContentType->Des());
    content.SetLength(aContentType.Length());
    content.Copy(aContentType);
    }



EXPORT_C TInt CLiwCriteriaItem::Id() const
    {
    return iCriteriaId;
    }



EXPORT_C const TUid& CLiwCriteriaItem::ServiceClass() const
    {
    return iServiceClass;
    }
    


EXPORT_C TInt CLiwCriteriaItem::ServiceCmd() const
    {   
    return iServiceCmd;
    }

EXPORT_C const TDesC8& CLiwCriteriaItem::ServiceCmdStr() const
    {   
    return ((iReserved.iData.iBufC8) ? *iReserved.iData.iBufC8 : KNullDesC8());
    }

EXPORT_C const TDesC8& CLiwCriteriaItem::ContentType() const
    {
    if (iContentType)
        {
        return *iContentType;
        }

    return KNullDesC8();
    }



EXPORT_C void CLiwCriteriaItem::SetOptions(TUint aOptions)
    {
    if(aOptions == 0)
    	return;
    
    iOptions.Set(aOptions);
    }


EXPORT_C void CLiwCriteriaItem::SetMetaDataOptions(const TLiwVariant& aMetaDataOptions)
    {
    iMetadataOptions.SetL(aMetaDataOptions);
    }

EXPORT_C void CLiwCriteriaItem::GetMetaDataOptions(TLiwVariant& aMetaDataOptions) const
    {
    aMetaDataOptions.SetL(iMetadataOptions);
    }

EXPORT_C TUint CLiwCriteriaItem::Options() const
    {
    return iOptions.AsTUint();
    }



EXPORT_C void CLiwCriteriaItem::ReadFromResoureL(TResourceReader& aReader)
    {
    delete iContentType;
    iContentType = NULL;

    delete iReserved.iData.iBufC8;   
    iReserved.iData.iBufC8 = NULL;   
    
    iCriteriaId = aReader.ReadInt32();
    iServiceCmd = aReader.ReadInt32();

    if (iServiceCmd == KLiwCmdAsStr)   
      {
        TPtrC8 txtptr = aReader.ReadTPtrC8();
        iReserved.iData.iBufC8 = HBufC8::NewL(txtptr.Length());
        TPtr8 content(iReserved.iData.iBufC8->Des());
        content.Copy( txtptr );
      }

    {
      TPtrC8 txtptr = aReader.ReadTPtrC8();
      iContentType = HBufC8::NewL(txtptr.Length());
      TPtr8 content(iContentType->Des());
      content.Copy( txtptr );
    }

    iServiceClass = TUid::Uid(aReader.ReadInt32());

    iDefaultProvider = TUid::Uid(aReader.ReadInt32());

    iMaxProviders = aReader.ReadUint8();
    
    SetOptions((TUint)aReader.ReadUint8());
    
    TInt capResID = aReader.ReadInt32(); // Skip over reserved field.
    
    
    }



EXPORT_C TUid CLiwCriteriaItem::DefaultProvider() const
    {
    return iDefaultProvider;
    }



EXPORT_C void CLiwCriteriaItem::SetDefaultProvider(TInt aDefault)
    {
    iDefaultProvider = TUid::Uid(aDefault);
    }   


EXPORT_C TInt CLiwCriteriaItem::MaxProviders() const
    {
    return iMaxProviders;
    }


EXPORT_C void CLiwCriteriaItem::SetMaxProviders(TInt aMaxProviders)
    {
    iMaxProviders = aMaxProviders;
    }


EXPORT_C TBool CLiwCriteriaItem::operator==(const CLiwCriteriaItem& aItem)
    {
      if (iServiceCmd != KLiwCmdAsStr) 
        {
          return ((iCriteriaId                                  == aItem.iCriteriaId)           &&
                  (iServiceCmd                                  == aItem.iServiceCmd)           &&
                  (iContentType->Compare(*(aItem.iContentType)) == KErrNone)                    &&
                  (iServiceClass.iUid                           == aItem.iServiceClass.iUid)    &&
                  (iDefaultProvider.iUid                        == aItem.iDefaultProvider.iUid) &&
                  (iMaxProviders                                == aItem.iMaxProviders)         &&
                  (iOptions                                     == aItem.iOptions)				&&       
                  (iMetadataOptions                             == aItem.iMetadataOptions));       
        }
      else
        {
        	if(iCriteriaId== aItem.iCriteriaId)
        	{
        		if(0 == iReserved.iData.iBufC8->Des().Compare(aItem.iReserved.iData.iBufC8->Des()))
        		{
        			if(iContentType->Compare(*(aItem.iContentType)) == KErrNone)
        			{
        				if(iServiceClass.iUid == aItem.iServiceClass.iUid)
        				{
        					if(iDefaultProvider.iUid == aItem.iDefaultProvider.iUid)
        					{
        						if(iMaxProviders == aItem.iMaxProviders)
        						{
        							if((iOptions == aItem.iOptions))
        							{
        								if((iMetadataOptions == aItem.iMetadataOptions))
        								{
        									return ETrue;
        								}
        							}
        						}
        					}
        				}
        			}
        		}
        	}
           	return EFalse;
        }
    }

EXPORT_C TInt32 MLiwNotifyCallback::GetTransactionID()
{
	TTime currentTime;
	const TInt KMin = 60;
	const TInt KSec = 60;
	const TInt KMcSec = 1000;
	currentTime.HomeTime();
	return	 (KMin * KSec * currentTime.DateTime().Hour())+ 
			 (KMin * currentTime.DateTime().Minute())+
			 (KMcSec * currentTime.DateTime().Second()) + 
			 (currentTime.DateTime().MicroSecond());
}
// End of file
