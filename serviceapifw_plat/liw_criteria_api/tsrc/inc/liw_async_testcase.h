/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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






#ifndef LIWTC_ASYNC_H
#define LIWTC_ASYNC_H

// INCLUDES
#include <liwcommon.h>
#include <e32base.h>
#include <LiwGenericParam.h>

// FORWARD DECLARATIONS
class CLiwServiceHandler;

// CLASS DECLARATION

/**
*  CLiwTC2 class.
* 
*/
class CLiw_AsyncTC : public CActive, public MLiwNotifyCallback/*, public MLiwTest*/
    {
    public:
        enum TRequestType {ECount, EStop=1000} ;
    
    public: // Constructors and destructor
        /**
        * Symbian default constructor.
        */
       // void ConstructL(CLiwTestLogHandler& aLogger, CLiwTestAppContainer& aContainer);

        /**
        * Constructor.
        */
        CLiw_AsyncTC( CLiwServiceHandler *iServiceHandler, TRequestType aRequest = ECount, TInt aPriority = EPriorityStandard);
                 
        /**
        * Destructor.
        */
        ~CLiw_AsyncTC();
        
        TUid Id() const;

    public:
         void DoCancel();
         void RunL();
         void RequestFunction( TRequestType aRequest );
         void ReadyToGo();
         void SetIncrementCounter( TInt aCount = 1 );
         TUint CountValue () const;
         TRequestType iRequest;
         TInt iCmdID;
         
    	TInt HandleNotifyL(
            TInt aCmdId,
            TInt aEventId,
            CLiwGenericParamList& aEventParamList,
            const CLiwGenericParamList& aInParamList);

    public: // MLiwTest
        TInt AsyncCase(TInt aTcNo);
        
		/*Test Items*/ 
        void LIW_ASYNC_001();
        void LIW_ASYNC_002();
        void LIW_ASYNC_003(TInt aEventState=0);
        void LIW_ASYNC_004();
       
    private: // Data
        CLiwServiceHandler* iServiceHandler;
        TInt iTransID;
        TInt iAsyncCount;
        TInt tcNo;
        TInt iResult;
        TUint iCount;
        TUint iIncrementCount;
       
    };

#endif

// End of File
