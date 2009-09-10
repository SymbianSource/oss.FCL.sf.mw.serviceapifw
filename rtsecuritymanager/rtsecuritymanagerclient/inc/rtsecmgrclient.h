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
* Description:       Security Manager Client Side sub-Session Class
 *
*/






#ifndef _R_RSECMGRSESSION_H_
#define _R_RSECMGRSESSION_H_

#include <e32base.h>
#include <e32std.h>
#include <f32file.h>

#include <rtsecmgrcommondef.h>
#include <rtsecmgrtrustinfo.h>
#include <rtsecmgrutility.h>

class CTrustInfo;
class CScript;
class CEikonEnv;

/**
 *
 * RSecMgrSession represents the client side session handle. 
 * 
 * This class mainly communicates with the corresponding server side session object. 
 * The main reponsibility of this class is to package the input parameters,
 * invoke a server request and unpack the server response appropriately.
 *
 * @lib rtsecmgrclient.lib
 */
NONSHARABLE_CLASS(RSecMgrSession) : public RSessionBase
	{
public:

	/**
	 * Default constructor
	 * 
	 */
	RSecMgrSession();

	/**
	 * Connects to the runtime security manager server resulting in the creation
	 * of peer server side session object
	 * 
	 * @return TInt KErrNone if successful; In case of failure, one of system-wide
	 * error codes
	 * 
	 */
	TInt Connect();

	/**
	 * Gets the version of runtime security manager component
	 * 
	 * @return TVersion version of runtime security manager
	 * 
	 */
	TVersion Version() const;

	/**
	 * Closes the session with the runtime security manager server
	 * 
	 */
	void Close();

	/**
	 * Registers the runtime security policy with security manager. This method
	 * packs the message parameters required for registering the policy. 
	 * 
	 * The various parameters required for SetPolicy operation are :
	 * 	
	 *  Operation Code  : ESetPolicy
	 * 	IPC Argument[0] : Policy Identifier (as inOut parameter)
	 *  IPC Argument[1] : SecurityPolicy FileHandle
	 *  IPC Argument[2] : SecurityPolicy FileSession object
	 *  IPC Argument[3] : none
	 * 
	 * @param aSecPolicy RFile Handle to security policy file
	 * 
	 * @return TPolicyID generated policy identifier if successul; Otherwise one of
	 *					 system wide error codes
	 * 
	 */
	TPolicyID SetPolicy(const RFile& aSecPolicy);
	
	/**
	 * Registers the runtime security policy with security manager. This method
	 * packs the message parameters required for registering the policy. 
	 * 
	 * The various parameters required for SetPolicy operation are :
	 * 	
	 *  Operation Code  : ESetPolicy
	 * 	IPC Argument[0] : Policy Identifier (as inOut parameter)
	 *  IPC Argument[1] : PolicyBuffer
	 *  IPC Argument[2] : SecurityPolicy FileSession object
	 *  IPC Argument[3] : none
	 * 
	 * @param aSecPolicy RFile Handle to security policy file
	 * 
	 * @return TPolicyID generated policy identifier if successul; Otherwise one of
	 *					 system wide error codes
	 * 
	 */
	TPolicyID SetPolicy(const TDesC8& aPolicyBuffer);

	/**
	 * UnRegisters a registered security policy. Runtimes should call this function
	 * to de-register the already registered security policy.
	 * 
	 * The various parameters required for UnSetPolicy operation are :
	 * 	
	 *  Operation Code  : EUnsetPolicy
	 * 	IPC Argument[0] : Policy Identifier to un-register
	 *  IPC Argument[1] : Successcode (as inOut parameter)
	 *  IPC Argument[2] : none
	 *  IPC Argument[3] : none
	 * 
	 * @param aPolicyID TPolicyID Policy identifier previously generated with SetPolicy
	 *
	 * @return TInt One of sytem wide error codes in case of failure; Otherwise KErrNone
	 * 
	 */
	TInt UnSetPolicy(TPolicyID aPolicyID);

	/**
	 * Updates an already registered security policy. Runtimes should call this function
	 * to update their policy.
	 *
	 * The various parameters required for UpdatePolicy operation are :
	 * 	
	 *  Operation Code  : EUpdatePolicy
	 * 	IPC Argument[0] : Policy Identifier
	 *  IPC Argument[1] : SecurityPolicy FileHandle
	 *  IPC Argument[2] : SecurityPolicy FileSession object
	 *  IPC Argument[3] : none
	 * 
	 * @param aPolicyID TPolicyID Policy identifier previously generated with SetPolicy
	 * @param aSecPolicy RFile Handle to security policy file
	 *
	 * @see SetPolicy for file session pre-conditions
	 *
	 * @return TPolicyID One of sytem wide error codes in case of failure; Otherwise the passed policyID
	 *
	 */
	TPolicyID UpdatePolicy(TPolicyID aPolicyID, const RFile& aSecPolicy);
	
	/**
	 * Updates an already registered security policy. Runtimes should call this function
	 * to update their policy.
	 *
	 * The various parameters required for UpdatePolicy operation are :
	 * 	
	 *  Operation Code  : EUpdatePolicy
	 * 	IPC Argument[0] : Policy Identifier
	 *  IPC Argument[1] : SecurityPolicy FileBuffer
	 *  IPC Argument[2] : SecurityPolicy FileSession object
	 *  IPC Argument[3] : none
	 * 
	 * @param aPolicyID TPolicyID Policy identifier previously generated with SetPolicy
	 * @param aSecPolicy RFile Handle to security policy file
	 *
	 * @see SetPolicy for file session pre-conditions
	 *
	 * @return TPolicyID One of sytem wide error codes in case of failure; Otherwise the passed policyID
	 *
	 */
	TPolicyID UpdatePolicy(TPolicyID aPolicyID, const TDesC8& aPolicyBuffer);

	/**
	 * Registers a script/executable. Runtimes should specify the trust information
	 * of the script to be registered. 
	 *
	 *  Operation Code  : ERegisterScript
	 * 	IPC Argument[0] : Policy Identifier
	 *  IPC Argument[1] : Script Identifier (as inOut Parameter)
	 *  IPC Argument[2] : none
	 *  IPC Argument[3] : none 
	 * 
	 * @param aPolicyID TPolicyID   Runtime's registered policy identifier
	 * @param aTrustInfo CTrustInfo a valid instance of CTrustInfo object
	 *
	 * @return TExecutableID generated executable identifier if successul; Otherwise one of
	 *					 	 system wide error codes
	 *
	 */
	TExecutableID RegisterScript(TPolicyID aPolicyID,
			const CTrustInfo& aTrustInfo);

	/**
	 * Registers a script/executable. Runtimes should specify the trust information
	 * of the script to be registered. 
	 *
	 *  Operation Code  : ERegisterScript
	 * 	IPC Argument[0] : Policy Identifier
	 *  IPC Argument[1] : Script Identifier (as inOut Parameter)
	 *  IPC Argument[2] : hash value of script
	 *  IPC Argument[3] : none 
	 * 
	 * 
	 * @param aPolicyID TPolicyID   Runtime's registered policy identifier
	 * @param aHashMarker const TDesC& Hash value to identify script when starting script session
	 * @param aTrustInfo CTrustInfo a valid instance of CTrustInfo object
	 *
	 * @return TExecutableID generated executable identifier if successul; Otherwise one of
	 *					 	 system wide error codes
	 *
	 */
	TExecutableID RegisterScript(TPolicyID aPolicyID, const TDesC& aHashMarker,
			const CTrustInfo& aTrustInfo);

	/**
	 * De-Registers a script/executable. Runtimes should pass the previously registered
	 * script identifier corresponding to the script to be de-registered.
	 *
	 *  Operation Code  : EUnRegisterScript
	 * 	IPC Argument[0] : Script Identifier
	 *  IPC Argument[1] : Policy Identifier
	 *  IPC Argument[2] : Success code (as inOut parameter)
	 *  IPC Argument[3] : none 
	 * 
	 * 
	 * @param aExeID TExecutableID   A valid script identifier
	 *
	 * @return TInt One of sytem wide error codes in case of failure; Otherwise KErrNone
	 *     
	 */
	TInt UnRegisterScript(TExecutableID aExeID, TPolicyID aPolicyID);


private:

	/**
	 * Starts security manager server if it is not already started
	 *
	 * @return TInt One of sytem wide error codes in case of failure; Otherwise KErrNone
	 *     
	 */
	TInt StartSecManagerServer() const;

	/**
	 * Returns security manager server location
	 *
	 * @return TFullName Server location path
	 *     
	 */
	TFullName ServerLocation() const;
	};

