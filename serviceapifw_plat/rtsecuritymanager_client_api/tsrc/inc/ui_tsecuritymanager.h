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









#ifndef UI_TSECURITYMANAGER_H
#define UI_TSECURITYMANAGER_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>

#include <e32base.h>
#include <e32std.h>
#include <f32file.h>
#include <rtsecmgrscriptsession.h>
#include <rtsecmgrcommondef.h>

// Forward reference
 class CRTSecManager;
class CTrustInfo;
class RSecMgrSession;
class RSecMgrSubSession;
// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( Kui_tsecuritymanagerLogPath, "\\logs\\testframework\\ui_tsecuritymanager\\" ); 
// Log file
_LIT( Kui_tsecuritymanagerLogFile, "ui_tsecuritymanager.txt" ); 
_LIT( Kui_tsecuritymanagerLogFileWithTitle, "ui_tsecuritymanager_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Cui_tsecuritymanager;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Cui_tsecuritymanager test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cui_tsecuritymanager) : public CScriptBase,public RSessionBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cui_tsecuritymanager* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cui_tsecuritymanager();

    public: // New functions
	TVersion Version() const;
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
        Cui_tsecuritymanager( CTestModuleIf& aTestModuleIf );

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
        virtual TInt serverstart1( CStifItemParser& aItem );
         virtual TInt serverstart2( CStifItemParser& aItem );
        virtual TInt secclient1( CStifItemParser& aItem );
         virtual TInt secclient2( CStifItemParser& aItem );
   virtual TInt secclient3(CStifItemParser& aItem ); 
    virtual TInt secclient4(CStifItemParser& aItem );          
     virtual TInt secclient5(CStifItemParser& aItem );
       virtual TInt secclient6(CStifItemParser& aItem );
       virtual TInt secclient7(CStifItemParser& aItem );
        virtual TInt secclient8(CStifItemParser& aItem );
         virtual TInt secclient9(CStifItemParser& aItem );
         virtual TInt secclient10(CStifItemParser& aItem );
          virtual TInt secclient11(CStifItemParser& aItem );
          virtual TInt secclient12(CStifItemParser& aItem );
          virtual TInt secclient13(CStifItemParser& aItem );
          
          
       // void SendTestClassVersion();
        
        
        TInt StartSecManagerServer() const;
        
        TFullName ServerLocation() const;
        TInt ExecuteThreadOne(TAny *aPtr);

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
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
   CTrustInfo* iTrust;
    //CLogHandler*      iLogger;
    CRTSecManager* iSession;
    CRTSecManager*   iSession2; 
    CRTSecMgrScriptSession* scriptSession;
    TInt iPolicyId;
    
     RThread iThread1;
      
    };
    
    
    const TInt KSecurityServerUid2Int(0x1020507E);
const TUid KSecMgrServerUid2 =
	{
			KSecurityServerUid2Int
	};

#endif      // UI_TSECURITYMANAGER_H

// End of File
