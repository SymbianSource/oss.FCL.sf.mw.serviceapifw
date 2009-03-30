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
* Description:       Utility XML parser to parse trust and access policy files
 *
*/






#ifndef C_RTSECMGRPOLICYPARSER_H
#define C_RTSECMGRPOLICYPARSER_H

#include <e32base.h>
#include <GMXMLParser.h>
#include <GMXMLNode.h>
#include <EscapeUtils.h>
#include "rtsecmgrdata.h"
#include "rtsecmgrcommondef.h"

/*
 * Enumerations for policy parsing status codes
 * 
 */
enum TParserErrCode
{
	EErrNone = 0, //zero for success , equivalent to KErrNone of symbian
	EErrInvalidDocNode = KErrXMLBase,
	EErrInvalidRootNode = EErrInvalidDocNode - 1,
	EErrNoPolicyInfo   = EErrInvalidRootNode - 1,
	EErrInvalidDomainName = EErrNoPolicyInfo - 1,
	EErrRepeatedDomainTag = EErrInvalidDomainName - 1,
	EErrJunkContent = EErrRepeatedDomainTag - 1,
	EErrNoDefaultCondition = EErrJunkContent - 1,
	EErrInvalidFormat = EErrNoDefaultCondition - 1,
	EErrInvalidCapability = EErrInvalidFormat - 1,
	EErrInvalidPermission = EErrInvalidCapability - 1,
	EErrRepeatedCaps = EErrInvalidPermission - 1,
	EErrRepeatedAliasTag = EErrRepeatedCaps - 1,
	EErrInvalidAliasName = EErrRepeatedAliasTag - 1,
	EErrMisplacedAlias = EErrInvalidAliasName
};

/*
 * Abstracts policy parsing logic.
 * 
 * Security access policy and trust policy are represented in
 * XML format. CPolicyParser uses native XML parser to parse
 * the policy files.
 * 
 *  
 * @see MMDXMLParserObserver
 * @see CMDXMLParser
 * @see CMDXMLElement
 * @see CProtectionDomain
 * @see CTrustInfo
 * @see TUserPromptOption
 * 
 * @exe rtsecmgrserver.exe
 */
