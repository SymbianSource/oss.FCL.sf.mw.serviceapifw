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
* Description:       Definition of security manager persistent store class
 *
*/






#ifndef C_RTSECMGRSTORE_H
#define C_RTSECMGRSTORE_H

#include <e32std.h>
#include <badesca.h>
#include <d32dbms.h>
#include <f32file.h>
#include <s32file.h>
#include "rtsecmgrcommondef.h"
#include "rtsecmgrserverdef.h"
#include "rtsecmgrdata.h"

/*
 * Abstracts the underlying security manager persistent storage
 * type.
 * 
 * Provides methods to store and retrieve security policy data
 * and script related data.
 *
 * @see CProtectionDomain
 * @see CScript
 * 
 * @exe rtsecmgrserver.exe
 */
NONSHARABLE_CLASS(CSecMgrStore) : public CBase
	{
public:

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CSecMgrStore instance
	 * 
	 * @return CSecMgrStore* pointer to an instance of CSecMgrStore
	 */
	static CSecMgrStore* NewL();

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CSecMgrStore instance and leaves the created
	 * instance on the cleanupstack
	 * 
	 * @return CSecMgrStore* pointer to an instance of CSecMgrStore
	 */
	static CSecMgrStore* NewLC();

	/**
	 * Destructor
	 * 
	 * Closes file session connection
	 * 
	 */
	inline ~CSecMgrStore();

	/**
	 * Updates policy data to the stream.
	 * 
	 * This method creates a new policy file with the name equivalent to
	 * policy identifier under security manager servers private directory.
	 * 
	 * The policy data are serialized to the newly created policy file.
	 * 
	 * If policy file already exists, the content will be overwritten.
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * @param aPolicies const RProtectionDomains& list of domains to serialize
	 * 
	 */
	void StorePolicyL(const CPolicy& aPolicy);

	/**
	 * Removes policy file from the persistent storage.
	 * 
	 * This method looks for an existing policy file with the name equivalent to
	 * policy identifier under security manager servers private directory. If policy
	 * file is found, the file will be deleted from file system.
	 * 
	 * @param aPolicyID TPolicyID policy identifier of the policy to be removed
	 *
	 * @return KErrNone if policy file removal corresponding to policy identifier passed is successful;
	 *                  Otherwise, one of system wide error codes
	 * 
	 */
	TInt RemovePolicy(TPolicyID aPolicyID);

	/**
	 * Restores policy data from the stream.
	 * 
	 * This method internalizes the policy data from the persistent
	 * storage.
	 * 
	 * @param aPolicies RPolicies& Input policy data to be internalized
	 * 
	 */
	void RestorePoliciesL(RPolicies& aPolicies);

	/**
	 * Registers a script with security manager persistent store.
	 * 
	 * This method creates a new script file externalizing the script
	 * data to the persistent stream.
	 * 
	 * @param aExecID TExecutableID script identifier
	 * @param aPolicyID TPolicyID policy identifier of the script
	 * @param aScript const CScript& script data to be serialized
	 * 
	 */
	void RegisterScriptL(TExecutableID aExecID, const CScript& aScript);

	/**
	 * UnRegisters a script with security manager persistent store. 
	 * 
	 * This method removes an existing script file from the persistent stream.
	 * 
	 * @param aExecID TExecutableID script identifier to be removed
	 * 
	 */
	void UnRegisterScriptL(TExecutableID aExecID);

	/**
	 * Internalizes the script data structure with script
	 * information read from the persistent store
	 * 
	 * @param aExecID TExecutableID script identifier
	 * @param aScriptInfo CScript& script data to be internalized
	 * 
	 */
	TInt ReadScriptInfo(TExecutableID aExecID, CScript& aScriptInfo);

	/**
	 * Utility method to get the script file name given the script identifier.
	 * 
	 * @param aExecID TExecutableID script identifier
	 * @param aScriptFile TDes& output file name to be returned
	 * 
	 * @return KErrNone if script file name is found; Otherwise one of
	 * system wide error codes
	 * 
	 */
	TInt GetScriptFile(TExecutableID aExecID,TDes& aScriptFile);

	/**
	 * Externalizes the script data structure with script
	 * information read to the persistent store
	 * 
	 * @param aExecID TExecutableID script identifier
	 * @param aScriptInfo CScript& script data to be externalized
	 * 
	 */
	void WriteScriptInfoL(TExecutableID aExecID, CScript& aScriptInfo);

	/**
	 * Reads the script identifier from security manager configuration
	 * repository
	 * 
	 * @param aExecID TExecutableID& output script identifier
	 * 
	 * @return KErrNone if reading of identifier is successful; Otherwise
	 * one of system wide error codes
	 */
	TInt ReadCounter(TExecutableID& aExecID);

	/**
	 * Writes the script identifier to security manager configuration
	 * repository
	 * 
	 * @param aExecID TExecutableID script identifier to be externalized
	 * 
	 * @return KErrNone if writing of identifier is successful; Otherwise
	 * one of system wide error codes
	 */
	TInt WriteCounter(TExecutableID aExecID);

	/*
	 * Utility method to create a backup for the policy file
	 * 
	 * @param aPolicyID TPolicyID policy identifier for the file
	 * getting backed up
	 * @return KErrNone if backupfile is successsfully created
	 *
	 */
		
	TInt BackupFile(TPolicyID aPolicyID);
	
	/*
	 * Utility method to remove the backup file created
	 * 
	 * @param aPolicyID TPolicyID policy identifier for the file
	 * whose backup is getting removed
	 * @return KErrNone if backup file is successsfully removed
	 *
	 */
	TInt RemoveTempPolicy(TPolicyID aPolicyID);
	
	/*
	 * Utility method to restore the backup file created if update fails
	 * 
	 * @param aPolicyID TPolicyID policy identifier for the file
	 * which is restored back
	 * @return KErrNone if backup file is successsfully restored
	 *
	 */
	TInt RestoreTempPolicy(TPolicyID aPolicyID);
	
private:

	/*
	 * Default private constructor
	 */
	inline CSecMgrStore();

	/*
	 * Two-phased constructor
	 * 
	 */
	inline void ConstructL();

	/*
	 * Utility method to get security manager's private path
	 * 
	 */
	TInt GetPrivatePath(TDes& aPrivatePath);

	/*
	 * Utility method to get security manager's configuration file
	 * 
	 */
	TInt GetConfigFile(TDes& aPrivatePath);

	/*
	 * Utility method to get security manager's policy file
	 * name
	 * 
	 */
	TInt GetPolicyPath(TDes& aFile);
	
	/*
	 * Utility method to get security manager's script file
	 * storage path
	 * 
	 */
	TInt GetScriptPath(TDes& aFile);

	/*
	 * Utility method to get security manager's script file
	 * name given a script identifier
	 * 
	 */
	TInt GetScriptFile(TDes& aFile,TExecutableID aExecID);



	/*
	 * Utility method to get security manager's policy file
	 * name given the policy identifier
	 * 
	 */
	TInt GetPolicyFile(TDes& aFile,TPolicyID aPolicyID);
	

private:

	//underlying file session 
	RFs iFsSession;
	};

#include "rtsecmgrstore.inl"
	
#endif
