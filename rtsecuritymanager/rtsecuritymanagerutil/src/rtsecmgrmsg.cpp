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
* Description:       Defines rtsecmgr common client server message types
 *
*/







#include "rtsecmgrmsg.h"

CRTSecMgrRegisterScriptMsg::CRTSecMgrRegisterScriptMsg(TPolicyID aPolicyID) :
	iPolicyID(aPolicyID)
	{
	// No implementation required
	}

EXPORT_C CRTSecMgrRegisterScriptMsg::~CRTSecMgrRegisterScriptMsg()
	{
	if ( iHashMarker)
		{
		delete iHashMarker;
		}
	}

EXPORT_C CRTSecMgrRegisterScriptMsg* CRTSecMgrRegisterScriptMsg::NewLC(
		TPolicyID aPolicyID, const TDesC& aHashValue)
	{
	CRTSecMgrRegisterScriptMsg* self = new (ELeave)CRTSecMgrRegisterScriptMsg(aPolicyID);
	CleanupStack::PushL (self);
	self->ConstructL (aHashValue);
	return self;
	}

EXPORT_C CRTSecMgrRegisterScriptMsg* CRTSecMgrRegisterScriptMsg::NewL(
		TPolicyID aPolicyID, const TDesC& aHashValue)
	{
	CRTSecMgrRegisterScriptMsg* self=CRTSecMgrRegisterScriptMsg::NewLC (
			aPolicyID, aHashValue);
	CleanupStack::Pop (self); // self;
	return self;
	}

// Creates a CRTSecMgrRegisterScriptMsg initialized with the contents of the
// descriptor parameter
EXPORT_C CRTSecMgrRegisterScriptMsg* CRTSecMgrRegisterScriptMsg::NewLC(const TDesC8& aStreamData)
	{
	// Reads descriptor data from a stream
	// and creates a new CRTSecMgrRegisterScriptMsg object
	CRTSecMgrRegisterScriptMsg* self = new (ELeave) CRTSecMgrRegisterScriptMsg();
	CleanupStack::PushL (self);
	
	// Open a read stream for the descriptor
	RDesReadStream stream(aStreamData);
	CleanupClosePushL (stream);
	self->InternalizeL (stream);
	CleanupStack::PopAndDestroy (&stream); // finished with the stream
	return (self);
	}

void CRTSecMgrRegisterScriptMsg::ConstructL(const TDesC& aHashValue)
	{
	if ( iHashMarker)
		{
		delete iHashMarker;
		iHashMarker = NULL;
		}

	iHashMarker = aHashValue.AllocL ();
	}

// Creates and returns a heap descriptor which holds contents of ’this’
EXPORT_C HBufC8* CRTSecMgrRegisterScriptMsg::PackMsgL() const
	{
	// Dynamic data buffer
	CBufFlat* buf = CBufFlat::NewL(KMaxMsgLength);
	CleanupStack::PushL(buf);
	RBufWriteStream stream(*buf); // Stream over the buffer
	CleanupClosePushL(stream);
	ExternalizeL(stream);
	CleanupStack::PopAndDestroy(&stream);
	// Create a heap descriptor from the buffer
	HBufC8* des = HBufC8::NewL(buf->Size());
	TPtr8 ptr(des->Des());
	buf->Read(0, ptr, buf->Size());
	CleanupStack::PopAndDestroy(buf); // Finished with the buffer
	return (des);
	}

// Writes ’this’ to aStream
void CRTSecMgrRegisterScriptMsg::ExternalizeL(RWriteStream& aStream) const
	{
	if ( iHashMarker)
		aStream << *iHashMarker;
	else
		aStream << KNullDesC8;
	
	aStream.WriteInt32L (iPolicyID); // Write iPolicyID to the stream
	}

// Initializes ’this’ with the contents of aStream
void CRTSecMgrRegisterScriptMsg::InternalizeL(RReadStream& aStream)
	{
	iHashMarker = HBufC::NewL (aStream, KMaxHashValueDesLen);
	iPolicyID = aStream.ReadInt32L (); // Read iPolicyID
	}
