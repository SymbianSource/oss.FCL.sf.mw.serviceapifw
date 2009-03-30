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
* Description:       Defines runtime security manager script session class
 *
*/






#include <aknlistquerydialog.h>
#include <aknquerydialog.h>
#include <aknmessagequerycontrol.h>
#include <aknmessagequerydialog.h> 
#include <badesca.h>
#include <bautils.h>
#include <coemain.h>
#include <data_caging_path_literals.hrh>
#include <eikenv.h>
#include <eikfrlb.h>
#include <eikfrlbd.h>
#include <pathinfo.h> 
#include <stringloader.h>

#include <rtsecmanager.h>
#include <rtsecmgrscriptsession.h>
#include <rtsecmgrscript.h>
#include "rtsecmgrclient.h"
#include "rtsecmgrdef.h"

//security manager resource file literal
_LIT(KSecMgrResourceFile, "RTSecManager.RSC");

//constant for descriptor array granularity
const TInt KDescArrayInit = 6;
const TInt KMaxBuf=256;

//typedef of selection index array
typedef RArray<TInt> RSelIdxArray;

//static resource IDs for capability text display in default prompt
static const TInt CAPABILITY_RESOURCEID[KCapabilitySize] =
	{
	0, //ECapabilityTCB (0) 
	R_RTSECMGR_COMMDD, //ECapabilityCommDD (1)
	R_RTSECMGR_POWERMGMT, //ECapabilityPowerMgmt (2)
	R_RTSECMGR_MMEDIADD, //ECapabilityMultimediaDD (3)
	R_RTSECMGR_READDEVICEDATA, //ECapabilityReadDeviceData (4)
	R_RTSECMGR_WRITEDEVICEDATA, //ECapabilityWriteDeviceData (5)
	R_RTSECMGR_DRM, //ECapabilityDRM (6)
	R_RTSECMGR_TRUSTEDUI, //ECapabilityTrustedUI (7)
	R_RTSECMGR_PROTSERV, //ECapabilityProtServ (8)
	R_RTSECMGR_DISKADMIN, //ECapabilityDiskAdmin (9)
	R_RTSECMGR_NWCONTROL, //ECapabilityNetworkControl (10)
	R_RTSECMGR_ALLFILES, //ECapabilityAllFiles (11)
	R_RTSECMGR_SWEVENT, //ECapabilitySwEvent (12)
	R_RTSECMGR_NWSERVICES, //ECapabilityNetworkServices (13)
	R_RTSECMGR_LOCALSERVICES, //ECapabilityLocalServices (14)
	R_RTSECMGR_READUSERDATA, //ECapabilityReadUserData (15)
	R_RTSECMGR_WRITEUSERDATA, //ECapabilityWriteUserData (16)
	R_RTSECMGR_LOCATION, //ECapabilityLocation (17)
	R_RTSECMGR_SURRDD, //ECapabilitySurroundingsDD (18)
	R_RTSECMGR_USERENV	//ECapabilityUserEnvironment (19)					
		};

static const TInt MOREINFO_CAP_RESOURCEID[KCapabilitySize] = 
	{
	0, //ECapabilityTCB (0) 
	R_RTSECMGR_MORE_INFO_COMMDD, //ECapabilityCommDD (1)
	R_RTSECMGR_POWERMGMT, //ECapabilityPowerMgmt (2)
	R_RTSECMGR_MORE_INFO_MMEDIADD, //ECapabilityMultimediaDD (3)
	R_RTSECMGR_MORE_INFO_READDEVICEDATA, //ECapabilityReadDeviceData (4)
	R_RTSECMGR_MORE_INFO_WRITEDEVICEDATA, //ECapabilityWriteDeviceData (5)
	0, //ECapabilityDRM (6)
	0, //ECapabilityTrustedUI (7)
	0, //ECapabilityProtServ (8)
	0, //ECapabilityDiskAdmin (9)
	R_RTSECMGR_MORE_INFO_NWCONTROL, //ECapabilityNetworkControl (10)
	0, //ECapabilityAllFiles (11)
	R_RTSECMGR_MORE_INFO_SWEVENT, //ECapabilitySwEvent (12)
	R_RTSECMGR_MORE_INFO_NWSERVICES, //ECapabilityNetworkServices (13)
	R_RTSECMGR_MORE_INFO_LOCALSERVICES, //ECapabilityLocalServices (14)
	R_RTSECMGR_MORE_INFO_READ_USERDATA, //ECapabilityReadUserData
	R_RTSECMGR_MORE_INFO_WRITE_USERDATA,//ECapabilityWriteUserData
	R_RTSECMGR_MORE_INFO_LOCATION,//ECapabilityLocation
	R_RTSECMGR_MORE_INFO_SURRDD, //ECapabilitySurroundingsDD
	R_RTSECMGR_MORE_INFO_USERENV //ECapabilityUserEnvironment
	};

struct TCallbackParam
{
	CRTSecMgrScriptSession* iRTSecMgrScriptSession;
	CPromptData *iPromptData;	
};

/**
 * Proxy delegate class for runtime security manager client-side
 * sub-session handle.
 * 
 * @lib rtsecmgrclient.lib
 */
