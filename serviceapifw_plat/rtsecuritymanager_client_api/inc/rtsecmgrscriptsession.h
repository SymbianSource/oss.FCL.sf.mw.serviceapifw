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
* Description:       Client side proxy class representing script session
 *
*/







#ifndef _CRTSECMGRSCRIPTSESSION_H_
#define	_CRTSECMGRSCRIPTSESSION_H_

#include <rtsecmanager.rsg>
#include <rtsecmgrcommondef.h>
#include <rtsecmgrpermission.h>
#include <rtsecmgrscript.h>
#include <rtsecmgrtrustinfo.h>

class CCoeEnv;
class CRTSecMgrSubSessionProxy;

/**
 * Type holding data to be used while prompting. 
 * 
 * The default or custom (runtime over-ridden) prompt handler requires the permission
 * type to prompt and the user-selection to be returned to security 
 * manager for access permission check
 * 
 * @lib rtsecmgrclient.lib
 */
class CPromptData : public CBase
	{
public:

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPromptData instance
	 *
	 * @return pointer to an instance of CPromptData
	 */
	static CPromptData* NewL()
		{
		CPromptData* self = CPromptData::NewLC ();
		CleanupStack::Pop (self);
		return self;
		}

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPromptData instance and leaves the created instance
	 * onto the cleanupstack.
	 *
	 * @return pointer to an instance of CPromptData
	 */
	
	static CPromptData* NewLC()
		{
		CPromptData* self = new (ELeave) CPromptData();
		CleanupStack::PushL (self);
		self->ConstructL ();
		return self;
		}
		

	/**
	 * Destructor
	 * 
	 */
	~CPromptData()
		{
		if(iPermission)
			delete iPermission;
		
		}

	
	/*
	 *
	 *
	 */
	void ConstructL()
	{
		
	}
	
	/**
	 * Returns permission data
	 * 
	 * @return an object containing the permission data
	 */
	inline CPermission* Permission() const
		{
		return iPermission;
		}

	/**
	 * Sets the user-selection data
	 * 
	 * @param aUserSelection TUserPromptOption user-selected data value
	 */
	inline void SetUserSelection(TUserPromptOption aUserSelection)
		{
		iUserSelection = aUserSelection;
		}
		
	/**
	 * Sets the permission value of the domain
	 * 
	 * @param aPermission CPermission permission value of the domain
	 */
	 
	inline void SetPermissions(CPermission& aPermission)
		{	
		 if(iPermission)
		 {
		 	delete iPermission;
		 	iPermission = NULL;
		 }
		 
		 iPermission = CPermission::NewL(aPermission);
 		//iPermission = &aPermission;
		}
private:
	//private default constructor
	CPromptData::CPromptData() :
		iUserSelection(RTUserPrompt_UnDefined)
		{
		}

	//private permission data
	CPermission* iPermission;

	//private user-selection
	TUserPromptOption iUserSelection;

	friend class CRTSecMgrScriptSession;
	};

//typdef to model list of prompt data structure
typedef RPointerArray<CPromptData> RPromptDataList;

/* 
 * Prompt callback handler class. 
 * 
 * Runtimes should implement the prompt handler function to prompt the user 
 * and to obtain the user option chosen. The prompt data are used by
 * runtime security manager for further access permission check. 
 * 
 * If runtimes do not override prompt handling, security manager component 
 * would then provide default prompt handler functionality.
 *
 * @lib rtsecmgrclient.lib
 */
class MSecMgrPromptHandler
	{
public:
	/**
	 * Prompts the user.
	 * 
	 * @param aPromptDataList RPromptDataList list of prompt data used by
	 * prompt callback handler to show to the user
	 * 
	 * @return KErrNone if prompting successful; In case of failure, one of
	 * system wide error codes
	 */
	virtual TInt Prompt(RPromptDataList& aPromptDataList , TExecutableID aExecID  = KAnonymousScript) =0;

	/**
	 * Sets the user chosen prompt selection.
	 * 
	 * @param aPromptUiOption TSecMgrPromptUIOption prompt selection
	 */
	virtual void SetPromptOption(TSecMgrPromptUIOption aPromptUiOption) =0;
	/**
	 * Returns the prompt selection
	 * 
	 * @return TSecMgrPromptUIOption the prompt selection
	 */
	virtual TSecMgrPromptUIOption PromptOption() const =0;
	};

