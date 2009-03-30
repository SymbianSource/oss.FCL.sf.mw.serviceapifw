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
* Description:       Implements API for consumer application to access Language
*                Interworking Framework. 
*
*/






#include <eikenv.h>
#include <liwservicehandler.rsg>
#include "liwmenubinding.h"
#include "liwservicehandler.h"
#include "liwservicehandlerimpl.h"
#include "liwmenu.h"
#include "liwuids.hrh"
#include "liwcommon.hrh"
#include "liwtlsdata.h"


//
// LiwServiceHandler
//

EXPORT_C CLiwServiceHandler* CLiwServiceHandler::NewL()
    {
    CLiwServiceHandler* handler = new (ELeave) CLiwServiceHandler();
    CleanupStack::PushL( handler );
    handler->ConstructL();
    CleanupStack::Pop(handler); // handler
    return handler;
    }


EXPORT_C CLiwServiceHandler* CLiwServiceHandler::NewLC()
    {
    CLiwServiceHandler* handler = new (ELeave) CLiwServiceHandler();
    CleanupStack::PushL( handler );
    handler->ConstructL();
    return handler;     
    }


CLiwServiceHandler::CLiwServiceHandler()
    {
    // Nothing to do here.
    }


void CLiwServiceHandler::ConstructL()
    {
    iImpl = CLiwServiceHandlerImpl::NewL();
    }


EXPORT_C CLiwServiceHandler::~CLiwServiceHandler()
    {
    delete iImpl;
    }


EXPORT_C void CLiwServiceHandler::Reset()
    {
    iImpl->Reset();
    }


EXPORT_C TInt CLiwServiceHandler::NbrOfProviders(const CLiwCriteriaItem* aCriteria)
    {
    return iImpl->NbrOfProviders(aCriteria);
    }


EXPORT_C void CLiwServiceHandler::AttachL(TInt aInterestResourceId)
    {
    iImpl->AttachL(aInterestResourceId);
    }


EXPORT_C TInt CLiwServiceHandler::AttachL(const RCriteriaArray& aInterest)
    {
    return (iImpl->AttachL(aInterest,NULL));
    }

EXPORT_C TInt CLiwServiceHandler::AttachL(const RCriteriaArray& aInterest,CRTSecMgrScriptSession& aSecMgrScriptSession)
    {
    return (iImpl->AttachL(aInterest,&aSecMgrScriptSession));
    }

EXPORT_C void CLiwServiceHandler::GetInterest(RCriteriaArray& aInterest)
    {
    iImpl->GetInterest(aInterest);
    }
    
    
EXPORT_C void CLiwServiceHandler::DetachL(const RCriteriaArray& aInterest)
    {
    iImpl->DetachL(aInterest);
    }


EXPORT_C void CLiwServiceHandler::DetachL(TInt aInterestResourceId)
    {
    iImpl->DetachL(aInterestResourceId);
    }


EXPORT_C const CLiwCriteriaItem* CLiwServiceHandler::GetCriteria(TInt aId)
    {
    return iImpl->GetCriteria(aId);
    }


EXPORT_C void CLiwServiceHandler::InitializeMenuPaneL(
    CEikMenuPane& aMenuPane,
    TInt aMenuResourceId,
    TInt aBaseMenuCmdId,
    const CLiwGenericParamList& aInParamList)
    {
    iImpl->InitializeMenuPaneL(aMenuPane, aMenuResourceId, aBaseMenuCmdId, aInParamList);   
    }
    
EXPORT_C void CLiwServiceHandler::InitializeMenuPaneL(
    CEikMenuPane& aMenuPane,
    TInt aMenuResourceId,
    TInt aBaseMenuCmdId,
    const CLiwGenericParamList& aInParamList,
    TBool aUseSubmenuTextsIfAvailable)
    {
    iImpl->InitializeMenuPaneL(aMenuPane, 
        aMenuResourceId, 
        aBaseMenuCmdId, 
        aInParamList, 
        aUseSubmenuTextsIfAvailable);
    }    

    
