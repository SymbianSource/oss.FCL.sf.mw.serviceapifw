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
* Description:       Inline function implementations of class TLiwVariant.
*
*/








inline TLiwVariant::TLiwVariant() : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    iData.iBufC = NULL;
    iData.iBufC8 = NULL;
    Reset();
    }

inline TLiwVariant::TLiwVariant(TInt32 aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {   
    Set(aValue);
    }

inline TLiwVariant::TLiwVariant(TInt64 aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {   
    Set(aValue);
    }

inline TLiwVariant::TLiwVariant(TBool aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {   
    Set(aValue);
    }
    
inline TLiwVariant::TLiwVariant(TUint aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {   
    Set(aValue);
    }
inline TLiwVariant::TLiwVariant(TReal aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {   
    Set(aValue);
    }
inline TLiwVariant::TLiwVariant(const CLiwBuffer* aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {   
    Set(aValue);
    }
        
inline TLiwVariant::TLiwVariant(const TUid& aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    Set(aValue);
    }

inline TLiwVariant::TLiwVariant(const TDesC& aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    iData.iBufC = NULL;
    Set(aValue);
    }

inline TLiwVariant::TLiwVariant(const HBufC* aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    iData.iBufC = NULL;
    if(aValue)
        Set(*aValue);
    else
        Reset();    
    }

inline TLiwVariant::TLiwVariant(const TTime& aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    Set(aValue);
    }

inline TLiwVariant::TLiwVariant(const TDesC8& aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    iData.iBufC8 = NULL;
    Set(aValue);
    }


inline TLiwVariant::TLiwVariant(const RFile& aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    Set(aValue);
    }


inline TLiwVariant::TLiwVariant(const CLiwList* aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    Set(aValue);        
    }                   

inline TLiwVariant::TLiwVariant(const CLiwMap* aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    Set(aValue);        
    }
                   
inline TLiwVariant::TLiwVariant(const MLiwInterface* aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    Set(aValue);        
    }

inline TLiwVariant::TLiwVariant(const CLiwIterable* aValue) : iTypeId(LIW::EVariantTypeNull), iPtrC(), iPtrC8()
    {
    Set(aValue);        
    }                   

inline LIW::TVariantTypeId TLiwVariant::TypeId() const
    {
    return static_cast<LIW::TVariantTypeId>(iTypeId);
    }

inline TBool TLiwVariant::IsEmpty() const
    {
    return (iTypeId == LIW::EVariantTypeNull);
    }

inline void TLiwVariant::Set(const HBufC* aValue)
    {
    Set(*aValue);
    }

inline TLiwVariant& TLiwVariant::operator=(TInt32 aValue)
    {
    Set(aValue);
    return *this;
    }

inline TLiwVariant& TLiwVariant::operator=(TInt64 aValue)
    {
    Set(aValue);
    return *this;
    }
    
inline TLiwVariant& TLiwVariant::operator=(TReal aValue)
    {
    Set(aValue);
    return *this;
    }
inline TLiwVariant& TLiwVariant::operator=(const CLiwBuffer* aValue)
    {
    Set(aValue);
    return *this;
    }
    
inline TLiwVariant& TLiwVariant::operator=(TBool aValue)
    {
    Set(aValue);
    return *this;
    }

inline TLiwVariant& TLiwVariant::operator=(TUint aValue)
    {
    Set(aValue);
    return *this;
    }

inline TLiwVariant& TLiwVariant::operator=(const TUid& aValue)
    {
    Set(aValue);
    return *this;
    }

inline TLiwVariant& TLiwVariant::operator=(const TDesC& aValue)
    {
    Set(aValue);
    return *this;
    }

inline TLiwVariant& TLiwVariant::operator=(const HBufC* aValue)
    {
    Set(*aValue);
    return *this;
    }

inline TLiwVariant& TLiwVariant::operator=(const TTime& aValue)
    {
    Set(aValue);
    return *this;
    }


inline TLiwVariant& TLiwVariant::operator=(const TDesC8& aValue)
    {
    Set(aValue);    
    return *this;
    }


inline TLiwVariant& TLiwVariant::operator=(const RFile& aValue)
    {
    Set(aValue);
    return *this;
    }


inline TLiwVariant& TLiwVariant::operator=(const CLiwList* aValue)
    {
    Set(aValue);
    return *this;
    }

inline TLiwVariant& TLiwVariant::operator=(const CLiwMap* aValue)
    {
    Set(aValue);
    return *this;
    }

inline TLiwVariant& TLiwVariant::operator=(const MLiwInterface* aValue)
    {
    Set(aValue);
    return *this;
    }

inline TLiwVariant& TLiwVariant::operator=(const CLiwIterable* aValue)
    {
    Set(aValue);
    return *this;
    }

inline TBool operator!=(const TLiwVariant& aLhs, const TLiwVariant& aRhs)
    {
    return !(aLhs == aRhs);
    }


inline void CLiwContainer::IncRef()
    {
    iRefCount++;
    }
    
inline void CLiwContainer::DecRef()
    {
    if (--iRefCount == 0)
        delete this;
    }
// End of file
