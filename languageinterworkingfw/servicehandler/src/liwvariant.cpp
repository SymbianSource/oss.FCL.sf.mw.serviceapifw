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
* Description:       Implementation of class TLiwVariant.
*
*/







// INCLUDES
#ifdef _DEBUG
#include <e32svr.h>
#endif
#include "liwvariant.h"
#include "liwgenericparam.h" 

// ============================= LOCAL FUNCTIONS ===============================

using namespace LIW;
namespace {

// CONSTANTS
/// TLiwVariant streaming version
const TInt KVersion = 10;  // Version 1.0


const TInt KBufSizeMid = 128;
const TInt KBufSizeSmall = 64;
const TInt KBufSizeLarge = 8192;

   
// Debug helpers
#ifdef _DEBUG

enum TPanicCode
    {
    EPanicPostCond_Constructor = 1,
    EPanicPostCond_Reset,
    EPanicPostCond_Set_TInt32,
    EPanicPostCond_Set_TUid,
    EPanicPostCond_Set_TTime,
    EPanicPostCond_Set_TDesC,
    EPanicPostCond_CopyL,
    EPanicInvariant_InvalidDesCState,
    EPanicPostCond_Set_TDesC8,
    EPanicPreCond_DataSizeMismatch,
    EPanicPostCond_Set_TBool,
    EPanicPostCond_Set_TUint,
    EPanicPostCond_Set_TReal,
    EPanicPostCond_Set_TInt64
    };

void Panic(TPanicCode aCode)
    {
    _LIT(KPanicText, "TLiwVariant");
    User::Panic(KPanicText, aCode);
    }

#endif  // #ifdef _DEBUG

}  // namespace
 
_LIT8(KDummy, "Dummy"); // needed for BC-preserving hack to store info about ownership of container

/*
 * class CLiwContainer, class CLiwIterable, class CLiwList, class CLiwMap,
 * class CLiwDefaultList, class CLiwDefaultMap
 */

EXPORT_C void CLiwContainer::PushL() 
    {
    CleanupClosePushL(*this);
    }

EXPORT_C void CLiwContainer::ExternalizeL(RWriteStream&) const
    {
    User::Leave(KErrNotSupported);
    }

EXPORT_C TInt CLiwContainer::Size() const
    {
    return KErrNotSupported;
    }

EXPORT_C CLiwContainer::~CLiwContainer()
    {
   /*
    * Since CLiwContainer is based on reference count mechanism, it does not go with CleanupStack::PushL()
    *
    * LIW takes care of managing the memory in heap for CLiwContainer using the reference count logic
    * 
    * The container should not be destroyed until the reference count is ZERO.
    * If the reference count is not equal to ZERO, it means there are some more owners of that container
    * 
    * This panics if the reference count is not equal to ZERO, in DEBUG Mode only
    *
    * Hence it is recommended to use CleanupClosePushL for Container objects when using CleanupStack
    */
    
    __ASSERT_DEBUG(iRefCount == 0, User::Panic(_L("E32USER-CBase: 33"), 1));
    }

EXPORT_C void CLiwContainer::Close()
    {
    DecRef();
    }


EXPORT_C TBool CLiwIterable::operator==(CLiwIterable& aOther)
    {
    if (this == &aOther)
    return ETrue;

    Reset();
    aOther.Reset();

    TBool rval = ETrue;
    TLiwVariant variant1, variant2;

    TRAPD(error, {
    while (NextL(variant1)) 
	    {
	      if (!aOther.NextL(variant2) || (variant1 != variant2))
	        rval = EFalse;
	    }
    });

    if (rval && (error == KErrNone))
        {
        TRAP(error, (rval = !(aOther.NextL(variant2))));
        }

    variant1.Reset();
    variant2.Reset();
    
    return (error == KErrNone) ? rval : EFalse;
    }

EXPORT_C TBool CLiwList::operator==(const CLiwList& aOther) const
    {
    TInt count = Count();
    if (count != aOther.Count())
    return EFalse;

    TInt error = KErrNone;
    TBool retval = ETrue;
    TLiwVariant variant1, variant2;
    for (TInt index = 0; index < count; index++) {
    TRAP(error, {
      AtL(index, variant1); aOther.AtL(index, variant2);
    });
    if ((error != KErrNone) || (!(variant1 == variant2)))
        {
        retval = EFalse;
        break;
        }
    }
    variant1.Reset();
    variant2.Reset();
    return retval;
    }

EXPORT_C TBool CLiwMap::operator==(const CLiwMap& aOther) const
    {
    TInt count = Count();
    if (count != aOther.Count())
    return EFalse;

    TInt error = KErrNone;
    TBool retval = ETrue;
    TLiwVariant variant1, variant2;
    TBuf8<KBufSizeMid> buf1, buf2;
    for (TInt index = 0; index < count; index++)
        {
        TRAP(error, {
          AtL(index, buf1); aOther.AtL(index, buf2);
          retval = (FindL(buf1, variant1) && aOther.FindL(buf2, variant2));
        });
        if (error != KErrNone)
          retval = EFalse;
        if (!retval || (!(variant1 == variant2)))
            {
            retval = EFalse;
            break;
            }
        }
    variant1.Reset();
    variant2.Reset();
    return retval;
    }

EXPORT_C CLiwDefaultList* CLiwDefaultList::NewL()
    {
    CLiwGenericParamList* gl = CLiwGenericParamList::NewLC();
    CLiwDefaultList* tempList = new (ELeave) CLiwDefaultList(gl);
    CleanupStack::Pop(gl); // gl
    return tempList;
    }

EXPORT_C CLiwDefaultList* CLiwDefaultList::NewLC()
    {
    CLiwGenericParamList* gl = CLiwGenericParamList::NewLC();
    CLiwDefaultList* tempList = new (ELeave) CLiwDefaultList(gl);
    CleanupStack::Pop(gl); // gl
    CleanupClosePushL(*tempList);
    return tempList;
    }

EXPORT_C CLiwDefaultList* CLiwDefaultList::NewLC(RReadStream& aStream)
    {
    CLiwDefaultList* tempList = CLiwDefaultList::NewL();
    CleanupClosePushL(*tempList);
    tempList->iList->InternalizeL(aStream);
    return tempList;
    }

