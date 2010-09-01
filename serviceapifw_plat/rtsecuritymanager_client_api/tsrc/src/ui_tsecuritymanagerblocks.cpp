/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:        ?Description
*
*/








#include <badesca.h>
#include <bautils.h>
// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "ui_tsecuritymanager.h"
#include<coemain.h>

#include <e32base.h>
#include <e32std.h>
#include <f32file.h>
#include <rtsecmanager.h>
#include <rtsecmgrutility.h>
#include <rtsecmgrscriptsession.h>
#include <rtsecmgrcommondef.h>
//#include <rtsecmgrclient.h>
#include<w32std.h>
_LIT(KSecMgrServerExeName, "RTSecMgrServer.exe");
_LIT(KServerStartFailed, "Security manager server starting failed");
_LIT(KSecServerProcessName, "SecurityManagerServer");
const TUint KDefaultMessageSlots= 16;
const TUint KRTSecMgrServMajorVersionNumber=0;
const TUint KRTSecMgrServMinorVersionNumber=1;
const TUint KRTSecMgrServBuildVersionNumber=1;



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cui_tsecuritymanager::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "serverstart1", Cui_tsecuritymanager::serverstart1 ),
         ENTRY( "serverstart2", Cui_tsecuritymanager::serverstart2 ),
         ENTRY( "secclient1", Cui_tsecuritymanager::secclient1 ),
          ENTRY( "secclient2", Cui_tsecuritymanager::secclient2 ),
		 ENTRY( "secclient3",Cui_tsecuritymanager::secclient3),  
 		 ENTRY( "secclient4",Cui_tsecuritymanager::secclient4), 
   			ENTRY( "secclient5",Cui_tsecuritymanager::secclient5),
   		 ENTRY( "secclient6",Cui_tsecuritymanager::secclient6),
   		  ENTRY( "secclient7",Cui_tsecuritymanager::secclient7), 
   		   ENTRY( "secclient8",Cui_tsecuritymanager::secclient8),
   		          ENTRY( "secclient9",Cui_tsecuritymanager::secclient9), 
   		     ENTRY( "secclient10",Cui_tsecuritymanager::secclient10), 
   		 ENTRY( "secclient11",Cui_tsecuritymanager::secclient11),   
   		 	 ENTRY( "secclient12",Cui_tsecuritymanager::secclient12),   
   		 	 ENTRY( "secclient13",Cui_tsecuritymanager::secclient13), 
  
        // [test cases entries] - Do not remove

        };
    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::serverstart1
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::serverstart1( CStifItemParser& aItem )
    {
    __UHEAP_MARK;

    	TInt retry(2); // Try this twice
	TInt err(KErrNone);
 
 	while (retry>KErrNone)
		{
		// Try to create a Server session
		err = CreateSession ( KSecServerProcessName, Version (),
				KDefaultMessageSlots);

		if ( err != KErrNotFound && err != KErrServerTerminated)
			{
			// KErrNone or unrecoverable error
			if ( err != KErrNone)
				{
			}
			retry = 0;
			}
		else
			{
			// Return code was KErrNotFound or KErrServerTerminated.
			// Try to start a new security manager server instance
		
			err = StartSecManagerServer ();
			
			
 
			if ( err != KErrNone && err != KErrAlreadyExists)
				{
				// Unrecoverable error

				retry = 0;
				}
			}

		retry--;
		}
		
		
//	__UHEAP_MARKEND;	
	 
	
   
    return KErrNone;

    }
    
    
   TVersion Cui_tsecuritymanager::Version(void) const
	{
	return (TVersion(KRTSecMgrServMajorVersionNumber,KRTSecMgrServMinorVersionNumber,KRTSecMgrServBuildVersionNumber));
	}
 
 
 
 TInt Cui_tsecuritymanager::StartSecManagerServer() const
	{
	
	RProcess server ,server1;
	const TUidType serverUid( KNullUid, KSecMgrServerUid2, KNullUid);
	TInt err = server.Create ( ServerLocation (), 
			KNullDesC, 
			serverUid, 
			EOwnerProcess);
			
		TInt err1 = server1.Create ( ServerLocation (), 
			KNullDesC, 
			serverUid, 
			EOwnerProcess);

	// Return error code if we the process couldn't be created
	if ( KErrNone == err)
		{
		// Rendezvous is used to detect server start
		TRequestStatus status;
		server.Rendezvous ( status);
	
		if ( status != KRequestPending)
			{
 			
			server.Kill ( 0); // Abort startup
			}
		else
			{
			server.Resume (); // Logon OK - start the server
			}
		User::WaitForRequest (status); // Wait for start or death

		if ( server.ExitType ()== EExitPanic)
			{
 
			err = KErrGeneral;
			}
		else
			{
			err = status.Int ();
			}

		// We can close the handle now
		server.Close ();
		
		}
	return err;
	}
  
  
  TFullName Cui_tsecuritymanager::ServerLocation() const
	{
	TFullName fullPathAndName;
	fullPathAndName.Append ( KSecMgrServerExeName);
	return fullPathAndName;
	}  
	
	
	
	
	
	
	
	
	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::serverstart1
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::serverstart2( CStifItemParser& aItem )
    {
    __UHEAP_MARK;
CRTSecMgrClientProxy  *proxy;
    RSecMgrSession *isecmgrsession;
   // Connect();
	//	proxy->ConstructL();
		
//	__UHEAP_MARKEND;	
	 
	
   
    return KErrNone;

    }
	
	
	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient1
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient1( CStifItemParser& aItem )
    {
    
    
    TInt cnt = User::CountAllocCells();
   // __UHEAP_MARK;
    TInt32 exId = 0;
    TPtrC sptr;
    	
    	 aItem.GetNextString(sptr);
    	 
    	iSession = CRTSecManager::NewL ();
    //		iSession2 = CRTSecManager::NewL ();
	iTrust = CTrustInfo::NewL ();
    
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
		  
		  	  
		  	TBuf8<5000>  aFilebuffer ;
				 
				 
				 
			 
				 
				 aItem.GetNextString(sptr);
				 
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				
				
				CleanupClosePushL (secPolicyFile);
				
			
					
				 if(!sptr.Compare(_L("File")))
				 {
				 	
				  
				
				iPolicyId = iSession->SetPolicy (secPolicyFile);
				 }
				 if(!sptr.Compare(_L("Buffer")))
				 {
				if(KErrNone == secPolicyFile.Read(aFilebuffer))
				{
							
				iPolicyId = iSession->SetPolicy (aFilebuffer);
				}
				 }
				 
				
				
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
		 aItem.GetNextString(sptr);
		 
		 if(!sptr.Compare(_L("nohash")))
			{
	
	
	 exId = iSession->RegisterScript (iPolicyId, *iTrust);
			}
			
			if(!sptr.Compare(_L("hash")))
			{
	
		_LIT(KHash,"sdsds");
		TBuf<10> aHash;
		aHash.Copy(_L("sdsds"));
		  exId = iSession->RegisterScript(iPolicyId,aHash,*iTrust);
 
			}
			
			
	
	CRTSecMgrScriptSession* scriptSession;
 
	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);	



