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
* Description:       Implements API for consumer application to access Language
*                Interworking Framework. 
*
*/






#include <eikenv.h>
#include <bautils.h>    // file helpers
#include <liwservicehandler.rsg>
#include "liwmenubinding.h"
#include "liwservicehandler.h"
#include "liwservicehandlerimpl.h"
#include "liwmenu.h"
#include "liwuids.hrh"
#include "liwcommon.hrh"
#include "liwmenuslot.hrh"
#include "liwecommonitor.h"
#include "liwtlsdata.h"
#include "data_caging_path_literals.hrh"

#include "liwxmlhandler.h"
#include "liwservicedata.h"

#include <rtsecmgrscriptsession.h>

// CONSTANTS
// Max number of empty menu resource slots.
const TInt KMaxMenuResources = 16;

// This value tells how many times consumer can call InitializeMenuPaneL() without 
// closing the Options-menu.
const TInt KMaxPaneIds = KMaxMenuResources;

// The range reserved for individual menu pane.
const TInt KIndividualMenuPaneIdRange = 10000;

// The whole range that is reserved to all menu panes. Currently value is 170 000.
const TInt KMenuPaneCommandRange = (KMaxMenuResources + 1) * KIndividualMenuPaneIdRange; 

_LIT(KLiwResourceFile, "liwServiceHandler.rsc");
_LIT(KLiwZDrive, "z:");
_LIT8(KDataSeparator, "||");
_LIT(KPerExtension,".per");

const TInt KMaxMenuTitleSize = 100;

// Command id space reserved for single placeholder.
const TInt KPlaceholderCmdIdRange = 200;

const TInt KMaxLength=255;

const TReal KDefVersion = 1.0;
const TReal KUnspVersion = 0.0;

void Cleanup(TAny* aAny);
void InterestCleanup(TAny* aAny);
void IntArrayCleanup(TAny* aAny);
void FilteredCleanup(TAny* aAny);
void Int32ArrayCleanup(TAny* aAny);
void InterfaceCleanup(TAny* aAny);

_LIT8(KResolverInterface, "IResolver");
_LIT8(KResolverDomain, "ServiceManager");
_LIT8(serviceCmdSeparator, "::");

const TInt KDummySrvCmd=1;
_LIT(KCapabilityCommDD,"CDD");
_LIT(KCapabilityPowerMgmt,"PMT");
_LIT(KCapabilityMultimediaDD,"MDD");
_LIT(KCapabilityReadDeviceData,"RDD");
_LIT(KCapabilityWriteDeviceData,"WDD");
_LIT(KCapabilityDRM,"DRM");
_LIT(KCapabilityTrustedUI,"TUI");
_LIT(KCapabilityProtServ,"PSV");
_LIT(KCapabilityDiskAdmin,"DAD");
_LIT(KCapabilityNetworkControl,"NWC");
_LIT(KCapabilityAllFiles,"ALF");
_LIT(KCapabilitySwEvent,"SWE");
_LIT(KCapabilityNetworkServices,"NWS");
_LIT(KCapabilityLocalServices,"LOS");
_LIT(KCapabilityReadUserData,"RUD");
_LIT(KCapabilityWriteUserData,"WUD");
_LIT(KCapabilityLocation,"LOC");
_LIT(KCapabilitySurroundingsDD,"SDD");
_LIT(KCapabilityUserEnvironment,"USE");

using namespace LIW;
//
// LiwServiceHandler
//

CLiwServiceHandlerImpl* CLiwServiceHandlerImpl::NewL()
    {
    CLiwServiceHandlerImpl* handler = new (ELeave) CLiwServiceHandlerImpl();
    CleanupStack::PushL( handler );
    handler->ConstructL();
    CleanupStack::Pop(handler); // handler
    return handler;
    }



CLiwServiceHandlerImpl::CLiwServiceHandlerImpl()
    {
    // Nothing to do here.
    }



void CLiwServiceHandlerImpl::ConstructL()
    {
    TFileName resFile;
    TCallBack callBack(SynchronizeCallBack, this);
    iEcomMonitor = CLiwEcomMonitor::NewL(callBack);
	
    
    iCoeEnv = CCoeEnv::Static();
    RFs rFs;
    TInt err = rFs.Connect();
    
    TFileName dllName;
    Dll::FileName(dllName);

    TBuf<2> drive = dllName.Left(2);
    resFile.Copy(drive);
    resFile.Append(KDC_RESOURCE_FILES_DIR);
    resFile.Append(KLiwResourceFile);	
    TBool fileExists( BaflUtils::FileExists(rFs, resFile) );
    if ( ! fileExists )
    {
	resFile.Copy(KLiwZDrive);
	resFile.Append(KDC_RESOURCE_FILES_DIR);
	resFile.Append(KLiwResourceFile);
    }
    rFs.Close();
    // A Service Handler instance can be created also when CCoeEnv is not 
    // available (e.g. from server applications). In this case, the methods 
    // needing CCoeEnv/CEikonEnv will leave with KErrNotSupported.
    if(iCoeEnv)
        {
        // This is commented to avoid resource file getting locked affecting the IAD update. 
	// Also, there is no use of resource file since LIW does not support menu related services
        // iResourceOffset = iCoeEnv->AddResourceFileL(resFile);
        }
    // CLiwTlsData has a reference count so each OpenL call
    // must have a matching Close call (done in destructor).
    // OpenL is called only here, the TLS data object can be
    // referenced by calling CLiwTlsData::Instance().
    CLiwTlsData* data = CLiwTlsData::OpenL();
    iTlsDataOpened = ETrue;

    // CEikMenuPane informs all menu launch observers
    // when an options menu is launched.
    data->AddMenuLaunchObserverL( this );
    }



CLiwServiceHandlerImpl::~CLiwServiceHandlerImpl()
    {
    if (iResourceOffset && iCoeEnv)
        {
	// This is commented to avoid resource file getting locked affecting the IAD update. 
	// Also, there is no use of resource file since LIW does not support menu related services
        // iCoeEnv->DeleteResourceFile(iResourceOffset);
        }
    Reset();

    delete iEcomMonitor;

    if ( iTlsDataOpened )
        {
        CLiwTlsData* data = CLiwTlsData::Instance();
        data->RemoveMenuLaunchObserver( this );
        CLiwTlsData::Close();
        }
    }



void CLiwServiceHandlerImpl::Reset()
    {
    iInterestList.ResetAndDestroy();
    iMenuBindings.ResetAndDestroy();
    iBaseBindings.ResetAndDestroy();
    iProviders.ResetAndDestroy();

    iLastInitialized.Reset();

    iMenuPanes.ResetAndDestroy();

    delete iInParams;
    iInParams = NULL;
    delete iOutParams;
    iOutParams = NULL;
    }

void CLiwServiceHandlerImpl::AttachServiceManagerPluginsL()
{
  CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(KDummySrvCmd, KResolverInterface, KResolverDomain);
  crit->SetServiceClass(TUid::Uid(KLiwClassBase));
  
  RCriteriaArray a;
  a.AppendL(crit);
  
  AttachL(a);
  
  CleanupStack::Pop(crit); // crit
  a.ResetAndDestroy();
}  

void CLiwServiceHandlerImpl::ServiceManagerPlugin_ListImplementationsL(RArray<TInt32>& aArray,
                                                                       CLiwCriteriaItem* aItem)
{
  CLiwGenericParamList* inps = &(InParamListL());
  CLiwGenericParamList* outps = &(OutParamListL());
  
  CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(KDummySrvCmd, KResolverInterface, KResolverDomain);
  crit->SetServiceClass(TUid::Uid(KLiwClassBase));

  ExecuteServiceCmdL(*crit, *inps, *outps);  
  CleanupStack::PopAndDestroy(crit); // crit

  TInt pos = 0;
  MLiwInterface* ifp;
  const TLiwGenericParam* p = outps->FindFirst(pos, KResolverInterface, EVariantTypeInterface);
  if (p) {
    ifp = p->Value().AsInterface();
    CleanupStack::PushL(TCleanupItem(InterfaceCleanup, ifp));
  }
  else
    return;
  
  outps->Reset();
  inps->Reset();
  
  _LIT8(KId,"id");
  _LIT8(KCmd,"cmd");
  _LIT8(KCmdStr,"cmd_str");
  _LIT8(KType,"type");
  _LIT8(KListImpl,"ListImplementations");
  
  inps->AppendL(TLiwGenericParam(KId, TLiwVariant(aItem->Id())));
  inps->AppendL(TLiwGenericParam(KCmd, TLiwVariant(aItem->ServiceCmd())));
  inps->AppendL(TLiwGenericParam(KCmdStr, TLiwVariant(aItem->ServiceCmdStr())));
  inps->AppendL(TLiwGenericParam(KType, TLiwVariant(aItem->ContentType())));
  ifp->ExecuteCmdL(KListImpl, *inps, *outps);
  
  pos = 0;
  p = outps->FindFirst(pos, EGenericParamError);
  if (p && (p->Value().AsTInt32() == KErrNone)) {
    pos = 0;
    const CLiwList* list;
    _LIT8(KIdList,"id_list");
    p = outps->FindFirst(pos, KIdList, EVariantTypeList);
    if (p) {
      list = p->Value().AsList();
      for (TInt i = 0; i < list->Count(); i++) {
        TLiwVariant v;
        v.PushL();
        list->AtL(i, v);
        aArray.AppendL(v.AsTInt32());
        CleanupStack::Pop(&v);
        v.Reset();
      }
    }
  }

  CleanupStack::Pop(ifp); // ifp
  ifp->Close();
}

CLiwServiceIfBase* CLiwServiceHandlerImpl::ServiceManagerPlugin_CreateImplementationL(TInt32 aImplUid)
{
  CLiwGenericParamList* inps = &(InParamListL());
  CLiwGenericParamList* outps = &(OutParamListL());
  
  CLiwCriteriaItem* crit = CLiwCriteriaItem::NewLC(KDummySrvCmd, KResolverInterface, KResolverDomain);
  crit->SetServiceClass(TUid::Uid(KLiwClassBase));

  ExecuteServiceCmdL(*crit, *inps, *outps);  
  CleanupStack::PopAndDestroy(crit); // crit

  TInt pos = 0;
  MLiwInterface* ifp = NULL;
  const TLiwGenericParam* p = outps->FindFirst(pos, KResolverInterface, EVariantTypeInterface);
  if (p) {
    ifp = p->Value().AsInterface();
    CleanupStack::PushL(TCleanupItem(InterfaceCleanup, ifp));
  }
  else
    User::Leave(KErrNotFound);
  
  outps->Reset();
  inps->Reset();
  _LIT8(KUid,"uid");
  _LIT8(KCreateImpl,"CreateImplementation");
  
  inps->AppendL(TLiwGenericParam(KUid, TLiwVariant(aImplUid)));
  ifp->ExecuteCmdL(KCreateImpl, *inps, *outps);
  
  pos = 0;
  p = outps->FindFirst(pos, EGenericParamError);
  User::LeaveIfError(p->Value().AsTInt32());

  pos = 0;
  CLiwServiceIfBase* iface = NULL;
  _LIT8(KImplPtr, "impl_ptr");
  p = outps->FindFirst(pos, KImplPtr, EVariantTypeDesC8);
  if (p) {
    TPtrC8 buf = p->Value().AsData();
    if (buf.Size() == sizeof(iface))
      Mem::Copy(&iface, buf.Ptr(), buf.Size());
    else
      User::Leave(KErrNotFound);
  }
  else
    User::Leave(KErrNotFound);

  CleanupStack::Pop(ifp); // ifp
  ifp->Close();

  return iface;
}

void CLiwServiceHandlerImpl::ListProvidersForCriteriaL(RArray<TInt>& aResult, 
    CLiwCriteriaItem& aItem)
    {
    TInt i;

    for (i = 0; i < iProviders.Count(); i++)
        {   
        if (iProviders[i]->HasCriteria(aItem))
            {
            User::LeaveIfError(aResult.Append(iProviders[i]->ImplementationUid().iUid));
            }
        }
    }