EXPORT_C void CLiwDefaultList::AppendL(const TLiwVariant& aValue)
    {
    iList->AppendL(TLiwGenericParam(EGenericParamUnspecified, aValue));
    }

EXPORT_C TBool CLiwDefaultList::AtL(TInt aIndex, TLiwVariant& aValue) const
    {
    if(0 <= aIndex && aIndex < iList->Count())
        {
        aValue.SetL((*iList)[aIndex].Value());
        return ETrue;   
        }
    else
        {
        return EFalse;    
        }
    }

EXPORT_C void CLiwDefaultList::Remove(TInt aIndex)
    {
    if(0 <= aIndex && aIndex < iList->Count())
        {
        iList->iParameters[aIndex].Destroy();
        iList->iParameters.Remove(aIndex);   
        }
    }

EXPORT_C TInt CLiwDefaultList::Count() const
    {
    return iList->Count();
    }

EXPORT_C void CLiwDefaultList::ExternalizeL(RWriteStream& aStream) const
    {
    iList->ExternalizeL(aStream);
    }

EXPORT_C TInt CLiwDefaultList::Size() const
    {
    return iList->Size();
    }

EXPORT_C CLiwDefaultList::~CLiwDefaultList()
    {
    delete iList;
    }

EXPORT_C CLiwDefaultMap* CLiwDefaultMap::NewL()
    {
    CLiwGenericParamList* gl = CLiwGenericParamList::NewLC();
    CLiwDefaultMap* tempMap = new (ELeave) CLiwDefaultMap(gl);
    CleanupStack::Pop(gl); // gl
    return tempMap;
    }

EXPORT_C CLiwDefaultMap* CLiwDefaultMap::NewLC()
    {
    CLiwGenericParamList* gl = CLiwGenericParamList::NewLC();
    CLiwDefaultMap* tempMap = new (ELeave) CLiwDefaultMap(gl);
    CleanupStack::Pop(gl); // gl
    CleanupClosePushL(*tempMap);
    return tempMap;
    }

EXPORT_C CLiwDefaultMap* CLiwDefaultMap::NewLC(RReadStream& aStream)
    {
    CLiwDefaultMap* tempMap = CLiwDefaultMap::NewL();
    CleanupClosePushL(*tempMap);
    tempMap->iMap->InternalizeL(aStream);
    return tempMap;
    }

EXPORT_C void CLiwDefaultMap::InsertL(const TDesC8& aKey, const TLiwVariant& aValue)
    {
    Remove(aKey);
    TLiwGenericParam mp;
    mp.SetNameAndValueL(aKey, aValue);
    iMap->AppendL(mp);
    mp.Reset();
    }

EXPORT_C TBool CLiwDefaultMap::FindL(const TDesC8& aKey, TLiwVariant& aValue) const
    {
    TInt pos = 0;
    const TLiwGenericParam* tempParam = iMap->FindFirst(pos, aKey);
    if (tempParam)
        {
        aValue.SetL(tempParam->Value());
        return ETrue;
        }
    else
        return EFalse;
    }

EXPORT_C TInt CLiwDefaultMap::Count() const
    {
    return iMap->Count();
    }

EXPORT_C TBool CLiwDefaultMap::AtL(TInt aIndex, TDes8& aKey) const
    {
    if(0 <= aIndex && aIndex < iMap->Count())
        {
        aKey = ((*iMap)[aIndex]).Name();
        return ETrue;   
        }
    else
        {
        return EFalse;  
        }
    }

EXPORT_C const TDesC8& CLiwDefaultMap::AtL(TInt aIndex) const
    {
    if(0 <= aIndex && aIndex < iMap->Count())
        {
        return ((*iMap)[aIndex]).Name();
        }
    else
        {
        User::LeaveIfError(KErrArgument);  
        return KNullDesC8;
        }
    }

EXPORT_C void CLiwDefaultMap::Remove(const TDesC8& aKey)
    {
    TInt pos = 0;
    iMap->FindFirst(pos, aKey);
    if (pos != KErrNotFound)
        {
        iMap->iParameters[pos].Destroy();
        iMap->iParameters.Remove(pos);
        }
    }

EXPORT_C void CLiwDefaultMap::ExternalizeL(RWriteStream& aStream) const
    {
    iMap->ExternalizeL(aStream);
    }

EXPORT_C TInt CLiwDefaultMap::Size() const
    {
    return iMap->Size();
    }

EXPORT_C CLiwDefaultMap::~CLiwDefaultMap()
    {
    delete iMap;
    }

// ============================ MEMBER FUNCTIONS ===============================

#ifdef _DEBUG
void TLiwVariant::__DbgTestInvariant() const
    {
    if (iTypeId==EVariantTypeDesC && iData.iBufC)
        {
        __ASSERT_ALWAYS(iData.iBufC->Ptr() == iPtrC.Ptr(), 
            Panic(EPanicInvariant_InvalidDesCState));
        
        }
    }
#endif  // #ifdef _DEBUG


EXPORT_C void TLiwVariant::PushL()
    {
    CleanupStack::PushL( TCleanupItem( TLiwVariant::VariantCleanup , this) );
    }

EXPORT_C void TLiwVariant::VariantCleanup( TAny* aObj )
    {
    static_cast<TLiwVariant*>(aObj)->Destroy();
    }

inline void TLiwVariant::SInt64::InternalizeL(RReadStream& aStream)
    {
    TInt32 low = aStream.ReadInt32L();
    iHigh = aStream.ReadInt32L();
    iLow = low;
    }

inline void TLiwVariant::SInt64::ExternalizeL(RWriteStream& aStream) const
    {
    aStream.WriteInt32L(iLow);
    aStream.WriteInt32L(iHigh);
    }

inline void TLiwVariant::SInt64::Set(const TInt64& aTInt64)
    {
    iLow = I64LOW(aTInt64);
    iHigh = I64HIGH(aTInt64);
    }

inline TLiwVariant::SInt64::operator TInt64() const
    {
    return MAKE_TINT64(iHigh,iLow);
    }

inline TBool TLiwVariant::SInt64::operator==(const SInt64& aRhs) const
    {
    return (iHigh==aRhs.iHigh && iLow==aRhs.iLow);
    }