NONSHARABLE_CLASS(CPolicyParser) : public CBase, public MMDXMLParserObserver
{
public:
	
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPolicyParser instance
	 * 
	 * @return CPolicyParser* pointer to an instance of CPolicyParser
	 */
	static CPolicyParser* NewL();

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CPolicyParser instance and leaves the created instance
	 * on the cleanupstack
	 * 
	 * @return CPolicyParser* pointer to an instance of CPolicyParser
	 */
	static CPolicyParser* NewLC();	


	/**
	 * Destructor
	 * 
	 */
	~CPolicyParser();
	
	/**
	 * Parses the security access policy file and populates
	 * the input protection domain array
	 * 
	 * @param aSecPolicy RFile& Handle to security policy file
	 * @param aPolicyInfo RProtectionDomains& input array of protection domains
	 * 
	 * @return EErrSuccess if there is no parsing error; Otherwise one of
	 * error codes defined in TParserErrCode
	 * 
	 * @see TParserErrCode
	 */
	TInt GetPolicyInfo(RFile& aSecPolicy, RProtectionDomains& aPolicyInfo, RAliasGroup& aAliasGroup);	
	
	/**
	 * Callback offered by MMDXMLParserObserver to notify the calling client that the XML 
	 * parsing is completed and DOM data structure is available for 
	 * navigation
	 * 
	 * @see MMDXMLParserObserver
	 * 
	 * This identifies whether the policy file is in the proper format
	 * if not in proper format, parsing errors as defined in TParserErrCode are thrown
	 * @see TParserErrCode
	 */
	void ParseFileCompleteL();	
private:	
	
	//private default constructor
	inline CPolicyParser();	
	
	/**
	 * Gets the list of capability nodes provided the parent domain
	 * node
	 * 
	 */
	TInt GetCapsNode(CMDXMLElement* aParentDomainNode, 
					 RPointerArray<CMDXMLElement>& aCapNodes, 
					 const TDesC& aNodeName);
	
	/**
	 * Overloaded version to get the list of capability nodes provided the parent domain
	 * node
	 * 
	 */
	CMDXMLElement* GetCapsNode(CMDXMLElement* aParentDomainNode, const TDesC& aNodeName);
	

	/**
	 * Populates the permission set data structure with capability
	 * information provided the parent capability node
	 * 
	 * @return EErrSuccess if there is no parsing error; Otherwise one of these error codes 
	 * @return EErrRepeatedCaps if there are capabilities specified more than once in same domain
	 * @return EErrInvalidCapability if the capability string is invalid
	 * 
	 * @see TParserErrCode
	 * 
	 */
	TInt GetCapabilities(CMDXMLElement* aParentNode, 
						 CPermissionSet& aCapInfo,
				   	 	 TUserPromptOption aUpOpt=RTUserPrompt_UnDefined,
				   	 	 TBool aUGCaps=EFalse,
						 TUserPromptOption aDefUpOpt=RTUserPrompt_UnDefined);
						 
	/**
	 * Gets the user prompt option provided the parent user node
	 * 
	 * @return EErrSuccess if there is no parsing error; Otherwise one of these error codes 
	 * @return EErrInvalidPermission if prompt sessions specified are invalid
	 */
	TInt GetConditions(CMDXMLElement* aParentNode, TUserPromptOption& aUserPromptOpt);
	
	/**
	 * Gets the default user prompt option provided the parent user node
	 * 
	 */
	TUserPromptOption GetDefaultCondition(CMDXMLElement* aParentNode);
	
	/**
	 * Utility to convert the stringified user prompt option text to
	 * TCapability enumeration
	 * 
	 * @see TCapability
	 * 
	 */
	TCapability GetCapability(const TDesC& aUserPromptOpt);
	
	/**
	 * Utility to convert the stringified user prompt option to
	 * TUserPromptOption structure
	 * 
	 */
	TUserPromptOption GetUserPromptOption(const TDesC& aUserPromptOpt);		
	
	/**
	 * Finds out if a domain node is present with the passed domain name
	 * 
	 */
	TBool isDomainPresent(const TDesC& aDomainName);
	
	/**
	 * Utility method to check if a capability is already part of 
	 * unconditional permissions
	 * 
	 */
	TBool IsPresent(const CPermissionSet& aCapInfo, TCapability aCap);
	
	/**
	 * Utility method to check if a capability is already part of user
	 * grantable permissions
	 * 
	 */
	TBool IsUserGrantPresent(const CPermissionSet& aCapInfo, TCapability aCap);

	
	/**
	 * Populates the permission set data structure with capability
	 * information provided the parent capability node
	 * 
	 * @return EErrSuccess if there is no parsing error; Otherwise one of these error codes 
	 * @return EErrRepeatedCaps if there are capabilities specified more than once in same domain
	 * @return EErrInvalidCapability if the capability string is invalid
	 * 
	 * @see TParserErrCode
	 * 
	 * This is similar to GetCapabilities method except that this retrives the capability information for the Group
	 */
	TInt GetCapabilitiesForGroup(CMDXMLElement* aParentNode, 
						 CPermissionSet& aCapInfo,
						 CPermission& aAliasInfo,
				   	 	 TUserPromptOption aUpOpt=RTUserPrompt_UnDefined,
				   	 	 TBool aUGCaps=EFalse,
						 TUserPromptOption aDefUpOpt=RTUserPrompt_UnDefined);
						 
	/**
	 * Finds out if the passed capability is present in alias group
	 * 
	 */
	TBool IsCapsAliasPresent(const CPermission& aAliasInfo, TCapability aCap);
	
	/**
	 * Finds out if a domain node is present with the passed domain name
	 * 
	 */
	TBool isAliasPresent(const TDesC& aAliasName);
	
		
private:	
	
	//reference to undelying native symbian XML parser
	CMDXMLParser* iDomParser;	
	
	//Completion status of parsing request made to XML parser.
	TRequestStatus* iStatus;
	CActiveSchedulerWait iWaitScheduler;
	
	//list of domains declared in the security access policy
	RProtectionDomains iPolicyInfo;
	
	//list of trust information specified in the trust policy file
	RTrustArray iTrustInfo;

	//variable to initiate parsing for trust policy file
	TBool isTrustInfo;
	
	//status code to store last parsing error
	TInt iLastError;

	
	//Array of TPermissions - used for Capability Grouping
	RAliasGroup iAliasGroup;

};

#endif  //C_RTSECMGRPOLICYPARSER_H

// End of file


