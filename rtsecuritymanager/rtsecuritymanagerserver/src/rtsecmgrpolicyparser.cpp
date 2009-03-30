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






#include <e32std.h>
#include <w32std.h>
#include <f32file.h>

#include "rtsecmgrpolicyparser.h"
#include "rtsecmgrdata.h"
#include "rtsecmgrserverdef.h"

#include <gmxmldocument.h>
#include <gmxmlelement.h>
#include <gmxmlcharacterdata.h>
#include <gmxmltext.h>
#include <gmxmlnode.h>

_LIT (KPolicy, "policy");
_LIT (KAllow, "allow");
_LIT (KUser, "user");
_LIT (KDomain, "domain");
_LIT (KNameAttr, "name");
_LIT (KCondition, "condition");
_LIT (KCapabilities, "capabilities");
_LIT (KDefCondition, "defaultCondition");
_LIT (KText, "#TEXT");
_LIT (KComment, "comment");
_LIT (KAlias, "alias");

CPolicyParser::CPolicyParser()
	{
	}

CPolicyParser* CPolicyParser::NewL()
	{
	CPolicyParser* self = CPolicyParser::NewLC ();
	//nothing to construct as of now
	CleanupStack::Pop (self);
	return self;
	}

CPolicyParser* CPolicyParser::NewLC()
	{
	CPolicyParser* self = new (ELeave) CPolicyParser();
	CleanupStack::PushL (self);
	//nothing to construct as of now
	return self;
	}

CPolicyParser::~CPolicyParser()
	{
	if ( iDomParser)
		{
		delete iDomParser;
		iDomParser = NULL;
		}
	}

TInt CPolicyParser::GetPolicyInfo(RFile& aSecPolicy,
		RProtectionDomains& aPolicyInfo, RAliasGroup& aAliasGroup)
	{
	isTrustInfo = EFalse;

	if ( iDomParser)
		{
		delete iDomParser;
		iDomParser = NULL;
		}

	iDomParser = CMDXMLParser::NewL (this);
	iLastError = KErrNone;
	
	RFile parseHandle;
	User::LeaveIfError ( parseHandle.Duplicate ( aSecPolicy));

	iPolicyInfo.Reset (); //cleans-up the array, but doesn't delete the entries..
	iAliasGroup.Reset ();

	iLastError = iDomParser->ParseFile (parseHandle); //xml file read operation
	
	if ( KErrNone == iLastError)
		iWaitScheduler.Start (); //actual parsing starts here asynchronously

	if ( KErrNone==iLastError)   //parsing is successful
	{
		aPolicyInfo = iPolicyInfo;
		aAliasGroup = iAliasGroup;
	}

	return iLastError;
	}