EXPORT_C TLiwVariant::TLiwVariant(const TLiwVariant& aSrc) :
    iTypeId(aSrc.iTypeId), iData(aSrc.iData), iPtrC(), iPtrC8()
    {
    if (iTypeId == EVariantTypeDesC)
        {
        iPtrC.Set(aSrc.iPtrC);
        // Do not take ownership of data
        iData.iBufC = NULL;
        }
    else if ( iTypeId == EVariantTypeDesC8 )
        {
        iPtrC8.Set( aSrc.iPtrC8 );
        // Do not take ownership of data
        iData.iBufC8 = NULL;
        }
    }

EXPORT_C TLiwVariant& TLiwVariant::operator=(const TLiwVariant& aSrc)
    {
    // Check self-assignment first.
    if (this == &aSrc)
        {
        return *this;
        }
    Reset(); 
    iTypeId = aSrc.iTypeId;
    iData = aSrc.iData;
    if (iTypeId == EVariantTypeDesC)
        {
        iPtrC.Set(aSrc.iPtrC);
        // Do not take ownership of data
        iData.iBufC = NULL;
        }
    else if ( iTypeId == EVariantTypeDesC8 )
        {
        iPtrC8.Set( aSrc.iPtrC8 );
        // Do not take ownership of data
        iData.iBufC8 = NULL;
        }
    return *this;
    }

EXPORT_C TBool TLiwVariant::Get(TInt32& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeTInt32)
        {
        aValue = iData.iInt32;
        return ETrue;
        }
    else if (iTypeId == EVariantTypeTReal)
	    {
	    aValue = (TInt32)iData.iReal;
	    return ETrue;
	    }
	else if (iTypeId == EVariantTypeDesC8)  //string to real conversion
		{
		TLex8 parseString(iPtrC8);
		TInt err = parseString.Val(aValue);
		if(err == KErrNone)
			return ETrue;
		}
	else if (iTypeId == EVariantTypeDesC)  //string to real conversion
		{
		TLex16 parseString(iPtrC);
		TInt err = parseString.Val(aValue);
		if(err == KErrNone)
			return ETrue;		
		}
	else if (iTypeId == EVariantTypeTUint) //TUint to int conversion
	    {
	    aValue = (TInt32)iData.iUint;
	    return ETrue;
	    }
	else if (iTypeId == EVariantTypeTInt64) //TInt64 to TInt32 conversion
	    {
	    aValue = (TInt32)iData.iLong; //Warning : Results in loss of data sometimes
	    return ETrue;
	    }
	return EFalse;
    }
    
EXPORT_C TBool TLiwVariant::Get(TInt64& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeTInt64)
        {
        aValue = iData.iLong;
        return ETrue;
        }
    else if (iTypeId == EVariantTypeTInt32)
        {
        aValue = iData.iInt32;
        return ETrue;
        }
    else if (iTypeId == EVariantTypeTReal)
	    {
	    aValue = (TInt32)iData.iReal;
	    return ETrue;
	    }
	else if (iTypeId == EVariantTypeDesC8)  //string to real conversion
		{
		TLex8 parseString(iPtrC8);
		TInt err = parseString.Val(aValue);
		if(err == KErrNone)
			return ETrue;
		}
	else if (iTypeId == EVariantTypeDesC)  //string to real conversion
		{
		TLex16 parseString(iPtrC);
		TInt err = parseString.Val(aValue);
		if(err == KErrNone)
			return ETrue;		
		}
	else if (iTypeId == EVariantTypeTUint) //TUint to int conversion
	    {
	    aValue = (TInt32)iData.iUint;
	    return ETrue;
	    }
    return EFalse;
    }
    
EXPORT_C TBool TLiwVariant::Get(TReal& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeTReal)
        {
        aValue = iData.iReal;
        return ETrue;
        }
    else if(iTypeId == EVariantTypeTInt32)
    	{
    	aValue = iData.iInt32;
    	return ETrue;
    	}
    else if (iTypeId == EVariantTypeDesC8)  //string to integer conversion
		{
		TLex8 parseString(iPtrC8);
		TInt err = parseString.Val(aValue);
		if(err == KErrNone)
			return ETrue;		
		}
	else if (iTypeId == EVariantTypeDesC)  //string to integer conversion
		{
		TLex16 parseString(iPtrC);
		TInt err = parseString.Val(aValue);
		if(err == KErrNone)
			return ETrue;		
		}
	else if (iTypeId == EVariantTypeTUint) //TUint to real conversion
	    {
	    aValue = iData.iUint;
	    return ETrue;
	    }
    else if(iTypeId == EVariantTypeTInt64)
    	{
    	aValue = iData.iLong; //Warning : Results in loss of data
    	return ETrue;
    	}
    return EFalse;
    }
EXPORT_C TBool TLiwVariant::Get(TBool& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeTBool)
        {
        aValue = iData.iBool;
        return ETrue;
        }
    return EFalse;
    }
    
EXPORT_C TBool TLiwVariant::Get(TUint& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeTUint)
        {
        aValue = iData.iUint;
        return ETrue;
        }        
    else if (iTypeId == EVariantTypeTReal) //real to TUint conversion
        {
        aValue = static_cast<TUint>(iData.iReal);
        return ETrue;
        }
    else if(iTypeId == EVariantTypeTInt32) // TInt32 to TUint conversion
    	{
        aValue = static_cast<TUint>(iData.iInt32);
    	return ETrue;
    	}
    else if (iTypeId == EVariantTypeDesC8)  //desc8 to TUint conversion
		{
		TLex8 parseString(iPtrC8);
		TInt err = parseString.Val(aValue);
		if(err == KErrNone)
			return ETrue;		
		}
	else if (iTypeId == EVariantTypeDesC)  //desc16 to TUint conversion
		{
		TLex16 parseString(iPtrC);
		TInt err = parseString.Val(aValue);
		if(err == KErrNone)
        return ETrue;
        }
    else if(iTypeId == EVariantTypeTInt64) // TInt32 to TUint conversion
    	{
        aValue = static_cast<TUint>(iData.iLong); //Warning : Results in loss of data
    	return ETrue;
    	}
    return EFalse;
    }

