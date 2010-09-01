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
* Description:       Declares an API for the consumer applications to access the 
*                Language Interworking Framework. 
*
*/







#ifndef LIW_SERVICE_HANDLER_H
#define LIW_SERVICE_HANDLER_H

// INCLUDES
#include <barsread.h> 
#include <liwcommon.h>
#include <e32capability.h>

// CONSTANTS

// MACROS

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CLiwServiceHandlerImpl;

class CRTSecMgrScriptSession;


enum TLiwLoadStatus
{
	KLiwUnknown = -5,
	KLiwMetaDataInvalidFormat, 			//-4
	KLiwInvalidVersionSpecification,	//-3
	KLiwVersionOutOfRange,				//-2
	KLiwSecurityAccessCheckFailed,		//-1
	KLiwServiceLoadSuccess,				//0
	KLiwServiceAlreadyLoaded			//1
	
	//..Other possible error codes	
};


// CLASS DECLARATION

/**
* CLiwServiceHandler is the main class of the Language Interworking
* Framework. The Service Handler implements support for dynamically
* loadable service providers which offer services to consumer applications. 
* The Service Handler maps consumers and service providers together via 
* interests and data agreements and hides the consumers from the providers. 
*
* SERVICE is any command or functionality offered by a provider to 
* consumer. The service includes typically menu item UI elements,
* but it can also just an engine type of command which executes specific 
* functionality and reports status back to the consumer.
*
* CONSUMER application accesses interesting services offered by
* service provider(s). The consumer uses only those services in which
* it is interested in. The interest is expressed using a set of criteria
* items.
*
* INTEREST is a list of criteria items.
*
* CRITERIA consists of set of attributes which guide the LIW Framework to use
* only those providers in which the consumer is interested in.
* The criteria consists of following attributes:
*   - Criteria UID (we can allocate pre-defined criteria items).
*   - Service command UID (we can allocate pre-defined commands).
*   - Content MIME type (string).
*   - Additional options (variant data type just in case).
*
* PROVIDER produces those services for a consumer that match the given criteria 
* specified by the consumer. A provider can offer menu items and their command
* handling logic to the consumer applications. A provider can also offer base
* services that don't require any UI elements.
*
* DATA AGREEMENT is an agreement between consumer and provider about parameters 
* needed to be passed in a use case.
*
* @lib ServiceHandler.lib
* @since Series 60 2.6
*/
NONSHARABLE_CLASS(CLiwServiceHandler) : public CBase
    {
    public:  // Construction & destruction

        /**
        * Constructs a Service Handler instance.
        */
        IMPORT_C static CLiwServiceHandler* NewL();
        
        /**
        * Constructs a Service Handler instance.
        */
        IMPORT_C static CLiwServiceHandler* NewLC();  

        /** 
        * Destructor.
        */
        IMPORT_C virtual ~CLiwServiceHandler();
        
    public:  // Management

        /**
        * Resets the Service Handler, discards existing interest and unloads 
        * corresponding service providers.
        */
        IMPORT_C void Reset();

        /**
        * Returns the amount of providers that fulfil the given criteria.
        *
        * @param aCriteria Criteria to match.
        * @return Number of providers matching the criteria.
        */
        IMPORT_C TInt NbrOfProviders(const CLiwCriteriaItem* aCriteria);
        
    public:  // Interest handling
        
        /**
        * Adds the given interest to the Service Handler from a resource and updates 
        * possibly existing old interest. Corresponding service providers are loaded.
        * If a provider leaves during initialization, it is trapped by the Service Handler.
        *
        * @param aInterestResourceId ID of the resource containing criteria items.
        * @leave KErrNotSupported CCoeEnv is not accessible.
        * @see Reset
        */
        IMPORT_C void AttachL(TInt aInterestResourceId);

        /**
        * Adds given interest to the Service Handler from an array of criteria items.
        * If a provider leaves during initialization, it is trapped by the Service Handler.
        *
        * @param aInterest Array of criteria items. Ownership is not transferred.
        */
        IMPORT_C TInt AttachL(const RCriteriaArray& aInterest);

        /**
        * Gets the currently valid interest in use by the Service Handler.
        *
        * @param aInterest An array of returned criteria items, may be empty.
        *                  Ownership is not transferred, i.e. the objects in the 
        *                  array must not be deleted.
        */
        IMPORT_C void GetInterest(RCriteriaArray& aInterest);

        /**
        * Removes given interest from the Service Handler. Corresponding service 
        * providers are unloaded.
        * 
        * @param aInterest Array of returned criteria items, may be empty.
        */
        IMPORT_C void DetachL(const RCriteriaArray& aInterest);

        /**
        * Removes given interest from the Service Handler. Corresponding service 
        * providers are unloaded.
        * 
        * @param aInterestResourceId ID of the resource containing criteria items.
        * @leave KErrNotSupported CCoeEnv is not accessible.
        */
        IMPORT_C void DetachL(TInt aInterestResourceId);
        
        /**
        * Returns criteria by ID.
        *
        * @param aId Criteria ID.
        * @return Criteria item pointer matching the ID. Ownership is not transferred.
        */
        IMPORT_C const CLiwCriteriaItem* GetCriteria(TInt aId);

        /**
        * Returns an empty instance of CLiwGenericParamList class. It can be
        * used for example as an input parameter list for the Service Handler's
        * API methods. This is just a convenience method and doesn't have
        * to be used. If consumer wants to create input list by itself
        * it is ok. If this method is used, the Service Handler takes care
        * of deleting returned generic parameter list.
        *
        * @return  An empty instance of CLiwGenericParameter list.
        */
        IMPORT_C CLiwGenericParamList& InParamListL();

        /**
        * Returns an empty instance of CLiwGenericParamList class. The instance can be
        * used for example as an output parameter list for Service Handler
        * API methods. This is just a convenience method and doesn't have
        * to be used. If consumer wants to create output list by itself
        * it is ok. If this method is used, Service Handler takes care
        * of deleting returned generic parameter list.
        *
        * @return  An empty instance of CLiwGenericParameter list.
        */
        IMPORT_C CLiwGenericParamList& OutParamListL();
        
    public:  // Menu handling

        /**
        * Initialises menu pane with service commands from a provider.
        * This method must be called upon DynInitMenuPaneL of consumer
        * application in order to let the provider to hook its menu items.
        *
        * @param aMenuPane Handle of the menu pane to initialise.
        * @param aMenuResourceId The menu to be attached.
        * @param aBaseMenuCmdId Base ID for the Service Handler to generate 
        *                       menu IDs for placeholders.
        * @param aInParamList Input parameter list for provider's parameters checking.
        * @leave KErrNotSupported CCoeEnv is not accessible.
        * @leave KErrOverflow Consumer application has too many LIW placeholders in its menu. 
        *                     Currently, maximum 16 is supported.
        */
        IMPORT_C void InitializeMenuPaneL(
            CEikMenuPane& aMenuPane,
            TInt aMenuResourceId,
            TInt aBaseMenuCmdId,
            const CLiwGenericParamList& aInParamList);
        /**
        * Initialises menu pane with service commands from a provider.
        * This method must be called upon DynInitMenuPaneL of consumer
        * application in order to let the provider to hook its menu items.
        * In normal circumstances, the other variant of this method should be used.
        *
        * @param aMenuPane Handle of the menu pane to initialise.
        * @param aMenuResourceId The menu to be attached.
        * @param aBaseMenuCmdId Base ID for the Service Handler to generate 
        *                       menu IDs for placeholders.
        * @param aInParamList Input parameter list for provider's parameters checking.
        * @param aUseSubmenuTextsIfAvailable If the provider has specified alternative submenu
        *                       texts for its menu items, those can be taken into use if this 
        *                       parameter is set to ETrue. This should be used only for manually 
        *                       created submenus. If using LIW_CASCADE_ID or 
        *                       LIW_INTELLIGENT_CASCADE_ID, the LIW framework can automatically 
        *                       decide whether to use the submenu texts or not, and this parameter 
        *                       has no effect.
        * @leave KErrNotSupported CCoeEnv is not accessible.
        * @leave KErrOverflow Consumer application has too many LIW placeholders in its menu. 
        *                     Currently, maximum 16 is supported.
        */
        IMPORT_C void InitializeMenuPaneL(
            CEikMenuPane& aMenuPane,
            TInt aMenuResourceId,
            TInt aBaseMenuCmdId,
            const CLiwGenericParamList& aInParamList,
            TBool aUseSubmenuTextsIfAvailable);            
        
        /**
        * Returns the service command ID associated to the menu command. If found, it means 
        * that there is a provider which can handle the menu command. Thus the command 
        * handling needs to be routed to the provider via ExecuteMenuCmdL.
        *
        * @param aMenuCmdId Menu command ID to be mapped to service cmd,
        *                   KNullServiceCmd is returned if no service command exists.
        * @return Service command ID, KLiwCmdNone if no ID is found.
        * @see ExecuteMenuCmdL
        */
        IMPORT_C TInt ServiceCmdByMenuCmd(TInt aMenuCmdId) const;

        /**
        * Tells the provider to execute a menu command invoked by the consumer.
        * Not supported if calling outside UI framework. Use ServiceCmdByMenuCmd to 
        * check if there is any provider for the menu command.
        *
        * @param aMenuCmdId The menu command to be executed.
        * @param aInParamList Input data parameters, can be an empty list.
        * @param aOutParamList Output data parameters, can be an empty list.
        * @param aCmdOptions Options for the command, see TLiwServiceCmdOptions in LiwCommon.hrh.
        * @param aCallback Callback for asynchronous command handling, parameter checking, etc.
        * @leave KErrArgument Callback is missing when required.
        * @leave KErrNotSupported No cmd matches given menu command or CCoeEnv is not accessible.
        * @see ServiceCmdByMenuCmd
        */
        IMPORT_C void ExecuteMenuCmdL(
            TInt aMenuCmdId,
            const CLiwGenericParamList& aInParamList,
            CLiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            MLiwNotifyCallback* aCallback= NULL);

        /**
        * Attach menu related criteria items to the given menu.
        * If a provider leaves during initialization, it is trapped by the Service Handler. 
        *
        * @param aMenuResourceId      Menu to be attached.
        * @param aInterestResourceId  Resource id for the interest list.
        * @leave KErrNotSupported     CCoeEnv is not accessible.
        */
        IMPORT_C void AttachMenuL(TInt aMenuResourceId, TInt aInterestResourceId);

        /**
        * Attach menu related criteria items to the given menu.
        * If a provider leaves during initialization, it is trapped by the Service Handler. 
        *
        * @param aMenuResourceId  Menu to be attached.
        * @param aReader          Resource reader for the interest list.
        * @leave KErrNotSupported CCoeEnv is not accessible.
        */
        IMPORT_C void AttachMenuL(TInt aMenuResourceId, TResourceReader& aReader);
         /**
        * Attach menu related criteria items to the given menu. 
        *
        * @param aMenuResourceId  Menu to be attached.
        * @param aInterest        Array of criteria items. Ownership is not transferred.
        * @leave KErrNotSupported CCoeEnv is not accessible.
        */
        IMPORT_C void AttachMenuL(TInt aMenuResourceId, const RCriteriaArray& aInterest);

        /**
        * Attach menu related criteria items to the given menu. 
        *
        * @param aMenuEntries     List of menu command ids, ids not related to interests may be set to 0.
        * @param aMenuResourceId  Menu to be attached.
        * @param aInterest        Interest list.
        */
        IMPORT_C void AttachMenuL(RArray<TInt>& aMenuEntries, TInt aMenuResourceId, RCriteriaArray& aInterest);

        /**
        * Detach menu related criteria items from the given menu.
        * In following cases this method just returns without doing anything:
        *   1. If interest resource id is non-zero and CCoeEnv is not accessible.
        *   2. If interest resource id is non-zero and there occurs an error when reading
        *      the interest (e.g. not enough memory). 
        * 
        * @param aMenuResourceId      Menu to be detached.
        * @param aInterestResourceId  Resource id for the interest list. If NULL, all 
        *                             criteria items are detached from the given menu.
        */
        IMPORT_C void DetachMenu(TInt aMenuResourceId, TInt aInterestResourceId);

        /**
        * Checks if there are menu providers attached to given menu id. Consumer 
        * application can use this information to decide whether a sub menu 
        * containing only LIW items should be hidden or not.
        *
        * @param  aSubMenuId The menu id to be checked.
        * @return ETrue  if there isn't any menu providers attached to this menu.
        *         EFalse otherwise. 
        */
        IMPORT_C TBool IsSubMenuEmpty(TInt aSubMenuId);

        /**
        * Returns boolean value indicating whether the given menu contains
        * currently attached placeholders.
        *
        * @param   aMenuResourceId  Resource id of the menu to be queried.
        * @return  ETrue  if aMenuResource contains currently attached placeholders.
        *          EFalse otherwise. 
        */
        IMPORT_C TBool IsLiwMenu(TInt aMenuResourceId);

        /**
        * Handles LIW submenus. This method should be called from consumer application's 
        * DynInitMenuPaneL.
        *
        * @param  aPane  Menu pane to be handled.
        * @return ETrue  if aPane was an LIW submenu and it was handled. 
        *                Consumer's DynInitMenuPaneL pane may now return.
        *         EFalse if aPane was not an LIW submenu and DynInitMenuPaneL should
        *                continue normally.
        */
        IMPORT_C TBool HandleSubmenuL(CEikMenuPane& aPane);

        /**
        * CEikMenuPane uses this method to inform AIF framework that a menu is launched.
        * This method does not need to be called by any other component.
        *
        * @since Series 60 3.0
        */
        IMPORT_C static void ReportMenuLaunch();

    public:  // Generic Service Command handling
        
        /**
        * Executes a service command for all providers. Otherwise similar to ExecuteMenuCmdL.
        *
        * @param aCmdId The command to be executed.
        * @param aInParamList Input data parameters, can be an empty list.
        * @param aOutParamList Output data parameters, can be an empty list.
        * @param aCmdOptions Options for the command, see TLiwServiceCmdOptions in LiwCommon.hrh.    
        * @param aCallback Callback for asynchronous command handling, parameter checking, etc.
        * @leave KErrArgument Callback is missing when required.
        * @leave KErrNotSupported No provider supports the service.
        */
        IMPORT_C void ExecuteServiceCmdL(
            const TInt& aCmdId,
            const CLiwGenericParamList& aInParamList,
            CLiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            MLiwNotifyCallback* aCallback = 0);

        /**
        * Executes a service command.
        *
        * @param aCmd The criteria item containing the service command and content type
        * @param aInParamList Input data parameters, can be an empty list.
        * @param aOutParamList Output data parameters, can be an empty list.
        * @param aCmdOptions Options for the command, see TLiwServiceCmdOptions in LiwCommon.hrh.    
        * @param aCallback Callback for asynchronous command handling, parameter checking, etc.
        * @leave KErrArgument Callback is missing when required.
        * @leave KErrNotSupported No provider supports the service.
        */
        IMPORT_C void ExecuteServiceCmdL(                     
            const CLiwCriteriaItem& aCmd,
            const CLiwGenericParamList& aInParamList,
            CLiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            MLiwNotifyCallback* aCallback = 0);
              
        /**
        * Gets provider command ID by dynamic command ID.
        * 
        * @since Series 60 3.0
        * @param aMenuCmdId The consumer's menu command ID generated by LIW framework. 
        *                   This can be get e.g. from consumer's HandleCommandL().
        * @return Found provider menu command ID, KErrNotFound if not found.
        */
        IMPORT_C TInt MenuCmdId(TInt aMenuCmdId) const;
        
        IMPORT_C TInt AttachL(const RCriteriaArray& aInterest,CRTSecMgrScriptSession& aSecMgrScriptSession);
        IMPORT_C TInt AttachL(const RCriteriaArray& aInterest,CRTSecMgrScriptSession& aSecMgrScriptSession,TInt aWidgetId);        
 	  /**
        * Lists available service implementations
        * 
        * @param aFilterItem 
        * @param aProviderList 
        *              
        * @return none
        */
        IMPORT_C void QueryImplementationL(RCriteriaArray& aFilterItem, RCriteriaArray& aProviderList);
        
    private:
        void ConstructL();
        CLiwServiceHandler();

    private:
        CLiwServiceHandlerImpl* iImpl;
    };

#endif // LIW_SERVICE_HANDLER_H

// END of File
