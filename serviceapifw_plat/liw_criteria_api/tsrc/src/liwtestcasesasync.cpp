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







// INCLUDE FILES
#include  <aknViewAppUi.h>
#include  <liwservicehandler.h>
#include  "liw_async_testcase.h"
//#include  "liwtestapp.hrh"
//#include  <liwtestapp.rsg>

TInt success;

using namespace LIW;

//_LIT8(KAsyncService,"S60.MultiMetaData");
//_LIT8(KSyncService,"S60.SyncServices");
//_LIT8(KInvalidService,"S60.InvalidService");
//_LIT8(KSecurityService,"S60.SecurityServices");
//_LIT8(KInvalidSecurityService,"S60.InvalidSecurityServices");

_LIT8(KAsyncSrv,"Test.AsyncServices");
_LIT8(KAsyncContent,"Test/Data");

//_LIT8(KTestPARAMNanmeTC2,"TestResult");
//_LIT8(KSecurityParameter,"SecurityParam");

//_LIT8(KResult_Pass, "Test Provider replied with <tt>EGenericParamError = KErrorNone</tt>");
//_LIT8(KResult_Fail,"Test Provider replied with Error : <tt>KErrNotFound</tt>");

//_LIT8(KAsyn_TestCmt, "These Test cases are designed to test LIW support for Asynchronous Invocation.\
                          <br><br><b>Test:-<b></br>\
                          <li>Attaching and Detaching service providers through string based service command</li>\
                          <li>Execute the service with callback</li>\
                          <li>After checking the event state, next Test Case is invoked</li>\
                          <br><b>Check - </b></br> \
                          Appropriate State of the event in the HandleNotify Method");
       
TInt IncomingEventCmdID=1000;
TInt XmlDataReaderCmdID=2000;

const TInt32 LIWASYNC001=100;
//const TInt32 LIWASYNC002=200;
const TInt32 LIWASYNC003=300;
const TInt32 LIWASYNC004=400;
//const TInt32 LIWASYNC005=500;
//const TInt32 LIWASYNC006=600;

const TInt32 KAsyncMaxCount=10;

const TUid KViewId4 = {4};

// ---------------------------------------------------------
// TUid CLiwtcbase::Id()
// ?implementation_description
// ---------------------------------------------------------
//
TUid CLiw_AsyncTC::Id() const
    {
    return KViewId4;
    }

//Async event states. New state could be added
enum TAsyncState
{
	eAsyncStarted=0,
	eAsyncStopped
};

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CLiw_AsyncTC::CLiw_AsyncTC
// Description: CLiw_AsyncTC constructer
// -----------------------------------------------------------------------------
//
CLiw_AsyncTC::CLiw_AsyncTC( CLiwServiceHandler *aServiceHandler, TRequestType aRequest, TInt aPriority): CActive(aPriority)
    {
    iCount = 0;
    iResult = 1;
    iIncrementCount = 1; 
    iRequest = aRequest;
    iServiceHandler = aServiceHandler;
    iAsyncCount = 0;
    tcNo = -1;
    CActiveScheduler::Add( this );
    }

TInt CLiw_AsyncTC::AsyncCase(TInt aTcNo)
{
    tcNo = aTcNo;
    //setactive, user::requestcomplete
        
     SetActive();
    
     iStatus=KRequestPending;
     TRequestStatus* temp=&iStatus;
     User::RequestComplete(temp,KErrNone);
     
     //wait
     CActiveScheduler::Start();
     
     return iResult;
}

//
//-----------------------------------------------------------------------------
// CLiw_AsyncTC::RequestFunction
// Description: Request Function for CLiw_AsyncTC
// ----------------------------------------------------------------------------
//
void CLiw_AsyncTC::RequestFunction( TRequestType aRequest )
{
 iRequest = aRequest;
 iStatus = KRequestPending;
 if( !IsActive() )
    SetActive();
}