/**
 *
 * Represents the client side sub-session handle. This class mainly
 * communicates with the corresponding server side sub-session handle. 
 * 
 * The main reponsibilities of this class are :
 * 
 * 	- to store the pre-computed capability set of the script associated with the session object
 *	- to invoke the prompt handler
 *	- to notify the server sub-session object in case of permanent grant change
 *
 * @lib rtsecmgrclient.lib
 */
NONSHARABLE_CLASS(RSecMgrSubSession) : public RSubSessionBase
	{
public:
	
	RSecMgrSubSession();

	/**
	 * Opens client-side sub-session. The script session is modelled as a
	 * client side sub-session with a peer server side sub-session.
	 * 
	 * @param aSession RSessionBase handle to client side session
	 * @param aScriptInfo CScript	the underlying script object
	 * @param aPolicyID TPolicyID	policy identifier of the runtime security policy associated with the
	 * 								script
	 * 
	 * @return KErrNone if creation of sub-session is successful; In case of failure,
	 * 		one of system-wide error codes
	 */
	TInt Open(const RSessionBase& aSession, CScript& aScriptInfo,
			TPolicyID aPolicyID, const TDesC& aHashValue = KNullDesC());	
	
	/**
	 * Opens client-side sub-session for an un-registered trusted script. The script session is modelled as a
	 * client side sub-session with a peer server side sub-session.
	 * 
	 * @param aSession RSessionBase handle to client side session
	 * @param aScriptInfo CScript	the underlying script object
	 * @param aPolicyID TPolicyID	policy identifier of the runtime security policy associated with the
	 * 								script
	 * @param aTrustInfo CTrustInfo trust data structure
	 * 
	 * @return KErrNone if creation of sub-session is successful; In case of failure,
	 * 		one of system-wide error codes
	 */
	TInt Open(const RSessionBase& aServer, CScript& aScriptInfo,
			TPolicyID aPolicyID,const CTrustInfo& aTrustInfo);

	/**
	 * Closes the script session, inturn closing the sub-session on
	 * either side of client and server
	 * 
	 */
	void Close();

	/**
	 * Updates the blanket permission data of the script
	 * 
	 * @param aScriptID TExecutableID Identifier of the scipt
	 * @param aPermGrant TPermGrant   Blanket allowed data
	 * @param aPermDenied TPermGrant  Blanket denied data
	 * 
	 * @return KErrNone if update of blanket permission successul; Otherwise one of
	 *					 system wide error codes
	 * 
	 */
	TInt UpdatePermGrant(TExecutableID aScriptID, TPermGrant aPermGrant,
			TPermGrant aPermDenied) const;
	
	/**
     * Updates the blanket permission data of the script
     * 
     * @param aScriptID TExecutableID Identifier of the scipt
     * @param aAllowedProviders RProviderArray   Blanket allowed providers
     * @param aDeniedProviders RProviderArray  Blanket denied providers
     * 
     * @return KErrNone if update of blanket permission successul; Otherwise one of
     *                   system wide error codes
     * 
     */
	TInt UpdatePermGrant(TExecutableID aScriptID, RProviderArray aAllowedProviders, RProviderArray aDeniedProviders) const;

	
private:
	
	//CEikonEnv* iEnv;	
	
	RFs iFs;
	};

#endif //_R_RSECMGRSESSION_H_