EXPORT_C TBool TLiwVariant::Get(TUid& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeTUid)
        {
        aValue = TUid::Uid(iData.iInt32);
        return ETrue;
        }
    return EFalse;
    }

EXPORT_C TBool TLiwVariant::Get(TPtrC& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeDesC)
        {
        aValue.Set(iPtrC);
        return ETrue;
        }
    return EFalse;
    }

EXPORT_C TBool TLiwVariant::Get(TTime& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeTTime)
        {
        aValue = TTime(iData.iInt64);
        return ETrue;
        }
    return EFalse;
    }


EXPORT_C TBool TLiwVariant::Get(TPtrC8& aValue) const
    {
     __TEST_INVARIANT;  
    if (iTypeId == EVariantTypeDesC8)
        {
        aValue.Set(iPtrC8);
        return ETrue;
        }   

    return EFalse;
    }

EXPORT_C TBool TLiwVariant::Get(TDes& aValue) const
    {
     __TEST_INVARIANT;  
    if (iTypeId == EVariantTypeDesC)
        {
        aValue.Copy(iPtrC);
        return ETrue;
        }   
	else if (iTypeId == EVariantTypeDesC8)
        {
        aValue.Copy(iPtrC8);
        return ETrue;
        }   
	else if (iTypeId == EVariantTypeTReal) //real to TDes conversion
        {
        const TRealFormat realFormat;
        aValue.Num(iData.iReal,realFormat);
        return ETrue;
        }
    else if(iTypeId == EVariantTypeTInt32) // TInt32 to TDes conversion
    	{
    	aValue.Num(iData.iInt32);
    	return ETrue;
    	}
    else if(iTypeId == EVariantTypeTUint) // TInt32 to TDes conversion
    	{
    	aValue.Num(iData.iUint,EDecimal);
    	return ETrue;
    	}
    else if(iTypeId == EVariantTypeTInt64) // TInt32 to TDes conversion
    	{
    	aValue.Num(iData.iLong);
    	return ETrue;
    	}
    return EFalse;
    }


EXPORT_C TBool TLiwVariant::Get(TDes8& aValue) const
    {
     __TEST_INVARIANT;  
    if (iTypeId == EVariantTypeDesC8)
        {
        aValue.Copy(iPtrC8);
        return ETrue;
        }   
	else if (iTypeId == EVariantTypeDesC)
        {
        aValue.Copy(iPtrC);
        return ETrue;
        }   
	else if (iTypeId == EVariantTypeTReal) //real to TDes8 conversion
        {
        const TRealFormat realFormat;
        aValue.Num(iData.iReal,realFormat);
        return ETrue;
        }
   else if(iTypeId == EVariantTypeTInt32) // TInt32 to TDes8 conversion
    	{
    	aValue.Num(iData.iInt32);
    	return ETrue;
    	}
    else if(iTypeId == EVariantTypeTUint) // TInt32 to TDes conversion
    	{
    	aValue.Num(iData.iUint,EDecimal);
    	return ETrue;
    	}
    else if(iTypeId == EVariantTypeTInt64) // TInt32 to TDes8 conversion
    	{
    	aValue.Num(iData.iLong);
    	return ETrue;
    	}
    return EFalse;
    }


EXPORT_C TBool TLiwVariant::Get(RFile& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeFileHandle)
        {
        aValue = *((RFile*)&iData.iInt64);      
        return ETrue;
        }

    return EFalse;
    }

EXPORT_C TBool TLiwVariant::Get(CLiwList& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeList)
        {
        TInt pos = 0;
        TLiwVariant tempVarient;
        tempVarient.PushL();
        // Overwrite the list
        if(0 != aValue.Count())
            {
            // make the list Empty
            for (pos = aValue.Count() - 1; pos >= 0 ; pos--)
                aValue.Remove(pos);
            }
        // Copy List varient by varient
        for (pos = 0; pos < iData.iList->Count(); pos++)
            {
            iData.iList->AtL(pos, tempVarient);
            aValue.AppendL(tempVarient);
            }
        CleanupStack::Pop(&tempVarient);
    	tempVarient.Reset();
        return ETrue;
        }

    return EFalse;
    }
    
EXPORT_C TBool TLiwVariant::Get(CLiwMap& aValue) const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeMap)
        {
        TInt pos = 0;
        TLiwVariant tempVarient;
        tempVarient.PushL();
        // Overwrite the Map
        if(0 != aValue.Count())
            {
            // make the Map Empty
            for (pos = aValue.Count() - 1; pos >= 0 ; pos--)
                {
                TBuf8<KBufSizeSmall> mapKey;
                aValue.AtL(pos, mapKey);
                aValue.Remove(mapKey);
                }
            }
        // Copy Map varient by varient
        for (pos = 0; pos < iData.iMap->Count(); pos++)
            {
            TBuf8<KBufSizeSmall> mapKey;
            iData.iMap->AtL(pos, mapKey);
            iData.iMap->FindL(mapKey, tempVarient);
            aValue.InsertL(mapKey, tempVarient);
            }
        CleanupStack::Pop(&tempVarient);
    	tempVarient.Reset();      
        return ETrue;
        }

    return EFalse;
    }
EXPORT_C TReal TLiwVariant::AsTReal() const
    {
    TReal value = 0;
    Get(value);
    return value;
    }   
    
EXPORT_C TInt32 TLiwVariant::AsTInt32() const
    {
    TInt32 value = 0;
    Get(value);
    return value;
    }   

EXPORT_C TInt64 TLiwVariant::AsTInt64() const
    {
    TInt64 value = 0;
    Get(value);
    return value;
    }   

EXPORT_C CLiwBuffer* TLiwVariant::AsBuffer() const
    {
    __TEST_INVARIANT;
    if (iTypeId == EVariantTypeBuffer)
    	{
	    return iData.iBuffer;
    	}    
    	
    return NULL;	
    }   
    
EXPORT_C TBool TLiwVariant::AsTBool() const
    {
    TBool value = 0;
    Get(value);
    return value;
    }   

EXPORT_C TBool TLiwVariant::AsTUint() const
    {
    TUint value = 0;
    Get(value);
    return value;
    } 

EXPORT_C TUid TLiwVariant::AsTUid() const
    {
    __TEST_INVARIANT;
    TUid value = {0};
    Get(value);
    return value;
    }

