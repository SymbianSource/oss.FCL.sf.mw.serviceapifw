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





/*
 * Security Manager Server's policy
 *
 */

/*
 * Total number of ranges 
 */
const TUint KSecMgrSrvPolicyRangeCount = 1;

// Definition of the ranges of IPC numbers
const TInt KSecMgrSrvRanges[KSecMgrSrvPolicyRangeCount] =
    {
    0
    //,   
    //ESecMgrSrvReqEnd             // 2nd range, non implemented function end of range check; ENotSupported
    };

// Policy to implement for each of the above ranges
const TUint8 KSecMgrSrvElementsIndex[KSecMgrSrvPolicyRangeCount] =
    {
    0
    //,                           // Applies to 0th range
    //CPolicyServer::ENotSupported // Applies to 1st range
    };

// Specific capability checks
const CPolicyServer::TPolicyElement KSecMgrSrvElements[] =
    {
    { _INIT_SECURITY_POLICY_C1( ECapabilityReadUserData ),  CPolicyServer::EFailClient },  // Policy "0"
    //{ _INIT_SECURITY_POLICY_C1( ECapabilityWriteDeviceData ), CPolicyServer::EFailClient },  // Policy "1"
    //{ _INIT_SECURITY_POLICY_S0( KSensrvTestFrameworkUid ),    CPolicyServer::EPanicClient }, // Policy "2", i.e. access is allowed only for test framework
    };

// Package all the above together into a policy
const CPolicyServer::TPolicy KSecMgrSrvPolicy =
    {
    CPolicyServer::EAlwaysPass, 	// All connect attempts to Security Manager Server pass
    KSecMgrSrvPolicyRangeCount,    // Number of ranges
    KSecMgrSrvRanges,              // Ranges array
    KSecMgrSrvElementsIndex,       // Elements <-> ranges index
    KSecMgrSrvElements,            // Array of elements
    };