//#endif			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
 	RCapabilityArray caps;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
					
 
 
   
 	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close(); 

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 	
 	//	TInt result = iSession->UnSetPolicy (iPolicyId);		

 
	
 	
 

 

//delete scriptSession1;
delete iSession;
//delete iSession2;
delete iTrust;

  cnt = User::CountAllocCells();
//__UHEAP_MARKEND;
return KErrNone;
}


	
	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient2
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient2( CStifItemParser& aItem )
    {
    
    __UHEAP_MARK;
    
    	iSession = CRTSecManager::NewL ();
    		iSession2 = CRTSecManager::NewL (); // 2nd session
				iTrust = CTrustInfo::NewL ();
    
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
			
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				CleanupClosePushL (secPolicyFile);
			
				iPolicyId = iSession->SetPolicy (secPolicyFile);
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
	TInt32 exId = iSession2->RegisterScript (iPolicyId, *iTrust);
	
	CRTSecMgrScriptSession* scriptSession;

//#ifdef AUTO_TESTING
//	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId, *iTrust, this);
//#else
	
	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);
//#endif			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
		RCapabilityArray caps;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
					
 
 
   
 	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close();

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 	
 		TInt result = iSession->UnSetPolicy (iPolicyId);		

 
	
 	
 

 

