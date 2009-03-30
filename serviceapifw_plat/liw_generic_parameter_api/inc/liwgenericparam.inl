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
* Description:       Inline function implementations of class TLiwGenericParam.
*
*/







inline TLiwGenericParam::TLiwGenericParam() :
    iSemanticId(LIW::EGenericParamUnspecified), iValue(), iReserved(NULL)
    {
    }

inline TLiwGenericParam::TLiwGenericParam(LIW::TGenericParamId aSemanticId) :
    iSemanticId(aSemanticId), iValue(), iReserved(NULL)
    {
    }

inline TLiwGenericParam::TLiwGenericParam(LIW::TGenericParamId aSemanticId, 
    const TLiwVariant& aValue) : 
    iSemanticId(aSemanticId), iValue(aValue), iReserved(NULL)
    {
    }


inline TLiwGenericParam::TLiwGenericParam(const TDesC8& aName, const TLiwVariant& aValue)
    {
    iSemanticId = LIW::EGenericParamNameAsStringTransient;
    iReserved = (TAny*)&aName;
    iValue = aValue;
    }

inline void TLiwGenericParam::SetSemanticId(LIW::TGenericParamId aSemanticId)
    {
    iSemanticId = aSemanticId;
    }

inline void TLiwGenericParam::SetNameL(const TDesC8& aName)
    {
    iSemanticId = LIW::EGenericParamNameAsString;
    delete iReserved;
    iReserved = (TAny*)aName.AllocL();
    }

inline void TLiwGenericParam::SetNameAndValueL(const TDesC8& aName, const TLiwVariant& aValue)
    {
    delete iReserved;
    iValue.Reset();
    iSemanticId = LIW::EGenericParamNameAsString;
    iReserved = (TAny*)aName.AllocL();
    iValue = aValue;
    }

inline LIW::TGenericParamId TLiwGenericParam::SemanticId() const
    {
    return iSemanticId;
    }

inline const TDesC8& TLiwGenericParam::Name() const
    {
    if (iSemanticId == LIW::EGenericParamNameAsStringTransient)
       return *((TDesC8*)iReserved);
    else
       return ((iReserved) ? *((HBufC8*)iReserved) : KNullDesC8());
    }

inline TLiwVariant& TLiwGenericParam::Value()
    {
    return iValue;
    }

inline const TLiwVariant& TLiwGenericParam::Value() const
    {
    return iValue;
    }

inline void TLiwGenericParam::Reset()
    {
    if (iSemanticId != LIW::EGenericParamNameAsStringTransient) delete iReserved;
    iReserved = NULL;
    iSemanticId = LIW::EGenericParamUnspecified;
    iValue.Reset();
    }

inline TBool operator!=(const TLiwGenericParam& aLhs, 
    const TLiwGenericParam& aRhs)
    {
    return !(aLhs == aRhs);
    }

// End of file

