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
* Description:       Security Manager Server Session Class
 *
*/






#ifndef C_CRTSECMGRSESSION_H
#define C_CRTSECMGRSESSION_H

#include "rtsecmgrcommondef.h"
#include "rtsecmgrdata.h"

class CSecMgrStore;
class CRepository;
class CRTSecMgrSubSession;

/*
 * Server side session class
 * 
 * @exe rtsecmgrserver.exe
 * 
 */
class CRTSecMgrSession : public CSession2
	{
public:

	/*
	 * Destructor
	 * 
	 */
	inline ~CRTSecMgrSession();

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CRTSecMgrSession instance
	 * 
	 * @return CRTSecMgrSession* pointer to an instance of CRTSecMgrSession
	 */
	inline static CRTSecMgrSession* NewL();

	/*
	 * Called by client/server framework after 
	 * session has been successfully created
	 * 
	 */
	virtual void CreateL();

	/*
	 * Treats incoming service requests
	 * and disatches accordingly
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 */
	inline void ServiceL(const RMessage2& aMessage);

	/*
	 * Dispatches message to sub-session if the message
	 * is to be handled by server sub-session. Server
	 * session related messages are treated locally
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 */
	void DispatchMessageL(const RMessage2& aMessage);

	/*
	 * Creates new subsession
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 */
	void NewSubSessionL(const RMessage2& aMessage);

	/*
	 * Closes the subsession
	 * 
	 */
	void CloseSession();

	/*
	 * Delete the subsession object through its handle.
	 * 
	 * @param aHandle TInt Subsession handle
	 */
	inline void DeleteSubSession(TInt aHandle);

	/*
	 * Gets The SubSession from Hnadle
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 * @param aHandle TInt Subsession handle
	 * 
	 * @return CRTSecMgrSubSession* pointer to subsession object
	 */
	CRTSecMgrSubSession* SubSessionFromHandle(const RMessage2& aMessage,
			TInt aHandle);

	/*
	 * Utility method to panic client
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 * @param aPanic TInt Panic error code
	 * 
	 */
	inline void PanicClient(const RMessage2& aMessage, TInt aPanic) const;

	/*
	 * Message handler method to set policy
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 * @param aIsUpdate TBool Toggle to identify whether the incoming
	 * message is for setting policy or updating policy
	 * 
	 */
	void SetPolicy(const RMessage2& aMessage, TBool aIsUpdate=EFalse);

	/*
	 * Message handler method to update policy
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 * 
	 */
	void UpdatePolicy(const RMessage2& aMessage);

	/*
	 * Message handler method to unset policy
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 * 
	 */
	void UnsetPolicy(const RMessage2& aMessage);

	/*
	 * Message handler method to register script
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 * @param aHashValue Toggle to indicate if script has hash value
	 * 
	 */
	void RegisterScript(const RMessage2& aMessage, TBool aHashValue=EFalse);

	/*
	 * Message handler method to un-register script
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 * 
	 */
	void UnregisterScript(const RMessage2& aMessage);

	/*
	 * Message handler method to create script sub-session
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 * 
	 */
	void GetScriptSessionL(const RMessage2& aMessage);

	/*
	 * Message handler method to create script sub-session
	 * for trusted unregistered scripts
	 * 
	 * @param aMessage const RMessage2& Incoming message
	 * 
	 */
	void GetTrustedUnRegScriptSessionL(const RMessage2& aMessage);

	/*
	 * Utility method to find out if a script sub-session is open
	 * with the script having the assocaited policy identifier 
	 * specified in the input argument
	 * 
	 * @param aPolicyID TPolicyID policy identifier to be checked
	 * 
	 * @return TBool EFalse if no parallel script sub-sessions are open; ETrue 
	 * otherwise ETrue
	 * 
	 */
	TBool IsScriptOpenWithPolicy(TPolicyID aPolicyID);

	/*
	 * Utility method to find out if a script sub-session is open
	 * with the script having the script identifier 
	 * specified in the input argument
	 * 
	 * @param aScriptID TExecutableID script identifier to be checked
	 * 
	 * @return TBool EFalse if no parallel script sub-sessions are open; ETrue 
	 * otherwise ETrue
	 * 
	 */
	TBool IsScriptSessionOpen(TExecutableID aScriptID,CRTSecMgrSubSession* aCurrentSession=NULL);

private:

	/*
	 * Default private constructor
	 * 
	 * 
	 */
	inline CRTSecMgrSession();

private:
	/*
	 *  Object container for this session.
	 * 
	 */
	CObjectCon *iContainer;

	/*
	 *  Object index which stores objects
	 * 
	 */
	CObjectIx* iSubSessionObjectIndex;

	/*
	 * subsession counter
	 * 
	 */
	TInt iSubSessionCount;
	/*
	 * Security manager server instance
	 * 
	 * Stored as member variable for convenient
	 * access
	 */
	CRTSecMgrServer* iSecMgrServer;
	};

#include "rtsecmgrsession.inl"

#endif //C_CRTSECMGRSESSION_H_