class CRTSecMgrSubSessionProxy : public CBase
	{
public:

	static CRTSecMgrSubSessionProxy* NewL()
		{
		CRTSecMgrSubSessionProxy* self = CRTSecMgrSubSessionProxy::NewLC ();
		CleanupStack::Pop (self);
		return self;
		}

	static CRTSecMgrSubSessionProxy* NewLC()
		{
		CRTSecMgrSubSessionProxy* self = new (ELeave) CRTSecMgrSubSessionProxy();
		CleanupStack::PushL (self);
		self->ConstructL ();
		return self;
		}

	virtual ~CRTSecMgrSubSessionProxy()
		{
		iSubSession.Close ();
		}

	inline RSecMgrSubSession& SubSession()
		{
		return iSubSession;
		}
	RSecMgrSubSession* operator ->()
	{
	return &iSubSession;
	}
private:
	inline CRTSecMgrSubSessionProxy()
		{
		}
	void ConstructL()
		{
		}

	RSecMgrSubSession iSubSession;
	};

// ---------------------------------------------------------------------------
// Defintiion of default private constructor
// ---------------------------------------------------------------------------
//
CRTSecMgrScriptSession::CRTSecMgrScriptSession(MSecMgrPromptHandler* aPromptHdlr) :
	iPermBits(KDefaultNullBit), iDeniedBits(KDefaultNullBit),
			iPromptHdlr(aPromptHdlr), iUIPromptOption(RTPROMPTUI_DEFAULT), isCustomPrompt(EFalse)
	{
	_sessionInfo.AllowedCaps = KDefaultNullBit;
	_sessionInfo.DeniedCaps = KDefaultNullBit;
}

// ---------------------------------------------------------------------------
// Defintiion of second-phase constructor
// ---------------------------------------------------------------------------
//
CRTSecMgrScriptSession* CRTSecMgrScriptSession::NewL(
		MSecMgrPromptHandler* aPromptHdlr)
	{
	CRTSecMgrScriptSession* self = CRTSecMgrScriptSession::NewLC (aPromptHdlr);
	CleanupStack::Pop (self);
	return self;
	}

// ---------------------------------------------------------------------------
// Defintiion of second-phase constructor
// ---------------------------------------------------------------------------
//
CRTSecMgrScriptSession* CRTSecMgrScriptSession::NewLC(
		MSecMgrPromptHandler* aPromptHdlr)
	{
	CRTSecMgrScriptSession* self = new (ELeave) CRTSecMgrScriptSession(aPromptHdlr);
	CleanupStack::PushL (self);
	self->ConstructL ();
	return self;
	}

// ---------------------------------------------------------------------------
// Defintiion of second-phase constructor
// This method instantiates client side sub-session proxy instance.
// In addition, this method loads the security manager resource
// ---------------------------------------------------------------------------
//
void CRTSecMgrScriptSession::ConstructL()
	{
	iCoeEnv = CCoeEnv::Static ();
	if ( !iCoeEnv && !iPromptHdlr)
		User::Leave (KErrNotSupported);

	iSubSessionProxy = CRTSecMgrSubSessionProxy::NewL ();

	if ( !iPromptHdlr)
	    {
	    iPromptHdlr = this; //default prompt handler	
	    isCustomPrompt = ETrue ;
	    }
				

	}

void CRTSecMgrScriptSession::AddResourceFiles()
    {
        if(iCoeEnv)
        {
            
        CDesCArray* diskList = new (ELeave) CDesCArrayFlat(KDescArrayInit);
        CleanupStack::PushL (diskList);

        BaflUtils::GetDiskListL (iCoeEnv->FsSession (), *diskList);
        for (TInt idx(0); idx<diskList->Count ();++idx)
            {
            TInt intDrive;
            TChar ch = ((*diskList)[idx])[0];
            RFs::CharToDrive (ch, intDrive);
            TDriveUnit curDrive(intDrive);

            TFileName resFile(curDrive.Name ());
            resFile.Append (KDC_APP_RESOURCE_DIR);
            resFile.Append (KSecMgrResourceFile);
            BaflUtils::NearestLanguageFile (iCoeEnv->FsSession (), resFile);
            if ( BaflUtils::FileExists (iCoeEnv->FsSession (), resFile))
                {
                TRAPD (err, iResourceOffset = iCoeEnv->AddResourceFileL (resFile));
                User::LeaveIfError (err);
                break;
                }
            }

        CleanupStack::PopAndDestroy (diskList);
        }        
    }

void CRTSecMgrScriptSession::CloseResourceFiles()
    {
    if ( iResourceOffset && iCoeEnv)
            iCoeEnv->DeleteResourceFile (iResourceOffset);
	iResourceOffset = 0;
    }
// ---------------------------------------------------------------------------
// Destructor
// Closes script sub-session handle
// ---------------------------------------------------------------------------
//
EXPORT_C CRTSecMgrScriptSession::~CRTSecMgrScriptSession()
	{
	Close ();
	}

// ---------------------------------------------------------------------------
// Opens script session, in turn opening client-side sub-session handle
// ---------------------------------------------------------------------------
//
TInt CRTSecMgrScriptSession::Open(const RSessionBase& aParentSession,
		TPolicyID aPolicyID, TExecutableID aExecID)
	{
	if (iScript)
		{
		delete iScript;
		iScript = NULL;
		}
	
	iScript = CScript::NewL (aPolicyID, aExecID);

	TInt ret((*iSubSessionProxy)->Open (aParentSession, *iScript, aPolicyID));
	if ( KErrNone==ret)
		{
		iPermBits = iScript->PermGranted ();
		iDeniedBits = iScript->PermDenied ();
		}

	return ret;
	}