TInt CLiwServiceHandlerImpl::NbrOfProviders(const CLiwCriteriaItem* aCriteria)
    {
    if(!aCriteria)
        {
        return 0;
        }
    
    TInt i, j;

    for (i = 0; i < iBaseBindings.Count(); i++)
        {
        for (j = 0; j < iBaseBindings[i]->Interest().Count(); j++)
            {
            if ((*iBaseBindings[i]->Interest()[j]) == (*aCriteria))
                {
                return iBaseBindings[i]->NumberOfProviders();
                }
            }
        }

    for (i = 0; i < iMenuBindings.Count(); i++)
        {
        for (j = 0; j < iMenuBindings[i]->Interest().Count(); j++)
            {
            if ((*iMenuBindings[i]->Interest()[j]) == (*aCriteria))
                {
                return iMenuBindings[i]->NumberOfProviders();
                }
            }       
        }

    return 0;
    }



void CLiwServiceHandlerImpl::AttachL(TInt aInterestResourceId)
    {
    // CCoeEnv/CEikonEnv needs to be accessible.
    if(!iCoeEnv)
        {
        User::Leave(KErrNotSupported);
        }
    
    RCriteriaArray interest, filtered;

    CleanupStack::PushL( TCleanupItem( InterestCleanup, &interest ) );
    CleanupStack::PushL( TCleanupItem( FilteredCleanup, &filtered ) );

    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC(reader, aInterestResourceId);
    ReadInterestListL(reader, interest);
    CleanupStack::PopAndDestroy(); //reader

    FilterInterestListL(interest, filtered);
    
    DoAttachL(filtered,NULL);

    filtered.Reset();

    CleanupStack::Pop(&filtered); // filtered
    CleanupStack::Pop(&interest); // interest
    }



TInt CLiwServiceHandlerImpl::AttachL(const RCriteriaArray& aInterest)
    {
    	return (this->AttachL(aInterest,NULL));
    }

TInt CLiwServiceHandlerImpl::AttachL(const RCriteriaArray& aInterest ,CRTSecMgrScriptSession* aSecMgrScriptSession)
    {
    RCriteriaArray interest, filtered;
    
    CleanupStack::PushL( TCleanupItem( InterestCleanup, &interest ) );
    CleanupStack::PushL( TCleanupItem( FilteredCleanup, &filtered ) );
        
    for(TInt i = 0; i < aInterest.Count(); i++)
        {
        CLiwCriteriaItem* item = CLiwCriteriaItem::NewLC();
        	
        item->SetId(               aInterest[i]->Id()                    );
        if (aInterest[i]->ServiceCmd() == KLiwCmdAsStr)
          item->SetServiceCmdL(    aInterest[i]->ServiceCmdStr()         );
        else
          item->SetServiceCmd(     aInterest[i]->ServiceCmd()            );
        item->SetContentTypeL(     aInterest[i]->ContentType()           );
        item->SetServiceClass(     aInterest[i]->ServiceClass()          );
        
        item->SetOptions(          aInterest[i]->Options()               );
		
		//Setting the imetadataOptions of item
		TLiwVariant metadataOption;
		metadataOption.PushL();
		
        aInterest[i]->GetMetaDataOptions(metadataOption);
		item->SetMetaDataOptions(metadataOption);
		metadataOption.Reset();
				
        item->SetDefaultProvider( (aInterest[i]->DefaultProvider()).iUid );
        item->SetMaxProviders(     aInterest[i]->MaxProviders()          ); 
        
        User::LeaveIfError(interest.Append(item));
        CleanupStack::Pop(&metadataOption); 
        CleanupStack::Pop(item); 
        }
        
    FilterInterestListL(interest, filtered);        

	TInt result = DoAttachL(filtered,aSecMgrScriptSession);

    filtered.Reset();
		
    CleanupStack::Pop(&filtered); // filtered
    CleanupStack::Pop(&interest); // interest
    
    return result;
    }

TInt CLiwServiceHandlerImpl::DoAttachL(const RCriteriaArray& aInterest,CRTSecMgrScriptSession* aSecMgrScriptSession)
    {
    CLiwBinding* bind;
    TInt success = -1;
    for (TInt i = 0; i < aInterest.Count(); i++)
        {
        bind = CLiwBinding::NewLC();
        
        success = ResolveProvidersL(bind, aInterest[i],aSecMgrScriptSession);
        
        if (success == KLiwServiceLoadSuccess)
            {
            User::LeaveIfError(iBaseBindings.Append( bind ));
            CleanupStack::Pop(bind); // bind
            bind->AddCriteriaL(aInterest[i]);

            // Initialise providers.
            for (TInt k = 0; k < bind->NumberOfProviders(); k++)
                {
                // Trap the initialisation. If not done, a leaving provider
                // could prevent the initialisation of other providers.
                TRAPD(err, bind->BaseProvider(k)->InitialiseL(*this, bind->Interest()));
                
                
                
                if(err)
                    {
#ifdef _DEBUG                    
                    RDebug::Print(_L("LIW PROVIDER ERROR: CLiwServiceIfBase::InitialiseL() failed, leave code:%d"), err);
#endif                    
                    }
                }
            }
        else
            {
            CleanupStack::PopAndDestroy(bind); // bind
            }
        }
    	return success; // returns status of 'n'th criteria in interest
    }


void CLiwServiceHandlerImpl::GetInterest(RCriteriaArray& aInterest)
    {
    for (TInt i = 0; i < iInterestList.Count(); i++)
        {
        if (aInterest.Append(iInterestList[i]) != KErrNone)
            {
            return;
            }
        }
    }
    
    

void CLiwServiceHandlerImpl::DetachL(const RCriteriaArray& aInterest)
    {
    // First, remove relevant criteria items from relevat base bindings.
    for (TInt i = 0; i < aInterest.Count(); i++)
        {
        for (TInt j = 0; j < iBaseBindings.Count(); j++)
            {
            TInt index = iBaseBindings[j]->HasCriteriaItem(*aInterest[i]);
            if (index != KErrNotFound)
                {
                iBaseBindings[j]->RemoveCriteria(index);
                }                           
            }
        }

    // Second pass removes empty bindings.
    for (TInt i = 0; i < iBaseBindings.Count(); i++)
        {
        if (iBaseBindings[i]->Interest().Count() == 0)
            {
            delete iBaseBindings[i];
            iBaseBindings.Remove(i);
            i--;
            }
        }

    // Then check if there were left obselete criteria items and remove them.   
    RemoveObsoleteCriteriaItems();
    
    // Finally check if there were left obselete providers and remove them.
    RemoveObsoleteProviders();        
    }



void CLiwServiceHandlerImpl::DetachL(TInt aInterestResourceId)
    { 
    // CCoeEnv/CEikonEnv needs to be accessible.
    if(!iCoeEnv)
        {
        User::Leave(KErrNotSupported);
        }
    
    RCriteriaArray interest;

    CleanupStack::PushL( TCleanupItem( InterestCleanup, &interest ) );

    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC(reader, aInterestResourceId);
    ReadInterestListL(reader, interest);
    CleanupStack::PopAndDestroy(); //reader

    DetachL( interest );

    interest.ResetAndDestroy();
    CleanupStack::Pop(&interest); // interest
    }


const CLiwCriteriaItem* CLiwServiceHandlerImpl::GetCriteria(TInt aId)
    {
    for (TInt i = 0; i < iInterestList.Count(); i++)
        {
        if (iInterestList[i]->Id() == aId)
            {
            return iInterestList[i];
            }
        }

    return NULL;  
    }
    
TInt CLiwServiceHandlerImpl::NumAlreadyInitializedPaneIdsL() const
    {
    TInt ret = 0;
    TInt paneIds[KMaxPaneIds] = {0};
    TBool found = EFalse;
    
    for (TInt i = 0; i < iLastInitialized.Count(); i++)
        {
        found = EFalse;
        
        for (TInt j = 0; j < ret; j++)
            {
            if (iLastInitialized[i]->MenuResourceId() == paneIds[j])
                {
                found = ETrue;
                break;              
                }
            }
                
        if (!found) 
            {
            // Create new item.
            if (ret >= KMaxPaneIds)
                {
#ifdef _DEBUG
                RDebug::Print(_L("ERROR: OVERFLOW in CLiwServiceHandlerImpl::NumAlreadyInitializedPaneIdsL()"));
#endif
                User::Leave(KErrOverflow);
                }
            paneIds[ret] = iLastInitialized[i]->MenuResourceId();
            ret++;
            }                       
        }
    return ret;         
    }    

void CLiwServiceHandlerImpl::InitializeMenuPaneL(
    CEikMenuPane& aMenuPane,
    TInt aMenuResourceId, 
    TInt aBaseMenuCmdId,
    const CLiwGenericParamList& aInParamList)
    {
    InitializeMenuPaneL(aMenuPane, aMenuResourceId, aBaseMenuCmdId, aInParamList, EFalse);    
    }        
        
void CLiwServiceHandlerImpl::InitializeMenuPaneL(
    CEikMenuPane& aMenuPane,
    TInt aMenuResourceId, 
    TInt aBaseMenuCmdId,
    const CLiwGenericParamList& aInParamList,
    TBool aUseSubmenuTextsIfAvailable)
    {        
    // CCoeEnv/CEikonEnv needs to be accessible.
    if(!iCoeEnv)
        {
        User::Leave(KErrNotSupported);
        }
    
    if (!iMenuBindings.Count())
        {
        // Either no menu is attached to interest or menu was attached but
        // it didn't contain any placeholders for criteria items. So
        // nothing to do, get out.
        return;
        }

    TInt index;
    TInt slotcmd;
    TBuf <KMaxMenuTitleSize> subTitle;
    TBool titleLocked;
    TInt paneOffset = NumAlreadyInitializedPaneIdsL() * KIndividualMenuPaneIdRange;

    iSubmenuCmd = aBaseMenuCmdId + KMenuPaneCommandRange;
    slotcmd = SlotItemCmd(aMenuPane);
    if (slotcmd >= 0)
        {
        // aMenuPane is liw submenu. At this point it is empty and we must
        // copy provider menu items to it.
        CLiwMenuPane* liwPane = MenuPaneForSlotCmd(slotcmd);
        if (liwPane)
            {
            CopyMenuItemsL(liwPane, aMenuPane, 0, ETrue);
            aMenuPane.DeleteMenuItem(slotcmd);
            iSubmenu = liwPane;
            }
        }
    else
        {
        iSubmenu = NULL;
        
        const TInt bindcount = iMenuBindings.Count();
        for (TInt i = 0; i < bindcount; i++)
            {
            if  ((iMenuBindings[i]->MenuId() == aMenuResourceId) &&
                (aMenuPane.MenuItemExists(iMenuBindings[i]->MenuCmd(), index)))
                {
                CLiwMenuPane* liwPane = iMenuBindings[i]->MenuPane();
                TInt menuResourceId = -1;
                if(liwPane)
                    {
                    // An LIW menu pane already exists (this means that a normal
                    // non-LIW submenu with LIW items has been opened more than once). 
                    // In this case we use the existing resource slot id.
                    menuResourceId = liwPane->ResourceSlotId();
                    paneOffset = liwPane->PaneOffset();
                    DeleteLiwMenuPane(liwPane);
                    liwPane = NULL;
                    }
                liwPane = CreateEmptyLiwMenuPaneL(aBaseMenuCmdId, menuResourceId);
                CleanupStack::PushL(liwPane);
                liwPane->SetPaneOffset(paneOffset);
                paneOffset += KPlaceholderCmdIdRange;
                iMenuBindings[i]->SetMenuPane(liwPane);     

                // Clean previous service commands from list.
                CLiwGenericParamList& list = const_cast<CLiwGenericParamList&>(aInParamList);
                while (list.Remove(EGenericParamServiceCommand)) 
                    {
                    // Intentionally left empty.    
                    }

                // Add service commands for current placeholder.
                const TInt icount = iMenuBindings[i]->Interest().Count();
                for (TInt k = 0; k < icount; k++)
                    {
                    list.AppendL(TLiwGenericParam(EGenericParamServiceCommand,
                        TLiwVariant(iMenuBindings[i]->Interest()[k]->ServiceCmd())));
                    }

                // Loop from last entry to first entry always inserting to same index.
                // Default provider is the first item in list, so if there is a default
                // provider defined, it will be the first one to appear in menus.               
                for (TInt j = iMenuBindings[i]->NumberOfProviders() - 1; j >= 0; j--)
                    {
                    liwPane->SetInitializingOwner(iMenuBindings[i]->MenuProvider(j));
                    iMenuBindings[i]->MenuProvider(j)->InitializeMenuPaneHookL(liwPane,
                        0, 0, aInParamList);
                    }

                GetSubmenuTitle(liwPane->MenuPane(), subTitle);
            
                TLiwPlaceholderType phtype = PlaceholderType(aMenuPane, 
                    iMenuBindings[i]->MenuCmd(), titleLocked);

                if ((phtype == ELiwPlaceholderCascade) ||
                    (phtype == ELiwPlaceholderIntelligentCascade))
                    {
                    if (liwPane->MenuPane().NumberOfItemsInPane() == 1)
                        {
                        // Remove placeholder item.
                        aMenuPane.DeleteMenuItem(iMenuBindings[i]->MenuCmd());
                        CleanupStack::PopAndDestroy(liwPane); // liwPane
                        continue;
                        }
                    else if ((liwPane->MenuPane().NumberOfItemsInPane() == 2) &&
                        (phtype == ELiwPlaceholderIntelligentCascade))
                        {
                        UnCascadeL(aMenuPane, iMenuBindings[i]->MenuCmd(), *liwPane);
                        User::LeaveIfError(iLastInitialized.Append(liwPane));
                        }
                    else
                        {
                        if (titleLocked)
                            {
                            subTitle.Zero();
                            }
                        ConvertPlaceholderL(aMenuPane, iMenuBindings[i]->MenuCmd(), *liwPane, 
                            subTitle);
                        }       
                    }
                else
                    {
                    // Remove placeholder item.
                    aMenuPane.DeleteMenuItem(iMenuBindings[i]->MenuCmd());
        
                    // Copy menu items to actual menu pane
                    CopyMenuItemsL(liwPane, aMenuPane, index, aUseSubmenuTextsIfAvailable);
                    User::LeaveIfError(iLastInitialized.Append(liwPane));
                    }
                liwPane->SetMenuResourceId(aMenuResourceId);
                User::LeaveIfError(iMenuPanes.Append(liwPane));
                CleanupStack::Pop(liwPane); // liwPane
                }
            }
        }
    }




