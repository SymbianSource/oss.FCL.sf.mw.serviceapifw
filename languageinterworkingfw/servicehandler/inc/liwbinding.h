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







#ifndef _LIW_BINDING_H
#define _LIW_BINDING_H

#include "liwcommon.h" 
#include "liwserviceifbase.h"

class CLiwBinding : public CBase
    {
    public:
        static CLiwBinding* NewL();
        static CLiwBinding* NewLC();

        inline virtual ~CLiwBinding();
        inline RPointerArray<CLiwCriteriaItem>& Interest(); 
        inline void AddCriteriaL(CLiwCriteriaItem* aItem);
        inline CLiwServiceIfBase* BaseProvider(TInt aIndex);
        inline TInt NumberOfProviders() const;
        void AddProviderL(CLiwServiceIfBase* aProvider, TBool aIsDefault);
        TBool RemoveProvider(TInt aImplUid);
        TInt HasCriteriaItem(CLiwCriteriaItem& aItem) const;
        void RemoveCriteria(TInt aIndex);
        TBool HasProvider(CLiwServiceIfBase* aProvider) const;
        TBool HasServiceCmd(TInt aServiceCmd) const;
    protected:
        inline CLiwBinding();
    
    private:
    
    protected:
        RPointerArray<CLiwCriteriaItem> iInterest;
        RPointerArray<CLiwServiceIfBase> iProviders;
    };


#include "liwbinding.inl"

#endif

// End of file