// ---------------------------------------------------------------------------
// Opens script session, in turn opening client-side sub-session handle
// ---------------------------------------------------------------------------
//
TInt CRTSecMgrScriptSession::Open(const RSessionBase& aParentSession,
		TPolicyID aPolicyID, TExecutableID aExecID, const TDesC& aHashValue)
	{
	if (iScript)
		{
		delete iScript;
		iScript = NULL;
		}
	
	iScript = CScript::NewL (aPolicyID, aExecID);
	TInt ret((*iSubSessionProxy)->Open (aParentSession, *iScript, aPolicyID, aHashValue));
	if ( KErrNone==ret)
		{
		iPermBits = iScript->PermGranted ();
		iDeniedBits = iScript->PermDenied ();
		}

	return ret;
	}

// ---------------------------------------------------------------------------
// Opens script session, in turn opening client-side sub-session handle
// ---------------------------------------------------------------------------
//
TInt CRTSecMgrScriptSession::Open(const RSessionBase& aParentSession,
		TPolicyID aPolicyID, TExecutableID aExecID, const CTrustInfo& aTrustInfo)
	{
	if (iScript)
		{
		delete iScript;
		iScript = NULL;
		}
	
	iScript = CScript::NewL (aPolicyID, aExecID);
	TInt ret((*iSubSessionProxy)->Open (aParentSession, *iScript, aPolicyID, aTrustInfo));

	if ( KErrNone==ret)
		{
		iPermBits = iScript->PermGranted ();
		iDeniedBits = iScript->PermDenied ();
		}

	return ret;
	}

// ---------------------------------------------------------------------------
// Performs access permission check, returns unmatched capabilities if any
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRTSecMgrScriptSession::IsAllowed(const RCapabilityArray& aCapabilitiesToCheck,RCapabilityArray& aUnMatchedCapabilities)
	{
	RPromptDataList promptDataList;
	TCapabilityBitSet unMatchedCapBits(KDefaultNullBit);
	TInt ret(IsAllowed(aCapabilitiesToCheck,promptDataList,unMatchedCapBits));
	
	promptDataList.ResetAndDestroy();

	if(KDefaultNullBit!=unMatchedCapBits)
	BuildCapsL(unMatchedCapBits,aUnMatchedCapabilities);
	return ret;
	}

// ---------------------------------------------------------------------------
// Performs access permission check
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CRTSecMgrScriptSession::IsAllowed(const RCapabilityArray& aCapabilitiesToCheck)
 	{
 	RPromptDataList promptDataList;
	TCapabilityBitSet unMatchedCapBits(KDefaultNullBit);
	TInt ret(IsAllowed(aCapabilitiesToCheck,promptDataList,unMatchedCapBits));
	promptDataList.ResetAndDestroy();
	return ret;
	}

// ---------------------------------------------------------------------------
// Private implementation to perform access perform access check.
// This function is the real task master
// ---------------------------------------------------------------------------
//
TInt CRTSecMgrScriptSession::IsAllowed(
		const RCapabilityArray& aCapabilitiesToCheck,
		RPromptDataList& aPromptDataList, TCapabilityBitSet& aUnMatchedCaps)
	{
	if ( aCapabilitiesToCheck.Count ()<=0)
		{
		return EAccessOk; //if no capabilities are required, safely return
		}
	if ( aCapabilitiesToCheck.Find(ECapabilityTCB) != KErrNotFound)
		return EAccessNok;
	aPromptDataList.Reset ();

	TCapabilityBitSet capToCheck(KDefaultNullBit);
	TCapabilityBitSet finalCaps(KDefaultNullBit);
	for (TInt i(0); i!=aCapabilitiesToCheck.Count (); ++i)
		AddCapability (capToCheck, aCapabilitiesToCheck[i]);

	capToCheck &= ~(iScript->PermissionSet().UnconditionalCaps());
	
	/* Check if a;ready denied. No point in going forward */
	if(capToCheck & _sessionInfo.DeniedCaps || capToCheck & iDeniedBits)
	        return EAccessNok;
	
	TCapabilityBitSet allowedCaps(KDefaultNullBit);
	allowedCaps |= _sessionInfo.AllowedCaps; //for session allowed
	allowedCaps &= ~iDeniedBits;
	allowedCaps |= iPermBits; //for permanently allowed
	
	//In case if all the capabilities required by the service provider
	//are allowed without prompting
	TCapabilityBitSet original_capToCheck = capToCheck;
	capToCheck &= allowedCaps;
	if(original_capToCheck == capToCheck)
		return EAccessOk;

	capToCheck ^= original_capToCheck ;
	TBool isPermGrantModified(EFalse);

	RPermissions perms = iScript->PermissionSet().Permissions ();
	
	for (TInt i(0); (i!=perms.Count ())&& capToCheck ;++i)
		{
		CPermission* perm = CPermission::NewLC(*perms[i]);

		TCapabilityBitSet tempCapToCheck(KDefaultNullBit);

		RCapabilityArray capabilities;
		perm->Capabilitilites (capabilities);

		for (TInt capsIdx(0); capsIdx!=capabilities.Count();++capsIdx)
			{
			TCapability cap(capabilities[capsIdx]);
			tempCapToCheck |= LOWBIT << cap;
			}
		capToCheck &= tempCapToCheck;
		if ( capToCheck )
			{
			if ( capToCheck & iDeniedBits) //check if permanently disabled
				{
				allowedCaps &= ~tempCapToCheck; //just in case...
				_sessionInfo.DeniedCaps &= ~tempCapToCheck; //just in case...
				}
			else
				if ( capToCheck & _sessionInfo.DeniedCaps) //not necessary to do this check...
					{
					allowedCaps &= ~tempCapToCheck; //just in case...	
					}
				else
					if ((capToCheck & allowedCaps) != capToCheck) //check if it's already allowed
						{
						//capToCheck &= allowedCaps;
						TCapabilityBitSet temp = capToCheck;
						temp &= allowedCaps;
						temp ^= capToCheck;
						capToCheck = temp;
						if ( (perm->Condition() & RTUserPrompt_OneShot) ||(perm->Condition() & RTUserPrompt_Session) ||((perm->Condition() & RTUserPrompt_Permanent) &&
								(iScript->ScriptID()!=KAnonymousScript)))
							{
							if ( !(_sessionInfo.AllowedCaps & capToCheck))
								{
								//check if it's denied for this session
								if ( !(_sessionInfo.DeniedCaps & capToCheck))
									{
									CPromptData* promptData = CPromptData::NewL();//should write NewL Function
									promptData->SetPermissions(*perm);
									promptData->SetUserSelection(perm->Default ()) ;
									aPromptDataList.Append (promptData);
									}
								
								}
							else
								{
								allowedCaps |= tempCapToCheck; //enable in allowed
								}
							}
						}
			}
		capToCheck ^= original_capToCheck;
		capabilities.Close ();
		CleanupStack::PopAndDestroy(perm);
		}//for loop

	if ( aPromptDataList.Count ()>0)
		{
		//TInt ret = EPromptOk;
		TInt ret = iPromptHdlr->Prompt (aPromptDataList , iScript->ScriptID());
		
		//Iterate over promptDataList and check the user selection
			RCapabilityArray capArray;
			for (TInt i(0); i!=aPromptDataList.Count ();++i)
				{
				CPermission *perm = (aPromptDataList[i])->iPermission;
				perm->Capabilitilites(capArray);
				TCapabilityBitSet tempCapToCheck(KDefaultEnableBit);
				TCapabilityBitSet userSelectedCaps = 0;
				for(TInt idx(0); idx != capArray.Count(); idx++)
					userSelectedCaps |= tempCapToCheck <<capArray[idx];	
								
				capArray.Close();
				HandleGrantChosen (aPromptDataList[i], userSelectedCaps, allowedCaps, isPermGrantModified);
				finalCaps |= allowedCaps ;
				}
		
		if ( EPromptOk!=ret)
			{
			return EAccessNok;
			}

		if ( (iScript->ScriptID()!=KAnonymousScript)&&(isPermGrantModified))
			UpdatePermGrant ();//commit perm grant change 

		finalCaps |= iPermBits ;
		
		if((finalCaps & NetworkServices_CAP) && isCustomPrompt)
		    PromptCostL() ;
		
		finalCaps |= _sessionInfo.AllowedCaps ;
		
	
		if ( KErrNone == ((finalCaps & original_capToCheck) ^ original_capToCheck))
			return EAccessOk;
		else
			{
			aUnMatchedCaps = (finalCaps & original_capToCheck) ^ original_capToCheck;
			return EAccessNok;
			}
		}
	else
		{
		return EAccessNok;
		}

	}