TInt CLiwServiceHandlerImpl::ServiceCmdByMenuCmd(TInt aMenuCmdId) const
    {
    for (TInt i = 0; i < iMenuBindings.Count(); i++)
        {
        if ((IsInLastInitialized(iMenuBindings[i]->MenuPane())) &&
            (iMenuBindings[i]->MenuPane()->IsCmdInRange(KPlaceholderCmdIdRange, aMenuCmdId)))
            {
            return iMenuBindings[i]->MenuPane()->ServiceCmdId(aMenuCmdId); 
            }
        }

    return 0;   
    }



void CLiwServiceHandlerImpl::ExecuteMenuCmdL(
    TInt aMenuCmdId,
    const CLiwGenericParamList& aInParamList,
    CLiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    MLiwNotifyCallback* aCallback)
    {
    // CCoeEnv/CEikonEnv needs to be accessible.
    if(!iCoeEnv)
        {
        User::Leave(KErrNotSupported);
        }
    
    if (!iMenuBindings.Count())
        {
        return;
        }   

    // Handle real menu providers.
    for (TInt i = 0; i < iMenuBindings.Count(); i++)
        {
        CLiwMenuPane* menuPane = iMenuBindings[i]->MenuPane();

        if (IsInLastInitialized(menuPane))
            {
            for (TInt j = 0; j < iMenuBindings[i]->NumberOfProviders(); j++)
                {
                if ((menuPane->IsCmdInRange(KPlaceholderCmdIdRange, aMenuCmdId)) && 
                    (menuPane->CommandOwner(aMenuCmdId) == iMenuBindings[i]->MenuProvider(j)))
                    {
                    iMenuBindings[i]->MenuProvider(j)->HandleMenuCmdHookL(
                        menuPane, 
                        aMenuCmdId, 
                        aInParamList, 
                        aOutParamList, 
                        aCmdOptions, 
                        aCallback); 
                    return;
                    }
                }
            }
        }
    }



void CLiwServiceHandlerImpl::AttachMenuL(TInt aMenuResourceId, TInt aInterestResourceId)
    {
    // CCoeEnv/CEikonEnv needs to be accessible.
    if(!iCoeEnv)
        {
        User::Leave(KErrNotSupported);
        }
    
    RCriteriaArray interest, filtered;
    TResourceReader reader;

    CleanupStack::PushL( TCleanupItem( InterestCleanup, &interest ) );
    CleanupStack::PushL( TCleanupItem( FilteredCleanup, &filtered ) );
    iCoeEnv->CreateResourceReaderLC(reader, aInterestResourceId);
    ReadInterestListL(reader, interest);
    CleanupStack::PopAndDestroy(); //reader
    FilterInterestListL(interest, filtered);

    iCoeEnv->CreateResourceReaderLC(reader, aMenuResourceId);
    DoAttachMenuL(reader, aMenuResourceId, filtered);
    filtered.Reset();
    CleanupStack::PopAndDestroy(); //reader
    CleanupStack::Pop(&filtered); // filtered
    CleanupStack::Pop(&interest); // interest
    }



void CLiwServiceHandlerImpl::AttachMenuL(TInt aMenuResourceId, TResourceReader& aReader)
    {
    // CCoeEnv/CEikonEnv needs to be accessible.
    if(!iCoeEnv)
        {
        User::Leave(KErrNotSupported);
        }
    
    RCriteriaArray interest, filtered;
    TResourceReader reader;

    CleanupStack::PushL( TCleanupItem( InterestCleanup, &interest ) );
    CleanupStack::PushL( TCleanupItem( FilteredCleanup, &filtered ) );
    ReadInterestListL(aReader, interest);
    FilterInterestListL(interest, filtered);

    iCoeEnv->CreateResourceReaderLC(reader, aMenuResourceId);
    DoAttachMenuL(reader, aMenuResourceId, filtered);
    filtered.Reset();
    CleanupStack::PopAndDestroy(); //reader
    CleanupStack::Pop(&filtered); // filtered
    CleanupStack::Pop(&interest); // interest
    }

void CLiwServiceHandlerImpl::AttachMenuL(TInt aMenuResourceId, const RCriteriaArray& aInterest)
    {
    // CCoeEnv/CEikonEnv needs to be accessible.
    if(!iCoeEnv)
        {
        User::Leave(KErrNotSupported);
        }    
    
    RCriteriaArray interest, filtered;
    TResourceReader reader;
    
    CleanupStack::PushL( TCleanupItem( InterestCleanup, &interest ) );
    CleanupStack::PushL( TCleanupItem( FilteredCleanup, &filtered ) );
        
    for(TInt i = 0; i < aInterest.Count(); i++)
        {
        CLiwCriteriaItem* item = CLiwCriteriaItem::NewLC();
        
        item->SetId(               aInterest[i]->Id()                    );
        item->SetServiceCmd(       aInterest[i]->ServiceCmd()            );
        item->SetContentTypeL(     aInterest[i]->ContentType()           );
        item->SetServiceClass(     aInterest[i]->ServiceClass()          );
        item->SetOptions(          aInterest[i]->Options()               );
        item->SetDefaultProvider( (aInterest[i]->DefaultProvider()).iUid );
        item->SetMaxProviders(     aInterest[i]->MaxProviders()          );       
        
        User::LeaveIfError(interest.Append(item));
        CleanupStack::Pop(item); 
        }
        
    FilterInterestListL(interest, filtered);        

    iCoeEnv->CreateResourceReaderLC(reader, aMenuResourceId);
    DoAttachMenuL(reader, aMenuResourceId, filtered);
    filtered.Reset();
    CleanupStack::PopAndDestroy(); //reader
    CleanupStack::Pop(&filtered); // filtered
    CleanupStack::Pop(&interest); // interest    
    }   

void CLiwServiceHandlerImpl::AttachMenuL(RArray<TInt>& aMenuEntries,
                                         TInt aMenuResourceId,
                                         RCriteriaArray& aInterest)
    {
    RCriteriaArray filtered;

    CleanupStack::PushL( TCleanupItem( FilteredCleanup, &filtered ) );
    FilterInterestListL(aInterest, filtered);
    
    TInt menuCmd;
    TInt count = aMenuEntries.Count();
    TBool bound;

    for (TInt i = 0; i < count; i++)
        {
        menuCmd = aMenuEntries[i];
        CLiwMenuBinding* bind = NULL;
        bound = EFalse;

        if (!menuCmd)
          continue;

        for (TInt j = 0; j < filtered.Count(); j++)
            {           
            if (filtered[j]->Id() == menuCmd)
                {                       
                if (!bind)
                    {
                    bind = AlreadyBound(aMenuResourceId, menuCmd, i);
                    if (!bind)
                        {
                        bind = CLiwMenuBinding::NewLC(i, aMenuResourceId);
                        bind->SetMenuCmd( menuCmd );
                        }
                    else
                        {
                        bound = ETrue;
                        }
                    }
                
                if (bind->HasCriteriaItem(*(filtered[j])) == KErrNotFound)
                    {
                    	ResolveProvidersL(bind, filtered[j],NULL);
                    bind->AddCriteriaL(filtered[j]);
                    }
                }
            }

        // Initialise providers.
        if (bind)
            {
            for (TInt k = 0; k < bind->NumberOfProviders(); k++)
                {
                bind->MenuProvider(k)->InitialiseL(*this, bind->Interest());
                }
            if (!bound)
                {
                User::LeaveIfError(iMenuBindings.Append( bind ));
                CleanupStack::Pop(bind);  // bind                       
                }
            }
        }

    filtered.Reset();
    CleanupStack::Pop(&filtered); // filtered
    }

void CLiwServiceHandlerImpl::DoAttachMenuL(TResourceReader& aReader, TInt aMenuId, 
    RCriteriaArray& aInterest)
    {
    TInt menuCmd;
    TInt count = aReader.ReadInt16();
    TBool bound;

    for (TInt i = 0; i < count; i++)
        {
        menuCmd = aReader.ReadInt32();
        CLiwMenuBinding* bind = NULL;
        bound = EFalse;

        for (TInt j = 0; j < aInterest.Count(); j++)
            {           
            if (aInterest[j]->Id() == menuCmd)
                {                       
                if (!bind)
                    {
                    bind = AlreadyBound(aMenuId, menuCmd, i);
                    if (!bind)
                        {
                        bind = CLiwMenuBinding::NewLC(i, aMenuId);
                        bind->SetMenuCmd( menuCmd );
                        }
                    else
                        {
                        bound = ETrue;
                        }
                    }
                
                if (bind->HasCriteriaItem(*(aInterest[j])) == KErrNotFound)
                    {
                    	ResolveProvidersL(bind, aInterest[j],NULL);
                    bind->AddCriteriaL(aInterest[j]);
                    }
                }
            }

        // Initialise providers.
        if (bind)
            {
            for (TInt k = 0; k < bind->NumberOfProviders(); k++)
                {
                TRAPD(err, bind->MenuProvider(k)->InitialiseL(*this, bind->Interest()));
                if(err)
                    {
#ifdef _DEBUG
                    RDebug::Print(_L("LIW PROVIDER ERROR: CLiwServiceIfMenu::InitialiseL() failed, leave code:%d"), err);
#endif                     
                    // The provider failed to initialise.
                    // Remove the failed provider from this menu binding.
                    CLiwServiceIfMenu* provider = bind->MenuProvider(k);
                    TInt implUid = provider->ImplementationUid().iUid;
                    bind->RemoveProvider(implUid);
                    
                    // Remove the failed provider also from other menu bindings.
                    for (TInt m = 0; m < iMenuBindings.Count(); m++)
                        {
                        iMenuBindings[m]->RemoveProvider(implUid);
                        }

                    // Then remove provider from the owner list and delete it.
                    for (TInt m = 0; m < iProviders.Count(); m++)
                        {
                        if (iProviders[m]->ImplementationUid().iUid == implUid)
                            {
                            delete iProviders[m];
                            iProviders.Remove(m);
                            m--;
                            }
                        }                    
                    }
                }
            if (!bound)
                {
                User::LeaveIfError(iMenuBindings.Append( bind ));
                CleanupStack::Pop(bind);  // bind                       
                }
            }
        SkipMenuFields(aReader);  // Jump to next menu item
        }
    }