// Call back function called when a parsing operation completes.
void CPolicyParser::ParseFileCompleteL()
	{
	iLastError = iDomParser->Error();
	
	if(iLastError == KErrXMLMissingDocTypeTag)
	    iLastError = KErrNone;
	
	iWaitScheduler.AsyncStop ();
	    	
	if(iLastError!=KErrNone)
		{
		    return;
		}
	else
	    {
          	if ( iDomParser && !isTrustInfo) //pre-requisite that the DOM instance is intact
        		{
        		CMDXMLDocument* documentNode = iDomParser->DetachXMLDoc ();
        
        		if ( documentNode)
        			{
        			CleanupStack::PushL(documentNode);
        			//Gets the root element of the DOM tree.
        			CMDXMLElement* rootElemNode = documentNode->DocumentElement ();
        
        			CMDXMLNode* rootNode = rootElemNode->FirstChild ();
        
        			if ( !rootNode)
        				{
        				iLastError = EErrInvalidRootNode;
        				CleanupStack::PopAndDestroy(documentNode);
        				return;
        				}
        
        			TBool isValid = EFalse;
        
        			for (; rootNode;rootNode=rootNode->NextSibling ())
        				{
        				if ( rootNode->NodeType ()==CMDXMLNode::EElementNode)
        					{
        					if ( 0==rootNode->NodeName().CompareF (KPolicy ()))
        						{
        						isValid = ETrue;
        						break;
        						}
        					else
        						continue;
        					}
        				else
        					{
        					 if(KErrNone == rootNode->NodeName().CompareF(KText))
        						{
        						iLastError = EErrJunkContent;
        						iPolicyInfo.ResetAndDestroy ();
        						iAliasGroup.ResetAndDestroy ();
        						CleanupStack::PopAndDestroy(documentNode);
        						return;										
        						}
        					}
        				}
        
        			if ( !isValid)
        				{
        				iLastError = EErrInvalidRootNode;
        				iPolicyInfo.ResetAndDestroy ();
        				iAliasGroup.ResetAndDestroy ();
        				CleanupStack::PopAndDestroy(documentNode);
        				return;
        				}
        
        			TPtrC attrVal;
        
        			if ( rootNode->HasChildNodes ())
        				{
        				CMDXMLNode* childNode = rootNode->FirstChild ();
        
        				//Now get the list of <alias> nodes
        				while ( childNode)
        					{
        					TPtrC nodeName = childNode->NodeName ();
        
        					//ALIAS PARSING
        					if ( nodeName.CompareF (KAlias)== 0)
        						{
        						CMDXMLElement
        								* aliasNode = dynamic_cast<CMDXMLElement*>(childNode);
        					
        						//store alias information in object
        						CPermission* aliasObj = NULL;
        																					
        							{//Get the attribute "name" to get the alias name		    								    		
        							aliasNode->GetAttribute(KNameAttr, attrVal);
        
        							if ( attrVal.Length ()>0)
        								{
        									if ( isAliasPresent (attrVal))
        									{
        									//invalid xml file
        									if(aliasObj)
        										delete aliasObj;
        									iPolicyInfo.ResetAndDestroy ();
        									iAliasGroup.ResetAndDestroy ();
        									iLastError = EErrRepeatedAliasTag;
        									CleanupStack::PopAndDestroy(documentNode);
        									return;
        									}
        									
        									aliasObj = CPermission::NewL();
        									aliasObj->SetPermName(attrVal);
        								}
        							else
        								{
        								//alias name is invalid
        								//check whether TPerm / alias name should be deleted
        								if(aliasObj)
        									delete aliasObj;
        								iPolicyInfo.ResetAndDestroy ();
        								iAliasGroup.ResetAndDestroy ();
        								iLastError = EErrInvalidAliasName;
        								CleanupStack::PopAndDestroy(documentNode);
        								return;
        								}
        							
        							if ( aliasNode->HasChildNodes ())
        								{
        									CPermissionSet *aliasCaps = CPermissionSet::NewL();
        						
        									TBool result = GetCapabilitiesForGroup(
        														(CMDXMLElement*)aliasNode,
        														*aliasCaps, *aliasObj); //get list of uncond caps
        									
        									if(result < 0)
        									{
        										//Invalid capability string in policy file
        										if(aliasObj)
        											delete aliasObj;
        										if(aliasCaps)
        											delete aliasCaps;
        										iPolicyInfo.ResetAndDestroy ();
        										iAliasGroup.ResetAndDestroy ();
        										iLastError = result;  //EErrInvalidCapability or EErrRepeatedCaps
        										CleanupStack::PopAndDestroy(documentNode);
        										return;
        									}
        									if(aliasCaps)
        										delete aliasCaps;
        									
        								}
        							}
        							
        							if(aliasObj)
        								iAliasGroup.AppendL(aliasObj); 
        						}
        						
        					//check if the domain name is already present..
        					//need to check to avoid repeated domain tags..
        					if ( nodeName.CompareF (KDomain)== 0)
        						{
        						CMDXMLElement
        								* domainNode = dynamic_cast<CMDXMLElement*>(childNode);
        						CProtectionDomain* pPolicy=  NULL;
        
        							{//Get the attribute "name" to get the domain name		    								    		
        							domainNode->GetAttribute (KNameAttr, attrVal);
        
        							if ( attrVal.Length ()>0)
        								{
        								if ( isDomainPresent (attrVal))
        									{
        									//invalid xml file
        									//delete pPolicy;
        									iPolicyInfo.ResetAndDestroy ();
        									iAliasGroup.ResetAndDestroy ();
        									iLastError = EErrRepeatedDomainTag;
        									CleanupStack::PopAndDestroy(documentNode);
        									return;
        									}
        
        								pPolicy = CProtectionDomain::NewL ();
        								pPolicy->SetDomainName (attrVal);
        								}
        							else
        								{
        								//domain name is invalid
        								if ( pPolicy)
        									delete pPolicy;
        								iPolicyInfo.ResetAndDestroy ();
        								iAliasGroup.ResetAndDestroy ();
        								iLastError = EErrInvalidDomainName;
        								CleanupStack::PopAndDestroy(documentNode);
        								return;
        								}
        
        							if ( domainNode->HasChildNodes ())
        								{
        								CPermissionSet* caps = CPermissionSet::NewL ();
        
        								CMDXMLNode
        										* domainChildNode = domainNode->FirstChild ();
        								for (; domainChildNode;domainChildNode=domainChildNode->NextSibling ())
        									{
        									if ( domainChildNode->NodeType ()==CMDXMLNode::EElementNode)
        										{
        										if ( 0==domainChildNode->NodeName().CompareF (KAllow))//<Allow> unconditional node
        											{
        												TBool result = GetCapabilities(
        														(CMDXMLElement*)domainChildNode,
        														*caps); //get list of uncond caps
        														
        												if(result < 0)
        												{
        													//Invalid capability string in policy file
        													if ( pPolicy)
        														delete pPolicy;
        													if(caps)
        														delete caps;
        													iPolicyInfo.ResetAndDestroy ();
        													iAliasGroup.ResetAndDestroy ();
        													iLastError = result;  //EErrInvalidCapability or EErrRepeatedCaps
        													CleanupStack::PopAndDestroy(documentNode);
        													return;
        												}
        											}	
        										else if ( 0==domainChildNode->NodeName().CompareF (KUser))//<User> Conditional node
        											{
        												//Get UGCaps
        												TUserPromptOption upOption(RTUserPrompt_UnDefined);
        												
        												TInt error = GetConditions ((CMDXMLElement*)domainChildNode, upOption);//Get conditions
        												
        												if(EErrInvalidPermission == error)
        												{
        													// invalid permission is specified
        													if ( pPolicy)
        														delete pPolicy;
        													if(caps)
        														delete caps;
        													iLastError = EErrInvalidPermission;
        													iPolicyInfo.ResetAndDestroy ();
        													iAliasGroup.ResetAndDestroy ();
        													CleanupStack::PopAndDestroy(documentNode);
        													return;
        												}
        													
        												TUserPromptOption
        														defCond = GetDefaultCondition ((CMDXMLElement*)domainChildNode);
        
        												if(RTUserPrompt_UnDefined == defCond)
        													{
        													//default condition is not specified
        													if ( pPolicy)
        														delete pPolicy;
        													if(caps)
        														delete caps;
        													iLastError = EErrNoDefaultCondition;
        													iPolicyInfo.ResetAndDestroy ();
        													iAliasGroup.ResetAndDestroy ();
        													CleanupStack::PopAndDestroy(documentNode);
        													return;
        													}
        													
        												//default condition is specified
        												error = GetCapabilities (
        															(CMDXMLElement*)domainChildNode,
        															*caps,
        															upOption,
        															ETrue, defCond); //get list of ug caps	
        												if(error < 0)
        												{
        													//Invalid capability string in policy file
        													if ( pPolicy)
        														delete pPolicy;
        													if(caps)
        														delete caps;
        													iPolicyInfo.ResetAndDestroy ();
        													iAliasGroup.ResetAndDestroy ();
        													iLastError = error;  //EErrInvalidCapability or EErrRepeatedCaps
        													CleanupStack::PopAndDestroy(documentNode);
        													return;
        												}
        											}
        											else  	// neither USER nor ALLOW node
        											{
        												//checking for misplaced alias tag
        												if((KErrNone == domainChildNode->NodeName().CompareF(KAlias)))
        												{
        													if ( pPolicy)
        														delete pPolicy;
        													if(caps)
        														delete caps;
        													iLastError = EErrMisplacedAlias;
        													iAliasGroup.ResetAndDestroy ();
        													iPolicyInfo.ResetAndDestroy ();
        													CleanupStack::PopAndDestroy(documentNode);
        													return;										
        												}
        											}
        										}
        										else
        										{
        											//the node is not an element node
        											//hence it should be comment / junk content
        											if((KErrNone == domainChildNode->NodeName().CompareF(KText)))
        											{
        												if ( pPolicy)
        													delete pPolicy;
        												if (caps)
        													delete caps;
        												iLastError = EErrJunkContent;
        												iPolicyInfo.ResetAndDestroy ();
        												iAliasGroup.ResetAndDestroy ();
        												CleanupStack::PopAndDestroy(documentNode);
        												return;										
        											}
        											else if(KErrNone == domainChildNode->NodeName().CompareF(KComment))
        												continue;
        										}
        									}									
        								if ( pPolicy)
        									pPolicy->SetCapInfo (caps);
        
        								}
        							}
        
        						if ( pPolicy)
        							iPolicyInfo.AppendL (pPolicy);
        						}
        						else if((KErrNone == nodeName.CompareF(KText)))
        						{
        							iLastError = EErrJunkContent;
        							iPolicyInfo.ResetAndDestroy ();
        							iAliasGroup.ResetAndDestroy ();
        							CleanupStack::PopAndDestroy(documentNode);
        							return;										
        						}
        						
        					childNode = childNode->NextSibling (); //traverse to the next sibling node			    			
        
        					}
        				} //rootNode->HasChildNodes()
        			else
        				{
        				//No policy information in the xml file
        				//Treat this as an error
        				iLastError = EErrNoPolicyInfo;
        				iPolicyInfo.ResetAndDestroy ();
        				iAliasGroup.ResetAndDestroy ();
        				CleanupStack::PopAndDestroy(documentNode);
        				return;
        				}
        
        			CleanupStack::PopAndDestroy(documentNode);
        			}
        		else
        			{
        			iLastError = EErrInvalidDocNode;
        			return;
        			}
        		}
	    }
	}

