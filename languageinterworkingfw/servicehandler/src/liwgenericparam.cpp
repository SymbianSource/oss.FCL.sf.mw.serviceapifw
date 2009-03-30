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
* Description:       Implementation of TLiwGenericParam and CLiwGenericParamList.
*
*/







// INCLUDES
#include <s32mem.h>
#include "liwgenericparam.h"
using namespace LIW;
namespace {

// CONSTANTS
// Protect against possible data format changes
const TInt KVersion = 10;  // Version 1.0

enum TPanicCode
    {
    EPanicPostCond_CopyLC = 1,
    EPanicPostCond_AppendL,
    EPanicPostCond_Reset
    };


// ============================= LOCAL FUNCTIONS ===============================

#ifdef _DEBUG
void Panic(TPanicCode aCode)
    {
    _LIT(KPanicText, "GenericParam");
    User::Panic(KPanicText, aCode);
    }
#endif

}


const TInt KRFsSlot = 2;
const TInt KRFileSlot = 3;


// ============================ MEMBER FUNCTIONS ===============================

/**
 * Releases any dynamic data owned by this parameter.
 */
void TLiwGenericParam::Destroy()
    {
    if (iSemanticId != EGenericParamNameAsStringTransient)
        {
        delete iReserved;
        iReserved = NULL;   
        }
    iValue.Destroy();
    }

/**
 * Copies aParam to this parameter. Takes an own copy of the value held in 
 * aParam.
 */
void TLiwGenericParam::CopyLC(const TLiwGenericParam& aParam)
    {
    // Take a copy of value
    iValue.SetL(aParam.iValue);
    // Copy semantic id
    iSemanticId = aParam.iSemanticId;
    // Copy name  
    if (iSemanticId == EGenericParamNameAsString) {
      delete iReserved;
      iReserved = NULL;
      iReserved = ((HBufC8*)aParam.iReserved)->AllocL();
    }
    else if (iSemanticId == EGenericParamNameAsStringTransient) {
      delete iReserved;
      iReserved = NULL;
      iReserved = ((TDesC8*)aParam.iReserved)->AllocL();
      iSemanticId = EGenericParamNameAsString;
    }
    CleanupStack::PushL(*this);

    //__ASSERT_DEBUG(*this==aParam, Panic(EPanicPostCond_CopyLC));
    }


EXPORT_C void TLiwGenericParam::PushL()
	{
	CleanupStack::PushL( TCleanupItem( TLiwGenericParam::ParamCleanup , this) ); 
	}

EXPORT_C void TLiwGenericParam::ParamCleanup( TAny* aObj )
    {
    static_cast<TLiwGenericParam*>(aObj)->Destroy();
    }

/**
 * Returns a cleanup item that will call Destroy to this object when 
 * PopAndDestroy'ed.
 */
TLiwGenericParam::operator TCleanupItem()
    {
    return TCleanupItem(&CleanupDestroy,this);
    }

/**
 * Callback for operator TCleanupItem().
 */
void TLiwGenericParam::CleanupDestroy(TAny* aObj)
    {
    static_cast<TLiwGenericParam*>(aObj)->Destroy();
    }
/**
 * Internalizes this parameter from aStream.
 */
void TLiwGenericParam::InternalizeL(RReadStream& aStream)
    {
    /*const TInt version =*/ aStream.ReadInt8L();
    const TGenericParamId id = 
        static_cast<TGenericParamId>(aStream.ReadInt32L());
    if (id == EGenericParamNameAsString)
      {
        TInt len = aStream.ReadInt32L();
        delete iReserved;
        iReserved = NULL;
        iReserved = HBufC8::NewL(aStream, len);
      }
    iValue.InternalizeL(aStream);
    iSemanticId = id;
    }

/**
 * Externalizes this parameter to aStream.
 */
void TLiwGenericParam::ExternalizeL(RWriteStream& aStream) const
    {
    aStream.WriteInt8L(KVersion);
    aStream.WriteInt32L(iSemanticId);
    if (iSemanticId == EGenericParamNameAsString)
      {
        aStream.WriteInt32L(((HBufC8*)iReserved)->Length());
        aStream << (*((HBufC8*)iReserved));
      }
    iValue.ExternalizeL(aStream);
    }