// ---------------------------------------------------------------------------
// Default prompt handler implementation
// ---------------------------------------------------------------------------
//
TInt CRTSecMgrScriptSession::Prompt(RPromptDataList& aPromptDataList,TExecutableID /*aExecID*/)
	{
	AddResourceFiles();
	TInt ret(EPromptOk);
    TInt promptCount = aPromptDataList.Count ();
	
	for (TInt i(0); i<promptCount; ++i)
		{
		ret = Prompt (aPromptDataList[i]);
		if ( EPromptOk!=ret)
			{
			for(TInt inLoopCnt(promptCount-1); inLoopCnt>=i; --inLoopCnt)
				{
				TUserPromptOption selected = aPromptDataList[inLoopCnt]->iUserSelection;
				if(!(selected == RTUserPrompt_Denied || selected == RTUserPrompt_SessionDenied || selected == RTUserPrompt_PermDenied))
				    {
    				delete aPromptDataList[inLoopCnt];
    				aPromptDataList.Remove(inLoopCnt);
				    }
				}
			break;
			}
		
		
		}
	CloseResourceFiles();
	return ret;
	}

// ---------------------------------------------------------------------------
// Private default prompt handler implementation
// ---------------------------------------------------------------------------
//
TInt CRTSecMgrScriptSession::Prompt(CPromptData* aPromptData)
	{
	if ( RTPROMPTUI_DEFAULT!=iUIPromptOption)
		return PromptAdvanced (aPromptData);

	TInt ret(EPromptOk);
	TInt stackResCnt(0);

	CDesCArray* settingPageText = new(ELeave) CDesCArrayFlat(KDescArrayInit);
	CleanupStack::PushL(settingPageText);
		
	HBufC* oneShotOptionText=   NULL;
	HBufC* oneShotDenyText=   NULL;
	HBufC* sessionOptionText=   NULL;
	HBufC* sessionDenyOption=   NULL;
	HBufC* permGrantOptionText=   NULL;
	HBufC* permDenyOption=   NULL;

	TInt selIndex(PROMPT_SELIDX_ZERO);
	RSelIdxArray selIdxArray;
	CleanupClosePushL (selIdxArray);
	
	if ( (RTUserPrompt_Permanent & (aPromptData->iPermission->Default ())) || (RTUserPrompt_Permanent & (aPromptData->iPermission->Condition ())))
		{
		permGrantOptionText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_PERM_YES);
		settingPageText->AppendL (permGrantOptionText->Des ());
		++stackResCnt;

		selIdxArray.Append (R_RTSECMGR_PROMPT_PERM_YES);

		if ( RTUserPrompt_Permanent&aPromptData->iPermission->Default ())
			selIndex = selIdxArray.Count ()-1;

	/*		permDenyOption = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_PERM_NO);
			++stackResCnt;
	*/		
		}
	
	if ( (RTUserPrompt_Session & (aPromptData->iPermission->Default ())) || (RTUserPrompt_Session & (aPromptData->iPermission->Condition ())))
		{
		sessionOptionText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_SESSION_YES);
		settingPageText->AppendL (sessionOptionText->Des());
		++stackResCnt;

		selIdxArray.Append (R_RTSECMGR_PROMPT_SESSION_YES);

		TUserPromptOption def = aPromptData->iPermission->Default ();
		if ( RTUserPrompt_Session&def)
			selIndex = selIdxArray.Count()-1;

	/*	if ( !permDenyOption)
			{
			sessionDenyOption = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_SESSION_NO);
			++stackResCnt;
			}*/
		}

	if ( (RTUserPrompt_OneShot & aPromptData->iPermission->Default ()) || (RTUserPrompt_OneShot & aPromptData->iPermission->Condition ()))
		{
		oneShotOptionText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_ONESHOT_YES);
		settingPageText->AppendL (oneShotOptionText->Des ());
		++stackResCnt;

		selIdxArray.Append (R_RTSECMGR_PROMPT_ONESHOT_YES);

		TUserPromptOption def = aPromptData->iPermission->Default ();
		if ( RTUserPrompt_OneShot&def)
			selIndex = selIdxArray.Count()-1;
	/*	if ( !permDenyOption && !sessionDenyOption)
			{
			oneShotDenyText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_ONESHOT_NO);
			++stackResCnt;
			}*/
		}
		
	
	if ( (RTUserPrompt_OneShot & (aPromptData->iPermission->Default ())) || (RTUserPrompt_OneShot & (aPromptData->iPermission->Condition ())))
	        {
	        oneShotDenyText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_ONESHOT_NO);
	        ++stackResCnt;
	            
	        }
	    
    if ( (RTUserPrompt_Session & (aPromptData->iPermission->Default ())) || (RTUserPrompt_Session & (aPromptData->iPermission->Condition ())))
        {
        if ( !oneShotDenyText)
            {
            sessionDenyOption = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_SESSION_NO);
            ++stackResCnt;
            }
        }

    if ( (RTUserPrompt_Permanent & aPromptData->iPermission->Default ()) || (RTUserPrompt_Permanent & aPromptData->iPermission->Condition ()))
        {
        if ( !oneShotDenyText && !sessionDenyOption)
            {
            permDenyOption = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_PERM_NO);
            ++stackResCnt;
            }
        }

	if ( oneShotDenyText)
		{
		settingPageText->AppendL (oneShotDenyText->Des ());
		selIdxArray.Append (R_RTSECMGR_PROMPT_ONESHOT_NO);
		}

	if ( sessionDenyOption)
		{
		settingPageText->AppendL (sessionDenyOption->Des ());
		selIdxArray.Append (R_RTSECMGR_PROMPT_SESSION_NO);
		}

	if ( permDenyOption)
		{
		settingPageText->AppendL (permDenyOption->Des ());
		selIdxArray.Append (R_RTSECMGR_PROMPT_PERM_NO);
		}
	
	HBufC* body(NULL);
	HBufC *moreInfo(NULL);
	
	if(((aPromptData->iPermission->PermName()).Compare(KNullDesC)) != KErrNone)
		{
		if(((aPromptData->iPermission->PermName()).Compare(_L("UserDataGroup"))) == KErrNone )
			{
			body = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_USERDATA_GROUP);
			++stackResCnt;	
			}
		else if(((aPromptData->iPermission->PermName()).Compare(_L("DeviceResourcesGroup"))) == KErrNone )
			{
			body = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_DEVICERESOURCES_GROUP);
			++stackResCnt;	
			}
		else if(((aPromptData->iPermission->PermName()).Compare(_L("NetworkGroup"))) == KErrNone )
			{
			body = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_NETWORK_GROUP);
			++stackResCnt;	
			}
		}
	else if ( (aPromptData->iPermission->Capability()<KCapabilitySize) && (aPromptData->iPermission->Capability()>ECapabilityTCB))
		{
		TInt textResID(CAPABILITY_RESOURCEID[aPromptData->iPermission->Capability()]);
		if(KErrNone!=textResID)
			{
			body = iCoeEnv->AllocReadResourceLC (textResID);
			++stackResCnt;			
			}
		}
	
	if(((aPromptData->iPermission->PermName()).Compare(KNullDesC)) != KErrNone)
		{
		moreInfo = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_ITEM_MORE_INFO);
		++stackResCnt;
		if( moreInfo )
			{
			settingPageText->AppendL (moreInfo->Des ());
			selIdxArray.Append (R_RTSECMGR_ITEM_MORE_INFO);
			}
		}
	
	CAknListQueryDialog* queryDialog = new (ELeave) CAknListQueryDialog(&selIndex);
	CleanupStack::PushL (queryDialog);
	++stackResCnt;

	queryDialog->PrepareLC (R_LIST_QUERY_MSGBOX);
		
	queryDialog->SetItemTextArray (settingPageText); 
	queryDialog->SetOwnershipType(ELbmDoesNotOwnItemArray);

	CAknMessageQueryControl* msgBox = queryDialog->MessageBox ();
	if ( msgBox && body )
		msgBox->SetMessageTextL (body);

	CAknPopupHeadingPane* heading = queryDialog->QueryHeading ();
	HBufC* headerText(NULL);
	if ( heading)
		{
		heading->SetLayout (CAknPopupHeadingPane::EMessageQueryHeadingPane);
		headerText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_QUERY_HEADER);
		heading->SetTextL (headerText->Des ());
		++stackResCnt;
		}
		
	if(moreInfo)
	    {
    	TListItemProperties moreInfoProperties;
    	moreInfoProperties.SetUnderlined(ETrue);
    	queryDialog->ListControl()->Listbox()->ItemDrawer()->SetPropertiesL(settingPageText->Count()-1,moreInfoProperties);
	    }
	
	CleanupStack::Pop (stackResCnt);
	
	queryDialog->ListBox()->SetCurrentItemIndex(selIndex);
	
	TInt queryOk = queryDialog->RunLD ();
	if ( queryOk == EAknSoftkeyOk)
		{
		if ( (selIndex>=0) && (selIndex<selIdxArray.Count()))
			{
			if ( R_RTSECMGR_PROMPT_ONESHOT_YES==selIdxArray[selIndex])
				aPromptData->iUserSelection = RTUserPrompt_OneShot;
			
			else if ( R_RTSECMGR_PROMPT_ONESHOT_NO==selIdxArray[selIndex])
				{
				aPromptData->iUserSelection = RTUserPrompt_Denied;
				ret = EPromptCancel;
				}
			
			else if ( R_RTSECMGR_PROMPT_SESSION_YES==selIdxArray[selIndex])
				aPromptData->iUserSelection = RTUserPrompt_Session;
			
			else if ( R_RTSECMGR_PROMPT_SESSION_NO==selIdxArray[selIndex])
				{
				aPromptData->iUserSelection = RTUserPrompt_SessionDenied;
				ret = EPromptCancel;
				}
				
			else if ( R_RTSECMGR_PROMPT_PERM_YES==selIdxArray[selIndex])
				aPromptData->iUserSelection = RTUserPrompt_Permanent;
			
			else if ( R_RTSECMGR_PROMPT_PERM_NO==selIdxArray[selIndex])
				{
				aPromptData->iUserSelection = RTUserPrompt_PermDenied;
				ret = EPromptCancel;
				}
				
			else if ( R_RTSECMGR_ITEM_MORE_INFO == selIdxArray[selIndex])
				{
				ret = MoreInfoL(*aPromptData);
				}
			}
		}
	else
		{
		ret = EPromptCancel;
		}
	
	if ( headerText)
		delete headerText;
	
	if ( body)
		delete body;

	if ( oneShotOptionText)
		delete oneShotOptionText;

	if ( sessionOptionText)
		delete sessionOptionText;

	if ( permGrantOptionText)
		delete permGrantOptionText;

	if ( oneShotDenyText)
		delete oneShotDenyText;

	if ( sessionDenyOption)
		delete sessionDenyOption;

	if ( permDenyOption)
		delete permDenyOption;
	
	if (moreInfo)
		delete moreInfo;

	CleanupStack::PopAndDestroy ();//settingPageText
	CleanupStack::PopAndDestroy ();//selIdxArray
	return ret;
	}