TInt CPolicyParser::GetCapsNode(CMDXMLElement* aParentDomainNode,
		RPointerArray<CMDXMLElement>& aCapNodes, const TDesC& aNodeName)
	{
	if ( aParentDomainNode->HasChildNodes ())
		{
		CMDXMLNode* childNode = aParentDomainNode->FirstChild ();

		do
			{
			if ( childNode)
				{
				if ( 0==childNode->NodeName().Compare (aNodeName))
					{
					CMDXMLElement* pAllowNode = dynamic_cast<CMDXMLElement*>(childNode);
					aCapNodes.AppendL (pAllowNode);
					}

				childNode = childNode->NextSibling ();
				}

			}
		while (NULL != childNode);
		}

	return EErrNone; //No other possible return err code...
	}

CMDXMLElement* CPolicyParser::GetCapsNode(CMDXMLElement* aParentDomainNode,
		const TDesC& aNodeName)
	{
	if ( aParentDomainNode->HasChildNodes ())
		{
		CMDXMLNode* childNode = aParentDomainNode->FirstChild ();

		do
			{
			if ( childNode)
				{
				if ( 0==childNode->NodeName().Compare (aNodeName))
					{
					CMDXMLElement* pAllowNode = dynamic_cast<CMDXMLElement*>(childNode);
					return pAllowNode;
					}

				childNode = childNode->NextSibling ();
				}

			}
		while (NULL != childNode);
		}
	return NULL;
	}