EXPORT_C TInt CLiwServiceHandler::ServiceCmdByMenuCmd(TInt aMenuCmdId) const
    {
    return iImpl->ServiceCmdByMenuCmd(aMenuCmdId);
    }


EXPORT_C void CLiwServiceHandler::ExecuteMenuCmdL(
    TInt aMenuCmdId,
    const CLiwGenericParamList& aInParamList,
    CLiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    MLiwNotifyCallback* aCallback)
    {
    iImpl->ExecuteMenuCmdL(aMenuCmdId, aInParamList, aOutParamList, aCmdOptions, aCallback);
    }


EXPORT_C void CLiwServiceHandler::AttachMenuL(TInt aMenuResourceId, TInt aInterestResourceId)
    {
    iImpl->AttachMenuL(aMenuResourceId, aInterestResourceId);
    }


EXPORT_C void CLiwServiceHandler::AttachMenuL(TInt aMenuResourceId, TResourceReader& aReader)
    {
    iImpl->AttachMenuL(aMenuResourceId, aReader);
    }
    
EXPORT_C void CLiwServiceHandler::AttachMenuL(TInt aMenuResourceId, const RCriteriaArray& aInterest)
    {
    iImpl->AttachMenuL(aMenuResourceId, aInterest);
    }  

EXPORT_C void CLiwServiceHandler::AttachMenuL(RArray<TInt>& aMenuEntries, TInt aMenuResourceId,
                                              RCriteriaArray& aInterest)
    {
    iImpl->AttachMenuL(aMenuEntries, aMenuResourceId, aInterest);
    }

EXPORT_C void CLiwServiceHandler::DetachMenu(TInt aMenuResourceId, TInt aInterestResourceId)
    {
    iImpl->DetachMenu(aMenuResourceId, aInterestResourceId);
    }


EXPORT_C TBool CLiwServiceHandler::IsSubMenuEmpty(TInt aSubMenuId)
    {
    return iImpl->IsSubMenuEmpty(aSubMenuId);
    }


EXPORT_C void CLiwServiceHandler::ExecuteServiceCmdL(
    const TInt& aCmdId,
    const CLiwGenericParamList& aInParamList,
    CLiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    MLiwNotifyCallback* aCallback)
    {
    iImpl->ExecuteServiceCmdL(aCmdId, aInParamList, aOutParamList, aCmdOptions, aCallback);
    }

EXPORT_C void CLiwServiceHandler::ExecuteServiceCmdL(
    const CLiwCriteriaItem& aCmd,
    const CLiwGenericParamList& aInParamList,
    CLiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    MLiwNotifyCallback* aCallback)
    {
    iImpl->ExecuteServiceCmdL(aCmd, aInParamList, aOutParamList, aCmdOptions, aCallback);
    }

EXPORT_C CLiwGenericParamList& CLiwServiceHandler::InParamListL()
    {
    return iImpl->InParamListL();
    }


EXPORT_C CLiwGenericParamList& CLiwServiceHandler::OutParamListL()
    {
    return iImpl->OutParamListL();
    }


EXPORT_C TBool CLiwServiceHandler::IsLiwMenu(TInt aMenuResourceId)
    {
    return iImpl->IsLiwMenu(aMenuResourceId);
    }


EXPORT_C TBool CLiwServiceHandler::HandleSubmenuL(CEikMenuPane& aPane)
    {
    return iImpl->HandleSubmenuL(aPane);
    }
    

EXPORT_C TInt CLiwServiceHandler::MenuCmdId(TInt aMenuCmdId) const
    {
    return iImpl->MenuCmdId(aMenuCmdId);
    }


EXPORT_C void CLiwServiceHandler::ReportMenuLaunch()
    {
    CLiwTlsData* data = CLiwTlsData::Instance();
    if(data)
        {
        data->ReportMenuLaunch();
        }
    }


EXPORT_C void CLiwServiceHandler::QueryImplementationL(RCriteriaArray& aFilterItem, RCriteriaArray& aProviderList)
	{
		for(TInt itemIndex = 0; itemIndex < aFilterItem.Count(); ++itemIndex)
		{
			iImpl->QueryImplementationL(aFilterItem[itemIndex], aProviderList);
		}
			
	}
// End of file