//
//-----------------------------------------------------------------------------
// CLiw_AsyncTC::ReadyToGo
// Description: This calls CLiw_AsyncTC's RunL
//-----------------------------------------------------------------------------
//
void CLiw_AsyncTC::ReadyToGo()
{
 TRequestStatus* status = &iStatus;
 User::RequestComplete( status, KErrNone );
}


//
//-----------------------------------------------------------------------------
// CLiw_AsyncTC::DoCancel
// Description: This calls CActive's Cancel
//-----------------------------------------------------------------------------
//
void CLiw_AsyncTC::DoCancel()
{
}

//
//-----------------------------------------------------------------------------
// CLiw_AsyncTC::SetIncrementCounter
// Description: Sets Increment Counter of CLiw_AsyncTC
//-----------------------------------------------------------------------------
//
void CLiw_AsyncTC::SetIncrementCounter( TInt aCount)
{
iIncrementCount = aCount;
}

//
//-----------------------------------------------------------------------------
// CLiw_AsyncTC::CountValue
// Description: Returns Count Value of CLiw_AsyncTC
//-----------------------------------------------------------------------------
//
TUint CLiw_AsyncTC::CountValue () const
{
return iCount;
}

// ---------------------------------------------------------
// CLiw_AsyncTC::ConstructL(const TRect& aRect)
// EPOC two-phased constructor
// ---------------------------------------------------------
//
/*void CLiw_AsyncTC::ConstructL(CLiwTestLogHandler& aLogger, CLiwTestAppContainer& aContainer)
    {
     iLogger = &aLogger;
    iContainer = &aContainer;
    
    // Create service handler instance.   
    CLiwServiceHandler* serviceHandler = CLiwServiceHandler::NewLC(); 
    iServiceHandler = serviceHandler;    
    CleanupStack::Pop(serviceHandler);
    }
*/

// ---------------------------------------------------------
// CLiw_AsyncTC::~CLiw_AsyncTC()
// ?implementation_description
// ---------------------------------------------------------
//
CLiw_AsyncTC::~CLiw_AsyncTC()
    {
    delete iServiceHandler;
    iServiceHandler = NULL;
    
    Cancel();
    }

void CLiw_AsyncTC::RunL()
    {
        switch(tcNo)
        {
        case 1:
            LIW_ASYNC_001();
            break;
        case 2:
            LIW_ASYNC_002();
            break;
        case 3:
            LIW_ASYNC_003();
            break;
        case 4:
            LIW_ASYNC_004();
            break;
        default:
            break;
        }
    }


