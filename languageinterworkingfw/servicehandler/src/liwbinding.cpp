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
* Description:       
*
*/






#include "liwbinding.h"


CLiwBinding* CLiwBinding::NewL()
    {
    return new (ELeave) CLiwBinding();
    }



CLiwBinding* CLiwBinding::NewLC()
    {
    CLiwBinding* bind = new (ELeave) CLiwBinding();
    CleanupStack::PushL(bind);
    return bind;
    }


void CLiwBinding::AddProviderL(CLiwServiceIfBase* aProvider, TBool aIsDefault)
    {
    if (aIsDefault)
        {
        // Default provider will be the first item in array.
        User::LeaveIfError(iProviders.Insert(aProvider, 0));
        }
    else
        {
        User::LeaveIfError(iProviders.Append(aProvider));
        }
    }



TInt CLiwBinding::HasCriteriaItem(CLiwCriteriaItem& aItem) const
    {
    const TInt count = iInterest.Count();
    for (TInt index = 0; index < count; index++)
        {
        if ((*iInterest[index]) == aItem)
            {
            return index;
            }
        }

    return KErrNotFound;
    }


TBool CLiwBinding::HasProvider(CLiwServiceIfBase* aProvider) const
    {
    for (TInt index = 0; index < iProviders.Count(); index++)
        {
        if (iProviders[index] == aProvider)
            {
            return ETrue;
            }
        }

    return EFalse;
    }


void CLiwBinding::RemoveCriteria(TInt aIndex)
    {
    iInterest.Remove( aIndex ); 
    }



TBool CLiwBinding::RemoveProvider(TInt aImplUid)
    {
    for (TInt index = 0; index < iProviders.Count(); index++)
        {
        if (iProviders[index]->ImplementationUid().iUid == aImplUid)
            {
            iProviders.Remove( index );
            return ETrue;
            }
        }

    return EFalse;
    }

TBool CLiwBinding::HasServiceCmd(TInt aServiceCmd) const    
    {
    for (TInt index = 0; index < iInterest.Count(); index++)
        {
        if(iInterest[index]->ServiceCmd() == aServiceCmd)
            {
            return ETrue;
            }
        }
    return EFalse;
    }
// End of file
