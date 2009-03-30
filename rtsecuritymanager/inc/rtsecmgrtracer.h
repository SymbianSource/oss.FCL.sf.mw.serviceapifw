/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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






#ifndef _RTSECMGRTRACE_H
#define _RTSECMGRTRACE_H
 
#include <e32base.h>


// Define tracer logging method if defined RTSECMGR_TRACE
// 
// Emulator    = Log to RDebug
// Device      = Log to file (RFileLogger)
 
// ============================================================================
 

#ifndef RTSECMGR_TRACE  // Logging off
 
    #define RTSecMgrTraceFunction(func)    
    #define RTSecMgrTraceFunctionRet(func,format)
    #define RTSecMgrCreateTraceFile()
    #define RTSecMgrTracePrintRet(a)
#else   // Logging on
 		 
       
    // Macro to print function entry, exit and leave. 
    // Example: RTSecMgrTraceFunction("CMyClass::MyFunction");
    #define RTSecMgrTraceFunction(func) TTracer function_tracer( _S(func), _S("") );
 
    // Macro to print function return value in addition to entry, exit 
    // and leave conditions Second parameter is a formatting string used 
    // to print the return value Example to print an integer return value:
    // RTSecMgrTraceFunctionRet("CMyclass::MyFunction", "%d");
    #define RTSecMgrTraceFunctionRet(func,format) TTracer func_tracer( _S(func), _S(format) );
    
	#define RTSecMgrTracePrintRet(a)   RFileLogger::WriteFormat(KLogDir, \
                            KLogFile,EFileLoggingModeAppend,a);
     
 /*	#if (defined(__WINS__) || defined(__WINSCW__))  // Print to RDebug
       
        #include <e32debug.h>
        
        #define TRACER_PRINT(a)         RDebug::Print(a,&iFunc);
        #define TRACER_PRINT_RET(a,b)   RDebug::Print(a,&iFunc,b);
  		#define RTSecMgrCreateTraceFile()
 	
 	 #else      //Print to file 	  
   */ 
  		#include <f32file.h>
  		#include <flogger.h>
        
        _LIT( KLogDir,  "rtsecmgrtracelogs" );     // Log directory: C:\logs\rtsecmgrtracelogs
        _LIT( KLogFile, "rtsecmgrtracelogs.txt" ); // Log file: c:\logs\tracer\rtsecmgrtracelogs.txt
        _LIT( KFullPath, "c:\\logs\\rtsecmgrtracelogs\\" );  
    	_LIT(KFullLogFileName , "c:\\logs\\rtsecmgrtracelogs\\rtsecmgrtracelogs.txt") ;
       
        LOCAL_C void CreateLogFile()  // Local function for creating Logdirectory and LogFile
        {
        RFs fs;
        RFile fileHandle ;
        if( fs.Connect() == KErrNone )
            {
            fs.MkDirAll( KFullPath );
            if(fileHandle.Open(fs ,KFullLogFileName , EFileWrite | EFileShareAny )  == KErrNotFound)
				{
				fileHandle.Create(fs ,KFullLogFileName , EFileWrite | EFileShareAny ) ;
				}
            fileHandle.Close() ;
            fs.Close();
            }
        }
     
    	#define RTSecMgrCreateTraceFile() CreateLogFile();
        
        #define TRACER_PRINT(a)         RFileLogger::WriteFormat(KLogDir, \
                            KLogFile,EFileLoggingModeAppend,a,&iFunc);
        #define TRACER_PRINT_RET(a,b)   RFileLogger::WriteFormat(KLogDir, \
                            KLogFile,EFileLoggingModeAppend,a,&iFunc,b);
 
 //   #endif
 
    _LIT( KLogEnter,    "%S: ENTER" );
    _LIT( KLogExit,     "%S: EXIT" );
    _LIT( KLogLeave,    "%S: LEAVE!" );
    _LIT( KLogExitRet,  "%S: EXIT, Returning " );
 
    /**
     * Simple tracer class that logs function enter, exit or leave
     */
    class TTracer
        {
    public:
 
        /**
         * inline constructor to write log of entering a function
         */
        TTracer( const TText* aFunc, const TText* aRetFormat )
            : iFunc( aFunc )
            , iRetFormat( aRetFormat )
            {
            TRACER_PRINT( KLogEnter );
            }
 
        /**
         * inline destructor to write log of exiting a function 
         * normally or with a leave
         */
        ~TTracer()
            {
            if ( std::uncaught_exception() ) // Leave is an exception
                {
                // The function exited with a leave
                TRACER_PRINT( KLogLeave );
                }
            else
                {
                // The function exited normally
                if ( iRetFormat.Length() == 0 )
                    {
                    TRACER_PRINT( KLogExit );
                    }
                else
                    {
                    // Log the return value
                    #ifdef __WINS__
                        TInt32 retVal = 0;
 
                        // The assembly bit. This needs to be reimplemented
                        // for every target. 
                        _asm( mov retVal, ebx );
 
                        TBuf<100> format( KLogExitRet );
                        format.Append( iRetFormat );
                        TRACER_PRINT_RET( format, retVal );
                    #else
                        TRACER_PRINT( KLogExit );
                   
                    #endif
                    }
                }
            }
 
    private:
 
        /** 
         * Pointer descriptor to function signature that is to be logged.
         */
        TPtrC iFunc;
        
 
        /**
         * Formatting string used to print the function return value
         */
        TPtrC iRetFormat;
 
        };
 
#endif // RTSECMGR_TRACE
 
#endif // _RTSECMGRTRACE_H