/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
#include "liwmenubinding.h"

CLiwMenuBinding* CLiwMenuBinding::NewL()
    {
    CLiwMenuBinding* bind = new (ELeave) CLiwMenuBinding();
    CleanupStack::PushL(bind);
    bind->ConstructL();
    CleanupStack::Pop(bind); // bind
    return bind;
    }


CLiwMenuBinding* CLiwMenuBinding::NewL(TInt aMenuItemIndex, TInt aParentId)
    {
    CLiwMenuBinding* bind = new (ELeave) CLiwMenuBinding(aMenuItemIndex, aParentId);
    CleanupStack::PushL(bind);
    bind->ConstructL();
    CleanupStack::Pop(bind); // bind
    return bind;
    }


CLiwMenuBinding* CLiwMenuBinding::NewLC()
    {
    CLiwMenuBinding* bind = new (ELeave) CLiwMenuBinding();
    CleanupStack::PushL(bind);
    bind->ConstructL();
    return bind;
    }


CLiwMenuBinding* CLiwMenuBinding::NewLC(TInt aMenuItemIndex, TInt aMenuId)
    {
    CLiwMenuBinding* bind = new (ELeave) CLiwMenuBinding(aMenuItemIndex, aMenuId);
    CleanupStack::PushL(bind);
    bind->ConstructL();
    return bind;
    }



CLiwMenuBinding::CLiwMenuBinding()
    {
    }


void CLiwMenuBinding::ConstructL()
    {
    }


CLiwMenuBinding::CLiwMenuBinding(TInt aMenuItemIndex, TInt aMenuId) 
:iMenuItemIndex(aMenuItemIndex), iMenuId(aMenuId)
    {
    // Nothing to do here.
    }



CLiwMenuBinding::~CLiwMenuBinding()
    {
    }


// End of file

