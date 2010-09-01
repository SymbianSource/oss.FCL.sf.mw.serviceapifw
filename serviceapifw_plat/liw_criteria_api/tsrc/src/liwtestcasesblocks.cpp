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
* Description:       ?Description
*
*/








// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include "liwtestcases.h"

#include  <aknViewAppUi.h>
#include  <liwservicehandler.h>
//#include  "liwTC1.h"
//#include  "liwtestapp.hrh"
//#include  <liwtestapp.rsg>
#include  <s32mem.h> // for read write stream

#include <rtsecmanager.h>
#include <rtsecmgrutility.h>
#include <rtsecmgrscriptsession.h>
#include <rtsecmgrcommondef.h>
#include "liwtestapplit.h"

#include  "liw_async_testcase.h"

using namespace LIW;

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cliwtestcases::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Cliwtestcases::Delete() 
    {
		if(iServiceHandler)
		{
    	iServiceHandler->Reset();
    	delete iServiceHandler;	
    	iServiceHandler = NULL;
		}
    
    }

// ---------------------------------------------------------
// CLiwtcbase::HandleNotifyL
// From - MLiwNotifyCallback
// ---------------------------------------------------------
TInt Cliwtestcases::HandleNotifyL
    (
    TInt /*aCmdId*/,
    TInt /*aEventId*/,
    CLiwGenericParamList& /*aEventParamList*/,
    const CLiwGenericParamList& /*aInParamList*/)
    {
    return KErrNone;    
    }
    

TInt Cliwtestcases::LoadL( )
    {
    RCriteriaArray interest;
    CleanupClosePushL(interest);
    
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    criteria->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    interest.AppendL(criteria);
    
    // Attach the MyServiceName provider to the LIW framework.
    TInt status = iServiceHandler->AttachL(interest);

    // Detach The MyServiceName Provider
    //iServiceHandler->DetachL(interest);    
    CleanupStack::PopAndDestroy(criteria);
    CleanupStack::PopAndDestroy(&interest);   
    return status;
    }   
        
// -----------------------------------------------------------------------------
// Cliwtestcases::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Cliwtestcases::RunMethodL( 
    CStifItemParser& aItem ) 
    {
        TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksHandles );
        
        static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "LIW_DATA_TYPES_001", Cliwtestcases::LIW_DATA_TYPES_001 ),
        ENTRY( "LIW_DATA_TYPES_002", Cliwtestcases::LIW_DATA_TYPES_002 ),
        ENTRY( "LIW_DATA_TYPES_003", Cliwtestcases::LIW_DATA_TYPES_003 ),
        ENTRY( "LIW_DATA_TYPES_004", Cliwtestcases::LIW_DATA_TYPES_004 ),
        ENTRY( "LIW_DATA_TYPES_005", Cliwtestcases::LIW_DATA_TYPES_005 ),
        ENTRY( "LIW_DATA_TYPES_006", Cliwtestcases::LIW_DATA_TYPES_006 ),
        ENTRY( "LIW_DATA_TYPES_007", Cliwtestcases::LIW_DATA_TYPES_007 ),
        ENTRY( "LIW_DATA_TYPES_008", Cliwtestcases::LIW_DATA_TYPES_008 ),
        ENTRY( "LIW_DATA_TYPES_009", Cliwtestcases::LIW_DATA_TYPES_009 ),
        ENTRY( "LIW_Map_AtL_New", Cliwtestcases::LIW_Map_AtL_New ),
        ENTRY( "LIW_DATA_TYPES_010", Cliwtestcases::LIW_DATA_TYPES_010 ),
        ENTRY( "LIW_DATA_TYPES_011", Cliwtestcases::LIW_DATA_TYPES_011 ),
        ENTRY( "LIW_DATA_TYPES_012", Cliwtestcases::LIW_DATA_TYPES_012 ),
        ENTRY( "LIW_DATA_TYPES_013", Cliwtestcases::LIW_DATA_TYPES_013 ),
        ENTRY( "LIW_DATA_TYPES_014", Cliwtestcases::LIW_DATA_TYPES_014 ),
        ENTRY( "LIW_DATA_TYPES_016", Cliwtestcases::LIW_DATA_TYPES_016 ),
        ENTRY( "LIW_DATA_TYPES_017", Cliwtestcases::LIW_DATA_TYPES_017 ),
        ENTRY( "LIW_DATA_TYPES_018", Cliwtestcases::LIW_DATA_TYPES_018 ),
        ENTRY( "LIW_DATA_TYPES_020", Cliwtestcases::LIW_DATA_TYPES_020 ),
        ENTRY( "LIW_DATA_TYPES_021", Cliwtestcases::LIW_DATA_TYPES_021 ),
        ENTRY( "LIW_DATA_TYPES_022", Cliwtestcases::LIW_DATA_TYPES_022 ),
        ENTRY( "LIW_DATA_TYPES_023", Cliwtestcases::LIW_DATA_TYPES_023 ),
        ENTRY( "LIW_DATA_TYPES_024", Cliwtestcases::LIW_DATA_TYPES_024 ),
        ENTRY( "LIW_DATA_TYPES_025", Cliwtestcases::LIW_DATA_TYPES_025 ),
        ENTRY( "LIW_DATA_TYPES_026", Cliwtestcases::LIW_DATA_TYPES_026 ),
        ENTRY( "LIW_DATA_TYPES_029", Cliwtestcases::LIW_DATA_TYPES_029 ),
        ENTRY( "LIW_DATA_TYPES_030", Cliwtestcases::LIW_DATA_TYPES_030 ),
        ENTRY( "LIW_DATA_TYPES_033", Cliwtestcases::LIW_DATA_TYPES_033 ),
        ENTRY( "LIW_DATA_TYPES_034", Cliwtestcases::LIW_DATA_TYPES_034 ),
        ENTRY( "LIW_DATA_TYPES_037", Cliwtestcases::LIW_DATA_TYPES_037 ),
        ENTRY( "LIW_DATA_TYPES_038", Cliwtestcases::LIW_DATA_TYPES_038 ),
        ENTRY( "LIW_DATA_TYPES_039", Cliwtestcases::LIW_DATA_TYPES_039 ),
        ENTRY( "LIW_DATA_TYPES_041", Cliwtestcases::LIW_DATA_TYPES_041 ),
        ENTRY( "LIW_DATA_TYPES_042", Cliwtestcases::LIW_DATA_TYPES_042 ),
        ENTRY( "LIW_DATA_TYPES_043", Cliwtestcases::LIW_DATA_TYPES_043 ),
        ENTRY( "LIW_DATA_TYPES_045", Cliwtestcases::LIW_DATA_TYPES_045 ),
        ENTRY( "LIW_DATA_TYPES_046", Cliwtestcases::LIW_DATA_TYPES_046 ),
        ENTRY( "LIW_DATA_TYPES_047", Cliwtestcases::LIW_DATA_TYPES_047 ),
        ENTRY( "LIW_DATA_TYPES_048", Cliwtestcases::LIW_DATA_TYPES_048 ),
        ENTRY( "LIW_DATA_TYPES_049", Cliwtestcases::LIW_DATA_TYPES_049 ),
        ENTRY( "LIW_MDAT_VER1", Cliwtestcases::LIW_MDAT_VER1 ),
        ENTRY( "LIW_MDAT_VER2", Cliwtestcases::LIW_MDAT_VER2 ),
        ENTRY( "LIW_MDAT_VER3", Cliwtestcases::LIW_MDAT_VER3 ),
        ENTRY( "LIW_MDAT_VER4", Cliwtestcases::LIW_MDAT_VER4 ),
        ENTRY( "LIW_MDAT_VER5", Cliwtestcases::LIW_MDAT_VER5 ),
        ENTRY( "LIW_MDAT_CAP1", Cliwtestcases::LIW_MDAT_CAP1 ),
        ENTRY( "LIW_ContainerCleanup", Cliwtestcases::LIW_ContainerCleanup ),
        ENTRY( "LIW_ParamCleanup", Cliwtestcases::LIW_ParamCleanup ),
        ENTRY( "LIW_VariantCleanup", Cliwtestcases::LIW_VariantCleanup ),
        ENTRY( "LIW_ASYNC_001", Cliwtestcases::LIW_ASYNC_001 ),
        ENTRY( "LIW_ASYNC_002", Cliwtestcases::LIW_ASYNC_002 ),
        ENTRY( "LIW_ASYNC_003", Cliwtestcases::LIW_ASYNC_003 ),
        ENTRY( "LIW_ASYNC_004", Cliwtestcases::LIW_ASYNC_004 ),
        ENTRY( "LIW_ASYNC_LongServiceCmd", Cliwtestcases::LIW_ASYNC_LongServiceCmd ),
        ENTRY( "LIW_ASYNC_LongInterface", Cliwtestcases::LIW_ASYNC_LongInterface ),
        ENTRY( "LIW_ASYNC_LongInterface1", Cliwtestcases::LIW_ASYNC_LongInterface1 ),
        ENTRY( "LIW_ASYNC_LongInvalidServiceCmd", Cliwtestcases::LIW_ASYNC_LongInvalidServiceCmd ),
        ENTRY( "LIW_ASYNC_LongInvalidInterface", Cliwtestcases::LIW_ASYNC_LongInvalidInterface ),
        ENTRY( "LIW_ParamCleanup1", Cliwtestcases::LIW_ParamCleanup1 ),
        ENTRY( "LIW_GetInterest", Cliwtestcases::LIW_GetInterest ),
        ENTRY( "LIW_Defaut_list_externalize", Cliwtestcases::LIW_Defaut_list_externalize ),
        ENTRY( "LIW_DATA_TYPES_013_A", Cliwtestcases::LIW_DATA_TYPES_013_A ),
        ENTRY( "LIW_DATA_TYPES_015_A", Cliwtestcases::LIW_DATA_TYPES_015_A ),
        ENTRY( "LIW_DATA_TYPES_015", Cliwtestcases::LIW_DATA_TYPES_015 ),
        ENTRY( "LIW_DATA_GenericParamList", Cliwtestcases::LIW_DATA_GenericParamList ),
        ENTRY( "LIW_DATA_NewLOverLoad", Cliwtestcases::LIW_DATA_NewLOverLoad )
                        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// Cliwtestcases::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Cliwtestcases::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Kliwtestcases, "liwtestcases" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Kliwtestcases, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Kliwtestcases, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// ---------------------------------------------------------
// Test Case Deffinition for LIW Data Types
// 
// ---------------------------------------------------------
TBool Cliwtestcases::LIW_DATA_TYPES_001()
    {
	    TBool flag = 0;
	    /*Testing String Based Command*/
	    //LoadL();
	    TLiwGenericParam param;
	    param.Value().Set(KLIW_DATA_TYPES_001);
	    param.SetNameL(KTestPARAMNanmeTC1);
	    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
	    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
	    inps->AppendL(param);
	    param.Reset();
	    
	    // Get the Test result from MyServiceName provider "ExecuteServiceCmdL"
	    CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
	    crit->SetServiceClass(TUid::Uid(KLiwClassBase));
	    iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps);  
	    CleanupStack::PopAndDestroy(crit); // crit
	    
	    TInt pos = 0;
	    outps->FindFirst(pos, EGenericParamError);
	    if(pos != KErrNotFound)
	        if ((*outps)[pos].Value().AsTInt32() == KErrNone)
	        {
	            flag = 1;
	        }
	        else
	        {
	            flag = 0;
	        }
		else
		{
	        flag = 0;
		}
		
        // Detach The MyServiceName Provider
	    //iServiceHandler->DetachL(interest);    
    	//CleanupStack::PopAndDestroy(criteria);
    	//CleanupStack::PopAndDestroy(&interest);   
		return !flag;
    }
   
