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
* Description:       Implementation for ECom monitoring class for LIW framework.
*
*/






#include "liwecommonitor.h"
#include "liwcommon.h"
#include "liwuids.hrh"
#include "liwserviceifbase.h"
#include <LiwServiceHandler.h>

// CONSTANTS
_LIT8(KContentTag, "<CONTENT>");
_LIT8(KOpaqueTag, "<OPAQUE>");
_LIT8(KLiwMimeTypeAll, "*");
const TInt KMaxCmdLength = 238;
const TInt KMaxDataParamSize = 255;

CLiwEcomMonitor* CLiwEcomMonitor::NewL(TCallBack& aSynchronizeCallBack)
    {
    CLiwEcomMonitor* self = new (ELeave) CLiwEcomMonitor(aSynchronizeCallBack); 
    CleanupStack::PushL(self);  
    self->ConstructL();
    CleanupStack::Pop(self); // self
    return self;
    }


CLiwEcomMonitor::CLiwEcomMonitor(TCallBack& aSynchronizeCallBack) 
:CActive(CActive::EPriorityStandard), iSynchronizeCallBack(aSynchronizeCallBack)
    {
    }


CLiwEcomMonitor::~CLiwEcomMonitor()
    {
    Cancel();
    iEcomSession.Close();
    REComSession::FinalClose();
    }


void CLiwEcomMonitor::ConstructL()
    {
    iEcomSession = REComSession::OpenL();
    CActiveScheduler::Add(this);
    StartMonitoring();
    }

      
void CLiwEcomMonitor::RunL()
    {
    iSynchronizeCallBack.CallBack();
    StartMonitoring();
    }


    
void CLiwEcomMonitor::DoCancel()
    {
    iEcomSession.CancelNotifyOnChange(iStatus);
    }


void CLiwEcomMonitor::StartMonitoring()
    {
    iEcomSession.NotifyOnChange(iStatus);
    SetActive();
    }



void CLiwEcomMonitor::ListImplemetationsL(RImplInfoPtrArray& aResult, 
    CLiwCriteriaItem* aItem) const
    {
    TEComResolverParams params;
    
    TUid resolvUid = { KLiwResolverImplUidValue };  
    
    TBuf8<KMaxDataParamSize> dataType;     
    if((aItem->ServiceCmdStr().Length() + aItem->ContentType().Length()) > KMaxCmdLength)
        {        
        User::Leave( KLiwUnknown );
        }
    dataType.Copy(KContentTag);
    dataType.Append(aItem->ContentType());
    dataType.Append(KOpaqueTag);
    if (aItem->ServiceCmd() != KLiwCmdAsStr)
      {
        dataType.AppendNumUC(aItem->ServiceCmd(), EHex);
      }
    else
      {
        dataType.Append(aItem->ServiceCmdStr());
      }
    params.SetDataType(dataType);
    params.SetWildcardMatch(dataType.Find(KLiwMimeTypeAll) != KErrNotFound);
    
    iEcomSession.ListImplementationsL(aItem->ServiceClass(), params, resolvUid, aResult);
    }



CLiwServiceIfBase* CLiwEcomMonitor::CreateImplementationL(TUid aImplUid)
    {
    TUid dtorKeyId;

    CLiwServiceIfBase* iface = (CLiwServiceIfBase*)iEcomSession.CreateImplementationL(aImplUid, 
        dtorKeyId);

    if (iface)
        {
        iface->SetDtorKeyId( dtorKeyId );               
        iface->SetImplementationUid( aImplUid );
        }           
    
    return iface;
    }

// End of file
