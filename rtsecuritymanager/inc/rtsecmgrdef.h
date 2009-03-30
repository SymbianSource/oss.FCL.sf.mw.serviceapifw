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
* Description:       Common definitions of client and server
 *
*/






#ifndef _RTSECMGRDEF_H
#define _RTSECMGRDEF_H

#include <e32base.h>

/*
 * Enumerations for client-server message argument indices
 */
enum TSecMgrMsgSlot
	{
	EMsgArgZero = 0,
	EMsgArgOne,
	EMsgArgTwo,
	EMsgArgThree,
	EMsgArgFour
	};

//opcodes used in message passing between client and server
enum TRTSecServRqst
	{
	ESecServCloseSession = 1,
	ESetPolicy,
	EUpdatePolicy,
	EUnsetPolicy,
	ERegisterScript,
	ERegisterScriptWithHash,
	EUnRegisterScript,
	EGetScriptSession,
	EGetTrustedUnRegScriptSession,
	ECloseScriptSession,
	ECheckPermission,
	EUpdatePermanentGrant,
	EGetScriptFile,
	ESecMgrSrvReqEnd
	};

const TInt KSecurityServerUid2Int(0x1020507E);
const TUid KSecMgrServerUid2 =
	{
			KSecurityServerUid2Int
	};

//the server version. A version must be specifyed when creating a session with the server
const TUint KRTSecMgrServMajorVersionNumber=0;
const TUint KRTSecMgrServMinorVersionNumber=1;
const TUint KRTSecMgrServBuildVersionNumber=1;

// number of message slots.
const TUint KDefaultMessageSlots= 16;
const TUint KSecSrvClientTryCount=2;
const TInt KCapabilitySize = 20;

_LIT(KSecMgrServerExeName, "RTSecMgrServer.exe");
_LIT(KZDrive, "z:");
_LIT(KSecServerProcessName, "SecurityManagerServer");
_LIT(KSecSrvMainThreadName, "SecSrvMain");

#endif //_RTSECMGRDEF_H