void CLiwServiceHandlerImpl::ReadInterestL(RCriteriaArray& aInterest, TInt aInterestResourceId)
    {
    CleanupStack::PushL( TCleanupItem( InterestCleanup, &aInterest ) );
    TResourceReader reader;
    iCoeEnv->CreateResourceReaderLC(reader, aInterestResourceId);
    ReadInterestListL(reader, aInterest);
    CleanupStack::PopAndDestroy(); //reader
    CleanupStack::Pop(&aInterest);
    }


void CLiwServiceHandlerImpl::DetachMenu(TInt aMenuResourceId, TInt aInterestResourceId)
    {
    // If interest resource id is null, then detach all items in the given menu.
    if (!aInterestResourceId)
        {
        DoDetachMenu(aMenuResourceId);        
        }
    else
        {
        // CCoeEnv/CEikonEnv needs to be accessible.
        if(!iCoeEnv)
            {
            // We cannot leave because this is a non-leaving method.
            return; 
            }    

        RCriteriaArray interest;
        TRAPD(err, ReadInterestL(interest, aInterestResourceId));
        if (err)
            {
            return;
            }
        
        DoDetachMenu(aMenuResourceId, interest);
        
        interest.ResetAndDestroy();
        }
    }
        

void CLiwServiceHandlerImpl::DoDetachMenu(TInt aMenuResourceId)
    {
    // First, delete the relevant menu bindings.
    for (TInt i = 0; i < iMenuBindings.Count(); i++)
        {
        if (iMenuBindings[i]->MenuId() == aMenuResourceId)
            {
            delete iMenuBindings[i];
            iMenuBindings.Remove(i);
            i--;
            }
        }

    // Then check if there were left obselete criteria items and remove them.   
    RemoveObsoleteCriteriaItems();
    
    // Finally check if there were left obselete providers and remove them.
    RemoveObsoleteProviders();  
    }

    
void CLiwServiceHandlerImpl::DoDetachMenu(TInt aMenuResourceId, RCriteriaArray& aInterest)
    {
    // First, remove relevant criteria items from relevant menu bindings.
    for (TInt i = 0; i < iMenuBindings.Count(); i++)
        {
        if (iMenuBindings[i]->MenuId() == aMenuResourceId) 
            {
            for (TInt j = 0; j < aInterest.Count(); j++)
                {
                TInt index = iMenuBindings[i]->HasCriteriaItem(*aInterest[j]);
                if (index != KErrNotFound)
                    {
                    iMenuBindings[i]->RemoveCriteria(index);
                    }
                }
            }
        }

    // Second pass removes empty bindings.
    for (TInt i = 0; i < iMenuBindings.Count(); i++)
        {
        if (iMenuBindings[i]->Interest().Count() == 0)
            {
            delete iMenuBindings[i];
            iMenuBindings.Remove(i);
            i--;
            }
        }
    
    // Then check if there were left obselete criteria items and remove them.   
    RemoveObsoleteCriteriaItems();
    
    // Finally check if there were left obselete providers and remove them.
    RemoveObsoleteProviders();        
    }    
    
    
void CLiwServiceHandlerImpl::RemoveObsoleteCriteriaItems()
    {
    for (TInt i = 0; i < iInterestList.Count(); i++)
        {
        CLiwCriteriaItem* criteria = iInterestList[i];
        TBool found = EFalse;
        
        // Loop through base bindings.
        for (TInt j = 0; j < iBaseBindings.Count(); j++)
            {
            if (iBaseBindings[j]->HasCriteriaItem(*criteria) != KErrNotFound)
                {
                found = ETrue;
                break;
                }
            }

        // If still not found, loop through menu bindings.        
        if (!found)
            {
            for (TInt j = 0; j < iMenuBindings.Count(); j++)
                {
                if (iMenuBindings[j]->HasCriteriaItem(*criteria) != KErrNotFound)
                    {
                    found = ETrue;
                    break;
                    }
                }            
            }
            
        // Criteria item can be deleted if it was not found.            
        if (!found)
            {
            delete iInterestList[i];
            iInterestList.Remove(i);
            i--;
            }
        }    
    }
    
    
void CLiwServiceHandlerImpl::RemoveObsoleteProviders()
    {
    for (TInt i = 0; i < iProviders.Count(); i++)
        {
        CLiwServiceIfBase* provider = iProviders[i];
        TBool found = EFalse;
        
        // Loop through base bindings.
        for (TInt j = 0; j < iBaseBindings.Count(); j++)
            {
            if (iBaseBindings[j]->HasProvider(provider))
                {
                found = ETrue;
                break;
                }
            }

        // If still not found, loop through menu bindings.        
        if (!found)
            {
            for (TInt j = 0; j < iMenuBindings.Count(); j++)
                {
                if (iMenuBindings[j]->HasProvider(provider))
                    {
                    found = ETrue;
                    break;
                    }
                }            
            }
            
        // Criteria item can be deleted if it was not found.            
        if (!found)
            {
            delete iProviders[i];
            iProviders.Remove(i);
            i--;
            }
        }    
    }


TBool CLiwServiceHandlerImpl::IsSubMenuEmpty(TInt aSubMenuId)
    {
    for (TInt i = 0; i < iMenuBindings.Count(); i++)
        {
        if (iMenuBindings[i]->MenuId() == aSubMenuId)
            {
            if (iMenuBindings[i]->NumberOfProviders() > 0)
                {
                return EFalse;
                }

            return ETrue;
            }
        }

    return EFalse;
    }




CLiwMenuBinding* CLiwServiceHandlerImpl::AlreadyBound(TInt aMenuId, TInt aMenuCmd, 
    TInt aMenuItemIndex) const
    {
    for (TInt i = 0; i < iMenuBindings.Count(); i++)
        {
        if ((iMenuBindings[i]->MenuId() == aMenuId) &&
            (iMenuBindings[i]->MenuCmd() == aMenuCmd) &&
            (iMenuBindings[i]->MenuItemIndex() == aMenuItemIndex))
            {
            return iMenuBindings[i];
            }
        }

    return NULL;
    }


void CLiwServiceHandlerImpl::ExecuteServiceCmdL(
    const TInt& aCmdId,
    const CLiwGenericParamList& aInParamList,
    CLiwGenericParamList& aOutParamList,
    TUint aCmdOptions,
    MLiwNotifyCallback* aCallback)
    {
    for (TInt i = 0; i < iBaseBindings.Count(); i++)
        {
        if(iBaseBindings[i]->HasServiceCmd(aCmdId))
            {
        for (TInt j = 0; j < iBaseBindings[i]->NumberOfProviders(); j++)
            {
            iBaseBindings[i]->BaseProvider(j)->HandleServiceCmdL(aCmdId,
                            aInParamList, aOutParamList, aCmdOptions, aCallback);
            }
        }
	}
}

void CLiwServiceHandlerImpl::ExecuteServiceCmdL(    
        const CLiwCriteriaItem& aCmd,
        const CLiwGenericParamList& aInParamList,
        CLiwGenericParamList& aOutParamList,
        TUint aCmdOptions,
        MLiwNotifyCallback* aCallback)
    {
      for (TInt i = 0; i < iBaseBindings.Count(); i++) {
        for (TInt k = 0; k < iBaseBindings[i]->Interest().Count(); k++) {
          if ((iBaseBindings[i]->Interest()[k]->ServiceCmd() == KLiwCmdAsStr) &&
              (aCmd.ServiceCmd() == KLiwCmdAsStr) &&
              (iBaseBindings[i]->Interest()[k]->ServiceCmdStr() == aCmd.ServiceCmdStr()) &&
              (iBaseBindings[i]->Interest()[k]->ContentType() == aCmd.ContentType()))
            {
              //call only one provider
              if (iBaseBindings[i]->NumberOfProviders() > 0) {
                iBaseBindings[i]->BaseProvider(0)->HandleServiceCmdL(aCmd.ServiceCmdStr(),
                  aInParamList, aOutParamList, aCmdOptions, aCallback);
              }
            }
        }
      }
    }

void CLiwServiceHandlerImpl::ReadInterestListL(TResourceReader& aReader, 
    RPointerArray<CLiwCriteriaItem>& aResult)  
    {
    const TInt count = aReader.ReadInt16();
    for (TInt ii = 0; ii < count; ++ii)
        {
        CLiwCriteriaItem* item = CLiwCriteriaItem::NewLC();
        item->ReadFromResoureL( aReader );
        User::LeaveIfError(aResult.Append(item));
        CleanupStack::Pop(item); // item
        }
    }

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
void CLiwServiceHandlerImpl::ParseMetaData(const TDesC8& aOpaque, TDes8& aMetaData)
    {
    
       _LIT8(serviceCmdSeparator, "::");
       const TInt metaDataStartPos=2;
       
       TInt separatorPos = aOpaque.Find(serviceCmdSeparator);
        
        if (separatorPos != KErrNotFound)
        {
        	// Find the first section, up to the separator
        	separatorPos += metaDataStartPos;
        	aMetaData.Copy(aOpaque.Mid(separatorPos,aOpaque.Length()-separatorPos));        
        }
    }