/**
 * Returns the maximum externalized size of this parameter.
 */
TInt TLiwGenericParam::Size() const
    {
    TInt size = sizeof (TInt8); // version
    size += sizeof (TInt32); // semantic ID
    if (iSemanticId == EGenericParamNameAsString)
      {
        size += sizeof (TInt32); // name length
        //size += 1;   // header written by operator<<
        if(((HBufC8*)iReserved)->Size()>=64)
        	{
           	size += 2;
        	}
        else
        	{
        	size += 1;
           	}
        	
        size += ((HBufC8*)iReserved)->Size();
      }
    size += iValue.Size();
    return size;
    }

EXPORT_C TBool operator==(const TLiwGenericParam& aLhs, 
    const TLiwGenericParam& aRhs)
    {
      if (((aLhs.SemanticId() == EGenericParamNameAsString) ||
           (aLhs.SemanticId() == EGenericParamNameAsStringTransient)) &&
          ((aRhs.SemanticId() == EGenericParamNameAsString) ||
           (aRhs.SemanticId() == EGenericParamNameAsStringTransient)))
        return ((aLhs.Name() == aRhs.Name()) && (aLhs.Value() == aRhs.Value()));
      else
        return 
          ((aLhs.SemanticId() == aRhs.SemanticId()) && 
           (aLhs.Value() == aRhs.Value()));
    }


// CLiwGenericParamList

inline CLiwGenericParamList::CLiwGenericParamList()
    {
    }

