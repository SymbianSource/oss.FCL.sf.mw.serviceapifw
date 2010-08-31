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
* Description:       Implementation class for CLiwServiceHandler. 
*                See also LiwServiceHandler.h.
*
*/







#ifndef LIW_SERVICE_HANDLER_IMPL_H
#define LIW_SERVICE_HANDLER_IMPL_H

// INCLUDES
#include <ecom/ecom.h>
#include <barsread.h> 
#include <liwcommon.h>
#include "liwtlsdata.h"
#include <eikmobs.h> 
#include <e32capability.h>

// CONSTANTS

// MACROS

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CLiwMenuBinding;
class CLiwMenuPane;
class CLiwServiceIfMenu;
class CLiwServiceIfBase;
class CLiwEcomMonitor;
class CRTSecMgrScriptSession;

// CLASS DECLARATION

/**
* Internal implementation class for CLiwServiceHandler. 
*
* Contains the actual implementation logic for Service Handler, whereas the
* CLiwServiceHandler class provides only the public API. 
*
* @see CLiwServiceHandler for API documentation.
* @since Series 60 2.6
*/
NONSHARABLE_CLASS(CLiwServiceHandlerImpl) :
    public CBase,
    public MLiwNotifyCallback,
    public MEikMenuObserver,
    public MLiwMenuLaunchObserver
    {
    public:  // Construction & destruction
        /**
        * Constructs CLiwServiceHandlerImpl instance.
        *
        * @return Pointer to the new instance.
        */
        static CLiwServiceHandlerImpl* NewL();

        /** 
        * Destructor.
        */
        virtual ~CLiwServiceHandlerImpl();
        
    public:  // Management
        /**
        * @see CLiwServiceHandler::Reset()
        */
        void Reset();

        /**
        * @see CLiwServiceHandler::NbrOfProviders() 
        */
        TInt NbrOfProviders(const CLiwCriteriaItem* aCriteria);
        
        
        
    public:  // Interest handling
        /**
        * @see CLiwServiceHandler::AttachL() 
        */
        void AttachL(TInt aInterestResourceId);

        /**
        * @see CLiwServiceHandler::AttachL() 
        */
        TInt AttachL(const RCriteriaArray& aInterest);
        
        TInt AttachL(const RCriteriaArray& aInterest,CRTSecMgrScriptSession* aSecMgrScriptSession);
        /**
        * @see CLiwServiceHandler::QueryImplementationL() 
        */
        void QueryImplementationL(CLiwCriteriaItem* aItem, RCriteriaArray& aProviderList);
		void GetCapabilitiesL(RArray<TCapability>& secMgrCapList,CLiwGenericParamList* pMetaData);
        /**
        * @see CLiwServiceHandler::GetProviderResourceFile() 
        */
        void GetProviderResourceFile(TDes& aFilePath, CLiwGenericParamList* pMetaData);
        
        /**
        * @see CLiwServiceHandler::GetInterest()
        */
        void GetInterest(RCriteriaArray& aInterest);

        /**
        * @see CLiwServiceHandler::DetachL()
        */
        void DetachL(const RCriteriaArray& aInterest);

        /**
        * @see CLiwServiceHandler::DetachL()
        */
        void DetachL(TInt aInterestResourceId); 
        
        /**
        * @see CLiwServiceHandler::GetCriteria()
        */
        const CLiwCriteriaItem* GetCriteria(TInt aId);

        /**
        * @see CLiwServiceHandler::InParamListL()
        */
        CLiwGenericParamList& InParamListL();

        /**
        * @see CLiwServiceHandler::OutParamListL()
        */
        CLiwGenericParamList& OutParamListL();
        
    public:  // Menu handling
        /**
        * @see CLiwServiceHandler::InitializeMenuPaneL()
        */
        void InitializeMenuPaneL(
            CEikMenuPane& aMenuPane,
            TInt aMenuResourceId,
            TInt aBaseMenuCmdId,
            const CLiwGenericParamList& aInParamList);
        /**
        * @see CLiwServiceHandler::InitializeMenuPaneL()
        */
        void InitializeMenuPaneL(
            CEikMenuPane& aMenuPane,
            TInt aMenuResourceId,
            TInt aBaseMenuCmdId,
            const CLiwGenericParamList& aInParamList,
            TBool aUseSubmenuTextsIfAvailable);            
        
        /**
        * @see CLiwServiceHandler::ServiceCmdByMenuCmd()
        */
        TInt ServiceCmdByMenuCmd(TInt aMenuCmdId) const;

        /**
        * @see CLiwServiceHandler::ExecuteMenuCmdL()
        */
        void ExecuteMenuCmdL(
            TInt aMenuCmdId,
            const CLiwGenericParamList& aInParamList,
            CLiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            MLiwNotifyCallback* aCallback= NULL);

        /**
        * @see CLiwServiceHandler::AttachMenuL()
        */
        void AttachMenuL(TInt aMenuResourceId, TInt aInterestResourceId);

        /**
        * @see CLiwServiceHandler::AttachMenuL()
        */
        void AttachMenuL(TInt aMenuResourceId, TResourceReader& aReader);
        
        /**
        * @see CLiwServiceHandler::AttachMenuL()
        */
        void AttachMenuL(TInt aMenuResourceId, const RCriteriaArray& aInterest);
        
        /**
        * Attach menu related criteria items to given menu.
        *
        * @param aMenuEntries     List of menu command ids.
        * @param aMenuResourceId  Menu to be attached.
        * @param aInterest        Interest list.
        */
        void AttachMenuL(RArray<TInt>& aMenuEntries, TInt aMenuResourceId, RCriteriaArray& aInterest);        

        /**
        * Attach menu related criteria items to given menu.
        *
        * @param aMenuResourceId    Resource reader pointing
        *                           to menu to be attached.
        */
        
        //void AttachMenuL(TResourceReader& aReader, TInt aMenuId, RCriteriaArray& aInterest);

        /**
        * @see CLiwServiceHandler::DetachMenu()
        */
        void DetachMenu(TInt aMenuResourceId, TInt aInterestResourceId);

        /**
        * @see CLiwServiceHandler::IsSubMenuEmpty()
        */
        TBool IsSubMenuEmpty(TInt aSubMenuId);

        /**
        * @see CLiwServiceHandler::IsLiwMenu()
        */
        TBool IsLiwMenu(TInt aMenuResourceId);

        /**
        * @see CLiwServiceHandler::HandleSubmenuL()
        */
        TBool HandleSubmenuL(CEikMenuPane& aPane);

    public:  // Generic service command handling
        /**
        * @see CLiwServiceHandler::ExecuteServiceCmdL()
        */
        void ExecuteServiceCmdL(
            const TInt& aCmdId,
            const CLiwGenericParamList& aInParamList,
            CLiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            MLiwNotifyCallback* aCallback = 0);
        
        /**
        * @see CLiwServiceHandler::ExecuteServiceCmdL()
        */
        void ExecuteServiceCmdL(                        
            const CLiwCriteriaItem& aCmd,
            const CLiwGenericParamList& aInParamList,
            CLiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            MLiwNotifyCallback* aCallback = 0);

        /**
        * @see CLiwServiceHandler::MenuCmdId()
        */
        TInt MenuCmdId(TInt aMenuCmdId) const;            

    public:  // Other new methods
        /** 
        * Called by CLiwEcomMonitor.
        *
        * @param aImpl An object representing this class.
        * @return An error code. KErrNone if successful.
        */ 
        static TInt SynchronizeCallBack(TAny* aImpl);
        
    public: // From MLiwNotifyCallback
        TInt HandleNotifyL(
            TInt aCmdId,
            TInt aEventId,
            CLiwGenericParamList& aEventParamList,
            const CLiwGenericParamList& aInParamList);

    public: // From MEikMenuObserver
        void SetEmphasis(CCoeControl* aMenuControl,TBool aEmphasis);

    public: // From MEikCommandObserver (via MEikMenuObserver)
        void ProcessCommandL(TInt aCommandId);

	    void GetProviderCapability(CLiwCriteriaItem* aItem,RCriteriaArray& aOutItem);
	
    private: // From MLiwMenuLaunchObserver
        void MenuLaunched();

    private:
        // Possible placeholder types.
        enum TLiwPlaceholderType
            {
            ELiwPlaceholderNormal,
            ELiwPlaceholderCascade,     
            ELiwPlaceholderIntelligentCascade
            };

    private:
        CLiwServiceHandlerImpl();
        void ConstructL();

        // Attach menu related criteria items to given menu. 
        void DoAttachMenuL(TResourceReader& aReader, TInt aMenuId, RCriteriaArray& aInterest);

        TInt DoAttachL(const RCriteriaArray& aInterest,CRTSecMgrScriptSession* aScriptSession);
        void DoAttachL(const RCriteriaArray& aInterest);
        
        void ReadInterestListL(TResourceReader& aReader, RPointerArray<CLiwCriteriaItem>& aResult);
        TInt ResolveProvidersL(CLiwBinding* aBinding, CLiwCriteriaItem* aItem, CRTSecMgrScriptSession* aScriptSession);
        TInt ResolveProvidersL(CLiwBinding* aBinding, CLiwCriteriaItem* aItem);
        
        TBool IsCached(CLiwServiceIfBase* aProvider);
        
        CLiwMenuPane* CreateEmptyLiwMenuPaneL(TInt aBaseMenuCmdId, TInt aResourceId);
        void DeleteLiwMenuPane(CLiwMenuPane* aLiwPane);
        
        TInt ResourceIdForNextFreeSlot();
        void CopyMenuItemsL(CLiwMenuPane* aSource, CEikMenuPane& aDest, TInt aStartIndex, 
            TBool aIsSubmenu);
        TInt SlotItemCmd(CEikMenuPane& aPane);
        CLiwMenuPane* MenuPaneForSlotCmd(TInt aCmdId);
        TLiwPlaceholderType PlaceholderType(CEikMenuPane& aPane, TInt aCmd, TBool& aTitleLocked);
        void ConvertPlaceholderL(CEikMenuPane& aPane, TInt aCmd, CLiwMenuPane& aLiwPane, 
            const TDesC& aTitle);
        void UnCascadeL(CEikMenuPane& aPane, TInt aCmd, CLiwMenuPane& aLiwPane);
        void SkipMenuFields(TResourceReader& aReader);
        TBool IsInLastInitialized(CLiwMenuPane* liwPane) const;
        TBool GetSubmenuTitle(CEikMenuPane& aPane, TDes& aResult);
        TBool IsCriteriaAlreadyInInterest(CLiwCriteriaItem& aItem, 
            RPointerArray<CLiwCriteriaItem>& aInterest) const;
        CLiwCriteriaItem* ConvertCriteriaItemPointerL(CLiwCriteriaItem* aCandidate);
        void FilterInterestListL(RPointerArray<CLiwCriteriaItem>& aOrginal,
            RPointerArray<CLiwCriteriaItem>& aFiltered);
        void FilterInfoArray(RImplInfoPtrArray& aArray, RArray<TInt32>& aArrayPlugin,
            CLiwCriteriaItem* aItem);   
        CLiwMenuBinding* AlreadyBound(TInt aMenuId, TInt aMenuCmd, TInt aMenuItemIndex) const;
        void SynchronizeDbL();
        void ListProvidersForCriteriaL(RArray<TInt>& aResult, CLiwCriteriaItem& aItem);
        void HandleRemovedProviders(RArray<TInt>& aInMemory, RImplInfoPtrArray& aOnSystem);
        void HandleNewProvidersL(RArray<TInt>& aInMemory, RImplInfoPtrArray& aOnSystem, 
            CLiwCriteriaItem* aItem);
        void RemoveProvider(TInt aImplUid);
        void AddProviderL(TUid aImplUid, CLiwCriteriaItem* aItem);
        inline void ClearMenuPaneArray();
        void AttachServiceManagerPluginsL();
        void ServiceManagerPlugin_ListImplementationsL(RArray<TInt32>& aArray, CLiwCriteriaItem* aItem);
        CLiwServiceIfBase* ServiceManagerPlugin_CreateImplementationL(TInt32 aImplUid);
        TInt NumAlreadyInitializedPaneIdsL() const;

        // Leaving helper method. Handler cleanupstack issues and calls ReadInterestListL().
        void ReadInterestL(RCriteriaArray& aInterest, TInt aInterestResourceId);

        // Detaches all items from given menu.        
        void DoDetachMenu(TInt aMenuResourceId);

        // Detaches only those items from a given menu, which match the given interest.


        void DoDetachMenu(TInt aMenuResourceId, RCriteriaArray& aInterest);
        
        // Removes unnecessary criteria items from Service Handler.
        void RemoveObsoleteCriteriaItems();
        
        // Removes unnecessary providers from Service Handler.
        void RemoveObsoleteProviders();
        
        /* Parses the metadata information stored in the opaque_data field
		 * of service provider registration information \c REGISTRY_INFO.
		 *
		 * The metadata information is seprated from the service command definition 
		 * in the opaque_data field using a separator "::".
		 *
		 * @param aOpaque	the opaque_data values specified in the registration information
		 * @param aMetaData the parsed metadata entries will be stored in this variable
		 * 
		 */
		void ParseMetaData(const TDesC8& aOpaque, TDes8& aMetaData);
		
		//To fetch the infoArray
        void QueryImplementationL(CLiwCriteriaItem* aItem, RImplInfoPtrArray& aInfoArray);

  		TCapability GetServiceCapability(const TDesC& aCapName);
		
		//To fetch the version range specified by the consumer while creating Criteria
		TBool GetVersionRange(CLiwCriteriaItem* aItem,TReal& aMinVersion, TReal& aMaxVersion);
		
		//To fetch the version range specified by the consumer while creating Criteria
		TBool GetVersion (CLiwCriteriaItem* aItem,TReal& aVersion);
		
		//To fetch the implementation version specified in provider metadata
		void ComputeIntfVersion(CLiwServiceData* pProvMetaData,TReal& aIntfVersion);
		
    private:
        // Pointer to the control environment.
        CCoeEnv* iCoeEnv;
        
        // Owns all the attached criteria items.
        RPointerArray<CLiwCriteriaItem> iInterestList;  
        
        // Criteria -> provider bindings for base service providers.
        RPointerArray<CLiwBinding>      iBaseBindings;  
        
        // Criteria -> provider bindings for menu service providers.
        RPointerArray<CLiwMenuBinding>  iMenuBindings;  
        
        // Contains the LIW menu panes.
        RPointerArray<CLiwMenuPane>     iMenuPanes;     

        // Input parameter list which can be given to the consumer application.
        CLiwGenericParamList* iInParams;
        
        // Output parameter list which can be given to the consumer application.
        CLiwGenericParamList* iOutParams; 
        
        // Resource offset for LiwServiceHandler.rsc.
        TInt iResourceOffset;
        
        // Owns all the providers currently in memory.
        RPointerArray<CLiwServiceIfBase> iProviders;  
          
        TInt                             iNextFreeSlot;
        RPointerArray<CLiwMenuPane>      iLastInitialized;
        CLiwMenuPane*                    iSubmenu;
        TInt                             iSubmenuCmd;
        CLiwEcomMonitor*                 iEcomMonitor;
        TBool                            iTlsDataOpened;
    };


inline void CLiwServiceHandlerImpl::ClearMenuPaneArray()
    {
    iMenuPanes.ResetAndDestroy();
    }

#endif // LIW_SERVICE_HANDLER_IMPL_H

// END of File