// ---------------------------------------------------------
// CLiw_AsyncTC::HandleCommandL
// From MLiwNotifyCallback
// ---------------------------------------------------------
//   
TInt CLiw_AsyncTC::HandleNotifyL(
    TInt aCmdId,
    TInt aEventId,
    CLiwGenericParamList& /*aEventParamList*/,
    const CLiwGenericParamList& /*aInParamList*/)
    {
    
    if(LIWASYNC001==aCmdId)
    {
    	if( (KLiwEventCompleted==aEventId) || 
    		(KLiwEventCanceled==aEventId) || 
    		(KLiwEventStopped==aEventId) ||
    		(KLiwEventError==aEventId) )
    	{
	    	//_LIT8(testCmt,"<b>LIW_ASYNC_001:</b> <br> Create a CLiwCriteriaItem with string command \
	    	    The provider with the same command must be attached with LIW FW.\
	            and call <tt>ExecuteServiceCmdL</tt>.</br>\
	            <br><b>Test -</b></br> <li>Attaching provider Programmatically</li> <li>string based serviceName</li>\
	            <br><b>Check - </b></br> Provider returns asynchronously : <tt>LiwEventCompleted</tt>");
	
			if(KLiwEventCompleted==aEventId)
			{
			    iResult = 0;
	            //iLogger->AddTestItem(testCmt,KResult_Pass,ETrue, EFalse);
			    //Venkat : return true as testcase passed
	        }
			else
            {
                iResult = 1;
                //iLogger->AddTestItem(testCmt,KResult_Fail,ETrue, EFalse);
                //Venkat : Test fails
    		}

            CActiveScheduler::Stop();
			//LIW_ASYNC_002();
    	}
    }
    else if(LIWASYNC003==aCmdId)
    {
    	//if the counter reaches 
    	//event notification should be cancelled
    	++iAsyncCount;
    	
    	if( (KLiwEventCompleted==aEventId) || 
    		(KLiwEventCanceled==aEventId) || 
    		(KLiwEventStopped==aEventId) ||
    		(KLiwEventError==aEventId) )
    	{
    		//_LIT8(testCmt,"<b>LIW_ASYNC_003:</b> <br> Create a CLiwCriteriaItem with string command \
                The provider with the same command must be attached with LIW FW.\
                and call <tt>ExecuteServiceCmdL</tt>.</br>\
                <br><b>Test -</b></br> <li>Attaching provider Programmatically</li> <li>string based serviceName</li>\
                <br><b>Check - </b></br> Provider returns asynchronously : <tt>LiwEventStopped</tt>");
  
    		if(KLiwEventStopped==aEventId)
    		{
    		    iResult = 0;
                //iLogger->AddTestItem(testCmt,KResult_Pass,ETrue, EFalse);
            }
            else
            {
                iResult = 1;
                //iLogger->AddTestItem(testCmt,KResult_Fail,EFalse, EFalse);
    		}
    			
    		//LIW_ASYNC_003((TInt32)eAsyncStopped);
    		iAsyncCount=0;    		
    		iTransID=0;
    		
    		return KErrNone;
    	}
    	    	
    	if(iAsyncCount >= KAsyncMaxCount)
    	{
    		LIW_ASYNC_003((TInt32)eAsyncStopped);
    		iAsyncCount=0;
    		iTransID=0;
    		//LIW_ASYNC_004();

            CActiveScheduler::Stop();
    	}
    }
    else if(LIWASYNC004==aCmdId)
    {
    	if( (KLiwEventCanceled==aEventId) || 
    		(KLiwEventStopped==aEventId) ||
    		(KLiwEventError==aEventId) )
    	{
	    	//_LIT8(testCmt,"<b>LIW_ASYNC_004:</b> <br> Create a CLiwCriteriaItem with string command \
	            The provider with the same command must be attached with LIW FW.\
	            and call <tt>ExecuteServiceCmdL</tt>.</br>\
	            <br><b>Test -</b></br> <li>Attaching provider Programmatically</li> <li>string based serviceName</li>\
	            <br><b>Check - </b></br> Provider returns asynchronously : <tt>LiwEventError</tt>");
	    	
	    	if(KLiwEventError==aEventId)
			{
			    iResult = 0;
				//iLogger->AddTestItem(testCmt,KResult_Pass,ETrue, EFalse);
			}
	    	else
	    	{
	    	    iResult = 1;
		        //iLogger->AddTestItem(testCmt,KResult_Fail,EFalse, EFalse);
	    	}

            CActiveScheduler::Stop();
    	}
    }
    return KErrNone;    
    }

// ---------------------------------------------------------
/*LIW Asynchronous service Test Cases are Defined here*/
// ---------------------------------------------------------

