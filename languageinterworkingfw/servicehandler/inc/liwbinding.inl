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
* Description:       Criteria binding base class declaration.
*
*/







inline CLiwBinding::~CLiwBinding()
    {
    iInterest.Reset();
    iProviders.Reset();
    }


inline CLiwBinding::CLiwBinding()
    {
    }


inline RPointerArray<CLiwCriteriaItem>& CLiwBinding::Interest()
    {
    return iInterest;
    }


inline void CLiwBinding::AddCriteriaL(CLiwCriteriaItem* aItem)
    {
    User::LeaveIfError(iInterest.Append(aItem));
    }


inline CLiwServiceIfBase* CLiwBinding::BaseProvider(TInt aIndex)
    {
    if (aIndex < iProviders.Count())
        {
        return iProviders[aIndex];
        }
    return NULL;
    }

    
inline TInt CLiwBinding::NumberOfProviders() const
    {
    return iProviders.Count();
    }

// End of file
