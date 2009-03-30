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
* Description:       Common server specific definitions
 *
*/






#ifndef _RTSECMGRSERVERDEF_H
#define _RTSECMGRSERVERDEF_H

#include <e32base.h>

#define KMaxName 256
enum TFileType
	{
	EPolicy = 1,
	EScript
	};

/** panic codes */
enum TSecMgrServPanic
	{
	EBadRequest = 1,
	EBadDescriptor,
	EDescriptorNonNumeric,
	EMainSchedulerError,
	ESvrCreateServer,
	ESvrStartServer,
	ECreateTrapCleanup,
	EBadCounterRemove,
	EBadSubsessionHandle
	};

const TUid KCRUIdSecMgr =
	{
			0x2000F847
	};

const TUint32 KSecMgrScriptID = 0x2000F847;

//Pre-defined capability strings that may be defined
//as part of metadata information
_LIT(KCapabilityTCB, "TCB");
_LIT(KCapabilityCommDD, "CommDD");
_LIT(KCapabilityPowerMgmt, "PowerMgmt");
_LIT(KCapabilityMultimediaDD, "MultimediaDD");
_LIT(KCapabilityReadDeviceData, "ReadDeviceData");
_LIT(KCapabilityWriteDeviceData, "WriteDeviceData");
_LIT(KCapabilityDRM, "DRM");
_LIT(KCapabilityTrustedUI, "TrustedUI");
_LIT(KCapabilityProtServ, "ProtServ");
_LIT(KCapabilityDiskAdmin, "DiskAdmin");
_LIT(KCapabilityNetworkControl, "NetworkControl");
_LIT(KCapabilityAllFiles, "AllFiles");
_LIT(KCapabilitySwEvent, "SwEvent");
_LIT(KCapabilityNetworkServices, "NetworkServices");
_LIT(KCapabilityLocalServices, "LocalServices");
_LIT(KCapabilityReadUserData, "ReadUserData");
_LIT(KCapabilityWriteUserData, "WriteUserData");
_LIT(KCapabilityLocation, "Location");
_LIT(KCapabilitySurroundingsDD, "SurroundingsDD");
_LIT(KCapabilityUserEnvironment, "UserEnvironment");

//pre-defined UserPromptOption strings
_LIT(KUserPromptOneShot, "oneshot");
_LIT(KUserPromptSession, "session");
_LIT(KUserPromptBlanket, "blanket");
_LIT(KNo, "no");

_LIT(KPolicyDir, "policy");
_LIT(KScriptDir, "script");
_LIT(KDirSeparator, "\\");
_LIT(KDatExtn, ".dat");
_LIT(KCDrive, "c:");
_LIT(KConfigFile, "config.dat");

#endif    
