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
* Description:       Definition of script manager class
 *
*/






#ifndef C_RTSECMGRSCRIPTMANAGER_H
#define C_RTSECMGRSCRIPTMANAGER_H

#include <e32capability.h>
#include <rtsecmgrutility.h>
#include <rtsecmgrscript.h>
#include "rtsecmgrdef.h"
#include "rtsecmgrstore.h"

//Forward declarations
class CSecMgrStore;
class CPolicyManager;

typedef RPointerArray<CScript> RScripts;

/*
 * Models the cache to maintain the list of registered script
 * data. Provides operations to register script, de-register 
 * script, query script related information.
 *
 * This class abstracts the underlying persistent storage to
 * store script related data.
 *
 * @see CSecMgrStore
 * @see CPolicyManager
 * @see CScript
 * 
 * @exe rtsecmgrserver.exe
 */
class CScriptManager : public CBase
	{
public:
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CScriptManager instance
	 * 
	 * @param aSecMgrDB CSecMgrStore* reference to security manager store
	 * @param aPolicyMgr CPolicyManager* reference to policymanager instance
	 * 
	 * @return CScriptManager* pointer to an instance of CScriptManager
	 */
	static CScriptManager* NewL(CSecMgrStore* aSecMgrDB,
			CPolicyManager* aPolicyMgr);

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CScriptManager instance and leaves the created instance
	 * on the cleanupstack
	 * 
	 * @param aSecMgrDB CSecMgrStore* reference to security manager store
	 * @param aPolicyMgr CPolicyManager* reference to policymanager instance
	 * 
	 * @return CScriptManager* pointer to an instance of CScriptManager
	 */
	static CScriptManager* NewLC(CSecMgrStore* aSecMgrDB,
			CPolicyManager* aPolicyMgr);

	/**
	 * Destructor
	 * 
	 * Performs clean-up of transient script store cache
	 * 
	 */
	virtual ~CScriptManager();

	/**
	 * Registers a script based on a policy identified by
	 * input policy identifier with security manager
	 * 
	 * @param aPolicyID TPolicyID input policy identifier of the script
	 * 
	 * @return TExecutableID script identifier of the registered script
	 */
	TExecutableID
			RegisterScript(TPolicyID aPolicyID);

	/**
	 * Registers a script with hashvalue based on a policy identified by
	 * input policy identifier with security manager. 
	 * 
	 * @param aPolicyID TPolicyID input policy identifier of the script
	 * @param aHashValue const TDesC& hashValue of the script
	 * 
	 * @return TExecutableID script identifier of the registered script
	 */
	TExecutableID
			RegisterScript(TPolicyID aPolicyID, const TDesC& aHashValue);

	/**
	 * Un-Registers a script
	 * 
	 * @param aExecID script identifier of the registered script
	 * @param aPolicyID policy identifier passed while registering script
	 * 
	 */
	void UnRegisterScriptL(TExecutableID aExecID, TPolicyID aPolicyID);

	/**
	 * Gets underlying script data
	 * 
	 * @param aScriptInfo CScript& output script data
	 * 
	 * @return KErrNone if script data could be returned; Otherwise one of
	 * system error codes
	 * 
	 */
	TInt ScriptInfo(CScript& aScriptInfo);

	/**
	 * Updates permanently granted or denied permission data
	 * of the script
	 * 
	 * @param aExecID 	 TExecutableID script identifier
	 * @param aPermGrant TPermGrant permanently allowed permission
	 * @param aPermDenied TPermGrant permanently denied permission
	 * 
	 */
	void UpdatePermGrantL(TExecutableID aExecID, TPermGrant aPermGrant,
			TPermGrant aPermDenied);

	/**
	 * Gets the script file name
	 * 
	 * @param aExecID 	 TExecutableID script identifier
	 * @param TDes& Output file name
	 * 
	 * @return KErrNone if operation is successful; Otherwise one of
	 * system wide error codes
	 * 
	 */
	TInt GetScriptFile(TExecutableID aExecID, TDes& aScriptFile);

private:

	/**
	 * Overloaded constructor to initialise internal
	 * state of CScriptManager
	 * 
	 */
	inline CScriptManager(CSecMgrStore* aSecMgrDB, CPolicyManager* aPolicyMgr) :
		iSecMgrDB(aSecMgrDB), iPolicyMgr(aPolicyMgr)
		{
		}

	/**
	 * Two-phased constructor
	 * 
	 */
	void ConstructL();

	/**
	 * Generates a new script identifier
	 * 
	 */
	inline TExecutableID GetID()
		{
		if ( --iID > 0)
			return iID;

		return (TExecutableID)KSecurityServerUid2Int;
		}

private:
	
	//list of script data structure
	RScripts iScripts;
	
	//reference to security manager store
	CSecMgrStore* iSecMgrDB;
	
	//reference to policymanager instance	
	CPolicyManager* iPolicyMgr;
	
	//Last generated script identifier
	TExecutableID iID;
	};

#endif  //C_RTSECMGRSCRIPTMANAGER_H