TInt CPolicyParser::GetCapabilities(CMDXMLElement* aParentNode,
		CPermissionSet& aCapInfo, TUserPromptOption aUpOpt, TBool aUGCaps,
		TUserPromptOption aDefUpOpt)
	{
	if ( aParentNode->HasChildNodes ())
		{
		CMDXMLNode* childNode = aParentNode->FirstChild ();

		do
			{
			if ( childNode)
				{
				CMDXMLNode* capabilitiesNode=  NULL;
				if ( 0==childNode->NodeName().CompareF (KCapabilities))
					{
					capabilitiesNode = dynamic_cast<CMDXMLElement*>(childNode);

					//traverse <capability/> nodes under <capabilities/>
					if ( capabilitiesNode->HasChildNodes ())
						{
						RCapabilityArray capabilities;
						
						for (CMDXMLNode
								* capabilityNode=capabilitiesNode->FirstChild (); capabilityNode;capabilityNode=capabilityNode->NextSibling ())
							{
							//Get the textnode under <capability/>
							if ( capabilityNode)
								{
								for (CMDXMLNode
										* capChildNode=capabilityNode->FirstChild (); capChildNode;capChildNode=capChildNode->NextSibling ())
									{
									//Get the textnode under <capability/>
									if ( CMDXMLNode::ETextNode==capChildNode->NodeType ())
										{
										CMDXMLText
												* capTextNode = dynamic_cast<CMDXMLText*>(capChildNode);
										//Extract text out of this node
										TCapability
												cap = GetCapability (capTextNode->Data ());

										if ((ECapability_None!=cap))
											{
											
											// Get LOGIC for the storage of Group capability
											
											if ( aUGCaps)
												{
												if ( (!IsPresent(aCapInfo,cap)) && (!IsUserGrantPresent(aCapInfo,cap)))
													{
														capabilities.Append(cap);
														
														if ( RTUserPrompt_UnDefined==aUpOpt)								
															{
																CPermission* perm = CPermission::NewL(capabilities, aDefUpOpt,aDefUpOpt);
																aCapInfo.AppendPermission (*perm);
															}
																
															else
															{
																CPermission* perm = CPermission::NewL(capabilities, aDefUpOpt,aUpOpt);
																aCapInfo.AppendPermission (*perm);
															}
													}
													else
													{
														return EErrRepeatedCaps;	
													}
												}
											else
												{
													//Check before appending unconditional capability that
													//whether the capability is already added or not...
													if ( (!IsPresent(aCapInfo,cap)) && (!IsUserGrantPresent(aCapInfo,cap)))
													{
														aCapInfo.AppendUncondCap (cap);	
													}
													else
													{
														//capability string given more than once.. hence invalid
														return EErrRepeatedCaps;		
													}
												}
											}
										else
											{
												TBool invalidCaps = ETrue;
												
												for(TInt idx = 0; idx < iAliasGroup.Count(); idx++)
												{
													CPermission *tempObj = iAliasGroup[idx];
													TPtrC name = tempObj->PermName();
													
													if ((KErrNone == capTextNode->Data().CompareF(name)))
													{
													//it is not a text node. so is an alias group
													
													//alias group name is found. so obtain the caps
													TUint32 capAlias = tempObj->PermissionData();
													invalidCaps = EFalse;
													if( aUGCaps)
														{
															for(TInt idx1 = 0; idx1 < 20; idx1++)
															{
																TUint32 tempCapToCheck(KDefaultEnableBit);
																tempCapToCheck = tempCapToCheck << (idx1);
																
																if(capAlias & tempCapToCheck)
																	capabilities.Append((TCapability)idx1);
																
															}
															if ( RTUserPrompt_UnDefined==aUpOpt)								
															{
																CPermission* perm = CPermission::NewL(capabilities, aDefUpOpt,aDefUpOpt);
																perm->SetPermName(name);
																aCapInfo.AppendPermission (*perm);
															}
																
															else
															{
																CPermission* perm = CPermission::NewL(capabilities, aDefUpOpt,aUpOpt);
																perm->SetPermName(name);
																aCapInfo.AppendPermission (*perm);
															}
														}
													else
														{
														TUint32 capPresent = aCapInfo.UnconditionalCaps();
														TUint32 capAfter = capAlias | capPresent;
														TUint32 capRep = capAlias ^ capPresent;
														TUint32 errChk = capRep & capAfter; //error if capAfter and error are different
														
														if(!(errChk & capAfter))
															{
															//capability string given more than once.. hence invalid
															return EErrRepeatedCaps;
															}
															
														aCapInfo.AppendUncondCapabilities(capAfter);	
														}
													}
												}

												if (invalidCaps)
												{
												//just log the error message
												RDebug::Print (_L ("CPolicyParser::GetCapabilities : Invalid capability string"));
												return EErrInvalidCapability;
												}
											}

										}
									}
								}
								capabilities.Close();						
							}
						}
					else
						{
						RDebug::Print (_L ("CPolicyParser::GetCapabilities : No <capability> under <capabilities/>"));
						}

					}

				childNode = childNode->NextSibling ();
				}
			else
				{
				RDebug::Print (_L ("CPolicyParser::GetCapabilities : Childnode NULL"));
				}
			}
		while (NULL != childNode);
		}
		
	return EErrNone;
	}