// ---------------------------------------------------------------------------
// Private default prompt handler implementation for advanced prompt UI
// ---------------------------------------------------------------------------
//
TInt CRTSecMgrScriptSession::PromptAdvanced(CPromptData* aPromptData)
	{
	TInt ret(EPromptOk);
	TInt selIndex(PROMPT_SELIDX_ZERO); //R_RTSECMGR_PROMPT_ONESHOT_YES
	TInt stackResCnt(0);

	HBufC* msgText(NULL);
	HBufC* moreInfo(NULL);
	TBuf<256> LinkText;
	
	if(((aPromptData->iPermission->PermName()).Compare(KNullDesC)) != KErrNone)
		{
		if(((aPromptData->iPermission->PermName()).Compare(_L("UserDataGroup"))) == KErrNone )
			{
			msgText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_USERDATA_GROUP);
			LinkText.Append(msgText->Des());
			++stackResCnt;	
			}
		else if(((aPromptData->iPermission->PermName()).Compare(_L("DeviceResourcesGroup"))) == KErrNone )
			{
			msgText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_DEVICERESOURCES_GROUP);
			LinkText.Append(msgText->Des());
			++stackResCnt;	
			}
		else if(((aPromptData->iPermission->PermName()).Compare(_L("NetworkGroup"))) == KErrNone )
			{
			msgText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_NETWORK_GROUP);
			LinkText.Append(msgText->Des());
			++stackResCnt;	
			}
		}
	else if ((aPromptData->iPermission->Capability()<KCapabilitySize) && (aPromptData->iPermission->Capability()>ECapabilityTCB))
		{
		TInt textResID(CAPABILITY_RESOURCEID[aPromptData->iPermission->Capability()]);
		if(KErrNone!=textResID)
			{
			msgText = iCoeEnv->AllocReadResourceLC (textResID);
			++stackResCnt;
			LinkText.Append(msgText->Des());
			}
		}
	
	if(((aPromptData->iPermission->PermName()).Compare(KNullDesC)) != KErrNone)
		{
		moreInfo = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_LINK_MORE_INFO);
		++stackResCnt;
		
		LinkText.Append(_L("\n\n"));
		LinkText.Append(_L("<AknMessageQuery Link>"));
		LinkText.Append(moreInfo->Des());
		LinkText.Append(_L("</AknMessageQuery Link>"));
		}
		
	CAknMessageQueryDialog* queryDialog = new(ELeave) CAknMessageQueryDialog();
	CleanupStack::PushL (queryDialog);
	++stackResCnt;

	queryDialog->PrepareLC (R_ADVPROMPT_MESSAGE_QUERY);
	
	queryDialog->SetMessageTextL(LinkText);
	HBufC* headerText(NULL);
	CAknPopupHeadingPane* heading = queryDialog->QueryHeading ();
	if ( heading)
		{
		heading->SetLayout (CAknPopupHeadingPane::EMessageQueryHeadingPane);
		headerText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_QUERY_HEADER);
		heading->SetTextL (headerText->Des ());
		++stackResCnt;
		}

	if(moreInfo)
		{
		TCallBack callback;
		TCallbackParam callbackParam;
				
		callbackParam.iPromptData = aPromptData;
		callbackParam.iRTSecMgrScriptSession = this;
		callback.iFunction = LinkCallback;
		callback.iPtr = &callbackParam;
		
		queryDialog->SetLink(callback);
		}
	
	CleanupStack::Pop (stackResCnt);
	TInt queryOk = queryDialog->RunLD ();
	if ( queryOk == EAknSoftkeyYes)
		{
		if ( selIndex==0)
			{
			if ( aPromptData->iPermission->Default ()& RTUserPrompt_OneShot)
				{
				aPromptData->SetUserSelection(RTUserPrompt_OneShot);
				}
			else
				if ( aPromptData->iPermission->Default ()& RTUserPrompt_Session)
					{
					aPromptData->SetUserSelection(RTUserPrompt_Session);
					}
				else
					if ( aPromptData->iPermission->Default ()& RTUserPrompt_Permanent)
						{
						aPromptData->SetUserSelection(RTUserPrompt_Permanent);
						}
			}
		else
			{
			aPromptData->SetUserSelection(RTUserPrompt_Denied);
			}
		}
	else
		{
		ret = EPromptCancel;
		}
	
	if ( headerText)
		delete headerText;

	if ( msgText)
		delete msgText;
	
	if ( moreInfo)
	    delete moreInfo;
	
	return ret;
	}