TBool Cliwtestcases::LIW_DATA_TYPES_002()
    {
        TBool flag = 0;
        /*Testing Boolean data Type*/
        
        //LoadL();
        
        TLiwGenericParam param;
        param.Value().Set(KLIW_DATA_TYPES_002);
        param.SetNameL(KTestPARAMNanmeTC1);
        CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
        CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
        inps->AppendL(param);
        param.Reset();
        
        // Set up Tbool varient and append TBool parameter (EGenericParamError).
        // One Argument constructer 
        TLiwVariant boolVarient(ETrue);
        // Test TLiwVariant::SetL(TLiwVariant&)
        TLiwGenericParam boolParam(EGenericParamError);
        boolParam.Value().SetL(boolVarient);
        
        inps->AppendL(boolParam);
        
        boolVarient.Reset();
        boolParam.Reset();
        
        // Call ExecuteServiceCmdL TO GET THE RESULT
        CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
        crit->SetServiceClass(TUid::Uid(KLiwClassBase));
        iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps); 
        CleanupStack::PopAndDestroy(crit); // crit
        
        // CHECK RESULT 
        TInt pos = 0;
        outps->FindFirst(pos, EGenericParamError);
        
        if(pos != KErrNotFound)
            if ((*outps)[pos].Value().AsTInt32() == KErrNone)
                flag = 1;
            else
                flag = 0;
         else
            flag = 0;
         
        return !flag;
    }

TBool Cliwtestcases::LIW_DATA_TYPES_003( )
    {
        TBool flag = 0;
        CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
        CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
        
        //LoadL();
        
        /*Testing List Data Type for TLiwVariant interface and container interface*/
        TLiwGenericParam param;
        param.Value().Set(KLIW_DATA_TYPES_003);
        param.SetNameL(KTestPARAMNanmeTC1);
        inps->AppendL(param);
        param.Reset();
        /*Checks Default constructer "CLiwDefaultList" Creation*/
        CLiwList* listType = CLiwDefaultList::NewL();
        
        /*Create list of 10 integer Varient Checks CLiwList::AppendL*/
        TLiwVariant intVairent;
        for (TInt32 index = 100; index < 110; index++)
            {
            intVairent.Set(index);
            listType->AppendL(intVairent);    
            }
            
        /*Checks CLiwList::TLiwVariant.Set(CLiwList*)*/  
        param.Value().Set(listType);
        param.SetNameL(_L8("IntegerList"));
        inps->AppendL(param);
        
        intVairent.Reset();
        param.Reset();
        
        CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
        crit->SetServiceClass(TUid::Uid(KLiwClassBase));
        iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps);  
        CleanupStack::PopAndDestroy(crit); // crit

        TInt pos = 0;
        TBool result = FALSE;
        listType->Remove(listType->Count() - 1);
        outps->FindFirst(pos, _L8("IntegerList"));
        if(pos != KErrNotFound)
            {
               const CLiwList* resultList = (*outps)[pos].Value().AsList();
               if(resultList != NULL )
               {
               if(resultList->Count() == listType->Count() && listType->Size() == resultList->Size())
                   {
                   result = TRUE;
                   } 
               }
            }
        /*Decrement count to delete the instance of list its self managed memory allocation*/      
        if (listType) listType->DecRef();
        pos = 0;
        outps->FindFirst(pos, EGenericParamError);
        
        if(pos != KErrNotFound)
            if ((*outps)[pos].Value().AsTInt32() == KErrNone && result)
                flag = 1;
            else
                flag = 0;
        else
            flag = 0;
        
        // Detach The MyServiceName Provider
        //iServiceHandler->DetachL(interest);    
        //CleanupStack::PopAndDestroy(criteria);
        //CleanupStack::PopAndDestroy(&interest);   
        
        return !flag;
    }
    
TBool Cliwtestcases::LIW_DATA_TYPES_004( )
    {
        TBool flag = 0;
        CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
        CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
        
        /*Testing List Data Type for TLiwVariant interface and container interface*/
        TLiwGenericParam param;
        param.Value().Set(KLIW_DATA_TYPES_004);
        param.SetNameL(KTestPARAMNanmeTC1);
        inps->AppendL(param);
        param.Reset();
        /*Checks Default constructer "CLiwDefaultMap" Creation*/
        CLiwMap* mapType = CLiwDefaultMap::NewL();
        
        /*Create Map of 10 integer Varient Checks CLiwMap::InsertL*/
        TLiwVariant contactNbr;
        TBuf8<32> contactName;
        for (TInt32 index = 100; index < 110; index++)
            {
            contactName.Format(KContact, index - 99);
            contactNbr.Set(index);
            mapType->InsertL(contactName, contactNbr);    
            }
            
        /*Checks CLiwList::TLiwVariant.Set(CLiwMap*)*/  
        param.Value().Set(mapType);
        param.SetNameL(_L8("ContactNumbers"));
        inps->AppendL(param);
        
        contactNbr.Reset();
        param.Reset();
        
        CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
        crit->SetServiceClass(TUid::Uid(KLiwClassBase));
        iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps);  
        CleanupStack::PopAndDestroy(crit); // crit

        TInt pos = 0;
        TBool result = FALSE;
        mapType->Remove(_L8("Contact 10"));
        outps->FindFirst(pos, _L8("ContactNumbers"));
        if(pos != KErrNotFound)
            {
               const CLiwMap* resultMap = (*outps)[pos].Value().AsMap();
               if(resultMap != NULL )
                   {
                   if( resultMap->Count() == mapType->Count() && mapType->Size() == resultMap->Size())
                       {
                       result = TRUE;
                       } 
                   }
            }
        /*Decrement count to delete the instance of Map its self managed memory allocation*/      
        if (mapType) mapType->DecRef();
        
        pos = 0;
        outps->FindFirst(pos, EGenericParamError);
        
        if(pos != KErrNotFound)
            if ((*outps)[pos].Value().AsTInt32() == KErrNone && result)
                flag = 1;
            else
                flag = 0;
        else
            flag = 0;
        
        return !flag;
    }
    
TBool Cliwtestcases::LIW_DATA_TYPES_005( )
    {
    TBool flag = 0;
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
    
    /*Testing List Data Type for TLiwVariant interface and container interface*/
    TLiwGenericParam param;
    param.Value().Set(KLIW_DATA_TYPES_005);
    param.SetNameL(KTestPARAMNanmeTC1);
    inps->AppendL(param);
    param.Reset();
    
    /*Checks Default constructer "CLiwDefaultMap" and "CLiwDefaultMap" Creation*/
    CLiwMap* mapType = CLiwDefaultMap::NewL();
    CLiwList* listType = CLiwDefaultList::NewL();
    
    /*Create list of 10 integer Varient Checks CLiwList::AppendL*/
    TLiwVariant intVairent;
    for (TInt32 index2 = 100; index2 < 110; index2++)
        {
        intVairent.Set(index2);
        listType->AppendL(intVairent);    
        }
        
    TBuf8<32> listName;
    TLiwVariant listVarient;
    listVarient.Set(listType);
    
    for (TInt32 index = 1; index < 11; index++)
        {
        listName.Format(KContact, index);
        mapType->InsertL(listName, listVarient);
        }
        
    listVarient.Reset();    
    /*Checks CLiwList::TLiwVariant.Set(CLiwMap*)*/  
    param.Value().Set(mapType);
    param.SetNameL(_L8("MapOfLists"));
    
    inps->AppendL(param);
    
    param.Reset();
        
    
    CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    crit->SetServiceClass(TUid::Uid(KLiwClassBase));
    iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps);  
    CleanupStack::PopAndDestroy(crit); // crit

    //remove the last list (10) and remove the last entry (9)in first List (0)
    mapType->Remove(_L8("Contact 10"));
    mapType->FindL(_L8("Contact 1"), listVarient);
    listVarient.Get(*listType);
    listType->Remove(9);
    
    listVarient.Reset();
    
    TInt pos = 0;
    TBool result = FALSE;
    outps->FindFirst(pos, _L8("MapOfLists"));
    if(pos != KErrNotFound)
        {
           const CLiwMap* resultMap = (*outps)[pos].Value().AsMap();
           if(resultMap != NULL && resultMap->Count() == mapType->Count() && mapType->Size() == resultMap->Size())
           {
           TBuf8<32> mapKey;
           resultMap->AtL(0, mapKey);
           resultMap->FindL(mapKey, listVarient);
           if(listVarient.AsList() != NULL )
               {
               if (listVarient.AsList()->Count() == listType->Count() && listType->Size() == listVarient.AsList()->Size())
                   {
                   result = TRUE; 
                   }
               }
           }
           listVarient.Reset();
        }
    
    
    pos = 0;
    outps->FindFirst(pos, EGenericParamError);
    
    if(pos != KErrNotFound)
        if ((*outps)[pos].Value().AsTInt32() == KErrNone && result)
            flag = 1;
        else
            flag = 0;
    else
        flag = 0;
    
    //Decrement count to delete the instance of Map its self managed memory allocation
    inps->Reset();
    outps->Reset();
    if (mapType) mapType->DecRef();
    if (listType) listType->DecRef();
    
    return !flag;
    }

TBool Cliwtestcases::LIW_DATA_TYPES_006( )
    {
    TBool flag = 0;    
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
    TLiwGenericParam param;
    param.Value().Set(KLIW_DATA_TYPES_006);
    param.SetNameL(KTestPARAMNanmeTC1);
    inps->AppendL(param);
    param.Reset();
      
    CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    crit->SetServiceClass(TUid::Uid(KLiwClassBase));
    iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps);  
    CleanupStack::PopAndDestroy(crit); // crit
    
    TInt pos = 0;
    MLiwInterface* ifp = NULL;
    outps->FindFirst(pos, KLIW_DATA_TYPES_006);
    if(pos != KErrNotFound)
        {
        ifp = (*outps)[pos].Value().AsInterface();
        outps->Reset();
        ifp->ExecuteCmdL(KLIW_DATA_TYPES_006, *inps, *outps);      
        }
    else
        {
        flag = 0;
        }
    pos = 0;
    outps->FindFirst(pos, EGenericParamError);
    if(pos != KErrNotFound)
        if ((*outps)[pos].Value().AsTInt32() == KErrNone)
            flag = 1;
        else
            flag = 0;
    else
        flag = 0;
    
    if(ifp) ifp->Close();
    inps->Reset();
    outps->Reset();
    return !flag;
    }

TBool Cliwtestcases::LIW_DATA_TYPES_007( )
    {
    TBool flag = 0;
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    TLiwGenericParam param;
    CLiwList* listType = CLiwDefaultList::NewL();
    CLiwMap* mapType = CLiwDefaultMap::NewL();
    listType->AppendL(TLiwVariant(_L8("Testing Stream Operations")));  
    mapType->InsertL(KLIW_DATA_TYPES_007, TLiwVariant(listType));
    param.Value().Set(mapType);
    param.SetNameL(KTestPARAMNanmeTC1);
    inps->AppendL(param);
    
    param.Reset();
    if (listType) listType->DecRef();
    if (mapType) mapType->DecRef();
    
    TInt sz = inps->Size();
    TAny* buf = User::AllocL(sz);
    RMemWriteStream ws(buf, sz);
    inps->ExternalizeL(ws);
    RMemReadStream rs(buf, sz);
    CLiwGenericParamList* ltmp = &(iServiceHandler->OutParamListL());
    ltmp->InternalizeL(rs);
    if ((ltmp->Count() == inps->Count()) && ((*inps)[0] == (*ltmp)[0]))
        flag = 1;
    else
        flag = 0;
    User::Free(buf);
    return !flag;
    }

TBool Cliwtestcases::LIW_DATA_TYPES_008( )
    {
    TBool flag = 0;
    /*Checks Default constructer "CLiwDefaultList" Creation*/
    CLiwList* listType = CLiwDefaultList::NewL();
    CLiwList* resultList;
    
    /*Create list of 10 integer Varient Checks CLiwList::AppendL*/
    TLiwVariant intVairent;
    for (TInt32 index = 100; index < 110; index++)
        {
        intVairent.Set(index);
        listType->AppendL(intVairent);    
        }
    
    intVairent.Reset();    
    TBool result = TRUE;
    
    // Checking Bound conditions
    resultList = listType;
    resultList->Remove(listType->Count() + 1);
    resultList->Remove(-1);
    if (resultList != listType)
        result = FALSE;
    result ? result = !(listType->AtL(listType->Count() + 1, intVairent)): result = FALSE;
    result ? result = !(listType->AtL(-1, intVairent)) : result = FALSE;      
    
    if(result == TRUE)
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }
    /*Decrement count to delete the instance of list its self managed memory allocation*/      
    if (listType) listType->DecRef(); 
    return !flag;
    }
    
