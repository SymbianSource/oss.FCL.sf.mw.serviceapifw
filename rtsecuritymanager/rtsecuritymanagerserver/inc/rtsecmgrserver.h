/*
* Copyright (c) 2003-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:       Core Runtime Security Manager Server Class
 *
*/







#ifndef C_CRTSECMGRSERVER_H
#define C_CRTSECMGRSERVER_H

#include <rtsecmgrcommondef.h>
#include "rtsecmgrserverdef.h"
#include "rtsecmgrpolicy.h"
#include "rtsecmgrstore.h"
#include "rtsecmgrpolicymanager.h"
#include "rtsecmgrscriptmanager.h"

class CPolicyManager;
class CScriptManager;
class CSecMgrStore;

//needed for creating server thread.
const TUint KDefaultHeapSize=0x10000;
const TUint KShutDownDelay = 200000; // Aprox 2 sec

/*
 * Shutdown timer class
 *
 * @exe rtsecmgrserver.exe 
 */
class CShutDown : public CTimer
	{
public:
	inline CShutDown();
	inline void ConstructL();
	inline void Start();
	inline virtual ~CShutDown();
	
private:
	inline void RunL();
	};

#include "rtsecmgrshutdown.inl"
	
#include "rtsecmgrserverpolicy.inl"	
	
/*
 CRTSecMgrServer class

 Represents the server.
 
 The server starts with the first client connect call.
 Start includes setting up active scheduler, the server active object,
 and the object container index which produces object object containers for each session.
 */