// ---------------------------------------------------------------------------
// Pops up a new query dialog when the user clicks on more info.
// ---------------------------------------------------------------------------
//
TInt CRTSecMgrScriptSession::MoreInfoL(CPromptData& aPromptData)
	{
	TInt ret(EPromptOk);
	TInt stackResCnt = 0;
	HBufC *msgText = NULL;
	HBufC *body = NULL;
	TBuf<KMaxBuf> message;
	RCapabilityArray Caps;
	
	msgText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_MORE_INFO_QUERY);
	++stackResCnt;
	message.Append(msgText->Des());
	
	aPromptData.Permission()->Capabilitilites(Caps);
	for(TInt i = 0; i != Caps.Count(); ++i)
        {
        TInt textResID(MOREINFO_CAP_RESOURCEID[Caps[i]]);
        body = iCoeEnv->AllocReadResourceLC (textResID);
        message.Append(_L("\n"));
        message.Append(body->Des());
        if(body)
            CleanupStack :: PopAndDestroy(body);
        }
	Caps.Close();
	
	CAknMessageQueryDialog* queryDialog = new(ELeave) CAknMessageQueryDialog();
	CleanupStack::PushL (queryDialog);
	++stackResCnt;

	queryDialog->PrepareLC (R_MOREINFO_MESSAGE_QUERY);
	
	queryDialog->SetMessageTextL(message);
	
	HBufC* headerText(NULL);
	CAknPopupHeadingPane* heading = queryDialog->QueryHeading ();
	if ( heading)
		{
		heading->SetLayout (CAknPopupHeadingPane::EMessageQueryHeadingPane);
		
		if(((aPromptData.iPermission->PermName()).Compare(_L("UserDataGroup"))) == KErrNone )
	    	{
		    headerText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_MORE_INFO_HEADER_USERDATA);
    		}
    	else if(((aPromptData.iPermission->PermName()).Compare(_L("DeviceResourcesGroup"))) == KErrNone )
    		{
    		headerText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_MORE_INFO_HEADER_DEVICERESOURCES);
    		}
    	else if(((aPromptData.iPermission->PermName()).Compare(_L("NetworkGroup"))) == KErrNone )
    		{
    		headerText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_MORE_INFO_HEADER_NETWORK);
    		}
		
	//	headerText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_QUERY_HEADER);
		heading->SetTextL (headerText->Des ());
		++stackResCnt;
		}
		
	CleanupStack::Pop(stackResCnt);
	
	TInt queryOk = queryDialog->RunLD ();
	
	if(headerText)
		delete headerText;
	
	if(msgText)
	    delete msgText;
	
	if(iUIPromptOption!=RTPROMPTUI_ADVANCED)
		ret = Prompt(&aPromptData);
	
	return ret;
	}