//delete scriptSession1;
delete iSession;
delete iSession2;
delete iTrust;
//__UHEAP_MARKEND;
return KErrNone;
}
	
	
	
	
	
		
	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient3
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient3( CStifItemParser& aItem )
    {
    
    __UHEAP_MARK;
    
    	iSession = CRTSecManager::NewL ();
    		iSession2 = CRTSecManager::NewL (); // 2nd session
				iTrust = CTrustInfo::NewL ();
    
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
			
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				CleanupClosePushL (secPolicyFile);
				iPolicyId = iSession->SetPolicy (secPolicyFile);
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
	TInt32 exId = iSession2->RegisterScript (iPolicyId, *iTrust);
	
	CRTSecMgrScriptSession* scriptSession;


	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);
			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
		RCapabilityArray caps;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
					
 
 
   
 	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close();

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 	
 		TInt result = iSession->UnSetPolicy (iPolicyId);		

 
		TExecutableID exId2 = iSession2->RegisterScript (iPolicyId, *iTrust);
 	
  
//  TInt32 x = -108;
 	if(exId2 != ErrRegisterScriptFailed) 
 	{
 	delete iSession;
delete iSession2;
delete iTrust;
 	return exId2;
 		
 	}
 

 
//delete scriptSession1;
delete iSession;
delete iSession2;
delete iTrust;
//__UHEAP_MARKEND;
return KErrNone;
}


		
	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient4
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient4(CStifItemParser& aItem )
    {
    
    __UHEAP_MARK;
    
    	iSession = CRTSecManager::NewL ();
    		iSession2 = CRTSecManager::NewL (); // 2nd session
				iTrust = CTrustInfo::NewL ();
    
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
			
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				CleanupClosePushL (secPolicyFile);
				iPolicyId = iSession->SetPolicy (secPolicyFile);
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
	TInt32 exId = iSession->RegisterScript (iPolicyId, *iTrust);
	
	CRTSecMgrScriptSession* scriptSession;


	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);
			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
		RCapabilityArray caps;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
					
 
 
   
 	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close();

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 		TInt result = iSession->UnRegisterScript (exId, iPolicyId);
 
 		scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);
  
 		if(scriptSession)
 		
 		{
 			return KErrGeneral;
 		}

 
//delete scriptSession1;
delete iSession;
delete iSession2;
delete iTrust;
//__UHEAP_MARKEND;
return KErrNone;
}
	
	
	
			
	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient5
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient5(CStifItemParser& aItem )
    {
    
    __UHEAP_MARK;
   
   
   int i= 0;
   
   
  CRTSecManager  *iSession[10];
   
  while(i<10)
  {
  	
   
   iSession[i] =  CRTSecManager::NewL ();
   i++;
  }
   
    		
    		
  iTrust = CTrustInfo::NewL ();
    
  		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
			
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				CleanupClosePushL (secPolicyFile);
				iPolicyId = iSession[0]->SetPolicy (secPolicyFile);
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
	TInt32 exId = iSession[1]->RegisterScript (iPolicyId, *iTrust);
	
	CRTSecMgrScriptSession* scriptSession;


	scriptSession = iSession[2]->GetScriptSessionL(iPolicyId, exId);
			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
		RCapabilityArray caps;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
					
 
 
   
 	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close();

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
  
 
 int j = 0;	 

  while(j<10)
  {
     
   delete iSession[j] ;
   j++;
  }

 
delete iTrust;
//__UHEAP_MARKEND;
return KErrNone;
}
	
	
	
	
				
	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient6
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient6(CStifItemParser& aItem )
    {
    
    __UHEAP_MARK;
    
    	iSession = CRTSecManager::NewL ();
    		iSession2 = CRTSecManager::NewL (); // 2nd session
				iTrust = CTrustInfo::NewL ();
    
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
			
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileReadDirectIO))
				{
				CleanupClosePushL (secPolicyFile);
				iPolicyId = iSession->SetPolicy (secPolicyFile);
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
	TInt32 exId = iSession->RegisterScript (iPolicyId, *iTrust);
	
	CRTSecMgrScriptSession* scriptSession;


	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);
			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
		RCapabilityArray caps;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
					
 
 
   
 	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close();

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 		TInt result = iSession->UnRegisterScript (exId, iPolicyId);
 		
 		TInt result1 = iSession->UnRegisterScript (exId, iPolicyId);
 
 		 