class CRTSecMgrServer : public CPolicyServer //CServer2 //CPolicyServer
	{
public:
	/* 
	 * Creates a new session with the server; 
	 * 
	 * @param aVersion const TVersion& version information of security manager server
	 * @param aMessage const RMessage2& server's current message
	 * 
	 * @return CSession2* pointer to base instance of created server session
	 */
	CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;

	/* 
	 * Two-phased constructor. Creates a new server object.
	 * 
	 * @param aActiveObjectPriority TPriority priorty of the server process
	 * 
	 * @return CRTSecMgrServer* pointer to new instance of CRTSecMgrServer
	 */
	static CRTSecMgrServer* NewL(CActive::TPriority aActiveObjectPriority);
	
	/* 
	 * Two-phased constructor. Creates a new server object and leaves
	 * the created instance on the cleanupstack.
	 * 
	 * @param aActiveObjectPriority TPriority priorty of the server process
	 * 
	 * @return CRTSecMgrServer* pointer to new instance of CRTSecMgrServer
	 */
	static CRTSecMgrServer* NewLC(CActive::TPriority aActiveObjectPriority);

	/* 
	 * Utility method to panic the server
	 * 
	 * @param aPanic TSecMgrServPanic Panic error code
	 * 
	 */
	inline static void PanicServer(TSecMgrServPanic aPanic);

	/* 
	 * Returns an object container, and guaranteed
	 * to produce object containers with unique ids
	 * within the server
	 * 
	 * This method is called by a new session to create 
	 * a container 
	 * 
	 * 
	 * @return CObjectCon* a new object container
	 */
	CObjectCon* NewContainerL();

	/* 
	 * Adds a new server side session
	 * 
	 */
	inline void AddSession();
	
	/* 
	 * Removes a server side session by decrementing
	 * the session count.
	 * 
	 * If the session count reaches zero, this method
	 * initiates server shut down timer
	 * 
	 */
	inline void RemoveSession();
	
	/*
	 * Removes a container object from the container
	 * index
	 * 
	 * @param aContainer CObjectCon* pointer to containter object
	 * to be removed
	 */
	inline void RemoveFromContainerIx(CObjectCon* aContainer);

	/*
	 * Generates a new script identifier
	 * 
	 * @param aExecID TExecutableID& output generated script identifier
	 *
	 * @return KErrNone if generation of script identifier successful
	 */
	inline TInt ReadCounter(TExecutableID& aExecID);

	/*
	 * Registers a new policy. The actual task is performed by
	 * server side session.
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * @param aDomains const RProtectionDomains& domains to be added
	 *
	 * @return KErrNone if generation of script identifier successful
	 */	
	inline void AddPolicyL(TPolicyID aPolicyID, const RProtectionDomains& aDomains, RAliasGroup& aAliasGroup);
	
	/*
	 * Checks whether policy with the passed policy identifier exists.
	 * The actual task is performed by server side session.
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * 
	 * @return EFalse if policy identifier does not exist; Otherwise ETrue
	 */
	inline TBool IsValidPolicy(TPolicyID aPolicyID) const;
	
	/*
	 * Removes an existing policy with the passed policy identifier.
	 * The actual task is performed by server side session.
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * 
	 * @return KErrNone if removal of policy is successful; Otherwise one
	 * of system wide error codes
	 */
	inline TInt RemovePolicy(TPolicyID aPolicyID);

	/*
	 * Register a runtime script with security manager.
	 * The actual task is performed by server side session.
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * @param aHashValue const TDesC& hashvalue of script
	 * 
	 * @return TExecutableID valid script identifier is registration
	 * is successful; otherwise one of system wide error codes
	 * 
	 */
	inline TExecutableID
			RegisterScript(TPolicyID aPolicyID, const TDesC& aHashValue);
	
	/*
	 * Register a runtime script with security manager.
	 * The actual task is performed by server side session.
	 * 
	 * @param aPolicyID TPolicyID policy identifier
	 * 
	 * @return TExecutableID valid script identifier is registration
	 * is successful; otherwise one of system wide error codes
	 * 
	 */
	inline TExecutableID
			RegisterScript(TPolicyID aPolicyID);
	
	/*
	 * UnRegisters a runtime script from security manager.
	 * The actual task is performed by server side session.
	 * 
	 * @param aExecID TExecutableID script identifier
	 * @param aPolicyID TPolicyID policy identifier
	 * 
	 * 
	 */
	inline void UnRegisterScriptL(TExecutableID aExecID, TPolicyID aPolicyID);
	
	/*
	 * Gets script data structure. The script identifier
	 * is specified in the input script object
	 * 
	 * @param aScript CScript& Input output script object
	 * 
	 * @return KErrNone if script object is successsfully internalized
	 * 
	 */
	inline TInt GetCapabilityInfo(CScript& aScript);
	
	/*
	 * Gets script data structure. The script identifier
	 * is specified in the input script object
	 * 
	 * @param aPolicyID TPolicyID  policy identifier
	 * @param aExecID TExecutableID script identifier
	 * @param aScript CScript& Input output script object
	 * 
	 * @return KErrNone if script object is successsfully internalized
	 * 
	 */
	TInt GetCapabilityInfo(TPolicyID aPolicyID, TExecutableID aExecID,
			CScript& aScript);
	
	/*
	 * Utility method to get the name of the script file
	 * 
	 * @param aPolicyID TPolicyID  policy identifier
	 * @param aScriptFile TFileName& input output name of script file
	 * 
	 * @return KErrNone if script filename is successsfully fetched
	 * 
	 */
	inline TInt GetScriptFile(TExecutableID aExecID, TDes& aScriptFile);

	/*
	 * Utility method to update the permanently allowed or denied
	 * permissions for the script object
	 * 
	 * @param aExecID TExecutableID script identifier of script object whose
	 * permanent permissions are modified
	 * @param aPermGrant TPermGrant  permanently allowed permissions
	 * @param aPermDenied TPermGrant permanently denied permissions
	 * 
	 */
	void UpdatePermGrantL(TExecutableID aExecID, TPermGrant aPermGrant,
			TPermGrant aPermDenied);
	
	/*
	 * Utility method to create a backup for the policy file
	 * 
	 * @param aPolicyID TPolicyID policy identifier for the file
	 * getting backed up
	 * @return KErrNone if backupfile is successsfully created
	 *
	 */
	inline TInt BackupFile(TPolicyID aPolicyID);
	
	/*
	 * Utility method to remove the backup file created
	 * 
	 * @param aPolicyID TPolicyID policy identifier for the file
	 * whose backup is getting removed
	 * @return KErrNone if backup file is successsfully removed
	 *
	 */
	inline TInt RemoveTempPolicy(TPolicyID aPolicyID);
	
	/*
	 * Utility method to restore the backup file created if update fails
	 * 
	 * @param aPolicyID TPolicyID policy identifier for the file
	 * which is restored back
	 * @return KErrNone if backup file is successsfully restored
	 *
	 */
	inline TInt RestoreTempPolicy(TPolicyID aPolicyID);
	
	/*
	 * Utility method to get the policy file information
	 * 
	 * @param aPolicyID TPolicyID policy identifier for the file
	 * which is restored back
	 * @return CPolicy* pointer to instance of Cpolicy stored in CPolicyManager 
	 *
	 */
	inline CPolicy* ReturnPolicyInfo(TPolicyID aPolicyID);
	
	/*
	 * Utility method to store the policy file information
	 * 
	 * @param aPolicy CPolicy policy information from parser
	 * 
	 */
	inline void StorePolicyL(const CPolicy& aPolicy);

	
private:
	/*
	 *  Constructor
	 */
	inline CRTSecMgrServer(CActive::TPriority aActiveObjectPriority);

	/*
	 *  Second phase constructor
	 * 
	 */
	void ConstructL();

	/*
	 *  Server destructor
	 * 
	 */
	~CRTSecMgrServer();

private:

	/*
	 *  shutdown timer object
	 * 
	 */
	CShutDown iShutDown;
	
	/*
	 *  Session counter
	 * 
	 */
	TInt iSessionCount;
	
	/*
	 *  Session container index
	 * 
	 */
	CObjectConIx* iContainerIndex;

	/*
	 *  Reference to policy manager
	 * 
	 */
	CPolicyManager* iPolicyMgr;
	
	/*
	 *  Reference to script manager
	 * 
	 */
	CScriptManager* iScriptMgr;
	
	/*
	 *  Reference to security manager store
	 * 
	 */
	CSecMgrStore* iSecMgrDb;
	};

#include "rtsecmgrserver.inl"
	
#endif //C_CRTSECMGRSERVER_H