TInt CPolicyParser::GetCapabilitiesForGroup(CMDXMLElement* aParentNode,
		CPermissionSet& aCapInfo, CPermission& aAliasInfo, TUserPromptOption aUpOpt, TBool aUGCaps,
		TUserPromptOption aDefUpOpt)
	{
	if ( aParentNode->HasChildNodes ())
		{
		CMDXMLNode* childNode = aParentNode->FirstChild ();

		do
			{
			if ( childNode)
				{
				CMDXMLNode* capabilitiesNode=  NULL;
				if ( 0==childNode->NodeName().CompareF (KCapabilities))
					{
					capabilitiesNode = dynamic_cast<CMDXMLElement*>(childNode);

					//traverse <capability/> nodes under <capabilities/>
					if ( capabilitiesNode->HasChildNodes ())
						{
						RCapabilityArray capabilities;
						
						for (CMDXMLNode
								* capabilityNode=capabilitiesNode->FirstChild (); capabilityNode;capabilityNode=capabilityNode->NextSibling ())
							{
							//Get the textnode under <capability/>
							if ( capabilityNode)
								{
								for (CMDXMLNode
										* capChildNode=capabilityNode->FirstChild (); capChildNode;capChildNode=capChildNode->NextSibling ())
									{
									//Get the textnode under <capability/>
									if ( CMDXMLNode::ETextNode==capChildNode->NodeType ())
										{
										CMDXMLText
												* capTextNode = dynamic_cast<CMDXMLText*>(capChildNode);
										//Extract text out of this node
										TCapability
												cap = GetCapability (capTextNode->Data ());

										
										if ((ECapability_None!=cap))
											{
											
											// Get LOGIC for the storage of Group capability
											
											if ( aUGCaps)
												{
												//For ALIAS GROUP this condition is not true and foll stm are not executed
												if ( (!IsPresent(aCapInfo,cap)) && (!IsUserGrantPresent(aCapInfo,cap)))
													{
													capabilities.Append(cap);
													
													if ( RTUserPrompt_UnDefined==aUpOpt)								
															{
																CPermission* perm = CPermission::NewL(capabilities, aDefUpOpt,aDefUpOpt);
																
																aCapInfo.AppendPermission (*perm);
															}
																
															else
															{
																CPermission* perm = CPermission::NewL(capabilities, aDefUpOpt,aUpOpt);
																
																aCapInfo.AppendPermission (*perm);
															}
													}
													else
													{
													return EErrRepeatedCaps;	
													}
												}
											else
												{
													//Check before appending unconditional capability that
													//whether the capability is already added or not...
													if ( (!IsPresent(aCapInfo,cap) && (!IsUserGrantPresent(aCapInfo,cap))) && (!IsCapsAliasPresent(aAliasInfo,cap)))
													{
														aCapInfo.AppendUncondCap (cap);	
														aAliasInfo.AppendCapPermData(cap);
													}
													else
													{
														//capability string given more than once.. hence invalid
														return EErrRepeatedCaps;		
													}
												}
											}
										else
											{
												TBool invalidCaps = ETrue;
												for(TInt idx = 0; idx < iAliasGroup.Count(); idx++)
												{
													CPermission *tempObj = iAliasGroup[idx];
													TPtrC name = tempObj->PermName();
													
													if ((KErrNone == capTextNode->Data().CompareF(name)))
													{
													invalidCaps = EFalse;
													//it is not a text node. so is an alias group
													
													//alias group name is found. so obtain the caps
													TUint32 capAlias = tempObj->PermissionData();
													TUint32 capPresent = aCapInfo.UnconditionalCaps();
													TUint32 capAfter = capAlias | capPresent;
													TUint32 capRep = capAlias ^ capPresent;
													TUint32 errChk = capRep & capAfter; //error if capAfter and error are different
													
													if(!(errChk & capAfter))
														{
														//capability string given more than once.. hence invalid
														return EErrRepeatedCaps;
														}
														
													aCapInfo.AppendUncondCapabilities(capAfter);
													aAliasInfo.SetPermissionData(capAfter);
													}
												}

												if (invalidCaps)
												{
												//just log the error message
												RDebug::Print (_L ("CPolicyParser::GetCapabilities : Invalid capability string"));
												return EErrInvalidCapability;
												}
											}

										}										
									}
								}
						
							
							capabilities.Close();						
							}
						}
					else
						{
						RDebug::Print (_L ("CPolicyParser::GetCapabilities : No <capability> under <capabilities/>"));
						}

					}

				childNode = childNode->NextSibling ();
				}
			else
				{
				RDebug::Print (_L ("CPolicyParser::GetCapabilities : Childnode NULL"));
				}

			}
		while (NULL != childNode);
		}
		
		return EErrNone;
	}