/* 
	QueryImplementationL finds the providers that match the given criteria item that is passed as the parameters
*/
void CLiwServiceHandlerImpl::QueryImplementationL(CLiwCriteriaItem* aItem, RCriteriaArray& aProviderList)
	{
    RImplInfoPtrArray infoArray;
	_LIT8(KWild,"*");

    if(0 == aItem->ContentType().Compare(KNullDesC8))
    	aItem->SetContentTypeL(KWild);
    
	if(0 == aItem->ServiceCmdStr().Compare(KNullDesC8))
		aItem->SetServiceCmdL(KWild);
    
    //to fetch the infoArray
    QueryImplementationL(aItem,infoArray);
    
   	CLiwXmlHandler* pXmlHandler = CLiwXmlHandler::NewLC();
		
	CLiwServiceData* pServiceData = NULL;
	
	TInt versionCheck = 0;
	
	for (TInt index = 0; index < infoArray.Count(); ++index)
    	{
        if ((aItem->Options() & LIW_OPTIONS_ROM_ONLY) && (infoArray[index]->RomBased() == EFalse))
           	{
            continue;
            }

		CImplementationInformation* pImplInfo = infoArray[index];
		const TInt separatorLength = KDataSeparator().Length();//find "||"		
		TInt separatorPos = pImplInfo->OpaqueData().Find(KDataSeparator);
		
		TInt leftExtractPos = 0;
		TBool separatorFound = EFalse;
		
		do
		{
			//Constructing the Criteria Item and appending to providerList
			CLiwCriteriaItem* item = CLiwCriteriaItem::NewLC(); //iCriteriaId & iServiceCmd are ignored...
			item->SetServiceClass(TUid::Uid(KLiwClassBase));
			item->SetContentTypeL(infoArray[index]->DataType());
			
			TBuf8<KMaxLength> sName;    		
			TBuf8<KMaxLength> opaq;
			
			if(separatorPos != KErrNotFound && (leftExtractPos < separatorPos))
			{
				separatorFound = ETrue;
				sName = pImplInfo->OpaqueData().Mid(leftExtractPos, separatorPos-leftExtractPos); //ServiceName					
				leftExtractPos = separatorLength + separatorPos;
		 	 	TPtrC8 remainingData = pImplInfo->OpaqueData().Mid(separatorPos + separatorLength);	
        	 	separatorPos = remainingData.Find(KDataSeparator) + separatorPos + separatorLength;	
			}
			else
			{
				separatorFound = EFalse;
				TInt mDataSepPos = pImplInfo->OpaqueData().Find(serviceCmdSeparator);
				if(mDataSepPos != KErrNotFound)
					sName = pImplInfo->OpaqueData().Mid(leftExtractPos, mDataSepPos - leftExtractPos);
				else
					sName = pImplInfo->OpaqueData().Mid(leftExtractPos);
			}

		   	//check for wildcard character *
		   	//if yes, return immediatly
			if(0 == aItem->ServiceCmdStr().Compare(sName) || 0 == aItem->ServiceCmdStr().Compare(KWild))
			{
				//parse metadata and if metadata valid,
				//setmetadataoptions to criteriaitem..
				pServiceData = CLiwServiceData::NewLC();  
				TInt loadStatus = CLiwXmlHandler::ESrvDataLoadFailed;
				
				ParseMetaData(pImplInfo->OpaqueData(),opaq);
				
				//Inline metadata defined
				if(opaq.Length()>0)
				{
					loadStatus=pXmlHandler->LoadServiceData(opaq,pServiceData);
				}
				else
				{
					//Obtain the capabilities from the metadata
		    		TUid implUid = pImplInfo->ImplementationUid();  
		    		TUidName srvProvUid = implUid.Name();        
		    		
		    		TPtrC16 ptrSrv = srvProvUid.Right(srvProvUid.Length()-1);      
		    		TPtrC16 srvFile = ptrSrv.Left(ptrSrv.Length()-1);		       
		    		TDriveUnit driveUnit = pImplInfo->Drive();
		    		TFileName fileName = driveUnit.Name();
		    		fileName.Append(KDC_RESOURCE_FILES_DIR);
		    		fileName.Append(srvFile);
		    		fileName.Append(KPerExtension);
		    		loadStatus=pXmlHandler->LoadServiceData(fileName,pServiceData);
				}
				
				if(CLiwXmlHandler::ESrvDataLoadSuccess==loadStatus)
				{	    		
					versionCheck = 0;
					
	    			TReal implVersion(KDefVersion);
	    			this->ComputeIntfVersion(pServiceData,implVersion); //fetch impl version..
	    			
	    			TReal minVer(KDefVersion);
	    			TReal maxVer(KUnspVersion);
	    			
	    			if(this->GetVersionRange(aItem,minVer,maxVer)) //Get version queried by consumer
	    			{
	    				if(minVer == KUnspVersion)
	    					minVer = KDefVersion;
	    				
	    				//perform comparison...
    					if(maxVer!=KUnspVersion)
    					{
	    					if((implVersion>=minVer) && (implVersion<=maxVer))
	    					{
	    						versionCheck = 1;
	    						//current impl is the best choice..this is THE CHOSEN ONE..
	    					}
    					}
    					else
    					{
    						//means maxVer == KUnspVersion
    						if(implVersion>=minVer)
	    					{
	    						versionCheck = 1;
	    						
	    						//current impl is the best choice..this is THE CHOSEN ONE..
	    				 	}
    					}
					}
				}
				else //means no metadata information
				{
					versionCheck = 1;
				}
				 
				if(versionCheck) 
				{
					//Since version matches, this item is appended to the providerList
					//before which the metadata information is SET
					CLiwMap* metadataMap = CLiwDefaultMap::NewLC();
					
					CLiwGenericParamList* pMetaData = pServiceData->GetMetaData();    					
					
					for(TInt mdataIdx(0); mdataIdx<pMetaData->Count(); ++mdataIdx)
					{
					    TLiwGenericParam param;	    			    
						pMetaData->AtL(mdataIdx,param);						
						metadataMap->InsertL(param.Name(),param.Value());						
						param.Reset();
					}
										
					TLiwVariant mdataVar(metadataMap);
					mdataVar.PushL();
					
					item->SetMetaDataOptions(mdataVar);
					
					item->SetServiceCmdL(sName);
					CleanupStack::Pop(&mdataVar);
					mdataVar.Reset();
					
					CleanupStack::Pop(metadataMap);
					metadataMap->DecRef();
					                     
					User::LeaveIfError(aProviderList.Append(item));
				}
				
				if(pServiceData)
				 		pServiceData->CleanUpMetaData();
				
				CleanupStack::PopAndDestroy(pServiceData); //pServiceData::CLiwServiceData*
				CleanupStack::Pop(item); //item::CLiwCriteriaItem*
			}
			else
			{
				CleanupStack::PopAndDestroy(item); //item::CLiwCriteriaItem*
			}
		}while(separatorFound);
	}// end of outer for loop

    CleanupStack::PopAndDestroy(pXmlHandler); //pXmlHandler::CLiwXMLHandler*
    infoArray.ResetAndDestroy();
	}


void CLiwServiceHandlerImpl::QueryImplementationL(CLiwCriteriaItem* aItem, RImplInfoPtrArray& infoArray)
	{
	iEcomMonitor->ListImplemetationsL(infoArray, (CLiwCriteriaItem*)aItem);
	}


void CLiwServiceHandlerImpl::FilterInfoArray(RImplInfoPtrArray& aArray,
                                             RArray<TInt32>& aArrayPlugin,
                                             CLiwCriteriaItem* aItem)
    {
    if (aItem->MaxProviders() <= 0)
        {
        aArray.ResetAndDestroy();
        aArrayPlugin.Reset();
        }
    else
        {
        while ((aArray.Count() + aArrayPlugin.Count()) > aItem->MaxProviders())
            {
            if (aArrayPlugin.Count() == 0)
              break;
            // Skip default provider.
            if (aArrayPlugin[0] == aItem->DefaultProvider())
                {
                if (aArrayPlugin.Count() == 1)
                  break;
                aArrayPlugin.Remove(1);
                }
            else
                {
                aArrayPlugin.Remove(0);               
                }
            }

        while ((aArray.Count() + aArrayPlugin.Count()) > aItem->MaxProviders())
            {
            // Skip default provider.
            if (aArray[0]->ImplementationUid() == aItem->DefaultProvider())
                {
                delete aArray[1];
                aArray.Remove(1);               
                }
            else
                {
                delete aArray[0];
                aArray.Remove(0);               
                }
            }
        }
    }



TBool CLiwServiceHandlerImpl::IsCached(CLiwServiceIfBase* /*aProvider*/)
    {
    return EFalse;
    }


CLiwGenericParamList& CLiwServiceHandlerImpl::InParamListL()
    {
    if (!iInParams)
        {
        iInParams = CLiwGenericParamList::NewL();
        }
    iInParams->Reset();
    return *iInParams;
    }



CLiwGenericParamList& CLiwServiceHandlerImpl::OutParamListL()
    {
    if (!iOutParams)
        {
        iOutParams = CLiwGenericParamList::NewL();
        }
    iOutParams->Reset();
    return *iOutParams;
    }



TBool CLiwServiceHandlerImpl::IsInLastInitialized(CLiwMenuPane* liwPane) const
    {
    if (liwPane)
        {
        if (iSubmenu == liwPane)
            {
            return ETrue;       
            }

        for (TInt i = 0; i < iLastInitialized.Count(); i++)
            {
            if (iLastInitialized[i] == liwPane)
                {
                return ETrue;
                }
            }
        }

    return EFalse;
    }


TInt CLiwServiceHandlerImpl::HandleNotifyL(
    TInt /*aCmdId*/,
    TInt /*aEventId*/,
    CLiwGenericParamList& /*aEventParamList*/,
    const CLiwGenericParamList& /*aInParamList*/)
    {
    return KErrNone;    
    }


// CEikMenuPane::ConstructFromresourceL is defined as 'protected' so
// we have to use a wrapper class for accessing it.
class CLiwMenuResource : public CEikMenuPane
    {
    public:
        CLiwMenuResource() : CEikMenuPane(NULL) {}
        CLiwMenuResource(MEikMenuObserver* aObserver) : CEikMenuPane(aObserver) {}

        void CreateL(TResourceReader& aReader)
            {
            ConstructFromResourceL(aReader);
            }
    };


CLiwMenuPane* CLiwServiceHandlerImpl::CreateEmptyLiwMenuPaneL(TInt aBaseMenuCmdId, 
    TInt aResourceId)
    {
    CLiwMenuPane* result = NULL;
    TResourceReader reader; 
    
    TInt id;
    if(aResourceId >= 0)
        {
        // Use existing id.
        id = aResourceId;
        }
    else
        {
        // Create new id.
        id = ResourceIdForNextFreeSlot();
        if (id < 0)
            {
            User::Leave(KErrOverflow);
            }
        }
    iCoeEnv->CreateResourceReaderLC(reader, id);
    
    CLiwMenuResource* pane = new (ELeave) CLiwMenuResource(this);
    CleanupStack::PushL(pane);
    pane->ConstructL(NULL);
    pane->CreateL(reader);

    result = new (ELeave) CLiwMenuPane(*pane, aBaseMenuCmdId);
    
    CleanupStack::Pop(pane);
    CleanupStack::PopAndDestroy(); //reader
    
    result->SetResourceSlotId( id );
    
    return result;
    }


void CLiwServiceHandlerImpl::DeleteLiwMenuPane(CLiwMenuPane* aLiwPane)
    {
    delete aLiwPane->iMenuPane;
    aLiwPane->iMenuPane = NULL;

    // Reset iIdMap and extraText.
    for(TInt i = 0; i < aLiwPane->iIdMap.Count(); i++)
        {
        aLiwPane->iIdMap[i].extraText.Close();
        }
    aLiwPane->iIdMap.Reset();
    
    // Remove the liw menu pane from iMenuPanes array.
    for(TInt i = 0; i < iMenuPanes.Count(); i++)
        {
        if(iMenuPanes[i] == aLiwPane)
            {
            iMenuPanes.Remove(i);
            break;                            
            }
        }
    
    // Remove the liw menu pane from iMenuLastInitialized array.
    for(TInt i = 0; i < iLastInitialized.Count(); i++)
        {
        if(iLastInitialized[i] == aLiwPane)
            {
            iLastInitialized.Remove(i);
            break;                            
            }
        }                        
    
    delete aLiwPane;
    aLiwPane = NULL;    
    }

const TInt resourceSlotIds[KMaxMenuResources] =
    {
    R_LIW_EMPTY_MENU_0,
    R_LIW_EMPTY_MENU_1,
    R_LIW_EMPTY_MENU_2,
    R_LIW_EMPTY_MENU_3,
    R_LIW_EMPTY_MENU_4,
    R_LIW_EMPTY_MENU_5,
    R_LIW_EMPTY_MENU_6,
    R_LIW_EMPTY_MENU_7,
    R_LIW_EMPTY_MENU_8,
    R_LIW_EMPTY_MENU_9,
    R_LIW_EMPTY_MENU_10,
    R_LIW_EMPTY_MENU_11,
    R_LIW_EMPTY_MENU_12,
    R_LIW_EMPTY_MENU_13,
    R_LIW_EMPTY_MENU_14,
    R_LIW_EMPTY_MENU_15
    };


TInt CLiwServiceHandlerImpl::ResourceIdForNextFreeSlot()
    {
    if (iNextFreeSlot < KMaxMenuResources)
        {
        return resourceSlotIds[iNextFreeSlot++];
        }

    return -1;
    }


void CLiwServiceHandlerImpl::SetEmphasis(CCoeControl* /*aMenuControl*/,TBool /*aEmphasis*/)
    {
    }


void CLiwServiceHandlerImpl::ProcessCommandL(TInt /*aCommandId*/) 
    {
    }


// Rewrite this method. It doesn't make sense. Variable j is not used at all.    
TInt CLiwServiceHandlerImpl::MenuCmdId(TInt aMenuCmdId) const
    {
    TInt ret( KErrNotFound );
    
    for (TInt i = 0; i < iMenuBindings.Count() && (ret == KErrNotFound); i++)
        {
        for (TInt j = 0; j < iMenuBindings[i]->NumberOfProviders() && (ret == KErrNotFound); j++)
            {
            if ((IsInLastInitialized(iMenuBindings[i]->MenuPane())) &&
                (iMenuBindings[i]->MenuPane()->IsCmdInRange(KPlaceholderCmdIdRange, aMenuCmdId)) &&
                (ret == KErrNotFound ))
                {
                ret = iMenuBindings[i]->MenuPane()->MenuCmdId(aMenuCmdId); 
                }   
            }
        }

    return ret;     
    }


