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
* Description:       Implements service API for providers offering menu services 
*                for consumers.
*
*/






#include <ecom/ecom.h>
#include "liwserviceifmenu.h"
#include "liwmenu.h"


void CLiwServiceIfMenu::InitializeMenuPaneHookL(
    CLiwMenuPane* aMenuPane,
    TInt aIndex,
    TInt aCascadeId,
    const CLiwGenericParamList& aInParamList)
    {
    iMenuPane = aMenuPane;  

    // Trapping prevents a leaving provider from breaking the whole consumer menu.  
    TRAPD(err, InitializeMenuPaneL(*aMenuPane, aIndex, aCascadeId, aInParamList));
    if(err)
        {
#ifdef _DEBUG
        RDebug::Print(_L("LIW PROVIDER ERROR: CLiwServiceIfMenu::InitializeMenuPaneL() failed, leave code:%d"), err);
#endif        
        }
    }



void CLiwServiceIfMenu::HandleMenuCmdHookL(
    CLiwMenuPane* aMenuPane,
    TInt aMenuCmdId,
    const CLiwGenericParamList& aInParamList,
    CLiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    const MLiwNotifyCallback* aCallback)
    {
    if (!iMenuPane)
        {
        return;
        }

    HandleMenuCmdL(aMenuPane->MenuCmdId(aMenuCmdId), aInParamList, aOutParamList,
        aCmdOptions, aCallback);
    }       


EXPORT_C const CLiwMenuPane* CLiwServiceIfMenu::MenuPane() const
    {
    return iMenuPane;
    }


EXPORT_C CLiwServiceIfMenu::~CLiwServiceIfMenu()
    {
    }
    
    
EXPORT_C void* CLiwServiceIfMenu::ExtensionInterface(TUid /*aInterface*/)
    {
    return NULL;
    }    

// End of file