TBool Cliwtestcases::LIW_DATA_TYPES_009( )
    {
    TBool flag = 0;
    /*Checks Default constructer "CLiwDefaultMap" Creation*/
    CLiwMap* mapType = CLiwDefaultMap::NewL();
    CLiwMap* resultMap;
    
    /*Create Map of 10 integer Varient Checks CLiwMap::InsertL*/
    TLiwVariant contactNbr;
    TBuf8<32> contactName;
    for (TInt32 index = 100; index < 110; index++)
        {
        contactName.Format(KContact, index - 99);
        contactNbr.Set(index);
        mapType->InsertL(contactName, contactNbr);    
        }
        
    /*Checks CLiwList::TLiwVariant.Set(CLiwMap*)*/  
       
    contactNbr.Reset();
    contactName.FillZ();
    contactName.Zero();
    TBool result = TRUE;
        
    // Checking Bound conditions
    resultMap = mapType;
    resultMap->Remove(_L8("Contact 11"));
    if (resultMap != mapType)
        result = FALSE;
    result ? result = !(mapType->AtL(mapType->Count() + 1, contactName)) : result = FALSE;
    result ? result = !(mapType->AtL(-1, contactName) ): result = FALSE;
    
    
    if(result == TRUE)
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }
    /*Decrement count to delete the instance of list its self managed memory allocation*/      
    if (mapType) mapType->DecRef();
    return !flag;
    }
    
TBool Cliwtestcases::LIW_Map_AtL_New( )
    {
    TBool flag = 0;
    /*Checks Default constructer "CLiwDefaultMap" Creation*/
    CLiwDefaultMap* mapType = CLiwDefaultMap::NewL();
    CLiwDefaultMap* resultMap;
    
    /*Create Map of 10 integer Varient Checks CLiwMap::InsertL*/
    TLiwVariant contactNbr;
    TBuf8<32> contactName;
    for (TInt32 index = 100; index < 110; index++)
        {
        contactName.Format(KContact, index - 99);
        contactNbr.Set(index);
        mapType->InsertL(contactName, contactNbr);    
        }
        
    /*Checks CLiwList::TLiwVariant.Set(CLiwMap*)*/  
       
    contactNbr.Reset();
    contactName.FillZ();
    contactName.Zero();
    TBool result = TRUE;
        
    // Checking Bound conditions
    resultMap = mapType;
    resultMap->Remove(_L8("Contact 11"));
    if (resultMap != mapType)
        result = FALSE;
    
    TRAPD(err1, contactName = mapType->AtL(mapType->Count() + 1));
    
    //result ? result = !(contactName.operator==(KNullDesC8)) : result = FALSE;
    
    TRAPD(err2, contactName = mapType->AtL(-1));
    
    //result ? result = !(contactName.operator==(KNullDesC8)): result = FALSE;
    
    if(err1 && err2)
        flag = 1;
    else
        flag = 0;
    
    
    /*
    if(err)
        flag = 0;
    
    if(result == TRUE)
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }
    */
    
    //Decrement count to delete the instance of list its self managed memory allocation
    if (mapType) mapType->DecRef();
    return !flag;
    }

TBool Cliwtestcases::LIW_DATA_TYPES_010()
    {
    TBool flag = 0;
    /*Testing Unsigned Integer data Type*/
    TLiwGenericParam param;
    param.Value().Set(KLIW_DATA_TYPES_010);
    param.SetNameL(KTestPARAMNanmeTC1);
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
    inps->AppendL(param);
    param.Reset();
    
    // Set up Tbool varient and append Unsigned Integer parameter (EGenericParamError).
    // One Argument constructer
    TUint arg = 10; 
    TLiwVariant uIntVarient(arg);
    // Test TLiwVariant::SetL(TLiwVariant&)
    TLiwGenericParam uIntParam(EGenericParamError);
    uIntParam.Value().SetL(uIntVarient);
    
    inps->AppendL(uIntParam);
    
    uIntVarient.Reset();
    uIntParam.Reset();
    
    // Call ExecuteServiceCmdL TO GET THE RESULT
    CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    crit->SetServiceClass(TUid::Uid(KLiwClassBase));
    iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps); 
    CleanupStack::PopAndDestroy(crit); // crit
    
    // CHECK RESULT 
    TInt pos = 0;
    outps->FindFirst(pos, EGenericParamError);
    
    if(pos != KErrNotFound)
        {
            if ((*outps)[pos].Value().AsTInt32() == KErrNone)
                {
                flag = 1;
                }
            else
                {
                
                flag = 0;
                }
        }
     else
        {
        
        flag = 0;
        }
    return !flag;
    }   
 
//CHECK FROM HERE

TBool Cliwtestcases::LIW_DATA_TYPES_011()
    {
    TBool flag = 0;
    TLiwGenericParam param;
    param.Value().Set(KLIW_DATA_TYPES_011);
    param.SetNameL(KTestPARAMNanmeTC1);
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
    inps->AppendL(param);
    param.Reset();
    
    // Call ExecuteServiceCmdL TO GET THE RESULT
    CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    crit->SetServiceClass(TUid::Uid(KLiwClassBase));
    iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps); 
    CleanupStack::PopAndDestroy(crit); // crit
    
    // CHECK RESULT 
    TInt pos = 0;
    outps->FindFirst(pos, EGenericParamError);
    
    if(pos != KErrNotFound)
        {
            if ((*outps)[pos].Value().AsTInt32() == KErrNone)
                {
                
                _LIT8(KIter,"Iterator");
                TInt iterPos=0;
                outps->FindFirst(iterPos, KIter);
                if(pos != KErrNotFound)
                {
                    CLiwIterable* pIter=(*outps)[iterPos].Value().AsIterable();
                    
                    //Iterate over the list
                    TLiwVariant var;
                    
                    while(EFalse != pIter->NextL(var))
                    {       
                        TPtrC8 entry;
                        var.Get(entry);
                        
                        if(0==entry.Length())
                        {
                            flag = 0;                               
                        }                       
                    }
                    
                    var.Reset();
                    
                }
                else
                {
                    flag = 0;
                }
                
                flag = 1;
                }
            else
                {
                flag = 0;
                }
        }
     else
        {
        
        flag = 0;
        }
    return !flag;
    }   
    
 TBool Cliwtestcases::LIW_DATA_TYPES_012()
    {
    TBool flag = 0;
    TLiwGenericParam param;
    param.Value().Set(KLIW_DATA_TYPES_012);
    param.SetNameL(KTestPARAMNanmeTC1);
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
    inps->AppendL(param);
    param.Reset();
    
    // Call ExecuteServiceCmdL TO GET THE RESULT
    CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    crit->SetServiceClass(TUid::Uid(KLiwClassBase));
    iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps); 
    CleanupStack::PopAndDestroy(crit); // crit
    
    // CHECK RESULT 
    TInt pos = 0;
    outps->FindFirst(pos, EGenericParamError);
    
    if(pos != KErrNotFound)
        {
            if ((*outps)[pos].Value().AsTInt32() == KErrNone)
                {                
                flag = 1;
                }
            else
                {
                flag = 0;
                }
        }
     else
        {
        
        flag = 0;
        }
    return !flag;
    }   
    
    
 TBool Cliwtestcases::LIW_DATA_TYPES_013()
    {
        TBool flag = 0;    
        CLiwList* pList = CLiwDefaultList::NewL();
        
        {
            TLiwVariant intVairent;
            for (TInt32 index = 100; index < 110; index++)
                {
                intVairent.Set(index);
                pList->AppendL(intVairent);    
                }   
        }
        
        if(EFalse == (pList->operator==(*pList)))
        {   
            flag = 0;
        }
        
        CLiwList* pAnotherList = CLiwDefaultList::NewL();
        
        {
            TLiwVariant intVairent;
            for (TInt32 index = 100; index < 110; index++)
                {
                intVairent.Set(index);
                pAnotherList->AppendL(intVairent);    
                }
        }
        
        
        if(EFalse == (pList->operator==(*pAnotherList)))
        {
             flag = 0;
        }
                
        pAnotherList->Remove(0);
        
        if(EFalse == (pList->operator==(*pAnotherList)))
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }   
    if(pList) pList->DecRef();
    if(pAnotherList) pAnotherList->DecRef();
    return !flag;
   }
    
TBool Cliwtestcases::LIW_DATA_TYPES_014()
    {
        TBool flag = 0; 
        CLiwMap* pMap = CLiwDefaultMap::NewL();
        
        {
            /*Create Map of 10 integer Varient Checks CLiwMap::InsertL*/
            TLiwVariant contactNbr;
            TBuf8<32> contactName;
            for (TInt32 index = 100; index < 110; index++)
                {
                contactName.Format(KContact, index - 99);
                contactNbr.Set(index);
                pMap->InsertL(contactName, contactNbr);    
                }
        }
        
        if(EFalse == (pMap->operator==(*pMap)))
        {   
            flag = 0;
        }
        
        CLiwMap* pAnotherMap = CLiwDefaultMap::NewL();
        TBuf8<32> keyToRem;
        
        {
            /*Create Map of 10 integer Varient Checks CLiwMap::InsertL*/
            TLiwVariant contactNbr;
            TBuf8<32> contactName;
            for (TInt32 index = 100; index < 110; index++)
                {
                contactName.Format(KContact, index - 99);
                contactNbr.Set(index);
                pAnotherMap->InsertL(contactName, contactNbr);  
                
                if(100==index)
                    keyToRem.Copy(contactName);
                }
        }
        
        if(EFalse == (pMap->operator==(*pAnotherMap)))
        {
             flag = 0;
        }
        
        pAnotherMap->Remove(keyToRem);
        
        if(EFalse == (pMap->operator==(*pAnotherMap)))
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }   
        if(pMap) pMap->DecRef();
        if(pAnotherMap) pAnotherMap->DecRef();
        return !flag;
    }
    
TBool Cliwtestcases::LIW_DATA_TYPES_015()
    {
         
         TBool flag = 1;
         TInt32 intVal=10;
         TLiwVariant intVar((TInt32)intVal);
         
         {//integer check
            TInt32 intRet;
            intVar.Get(intRet);
            
            Dump(intVar);
            
            if(intRet!=intVal)
            {
                flag = 0;
            }       
            
         }
         
         {//RFile check
            RFile fileRet;
            if(EFalse!=intVar.Get(fileRet))
            {
                flag = 0;
            }           
            fileRet.Close();            
         }
         
         {//Uid check
            TUid uidRet;
            if(EFalse!=intVar.Get(uidRet))
            {
                flag = 0;
            }           
         }
         
         {//TUint check
            TUint uintRet;
            if(EFalse==intVar.Get(uintRet))
            {
                flag = 0;
            }           
         }
         
         {//TBool check
            TBool boolRet=EFalse;
            if(EFalse!=intVar.Get(boolRet))
            {
                flag = 0;
            }           
         }
         
         {//TPtrC check
            TPtrC ptrcRet;
            if(EFalse!=intVar.Get(ptrcRet))
            {
                flag = 0;
            }           
         }
         
         {//TTime check
            TTime timeRet;
            if(EFalse!=intVar.Get(timeRet))
            {
                flag = 0;
            }           
         }
         
         {//TPtrC8 check
            TPtrC8 ptrcRet;
            if(EFalse!=intVar.Get(ptrcRet))
            {
                flag = 0;
            }           
         }
         
         {
            TLiwVariant boolVar((TBool)EFalse);
            
            /*TInt32 intRet;
            if(EFalse!=boolVar.Get((TInt32)intRet))
            {
                flag = 0;
            }*/
         }
         
         {//TInt64 check
                     TInt64 int64Ret;
                     if(EFalse==intVar.Get(int64Ret))
                     {
                         flag = 0;
                     }           
         }
         
         {//TReal check
                     TReal realRet;
                     if(EFalse==intVar.Get(realRet))
                     {
                         flag = 0;
                     }           
         }
         return !flag;           
    }
    