void Cleanup( TAny* aAny )
    {
    RImplInfoPtrArray* implArray = 
        reinterpret_cast< RImplInfoPtrArray*> ( aAny );
    implArray->ResetAndDestroy();
    implArray->Close();
    }


void InterestCleanup( TAny* aAny )
    {
    RPointerArray<CLiwCriteriaItem>* interestArray = 
        reinterpret_cast<RPointerArray<CLiwCriteriaItem>*> ( aAny );

    interestArray->ResetAndDestroy();   
    }

void FilteredCleanup( TAny* aAny )
    {
    RPointerArray<CLiwCriteriaItem>* filteredArray = 
        reinterpret_cast<RPointerArray<CLiwCriteriaItem>*> ( aAny );

    filteredArray->Reset();   
    }


void IntArrayCleanup(TAny* aAny)
    {
    RArray<TInt>* intArray = 
        reinterpret_cast<RArray<TInt>*> ( aAny );

    intArray->Close();
    }

void Int32ArrayCleanup(TAny* aAny)
    {
    RArray<TInt32>* intArray = 
        reinterpret_cast<RArray<TInt32>*> ( aAny );

    intArray->Close();
    }

void InterfaceCleanup( TAny* aAny )
    {
    MLiwInterface* interface = reinterpret_cast<MLiwInterface*>(aAny);
    interface->Close();
    }

void CLiwServiceHandlerImpl::CopyMenuItemsL(CLiwMenuPane* aSource, CEikMenuPane& aDest, 
    TInt aStartIndex, TBool aIsSubmenu)
    {
    TInt cmdId;
    TInt inPos = aStartIndex;

    for (TInt i = 0; i < aSource->MenuPane().NumberOfItemsInPane(); i++)
        {
        cmdId = aSource->FindCmdId(i);
        if (cmdId >= 0)
            {
            CEikMenuPaneItem::SData itemData = aSource->MenuPane().ItemData(cmdId);
            
            // The menu item might include alternative texts for a main menu level 
            // and for submenu. Use submenu string if it is intended so.       
            if(aIsSubmenu)
                {
                const TDesC& extraText = aSource->ExtraText(cmdId);
                if(extraText.Length())
                    {
                    itemData.iText.Zero();
                    itemData.iText.Append(extraText);
                    }                 
                }
            
            aDest.InsertMenuItemL(itemData, inPos++);
            }   
        }   
    }



TInt CLiwServiceHandlerImpl::SlotItemCmd(CEikMenuPane& aPane)
    {
    TInt index;

    for (TInt i = 0; i < KMaxMenuResources; i++)
        {
        if (aPane.MenuItemExists(ELiwMenuSlotBase + i, index))
            {
            return ELiwMenuSlotBase + i;
            }
        }

    return -1;
    }



CLiwMenuPane* CLiwServiceHandlerImpl::MenuPaneForSlotCmd(TInt aCmdId)
    {
    TInt index = aCmdId - ELiwMenuSlotBase; 

    if (index < KMaxMenuResources)
        {
        TInt resId = resourceSlotIds[index];
        for (TInt i = 0; i < iMenuPanes.Count(); i++)
            {
            if (iMenuPanes[i]->ResourceSlotId() == resId)
                {
                return iMenuPanes[i];
                }
            }
        }

    return NULL;
    }



CLiwServiceHandlerImpl::TLiwPlaceholderType CLiwServiceHandlerImpl::PlaceholderType(
    CEikMenuPane& aPane, TInt aCmd, TBool& aTitleLocked)
    {
    CEikMenuPaneItem::SData& itemData = aPane.ItemData(aCmd);

    aTitleLocked = EFalse;

    if ((itemData.iCascadeId & LIW_CASCADE_ID) == LIW_CASCADE_ID)
        {
        if (itemData.iCascadeId & LIW_LOCK_SUBMENU_TITLE)
            {
            aTitleLocked = ETrue;
            }
        return ELiwPlaceholderCascade;
        }
    else if ((itemData.iCascadeId & LIW_INTELLIGENT_CASCADE_ID) == LIW_INTELLIGENT_CASCADE_ID)
        {
        if (itemData.iCascadeId & LIW_LOCK_SUBMENU_TITLE)
            {
            aTitleLocked = ETrue;
            }
        return ELiwPlaceholderIntelligentCascade;
        }

    return ELiwPlaceholderNormal;
    }



void CLiwServiceHandlerImpl::ConvertPlaceholderL(CEikMenuPane& aPane, TInt aCmd, 
    CLiwMenuPane& aLiwPane, const TDesC& aTitle)
    {
    CEikMenuPaneItem::SData itemData = aPane.ItemData(aCmd);
    TInt index;

    // Remenber index.
    aPane.MenuItemExists(aCmd, index);

    // Remove placeholder item.
    aPane.DeleteMenuItem(aCmd);

    // Replace liw cascade id with actual menu resource id.
    itemData.iCascadeId = aLiwPane.iResourceSlotId;

    if (aTitle.Length())
        {
        itemData.iText.Copy(aTitle);
        }

    // Set unused dynamic cmd id.   
    itemData.iCommandId = iSubmenuCmd++;

    // Insert cascade item.
    aPane.InsertMenuItemL(itemData, index);
    }



void CLiwServiceHandlerImpl::UnCascadeL(CEikMenuPane& aPane, TInt aCmd, CLiwMenuPane& aLiwPane)
    {
    CEikMenuPaneItem::SData itemData = aLiwPane.MenuPane().ItemData(aLiwPane.FindCmdId(0));
    TInt index;

    // Remenber index.
    aPane.MenuItemExists(aCmd, index);

    // Remove placeholder item.
    aPane.DeleteMenuItem(aCmd);

    // Uncascade 
    itemData.iCascadeId = 0;

    // Insert cascade item.
    aPane.InsertMenuItemL(itemData, index);     
    }



void CLiwServiceHandlerImpl::SkipMenuFields(TResourceReader& aReader)
    {
    aReader.ReadInt32(); // Skip cascade id
    aReader.ReadInt32(); // Skip flags
    aReader.ReadTPtrC(); // Skip text
    aReader.ReadTPtrC(); // Skip extra text
    aReader.ReadTPtrC(); // Skip bmpfile.
    aReader.ReadInt16(); // Skip bmpid.
    aReader.ReadInt16(); // Skip bmpmask. 
    aReader.ReadInt32(); // Skip extension.   
    }


TBool CLiwServiceHandlerImpl::IsLiwMenu(TInt aMenuResourceId)
    {
    TInt index;

    // First check if this is liw submenu id
    for (index = 0; index < KMaxMenuResources; index++)
        {
        if (aMenuResourceId == resourceSlotIds[index])
            {
            return ETrue;
            }
        }

    // Then check if this menu is among attached menus.
    for (index = 0; index < iMenuBindings.Count(); index++)
        {
        if (iMenuBindings[index]->MenuId() == aMenuResourceId)
            {
            return ETrue;
            }
        }

    return EFalse;
    }



TBool CLiwServiceHandlerImpl::HandleSubmenuL(CEikMenuPane& aPane)
    {
    TInt slotcmd = SlotItemCmd(aPane);
    if (slotcmd >= 0)
        {
        // aPane is liw submenu. At this point it is empty and we must
        // copy provider menu items to it.
        CLiwMenuPane* liwPane = MenuPaneForSlotCmd(slotcmd);
        if (liwPane)
            {
            CopyMenuItemsL(liwPane, aPane, 0, ETrue);
            aPane.DeleteMenuItem(slotcmd);
            iSubmenu = liwPane;     
            return ETrue;
            }
        }

    return EFalse;
    }



TBool CLiwServiceHandlerImpl::GetSubmenuTitle(CEikMenuPane& aPane, TDes& aResult)
    {
    TInt index;
    
    aResult.Zero();
    while (aPane.MenuItemExists(LIW_SUBMENU_TITLE, index))
        {
        CEikMenuPaneItem::SData& itemData = aPane.ItemData(LIW_SUBMENU_TITLE);
        if (aResult.Length() == 0)
            {
            aResult.Copy(itemData.iText);
            }
        aPane.DeleteMenuItem(LIW_SUBMENU_TITLE);
        return ETrue;
        }

    return EFalse;
    }



CLiwCriteriaItem* CLiwServiceHandlerImpl::ConvertCriteriaItemPointerL(CLiwCriteriaItem* aCandidate)
    {
    for (TInt index = 0; index < iInterestList.Count(); index++)
        {
        if ((*iInterestList[index]) == (*aCandidate))
            {
            // Already in list, aCandidate is not needed.
            delete aCandidate;
            return iInterestList[index];
            }
        }

    CleanupStack::PushL(aCandidate);
    User::LeaveIfError(iInterestList.Append(aCandidate));
    CleanupStack::Pop(aCandidate); // aCandidate

    return aCandidate;
    }



void CLiwServiceHandlerImpl::FilterInterestListL(RPointerArray<CLiwCriteriaItem>& aOrginal,
    RPointerArray<CLiwCriteriaItem>& aFiltered)
    {
    CLiwCriteriaItem* item;

    while (aOrginal.Count() > 0)
        {
        item = aOrginal[0];
        aOrginal.Remove(0);
        item = ConvertCriteriaItemPointerL(item);
        User::LeaveIfError(aFiltered.Append(item));
        }
    aOrginal.Reset();
    }



void CLiwServiceHandlerImpl::RemoveProvider(TInt aImplUid)
    {
    TInt index;

    // First go through bindings and remove all the 
    // references to given provider.
    for (index = 0; index < iBaseBindings.Count(); index++)
        {
        iBaseBindings[index]->RemoveProvider(aImplUid);
        }

    for (index = 0; index < iMenuBindings.Count(); index++)
        {
        iMenuBindings[index]->RemoveProvider(aImplUid);
        }
    
    // Then remove provider from the owner list and delete it.
    for (index = 0; index < iProviders.Count(); index++)
        {
        if (iProviders[index]->ImplementationUid().iUid == aImplUid)
            {
            delete iProviders[index];
            iProviders.Remove(index);
            index--;
            }
        }
    }


void CLiwServiceHandlerImpl::AddProviderL(TUid aImplUid, CLiwCriteriaItem* aItem)
    {
    TInt index;
    CLiwServiceIfBase* iface = iEcomMonitor->CreateImplementationL(aImplUid);
    
    if (iface)
        {
        CleanupStack::PushL(iface);
        iface->AddCriteria(aItem);
        User::LeaveIfError(iProviders.Append( iface ));
        CleanupStack::Pop(iface);

        for (index = 0; index < iBaseBindings.Count(); index++)
            {
            if (iBaseBindings[index]->HasCriteriaItem(*aItem) != KErrNotFound)
                {
                iBaseBindings[index]->AddProviderL(iface, aImplUid == aItem->DefaultProvider());
                iface->InitialiseL(*this, iBaseBindings[index]->Interest());
                }               
            }

        for (index = 0; index < iMenuBindings.Count(); index++)
            {
            if (iMenuBindings[index]->HasCriteriaItem(*aItem) != KErrNotFound)
                {
                iMenuBindings[index]->AddProviderL(iface, aImplUid == aItem->DefaultProvider());
                iface->InitialiseL(*this, iMenuBindings[index]->Interest());
                }               
            }
        }
    }



TInt CLiwServiceHandlerImpl::SynchronizeCallBack(TAny* aImpl)
    {
    CLiwServiceHandlerImpl* impl = reinterpret_cast<CLiwServiceHandlerImpl*>(aImpl);
    TRAPD(err, impl->SynchronizeDbL());
    return err;
    }



void CLiwServiceHandlerImpl::SynchronizeDbL()
    {
    TInt index;
    RArray<TInt> providers;
    RImplInfoPtrArray infoArray;

    CleanupStack::PushL( TCleanupItem( IntArrayCleanup, &providers ) );
    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );

    for (index = 0; index < iInterestList.Count(); index++)
        {
        if (iInterestList[index]->RomOnly())  // Rom-only criterias can be skipped.
            {
            continue;
            }

        providers.Reset();
        infoArray.ResetAndDestroy();
        ListProvidersForCriteriaL(providers, *(iInterestList[index]));
        iEcomMonitor->ListImplemetationsL(infoArray, iInterestList[index]);
        HandleRemovedProviders(providers, infoArray);          
        HandleNewProvidersL(providers, infoArray, iInterestList[index]);        
        }

    CleanupStack::PopAndDestroy(&infoArray); // providers, infoArray
    CleanupStack::PopAndDestroy(&providers);
    }