if(result1 != ErrUnRegisterScriptFailed )
{
	
delete iSession;
delete iSession2;
delete iTrust;
return result1;
    }
 
//delete scriptSession1;
delete iSession;
delete iSession2;
delete iTrust;
//__UHEAP_MARKEND;
return KErrNone;
}





	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient7
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient7(CStifItemParser& aItem )
    {
    
    __UHEAP_MARK;
      TPtrC sptr;
    	TInt32 exId;
    	 aItem.GetNextString(sptr);
    	 
    	iSession = CRTSecManager::NewL ();
    		iSession2 = CRTSecManager::NewL (); // 2nd session
				iTrust = CTrustInfo::NewL ();
    	RFile secPolicyFile;
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
		
			
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				CleanupClosePushL (secPolicyFile);
			//	iPolicyId = iSession->SetPolicy (secPolicyFile);
				iPolicyId = iSession->UpdatePolicy(iPolicyId,secPolicyFile);
			
			
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
 
	
 

 if(iPolicyId!= ErrInvalidPolicyID)
 return KErrGeneral;
 
 		if(!sptr.Compare(_L("gsswithtrust")))
 		{
 			
 		 
 		scriptSession = iSession->GetScriptSessionL(iPolicyId,*iTrust );	 
if(scriptSession != NULL)
return KErrGeneral;
 		}
 		
 		if(!sptr.Compare(_L("gsswithouttrust")))
 		{
exId = iSession->RegisterScript(iPolicyId, *iTrust);
	 	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);

if(scriptSession != NULL)
return KErrGeneral;
 		}



 
//delete scriptSession1;
delete iSession;
delete iSession2;
delete iTrust;
//__UHEAP_MARKEND;
return KErrNone;
}





	
	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient8
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient8( CStifItemParser& aItem )
    {
    
    __UHEAP_MARK;
    TInt32 exId;
    TPtrC sptr;
    	
    	 aItem.GetNextString(sptr);
 	 	CRTSecMgrScriptSession* scriptSession = NULL;
    	iSession = CRTSecManager::NewL ();
    //		iSession2 = CRTSecManager::NewL ();
	iTrust = CTrustInfo::NewL ();
    
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
		  
		  	TBuf8<5000>  aFilebuffer ;
			
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				
				
				CleanupClosePushL (secPolicyFile);
				
			
					
				 if(!sptr.Compare(_L("File")))
				 {
				 	
				  
				
				iPolicyId = iSession->SetPolicy (secPolicyFile);
				
					iPolicyId = iSession->UpdatePolicy(iPolicyId,secPolicyFile);
				 
				 
				 
				 }
				 
				 if(!sptr.Compare(_L("Buffer")))
				 {
				if(KErrNone == secPolicyFile.Read(aFilebuffer))
				{
							
				iPolicyId = iSession->SetPolicy (aFilebuffer);
					iPolicyId = iSession->UpdatePolicy(iPolicyId,aFilebuffer);
			
			
				}
				 }
				 
				
				
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	 	 aItem.GetNextString(sptr);
	 	 
	if(!sptr.Compare(_L("registerscript")))
	{
		

	 exId = iSession->RegisterScript(iPolicyId, *iTrust);
	 	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);
	}
	
	if(!sptr.Compare(_L("noregisterscript")))
	{
	//no implementaion
	
		scriptSession = iSession->GetScriptSessionL(iPolicyId,*iTrust );
	}
		 
			
			
	

 
	



//#endif			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
 	RCapabilityArray caps ;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
					 aItem.GetNextString(sptr);			
 	if(!sptr.Compare(_L("advancedprompt")))
			{
			
				scriptSession->PromptHandler()->SetPromptOption(RTPROMPTUI_ADVANCED);
			}
					
 
 	RCapabilityArray unMatchedCapabilities;
		TInt isAllowed(scriptSession->IsAllowed (caps,unMatchedCapabilities));
   
 //	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
	

	
		caps.Close(); 

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 	
 	//	TInt result = iSession->UnSetPolicy (iPolicyId);		

 
	
 	
 

 