TBool Cliwtestcases::LIW_DATA_TYPES_016()
{
     TBool flag = 1;
     TInt32 intVal=10;
     TLiwVariant intVar((TInt32)intVal);
     
     {//AsList check
        const CLiwList* pRetList=intVar.AsList();
        
        if(NULL!=pRetList)
        {
            flag = 0;
        }
     }
     
     {//AsMap check
        const CLiwMap* pRetMap=intVar.AsMap();
        
        if(NULL!=pRetMap)
        {
            flag = 0;
        }
     }
     
     {//AsIterable check
        CLiwIterable* pRet=intVar.AsIterable();
        
        if(NULL!=pRet)
        {
            flag = 0;
        }
     }
     
     {//AsInterface check
        MLiwInterface* pRet=intVar.AsInterface();
        
        if(NULL!=pRet)
        {
            flag = 0;
        }
     }
     
     {//AsFileHandle check
        RFile pRet=intVar.AsFileHandle();
        
        //Don't check pRet for RFile
     }
     
     {
         CLiwBuffer* pRet = intVar.AsBuffer();
         if(NULL!=pRet)
                 {
                     flag = 0;
                 }
     }
     
     {
         TInt64 pRet = intVar.AsTInt64();
         if(pRet != NULL)
             flag = 1;
     }
     
     {
         const TTime pRet = intVar.AsTTime();
     }
     
     {
         TUid pRet = intVar.AsTUid();
     }
     
     {
         TBool pRet = intVar.AsTBool();
     }
     return !flag;           
}

TBool Cliwtestcases::LIW_DATA_TYPES_017()
{
    TBool flag = 0;
    TInt intServiceCmd=100;
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewLC(1, intServiceCmd, KContentTypeTxt);
    
    TBuf8<8> retStr=criteria->ServiceCmdStr();
    if(0!=retStr.Length())
    {
        CleanupStack::PopAndDestroy(criteria);
        flag = 0;
    }
    
    if(intServiceCmd==criteria->ServiceCmd())
    {
        CleanupStack::PopAndDestroy(criteria);
        flag = 1;
    }
    return !flag;
}

TBool Cliwtestcases::LIW_DATA_TYPES_018()
{
    TBool flag = 1;
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewL();
    CleanupStack::PushL(criteria);
    
    
    TBuf8<8> retStr=criteria->ServiceCmdStr();
    if(0!=retStr.Length())
    {
        flag = 0;
    }
    
    if(0!=criteria->ServiceCmd())
    {
        flag = 0;
    }
    
    CleanupStack::PopAndDestroy(criteria);
    
    return !flag;
}

TBool Cliwtestcases::LIW_DATA_TYPES_019()
{
    TBool flag = 0;
    /*Testing String Based Command*/
    RCriteriaArray interest;
    CleanupClosePushL(interest);
    
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    
    criteria->SetServiceClass(TUid::Uid(KLiwClassBase));

    interest.AppendL(criteria);
    
   // Attach the MyServiceName provider to the LIW framework.
    TInt status = iServiceHandler->AttachL(interest);
        
    TLiwGenericParam param;
    param.Value().Set(KLIW_DATA_TYPES_019);
    param.SetNameL(KTestPARAMNanmeTC1);
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
    inps->AppendL(param);
    param.Reset();
    
    // Get the Test result from MyServiceName provider "ExecuteServiceCmdL"
    CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    crit->SetServiceClass(TUid::Uid(KLiwClassBase));
    iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps);  
    CleanupStack::PopAndDestroy(crit); // crit
 
    
    TInt pos = 0;
    outps->FindFirst(pos, EGenericParamError);
    
    if(pos != KErrNotFound)
        {
            if ((*outps)[pos].Value().AsTInt32() == KErrNone)
                {
                flag = 1;
                }
            else
                {
                
                flag = 0;
                }
        }
     else
        {
        flag = 0;
        }

    // Detach The MyServiceName Provider
    iServiceHandler->DetachL(interest);    
    
    CleanupStack::PopAndDestroy(criteria);
    CleanupStack::PopAndDestroy(&interest);   
    return !flag;
}

TBool Cliwtestcases::LIW_DATA_TYPES_020()
{
    TBool flag = 1;
    TLiwVariant lhsVar;
    TLiwVariant rhsVar;
    
    if(EFalse == (lhsVar==rhsVar))
    {
        //Not matching - test case fails        
        flag = 0;        
    }
    
    
    {//For TUint check
        lhsVar.Set(TUint(10));
        rhsVar.Set(TUint(0));
        
        if(EFalse != (lhsVar==rhsVar))
        {
            //Not matching - test case fails        
            flag = 0;        
        }
        
        rhsVar.Set(TUint(10));
        
        //should match this time
        if(EFalse == (lhsVar==rhsVar))
        {
            flag = 0;        
        }
    
    }
    
    {//For TInt32 check
        lhsVar.Set(TInt32(10));
        rhsVar.Set(TInt32(0));
        
        if(EFalse != (lhsVar==rhsVar))
        {
            //Not matching - test case fails        
            flag = 0;        
        }
        
        rhsVar.Set(TInt32(10));
        
        //should match this time
        if(EFalse == (lhsVar==rhsVar))
        {
            //Not matching - test case fails        
            flag = 0;        
        }
    
    }
    
    {//For  TBool check
        lhsVar.Set(TBool(EFalse));
        rhsVar.Set(TBool(ETrue));
        
        if(EFalse != (lhsVar==rhsVar))
        {
            //Not matching - test case fails        
            flag = 0;        
        }
        
        rhsVar.Set(TBool(EFalse));
        
        //should match this time
        if(EFalse == (lhsVar==rhsVar))
        {
            //Not matching - test case fails        
            flag = 0;        
        }
    
    }
    
    {//For  TTime check
        lhsVar.Set(TTime(100));
        rhsVar.Set(TTime(200));
        
        if(EFalse != (lhsVar==rhsVar))
        {
            //Not matching - test case fails        
            flag = 0;        
        }
        
        rhsVar.Set(TTime(100));
        
        //should match this time
        if(EFalse == (lhsVar==rhsVar))
        {
            //Not matching - test case fails        
            flag = 0;        
        }
    
    }
    return !flag;
}

TBool Cliwtestcases::LIW_DATA_TYPES_021()
{
    TBool flag = 1;
    /*
    TLiwGenericParam param;
    param.Value().Set(KLIW_DATA_TYPES_001);
    param.SetNameL(KTestPARAMNanmeTC1);
    */
    
    //CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
    
    //inps->AppendL(param);
    //param.Reset();
    
    TInt pos = -1;
    outps->FindFirst(pos, EGenericParamError);
    
    
    if(pos != KErrNotFound)
        {
            flag = 0;            
        }

    pos = 0;
    outps->FindFirst(pos, EGenericParamError);
    if(pos != KErrNotFound)
        {
            flag = 0;            
        }  
        
    pos = -1;
    outps->FindNext(pos, EGenericParamError);
    
    if(pos != KErrNotFound)
        {
            flag = 0;            
        }

    pos = 0;
    outps->FindNext(pos, EGenericParamError);
    if(pos != KErrNotFound)
        {
            flag = 0;            
        }               
    return !flag;
}

TBool Cliwtestcases::LIW_DATA_TYPES_022()
{
    TBool flag = 0; 
    TLiwGenericParam param;
    param.Value().Set(TInt32(100));
    
    Dump(param.Value());
    
    param.SetNameL(KTestPARAMNanmeTC1);
    
    CLiwGenericParamList* pParamList = CLiwGenericParamList::NewLC();
    pParamList->AppendL(param);
    
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    
    inps->AppendL(*pParamList);
    
    TInt cnt = inps->Count();

    if(cnt != 0)
        {            
            flag = 1;                        
        }
        else
        {
            flag = 0;
        }
    
    param.Reset();
    inps->Reset();
    CleanupStack::PopAndDestroy(pParamList);
    return !flag;
}

TBool Cliwtestcases::LIW_DATA_TYPES_023()
{
    TBool flag = 0;
    //Reusing LIW_DATA_TYPES_011 test case
    TLiwGenericParam param;
    param.Value().Set(KLIW_DATA_TYPES_011);
    
    Dump(param.Value());
    
    param.SetNameL(KTestPARAMNanmeTC1);
    CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
    inps->AppendL(param);
    param.Reset();
    
    // Call ExecuteServiceCmdL TO GET THE RESULT
    CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    crit->SetServiceClass(TUid::Uid(KLiwClassBase));
    iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps); 
    CleanupStack::PopAndDestroy(crit); // crit
    
    // CHECK RESULT 
    TInt pos = 0;
    outps->FindFirst(pos, EGenericParamError);
    
    if(pos != KErrNotFound)
        {
            if ((*outps)[pos].Value().AsTInt32() == KErrNone)
                {
                
                _LIT8(KIter,"Iterator");
                TInt iterPos=0;
                outps->FindFirst(iterPos, KIter);
                if(pos != KErrNotFound)
                {
                    CLiwIterable* pIter=(*outps)[iterPos].Value().AsIterable();
                    
                    RWriteStream wstream;
                    TRAPD(err,pIter->ExternalizeL(wstream));
                    
                    if(KErrNotSupported!=err)
                    {
                        flag = 0;   
                    }
                    else
                    {
                        flag = 1;
                    }
                }
                else
                {
                    flag = 0;
                }
                }
            else
                {
                flag = 0;
                }
        }
     else
        {
        flag = 0;
        }
     return !flag;        
}

TBool Cliwtestcases::LIW_DATA_TYPES_024()
{
    TBool flag = 1;
    TLiwVariant intVar(TInt32(100));    
    Dump(intVar);
    
    TLiwVariant nullVar;    
    Dump(nullVar);
    
    Dump(TLiwVariant(TBool(ETrue)));    
    
    TLiwVariant uintVar(TUint(200));    
    Dump(uintVar);
    
    Dump(TLiwVariant(TUid::Uid(0x00)));    
    
    TLiwVariant timeVar(TTime(100));    
    Dump(timeVar);    

    RFile fHdl;
    TLiwVariant fHdlVar;
    fHdlVar.Set(fHdl);
    
    Dump(fHdlVar);  
    
    TBuf8<16> buff(_L8("Hello"));
    TLiwVariant bufVar(buff);
    
    Dump(bufVar);
    
    CLiwList* listType = CLiwDefaultList::NewL();
    
    /*Create list of 10 integer Varient Checks CLiwList::AppendL*/
    TLiwVariant intVairent;
    for (TInt32 index = 100; index < 110; index++)
        {
        intVairent.Set(index);
        listType->AppendL(intVairent);    
        }
        
    Dump(TLiwVariant(listType));
    
    listType->DecRef();
    
    CLiwMap* mapType = CLiwDefaultMap::NewL();
    
    /*Create Map of 10 integer Varient Checks CLiwMap::InsertL*/
    TLiwVariant contactNbr;
    TBuf8<32> contactName;
    for (TInt32 index = 100; index < 110; index++)
        {
        contactName.Format(KContact, index - 99);
        contactNbr.Set(index);
        mapType->InsertL(contactName, contactNbr);    
        }
        
    Dump(TLiwVariant(mapType));
    mapType->DecRef();
    return !flag;     
}

TBool Cliwtestcases::LIW_CLEANUP_LIW_001()
{
    TBool flag = 1;
    iServiceHandler->Reset();
    delete iServiceHandler;
    iServiceHandler=NULL;
    return !flag;
}

//for conversion utils
TBool Cliwtestcases::LIW_DATA_TYPES_025()
{
    TLiwVariant a1;
    TUint varUint;
    TInt32 varInt;
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varUint = 45;
    a1.Set(varUint);    //set TUint
        
    varInt = a1.AsTInt32();
    
    if(varUint == varInt)
        flag = 1;
    
    a1.Reset(); return !flag;    
    
}