void CLiwServiceHandlerImpl::HandleRemovedProviders(RArray<TInt>& aInMemory, 
    RImplInfoPtrArray& aInSystem)
    {
    TInt index, index2;

    for (index = 0; index < aInMemory.Count(); index++)
        {
        for (index2 = 0; index2 < aInSystem.Count(); index2++)
            {
            if (aInSystem[index2]->ImplementationUid().iUid == aInMemory[index])
                {
                break;
                }
            }
        if (index2 >= aInSystem.Count())  // Was removed from system.
            {
            RemoveProvider(aInMemory[index]);
            }
        }
    }


void CLiwServiceHandlerImpl::HandleNewProvidersL(RArray<TInt>& aInMemory, 
    RImplInfoPtrArray& aInSystem, CLiwCriteriaItem* aItem)
    {
    TInt index;

    for (index = 0; index < aInSystem.Count(); index++)
        {
        if (aInMemory.Find(aInSystem[index]->ImplementationUid().iUid) == KErrNotFound)
            {
            AddProviderL(aInSystem[index]->ImplementationUid(), aItem);
            }       
        }
    }
    
void CLiwServiceHandlerImpl::MenuLaunched()
    {  
    ClearMenuPaneArray();
    iNextFreeSlot = 0;
    iLastInitialized.Reset();

    // Reset the iMenuPane pointers from iMenuBindings.
    for(TInt index = 0; index < iMenuBindings.Count(); index++)
        {
        iMenuBindings[index]->SetMenuPane(NULL);
        }
    }

/* Utility function to get the symbian TCapability enum value
 * from the string defined in the providers xml file
 *
 * @param aCapName name of the capabiility to be converted to \c TCapability enum
 *
 */

TCapability CLiwServiceHandlerImpl::GetServiceCapability(const TDesC& aCapName)
{
	TCapability cap(ECapability_None);

	if(0==aCapName.Compare(KCapabilityCommDD))
	{
		cap=ECapabilityCommDD;
	}
	else if(0==aCapName.Compare(KCapabilityPowerMgmt))
	{
		cap=ECapabilityPowerMgmt;
	}
	else if(0==aCapName.Compare(KCapabilityMultimediaDD))
	{
		cap=ECapabilityMultimediaDD;
	}
	else if(0==aCapName.Compare(KCapabilityReadDeviceData))
	{
		cap=ECapabilityReadDeviceData;
	}
	else if(0==aCapName.Compare(KCapabilityWriteDeviceData))
	{
		cap=ECapabilityWriteDeviceData;
	}
	else if(0==aCapName.Compare(KCapabilityDRM))
	{
		cap=ECapabilityDRM;
	}
	else if(0==aCapName.Compare(KCapabilityTrustedUI))
	{
		cap=ECapabilityTrustedUI;
	}
	else if(0==aCapName.Compare(KCapabilityProtServ))
	{
		cap=ECapabilityProtServ;
	}
	else if(0==aCapName.Compare(KCapabilityDiskAdmin))
	{
		cap=ECapabilityDiskAdmin;
	}
	else if(0==aCapName.Compare(KCapabilityNetworkControl))
	{
		cap=ECapabilityNetworkControl;
	}
	else if(0==aCapName.Compare(KCapabilityAllFiles))
	{
		cap=ECapabilityAllFiles;
	}
	else if(0==aCapName.Compare(KCapabilitySwEvent))
	{
		cap=ECapabilitySwEvent;
	}
	else if(0==aCapName.Compare(KCapabilityNetworkServices))
	{
		cap=ECapabilityNetworkServices;
	}
	else if(0==aCapName.Compare(KCapabilityLocalServices))
	{
		cap=ECapabilityLocalServices;
	}
	else if(0==aCapName.Compare(KCapabilityReadUserData))
	{
		cap=ECapabilityReadUserData;
	}
	else if(0==aCapName.Compare(KCapabilityWriteUserData))
	{
		cap=ECapabilityWriteUserData;
	}
	else if(0==aCapName.Compare(KCapabilityLocation))
	{
		cap=ECapabilityLocation;
	}
	else if(0==aCapName.Compare(KCapabilitySurroundingsDD))
	{
		cap=ECapabilitySurroundingsDD;
	}
	else if(0==aCapName.Compare(KCapabilityUserEnvironment))
	{
		cap=ECapabilityUserEnvironment;
	}

	return cap;
}


/**
* Returns the capability set defined in the service provider
* metadata information. The capability set is a pre-requisite
* for the service consumer to load the service provider module.
*
* The capability metadata information are defined as XML character
* data inside the element <capability/>. The capability information
* are type of metadata information. Hence, the capability element tags
* should appear as child element of <metadata> element.
*
* @param aCapability Capability set that the consumer should posess while
*		 loading the service provider
*
* @example
*
* @code
* <!-- consumer should posess the following capability set -->
* <metadata>
*	<capability>CapabilityReadUserData</capability>
*	<capability>CapabilityWriteUserData</capability>
*	<capability>CapabilityDRM</capability>
* </metadata>
* @endcode
*
*/

void CLiwServiceHandlerImpl::GetCapabilitiesL(RArray<TCapability>& secMgrCapList,CLiwGenericParamList* pMetaData)
{	
	_LIT8(KCapability,"cap");

	TInt pos = 0;
	const TLiwGenericParam* pCapData = pMetaData->FindFirst(pos,KCapability);

	if(pCapData)
	{
		const CLiwList* pCapList = pCapData->Value().AsList();
		if(pCapList)
		{
			for(TInt idx(0);idx!=pCapList->Count();++idx)
			{
				TLiwVariant capVar;
				capVar.PushL();
				pCapList->AtL(idx, capVar);
				TPtrC capStr = capVar.AsDes();
				
				TCapability cap = this->GetServiceCapability(capStr);
				if( (cap>=ECapabilityTCB)  && (cap < ECapability_Limit))
					secMgrCapList.AppendL(cap);
				CleanupStack::Pop(&capVar);
				capVar.Reset();
			}
		}
		
	}
}

void CLiwServiceHandlerImpl::GetProviderResourceFile(TDes& aFilePath,CLiwGenericParamList* pMetaData)
{	
	_LIT8(KResourceFile,"res");

	TInt pos = 0;
	const TLiwGenericParam* pCapData = pMetaData->FindFirst(pos,KResourceFile);

	if(pCapData)
	{
		const CLiwList* pCapList = pCapData->Value().AsList();
		if(pCapList)
		{
			for(TInt idx(0);idx!=pCapList->Count();++idx)
			{
				TLiwVariant capVar;
				capVar.PushL();
				pCapList->AtL(idx, capVar);
				aFilePath = capVar.AsDes();
				CleanupStack::Pop(&capVar);
				capVar.Reset();
			}
		}
		
	}
}

void CLiwServiceHandlerImpl::ComputeIntfVersion(CLiwServiceData* pProvMetaData,TReal& aIntfVersion)
{
	CLiwGenericParamList* pMetaDataList = pProvMetaData->GetMetaData();
	
	if(pMetaDataList)
	{	
		TInt verPos(KErrNone);
		_LIT8(KVer,"ver");
		const TLiwGenericParam* pVerParam = pMetaDataList->FindFirst(verPos,KVer);
		if(pVerParam)
		{
			const CLiwList* pVersionList = pVerParam->Value().AsList(); 
			if(pVersionList)
			{
				if(pVersionList->Count())
				{
					TLiwVariant verVar;
					verVar.PushL();
					pVersionList->AtL(0,verVar);//pick up the value in 0th index..
					aIntfVersion = verVar.AsTReal();
					CleanupStack::Pop(&verVar);
					verVar.Reset();
				}					
			}			
		}	
	}	
}


