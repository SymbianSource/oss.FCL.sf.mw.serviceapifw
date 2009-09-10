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

EXPORT_C CRTPermGrantMessage::~CRTPermGrantMessage()
    {
    iAllowedProviders.Close();
    iDeniedProviders.Close();
    }

EXPORT_C CRTPermGrantMessage* CRTPermGrantMessage::NewL()
    {
    CRTPermGrantMessage* self = CRTPermGrantMessage::NewLC();
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CRTPermGrantMessage* CRTPermGrantMessage::NewLC()
    {
    CRTPermGrantMessage* self = new(ELeave) CRTPermGrantMessage();
    CleanupStack::PushL(self);
    return self;
    }

EXPORT_C CRTPermGrantMessage* CRTPermGrantMessage::NewL(const TDesC8& aBuf)
    {
    CRTPermGrantMessage* self = CRTPermGrantMessage::NewLC(aBuf);
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CRTPermGrantMessage* CRTPermGrantMessage::NewLC(const TDesC8& aBuf)
    {
    CRTPermGrantMessage* self = new(ELeave) CRTPermGrantMessage();
    CleanupStack::PushL(self);
    self->ConstructL(aBuf);
    return self;
    }

EXPORT_C CRTPermGrantMessage* CRTPermGrantMessage::NewL(RProviderArray aAllowedProviders, RProviderArray aDeniedProviders,TExecutableID aScriptId)
    {
    CRTPermGrantMessage* self = CRTPermGrantMessage::NewLC(aAllowedProviders,aDeniedProviders,aScriptId);
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CRTPermGrantMessage* CRTPermGrantMessage::NewLC(RProviderArray aAllowedProviders, RProviderArray aDeniedProviders,TExecutableID aScriptId)
    {
    CRTPermGrantMessage* self = new(ELeave) CRTPermGrantMessage(aAllowedProviders,aDeniedProviders,aScriptId);
    CleanupStack::PushL(self);
    return self;
    }

CRTPermGrantMessage::CRTPermGrantMessage()
    {
    
    }

CRTPermGrantMessage::CRTPermGrantMessage(RProviderArray aAllowedProviders,RProviderArray aDeniedProviders,TExecutableID aScriptId)
    {
    iAllowedProviders.Reset();
    for(TInt i(0); i < aAllowedProviders.Count(); i++)
        iAllowedProviders.Append(aAllowedProviders[i]);
    iDeniedProviders.Reset();
    for(TInt i(0); i < aDeniedProviders.Count(); i++)
        iDeniedProviders.Append(aDeniedProviders[i]);
    iScriptId = aScriptId;
    }

void CRTPermGrantMessage::ConstructL(const TDesC8& aBuf)
    {
    RDesReadStream stream(aBuf);
    CleanupClosePushL (stream);
    InternalizeL (stream);
    CleanupStack::PopAndDestroy (&stream);
    }

EXPORT_C void CRTPermGrantMessage::AllowedProviders(RProviderArray& aAllowedProviders)
    {
    aAllowedProviders.Reset();
    for(TInt i(0); i < iAllowedProviders.Count(); i++)
        aAllowedProviders.Append(iAllowedProviders[i]);
    }

EXPORT_C void CRTPermGrantMessage::DeniedProviders(RProviderArray& aDeniedProviders)
    {
    aDeniedProviders.Reset();
    for(TInt i(0); i < iDeniedProviders.Count(); i++)
        aDeniedProviders.Append(iDeniedProviders[i]);
    }

EXPORT_C TExecutableID CRTPermGrantMessage::ScriptID()
    {
    return iScriptId;
    }

EXPORT_C void CRTPermGrantMessage::setAllowedProviders(RProviderArray aAllowedProviders)
    {
    iAllowedProviders.Reset();
    for(TInt i(0); i < aAllowedProviders.Count(); i++)
            iAllowedProviders.Append(aAllowedProviders[i]);
    }

EXPORT_C void CRTPermGrantMessage::setDeniedProviders(RProviderArray aDeniedProviders)
    {
    iDeniedProviders.Reset();
    for(TInt i(0); i < aDeniedProviders.Count(); i++)
            iDeniedProviders.Append(aDeniedProviders[i]);
    }

EXPORT_C void CRTPermGrantMessage::setScriptID(TExecutableID aScriptId)
    {
    iScriptId = aScriptId;
    }

void CRTPermGrantMessage::InternalizeL(RReadStream& aSource)
    {
    iScriptId = aSource.ReadInt32L();
    TInt allowCnt = aSource.ReadInt32L();
    iAllowedProviders.Reset();
    for(TInt i(0); i < allowCnt; i++)
        {
        TInt uid = aSource.ReadInt32L();
        TUid allowPid = TUid::Uid(uid);
        iAllowedProviders.Append(allowPid);
        }
    TInt denyCnt = aSource.ReadInt32L();
    iDeniedProviders.Reset();
    for(TInt i(0); i < denyCnt; i++)
        {
        TInt uid = aSource.ReadInt32L();
        TUid denyPid = TUid::Uid(uid);
        iDeniedProviders.Append(denyPid);
        }
    }

void CRTPermGrantMessage::ExternalizeL(RWriteStream& aSink)
    {
    aSink.WriteInt32L(iScriptId);
    TInt cnt = iAllowedProviders.Count();
    aSink.WriteInt32L(cnt);
    for(TInt i(0); i < iAllowedProviders.Count(); i++)
        aSink.WriteInt32L(iAllowedProviders[i].iUid);
    cnt = iDeniedProviders.Count();
    aSink.WriteInt32L(cnt);
    for(TInt i(0); i < iDeniedProviders.Count(); i++)
        aSink.WriteInt32L(iDeniedProviders[i].iUid);
    }

EXPORT_C HBufC8* CRTPermGrantMessage::PackMessageL()
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