TInt CPolicyParser::GetConditions(CMDXMLElement* aParentNode, TUserPromptOption& aUserPromptOpt)
	{
	TUint32 ret(RTUserPrompt_UnDefined);
	
	if ( aParentNode->HasChildNodes ())
		{
		CMDXMLNode* childNode = aParentNode->FirstChild ();

		do
			{
			if ( childNode)
				{
				CMDXMLNode* condNode=  NULL;
				if ( 0==childNode->NodeName().CompareF (KCondition))
					{
					condNode = dynamic_cast<CMDXMLElement*>(childNode);

					if ( condNode->HasChildNodes ())
						{
						for (CMDXMLNode* condChildNode=condNode->FirstChild (); condChildNode;condChildNode=condChildNode->NextSibling ())
							{
							//Get the textnode under <condition/>
							if ( CMDXMLNode::ETextNode==condChildNode->NodeType ())
								{
								CMDXMLText
										* condition = dynamic_cast<CMDXMLText*>(condChildNode);

								if ( condition)
									{
									TInt32 returnVal = GetUserPromptOption (condition->Data ());
									
										if(RTUserPrompt_UnDefined == returnVal)
										{
										  aUserPromptOpt = (TUserPromptOption)returnVal;
										  return EErrInvalidPermission;						
										}
									
									ret |= returnVal;
									}
								}
							}
						}
					}
				childNode = childNode->NextSibling ();
				}

			}
		while (NULL != childNode);
		}

	aUserPromptOpt = (TUserPromptOption)ret;
	return EErrNone;
	}