// ---------------------------------------------------------------------------
// Pops up a new query dialog when the user clicks on more info.
// ---------------------------------------------------------------------------
//
void CRTSecMgrScriptSession::PromptCostL()
	{
	AddResourceFiles();
	TInt stackResCnt = 0;
	HBufC *msgText = NULL;
	TBuf<KMaxBuf> message;
	RCapabilityArray Caps;
	
	
	msgText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_COST_INFO);
	++stackResCnt;
	message.Append(msgText->Des());
	
	CAknMessageQueryDialog* queryDialog = new(ELeave) CAknMessageQueryDialog();
	CleanupStack::PushL (queryDialog);
	++stackResCnt;

	queryDialog->PrepareLC (R_MOREINFO_MESSAGE_QUERY);
	
	queryDialog->SetMessageTextL(message);
	
	HBufC* headerText(NULL);
	CAknPopupHeadingPane* heading = queryDialog->QueryHeading ();
	if ( heading)
		{
		heading->SetLayout (CAknPopupHeadingPane::EMessageQueryHeadingPane);
		headerText = iCoeEnv->AllocReadResourceLC (R_RTSECMGR_PROMPT_QUERY_HEADER);
		heading->SetTextL (headerText->Des ());
		++stackResCnt;
		}
		
	CleanupStack::Pop(stackResCnt);
	
	TInt queryOk = queryDialog->RunLD ();
	
	if(headerText)
		delete headerText;
	
	if(msgText)
	    delete msgText;
	CloseResourceFiles();
	}