//delete scriptSession1;
delete iSession;
//delete iSession2;
delete iTrust;
//__UHEAP_MARKEND;
return KErrNone;
}




	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient1
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient9( CStifItemParser& aItem )
    {
    
    __UHEAP_MARK;
    TInt32 exId;
    TPtrC sptr;
    	
    	 
    	 
    	iSession = CRTSecManager::NewL ();
    //		iSession2 = CRTSecManager::NewL ();
	iTrust = CTrustInfo::NewL ();
    
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
		  
		  	  
		  	TBuf8<5000>  aFilebuffer ;
				 
				 
				 
			 
				 
			 
				 
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				
				
				CleanupClosePushL (secPolicyFile);
				
			
				if(KErrNone == secPolicyFile.Read(aFilebuffer))
				{
							
				iPolicyId = iSession->SetPolicy (aFilebuffer);
				}
				 
				 
				
				
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
		 aItem.GetNextString(sptr);
		 
		 if(!sptr.Compare(_L("nohash")))
			{
	
	
	 exId = iSession->RegisterScript (iPolicyId, *iTrust);
	 	scriptSession = iSession->GetScriptSessionL(iPolicyId,*iTrust );
			}
			
			if(!sptr.Compare(_L("hash")))
			{
	
		_LIT(KHash,"sdsds");
		TBuf<10> aHash;
		aHash.Copy(_L("sdsds"));
		  exId = iSession->RegisterScript(iPolicyId,aHash,*iTrust);
 	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);
			}
			
			
	
//	CRTSecMgrScriptSession* scriptSession;
 
	



//#endif			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
 	RCapabilityArray caps;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
	
			
 	//	RCapabilityArray caps;
		RCapabilityArray unMatchedCapabilities;
		TInt isAllowed(scriptSession->IsAllowed (caps,unMatchedCapabilities));
   
 //	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close(); 

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 	
 	//	TInt result = iSession->UnSetPolicy (iPolicyId);		

 
	
 	
 

 

//delete scriptSession1;
delete iSession;
//delete iSession2;
delete iTrust;
//__UHEAP_MARKEND;
return KErrNone;
}


 
 
 
 
	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient10
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient10( CStifItemParser& aItem )
    {
    
    __UHEAP_MARK;
    TInt32 exId;
    TPtrC sptr;
    	
    	 aItem.GetNextString(sptr);
    	 	 	CRTSecMgrScriptSession* scriptSession = NULL;
    	iSession = CRTSecManager::NewL ();
    //		iSession2 = CRTSecManager::NewL ();
	iTrust = CTrustInfo::NewL ();
    
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
		  
		  	TBuf8<5000>  aFilebuffer ;
			
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				
				
				CleanupClosePushL (secPolicyFile);
				
			
					
				 if(!sptr.Compare(_L("File")))
				 {
				 	
				  
				
				iPolicyId = iSession->SetPolicy (secPolicyFile);
				
					iPolicyId = iSession->UpdatePolicy(iPolicyId,secPolicyFile);
				 
				 
				 
				 }
				 
				 if(!sptr.Compare(_L("Buffer")))
				 {
				if(KErrNone == secPolicyFile.Read(aFilebuffer))
				{
							
				iPolicyId = iSession->SetPolicy (aFilebuffer);
					iPolicyId = iSession->UpdatePolicy(iPolicyId,aFilebuffer);
			
			
				}
				 }
				 
				
				
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	 	 aItem.GetNextString(sptr);
	 	 
	if(!sptr.Compare(_L("registerscript")))
	{
		

	 exId = iSession->RegisterScript(iPolicyId, *iTrust);
	 	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);
	}
	
	if(!sptr.Compare(_L("noregisterscript")))
	{
	//no implementaion
	
		scriptSession = iSession->GetScriptSessionL(iPolicyId,*iTrust );
	}
		 
			
			
	

 
	



