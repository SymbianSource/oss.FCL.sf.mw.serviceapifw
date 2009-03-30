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
* Description:       Security Manager Server Script Sub-Session Class
 *
*/






#ifndef C_CRTSECMGRSUBSESSION_H
#define C_CRTSECMGRSUBSESSION_H

#include "rtsecmgrcommondef.h"
#include "rtsecmgrdata.h"
#include "rtsecmgrserver.h"

class CRTSecMgrSession;

/*
 * Server side script sub-session class
 * 
 * @exe rtsecmgrserver.exe
 * 
 */
class CRTSecMgrSubSession : public CObject
	{
public:

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CRTSecMgrSubSession instance
	 * 
	 * @return CRTSecMgrSubSession* pointer to an instance of CRTSecMgrSession
	 */
	inline static CRTSecMgrSubSession* NewL(CRTSecMgrSession* aSession,
			CScript* aScript,CRTSecMgrServer* aSecMgrServer);

	/**
	 * Destructor
	 * 
	 */
	inline ~CRTSecMgrSubSession();

	/**
	 * Handles get script file service
	 * 
	 * @param aMessage const RMessage2& Input message
	 * 
	 */
	void GetScriptFile(const RMessage2& aMessage);

	/**
	 * Handles update permanent permission service
	 * 
	 * @param aMessage const RMessage2& Input message
	 * 
	 */
	void UpdatePermGrantL(const RMessage2& aMessage);

	/**
	 * Gets script identifier of the underlying script object
	 * 
	 * @return TExecutableID script identifier of script object
	 * 
	 */
	inline TExecutableID ScriptID() const;

	/**
	 * Gets the underlying script object
	 * 
	 * @return const CScript& the script object
	 * 
	 */
	inline const CScript& Script() const;
private:

	/**
	 * Constructor
	 * 
	 */
	inline CRTSecMgrSubSession(CRTSecMgrSession* aSession, CScript* aScript,CRTSecMgrServer* aSecMgrServer);

private:

	/*
	 * 
	 * Reference to parent session object
	 */
	CRTSecMgrSession* iSession; // The session that owns this CRTSecMgrSession object.

	/*
	 * script identifier
	 * 
	 */
	TExecutableID iScriptID;

	/*
	 * 
	 * pointer to script object
	 */
	CScript* iScript;
	/*
	 * Security manager server instance
	 * 
	 * Stored as member variable for convenient
	 * access
	 */
	CRTSecMgrServer* iSecMgrServer;
	};

#include "rtsecmgrsubsession.inl"

#endif //C_CRTSECMGRSUBSESSION_H

