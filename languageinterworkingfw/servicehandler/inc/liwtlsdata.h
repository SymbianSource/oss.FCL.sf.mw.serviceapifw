/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:       TLS data object.
*
*/







#ifndef _LIW_TLS_DATA_H
#define _LIW_TLS_DATA_H

#include <e32base.h>

class MLiwMenuLaunchObserver
    {
    public:
        virtual void MenuLaunched() = 0;
    };

NONSHARABLE_CLASS( CLiwTlsData ) : public CBase
	{
	public:
		static CLiwTlsData* OpenL();
		static void Close();
		
		inline static CLiwTlsData* Instance()
		    {
		    return static_cast<CLiwTlsData*>( Dll::Tls() );
		    }

        void AddMenuLaunchObserverL( MLiwMenuLaunchObserver* aObserver );
        void RemoveMenuLaunchObserver( MLiwMenuLaunchObserver* aObserver );
	
	    // CEikMenuPane uses this to inform LIW when a menu is launched.
	    void ReportMenuLaunch();
	
	private:
	    CLiwTlsData();
	    virtual ~CLiwTlsData();
	    
	    static CLiwTlsData* NewL();
	    void ConstructL();
	
	private:
	    TInt iRefCount;
		RPointerArray<MLiwMenuLaunchObserver> iMenuLaunchObservers;
	};

#endif // _LIW_TLS_DATA_H

// End of file