//#endif			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
 	RCapabilityArray caps ;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
		//	caps.Append(ECapabilitySurroundingsDD);
					 aItem.GetNextString(sptr);			
 	if(!sptr.Compare(_L("advancedprompt")))
			{
			
				scriptSession->PromptHandler()->SetPromptOption(RTPROMPTUI_ADVANCED);
			}
					
 
 	RCapabilityArray unMatchedCapabilities;
 	unMatchedCapabilities.Append (ECapabilitySurroundingsDD);
		TInt isAllowed(scriptSession->IsAllowed (caps,unMatchedCapabilities));
   
 //	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
	

	
		caps.Close(); 

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 	
 	//	TInt result = iSession->UnSetPolicy (iPolicyId);		

 
	
 	
 

 

//delete scriptSession1;
delete iSession;
//delete iSession2;
delete iTrust;
//__UHEAP_MARKEND;
return KErrNone;
}






	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient1
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient11( CStifItemParser& aItem )
    {
    
    
    TInt cnt = User::CountAllocCells();
   // __UHEAP_MARK;
    TInt32 exId = 0;
    TPtrC sptr;
    	
    	 aItem.GetNextString(sptr);
    	 
    	iSession = CRTSecManager::NewL ();
    //		iSession2 = CRTSecManager::NewL ();
	iTrust = CTrustInfo::NewL ();
    
   // isubsession = RSecMgrSubSession::NewL();
 	// SubSessionHandle();
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
		  
		  	  
		  	TBuf8<5000>  aFilebuffer ;
				 
				 
				 
			 
				 
				 aItem.GetNextString(sptr);
				 
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				
				
				CleanupClosePushL (secPolicyFile);
				
			
					
				 if(!sptr.Compare(_L("File")))
				 {
				 	
				  
				
				iPolicyId = iSession->SetPolicy (secPolicyFile);
				 }
				 if(!sptr.Compare(_L("Buffer")))
				 {
				if(KErrNone == secPolicyFile.Read(aFilebuffer))
				{
							
				iPolicyId = iSession->SetPolicy (aFilebuffer);
				}
				 }
				 
				
				
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
		 aItem.GetNextString(sptr);
		 
		 if(!sptr.Compare(_L("nohash")))
			{
	
	
	 exId = iSession->RegisterScript (iPolicyId, *iTrust);
			}
			
			if(!sptr.Compare(_L("hash")))
			{
	
		_LIT(KHash,"sdsds");
		TBuf<10> aHash;
		aHash.Copy(_L("sdsds"));
		  exId = iSession->RegisterScript(iPolicyId,aHash,*iTrust);
 
			}
			
			
	
	CRTSecMgrScriptSession* scriptSession;
 
	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);	



//#endif			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
 	RCapabilityArray caps;
	//	caps.Append (ECapabilityWriteUserData);
	//	caps.Append (ECapabilityReadUserData);
	//	caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityNetworkServices);
					
 
 
   
 	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close(); 

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 	
 	//	TInt result = iSession->UnSetPolicy (iPolicyId);		

 
	
 	
 

 

//delete scriptSession1;
delete iSession;
//delete iSession2;
delete iTrust;

  cnt = User::CountAllocCells();
//__UHEAP_MARKEND;
return KErrNone;
}


	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient12
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient12( CStifItemParser& aItem )
    {
    
    
    TInt cnt = User::CountAllocCells();
   // __UHEAP_MARK;
    TInt32 exId;
    TPtrC sptr;
    	
    	 aItem.GetNextString(sptr);
    	 
    	iSession = CRTSecManager::NewL ();
    //		iSession2 = CRTSecManager::NewL ();
	iTrust = CTrustInfo::NewL ();
    
 		
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
		  
		  	  
		  	TBuf8<500>  aFilebuffer ;
				 
				 
				 
			 
				 
				 aItem.GetNextString(sptr);
				 
			if ( KErrNone == secPolicyFile.Open (fileSession, _L("c:\\data\\Others\\accesspolicy.xml"),
					EFileShareAny))
				{
				
				
				
				
				
				
				CleanupClosePushL (secPolicyFile);
				
			
			
				if(KErrNone == secPolicyFile.Read(aFilebuffer))
				{
							
				iPolicyId = iSession->SetPolicy (aFilebuffer);
				}
				 
				 if(iPolicyId!=ErrInvalidPolicyFormat )
				 return KErrGeneral;
				
				
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
	 
	
	
	 exId = iSession->RegisterScript (iPolicyId, *iTrust);
		 
			if(exId!=ErrInvalidPolicyID)
	return KErrGeneral;
			
	
	CRTSecMgrScriptSession* scriptSession;
 
	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);	



//#endif			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
 	RCapabilityArray caps;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
					
 
 
   
 	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close(); 

		CleanupStack::PopAndDestroy ();//scriptSession
	}
	

 	
 	
 	//	TInt result = iSession->UnSetPolicy (iPolicyId);		

 
	
 	
 

 

