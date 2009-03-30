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
* Description:       Inline methods for CLiwMenuBinding class.    
*
*/






inline void CLiwMenuBinding::SetMenuItemIndex(TInt aMenuItemIndex)
    {
    iMenuItemIndex = aMenuItemIndex;
    }


inline TInt CLiwMenuBinding::MenuItemIndex() const
    {
    return iMenuItemIndex;
    }


inline TInt CLiwMenuBinding::MenuCmd() const
    {
    return iMenuCmd;
    }


inline void CLiwMenuBinding::SetMenuCmd(TInt aMenuCmd)
    {
    iMenuCmd = aMenuCmd;
    }


inline TInt CLiwMenuBinding::MenuId()
    {
    return iMenuId;
    }


inline void CLiwMenuBinding::SetMenuId(TInt aId)
    {
    iMenuId = aId;
    }


inline CLiwServiceIfMenu* CLiwMenuBinding::MenuProvider(TInt aIndex)
    {
    if (aIndex >= 0 && aIndex < iProviders.Count())
        {
        return reinterpret_cast<CLiwServiceIfMenu*>(iProviders[aIndex]);
        }
    return NULL;
    }


inline void CLiwMenuBinding::SetMenuPane(CLiwMenuPane* aPane)
    {
    iMenuPane = aPane;
    }


inline CLiwMenuPane* CLiwMenuBinding::MenuPane()
    {
    return iMenuPane;
    }   


// End of file