// ---------------------------------------------------------------------------
// Handles the prompt option chosen by the user through default prompt UI
// ---------------------------------------------------------------------------
//
void CRTSecMgrScriptSession::HandleGrantChosen(CPromptData* aPromptData,
		TCapabilityBitSet aCapBitSet, TCapabilityBitSet& aAllowedCaps,
		TBool& aIsPermGrantModified)
	{
	TUserPromptOption optionChosen = aPromptData->iUserSelection;
	
	if ( RTUserPrompt_OneShot==optionChosen)
		{
		_sessionInfo.AllowedCaps &= ~aCapBitSet;
		aAllowedCaps = aCapBitSet;
		}
	else
		if ( RTUserPrompt_Denied==optionChosen) //one-shot denied
			{
			aAllowedCaps &= ~aCapBitSet;
			_sessionInfo.AllowedCaps &= ~aCapBitSet; //disable in session pattern too...	
			}
		else
			if ( RTUserPrompt_Session==optionChosen)
				{
				_sessionInfo.AllowedCaps |= aCapBitSet;//Enable in _sessionInfo.Allowed & allowed
				aAllowedCaps = aCapBitSet;
				}
			else
				if ( RTUserPrompt_SessionDenied==optionChosen) //session denied
					{
					aAllowedCaps &= ~aCapBitSet;
					_sessionInfo.AllowedCaps &= ~aCapBitSet; //disable in session pattern too...
					_sessionInfo.DeniedCaps |= aCapBitSet; //enable denied in session bit
					}
				else
					if ( RTUserPrompt_Permanent==optionChosen)
						{
						aIsPermGrantModified = ETrue; //to commit the change to persistent store
						_sessionInfo.AllowedCaps |= aCapBitSet;
						aAllowedCaps = aCapBitSet;
						iPermBits |= aCapBitSet;
						iDeniedBits &= ~aCapBitSet; //just in case....
						}
					else
						if ( RTUserPrompt_PermDenied==optionChosen) //permanent denied
							{
							aIsPermGrantModified = ETrue;
							aAllowedCaps &= ~aCapBitSet;
							_sessionInfo.AllowedCaps &= ~aCapBitSet; //disable in session pattern too...
							_sessionInfo.DeniedCaps |= aCapBitSet; //enable denied in session bit 
							iPermBits &= ~aCapBitSet; //disable in perm bits
							iDeniedBits |= aCapBitSet; //enable in perm denied bit pattern
							//Commit the change to persistent store...
							}
	}

// ---------------------------------------------------------------------------
// Updates blanket permission data
// ---------------------------------------------------------------------------
//
void CRTSecMgrScriptSession::UpdatePermGrant()
	{
	(*iSubSessionProxy)->UpdatePermGrant (iScript->ScriptID (), iPermBits,
			iDeniedBits);
	}

//
// Conversion utility to convert a single 32-bit value to the list of
// capabilities (RArray<TCapability>)
// 
void CRTSecMgrScriptSession::BuildCapsL(TCapabilityBitSet aCapBitSet,
		RCapabilityArray& aInOutTCapList)
	{
	TInt idx(KErrNotFound);

	while (++idx!=KCapabilitySize)
		{
		TCapabilityBitSet temp(KDefaultEnableBit);
		temp = temp << idx;
		if ( aCapBitSet & temp)
			aInOutTCapList.AppendL ((TCapability)(idx+1));
		}
	}

//
// Conversion utility to generate a single 32-bit value from a list of
// capabilities (RArray<TCapability>)
//
void CRTSecMgrScriptSession::AddCapability(TCapabilityBitSet& aInOutCapBitSet,
		TCapability aCapToSet)
	{
	if ( aCapToSet>=0)
		aInOutCapBitSet = aInOutCapBitSet | (LOWBIT << aCapToSet);
	}

/*
 * Closes the script sub-session
 * 
 */
void CRTSecMgrScriptSession::Close()
	{
	if ( iResourceOffset && iCoeEnv)
		iCoeEnv->DeleteResourceFile (iResourceOffset);

	if ( iScript)
		{
		delete iScript;
		iScript = NULL;
		}

	if ( iSubSessionProxy)
		{
		delete iSubSessionProxy;
		iSubSessionProxy = NULL;
		}
	}

//--------------------------------------------------------------------------- 
// Callback function called from moreinfo link in advanced prompt
//---------------------------------------------------------------------------
TInt CRTSecMgrScriptSession::LinkCallback(TAny * aCallbackParam)
	{
	TCallbackParam *cb = (TCallbackParam*)aCallbackParam;
	(cb->iRTSecMgrScriptSession)->MoreInfoL(*(cb->iPromptData));
	return KErrNone;
	}
