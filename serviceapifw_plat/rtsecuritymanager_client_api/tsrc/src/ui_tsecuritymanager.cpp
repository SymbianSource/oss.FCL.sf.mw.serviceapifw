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









// INCLUDE FILES
#include <StifTestInterface.h>
#include "ui_tsecuritymanager.h"
#include <SettingServerClient.h>

 

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::Cui_tsecuritymanager
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
Cui_tsecuritymanager::Cui_tsecuritymanager( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf )
    {
    }

// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void Cui_tsecuritymanager::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
        {
        User::Leave(ret);
        } 
    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;
    
    if(loggerSettings.iAddTestCaseTitle)
        {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(Kui_tsecuritymanagerLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(Kui_tsecuritymanagerLogFile);
        }

    iLog = CStifLogger::NewL( Kui_tsecuritymanagerLogPath, 
                          logFileName,
                          CStifLogger::ETxt,
                          CStifLogger::EFile,
                          EFalse );
    
   // SendTestClassVersion();
    }

// -----------------------------------------------------------------------------
// Cui_tsecuritymanager::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
Cui_tsecuritymanager* Cui_tsecuritymanager::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    Cui_tsecuritymanager* self = new (ELeave) Cui_tsecuritymanager( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
Cui_tsecuritymanager::~Cui_tsecuritymanager()
    { 

    // Delete resources allocated from test methods
    Delete();

    // Delete logger
    delete iLog; 

    }

//-----------------------------------------------------------------------------
// Cui_tsecuritymanager::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
/*void Cui_tsecuritymanager::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("ui_tsecuritymanager.dll");

	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName);
	}*/

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) Cui_tsecuritymanager::NewL( aTestModuleIf );

    }


//  End of File
