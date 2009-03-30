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
* Description:       Security Manager common header
 *
*/






#ifndef _RTSECMGR_COMMON_DEF_H_
#define _RTSECMGR_COMMON_DEF_H_

#include <e32base.h>
#include <avkon.hrh>

typedef TInt TPolicyID;
typedef TInt TExecutableID;
typedef TUint32 TPermGrant;
typedef RArray<TCapability> RCapabilityArray;

typedef TUint32 TCapabilityBitSet;

//constant for default NULL capability bitset
const TCapabilityBitSet KDefaultNullBit = 0x0000;

//constant for default capability bitset with LSB enabled
const TCapabilityBitSet KDefaultEnableBit = 0x0001;

enum TAccessCheckCode
	{
	EAccessNok = -1,
	EAccessOk
	};

enum       
    {
     EPromptOk = EAknSoftkeyOk,
     EPromptCancel = EAknSoftkeyCancel
    };

/*
 * Enumerations for various duration option
 */
const TUint32 DEFAULT_ONESHOT = 0x80000000;
const TUint32 DEFAULT_SESSION = 0x40000000;
const TUint32 DEFAULT_BLANKET = 0x20000000;

const TUint32 CONDITION_ONESHOT = 0x8000000;
const TUint32 CONDITION_SESSION = 0x4000000;
const TUint32 CONDITION_BLANKET = 0x2000000;

const TUint32 ONESHOT_DENIED = 0x88000000;
const TUint32 SESSION_DENIED = 0x44000000;
const TUint32 BLANKET_DENIED = 0x22000000;

const TUint32 DEFAULT_ALL = 0xE0000000;
const TUint32 CONDITION_ALL = 0xE000000;
const TUint32 CAPABILITY_ALL = 0xFFFFF;

const TUint32 NetworkServices_CAP = 0x00002000;

enum TUserPromptOption
	{
	RTUserPrompt_UnDefined = 0, //can be used for permisssions denied by default
	RTUserPrompt_OneShot = CONDITION_ONESHOT,
	RTUserPrompt_Session = CONDITION_SESSION,
	RTUserPrompt_Permanent = CONDITION_BLANKET,
	RTUserPrompt_Denied = ONESHOT_DENIED,
	RTUserPrompt_SessionDenied = SESSION_DENIED,
	RTUserPrompt_PermDenied = BLANKET_DENIED
	};

enum TSecMgrPromptUIOption
	{
	RTPROMPTUI_DEFAULT = 0,
	RTPROMPTUI_ADVANCED
	};

//error code used btn server and client
//error code starts with -101 as predefined symbian codes
//range upto -48
enum TSecMgrErrCode
	{
	ErrInvalidParameters = -101,
	ErrFileSessionNotShared = ErrInvalidParameters -1,
	ErrInvalidPolicyFormat = ErrFileSessionNotShared -1,
	ErrFatalDBError = ErrInvalidPolicyFormat -1,
	ErrSetPolicyFailed = ErrFatalDBError -1,
	ErrUpdatePolicyFailed = ErrSetPolicyFailed -1,
	ErrInvalidPolicyID = ErrUpdatePolicyFailed -1,
	ErrPolicyIDDoesNotExist = ErrInvalidPolicyID -1,
	ErrInvalidScriptID = ErrPolicyIDDoesNotExist -1,
	ErrRegisterScriptFailed = ErrInvalidScriptID -1,
	ErrUpdateGrantFailed = ErrRegisterScriptFailed -1,
	ErrServerReadConfig = ErrUpdateGrantFailed -1,
	ErrUnRegisterScriptFailed = ErrServerReadConfig -1,
	ErrUnSetPolicyFailed = ErrUnRegisterScriptFailed -1,
	ErrUpdatePermGrantFailed = ErrUnSetPolicyFailed -1,
	ErrBackupNotCreated = ErrUpdatePermGrantFailed - 1,
	ErrRestoreTempFailed = ErrBackupNotCreated - 1,
	ErrAccessDenied = ErrRestoreTempFailed - 1
	};

const TInt KAnonymousScript = -1;
const TInt KInvalidPolicyID = -1;
const TInt KMaxTrustInfoLen = 512; //magic number - half of 1 KB

#endif  //_RTSECMGR_COMMON_DEF_H_