EXPORT_C CLiwGenericParamList* CLiwGenericParamList::NewL()
    {
    CLiwGenericParamList *self = NewLC();
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CLiwGenericParamList* CLiwGenericParamList::NewL(RReadStream& aReadStream)
    {
    CLiwGenericParamList *self = NewLC(aReadStream);
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CLiwGenericParamList* CLiwGenericParamList::NewLC()
    {
    CLiwGenericParamList *self = new( ELeave ) CLiwGenericParamList();
    CleanupStack::PushL( self );
    return self;
    }

EXPORT_C CLiwGenericParamList* CLiwGenericParamList::NewLC(RReadStream& aReadStream)
    {
    CLiwGenericParamList *self = new( ELeave ) CLiwGenericParamList();
    CleanupStack::PushL( self );
    self->AppendL(aReadStream);
    return self;
    }

CLiwGenericParamList::~CLiwGenericParamList()
    {
    for (TInt index = iParameters.Count()-1; index >= 0; --index)
        {
        // Can use Destroy because the parameters cannot be accessed anymore
        iParameters[index].Destroy();
        }
    iParameters.Close();
    }

EXPORT_C TInt CLiwGenericParamList::Count() const
    {
    return iParameters.Count();
    }
    
EXPORT_C TInt CLiwGenericParamList::Count(TGenericParamId aSemanticId, 
    TVariantTypeId aDataType) const
    {
    TInt result = 0;
    const TInt count = iParameters.Count();

    for (TInt index = 0; index < count; index++)
        {
        const TLiwGenericParam& param = iParameters[index];
        if (param.SemanticId() == aSemanticId)
            {
            if (aDataType==EVariantTypeAny || param.Value().TypeId()==aDataType)
                {
                result++;                   
                }
            }
        }       
    return result;     
    }


EXPORT_C const TLiwGenericParam& CLiwGenericParamList::operator[](TInt aIndex) const
    {
        return iParameters[aIndex]; 
    }

EXPORT_C void CLiwGenericParamList::AtL(TInt aIndex, TLiwGenericParam& aParam) const
    {
    aParam.CopyLC(iParameters[aIndex]); 
    CleanupStack::Pop(&aParam);   
    }

EXPORT_C void CLiwGenericParamList::AppendL(const TLiwGenericParam& aParam)
    {
    TLiwGenericParam newParam;
    newParam.CopyLC(aParam);
    User::LeaveIfError(iParameters.Append(newParam));
    CleanupStack::Pop(&newParam);

    //__ASSERT_DEBUG((*this)[Count()-1]==aParam, Panic(EPanicPostCond_AppendL));
    }


EXPORT_C TBool CLiwGenericParamList::Remove(TInt aSemanticId)
    {
    const TInt count = iParameters.Count();
    for (TInt index = 0; index < count; ++index)
        {
        const TLiwGenericParam& param = iParameters[index];
        if (param.SemanticId() == aSemanticId)
            {
            iParameters.Remove(index);
            return ETrue;
            }
        }

    return EFalse;
    }

EXPORT_C void CLiwGenericParamList::Remove(const TDesC8& aName)
    {
    TInt start = 0;
    FindFirst(start, aName);
    while (start != KErrNotFound)
        {
        // Can use Destroy because the parameters cannot be accessed anymore
        iParameters[start].Destroy();
        iParameters.Remove(start);
        start = 0;   
        FindFirst(start, aName);
        }
    }
    
EXPORT_C void CLiwGenericParamList::Reset()
    {
    for (TInt index = iParameters.Count()-1; index >= 0; --index)
        {
        // Can use Destroy because the parameters cannot be accessed anymore
        iParameters[index].Destroy();
        iParameters.Remove(index);
        }

    __ASSERT_DEBUG(Count()==0, Panic(EPanicPostCond_Reset));
    }

EXPORT_C const TLiwGenericParam* CLiwGenericParamList::FindFirst(
    TInt &aIndex, 
    TGenericParamId aSemanticId, 
    TVariantTypeId aDataType) const
    {
    if (aIndex >= 0)
        {
        const TInt count = iParameters.Count();
        for (TInt index = aIndex; index < count; ++index)
            {
            const TLiwGenericParam& param = iParameters[index];
            if (param.SemanticId() == aSemanticId)
                {
                if (aDataType==EVariantTypeAny || param.Value().TypeId()==aDataType)
                    {
                    aIndex = index;
                    return &param;
                    }
                }
            }
        }
    aIndex = KErrNotFound;
    return NULL;
    }

EXPORT_C const TLiwGenericParam* CLiwGenericParamList::FindNext(
    TInt &aIndex, 
    TGenericParamId aSemanticId,
    TVariantTypeId aDataType) const
    {
    if (aIndex < 0)
        {
        aIndex = KErrNotFound;
        return NULL;
        }

    TInt index = aIndex+1;
    const TLiwGenericParam* result = FindFirst(index, aSemanticId, aDataType);
    aIndex = index;
    return result;
    }

EXPORT_C const TLiwGenericParam* CLiwGenericParamList::FindNext(
    TInt &aIndex, 
    const TDesC8& aName,
    TVariantTypeId aDataType) const
    {
    if (aIndex < 0)
        {
        aIndex = KErrNotFound;
        return NULL;
        }

    TInt index = aIndex+1;
    const TLiwGenericParam* result = FindFirst(index, aName, aDataType);
    aIndex = index;
    return result;
    }

EXPORT_C const TLiwGenericParam* CLiwGenericParamList::FindFirst 
        (TInt &aIndex, const TDesC8& aName, 
        TVariantTypeId aDataType) const
    {
    if (aIndex >= 0)
        {
        const TInt count = iParameters.Count();
        for (TInt index = aIndex; index < count; ++index)
            {
            const TLiwGenericParam& param = iParameters[index];
            if ((param.SemanticId() == EGenericParamNameAsString) &&
                (param.Name() == aName))
                {
                if (aDataType==EVariantTypeAny || param.Value().TypeId()==aDataType)
                    {
                    aIndex = index;
                    return &param;
                    }
                }
            }
        }
    aIndex = KErrNotFound;
    return NULL;
    }

EXPORT_C void CLiwGenericParamList::ExternalizeL(RWriteStream& aStream) const
    {
    aStream.WriteInt8L(KVersion);
    const TInt count = iParameters.Count();
    aStream.WriteInt32L(count);
    for (TInt index = 0; index < count; ++index)
        {
        iParameters[index].ExternalizeL(aStream);
        }
    }

EXPORT_C TInt CLiwGenericParamList::Size() const
    {
    TInt size = sizeof (TInt8); // version
    size += sizeof (TInt32);  // param count
    const TInt count = iParameters.Count();
    for (TInt index = 0; index < count; ++index)
        {
        size += iParameters[index].Size();
        }
    return size;
    }

/**
 * Appends parameters to this list from aStream.
 */
void CLiwGenericParamList::AppendL(RReadStream& aReadStream)
    {
    /*const TInt version =*/ aReadStream.ReadInt8L();
    
    TInt count = aReadStream.ReadInt32L();
    while (count--)
        {
        TLiwGenericParam param;
        CleanupStack::PushL(param);
        param.InternalizeL(aReadStream);
        User::LeaveIfError(iParameters.Append(param));
        CleanupStack::Pop(&param);
        }
    }



EXPORT_C void CLiwGenericParamList::InternalizeL(RReadStream& aStream)
    {
    AppendL(aStream);
    }


EXPORT_C HBufC8* CLiwGenericParamList::PackForServerL(TIpcArgs& aArgs)
    {   
    TInt index;
    TInt rfileCount = 0;
    RFile fhandle;

    HBufC8* buf = HBufC8::NewLC(Size() + iParameters.Count());
    TPtr8 des = buf->Des();
    RDesWriteStream outstrm(des);    
    CleanupClosePushL(outstrm);

    outstrm.WriteInt16L(iParameters.Count());   

    for (index = 0; index < iParameters.Count(); index++)
        {       
        if (iParameters[index].Value().TypeId() == EVariantTypeFileHandle)
            {
            if (rfileCount == 0)
                {
                outstrm.WriteInt8L(iParameters[index].Value().TypeId());
                outstrm.WriteInt32L(iParameters[index].SemanticId());
                if (iParameters[index].Value().Get(fhandle))
                    {
                    rfileCount++;
                    }
                }
            }
        else
            {
            outstrm.WriteInt8L(iParameters[index].Value().TypeId());
            iParameters[index].ExternalizeL( outstrm );
            }
        }
        
    outstrm.CommitL();  
    CleanupStack::PopAndDestroy(&outstrm);
                        
    aArgs.Set(0, buf);
    aArgs.Set(1, des.MaxLength());    
        
    if (rfileCount)
        {
        fhandle.TransferToServer(aArgs, KRFsSlot, KRFileSlot);      
        }   

    CleanupStack::Pop(buf);    // buf

    return buf;
    }


EXPORT_C void CLiwGenericParamList::UnpackFromClientL(const RMessage2& aMsg)
    {
    TInt8 type;
    TInt index;
    TLiwGenericParam param; 
    TGenericParamId handleId = EGenericParamUnspecified;
    TInt numFileHandles = 0;

    HBufC8* buf = HBufC8::NewLC(aMsg.Int1());
    TPtr8 ptr(buf->Des());
    aMsg.ReadL(0, ptr);
    RDesReadStream instrm(buf->Des());

    const TInt16 count = instrm.ReadInt16L();
    for (index = 0; index < count; index++)
        {
        type = instrm.ReadInt8L();
        if (type == EVariantTypeFileHandle)
            {
            if (numFileHandles == 0)
                {
                handleId = (TGenericParamId)instrm.ReadInt32L();
                numFileHandles++;
                }
            }
        else
            {
            param.InternalizeL(instrm);
            AppendL(param);
            param.Reset();
            }
        }

    if (numFileHandles)
        {
        RFile file;
        file.AdoptFromClient(aMsg, KRFsSlot, KRFileSlot);
        TLiwVariant variant(file);
        AppendL(TLiwGenericParam(handleId, variant));               
        }
        
    CleanupStack::PopAndDestroy(buf); // buf       
    }
    

EXPORT_C void CLiwGenericParamList::AppendL(const CLiwGenericParamList& aList)
    {
    for (TInt index = 0; index < aList.iParameters.Count(); index++)
        {
        AppendL(aList.iParameters[index]);
        }
    }

// End of file