TBool CPolicyParser::isDomainPresent(const TDesC& aDomainName)
	{
	TBool isPresent(EFalse);

	for (TInt i(0); i!=iPolicyInfo.Count ();++i)
		{
		if ( 0==iPolicyInfo[i]->DomainName().CompareF (aDomainName))
			{
			isPresent=ETrue;
			break;
			}
		}

	return isPresent;
	}
	
TBool CPolicyParser::IsPresent(const CPermissionSet& aCapInfo, TCapability aCap)
	{
	TUint32 uncondCaps = aCapInfo.UnconditionalCaps ();

	TUint32 tempCapToCheck(KDefaultEnableBit);
	tempCapToCheck = tempCapToCheck << (aCap);

	return (uncondCaps & tempCapToCheck);
	}

TBool CPolicyParser::IsCapsAliasPresent(const CPermission& aAliasInfo, TCapability aCap)
	{
	TUint32 uncondCaps = aAliasInfo.PermissionData ();

	TUint32 tempCapToCheck(KDefaultEnableBit);
	tempCapToCheck = tempCapToCheck << (aCap);

	return (uncondCaps & tempCapToCheck);
	}

TBool CPolicyParser::IsUserGrantPresent(const CPermissionSet& aCapInfo,
		TCapability aCap)
	{
	TUint32 perms(KDefaultNullBit);
	//RPermissions tempPerms = aCapInfo.Permissions();
	for (TInt i(0); i!=(aCapInfo.Permissions()).Count ();++i)
		{
		TUint32 temp(KDefaultEnableBit);
		RCapabilityArray capabilities;
		(aCapInfo.Permissions())[i]->Capabilitilites(capabilities);
		for(TInt capIdx(0);capIdx!=capabilities.Count();++capIdx)
			{
			temp = temp << (capabilities[capIdx]);
			perms |= temp;
			}		
		capabilities.Close();
		}

	TUint32 temp(KDefaultEnableBit);
	temp = temp << (aCap);
	return (perms & temp);
	}

TUserPromptOption CPolicyParser::GetDefaultCondition(CMDXMLElement* aParentNode)
	{
	if ( aParentNode->HasChildNodes ())
		{
		CMDXMLNode* childNode = aParentNode->FirstChild ();

		do
			{
			if ( childNode)
				{
				CMDXMLNode* condNode=  NULL;
				if ( 0==childNode->NodeName().CompareF (KDefCondition))
					{
					condNode = dynamic_cast<CMDXMLElement*>(childNode);

					if ( condNode->HasChildNodes ())
						{
						for (CMDXMLNode* condChildNode=condNode->FirstChild (); condChildNode;condChildNode=condChildNode->NextSibling ())
							{
							if ( CMDXMLNode::ETextNode==condChildNode->NodeType ())
								{
								CMDXMLText
										* condition = dynamic_cast<CMDXMLText*>(condChildNode);

								if ( condition)
									{
									return GetUserPromptOption (condition->Data ());
									}
								}
							}
						}
					}

				childNode = childNode->NextSibling ();
				}

			}
		while (NULL != childNode);
		}

	return RTUserPrompt_UnDefined;
	}