void CLiw_AsyncTC::LIW_ASYNC_004()
{

	/*Detached all the service provider and reset all the Entries of previous test case*/
    iServiceHandler->Reset();
    
    RCriteriaArray interest;
    CleanupClosePushL(interest);
    
    const TDesC8* cmd = &KAsyncSrv;
    
    //Define Criteria Item Dynamically with string based command 
    CLiwCriteriaItem* criteria1 = CLiwCriteriaItem::NewLC(1, *cmd, KAsyncContent);
    criteria1->SetServiceClass(TUid::Uid(KLiwClassBase));
    interest.AppendL(criteria1);
   
    /*Attach the base provider*/
	iServiceHandler->AttachL(interest);
	CleanupStack::PopAndDestroy(criteria1);
    CleanupStack::PopAndDestroy(&interest);

	TLiwGenericParam asyncSrvCmdIDParam;
	TLiwGenericParam asyncStateParam;
	TLiwGenericParam asyncTransIDParam;
	TLiwGenericParam testCaseIDParam;

	_LIT8(KAsyncCmdID, "AsyncCmdID");
//	_LIT8(KAsyncCmdName, "IncomingEvents");
	_LIT8(KAsyncEventState, "AsyncEventState");
	//_LIT8(KAsyncTransID, "TransactionID");
	_LIT8(KTestCaseID, "TestCaseID");
	
	//_LIT8(KLIW_ASYNC_001, "LIW_ASYNC_001");
	TInt32 testCaseID=LIWASYNC004;

	CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
	inps->Reset();

	asyncSrvCmdIDParam.SetNameAndValueL(KAsyncCmdID, TLiwVariant((TInt32)XmlDataReaderCmdID));
	asyncStateParam.SetNameAndValueL(KAsyncEventState, TLiwVariant((TInt32)eAsyncStarted));
	testCaseIDParam.SetNameAndValueL(KTestCaseID, TLiwVariant((TInt32)testCaseID));
		
	inps->AppendL(asyncSrvCmdIDParam);
	inps->AppendL(asyncStateParam);	            		  		
	inps->AppendL(testCaseIDParam);
	      		
	CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, *cmd, KAsyncContent);
	crit->SetServiceClass(TUid::Uid(KLiwClassBase));

	CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());

	iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps,KLiwOptASyncronous,this); 
	
	asyncSrvCmdIDParam.Reset();
	asyncStateParam.Reset();
	asyncTransIDParam.Reset();
	testCaseIDParam.Reset();
	CleanupStack::PopAndDestroy(crit); // crit
}

void CLiw_AsyncTC::LIW_ASYNC_003(TInt aEventState)
{

	const TDesC8* cmd = &KAsyncSrv;

	if(eAsyncStarted==aEventState)
	{
		/*Detached all the service provider and reset all the Entries of previous test case*/
	    iServiceHandler->Reset();
	    
	    RCriteriaArray interest;
	    CleanupClosePushL(interest);
	    
	    
	    
	    //Define Criteria Item Dynamically with string based command 
	    CLiwCriteriaItem* criteria1 = CLiwCriteriaItem::NewLC(1, *cmd, KAsyncContent);
	    criteria1->SetServiceClass(TUid::Uid(KLiwClassBase));
	    interest.AppendL(criteria1);
	   
	    /*Attach the base provider*/
		iServiceHandler->AttachL(interest);
		CleanupStack::PopAndDestroy(criteria1);
	    CleanupStack::PopAndDestroy(&interest);
	}
	

	TLiwGenericParam asyncSrvCmdIDParam;
	TLiwGenericParam asyncStateParam;
	TLiwGenericParam asyncTransIDParam;
	TLiwGenericParam testCaseIDParam;

	_LIT8(KAsyncCmdID, "AsyncCmdID");
//	_LIT8(KAsyncCmdName, "IncomingEvents");
	_LIT8(KAsyncEventState, "AsyncEventState");
	//_LIT8(KAsyncTransID, "TransactionID");
	_LIT8(KTestCaseID, "TestCaseID");
	
	TInt32 testCaseID=LIWASYNC003;

	CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
	inps->Reset();

	asyncSrvCmdIDParam.SetNameAndValueL(KAsyncCmdID, TLiwVariant((TInt32)IncomingEventCmdID));
	asyncStateParam.SetNameAndValueL(KAsyncEventState, TLiwVariant((TInt32)aEventState));
	testCaseIDParam.SetNameAndValueL(KTestCaseID, TLiwVariant((TInt32)testCaseID));
		
	inps->AppendL(asyncSrvCmdIDParam);
	inps->AppendL(asyncStateParam);	            		  		
	inps->AppendL(testCaseIDParam);

	if(eAsyncStopped==aEventState)
	{
		TLiwGenericParam asyncTransIDParam;					
		_LIT8(KAsyncTransID, "TransactionID");
		asyncTransIDParam.SetNameAndValueL(KAsyncTransID, TLiwVariant((TInt32)iTransID));					
		inps->AppendL(asyncTransIDParam);
		asyncTransIDParam.Reset();
	}
	      		
	CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, *cmd, KAsyncContent);
	crit->SetServiceClass(TUid::Uid(KLiwClassBase));

	CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());

	iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps,KLiwOptASyncronous,this); 
	
	asyncSrvCmdIDParam.Reset();
	asyncStateParam.Reset();
	asyncTransIDParam.Reset();
	testCaseIDParam.Reset();
	CleanupStack::PopAndDestroy(crit); // crit
	
	if(eAsyncStarted==aEventState)
	{
		TInt pos = 0;
		_LIT8(KAsyncTransID, "TransactionID");
		outps->FindFirst(pos, KAsyncTransID);

		TLiwGenericParam transIDParam = outps->operator[](pos);
		iTransID = transIDParam.Value().AsTInt32();
	}

	//LIW_ASYNC_004();
}

