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






#include <ecom/ecom.h>
#include "liwserviceifbase.h"
#include "liwcommon.h"
#include "liwservicedata.h"

EXPORT_C CLiwServiceIfBase::~CLiwServiceIfBase()
    {
    if (iDtorKeyId.iUid)
        {
        REComSession::DestroyedImplementation(iDtorKeyId);
        }
    iCriterias.Close();
    
    if(iReserved)
    	delete (CLiwServiceData*)iReserved;
    }


void CLiwServiceIfBase::SetDtorKeyId(TUid aDtorKeyId)
    {
    iDtorKeyId = aDtorKeyId;
    }


TBool CLiwServiceIfBase::Match(CLiwCriteriaItem* aItem)
    {
    for (TInt index = 0; index < iCriterias.Count(); index++)
        {   
        if ((iCriterias[index]->ServiceClass() == aItem->ServiceClass()) &&
            (iCriterias[index]->ServiceCmd() == aItem->ServiceCmd()) &&
            (iCriterias[index]->ContentType() == aItem->ContentType()) &&
            (iCriterias[index]->Options() == aItem->Options()) &&
            (iCriterias[index]->DefaultProvider() == aItem->DefaultProvider()))
            {
            if (iCriterias[index]->ServiceCmd() == KLiwCmdAsStr)
                {
                return (iCriterias[index]->ServiceCmdStr() == aItem->ServiceCmdStr()) ? ETrue : EFalse;   
                }
              
            else
                {
                 return ETrue;    
                }
            }
        }

    return EFalse;
    }


void CLiwServiceIfBase::AddCriteria(CLiwCriteriaItem* aItem)
    {
    iCriterias.Append(aItem);
    }



TUid CLiwServiceIfBase::ImplementationUid() const
    {
    return iImplUid;
    }



void CLiwServiceIfBase::SetImplementationUid(TUid aUid)
    {
    iImplUid = aUid;
    }



TBool CLiwServiceIfBase::HasCriteria(CLiwCriteriaItem& aItem) const
    {
    TInt index;

    for (index = 0; index < iCriterias.Count(); index++)
        {
        if (aItem == *(iCriterias[index]))
            {
            return ETrue;
            }
        }

    return EFalse;
    }


EXPORT_C void* CLiwServiceIfBase::ExtensionInterface(TUid aInterface)
    {
    if(KLiwGetServiceDataIf==aInterface.iUid)
    {
    	return iReserved;
    }
    
    return NULL;
    }
// Enf of file
