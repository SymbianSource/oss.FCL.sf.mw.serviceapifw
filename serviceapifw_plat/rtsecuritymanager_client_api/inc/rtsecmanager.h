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
* Description:       Defines Runtime security manager's core class. Defines
 * 				  APIs for runtime bindings and clients to access security
 * 				  management functionalities
 *
*/






#ifndef _CRTSECMANAGER_H
#define _CRTSECMANAGER_H

#include <e32base.h>

#include <rtsecmgrcommondef.h>
#include <rtsecmgrtrustinfo.h>

class CRTSecMgrScriptSession;
class MSecMgrPromptHandler;
class CRTSecMgrClientProxy;
class CTrustInfo;
class CPermission;

/**
 * Core class of the runtime recurity manager component.
 * 
 * Runtime security manager broadly offers 
 *
 *  - Policy management functions
 *	- Script management functions
 *  - Access permission check
 *
 * CRTSecManager offers methods to perform policy management functionalities
 * such as 
 * 	- Registering runtime security access policy
 * 	- Updating the registered security access policy
 * 	- Un-registering the registered security access policy 
 * 
 * Similarly, CRTSecManager offers methods for script management such as
 * 	- Registering a runtime script with security manager
 * 	- Un-registering the registered script
 * 	- Obtaining scipt session associated with the registered script
 * 
 * The main functionality of CRTSecManager is to perform access permission
 * check using script session functions
 * 
 * @lib rtsecmgrclient.lib
 */