/**
 *
 * Thin proxy to the client side sub-session handle. 
 * 
 * This class implements the default prompt handling functionality. In addition, the main
 * functionality of native platform access check is performed by CRTSecMgrScriptSession.
 *
 * @lib rtsecmgrclient.lib
 * 
 */
NONSHARABLE_CLASS(CRTSecMgrScriptSession) : public CBase, MSecMgrPromptHandler
	{
public:
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CRTSecMgrScriptSession instance
	 *
	 * Initializes client side script sub-session. 
	 * 
	 * @param MSecMgrPromptHandler pointer to a prompt handler callback
	 * 
	 * @return pointer to an instance of CRTSecMgrScriptSession if succesful; 
	 * Otherwise NULL
	 */
	static CRTSecMgrScriptSession* NewL(MSecMgrPromptHandler* aPromptHdlr=0);

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CRTSecMgrScriptSession instance and leaves the created instance
	 * on the cleanupstack
	 *
	 * Initializes client side script sub-session. 
	 * 
	 * @param MSecMgrPromptHandler pointer to a prompt handler callback
	 * 
	 * @return pointer to an instance of CRTSecMgrScriptSession if succesful; 
	 * Otherwise NULL
	 */
	static CRTSecMgrScriptSession* NewLC(MSecMgrPromptHandler* aPromptHdlr=0);
	
	/**
	 * Callback funciton for moreinfo link in advanced prompt
	 * 
	 * Calls the moreinfo function for indivudual capability or for a alias group
	 * 
	 * @param aCallbackParam TAny* pointer to TCallbackParam
	 * 
	 */
  static TInt LinkCallback(TAny *aCallbackParam);

	/**
	 * Destructor
	 * 
	 * Closes the sub-session
	 *
	 */
	IMPORT_C ~CRTSecMgrScriptSession();

	/**
	 * Opens security manager script sub-session. This method in turn opens a
	 * sub-session on the server side, by bringing script related data onto the cache. 
	 * 
	 * @param aParentSession RSessionBase handle to parent security manager session
	 * @param aPolicyID TPolicyID policy identifier of the script associated with this script session
	 * @param aExecID TExecutableID identifier of the script associated with this script session
	 * 
	 * @return KErrNone if successful; In case of failure, one of system wide error cods
	 */
	TInt Open(const RSessionBase& aParentSession,TPolicyID aPolicyID,TExecutableID aExecID);

	/**
	 * Opens security manager script sub-session. This method in turn opens a
	 * sub-session on the server side, by bringing script related data onto the cache. 
	 * 
	 * @param aParentSession RSessionBase handle to parent security manager session
	 * @param aPolicyID TPolicyID policy identifier of the script associated with this script session
	 * @param aExecID TExecutableID identifier of the script associated with this script session
	 * @param aHashValue TDesC		hash value of the scrpt passed while registering the script
	 * 
	 * @return KErrNone if successful; In case of failure, one of system wide error cods
	 */
	TInt Open(const RSessionBase& aParentSession,TPolicyID aPolicyID,TExecutableID aExecID,const TDesC& aHashValue);

	/**
	 * Opens security manager script sub-session. This method in turn opens a
	 * sub-session on the server side, by bringing script related data onto the cache. 
	 * 
	 * @param aParentSession RSessionBase handle to parent security manager session
	 * @param aPolicyID TPolicyID policy identifier of the script associated with this script session
	 * @param aExecID TExecutableID identifier of the script associated with this script session
	 * @param aTrustInfo CTrustInfo trust data structure
	 * 
	 * @return KErrNone if successful; In case of failure, one of system wide error cods
	 */
	TInt Open(const RSessionBase& aParentSession,TPolicyID aPolicyID,TExecutableID aExecID,const CTrustInfo& aTrustInfo);

	/**
	 * Performs access permission check
	 * 
	 * This method evaluates access permission by comparing the expected capabilities
	 * to perform service invocation with the capabilities of the script. The
	 * capabilities of the script is computed as sum of :
	 * 	
	 * 	-	default allowed capabilities as specified in security access policy
	 * 	-	user-granted capabilities, allowed by user while prompting
	 *
	 * The capabilities allowed by the user could be of various durations, like
	 * session-based, blanket/permanent and the one valid for the current invocation only 
	 * (one-shot)
	 * 
	 * @param aCapabilitiesToCheck RCapabilityArray list of capabilities to be checked against
	 * script's capbilities
	 * 
	 * @return EAccessOk if the access permission check is successful; Else, EAccessNOk
	 */
	IMPORT_C TInt IsAllowed(const RCapabilityArray& aCapabilitiesToCheck);

	/**
	 * Performs access permission check
	 * 
	 * This overloaded method evaluates access permission by comparing the expected capabilities
	 * to perform service invocation with the capabilities of the script. The
	 * capabilities of the script is computed as sum of :
	 * 	
	 * 	-	default allowed capabilities as specified in security access policy
	 * 	-	user-granted capabilities, allowed by user while prompting
	 *
	 * The capabilities allowed by the user could be of various durations, like
	 * session-based, blanket/permanent and the one valid for the current invocation only 
	 * (one-shot)
	 * 
	 * This overloaded version returns the list of capabilities that do not match after
	 * access permission check. This can be used by the client to display to the user the
	 * list of unmatched capabilities
	 * 
	 * @param aCapabilitiesToCheck RCapabilityArray list of capabilities to be checked against
	 * script's capbilities
	 * @param aUnMatchedCapabilities RCapabilityArray list of unmatched capabilities
	 * 
	 * @return EAccessOk if the access permission check is successful; Else, EAccessNOk
	 */
	IMPORT_C TInt IsAllowed(const RCapabilityArray& aCapabilitiesToCheck,RCapabilityArray& aUnMatchedCapabilities);

	/**
	 * Definition of default prompt handler.
	 * 
	 * @param aPromptDataList RPromptDataList list of prompt data used by
	 * prompt callback handler to show to the user
	 * 
	 * @return KErrNone if prompting successful; In case of failure, one of
	 * system wide error codes
	 *
	 */
	TInt Prompt(RPromptDataList& aPromptDataList , TExecutableID aExecID = KAnonymousScript);
    
    /**
	 * Definition of cost notification.
	 * 
	 */
    void PromptCostL() ; 
    
	/**
	 * Gets the underlying script object
	 * 
	 * This method is to be used by components, such as application manager,
	 * that are interested in displaying script related information to the user.
	 * 
	 * Following are the script related information:
	 * 
	 * 	- Capabilities allowed by default
	 * 	- User-grantable capabilities
	 * 	- Currently allowed or denied blanket permissions
	 *
	 * Note that this method should not be used by runtimes unless and until there is
	 * a strong design justification
	 * 
	 * @return a reference to underlying script object
	 */
	inline CScript& CRTSecMgrScriptSession::Script()
		{
		return *iScript;
		}

	/**
	 * Returns prompt handler reference
	 * 
	 * @return reference to prompt handler
	 */
	inline MSecMgrPromptHandler* PromptHandler() const
		{
		return iPromptHdlr;
		}

	/**
	 * Sets prompt UI option. The supported prompt options are :
	 * 
	 * 	-	Basic/Default
	 * 	- 	Advanced
	 * 
	 * The difference between the basic and advanced prompt option
	 * reisdes in the fact the number of prompt options and the corresponding
	 * prompt texts displayed would be different.
	 * 
	 * If not explictly, the default prompt UI option is set to basic/default prompt UI.
	 * 
	 * @param aUIPromptOption TSecMgrPromptUIOption basic/advanced prompt UI option
	 */
	inline void SetPromptOption(TSecMgrPromptUIOption aUIPromptOption)
		{
		iUIPromptOption = aUIPromptOption;
		}

	/**
	 * Gets prompt UI option. The supported prompt options are :
	 * 
	 * 	-	Basic/Default
	 * 	- 	Advanced
	 * 
	 * The difference between the basic and advanced prompt option
	 * reisdes in the fact the number of prompt options and the corresponding
	 * prompt texts displayed would be different.
	 * 
	 * @return aUIPromptOption TSecMgrPromptUIOption basic/advanced prompt UI option
	 */
	inline TSecMgrPromptUIOption PromptOption() const
		{
		return iUIPromptOption;
		}

private:
	/*
	 * Default private constructor
	 * 
	 */
	CRTSecMgrScriptSession(MSecMgrPromptHandler* aPromptHdlr=0);

	/*
	 * Two phased constructor
	 * 
	 */
	void ConstructL();

	/*
	 * Contains prompt logic
	 * 
	 */
	TInt Prompt(CPromptData* aPromptData);
	
	/*
	 * Private default implementation for advanced prompt UI option
	 * 
	 */
	TInt PromptAdvanced(CPromptData* aPromptData);

	/* 
	 * Logic for displaying more inormation when the user selects more info
	 * 
	 */
	TInt MoreInfoL(CPromptData& aPromptData);
	
	/*
	 * Logic identifying the user-selection of prompt
	 * duration
	 * 
	 */
	void HandleGrantChosen(CPromptData* aPromptData,
		TCapabilityBitSet aCapBitSet, TCapabilityBitSet& aAllowedCaps,
		TBool& aIsPermGrantModified);

	/*
	 * Private implementation to update blanket permission data
	 * 
	 */
	void UpdatePermGrant();
	
	/*
	 * Conversion utility to convert a single 32-bit value to the list of
	 * capabilities (RArray<TCapability>)
	 * 
	 */
	void BuildCapsL(TCapabilityBitSet aCapBitSet, RCapabilityArray& aInOutTCapList);

	/*
	 * Private implementation to evaluate access permission check
	 * 
	 */
	TInt IsAllowed(const RCapabilityArray& aCapsToCheck,RPromptDataList& aPromptDataList,TCapabilityBitSet& aUnMatchedCapBits);

	/*
	 * Private implementation to evaluate access permission check. This
	 * overloaded version additionally returns the unmatched capabilities
	 * 
	 */
	TInt IsAllowed(const RCapabilityArray& aCapsToCheck,TCapabilityBitSet& aUnMatchedCapBits);

	/*
	 * Conversion utility to generate an unsigned 32-bit value toggling the individual bits
	 * to the corresponding TCapability value
	 * 
	 */
	void AddCapability(TCapabilityBitSet& aInOutCapBitSet, TCapability aCapToSet);

	/*
	 * Attempts to close the script sub-session
	 * 
	 */
	void Close();

	void AddResourceFiles();
	void CloseResourceFiles();

private:
	/*
	 * permissions allowed or denied for the current session
	 * The lifetime of this data structure is associated
	 * with this script session
	 * 
	 */
	struct
		{
		TCapabilityBitSet AllowedCaps; //caps allowed for this session (caps allowed for this session & perm allowed)
		TCapabilityBitSet DeniedCaps; //caps denied for this sesion (caps denied for this session & perm denied)
		}_sessionInfo;

	/*
	 * anonymous enumerations for selection index
	 * 
	 */
	enum
		{
		PROMPT_SELIDX_ZERO = 0,
		PROMPT_SELIDX_ONE = PROMPT_SELIDX_ZERO + 1,
		PROMPT_SELIDX_TWO = PROMPT_SELIDX_ZERO + 2,
		PROMPT_SELIDX_THREE = PROMPT_SELIDX_ZERO + 3,
		PROMPT_SELIDX_FOUR = PROMPT_SELIDX_ZERO + 4,
		PROMPT_SELIDX_FIVE = PROMPT_SELIDX_ZERO + 5
		};

	/*
	 * private script reference
	 * 
	 */
	CScript* iScript;

	/*
	 * permanently allowed capability bits
	 * 
	 */
	TPermGrant iPermBits; //perm allowed caps, persistently stored for this script

	/*
	 * permanently denied capability bits
	 * 
	 */
	TPermGrant iDeniedBits; //perm denied caps, persistently stored for this script

	/*
	 * reference to prompt handler instance
	 * 
	 */
	MSecMgrPromptHandler* iPromptHdlr;

	/*
	 * sub-session proxy pointer
	 * 
	 */
	CRTSecMgrSubSessionProxy* iSubSessionProxy;

	/*
	 * pointer to Coe environment
	 */
	CCoeEnv* iCoeEnv;

	/*
	 * security manager resource file offset value
	 * 
	 */
	TInt iResourceOffset;

	/*
	 * Prompt UI option
	 * 
	 */
	TSecMgrPromptUIOption iUIPromptOption;
	
	/*
	 * Custom prompt flag
	 * 
	 */
	TBool isCustomPrompt;
	};

#endif //_CRTSECMGRSCRIPTSESSION_H_