//delete scriptSession1;
delete iSession;
//delete iSession2;
delete iTrust;

  cnt = User::CountAllocCells();
//__UHEAP_MARKEND;
return KErrNone;
}





	// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::secclient13
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cui_tsecuritymanager::secclient13( CStifItemParser& aItem )
    {
    
    
    TInt cnt = User::CountAllocCells();
   // __UHEAP_MARK;
    TInt32 exId;
    TPtrC sptr;
    
    	TBuf<50> path;
    	 aItem.GetNextString(sptr);
    	 
    	 
    	 
    	   // _LIT(path,"c:\\data\\Others\\accesspolicy_1.xml");
    	 
    	 	 if(!sptr.Compare(_L("1")))
				 {			 
				   	path.Copy(_L("c:\\data\\Others\\accesspolicy_1.xml")) ;
 				 }
				 
				 	 if(!sptr.Compare(_L("2")))
				 {
    	 
			     path.Copy(_L("c:\\data\\Others\\accesspolicy_2.xml")) ;
				 }
				 	 if(!sptr.Compare(_L("3")))
				 {
    	 
				   path.Copy(_L("c:\\data\\Others\\accesspolicy_3.xml")) ;
				 }
    
    
    	
     
    	 
    	iSession = CRTSecManager::NewL ();
 	iTrust = CTrustInfo::NewL ();
    
    
    
	RFs fileSession;
	if ( KErrNone==fileSession.Connect ())
		{
		CleanupClosePushL (fileSession);
		if(KErrNone==fileSession.ShareProtected())
			{
			RFile secPolicyFile;
		  
		  	  
		  	TBuf8<5000>  aFilebuffer ;
				 
				 
				 
			 
		 
				 
			if ( KErrNone == secPolicyFile.Open (fileSession,path,
					EFileShareAny))
				{
				
				
				
				CleanupClosePushL (secPolicyFile);
				
			
			
				if(KErrNone == secPolicyFile.Read(aFilebuffer))
				{
							
				iPolicyId = iSession->SetPolicy (aFilebuffer);
				}
				 
				 if(iPolicyId!=ErrInvalidPolicyFormat )
				 return KErrGeneral;
				
				
				CleanupStack::PopAndDestroy ();//secPolicyFile
				}
			}
			CleanupStack::PopAndDestroy ();//fileSession
		}
	
	
	 
	
	
	 exId = iSession->RegisterScript (iPolicyId, *iTrust);
		 
	if(exId!=ErrInvalidPolicyID)
	return KErrGeneral;
			
	
	CRTSecMgrScriptSession* scriptSession;
 
	scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);	



//#endif			

  

	if(scriptSession)
	{
		CleanupStack::PushL (scriptSession);
 	RCapabilityArray caps;
		caps.Append (ECapabilityWriteUserData);
		caps.Append (ECapabilityReadUserData);
		caps.Append (ECapabilityLocation);
		caps.Append (ECapabilityUserEnvironment);
					
 
 
   
 	TInt IsAllowed(scriptSession->IsAllowed(caps));
 		
		

	
		caps.Close(); 

		CleanupStack::PopAndDestroy ();//scriptSession
		
		return KErrGeneral;
	}
	
 	
 	
 	//	TInt result = iSession->UnSetPolicy (iPolicyId);		

 
	
 	
 

 

//delete scriptSession1;
delete iSession;
//delete iSession2;
delete iTrust;

  cnt = User::CountAllocCells();
//__UHEAP_MARKEND;
return KErrNone;
}