EXPORT_C TPtrC TLiwVariant::AsDes() const
    {
    __TEST_INVARIANT;
    TPtrC value;
    Get(value);
    return value;
    }

EXPORT_C TTime TLiwVariant::AsTTime() const
    {
    __TEST_INVARIANT;
    TTime value(Time::NullTTime());
    Get(value);
    return value;
    }


EXPORT_C TPtrC8 TLiwVariant::AsData() const
    {
     __TEST_INVARIANT;
    TPtrC8 value;
    Get(value);
    return value;
    }


EXPORT_C RFile TLiwVariant::AsFileHandle() const
    {
    __TEST_INVARIANT;
    RFile value;
    Get(value);
    return value;
    }

EXPORT_C const CLiwList* TLiwVariant::AsList() const
    {
    __TEST_INVARIANT;
    return (iTypeId == EVariantTypeList) ? iData.iList : NULL;
    }

EXPORT_C const CLiwMap* TLiwVariant::AsMap() const
   {
   __TEST_INVARIANT;
   return (iTypeId == EVariantTypeMap) ? iData.iMap : NULL;
   }

EXPORT_C MLiwInterface* TLiwVariant::AsInterface() const
   {
   __TEST_INVARIANT;
   return (iTypeId == EVariantTypeInterface) ? iData.iSession : NULL;
   }

EXPORT_C CLiwIterable* TLiwVariant::AsIterable() const
    {
    __TEST_INVARIANT;
    return (iTypeId == EVariantTypeIterable) ? iData.iIterable : NULL;
    }