void CLiw_AsyncTC::LIW_ASYNC_002()
{

	/*Detached all the service provider and reset all the Entries of previous test case*/
    iServiceHandler->Reset();
    
    RCriteriaArray interest;
    CleanupClosePushL(interest);
    
    const TDesC8* cmd = &KAsyncSrv;
    
    //Define Criteria Item Dynamically with string based command 
    CLiwCriteriaItem* criteria1 = CLiwCriteriaItem::NewLC(1, *cmd, KAsyncContent);
    criteria1->SetServiceClass(TUid::Uid(KLiwClassBase));
    interest.AppendL(criteria1);
   
    /*Attach the base provider*/
	iServiceHandler->AttachL(interest);
	CleanupStack::PopAndDestroy(criteria1);
    CleanupStack::PopAndDestroy(&interest);

	TLiwGenericParam asyncSrvCmdIDParam;
	TLiwGenericParam asyncStateParam;
	TLiwGenericParam asyncTransIDParam;
	TLiwGenericParam testCaseIDParam;

	_LIT8(KAsyncCmdID, "AsyncCmdID");
//	_LIT8(KAsyncCmdName, "IncomingEvents");
	_LIT8(KAsyncEventState, "AsyncEventState");
	//_LIT8(KAsyncTransID, "TransactionID");
	_LIT8(KTestCaseID, "TestCaseID");
	
	//_LIT8(KLIW_ASYNC_001, "LIW_ASYNC_001");
	TInt32 testCaseID=LIWASYNC001;

	CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
	inps->Reset();

	asyncSrvCmdIDParam.SetNameAndValueL(KAsyncCmdID, TLiwVariant((TInt32)XmlDataReaderCmdID));
	asyncStateParam.SetNameAndValueL(KAsyncEventState, TLiwVariant((TInt32)eAsyncStarted));
	testCaseIDParam.SetNameAndValueL(KTestCaseID, TLiwVariant((TInt32)testCaseID));
		
	inps->AppendL(asyncSrvCmdIDParam);
	inps->AppendL(asyncStateParam);	            		  		
	inps->AppendL(testCaseIDParam);
	      		
	CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, *cmd, KAsyncContent);
	crit->SetServiceClass(TUid::Uid(KLiwClassBase));

	CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());

	iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps,KLiwOptASyncronous); 
	
	//Since there is no callback, the control returns here
	//Checking for the success of this testcase
	
	TInt pos=0;
	
	outps->FindFirst(pos, EGenericParamError);
	    
    if(pos != KErrNotFound)
    {
		//_LIT8(testCmt,"<b>LIW_ASYNC_001:</b> <br> Create a CLiwCriteriaItem with string command \
	    The provider with the same command must be attached with LIW FW.\
        and call <tt>ExecuteServiceCmdL</tt>.</br>\
        <br><b>Test -</b></br> <li>Attaching provider Programmatically</li> <li>string based serviceName</li>\
        <li>No CallBack is mentioned</li><br><b>Check - </b></br>\
        Provider returns control immediately (Synchronous) <tt>EGenericParamError = KErrCancel</tt>");

		if ((*outps)[pos].Value().AsTInt32() == KErrCancel)
		{
		    iResult = 0;
		    //iLogger->AddTestItem(testCmt,KResult_Pass,ETrue, EFalse);	            
        }
		else
        {
            iResult = 1;
            //iLogger->AddTestItem(testCmt,KResult_Fail,ETrue, EFalse);
		}	
	}	
	asyncSrvCmdIDParam.Reset();
	asyncStateParam.Reset();
	asyncTransIDParam.Reset();
	testCaseIDParam.Reset();
	CleanupStack::PopAndDestroy(crit); // crit
	
	CActiveScheduler::Stop();
	//LIW_ASYNC_003();
}


