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








#ifndef LIWTESTCASES_H
#define LIWTESTCASES_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <liwservicehandler.h>
#include <rtsecmgrscriptsession.h>

#include <eikapp.h>
#include <eikdoc.h>
#include <e32std.h>
#include <coeccntx.h>
#include <aknViewAppUi.h>
#include <akntabgrp.h>
#include <aknnavide.h>
#include <AknTabObserver.h>

#include  "liw_async_testcase.h"


// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KliwtestcasesLogPath, "\\logs\\testframework\\liwtestcases\\" ); 
// Log file
_LIT( KliwtestcasesLogFile, "liwtestcases.txt" ); 
_LIT( KliwtestcasesLogFileWithTitle, "liwtestcases_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Cliwtestcases;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Cliwtestcases test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cliwtestcases) : public CScriptBase, public MLiwNotifyCallback, public MSecMgrPromptHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cliwtestcases* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cliwtestcases();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );
        
    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );
        
    public: // Functions from base classes

        TInt Prompt(const RCapabilityArray& /*aCapsToCheck*/,CScript& /*aScript*/)
        {
            return EAccessNok;
        }
    
        TInt Prompt(const RCapabilityArray& /*aCapsToCheck*/,RCapabilityArray& /*aCapsNotMatching*/, CScript& /*aScript*/)
        {
            return EAccessNok;
        }
        
        TInt Prompt(RPromptDataList& aPromptDataList , TExecutableID /*aExecID = KAnonymousScript*/)
        {
           // CEikonEnv::Static()->InfoMsg(_L("Custom Prompt"));
            
            for(TInt i(0);i!=aPromptDataList.Count();++i)
            {
                aPromptDataList[i]->SetUserSelection(RTUserPrompt_OneShot);
                //aPromptDataList[i]->iUserSelection = RTUserPrompt_OneShot;
            }
            
            return EAccessOk;
        }
       
       void SetPromptOption(TSecMgrPromptUIOption)
        {
            
        }
       
       TSecMgrPromptUIOption PromptOption() const
        {
            return RTPROMPTUI_DEFAULT;
        }
        
       TInt HandleNotifyL(
           TInt aCmdId,
           TInt aEventId,
           CLiwGenericParamList& aEventParamList,
           const CLiwGenericParamList& aInParamList);

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        Cliwtestcases( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt ExampleL( CStifItemParser& aItem );
		virtual TBool LIW_DATA_TYPES_001();
		virtual TBool LIW_DATA_TYPES_002();
		virtual TBool LIW_DATA_TYPES_003();
		virtual TBool LIW_DATA_TYPES_004();
		virtual TBool LIW_DATA_TYPES_005();
		virtual TBool LIW_DATA_TYPES_006();
        virtual TBool LIW_DATA_TYPES_007();
        virtual TBool LIW_DATA_TYPES_008();
        virtual TBool LIW_DATA_TYPES_009();
        virtual TBool LIW_DATA_TYPES_010();
		
        virtual TBool LIW_DATA_TYPES_011();
       virtual TBool LIW_DATA_TYPES_012();
       
       //List comparision
       virtual TBool LIW_DATA_TYPES_013();
       
       //Map comparison
       virtual TBool LIW_DATA_TYPES_014();
       
       //Negative case for TLiwVariant::Get
       virtual TBool LIW_DATA_TYPES_015();
       
       //Negative case for TLiwVariant::AsXYZ interfaces
       virtual TBool LIW_DATA_TYPES_016();
       
       //ServiceCmdStr on integeral service commmand ID
       virtual TBool LIW_DATA_TYPES_017();
       virtual TBool LIW_DATA_TYPES_018();
       
       //HasCriteria check
       virtual TBool LIW_DATA_TYPES_019();
       
       //TLiwVariant::operator == check
       virtual TBool LIW_DATA_TYPES_020();
       
       //CLiwGenericParamList::FindFirst and FindNext
       virtual TBool LIW_DATA_TYPES_021();
       
       //CLiwGenericParamList::FindFirst and FindNext and AppendL(CLiwGenericParamList&)
       virtual TBool LIW_DATA_TYPES_022();
       
       //CLiwContainer::ExternalizeL
       virtual TBool LIW_DATA_TYPES_023();
       
       //TLiwVariant::Dump // only for debug version
       virtual TBool LIW_DATA_TYPES_024();
       
       //Should be called as the last case
       virtual TBool LIW_CLEANUP_LIW_001();
       
       //for conversion utilities
       virtual TBool LIW_DATA_TYPES_025();
       virtual TBool LIW_DATA_TYPES_026();
       //virtual TBool LIW_DATA_TYPES_027();
       //virtual TBool LIW_DATA_TYPES_028();
       virtual TBool LIW_DATA_TYPES_029();
       virtual TBool LIW_DATA_TYPES_030();
       //virtual TBool LIW_DATA_TYPES_031();
       //virtual TBool LIW_DATA_TYPES_032();
       virtual TBool LIW_DATA_TYPES_033();
       virtual TBool LIW_DATA_TYPES_034();
       //virtual TBool LIW_DATA_TYPES_035();
       //virtual TBool LIW_DATA_TYPES_036();
       virtual TBool LIW_DATA_TYPES_037();
       virtual TBool LIW_DATA_TYPES_038();
       virtual TBool LIW_DATA_TYPES_039();
       //virtual TBool LIW_DATA_TYPES_040();
       virtual TBool LIW_DATA_TYPES_041();
       virtual TBool LIW_DATA_TYPES_042();
       virtual TBool LIW_DATA_TYPES_043();
       //virtual TBool LIW_DATA_TYPES_044();
       
       //QueryImplementation TestCases
       virtual TBool LIW_DATA_TYPES_045();
       virtual TBool LIW_DATA_TYPES_046();
       virtual TBool LIW_DATA_TYPES_047();
       virtual TBool LIW_DATA_TYPES_048();
       virtual TBool LIW_DATA_TYPES_049();
       
       //Versioning TestCases
       virtual TBool LIW_MDAT_VER1();
       virtual TBool LIW_MDAT_VER2();
       virtual TBool LIW_MDAT_VER3();
       virtual TBool LIW_MDAT_VER4();
       virtual TBool LIW_MDAT_VER5();
       
       //Generic Metadata TestCases
       virtual TBool LIW_MDAT_CAP1();
       virtual TBool LIW_MDAT_CAP2();
       
       //Test case for Data Type - Map AtL returning const TDesC8&
       virtual TBool LIW_Map_AtL_New();
       
       //Test case for Data Type - Cleanup check
       virtual TBool LIW_ContainerCleanup();
       virtual TBool LIW_ParamCleanup();
       virtual TBool LIW_VariantCleanup();
       
        //Checking TInt64 datatype & conversion utilities
        virtual TBool LIW_DATA_TYPES_050();
       
        virtual void TestL(CLiwMap *map);
        virtual void TestParamL(CLiwGenericParamList *param);
        virtual void TestVariantL(CLiwMap* map);
	        
        virtual TInt LIW_ASYNC_001();
        virtual TInt LIW_ASYNC_002();
        virtual TInt LIW_ASYNC_003();
        virtual TInt LIW_ASYNC_004();
                
        virtual TInt LIW_ASYNC_LongServiceCmd();
        virtual TInt LIW_ASYNC_LongInterface();
        virtual TInt LIW_ASYNC_LongInvalidServiceCmd();
        virtual TInt LIW_ASYNC_LongInvalidInterface();
        virtual TInt LIW_ASYNC_LongInterface1();
        virtual TInt LIW_ParamCleanup1();
        virtual void TestParam1L();
        virtual TInt LIW_GetInterest();
        virtual TBool LIW_Defaut_list_externalize();
        virtual TBool LIW_DATA_TYPES_013_A();
        virtual TBool LIW_DATA_TYPES_015_A();
        virtual TInt LIW_DATA_GenericParamList();
        virtual TInt LIW_DATA_NewLOverLoad();
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();
        TInt LoadL();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
        
    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        CLiwServiceHandler *iServiceHandler;
        CLiw_AsyncTC *asyObj;
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;
        
    };

#endif      // LIWTESTCASES_H

// End of File
