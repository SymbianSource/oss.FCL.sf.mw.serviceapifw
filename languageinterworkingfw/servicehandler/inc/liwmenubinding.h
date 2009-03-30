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
* Description:       Declares LIW Menu utilities for providers to access consumer menu.
*
*/






#ifndef _LIW_MENUBINDING_H
#define _LIW_MENUBINDING_H

#include "liwserviceifmenu.h"
#include "liwbinding.h"

/**
* This utility class is used for binding a set of providers to a menu placeholder.
*/
NONSHARABLE_CLASS(CLiwMenuBinding) : public CLiwBinding
    {
    public:
        static CLiwMenuBinding* NewL();
        static CLiwMenuBinding* NewL(TInt aMenuItemIndex, TInt aMenuId);
        static CLiwMenuBinding* NewLC();
        static CLiwMenuBinding* NewLC(TInt aMenuItemIndex, TInt aMenuId);

        virtual ~CLiwMenuBinding();

        inline void SetMenuItemIndex(TInt aMenuItemIndex);
        inline TInt MenuItemIndex() const;
        inline CLiwServiceIfMenu* MenuProvider(TInt aIndex);
        inline TInt MenuCmd() const;
        inline void SetMenuCmd(TInt aMenuCmd);
        inline TInt MenuId();
        inline void SetMenuId(TInt aId);
        inline void SetMenuPane(CLiwMenuPane* aPane);
        inline CLiwMenuPane* MenuPane();
        
    private:
        CLiwMenuBinding();
        CLiwMenuBinding(TInt aMenItemIndex, TInt aParentId);
        void ConstructL();

    private:
        TInt iMenuItemIndex;
        TInt iMenuCmd;      
        TInt iMenuId;
        CLiwMenuPane* iMenuPane;
    };

#include "liwmenubinding.inl"

#endif // _LIW_MENUBINDING_H

// End of file