TCapability CPolicyParser::GetCapability(const TDesC& aUserPromptOpt)
	{
	TCapability cap(ECapability_None);

	if ( 0==aUserPromptOpt.CompareF (KCapabilityTCB))
		{
		cap=ECapabilityTCB;
		}
	else
		if ( 0==aUserPromptOpt.CompareF (KCapabilityCommDD))
			{
			cap=ECapabilityCommDD;
			}
		else
			if ( 0==aUserPromptOpt.CompareF (KCapabilityPowerMgmt))
				{
				cap=ECapabilityPowerMgmt;
				}
			else
				if ( 0==aUserPromptOpt.CompareF (KCapabilityMultimediaDD))
					{
					cap=ECapabilityMultimediaDD;
					}
				else
					if ( 0==aUserPromptOpt.CompareF (KCapabilityReadDeviceData))
						{
						cap=ECapabilityReadDeviceData;
						}
					else
						if ( 0==aUserPromptOpt.CompareF (KCapabilityWriteDeviceData))
							{
							cap=ECapabilityWriteDeviceData;
							}
						else
							if ( 0==aUserPromptOpt.CompareF (KCapabilityDRM))
								{
								cap=ECapabilityDRM;
								}
							else
								if ( 0==aUserPromptOpt.CompareF (KCapabilityTrustedUI))
									{
									cap=ECapabilityTrustedUI;
									}
								else
									if ( 0==aUserPromptOpt.CompareF (KCapabilityProtServ))
										{
										cap=ECapabilityProtServ;
										}
									else
										if ( 0==aUserPromptOpt.CompareF (KCapabilityDiskAdmin))
											{
											cap=ECapabilityDiskAdmin;
											}
										else
											if ( 0==aUserPromptOpt.CompareF (KCapabilityNetworkControl))
												{
												cap=ECapabilityNetworkControl;
												}
											else
												if ( 0==aUserPromptOpt.CompareF (KCapabilityAllFiles))
													{
													cap=ECapabilityAllFiles;
													}
												else
													if ( 0==aUserPromptOpt.CompareF (KCapabilitySwEvent))
														{
														cap=ECapabilitySwEvent;
														}
													else
														if ( 0==aUserPromptOpt.CompareF (KCapabilityNetworkServices))
															{
															cap=ECapabilityNetworkServices;
															}
														else
															if ( 0==aUserPromptOpt.CompareF (KCapabilityLocalServices))
																{
																cap=ECapabilityLocalServices;
																}
															else
																if ( 0==aUserPromptOpt.CompareF (KCapabilityReadUserData))
																	{
																	cap=ECapabilityReadUserData;
																	}
																else
																	if ( 0==aUserPromptOpt.CompareF (KCapabilityWriteUserData))
																		{
																		cap=ECapabilityWriteUserData;
																		}
																	else
																		if ( 0==aUserPromptOpt.CompareF (KCapabilityLocation))
																			{
																			cap=ECapabilityLocation;
																			}
																		else
																			if ( 0==aUserPromptOpt.CompareF (KCapabilitySurroundingsDD))
																				{
																				cap=ECapabilitySurroundingsDD;
																				}
																			else
																				if ( 0==aUserPromptOpt.CompareF (KCapabilityUserEnvironment))
																					{
																					cap=ECapabilityUserEnvironment;
																					}
	return cap;
	}

TUserPromptOption CPolicyParser::GetUserPromptOption(const TDesC& aUserPromptOpt)
	{
	TUserPromptOption userPromptOpt = RTUserPrompt_UnDefined;

	if ( 0==aUserPromptOpt.CompareF (KUserPromptOneShot))
		{
		userPromptOpt=RTUserPrompt_OneShot;
		}
	else
		if ( 0==aUserPromptOpt.CompareF (KUserPromptSession))
			{
			userPromptOpt=RTUserPrompt_Session;
			}
		else
			if ( 0==aUserPromptOpt.CompareF (KUserPromptBlanket))
				{
				userPromptOpt=RTUserPrompt_Permanent;
				}
	return userPromptOpt;
	}

TBool CPolicyParser::isAliasPresent(const TDesC& aAliasName)
	{
		TBool isPresent(EFalse);

		for(TInt idx = 0; idx < iAliasGroup.Count(); idx++)
		{
			if( KErrNone == iAliasGroup[idx]->PermName().CompareF(aAliasName))
			{
				isPresent = ETrue;
				break;
			}
			
		}
		
		return isPresent;
	}