TBool Cliwtestcases::LIW_DATA_TYPES_026()
{
    TLiwVariant a1;
    TUint varUint;
    TReal varReal;
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varUint = 45;
    a1.Set(varUint);    //set TUint
        
    varReal = a1.AsTReal();
    
    if(varUint == varReal)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

/*TBool Cliwtestcases::LIW_DATA_TYPES_027()
{
    TLiwVariant a1;
    TUint varUint;
    TBuf<255> varBuf;
    _LIT(KResult, "45");
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varUint = 45;
    a1.Set(varUint);    //set TUint
        
    //varBuf = a1.AsDes();  //Does not work. Use Get method   
    a1.Get(varBuf);   
    
    if(0 == varBuf.Compare(KResult))
        flag = 1;
    
    a1.Reset(); return !flag;    
}

TBool Cliwtestcases::LIW_DATA_TYPES_028()
{
    TLiwVariant a1;
    TUint varUint;
    TBuf8<255> varBuf8;
    _LIT8(KResult, "45");
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varUint = 45;
    a1.Set(varUint);    //set TUint
        
    //varBuf8 = a1.AsData();
    a1.Get(varBuf8 );
    
    if(0 == varBuf8.Compare(KResult))
        flag = 1;
    
    a1.Reset(); return !flag;    
}
*/

TBool Cliwtestcases::LIW_DATA_TYPES_029()
{
    TLiwVariant a1;
    TBool flag = 0;
    TUint varUint;
    TInt32 varInt;
    
    //conversion check
    //set variant for one type and get it in other types 
    varInt = 55;
    a1.Set(varInt);     //set TInt
        
    varUint = a1.AsTUint();
    
    if(varUint == varInt)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

TBool Cliwtestcases::LIW_DATA_TYPES_030()
{
    TLiwVariant a1;
    TInt32 varInt;
    TReal varReal;
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varInt = 55;
    a1.Set(varInt);     //set TInt
    varReal = a1.AsTReal();
    
    if(varInt == varReal)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

/*
TBool Cliwtestcases::LIW_DATA_TYPES_031()
{
    TLiwVariant a1;
    TInt32 varInt;
    TBuf<255> varBuf;
    
    _LIT(KResult, "55");
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varInt = 55;
    a1.Set(varInt);     //set TInt
    
    //varBuf = a1.AsDes();  //Does not work. Use Get method
    a1.Get(varBuf);
    
    if(0 == varBuf.Compare(KResult))
        flag = 1;
    
    a1.Reset(); return !flag;    
}

TBool Cliwtestcases::LIW_DATA_TYPES_032()
{
    TLiwVariant a1;
    TInt32 varInt;
    TBuf8<255> varBuf8;
    _LIT8(KResult, "55");
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varInt = 55;
    a1.Set(varInt);     //set TInt
        
    //varBuf8 = a1.AsData();
    a1.Get(varBuf8 );  a1.Get(varBuf8 );
    
    if(0 == varBuf8.Compare(KResult))
        flag = 1;
    
    a1.Reset(); return !flag;    
}
*/

TBool Cliwtestcases::LIW_DATA_TYPES_033()
{
    TLiwVariant a1;
    TUint varUint;
    TReal varReal;
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varReal = 65.2345;
    a1.Set(varReal);    //set TReal     
    varUint = a1.AsTUint();
    
    if(65 == varUint)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

TBool Cliwtestcases::LIW_DATA_TYPES_034()
{
    TLiwVariant a1;
    TInt32 varInt;
    TReal varReal;
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varReal = 65.2345;
    a1.Set(varReal);    //set TReal
    varInt = a1.AsTInt32();
    
    if(65 == varInt)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

/*
TBool Cliwtestcases::LIW_DATA_TYPES_035()
{
    TLiwVariant a1;
    TReal varReal;
    TBuf<255> varBuf;
    _LIT(KResult, "65.2345");
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varReal = 65.2345;
    a1.Set(varReal);    //set TReal
    
    //varBuf = a1.AsDes();  //Does not work. Use Get method 
    a1.Get(varBuf);
    
    if(0 == varBuf.Compare(KResult))
        flag = 1;
    
    a1.Reset(); return !flag;    
}

TBool Cliwtestcases::LIW_DATA_TYPES_036()
{
    TLiwVariant a1;
    TReal varReal;
    TBuf8<255> varBuf8;
    _LIT8(KResult, "65.2345");
    TBool flag = 0;
    
    //conversion check
    //set variant for one type and get it in other types 
    varReal = 65.2345;
    a1.Set(varReal);    //set TReal
    //varBuf8 = a1.AsData();
    a1.Get(varBuf8 );
    
    if(0 == varBuf8.Compare(KResult))
        flag = 1;
    
    a1.Reset(); return !flag;    
}
*/

TBool Cliwtestcases::LIW_DATA_TYPES_037()
{
    TLiwVariant a1;
    TReal varUint;
    TBuf8<255> varBuf8;
    _LIT8(KTest, "75");
    TBool flag = 0;
    varBuf8 = KTest;
    
    //conversion check
    //set variant for one type and get it in other types 
    a1.Set(varBuf8);    //set TDes8
    varUint = a1.AsTUint();
    
    if(75 == varUint)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

TBool Cliwtestcases::LIW_DATA_TYPES_038()
{
    TLiwVariant a1;
    TInt32 varInt;
    TBuf8<255> varBuf8;
    _LIT8(KTest, "75");
    TBool flag = 0;
    varBuf8 = KTest;
    
    //conversion check
    //set variant for one type and get it in other types 
    a1.Set(varBuf8);    //set TDes8
    varInt = a1.AsTInt32();
    
    if(75 == varInt)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

TBool Cliwtestcases::LIW_DATA_TYPES_039()
{
    TLiwVariant a1;
    TReal varReal;
    TBuf8<255> varBuf8;
    _LIT8(KTest, "75.897");
    TBool flag = 0;
    varBuf8 = KTest;
    
    //conversion check
    //set variant for one type and get it in other types 
    a1.Set(varBuf8);    //set TDes8
    varReal = a1.AsTReal();
    
    if(75.897 == varReal)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

/*
TBool Cliwtestcases::LIW_DATA_TYPES_040()
{
    TLiwVariant a1;
    TBuf<255> varBuf;
    TBuf8<255> varBuf8;
    _LIT8(KTest, "test");
    _LIT(KResult, "test");
    TBool flag = 0;
    
    varBuf8 = KTest;
    //conversion check
    //set variant for one type and get it in other types 
    a1.Set(varBuf8);    //set TDes8
    //varBuf = a1.AsDes();  //Does not work. Use Get method
    a1.Get(varBuf);
    
    if(0 == varBuf.Compare(KResult))
        flag = 1;
    
    a1.Reset(); return !flag;    
}
*/

TBool Cliwtestcases::LIW_DATA_TYPES_041()
{
    TLiwVariant a1;
    TReal varUint;
    TBuf<255> varBuf;
    _LIT(KTest, "75");
    TBool flag = 0;
    varBuf = KTest;
    
    //conversion check
    //set variant for one type and get it in other types 
    a1.Set(varBuf);     //set TDes
    varUint = a1.AsTUint();
    
    if(75 == varUint)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

TBool Cliwtestcases::LIW_DATA_TYPES_042()
{
    TLiwVariant a1;
    TInt32 varInt;
    TBuf<255> varBuf;
    _LIT(KTest, "75");
    TBool flag = 0;
    varBuf = KTest;
    
    //conversion check
    //set variant for one type and get it in other types 
    a1.Set(varBuf);     //set TDes
    varInt = a1.AsTInt32();
    
    if(75 == varInt)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

TBool Cliwtestcases::LIW_DATA_TYPES_043()
{
    TLiwVariant a1;
    TReal varReal;
    TBuf<255> varBuf;
    _LIT(KTest, "75.897");
    TBool flag = 0;
    varBuf = KTest;
    
    //conversion check
    //set variant for one type and get it in other types 
    a1.Set(varBuf);     //set TDes
    varReal = a1.AsTReal();
    
    if(75.897 == varReal)
        flag = 1;
    
    a1.Reset(); return !flag;    
}

/*
TBool Cliwtestcases::LIW_DATA_TYPES_044()
{
    TLiwVariant a1;
    TBuf<255> varBuf;
    TBuf8<255> varBuf8;
    _LIT(KTest, "test");
    _LIT8(KResult, "test");
    TBool flag = 0;
    varBuf = KTest;

    //conversion check
    //set variant for one type and get it in other types 
    a1.Set(varBuf);     //set TDes
    //varBuf8 = a1.AsData();
    a1.Get(varBuf8 );
    
    if(0 == varBuf8.Compare(KResult))
        flag = 1;
    
    a1.Reset(); return !flag;    
}
*/

TBool Cliwtestcases::LIW_DATA_TYPES_045()
{
    TBool flag = 0;
    /*Testing String Based Command*/
    RCriteriaArray interest, providerList;
    
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewL(1, KTestCommandTC1, KContentTypeTxt);
    criteria->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    interest.AppendL(criteria);
    
    iServiceHandler->QueryImplementationL(interest, providerList);

    if(providerList.Count())
    {
        if(0 == ((*(providerList[0])).ServiceCmdStr().Compare((*criteria).ServiceCmdStr())))
        {
            if(0 == ((*(providerList[0])).ContentType().Compare((*criteria).ContentType())))
            {
                providerList.ResetAndDestroy();
                interest.ResetAndDestroy();   
                flag = 1;       
            }
            
        }         
    }
    else
    {
        providerList.ResetAndDestroy();
        interest.ResetAndDestroy();   
        flag = 0;
    }
    
    return !flag;
}
    
TBool Cliwtestcases::LIW_DATA_TYPES_046()
{
    TBool flag = 0;
    /*Testing String Based Command*/
    RCriteriaArray interest, providerList;
    _LIT8(KJunkContentTypeTxt, "text");
    
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewL(1, KTestCommandTC1, KJunkContentTypeTxt);
    criteria->SetServiceClass(TUid::Uid(KLiwClassBase));

    interest.AppendL(criteria);
    
    iServiceHandler->QueryImplementationL(interest, providerList);
    
    
     
     
    if(!providerList.Count())
    {
         flag = 1;
    }
    else
    {
        flag = 0;
    }

    providerList.ResetAndDestroy();
    interest.ResetAndDestroy();   
    return !flag;
}

TBool Cliwtestcases::LIW_DATA_TYPES_047()
{
    TBool flag = 0;
    /*Testing String Based Command*/
    RCriteriaArray interest, providerList;
    
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewL(1, KWild, KContentTypeTxt);
    criteria->SetServiceClass(TUid::Uid(KLiwClassBase));

    interest.AppendL(criteria);
    
    iServiceHandler->QueryImplementationL(interest, providerList);
    
    
     
     
    if(providerList.Count())
    {
        if(0 == ((*(providerList[0])).ContentType().Compare((*criteria).ContentType())))
        {
            providerList.ResetAndDestroy();
            interest.ResetAndDestroy();   
            flag = 1;       
        }       
    }
    else
    {
        providerList.ResetAndDestroy();
        interest.ResetAndDestroy();   
        flag = 0;
    }
    
    return !flag;
}

TBool Cliwtestcases::LIW_DATA_TYPES_048()
{
    TBool flag = 0;
    /*Testing String Based Command*/
    RCriteriaArray interest, providerList;
    
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewL(1, KTestCommandTC1, KWild);
    criteria->SetServiceClass(TUid::Uid(KLiwClassBase));

    interest.AppendL(criteria);
    
    iServiceHandler->QueryImplementationL(interest, providerList);
    
    
     
     
    if(providerList.Count())
    {
        if(0 == ((*(providerList[0])).ServiceCmdStr().Compare((*criteria).ServiceCmdStr())))
        {
            providerList.ResetAndDestroy();
            interest.ResetAndDestroy();
            flag = 1;       
        }         
    }
    else
    {
        providerList.ResetAndDestroy();
        interest.ResetAndDestroy();
        flag = 0;
    }
    return !flag;
}

TBool Cliwtestcases::LIW_DATA_TYPES_049()
{
    TBool flag = 0;
    /*Testing String Based Command*/
    RCriteriaArray interest, providerList;
    
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewL(1, KWild, KWild);     //Queries for a list of all the LIW providers present
    criteria->SetServiceClass(TUid::Uid(KLiwClassBase));          //Presently this value is changed to differentiate LIW Providers from other Ecom plug-ins
    
    TReal mini = 2.0;
    TReal maxi = 3.0;
    TLiwVariant mdOption;
    CLiwMap* pMetaDataMap = CLiwDefaultMap::NewL();
    CLiwList* pRangeList = CLiwDefaultList::NewL();
    _LIT8(KVersion,"ver");
    _LIT8(KRange,"range");
    
    pRangeList->AppendL(TLiwVariant(KVersion));
    pRangeList->AppendL(TLiwVariant(TReal(mini)));
    pRangeList->AppendL(TLiwVariant(TReal(maxi)));
    
    pMetaDataMap->InsertL(KRange,pRangeList);
    mdOption.Set(pMetaDataMap);
    criteria->SetMetaDataOptions(mdOption);

    pRangeList->DecRef();
    pMetaDataMap->DecRef();
    mdOption.Reset();
    
    interest.AppendL(criteria);
    
    iServiceHandler->QueryImplementationL(interest, providerList);      //providerList contains the list of all the LIW providers with metadata information being loaded for each
    
    RCriteriaArray loadCritArray;                        
      
    for(TInt idx = 0; idx < providerList.Count(); ++idx)         // for each provider
    {
      CLiwCriteriaItem* provInfo = providerList[idx];      
      
      TUid interfaceId = provInfo->ServiceClass();      //retrieving the service class (interface Id) of the provider
      TPtrC8 intfName = provInfo->ServiceCmdStr();      //retrieving the content type or the interface name of the provider (e.g IDataSource)
      TPtrC8 servName = provInfo->ContentType();        //retrieving the service command name of the provider (e.g s60.Messaging)
      TReal provVersion = 1.0;                          //since Default version is 1.0 for all the providers
      
      //Creating a criteria using the above extracted information
      CLiwCriteriaItem* item = CLiwCriteriaItem::NewL(1, intfName, servName);
      item->SetServiceClass(interfaceId);
                         
      TLiwVariant provMetadata;
      provInfo->GetMetaDataOptions(provMetadata);      //getting metadata from the provider (contains version information)
      
      const CLiwMap* metaDataMap = provMetadata.AsMap(); 
            
      if(metaDataMap)
      {
         _LIT8(KVersionKey,"ver");         //Key name in the metadata map is "ver" (as specified in the provider RSS File)
         TLiwVariant versionVar;
         
         if(metaDataMap->FindL(KVersionKey, versionVar))    //This should be true. If false this means that there is no version information in the provider metadata
         {
         /*   ------------------------------------
            |   key   |        value      |
            |         |                   |
            |   ver   |      (List) 2.2   |
            -------------------------------------

         */ 
            const CLiwList* pVersionList = versionVar.AsList();
            if(pVersionList)
            {
               TLiwVariant verCheck;
               for(TInt idx=0; idx < pVersionList->Count(); ++idx)         //the count is always ZERO. But to be on safer side, I have given the loop
               {
                  if(pVersionList->AtL(idx,verCheck))      //Ideally the index should only be Zero. But the version, if exists in any index will be returned now
                  {
                     provVersion = verCheck.AsTReal();   //The provider version is retrieved from the metadata
                     
                     //using switch case or if stmts further. This implementation is left to the Consumer's logic. But just a sample is shown below
                     if(2.5 == provVersion || 2.4 == provVersion)         
                     {
                        TLiwVariant mdOption1;      //Setting the metadata option with the version information. 
                        CLiwMap* pMetaDataMap = CLiwDefaultMap::NewL();
                        CLiwList* pRangeList = CLiwDefaultList::NewL();
                        _LIT8(KVersion,"ver");      //Key in metadata for version is "version"
                        _LIT8(KRange,"range");      //Since this is an exact version match done in AttachL, hence the key "exact"
                        
                        pRangeList->AppendL(TLiwVariant(KVersion));
                        pRangeList->AppendL(TLiwVariant(provVersion));
                        pRangeList->AppendL(TLiwVariant(provVersion));
                        
                        pMetaDataMap->InsertL(KRange,pRangeList);
                        mdOption1.Set(pMetaDataMap);
                        item->SetMetaDataOptions(mdOption1); //metadata is set with the version information

                        pRangeList->DecRef();
                        pMetaDataMap->DecRef();
                        mdOption1.Reset();
                        
                        loadCritArray.AppendL(item);
                        
                        iServiceHandler->DetachL(loadCritArray);
                        
                        TInt retVal = iServiceHandler->AttachL(loadCritArray); // This just loads the provider sent as input. No version range is to be sent here,
                        
                        if(retVal >= 0)
                        {
                            flag = 1;
                        }
        
                        iServiceHandler->DetachL(loadCritArray);
                     }
                     else if(2.6 == provVersion)
                     {
                        // statements
                     }
                  }
                  
                  verCheck.Reset();
               }
            }
         }
         versionVar.Reset();
      }
      provMetadata.Reset();
   }
   
    loadCritArray.ResetAndDestroy(); 
    providerList.ResetAndDestroy();
    interest.ResetAndDestroy();

    return !flag;
}

TBool Cliwtestcases::LIW_MDAT_VER1()
{
    TBool flag = 0;
    CLiwCriteriaItem* item1 = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    item1->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    TReal mini = 2.0;
    TReal maxi = 3.0;
    TLiwVariant mdOption1;
    CLiwMap* pMetaDataMap = CLiwDefaultMap::NewL();
    CLiwList* pRangeList = CLiwDefaultList::NewL();
    _LIT8(KVersion,"ver");
    _LIT8(KRange,"range");
    
    pRangeList->AppendL(TLiwVariant(KVersion));
    pRangeList->AppendL(TLiwVariant(TReal(mini)));
    pRangeList->AppendL(TLiwVariant(TReal(maxi)));
    
    pMetaDataMap->InsertL(KRange,pRangeList);
    mdOption1.Set(pMetaDataMap);
    item1->SetMetaDataOptions(mdOption1);

    pRangeList->DecRef();
    pMetaDataMap->DecRef();
    mdOption1.Reset();
    
    RCriteriaArray critArray, providerList;
    
    critArray.AppendL(item1);
    
    iServiceHandler->QueryImplementationL(critArray, providerList);
    
    TInt status = iServiceHandler->AttachL(providerList);
    
    
    if(status > 0)
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }
    
    CleanupStack::Pop();
    iServiceHandler->DetachL(providerList);
    critArray.ResetAndDestroy();
    providerList.ResetAndDestroy();
    return !flag;
}


TBool Cliwtestcases::LIW_MDAT_VER2()
{
    TBool flag = 0;
    CLiwCriteriaItem* item2 = CLiwCriteriaItem::NewLC(2, KTestCommandTC1, KContentTypeTxt);
    item2->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    TReal mini = 0.0;
    TReal maxi = 3.0;
    CLiwMap* pMetaDataMap = CLiwDefaultMap::NewL();
    CLiwList* pRangeList = CLiwDefaultList::NewL();
    TLiwVariant mdOption1;
    _LIT8(KVersion,"ver");
    _LIT8(KRange,"range");
    
    pRangeList->AppendL(TLiwVariant(KVersion));
    pRangeList->AppendL(TLiwVariant(TReal(mini)));
    pRangeList->AppendL(TLiwVariant(TReal(maxi)));
    
    pMetaDataMap->InsertL(KRange,pRangeList);
    mdOption1.Set(pMetaDataMap);
    item2->SetMetaDataOptions(mdOption1);
    
    pRangeList->DecRef();
    pMetaDataMap->DecRef();
    mdOption1.Reset();
        
    RCriteriaArray critArray, providerList;
    
    critArray.AppendL(item2);
    
    iServiceHandler->QueryImplementationL(critArray, providerList);
    
    iServiceHandler->AttachL(providerList);
    
    
    if(providerList.Count() >= 0)
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }
    
    CleanupStack::Pop();
    iServiceHandler->DetachL(providerList);
    critArray.ResetAndDestroy();
    providerList.ResetAndDestroy();
    return !flag;
}


TBool Cliwtestcases::LIW_MDAT_VER3()
{
    TBool flag = 0;
    CLiwCriteriaItem* item3 = CLiwCriteriaItem::NewLC(3, KTestCommandTC1, KContentTypeTxt);
    item3->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    TReal mini = 2.0;
    TReal maxi = 0.0;
    CLiwMap* pMetaDataMap = CLiwDefaultMap::NewL();
    CLiwList* pRangeList = CLiwDefaultList::NewL();
    TLiwVariant mdOption1;
    _LIT8(KVersion,"ver");
    _LIT8(KRange,"range");
    
    pRangeList->AppendL(TLiwVariant(KVersion));
    pRangeList->AppendL(TLiwVariant(TReal(mini)));
    pRangeList->AppendL(TLiwVariant(TReal(maxi)));
    
    pMetaDataMap->InsertL(KRange,pRangeList);
    mdOption1.Set(pMetaDataMap);
    item3->SetMetaDataOptions(mdOption1);
    
    pRangeList->DecRef();
    pMetaDataMap->DecRef();
    mdOption1.Reset();
        
    RCriteriaArray critArray, providerList;
    
    critArray.AppendL(item3);
    
    iServiceHandler->QueryImplementationL(critArray, providerList);
    
    iServiceHandler->AttachL(providerList);
    
    if(providerList.Count() >= 0)
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }

    CleanupStack::Pop();
    iServiceHandler->DetachL(providerList);
    critArray.ResetAndDestroy();
    providerList.ResetAndDestroy();
    return !flag;
}


TBool Cliwtestcases::LIW_MDAT_VER4()
{   
    TBool flag = 0;
   CLiwCriteriaItem* item4 = CLiwCriteriaItem::NewLC(4, KTestCommandTC1, KContentTypeTxt);
    item4->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    TReal mini = 0.0;
    TReal maxi = 0.0;
    CLiwMap* pMetaDataMap = CLiwDefaultMap::NewL();
    CLiwList* pRangeList = CLiwDefaultList::NewL();
    TLiwVariant mdOption1;
    _LIT8(KVersion,"ver");
    _LIT8(KRange,"range");
    
    pRangeList->AppendL(TLiwVariant(KVersion));
    pRangeList->AppendL(TLiwVariant(TReal(mini)));
    pRangeList->AppendL(TLiwVariant(TReal(maxi)));
    
    pMetaDataMap->InsertL(KRange,pRangeList);
    mdOption1.Set(pMetaDataMap);
    item4->SetMetaDataOptions(mdOption1);
    
    pRangeList->DecRef();
    pMetaDataMap->DecRef();
    mdOption1.Reset();
        
    RCriteriaArray critArray, providerList;
    
    critArray.AppendL(item4);
    
    iServiceHandler->QueryImplementationL(critArray, providerList);
    
    iServiceHandler->AttachL(providerList);
    
    
    if(providerList.Count() >= 0)
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }
    
    CleanupStack::Pop();
    iServiceHandler->DetachL(providerList);
    critArray.ResetAndDestroy();
    providerList.ResetAndDestroy();
    return !flag;
}

TBool Cliwtestcases::LIW_MDAT_VER5()
{
    TBool flag = 0;
    CLiwCriteriaItem* item5 = CLiwCriteriaItem::NewL();
    item5->SetId(5);
    item5->SetServiceCmdL(KTestCommandTC1);
    item5->SetContentTypeL(KContentTypeTxt);
    item5->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    TReal mini = 3.0;
    TReal maxi = 2.0;
    CLiwMap* pMetaDataMap = CLiwDefaultMap::NewL();
    CLiwList* pRangeList = CLiwDefaultList::NewL();
    TLiwVariant mdOption1;
    _LIT8(KVersion,"ver");
    _LIT8(KRange,"range");
    
    pRangeList->AppendL(TLiwVariant(KVersion));
    pRangeList->AppendL(TLiwVariant(TReal(mini)));
    pRangeList->AppendL(TLiwVariant(TReal(maxi)));
    
    pMetaDataMap->InsertL(KRange,pRangeList);
    mdOption1.Set(pMetaDataMap);
    item5->SetMetaDataOptions(mdOption1);
    
    pRangeList->DecRef();
    pMetaDataMap->DecRef();
    mdOption1.Reset();
        
    RCriteriaArray critArray, providerList;
    
    critArray.AppendL(item5);
    
    iServiceHandler->QueryImplementationL(critArray, providerList);
    
    iServiceHandler->AttachL(providerList);
    
    if(providerList.Count() >= 0) 
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }
    
    iServiceHandler->DetachL(providerList);
    critArray.ResetAndDestroy();
    providerList.ResetAndDestroy();
    return !flag;
}

TBool Cliwtestcases::LIW_MDAT_CAP1()
{
    TBool flag = 1;
    //TInt status = 0;
    iServiceHandler->Reset();
    CLiwCriteriaItem* item1 = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    item1->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    TReal mini = 2.0;
    TReal maxi = 5.0;
    TLiwVariant mdOption1;
    CLiwMap* pMetaDataMap = CLiwDefaultMap::NewL();
    CLiwList* pRangeList = CLiwDefaultList::NewL();
    _LIT8(KVersion,"ver");
    _LIT8(KRange,"range");
    
    pRangeList->AppendL(TLiwVariant(KVersion));
    pRangeList->AppendL(TLiwVariant(TReal(mini)));
    pRangeList->AppendL(TLiwVariant(TReal(maxi)));
    
    pMetaDataMap->InsertL(KRange,pRangeList);
    mdOption1.Set(pMetaDataMap);
    item1->SetMetaDataOptions(mdOption1);

    pRangeList->DecRef();
    pMetaDataMap->DecRef();
    mdOption1.Reset();
    
    RCriteriaArray critArray, providerList;
    
    critArray.AppendL(item1);
    iServiceHandler->QueryImplementationL(critArray, providerList);
                        
    
    // beginning of securitymanager code
    
    CRTSecManager* iSession = CRTSecManager::NewL();
    CTrustInfo* iTrust = CTrustInfo::NewL();
    
    RFs fileSession;
    if(KErrNone==fileSession.Connect())
    {
        CleanupClosePushL(fileSession);
        if(KErrNone==fileSession.ShareProtected())
        {
            RFile secPolicyFile;    
            if(KErrNone == secPolicyFile.Open(fileSession, _L("c:\\data\\others\\liwaccesspolicy.xml"), EFileShareAny ))
            {
                CleanupClosePushL(secPolicyFile);
                TPolicyID iPolicyId = iSession->SetPolicy(secPolicyFile);
                
                if(iPolicyId>KErrNone)
                {
                    TInt32 exId = iSession->RegisterScript(iPolicyId, *iTrust);
                    
                    CRTSecMgrScriptSession* scriptSession = NULL;
                    
                    //#ifdef  AUTO_TESTING
                                        scriptSession = iSession->GetScriptSessionL(iPolicyId, exId, this);
                    //#else
                      //                  scriptSession = iSession->GetScriptSessionL(iPolicyId, exId);
                    //#endif

                    //CRTSecMgrScriptSession* scriptSession = iSession->GetScriptSession(iPolicyId,exId);

                    TInt status = iServiceHandler->AttachL(critArray,*scriptSession);
                    
                    delete scriptSession;
                    
                }
                
                CleanupStack::PopAndDestroy();//secPolicyFile
            }       
        }
        
        CleanupStack::PopAndDestroy();//fileSession
        
    }

    delete iTrust;
    delete iSession;
    // end of securitymanager code
    
     
    if(providerList.Count())
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }

    CleanupStack::Pop(item1); 
    critArray.ResetAndDestroy();
    providerList.ResetAndDestroy();
    return !flag;
}

TBool Cliwtestcases::LIW_MDAT_CAP2()
{
    TBool flag = 0;
    TInt status = 0;
    iServiceHandler->Reset();
    CLiwCriteriaItem* item1 = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
    item1->SetServiceClass(TUid::Uid(KLiwClassBase));
    
    TReal mini = 5.0;
    TReal maxi = 7.0;
    TLiwVariant mdOption1;
    CLiwMap* pMetaDataMap = CLiwDefaultMap::NewL();
    CLiwList* pRangeList = CLiwDefaultList::NewL();
    _LIT8(KVersion,"ver");
    _LIT8(KRange,"range");
    
    pRangeList->AppendL(TLiwVariant(KVersion));
    pRangeList->AppendL(TLiwVariant(TReal(mini)));
    pRangeList->AppendL(TLiwVariant(TReal(maxi)));
    
    pMetaDataMap->InsertL(KRange,pRangeList);
    mdOption1.Set(pMetaDataMap);
    item1->SetMetaDataOptions(mdOption1);

    pRangeList->DecRef();
    pMetaDataMap->DecRef();
    mdOption1.Reset();
    
    RCriteriaArray critArray, providerList;
    
    critArray.AppendL(item1);
    
    // beginning of securitymanager code
    
    CRTSecManager* iSession = CRTSecManager::NewL();
    CTrustInfo* iTrust = CTrustInfo::NewL();
    
    RFs fileSession;
    if(KErrNone==fileSession.Connect())
    {
        CleanupClosePushL(fileSession);
        if(KErrNone==fileSession.ShareProtected())
        {
            RFile secPolicyFile;    
            if(KErrNone == secPolicyFile.Open(fileSession, _L("c:\\data\\Others\\AccessPolicy_V1.xml"), EFileShareAny ))
            {
                CleanupClosePushL(secPolicyFile);
                TPolicyID iPolicyId = iSession->SetPolicy(secPolicyFile);
                
                if(iPolicyId>KErrNone)
                {
                    TInt32 exId = iSession->RegisterScript(iPolicyId, *iTrust);
                    CRTSecMgrScriptSession* scriptSession = iSession->GetScriptSessionL(iPolicyId,exId);

                    iServiceHandler->QueryImplementationL(critArray, providerList);
                    
                    status = iServiceHandler->AttachL(critArray,*scriptSession);
                    
                    delete scriptSession;
                }
                
                CleanupStack::PopAndDestroy();//secPolicyFile
            }       
        }
        
        CleanupStack::PopAndDestroy();//fileSession
    }

    delete iTrust;
    delete iSession;
    // end of securitymanager code

    if(!status > 0)
        {
        flag = 1;
        }
    else
        {
        flag = 0;
        }

    CleanupStack::Pop(item1); 
    critArray.ResetAndDestroy();
    providerList.ResetAndDestroy();
    return !flag;
}

TBool Cliwtestcases::LIW_ContainerCleanup()
{
   
    __UHEAP_MARK;
    _LIT(KName,"Name");
    TLiwVariant variant(KName());
    CLiwMap* map = CLiwDefaultMap::NewL();
    CLiwList *listTemp = CLiwDefaultList::NewL(); 
    map->PushL();
    listTemp->PushL();    
    //CleanupClosePushL(*map);
    //CleanupClosePushL(*listTemp);
    map->InsertL(KNullDesC8,variant);
    TRAP_IGNORE(TestL(map));
    CleanupStack::PopAndDestroy(listTemp);
    CleanupStack::PopAndDestroy(map);
    __UHEAP_MARKEND;
    
    return KErrNone;   
}

void Cliwtestcases::TestL(CLiwMap *map)
{
    CLiwGenericParamList* param = CLiwGenericParamList::NewLC();
    param->AppendL(TLiwGenericParam(KNullDesC8,TLiwVariant(map)));
    User::Leave(KErrGeneral);
    param->Reset();
    CleanupStack::Pop(param);
}

TBool Cliwtestcases::LIW_ParamCleanup()
{
    __UHEAP_MARK;
    CLiwGenericParamList* param = CLiwGenericParamList::NewLC();
    //CLiwMap* map = CLiwDefaultMap::NewL();
    //CleanupClosePushL(*map);
    //map->InsertL(KNullDesC8,variant);
    TLiwGenericParam par;
    TLiwVariant var;
    par.SetNameAndValueL(KNullDesC8, var);
    //param->AppendL(TLiwGenericParam(KNullDesC8,TLiwVariant(map)));
    param->AppendL(par);
    //CleanupStack::PopAndDestroy(map);
    
    TRAP_IGNORE(TestParamL(param));
    
    par.Reset();
    CleanupStack::PopAndDestroy(param);
    __UHEAP_MARKEND;
    
    return KErrNone;
}

void Cliwtestcases::TestParamL(CLiwGenericParamList *param)
{
    TLiwGenericParam outParam;
    outParam.PushL();
    //Pushing the TLiwGenericParam into the CleanupStack
    //CleanupStack::PushL( TCleanupItem( TLiwGenericParam::ParamCleanup , &outParam ) ); 
    
    param->AtL(0,outParam);
    const CLiwMap* outMap = outParam.Value().AsMap();
    
    //Leaves before CleanupStack::Pop and reset is called
    User::Leave(KErrGeneral);
    
    //CleanupStack::Pop(&outParam);
    outParam.Reset();
}

TBool Cliwtestcases::LIW_VariantCleanup()
{
    
    __UHEAP_MARK;
    _LIT(KName,"Name");
    TLiwVariant variant(KName());
    CLiwMap* map = CLiwDefaultMap::NewL();
    map->PushL();
    //CleanupClosePushL(*map);
    //map->InsertL(KNullDesC8,variant);
    
    TRAP_IGNORE(TestVariantL(map));
    
    CleanupStack::Pop(map);                 
    map->DecRef();                      //map is destroyed
    __UHEAP_MARKEND;            //There is no memory Leak since the TLiwVariant "var" is destroyed by the CleanupStack during User::Leave

    
    return KErrNone;               
} 

void Cliwtestcases::TestVariantL(CLiwMap* map)
{
  
    TLiwVariant var;
    var.PushL();
    //CleanupStack::PushL( TCleanupItem( TLiwVariant::VariantCleanup , &var) );
    
    map->FindL(KNullDesC8,var);         //This makes a copy of TLiwVariant "variant" to "var"
    User::Leave(KErrNoMemory);          //The TLiwVariant "var" is popped from the CleanupStack and VariantCleanup method is called
    
    CleanupStack::Pop(&var);        //These statements are not executed
    var.Reset();                                            //This statement is not executed resulting in memory leak
}


TBool Cliwtestcases::LIW_DATA_TYPES_050()
{
    TLiwVariant a1;
    TInt32 varInt32;
    TInt64 varInt64 = 0xaaaaaaa;
    TBool flag = 0;
    
    //First checking the TInt64 datatype
    a1.Set(varInt64);
    
    Dump(a1);
    //conversion check
    //set variant for TInt64 and get it in TInt32
    varInt32 = a1.AsTInt32();   //get TInt32
    
    if(varInt32 == varInt64)
        flag = 0;
    
    TUint varUint;
    varUint = a1.AsTUint();     //get TUint
    
    if(varUint == varInt64)
        flag = 0;
    
    TReal varReal;
    varReal = a1.AsTReal();     //get TReal
    
    if(varReal == varInt64)
        flag = 0;
    
    TBuf<20> varBuf;
    //varBuf = a1.AsDes();  //Does not work. Use Get method    
    a1.Get(varBuf);     //get TDes
    
    TBuf8<20> varBuf8;
    //varBuf8 = a1.AsData();
    a1.Get(varBuf8 );   //get TDes8
        
    a1.Reset(); 
    
    flag = 1;
    return !flag;    
}

TInt Cliwtestcases::LIW_ASYNC_001()
    {
    return asyObj->AsyncCase(1);
    }

TInt Cliwtestcases::LIW_ASYNC_002()
    {
    return asyObj->AsyncCase(2);
    }

TInt Cliwtestcases::LIW_ASYNC_003()
    {
    return asyObj->AsyncCase(3);
    }

TInt Cliwtestcases::LIW_ASYNC_004()
    {
    return asyObj->AsyncCase(4);
    }

TInt Cliwtestcases::LIW_ASYNC_LongServiceCmd()
    {
        TBool flag = 0;
        /*Testing String Based Command*/
        RCriteriaArray interest;
        CleanupClosePushL(interest);
        _LIT8(KLongCmd,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        _LIT8(KNoCmd,"");
       
        /******For a different test************/
        CLiwCriteriaItem* criteria1 = CLiwCriteriaItem::NewLC(1, KLongCmd, KNoCmd);
        criteria1->SetServiceCmd(1);
        CleanupStack::PopAndDestroy(criteria1);
        /******End For a different test************/
        
        
        
        
        
        CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewLC(1, KLongCmd, KNoCmd);
        
        criteria->SetServiceClass(TUid::Uid(KLiwClassBase));
        
        

        interest.AppendL(criteria);
        TInt status = 0;
        // Attach the MyServiceName provider to the LIW framework.
        TRAPD(status1,status = iServiceHandler->AttachL(interest));
         
        if(!status || status1)
            {
            iServiceHandler->DetachL(interest);    
            CleanupStack::PopAndDestroy(criteria);
            CleanupStack::PopAndDestroy(&interest);   
            return KErrGeneral;
            }

        
        // Detach The MyServiceName Provider
        iServiceHandler->DetachL(interest);    
        CleanupStack::PopAndDestroy(criteria);
        CleanupStack::PopAndDestroy(&interest);   
        return KErrNone;
    }
    

TInt Cliwtestcases::LIW_ASYNC_LongInvalidServiceCmd()
    {
        TBool flag = 0;
        /*Testing String Based Command*/
        RCriteriaArray interest;
        CleanupClosePushL(interest);
        _LIT8(KLongCmd,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        _LIT8(KNoCmd,"");
       
        CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewLC(1, KLongCmd, KNoCmd);
       
        criteria->SetServiceClass(TUid::Uid(KLiwClassBase));

        interest.AppendL(criteria);
       
        // Attach the MyServiceName provider to the LIW framework.
        TRAPD(status1,iServiceHandler->AttachL(interest));
       
        TInt status = status1;
       
        
        //iServiceHandler->DetachL(interest);    
        CleanupStack::PopAndDestroy(criteria);
        CleanupStack::PopAndDestroy(&interest);
        if(status == KLiwUnknown)
            {
       
            return KErrNone;
            }
        
        else
            return KErrGeneral;

       
    }
    

TInt Cliwtestcases::LIW_ASYNC_LongInterface()
    {
        TBool flag = 0;
        /*Testing String Based Command*/
        RCriteriaArray interest;
        CleanupClosePushL(interest);
        _LIT8(KNoCmd,"");
        _LIT8(KLongCmd,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewLC(1, KNoCmd, KLongCmd);
        criteria->SetServiceClass(TUid::Uid(KLiwClassBase));

        interest.AppendL(criteria);
        TInt status = 0;
        // Attach the MyServiceName provider to the LIW framework.
        TRAPD(status1,status  = iServiceHandler->AttachL(interest));
       

       // Detach The MyServiceName Provider
       iServiceHandler->DetachL(interest);    
       CleanupStack::PopAndDestroy(criteria);
       CleanupStack::PopAndDestroy(&interest);
       
       if(!status || status1)
           {
           return KErrGeneral;
           }
       
          
       return KErrNone;
    }
    

TInt Cliwtestcases::LIW_ASYNC_LongInvalidInterface()
    {
        TBool flag = 0;
        /*Testing String Based Command*/
        RCriteriaArray interest;
        CleanupClosePushL(interest);
        _LIT8(KNoCmd,"");
        _LIT8(KLongCmd,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewLC(1, KNoCmd, KLongCmd);
        criteria->SetServiceClass(TUid::Uid(KLiwClassBase));

        interest.AppendL(criteria);
        TInt status ;
        // Attach the MyServiceName provider to the LIW framework.
        TRAPD(status1,status = iServiceHandler->AttachL(interest));
       

       // Detach The MyServiceName Provider
       //iServiceHandler->DetachL(interest);    
       CleanupStack::PopAndDestroy(criteria);
       CleanupStack::PopAndDestroy(&interest);
       
       if(status1 == KLiwUnknown)
           {
           return KErrNone;
           }
       
          
       return KErrGeneral;
    }


TInt Cliwtestcases::LIW_ASYNC_LongInterface1()
    {
        TBool flag = 0;
        /*Testing String Based Command*/
        RCriteriaArray interest;
        CleanupClosePushL(interest);
        _LIT8(KNoCmd,"aaaaaaaaaaaaaaaaaa"); //18
        _LIT8(KLongCmd,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"); //220
        CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewLC(1, KNoCmd, KLongCmd);
        criteria->SetServiceClass(TUid::Uid(KLiwClassBase));

        interest.AppendL(criteria);
        TInt status = 0;
        // Attach the MyServiceName provider to the LIW framework.
        TRAPD(status1,status = iServiceHandler->AttachL(interest));
       

       // Detach The MyServiceName Provider
       iServiceHandler->DetachL(interest);    
       CleanupStack::PopAndDestroy(criteria);
       CleanupStack::PopAndDestroy(&interest);
       
       if(!status || status1)
           {
           return KErrGeneral;
           
           }
       
       return KErrNone; 
      
    }

TInt Cliwtestcases::LIW_ParamCleanup1()
{
    TRAP_IGNORE(TestParam1L());
    
    return KErrNone;
}

void Cliwtestcases::TestParam1L()
{    
    TLiwGenericParam param;
    CleanupStack::PushL( TCleanupItem( TLiwGenericParam::ParamCleanup , &param ) ); 
    User::Leave(KErrGeneral);  //This calls the ParamCleanup method which cleans up TLiwGenericParam 
    CleanupStack::Pop(&param);
    param.Reset();
    //Leaves before CleanupStack::Pop and reset is called
    User::Leave(KErrGeneral);
    
}


TInt Cliwtestcases::LIW_GetInterest()
{

        RCriteriaArray interest1;
        
        iServiceHandler->GetInterest(interest1);
        CLiwCriteriaItem* item1 = CLiwCriteriaItem::NewLC();
        for(TInt i = 0; i < interest1.Count(); i++)
               {
               
                   
               item1->SetId(interest1[i]->Id());
               if (interest1[i]->ServiceCmd() == KLiwCmdAsStr)
                 item1->SetServiceCmdL(    interest1[i]->ServiceCmdStr()         );
               else
                 item1->SetServiceCmd(interest1[i]->ServiceCmd());
               
               item1->SetContentTypeL(     interest1[i]->ContentType()           );
               item1->SetServiceClass(     interest1[i]->ServiceClass()          );
                
               }
        CleanupStack::PopAndDestroy(item1);
        return KErrNone;
        
}



TBool Cliwtestcases::LIW_Defaut_list_externalize()
{

    _LIT8(KText,"Test");
    _LIT8(KExamplekey,"key");
    CLiwGenericParamList* pList = CLiwGenericParamList::NewLC();
    RBuf8 binaryData;
    binaryData.Create(100);
    binaryData.Append(KText);
    TLiwGenericParam param;
    TPtrC8 ptrVar(KExamplekey);
    param.SetNameAndValueL(ptrVar, TLiwVariant( binaryData ));
    
    pList->AppendL(param);
    //stringsMap->InsertL( KExamplekey, TLiwVariant( binaryData ));
    
    RBuf8 datadesc;
    CleanupClosePushL( datadesc );
    datadesc.CreateL( pList->Size() );
    RDesWriteStream datastrm(datadesc);
    CleanupClosePushL( datastrm );
    TRAPD(err,pList->ExternalizeL( datastrm ));
    
    datastrm.CommitL( );
    CleanupStack::PopAndDestroy( &datastrm );
    CleanupStack::PopAndDestroy( &datadesc );
    binaryData.Close();
    CleanupStack::PopAndDestroy( pList );
    
    if(KErrNone==err)
        {
            return err;
            
        }
        
    return KErrGeneral;   
         
}


TBool Cliwtestcases::LIW_DATA_TYPES_013_A()
   {
       
       TBool flag = 1;
          //Reusing LIW_DATA_TYPES_011 test case
          TLiwGenericParam param;
          param.Value().Set(KLIW_DATA_TYPES_011);
          
          Dump(param.Value());
          
          param.SetNameL(KTestPARAMNanmeTC1);
          CLiwGenericParamList* inps = &(iServiceHandler->InParamListL());
          CLiwGenericParamList* outps = &(iServiceHandler->OutParamListL());
          inps->AppendL(param);
          param.Reset();
          
          // Call ExecuteServiceCmdL TO GET THE RESULT
          CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(1, KTestCommandTC1, KContentTypeTxt);
          crit->SetServiceClass(TUid::Uid(KLiwClassBase));
          iServiceHandler->ExecuteServiceCmdL(*crit, *inps, *outps); 
          CleanupStack::PopAndDestroy(crit); // crit
          
          // CHECK RESULT 
          TInt pos = 0;
          outps->FindFirst(pos, EGenericParamError);
          
          if(pos != KErrNotFound)
              {
                  if ((*outps)[pos].Value().AsTInt32() == KErrNone)
                      {
                      
                          _LIT8(KIter,"Iterator");
                          TInt iterPos=0;
                          outps->FindFirst(iterPos, KIter);
                          if(pos != KErrNotFound)
                          {
                              CLiwIterable* pIter=(*outps)[iterPos].Value().AsIterable();
                                      
                                      if((pIter->operator==(*pIter)))
                                      {   
                                          flag = 0;
                                      }
                          }
                      }
                 
              }
           
           return flag;        
  }
   

TBool Cliwtestcases::LIW_DATA_TYPES_015_A()
    {
         
         TBool flag = 1;
         TInt32 intVal=10;
         TLiwVariant intVar;
         
         {//integer check
            TInt32 intRet=12;
            intVar.Set(intRet);
            
            if(intVar.AsTInt32() == 12)
                flag = 0;
           
         }
         
         {//RFile check
            RFile fileRet;
            intVar.Set(fileRet);                   
            fileRet.Close();            
         }
         
         {//Uid check
            TUid uidRet;
            intVar.Set(uidRet);
                       
         }
         
         {//TUint check
            TUint uintRet=12;
            intVar.Set(uintRet);
            
         }
         
         {//TBool check
            TBool boolRet=EFalse;
            intVar.Set(boolRet);
                       
         }
         
         {//TPtrC check
            TPtrC ptrcRet;
            intVar.Set(ptrcRet);
                
         }
         
         {//TTime check
            TTime timeRet;
            intVar.Set(timeRet);
                       
         }
         
         {//TPtrC8 check
            TPtrC8 ptrcRet;
            intVar.Set(ptrcRet);
                       
         }
         
         
         {//TInt64 check
                     TInt64 int64Ret = 13;
                     intVar.Set(int64Ret);
                               
         }
         
         {//TReal check
                     TReal realRet = 14.5;
                     intVar.Set(realRet);
        }
         
         {
             CLiwBuffer *buf = NULL;
             intVar.Set(buf);
         }
                  
         return flag;           
    }
   

TInt Cliwtestcases::LIW_DATA_GenericParamList()
{
        
      _LIT8(KText,"Test");
      
      _LIT8(KExamplekey,"key");
       RBuf8 binaryData;
      binaryData.Create(100);
      binaryData.Append(KText);
      
      TLiwGenericParam param;
      TPtrC8 ptrVar(KExamplekey);
      param.SetNameAndValueL(ptrVar, TLiwVariant( binaryData ));
      TGenericParamId SemId = 12;
      param.SetSemanticId(SemId);
      
   /*   
      CBufFlat *flatBuf = CBufFlat::NewL(10);
      //flatBuf->SetReserveL(10);
      flatBuf->ExpandL(0,101);
      //flatBuf->DoInsertL(0,&ptrVar,ptrVar.Length());
      TBuf8<5> buf23(KExamplekey);
      flatBuf->Write(0,&param,100);
      RBufReadStream buf1(*flatBuf);
      //strmBuf->WriteL(&binaryData,binaryData.Size());
     */     
      CLiwGenericParamList* pList = CLiwGenericParamList::NewLC();
      pList->AppendL(param);
      
      
      _LIT8(KText1,"Test1");
      _LIT8(KExamplekey1,"key1");
      RBuf8 binaryData1;
      binaryData1.Create(100);
      binaryData1.Append(KText1);
      
      TLiwGenericParam param1;
      TPtrC8 ptrVar1(KExamplekey1);
      param1.SetNameAndValueL(ptrVar1, TLiwVariant( binaryData1 ));
      
      pList->AppendL(param1);
      TInt flag = 1;
      TInt index = 0;
      pList->FindFirst(index,ptrVar,LIW::EVariantTypeAny);
      
      if(index == KErrNotFound)
          flag = 0;
      
      index = 0;
      pList->FindNext(index,ptrVar1,LIW::EVariantTypeAny);
      if(index == KErrNotFound)
                flag = 0;
      
      TInt cnt = 0;
      cnt = pList->Count(SemId,LIW::EVariantTypeAny);
      
      if(cnt != 2)
                flag = 0;
      
      pList->Remove(SemId);
      cnt = pList->Count(SemId,LIW::EVariantTypeAny);
      
      binaryData.Close();
      CleanupStack::PopAndDestroy( pList );
      
      if(cnt == 1 && flag == 1)
          return KErrNone;
      
      return KErrGeneral;
        
}


TInt Cliwtestcases::LIW_DATA_NewLOverLoad()
{
          
    CLiwCriteriaItem* criteria = CLiwCriteriaItem::NewLC(1, 12, KContentTypeTxt);
    CLiwCriteriaItem* criteria1 = CLiwCriteriaItem::NewLC(1, 13, KContentTypeTxt);
    CleanupStack::PopAndDestroy( criteria1 );
    CleanupStack::PopAndDestroy( criteria );
    return KErrNone;
}
// -----------------------------------------------------------------------------
// Cliwtestcases::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt Cliwtestcases::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove

