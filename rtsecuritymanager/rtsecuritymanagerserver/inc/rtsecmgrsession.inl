/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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





inline CRTSecMgrSession::CRTSecMgrSession()
	{
	iSecMgrServer = dynamic_cast<CRTSecMgrServer*> (const_cast<CServer2*> (Server()));
	}
inline CRTSecMgrSession::~CRTSecMgrSession()
	{
	}

inline CRTSecMgrSession* CRTSecMgrSession::NewL()
	{
	return new (ELeave) CRTSecMgrSession();
	}

/**
 * First line servicing of a client request.

 * This function dispatches requests to the appropriate handler.
 * Some messages are handled by the session itself, and are
 * implemented as CRTSecMgrSession member functions, while 
 * other messages are handled by the subsession, and are
 * implemented as CRTSecMgrSubSession member functions.
 */
inline void CRTSecMgrSession::ServiceL(const RMessage2& aMessage)
	{
	TRAPD(err,DispatchMessageL(aMessage));
	aMessage.Complete (err);
	}

/**
 * Delete a subsession object through its handle.
 * 
 */
inline void CRTSecMgrSession::DeleteSubSession(TInt aHandle)
	{
	// This will delete the SubSession object; the object is
	// reference counted, and removing the handle causes the object to be closed
	// [closing reduces the access count - the object is deleted if the access
	//  count reaches zero etc].
	iSubSessionObjectIndex->Remove (aHandle);
	// decrement resource count
	iSubSessionCount--;
	}

inline void CRTSecMgrSession::PanicClient(const RMessage2& aMessage, TInt aPanic) const
	{
	_LIT(KTxtServer,"SecurityMgr server");
	aMessage.Panic (KTxtServer, aPanic);
	}
