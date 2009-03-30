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
* Description:       Declares ECom monitoring class for LIW framework.
*
*/






#ifndef _LIWECOMMONITOR_H
#define _LIWECOMMONITOR_H

#include <ecom/ecom.h>

// FORWARD DECLARATIONS
class CLiwCriteriaItem;
class CLiwServiceIfBase;

class CLiwEcomMonitor : public CActive
    {
    public:
        static CLiwEcomMonitor* NewL(TCallBack& aSynchronizeCallBack);
        virtual ~CLiwEcomMonitor();

    protected:  // Functions from base classes
        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        */
        void DoCancel();

    public:
        inline REComSession& EComSession();
        void ListImplemetationsL(RImplInfoPtrArray& aResult, CLiwCriteriaItem* aItem) const;
        CLiwServiceIfBase* CreateImplementationL(TUid aImplUid);

    protected:
        /**
        * C++ default constructor.
        */
        CLiwEcomMonitor(TCallBack& aSynchronizeCallBack);
        void ConstructL();
        void StartMonitoring();

    private:
        TCallBack    iSynchronizeCallBack;
        REComSession iEcomSession;
    };


inline REComSession& CLiwEcomMonitor::EComSession()
    {
    return iEcomSession;
    }

#endif // _LIWECOMMONITOR_H

// End of file