TInt CLiwServiceHandlerImpl::ResolveProvidersL(CLiwBinding* aBinding, 
					       CLiwCriteriaItem* aItem,
					       CRTSecMgrScriptSession* aScriptSession)
    {
    TInt result = 0;
    TInt status = KLiwUnknown;

    // First resolve for providers already in memory.
    TInt index;
    for (index = 0; index < iProviders.Count(); index++)
        {
        if (iProviders[index]->Match(aItem))
            {
            aBinding->AddProviderL((CLiwServiceIfBase*)iProviders[index], 
                iProviders[index]->ImplementationUid() == aItem->DefaultProvider());          
            result++;
            }     
        }


    // If cached providers were found, then it means that all the matching
    // providers must be already in memory. No need to query from ECom framework.
    if (!result)
        {
        RImplInfoPtrArray infoArray;
        RArray<TInt32> infoArrayPlugin;

        CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
        CleanupStack::PushL( TCleanupItem( Int32ArrayCleanup, &infoArrayPlugin ) );
        
        iEcomMonitor->ListImplemetationsL(infoArray, aItem);
        
        FilterInfoArray(infoArray, infoArrayPlugin, aItem);

		CLiwServiceData* pServiceData = NULL;
		
		CLiwServiceData* pPrevSData = NULL;
		
		CImplementationInformation* pChosenImpl = NULL;
		
		TReal currentMax(KDefVersion); 

        for (index = 0; index < infoArray.Count(); index++)
            {
            TBool stackPop = EFalse;
            if ((aItem->Options() & LIW_OPTIONS_ROM_ONLY) && (infoArray[index]->RomBased() == EFalse))
                {
                continue;
                }
    
    		//Check whether consumer has capability
    		//mandated by provider
    		CImplementationInformation* pImplInfo = infoArray[index];
    		
    		TBuf8<KMaxLength> opaq;
    		
    		ParseMetaData(pImplInfo->OpaqueData(),opaq);
    		
    		CLiwXmlHandler* pXmlHandler = CLiwXmlHandler::NewLC();
    		
			pServiceData = CLiwServiceData::NewLC();
    		
    		TInt loadStatus= CLiwXmlHandler::ESrvDataLoadFailed;
    		
    		//Inline metadata defined
    		if(opaq.Length()>0)
    		{
    			loadStatus=pXmlHandler->LoadServiceData(opaq,pServiceData);
    		}
    		else
    		{
    			//Obtain the capabilities from the metadata
	    		TUid implUid = pImplInfo->ImplementationUid();
	    		TUidName srvProvUid = implUid.Name();
	    
	    		
	    		TPtrC16 ptrSrv = srvProvUid.Right(srvProvUid.Length()-1);
	    		TPtrC16 srvFile = ptrSrv.Left(ptrSrv.Length()-1);		
	    		TDriveUnit driveUnit = pImplInfo->Drive();
	    		TFileName fileName = driveUnit.Name();
	    		fileName.Append(KDC_RESOURCE_FILES_DIR);
	    		fileName.Append(srvFile);
	    		fileName.Append(KPerExtension);
	    		loadStatus=pXmlHandler->LoadServiceData(fileName,pServiceData);
    		}
    		
    		/*
    		 *  - Get version range specified by the consumer
    		 *  - Iterate over the list of intf impl
    		 *
    		 * //LOOP:
    		 *  - For each implementation item,
    		 *
    		 *  	- Check if intf impl has version tag from its metadata
    		 *		- Pick up interface impl's version tag
    		 *		- If intf impl has NO specifed version tag
    		 *			- set this intf impl version as DEFAULT VERSION (//pref 1.0)..
    		 *
    		 *   //CHOOSE LATEST VERSION:
    		 *		- If consumer has specifed version range
    		 *			- CALL COMPARE routine (//To check if this is the latest version so far)
    		 *				- Mark this as the chosen implementation
    		 *			- Else (//This is NOT the latest)
    		 *				- Continue;
    		 *
    		 *		- Else (//If consumer did not specify version range)
    		 *			- CALL COMPARE routine (//To check if this is the latest version so far)
    		 *				- Mark this as the chosen implementation
    		 *			- Else (//This is NOT the latest)
    		 *				- Continue;
    		 *  	
    		 *  //COMPARE (currentMax,implVersion,minVer,maxVer): //default minVer=1.0
    		 *		- if(implVersion>currentMax)
    		 *			- if(implVersion>=minVer && implVersion<=maxVer)
    		 *				- currentMax = implVersion;
    		 *				- return; //mark pServiceData to point to the current impl's service data
    		 *			- else
    		 *				- return; //leave pServiceData as it is
    		 *		- else
    		 *			- return; //leave pServiceData as it is
    		 *
    		 */
    		 
    		if(CLiwXmlHandler::ESrvDataLoadSuccess==loadStatus) //metadata is parsed successfully
    		{
    			TReal implVersion(KDefVersion);
    			this->ComputeIntfVersion(pServiceData,implVersion); //fetch impl version..
    			
    			TReal minVer(KDefVersion);
    			TReal maxVer(KUnspVersion);
    			
    			TBool verChk = this->GetVersionRange(aItem,minVer,maxVer);
    			
    			if(verChk) //Get version queried by consumer
    			{
    				if(minVer == KUnspVersion)
    					minVer = KDefVersion;
    				
    				//perform comparison...
    				if(implVersion>=currentMax)
    				{
    					if(maxVer!=KUnspVersion)
    					{
	    					if((implVersion>=minVer) && (implVersion<=maxVer))
	    					{
	    						currentMax = implVersion;
	    						pChosenImpl = infoArray[index];
	    						//current impl is the best choice..this is THE CHOSEN ONE..
	    						if(pPrevSData)
	    						{
	    							pPrevSData->CleanUpMetaData();
	    							delete pPrevSData;
	    							
	    						}
	    						
	    						pPrevSData = pServiceData;
	    						
	    					}
	    					else
	    					{
	    						//current impl is NOT THE RIGHT CHOICE.. since not within the range
		    					if(pServiceData)
		    					{
		    						if(!stackPop)
						    		{
							    		stackPop = ETrue;
										CleanupStack::Pop(pServiceData);	
						    		}
						    		pServiceData->CleanUpMetaData();
		    						delete pServiceData;
		    						pServiceData = NULL;
		    					}
		    					status = KLiwVersionOutOfRange;
	    					}
    					}
    					else
    					{
    						//means maxVer == KUnspVersion
    						if(implVersion>=minVer)
	    					{
	    						currentMax = implVersion;
	    						pChosenImpl = infoArray[index];
	    						
	    						//current impl is the best choice..this is THE CHOSEN ONE..
	    						if(pPrevSData)
	    						{
	    							pPrevSData->CleanUpMetaData();
	    							delete pPrevSData;
	    						}
	    						
	    						pPrevSData = pServiceData;
	    				 	}
	    					else
	    					{
	    						//current impl is NOT THE RIGHT CHOICE..
		    					if(pServiceData)
		    					{
		    						if(!stackPop)
						    		{
							    		stackPop = ETrue;
										CleanupStack::Pop(pServiceData);	
						    		}
		    						pServiceData->CleanUpMetaData();
		    						delete pServiceData;
		    						pServiceData = NULL;
		    					}
		    					
		    					status = KLiwVersionOutOfRange;
	    					 }
    					}
    				}
    				else
    				{
    					//current impl is NOT THE RIGHT CHOICE..since implVer > maxVer
    					if(pServiceData)
    					{
    						if(!stackPop)
				    		{
					    		stackPop = ETrue;
								CleanupStack::Pop(pServiceData);	
				    		}
		    				pServiceData->CleanUpMetaData();
    						delete pServiceData;
    						pServiceData = NULL;
    					}
    					
    					status = KLiwVersionOutOfRange;
    				}    				
    			}
    			else
    			{
    				//GetVersionRange Fails.. 
    				//abort service resolution process..
    				if(pServiceData)
					{
						if(!stackPop)
			    		{
				    		stackPop = ETrue;
							CleanupStack::Pop(pServiceData);	
			    		}
			    		pServiceData->CleanUpMetaData();
						delete pServiceData;
						pServiceData = NULL;
					} 
					
    				status = KLiwInvalidVersionSpecification;
    			}
    			
	    		//other cases like parse error, capability not
	    		//specified in meta data are handled
				
    			if(currentMax == implVersion)
		    	{
			    	if(pPrevSData && (pPrevSData!=pServiceData)) //just in case...
			    	{
			    		pPrevSData->CleanUpMetaData();
						delete pPrevSData;
						pPrevSData = NULL;
			    	}	
		    	}
		    	else
		    	{
		    		if(!stackPop)
		    		{
			    		stackPop = ETrue;
						CleanupStack::Pop(pServiceData);	
		    		}
					pServiceData = pPrevSData;
		    	}	
    		}
    		else
    		{
    			//Metadata specification not found.. Hence Load Fails
    			if(CLiwXmlHandler::ESrvDataFileNotFnd==loadStatus)
    			{	
					pChosenImpl = infoArray[index];    				
    			}
    			else  //Some error like parse error, capability not specified are handled
    			{
    				if(pServiceData)
					{
						if(!stackPop)
			    		{
				    		stackPop = ETrue;
							CleanupStack::Pop(pServiceData);	
			    		}
		    			pServiceData->CleanUpMetaData();
						delete pServiceData;
						pServiceData = NULL;
					}	
					
					status = KLiwMetaDataInvalidFormat;
    			}
	    	}	    		
    		
    		for (TInt idx = 0; idx < infoArrayPlugin.Count(); idx++)
            {
            	// currently assumed that implementations managed by plugins cannot be in ROM
            	if (aItem->Options() & LIW_OPTIONS_ROM_ONLY) 
                {
                continue;
                }
            }
            
            if(!stackPop)
            {
            	CleanupStack::Pop(pServiceData);	
            }
            
            CleanupStack::Pop(pXmlHandler);
            
            if(pXmlHandler)
			{
				delete pXmlHandler;
			}			

	        
        } //end of for loop
        
    	if(pChosenImpl)
		{
		 	RArray<TCapability> provCaps;
		 	TFileName provResourcePath;
	    	GetCapabilitiesL(provCaps,pServiceData->GetMetaData());	    	
	    	  		
	    	TInt isAllowed(KErrNone);
	    	
	    	if(aScriptSession)
	    	    {
	    	    if(aScriptSession->PromptOption() == RTPROMPTUI_PROVIDER)
	    	    	{
	    	    		GetProviderResourceFile(provResourcePath, pServiceData->GetMetaData());
	    	        isAllowed = aScriptSession->IsAllowed(provCaps, pChosenImpl->ImplementationUid(), provResourcePath);	
	    	      }
                else
                    isAllowed = aScriptSession->IsAllowed(provCaps);
	    	    }
	    		
	    	if(KErrNone==isAllowed)
		    {
				CLiwServiceIfBase* iface = iEcomMonitor->CreateImplementationL(
	            pChosenImpl->ImplementationUid());

	            if (iface)
		        {
		            if(pServiceData)
		            {
		            	iface->iReserved=pServiceData;
		            }
		            	
		            
		            if (!IsCached(iface))
		            {
		                CleanupStack::PushL(iface);
		                status = KLiwServiceLoadSuccess;
		                iface->AddCriteria(aItem);
		                User::LeaveIfError(iProviders.Append( iface ));
		                CleanupStack::Pop(iface);
		                
		                aBinding->AddProviderL(iface, 
		                    pChosenImpl->ImplementationUid() == aItem->DefaultProvider());
		            }                           
		            else    
		            {
		                delete iface;
		                iface = NULL;                
		             }
		         }
		         
			  }
			  else
			  {
			  	  status = KLiwSecurityAccessCheckFailed;
				 
					if(pServiceData)
			      	{
			      		pServiceData->CleanUpMetaData();
			      		delete pServiceData;
			       		pServiceData = NULL;	
			      	}
			     
				//enhancement : Should assign this to the previous service data						      	
			  }
			  
			  provCaps.Close();
		  }
		  else
          {
          	//No Chosen implementation.. 
          	
          	if(pServiceData)  //This should ideally fail always
          	{
          		pServiceData->CleanUpMetaData();
          		delete pServiceData;
           		pServiceData = NULL;	
          	}
          } 
          
          if(0 == infoArray.Count())
          	status = KLiwUnknown; 
         
         CleanupStack::PopAndDestroy(2);//infoArray and infoArrayPlugin 
        }
        else
        {
        	status = KLiwServiceAlreadyLoaded;
        }
    
    return status;
    
    }


TBool CLiwServiceHandlerImpl::GetVersionRange(CLiwCriteriaItem* aItem,TReal& aMinVersion, TReal& aMaxVersion)
{
	TBool bFailed=EFalse;
	TLiwVariant metaDataVar;
	metaDataVar.PushL();
	
	aItem->GetMetaDataOptions(metaDataVar);
	
	const CLiwMap* metaDataMap = metaDataVar.AsMap();
	
	if(metaDataMap)
	{
	_LIT8(KRangeKey,"range");
	TLiwVariant rangeVar;
	rangeVar.PushL();
	
	if(metaDataMap->FindL(KRangeKey, rangeVar))
	{
		const CLiwList* pRangeList = rangeVar.AsList();
		if(pRangeList)
		{
			TLiwVariant verCheck;
			verCheck.PushL();
			_LIT8(KVersion,"ver");
			pRangeList->AtL(0,verCheck);
			
			if(EVariantTypeDesC8==verCheck.TypeId())
			{
				if(0==KVersion().CompareF(verCheck.AsData()))
				{
					TLiwVariant minVerVar, maxVerVar;
					minVerVar.PushL();
					maxVerVar.PushL();
					
					pRangeList->AtL(1,minVerVar);
					aMinVersion = minVerVar.AsTReal();
					
					if(minVerVar.AsTReal() < KDefVersion)
						bFailed = ETrue;
					
					pRangeList->AtL(2,maxVerVar);
					aMaxVersion = maxVerVar.AsTReal();
					
					if(maxVerVar.AsTReal() < aMinVersion && maxVerVar.AsTReal() != KUnspVersion)
						bFailed = ETrue;
					
					CleanupStack::Pop(&maxVerVar); 
    				CleanupStack::Pop(&minVerVar); 
    				minVerVar.Reset();
					maxVerVar.Reset();
				}
			}
			CleanupStack::Pop(&verCheck); 
    		verCheck.Reset();
		}	
	}
	CleanupStack::Pop(&rangeVar); 
    rangeVar.Reset();
	}
	
	CleanupStack::Pop(&metaDataVar); 
    metaDataVar.Reset();

    return !bFailed;
}

TBool CLiwServiceHandlerImpl::GetVersion(CLiwCriteriaItem* aItem,TReal& aVersion)
{
	TBool bFailed=EFalse;
	TLiwVariant metaDataVar;
	metaDataVar.PushL();
	
	aItem->GetMetaDataOptions(metaDataVar);
	
	const CLiwMap* metaDataMap = metaDataVar.AsMap();
	
	if(metaDataMap)
	{
	_LIT8(KExactKey,"exact");
	TLiwVariant exactVar;
	exactVar.PushL();
	
		if(metaDataMap->FindL(KExactKey, exactVar))
		{
			const CLiwList* pExactList = exactVar.AsList();
			if(pExactList)
			{
				TLiwVariant verCheck;
				verCheck.PushL();
				_LIT8(KVersion,"ver");
				pExactList->AtL(0,verCheck);
				
				if(EVariantTypeDesC8==verCheck.TypeId())
				{
					if(0==KVersion().CompareF(verCheck.AsData()))
					{
						TLiwVariant versionVar;
						versionVar.PushL();
						
						pExactList->AtL(1,versionVar);
						
	    				if((versionVar.AsTReal()) < KDefVersion)
	    				{
	    					bFailed = ETrue;
	    				}
	    				
	    				aVersion = versionVar.AsTReal();
						CleanupStack::Pop(&versionVar); 
	    				versionVar.Reset();
					}
				}
				CleanupStack::Pop(&verCheck); 
	    		verCheck.Reset();
			}	
		}
	
	CleanupStack::Pop(&exactVar); 
    exactVar.Reset();
	}
	
	CleanupStack::Pop(&metaDataVar); 
    metaDataVar.Reset();
	
	return !bFailed;
}

// End of file