EXPORT_C void TLiwVariant::Reset()
    {
    __TEST_INVARIANT;

    if (iTypeId == EVariantTypeDesC)
        {
        // Delete any owned buffer
        delete iData.iBufC;
        iData.iBufC = NULL;
        }
    else if (iTypeId == EVariantTypeDesC8)
        {
        delete iData.iBufC8;
        iData.iBufC8 = NULL;
        }       
    else if (iTypeId == EVariantTypeList)
        {
        if (iPtrC8.Compare(KDummy) == 0)
            {
            iData.iList->DecRef();
            iData.iList = NULL;
            iPtrC8.Set(TPtrC8());
            }
        }   
    else if (iTypeId == EVariantTypeMap)
        {
        if (iPtrC8.Compare(KDummy) == 0)
            {
            iData.iMap->DecRef();
            iData.iMap = NULL;
            iPtrC8.Set(TPtrC8());
            }
        }   
    else if (iTypeId == EVariantTypeIterable)
        {
        if (iPtrC8.Compare(KDummy) == 0)
            {
            iData.iIterable->DecRef();
            iData.iIterable = NULL;
            iPtrC8.Set(TPtrC8());
            }
        }
	else if (iTypeId == EVariantTypeBuffer)
        {
        if (iPtrC8.Compare(KDummy) == 0)
            {
            iData.iBuffer->DecRef();
            iData.iBuffer = NULL;
            iPtrC8.Set(TPtrC8());
            }
        }        
    // No need to clear other data, because Get methods wont't do anything if type
    // is Null.
    iTypeId = EVariantTypeNull;

    __ASSERT_DEBUG(IsEmpty(), Panic(EPanicPostCond_Reset));
    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(TInt32 aValue)
    {
    __TEST_INVARIANT;
    
    Reset();
    iTypeId = EVariantTypeTInt32;
    iData.iInt32 = aValue;
    
    __ASSERT_DEBUG(this->AsTInt32()==aValue, Panic(EPanicPostCond_Set_TInt32));
    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(TInt64 aValue)
    {
    __TEST_INVARIANT;
    
    Reset();
    iTypeId = EVariantTypeTInt64;
    iData.iLong = aValue;
    
    __ASSERT_DEBUG(this->AsTInt64()==aValue, Panic(EPanicPostCond_Set_TInt64));
    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(TReal aValue)
    {
    __TEST_INVARIANT;
    
    Reset();
    iTypeId = EVariantTypeTReal;
    iData.iReal = aValue;
    
    __ASSERT_DEBUG(this->AsTReal()==aValue, Panic(EPanicPostCond_Set_TReal));
    __TEST_INVARIANT;
    }
EXPORT_C void TLiwVariant::Set(const CLiwBuffer* aValue)
    {
    __TEST_INVARIANT;
    
    Reset();
    iTypeId = EVariantTypeBuffer;
    iData.iBuffer = (CLiwBuffer*)aValue;
    
    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(TBool aValue)
    {
    __TEST_INVARIANT;
    
    Reset();
    iTypeId = EVariantTypeTBool;
    iData.iBool = aValue;
    
    __ASSERT_DEBUG(this->AsTBool()==aValue, Panic(EPanicPostCond_Set_TBool));
    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(TUint aValue)
    {
    __TEST_INVARIANT;
    
    Reset();
    iTypeId = EVariantTypeTUint;
    iData.iUint = aValue;
    
    __ASSERT_DEBUG(this->AsTUint()==aValue, Panic(EPanicPostCond_Set_TUint));
    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(const TUid& aValue)
    {
    __TEST_INVARIANT;
    
    Reset();
    iTypeId = EVariantTypeTUid;
    iData.iInt32 = aValue.iUid;
    
    __ASSERT_DEBUG(this->AsTUid()==aValue, Panic(EPanicPostCond_Set_TUid));
    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(const TTime& aValue)
    {
    __TEST_INVARIANT;
    
    Reset();
    iTypeId = EVariantTypeTTime;
    iData.iInt64.Set(aValue.Int64());
    
    __ASSERT_DEBUG(this->AsTTime()==aValue, Panic(EPanicPostCond_Set_TTime));
    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(const TDesC& aValue)
    {
    __TEST_INVARIANT;

    Reset();
    iTypeId = EVariantTypeDesC;
    iData.iBufC = NULL;  // not owned
    iPtrC.Set(aValue);

    __ASSERT_DEBUG(this->AsDes()==aValue, Panic(EPanicPostCond_Set_TDesC));
    __TEST_INVARIANT;
    }



EXPORT_C void TLiwVariant::Set(const TDesC8& aValue)
    {
   __TEST_INVARIANT;

    Reset();
    iTypeId = EVariantTypeDesC8;
    iData.iBufC8 = NULL;  // not owned
    iPtrC8.Set(aValue);

    __ASSERT_DEBUG(this->AsData()==aValue, Panic(EPanicPostCond_Set_TDesC));
    __TEST_INVARIANT;
    }



EXPORT_C void TLiwVariant::Set(const RFile& aValue)
    {
    __TEST_INVARIANT;
    __ASSERT_DEBUG(sizeof(SInt64) == sizeof(RFile), Panic(EPanicPreCond_DataSizeMismatch));

    Reset();
    iTypeId = EVariantTypeFileHandle;
    *((RFile*)&iData.iInt64) = aValue;

    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(const CLiwList* aValue)
    {
    __TEST_INVARIANT;

    Reset();
    iTypeId = EVariantTypeList;
    iData.iList = (CLiwList*)aValue;

    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(const CLiwMap* aValue)
    {
    __TEST_INVARIANT;

    Reset();
    iTypeId = EVariantTypeMap;
    iData.iMap = (CLiwMap*)aValue;

    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(const MLiwInterface* aValue)
    {
    __TEST_INVARIANT;

    Reset();
    iTypeId = EVariantTypeInterface;
    iData.iSession = (MLiwInterface*)aValue;

    __TEST_INVARIANT;
    }

EXPORT_C void TLiwVariant::Set(const CLiwIterable* aValue)
    {
    __TEST_INVARIANT;

    Reset();
    iTypeId = EVariantTypeIterable;
    iData.iIterable = (CLiwIterable*)aValue;

    __TEST_INVARIANT;
    }
    
EXPORT_C void TLiwVariant::SetL(const TLiwVariant& aValue)
    {
    __TEST_INVARIANT;

    if (aValue.iTypeId == EVariantTypeDesC)
        {
        // Take an own copy of the string value
        HBufC* buf = aValue.iPtrC.AllocL();
        Reset();
        iTypeId = EVariantTypeDesC;
        iData.iBufC = buf;
        iPtrC.Set(*iData.iBufC);
        }
    else if (aValue.iTypeId == EVariantTypeDesC8)
        {
        // Take an own copy of the data
        HBufC8* buf = aValue.iPtrC8.AllocL();
        Reset();
        iTypeId = EVariantTypeDesC8;
        iData.iBufC8 = buf;
        iPtrC8.Set(*iData.iBufC8);
        }
    else if (aValue.iTypeId == EVariantTypeList)
        {
        // Take an own copy of the data by increasing the reference count
        Reset();
        iTypeId = EVariantTypeList;
        iData.iList = aValue.iData.iList;
        iData.iList->IncRef();
        iPtrC8.Set(KDummy);  // hack: mark owned
        }
    else if (aValue.iTypeId == EVariantTypeMap) 
        {
        // Take an own copy of the data by increasing the reference count
        Reset();
        iTypeId = EVariantTypeMap;
        iData.iMap = aValue.iData.iMap;
        iData.iMap->IncRef();
        iPtrC8.Set(KDummy);  // hack: mark owned
        }
    else if (aValue.iTypeId == EVariantTypeIterable) 
        {
        // Take an own copy of the data by increasing the reference count
        Reset();
        iTypeId = EVariantTypeIterable;
        iData.iIterable = aValue.iData.iIterable;
        iData.iIterable->IncRef();
        iPtrC8.Set(KDummy);  // hack: mark owned
        }
	else if (aValue.iTypeId == EVariantTypeBuffer) 
        {
        Reset();
        iTypeId = EVariantTypeBuffer;
        iData.iBuffer = aValue.iData.iBuffer;
       	iData.iBuffer->IncRef();
        iPtrC8.Set(KDummy);  // hack: mark owned
        }        
    else
        {
        Reset();
        iTypeId = aValue.iTypeId;
        // Copy the data union as one block
        iData = aValue.iData;
        }

    //__ASSERT_DEBUG(*this == aValue, Panic(EPanicPostCond_CopyL));
    __TEST_INVARIANT;
    }

void TLiwVariant::Destroy()
    {
    __TEST_INVARIANT;

    if (iTypeId == EVariantTypeDesC)
        {
        // Delete any owned buffer
        delete iData.iBufC;
        iData.iBufC = NULL;
        }
    else if (iTypeId == EVariantTypeDesC8)
        {
        delete iData.iBufC8;
        iData.iBufC8 = NULL;
        }
    else if (iTypeId == EVariantTypeList)
        {
        if (iPtrC8.Compare(KDummy) == 0)
            {
            iData.iList->DecRef();
            iData.iList = NULL;
            iPtrC8.Set(TPtrC8());
            }
        }
    else if (iTypeId == EVariantTypeMap) 
        {
        if (iPtrC8.Compare(KDummy) == 0)
            {
            iData.iMap->DecRef();
            iData.iMap = NULL;
            iPtrC8.Set(TPtrC8());
            }
        }
    else if (iTypeId == EVariantTypeIterable)  
        {
        if (iPtrC8.Compare(KDummy) == 0)
            {
            iData.iIterable->DecRef();
            iData.iIterable = NULL;
            iPtrC8.Set(TPtrC8());
            }
        }
    else if (iTypeId == EVariantTypeBuffer)  
        {
        if (iPtrC8.Compare(KDummy) == 0)
            {
            iData.iBuffer->DecRef();
            iData.iBuffer = NULL;
            iPtrC8.Set(TPtrC8());
            }
        }
    }

void TLiwVariant::InternalizeL(RReadStream& aStream)
    {
    __TEST_INVARIANT;
        
    aStream.ReadInt8L(); // version
    // if older version adapt to changes (who knows if
    // parameters would be also persistent...)

    const TUint8 typeId = aStream.ReadUint8L();
    switch (typeId)
        {
        case EVariantTypeNull:
            {
            Reset();
            break;
            }
        case EVariantTypeTInt32:  // FALLTHROUGH
        case EVariantTypeTUid:
        case EVariantTypeTBool:
        case EVariantTypeTUint:
            {
            TInt32 value = aStream.ReadInt32L();
            Reset();
            iTypeId = typeId;
            iData.iInt32 = value;
            break;
            }
        case EVariantTypeTInt64:
            {
            TInt64 value = aStream.ReadReal64L();
            Reset();
            iTypeId = typeId;
            iData.iLong = value;
            break;
            }
        case EVariantTypeFileHandle: // FALLTHROUGH
        case EVariantTypeTTime:
        case EVariantTypeTReal:
            {
            SInt64 value;
            value.InternalizeL(aStream);
            Reset();
            iTypeId = typeId;
            iData.iInt64 = value;
            break;
            }
        case EVariantTypeDesC:
            {
            const TInt len = aStream.ReadInt32L();
            HBufC* buf = HBufC::NewL(aStream,len);
            Reset();
            iTypeId = typeId;
            iData.iBufC = buf;
            iPtrC.Set(*iData.iBufC);
            break;
            }
        case EVariantTypeDesC8:
            {
            const TInt len = aStream.ReadInt32L();
            HBufC8* buf = HBufC8::NewL(aStream,len);
            Reset();
            iTypeId = typeId;
            iData.iBufC8 = buf;
            iPtrC8.Set(*iData.iBufC8);
            break;
            }
        case EVariantTypeList:  
            {
            CLiwList* list = CLiwDefaultList::NewLC(aStream);
            Reset();
            iTypeId = EVariantTypeList;
            iData.iList = list;
            iPtrC8.Set(KDummy);  // hack: mark owned
            CleanupStack::Pop(list); // list
            break;
            }
        case EVariantTypeMap:  
            {
            CLiwMap* map = CLiwDefaultMap::NewLC(aStream);
            Reset();
            iTypeId = EVariantTypeMap;
            iData.iMap = map;
            iPtrC8.Set(KDummy);  // hack: mark owned
            CleanupStack::Pop(map); // map
            break;
            }
        default:
            {
             // Corrupted data stream.
#ifdef _DEBUG
            RDebug::Print(_L("***ERROR TLiwVariant::InternalizeL"));
#endif
            User::Leave(KErrCorrupt);
            return;
            }
        }

    __TEST_INVARIANT;
    }
    
void TLiwVariant::ExternalizeL(RWriteStream& aStream) const
    {
    __TEST_INVARIANT;

    aStream.WriteInt8L(KVersion);
    // if older version adapt to changes (who knows if parameters would be also persistent...)

    aStream.WriteUint8L(iTypeId);
    switch (iTypeId)
        {
        case EVariantTypeTInt32: // FALLTHROUGH
        case EVariantTypeTUid:
        case EVariantTypeTBool:
        case EVariantTypeTUint:
            {
            aStream.WriteInt32L(iData.iInt32);
            break;
            }
        case EVariantTypeTInt64:
            {
            aStream.WriteReal64L(iData.iLong);
            break;
            }
        case EVariantTypeFileHandle: // FALLTHROUGH
        case EVariantTypeTTime:
        case EVariantTypeTReal:
            {
            iData.iInt64.ExternalizeL(aStream);
            break;
            }
        case EVariantTypeDesC:
            {
            aStream.WriteInt32L(iPtrC.Length());
            aStream << iPtrC;
            break;
            }
        case EVariantTypeDesC8:
            {
            aStream.WriteInt32L(iPtrC8.Length());
            aStream << iPtrC8;
            break;
            }
        case EVariantTypeList:  
            {
            iData.iList->ExternalizeL(aStream);
            break;
            }
        case EVariantTypeMap:  
            {
            iData.iMap->ExternalizeL(aStream);
            break;
            }
        case EVariantTypeInterface:  
            {
            _LIT(KMsg, "TLiwVariant: type Interface not externizable");
            User::Panic(KMsg, 1);
            break;
            }
        default:
            break;
        }

    __TEST_INVARIANT;
    }

TInt TLiwVariant::Size() const
    {
    __TEST_INVARIANT;

    TInt size = sizeof (TInt8); // version
    size += sizeof (TUint8);    // iTypeId
    switch (iTypeId)
        {
        case EVariantTypeTInt32:    // FALLTHROUGH
        case EVariantTypeTUid:
        case EVariantTypeTBool:
        case EVariantTypeTUint:
            {
            size += sizeof (TInt32);
            break;
            }
        case EVariantTypeFileHandle: // FALLTHROUGH
        case EVariantTypeTTime:
        case EVariantTypeTReal:
        case EVariantTypeTInt64:
            {
            size += sizeof (TInt64);
            break;
            }
        case EVariantTypeDesC:
            {
            size += sizeof (TInt32); // length
            //size += 1;              // the externalization method used adds a header byte
            if(iPtrC.Size() >= KBufSizeLarge)
            	{
            	size += 4;
            	}
            else if(iPtrC.Size() >= KBufSizeSmall)
            	{
            	size += 2;
            	}
            else
            	{
            	size += 1;	
            	}
            size += iPtrC.Size();
            break;
            }
        case EVariantTypeDesC8:
            {
            size += sizeof (TInt32); // length
            //size += 1;              // the externalization method used adds a header byte
            if(iPtrC8.Size() >= KBufSizeLarge)
            	{
            	size += 4;
            	}
                      	
            else if(iPtrC8.Size() >= KBufSizeSmall)
            	{
            	size += 2;
            	}
            else
            	{
            	size += 1;
            	}
            size += iPtrC8.Size();
            break;
            }
        case EVariantTypeList:  
            {
            size += iData.iList->Size();
            break;
            }
        case EVariantTypeMap:  
            {
            size += iData.iMap->Size();
            break;
            }
        default:
            break;
        }
    return size;
    }

// ============================ EXTERNAL FUNCTIONS ===============================

EXPORT_C TBool operator==(const TLiwVariant& aLhs, const TLiwVariant& aRhs)
    {
    if (aLhs.iTypeId == aRhs.iTypeId)
        {
        switch (aLhs.iTypeId)
            {
            case EVariantTypeNull:
                {
                // Null equals Null
                return ETrue;
                }
            case EVariantTypeTInt32:  // FALLTHROUGH
            case EVariantTypeTUid:
            case EVariantTypeTBool:
            case EVariantTypeTUint:
                {
                return (aLhs.iData.iInt32 == aRhs.iData.iInt32);
                }
            case EVariantTypeTInt64:
                {
                return (aLhs.iData.iLong == aRhs.iData.iLong);
                }
            case EVariantTypeDesC:
                {
                return (aLhs.iPtrC == aRhs.iPtrC);
                }
            case EVariantTypeDesC8:
                {
                return (aLhs.iPtrC8 == aRhs.iPtrC8);
                }
            case EVariantTypeFileHandle: // FALLTHROUGH
            case EVariantTypeTTime:
            case EVariantTypeTReal:
                {
                return (aLhs.iData.iInt64 == aRhs.iData.iInt64);
                }
            case EVariantTypeList:
                {
                return (*(aLhs.iData.iList) == *(aRhs.iData.iList));
                }
            case EVariantTypeMap:
                {
                return (*(aLhs.iData.iMap) == *(aRhs.iData.iMap));
                }
            case EVariantTypeInterface:
                {
                return (aLhs.iData.iSession == aRhs.iData.iSession);
                }
            case EVariantTypeIterable:
                {
                return (*(aLhs.iData.iIterable) == *(aRhs.iData.iIterable));
                }
            case EVariantTypeBuffer:
                {
                return (aLhs.iData.iBuffer == aRhs.iData.iBuffer);
                }                
            default:
                {
                break;
                }
            }
        }
    return EFalse;
    }

#ifdef _DEBUG
EXPORT_C void Dump(const TLiwVariant& aVariant)
    {
    switch (aVariant.TypeId())
        {
        case EVariantTypeNull:
            {
            RDebug::Print(_L("   TLiwVariant::Dump = Null"));
            break;
            }
        case EVariantTypeTInt32:
            {
            RDebug::Print(_L("   TLiwVariant::Dump(TInt32) = %d"), aVariant.AsTInt32());
            break;
            }
        case EVariantTypeTInt64:
            {
            RDebug::Print(_L("   TLiwVariant::Dump(TInt64) = %d"), aVariant.AsTInt64());
            break;
            }
        case EVariantTypeTReal:
            {
            RDebug::Print(_L("   TLiwVariant::Dump(TReal) = %d"), aVariant.AsTReal());
            break;
            }
        case EVariantTypeTBool:
            {
            RDebug::Print(_L("   TLiwVariant::Dump(TBool) = %d"), aVariant.AsTBool());
            break;
            }
        case EVariantTypeTUint:
            {
            RDebug::Print(_L("   TLiwVariant::Dump(TUint) = %d"), aVariant.AsTUint());
            break;
            }

        case EVariantTypeTUid:
            {
            const TUidName& uidName = aVariant.AsTUid().Name();
            RDebug::Print(_L("   TLiwVariant::Dump(TUid) = %S"), &uidName);
            break;
            }
        case EVariantTypeDesC:
            {
            TPtrC des = aVariant.AsDes();
            RDebug::Print(_L("   TLiwVariant::Dump(TBufC) = %S"), &des);
            break;
            }
        case EVariantTypeDesC8:
            {
            TPtrC8 des = aVariant.AsData();
            RDebug::Print(_L("   TLiwVariant::Dump(TBufC8) = %S"), &des);
            break;
            }
        case EVariantTypeTTime:
            {
            TDateTime dt = aVariant.AsTTime().DateTime();
            RDebug::Print(_L("   TLiwVariant::Dump(TTime): day=%d,mon=%d,year=%d,hh=%d,mm=%d,ss=%d"),
                          dt.Day()+1, dt.Month()+1, dt.Year(),
                          dt.Hour(),dt.Minute(), dt.Second());
            break;          
            }
        case EVariantTypeFileHandle:
            {
            RDebug::Print(_L("   TLiwVariant::Dump(RFile): Value is file handle."));
            break;
            }            
        case EVariantTypeList:  
            {
            const CLiwList* tempList = aVariant.AsList();
            TInt count = tempList->Count();
            TLiwVariant tempVariant;
            tempVariant.PushL();
            RDebug::Print(_L("   TLiwVariant::Dump(List[%d]):"), count);
            for (TInt index = 0; index < count; index++)
                {
                TRAPD(error, {tempList->AtL(index, tempVariant);});
                if(error != KErrNone)
                    RDebug::Print(_L("TLiwVariant::Dump ERROR: %d"), error);
                Dump(tempVariant);
                }
            CleanupStack::Pop(&tempVariant);
    		tempVariant.Reset();
            RDebug::Print(_L("   TLiwVariant::Dump List END"));
            break;
            }
        case EVariantTypeIterable:  
          {
            CLiwIterable* tempItr = aVariant.AsIterable();
            TLiwVariant tempVariant;
            RDebug::Print(_L("   TLiwVariant::Dump(Iterable):"));
            tempItr->Reset();
            TRAPD(errno, {
              while (tempItr->NextL(tempVariant)) {
                Dump(tempVariant);
              }
            });
            if(errno != KErrNone)
                RDebug::Print(_L("TLiwVariant::Dump ERROR: %d"), errno);
            tempVariant.Reset();
            RDebug::Print(_L("   TLiwVariant::Dump Iterable END"));
            break;
          }
        case EVariantTypeMap:  
          {
            const CLiwMap* tempMap = aVariant.AsMap();
            TInt count = tempMap->Count();
            TLiwVariant tempVariant;
            RDebug::Print(_L("   TLiwVariant::Dump(Map):"));
            for (TInt index = 0; index < count; index++) {
              TBuf8<KBufSizeMid> key;
              TRAPD(error, {tempMap->AtL(index, key);});
              RDebug::Print(_L("    key=%S, value= "), &key);
              TRAP(error, {tempMap->FindL(key, tempVariant);});
              Dump(tempVariant);
            }
            tempVariant.Reset();
            RDebug::Print(_L("   TLiwVariant::Dump Map END"));
            break;
          }
        case EVariantTypeInterface:  
            {
            RDebug::Print(_L("   TLiwVariant::Dump(Interface): Value is interface pointer."));
            break;
            }
        default:
            {
            RDebug::Print(_L("  *** TLiwVariant::Dump(Unknown) ***"));
            break;
            }
        }
    }
#else
EXPORT_C void Dump(const TLiwVariant& /*aVariant*/)
    {
    }
    
#endif // ifdef _DEBUG