NONSHARABLE_CLASS(CRTSecManager) : public CBase
	{
public:

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CRTSecManager instance
	 *
	 * Initializes client side security manager session. Starts the security
	 * manager server, if it is not already running.
	 * 
	 * @return pointer to an instance of CRTSecManager
	 */
	IMPORT_C static CRTSecManager* NewL();

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CRTSecManager instance and leaves the created instance
	 * onto the cleanupstack.
	 *
	 * Initializes client side security manager session. Starts the security
	 * manager server, if it is not already running.
	 * 
	 * @return pointer to an instance of CRTSecManager
	 */
	IMPORT_C static CRTSecManager* NewLC();

	/**
	 * Destructor
	 * 
	 * Closes client side security manager session
	 */
	IMPORT_C ~CRTSecManager();

	/**
	 * Registers a runtime security policy. Runtimes should call this function
	 * to register their security access and trust policies.
	 *
	 * @param aSecPolicy RFile Handle to security policy file
	 *
	 * @return TPolicyID generated policy identifier if successul; Otherwise one of
	 *					 system wide error codes
	 *
	 * \note
	 * Clients should call ShareProtected on the file session object as shown below.
	 * 
	 * @code	 * 
	 * 	RFs fileSession;
	 * 	fileSession.Connect();
	 * 	fileSession.ShareProtected(); //Mandatorily call before invoking SetPolicy
	 * 
	 * 	RFile secPolicyFile;
	 * 	secPolicyFile.Open(fileSession, _L("AccessPolicy.xml"), EFileShareAny );
	 * 	TPolicyID policyID = secMgr->SetPolicy(secPolicyFile);
	 *
	 *  if(policyID <= KErrNone)
	 *  {
	 *		//error..
	 *	}
	 * 
	 * @endcode
	 * 
	 */
	IMPORT_C TPolicyID SetPolicy(const RFile& aSecPolicy);
	
	/**
	 * Registers a runtime security policy. Runtimes should call this function
	 * to register their security access and trust policies.
	 *
	 * @param aPolicyBuffer const TDesC& security policy file buffer
	 *
	 * @return TPolicyID generated policy identifier if successul; Otherwise one of
	 *					 system wide error codes
	 *
	 * \note
	 * Clients should call ShareProtected on the file session object as shown below.
	 * 
	 * @code	 * 
	 * 	RFs fileSession;
	 * 	fileSession.Connect();
	 * 	fileSession.ShareProtected(); //Mandatorily call before invoking SetPolicy
	 * 
	 * 	RFile secPolicyFile;
	 * 	secPolicyFile.Open(fileSession, _L("AccessPolicy.xml"), EFileShareAny );	 
	 *  HBufC8* fileBuffer = HBufC8::NewL(KFileBufferMaxLen);
	 *  secPolicyFile.Read(*fileBuffer);
	 *   
	 * 	TPolicyID policyID = secMgr->SetPolicy(*fileBuffer);
	 *
	 *  if(policyID <= KErrNone)
	 *  {
	 *		//error..
	 *	}
	 * 
	 * @endcode
	 * 
	 */
	IMPORT_C TPolicyID SetPolicy(const TDesC8& aPolicyBuffer);

	/**
	 * UnRegisters a registered security policy. Runtimes should call this function
	 * to de-register the already registered security policy.
	 *
	 * @param aPolicyID TPolicyID Policy identifier previously generated with SetPolicy
	 *
	 * @return TInt One of sytem wide error codes in case of failure; Otherwise KErrNone
	 *    
	 */
	IMPORT_C TInt UnSetPolicy(TPolicyID aPolicyID);

	/**
	 * Updates an already registered security policy. Runtimes should call this function
	 * to update their policy.
	 *
	 * @param aPolicyID TPolicyID Policy identifier previously generated with SetPolicy
	 * @param aSecPolicy RFile Handle to security policy file
	 *
	 * @see SetPolicy for file session pre-conditions
	 *
	 * @return TPolicyID One of sytem wide error codes in case of failure; Otherwise the passed policyID
	 *
	 */
	IMPORT_C TPolicyID UpdatePolicy(TPolicyID aPolicyID,const RFile& aSecPolicy);

	/**
	 * Updates an already registered security policy. Runtimes should call this function
	 * to update their policy.
	 *
	 * @param aPolicyID TPolicyID Policy identifier previously generated with SetPolicy
	 * @param aPolicyBuffer const TDesC& security policy file buffer
	 *
	 * @see SetPolicy for file session pre-conditions
	 *
	 * @return TPolicyID One of sytem wide error codes in case of failure; Otherwise the passed policyID
	 *
	 */
	IMPORT_C TPolicyID UpdatePolicy(TPolicyID aPolicyID,const TDesC8& aPolicyBuffer);
	
	/**
	 * Registers a script/executable. Runtimes should specify the trust information
	 * of the script to be registered. 
	 *
	 * @param aPolicyID TPolicyID   Runtime's registered policy identifier
	 * @param aTrustInfo CTrustInfo a valid instance of CTrustInfo object
	 *
	 * @return TExecutableID generated executable identifier if successul; Otherwise one of
	 *					 	 system wide error codes
	 *
	 */
	IMPORT_C TExecutableID RegisterScript(TPolicyID aPolicyID, const CTrustInfo& aTrustInfo);

	/**
	 * Registers a script/executable. Runtimes should specify the trust information
	 * of the script to be registered. 
	 *
	 * @param aPolicyID TPolicyID   Runtime's registered policy identifier
	 * @param aHashMarker const TDesC& Hash value to identify script when starting script session
	 * @param aTrustInfo CTrustInfo a valid instance of CTrustInfo object
	 *
	 * @return TExecutableID generated executable identifier if successul; Otherwise one of
	 *					 	 system wide error codes
	 *
	 */
	IMPORT_C TExecutableID RegisterScript(TPolicyID aPolicyID, const TDesC& aHashMarker, const CTrustInfo& aTrustInfo);

	/**
	 * De-Registers a script/executable. Runtimes should pass the previously registered
	 * script identifier corresponding to the script to be de-registered.
	 *
	 * @param aExeID TExecutableID   A valid script identifier
	 *
	 * @return TInt One of sytem wide error codes in case of failure; Otherwise KErrNone
	 *     
	 */
	IMPORT_C TInt UnRegisterScript(TExecutableID aExeID, TPolicyID aPolicyID);

	/**
	 * Creates a script session instance. CRTSecMgrScriptSession performs access permission
	 * check for native platform service invocation. A CRTSecMgrScriptSession instance needs to
	 * be created for every instance of scripts which could potentially invoke platform service.
	 *
	 * @param aPolicyID TPolicyID    Valid registered policy identifier
	 * @param aExeID TExecutableID   Script identifier, KAnonymousScript in case of anonymous script session
	 * @param aPromptHdlr MSecMgrPromptHandler An optional prompt handler. If not provided, Security manager
	 *							     will supply a default prompt handler
	 *
	 * @return CRTSecMgrScriptSession* A pointer to the created instance of CRTSecMgrScriptSession if the executableID is valid;
	 * Otherwise NULL
	 */
	IMPORT_C CRTSecMgrScriptSession* GetScriptSessionL(TPolicyID aPolicyID, TExecutableID aExecID, MSecMgrPromptHandler* aPromptHdlr=NULL , const TDesC& aHashValue = KNullDesC);

	/**
	 * Creates a script session instance for an unregisterd trusted script. CRTSecMgrScriptSession performs access permission
	 * check for native platform service invocation. 
	 *
	 * @param aPolicyID TPolicyID    Valid registered policy identifier
	 * @param aTrustInfo CTrustInfo a valid instance of CTrustInfo object
	 * @param aPromptHdlr MSecMgrPromptHandler An optional prompt handler. If not provided, Security manager
	 *							     will supply a default prompt handler
	 *
	 * @return CRTSecMgrScriptSession* A pointer to the created instance of CRTSecMgrScriptSession;
	 * 								  NULL in case of invalid policy identifier
	 *           
	 *     
	 */
	IMPORT_C CRTSecMgrScriptSession* GetScriptSessionL(TPolicyID aPolicyID, const CTrustInfo& aTrustInfo, MSecMgrPromptHandler* aPromptHdlr=NULL);

	/**
	 * Creates a script session instance. CRTSecMgrScriptSession performs access permission
	 * check for native platform service invocation. A CRTSecMgrScriptSession instance needs to
	 * be created for every instance of scripts which could potentially invoke platform service.
	 *
	 * @param aPolicyID TPolicyID    Valid registered policy identifier
	 * @param aExeID TExecutableID   Script identifier, KAnonymousScript in case of anonymous script session
	 * @param aHashValue TDesC		 hash value passed while registering the script
	 * @param aPromptHdlr MSecMgrPromptHandler An optional prompt handler. If not provided, Security manager
	 *							     will supply a default prompt handler
	 *
	 * @return CRTSecMgrScriptSession* A pointer to the created instance of CRTSecMgrScriptSession if the executableID is valid;
	 * Otherwise NULL
	 
	IMPORT_C CRTSecMgrScriptSession* GetScriptSession(TPolicyID aPolicyID, TExecutableID aExecID, const TDesC& aHashValue, MSecMgrPromptHandler* aPromptHdlr=NULL);*/
private:
	//Private default constructor
	CRTSecManager();

	//Part of second-phase constructor
	void ConstructL();

private:
	//Proxy to client side session object
	CRTSecMgrClientProxy* iClientProxy;
	};
#endif //_CRTSECMANAGER_H