void CLiw_AsyncTC::LIW_ASYNC_001()
{
    /*Detached all the service provider and reset all the Entries of previous test case*/
    iServiceHandler->Reset();
    
    RCriteriaArray interest;
    CleanupClosePushL(interest);
    
    //Define Criteria Item Dynamically with string based command 
    CLiwCriteriaItem* criteria1 = CLiwCriteriaItem::NewLC(1, KAsyncSrv, KAsyncContent);
    criteria1->SetServiceClass(TUid::Uid(KLiwClassBase));
    interest.AppendL(criteria1);
    
    /*Attach the base provider*/
    TInt status = iServiceHandler->AttachL(interest);
    CleanupStack::PopAndDestroy(criteria1);
    CleanupStack::PopAndDestroy(&interest);
    
    TLiwGenericParam asyncSrvCmdIDParam;
    TLiwGenericParam asyncStateParam;
    TLiwGenericParam asyncTransIDParam;
    TLiwGenericParam testCaseIDParam;
    
    _LIT8(KAsyncCmdID, "AsyncCmdID");
    //_LIT8(KAsyncCmdName, "IncomingEvents");
    _LIT8(KAsyncEventState, "AsyncEventState");
    //_LIT8(KAsyncTransID, "TransactionID");
    _LIT8(KTestCaseID, "TestCaseID");
    
    //_LIT8(KLIW_ASYNC_001, "LIW_ASYNC_001");
    TInt32 testCaseID=LIWASYNC001;
    
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    inps->Reset();
    
    asyncSrvCmdIDParam.SetNameAndValueL(KAsyncCmdID, TLiwVariant((TInt32)XmlDataReaderCmdID));
    asyncStateParam.SetNameAndValueL(KAsyncEventState, TLiwVariant((TInt32)eAsyncStarted));
    testCaseIDParam.SetNameAndValueL(KTestCaseID, TLiwVariant((TInt32)testCaseID));
        
    inps->AppendL(asyncSrvCmdIDParam);
    inps->AppendL(asyncStateParam);                             
    inps->AppendL(testCaseIDParam);
                
    CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KAsyncSrv, KAsyncContent);
    crit->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
    
    iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps,KLiwOptASyncronous,this); 
    
    asyncSrvCmdIDParam.Reset();
    asyncStateParam.Reset();
    asyncTransIDParam.Reset();
    testCaseIDParam.Reset();
    
    CleanupStack::PopAndDestroy(crit); // crit
}
