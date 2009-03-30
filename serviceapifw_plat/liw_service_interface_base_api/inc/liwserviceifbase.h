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
* Description:       Declares Base service API for all providers to implement in 
*                order to offer services to Language Interworking Framework.
*
*/






#ifndef LIW_SERVICE_IF_BASE_H
#define LIW_SERVICE_IF_BASE_H

// INCLUDES
#include <liwgenericparam.h>
#include <liwcommon.h>

// CONSTANTS

// MACROS

// DATA TYPES
/**
* Extension interface UIDs.
*/
enum TLiwExtendedIf
{
	/*
	 *  Extension interface UID to get service data interface
	 *
	 * @see CLiwServiceData
	 */
	KLiwGetServiceDataIf = 0x102830B2
};

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class MLiwNotifyCallback;
class CLiwCriteriaItem;
//typedef RPointerArray<CLiwCriteriaItem> RCriteriaArray;
class CLiwServiceData;
// CLASS DECLARATION

/**
* This is the base service interface to access providers that offer base services.
*
* This interface needs to be implemented by all the LIW providers. Base service
* providers should inherit from this class directly. Menu service providers 
* should inherit from CLiwServiceIfMenu.
*
* @lib ServiceHandler.lib
* @since Series 60 2.6
* @see CLiwServiceIfMenu
**/
class CLiwServiceIfBase : public CBase
    {
    public:
        /** 
        * Destructor.
        */
        IMPORT_C virtual ~CLiwServiceIfBase();

        /**
        * Called by the LIW framework to initialise provider with necessary information 
        * from the Service Handler. This method is called when the consumer makes 
        * the attach operation.
        *
        * @param aFrameworkCallback Framework provided callback for provider to send 
        *                           events to framework.
        * @param aInterest List of criteria items which invoked the provider.
        */
        virtual void InitialiseL(
            MLiwNotifyCallback& aFrameworkCallback,
            const RCriteriaArray& aInterest) = 0;
        
        /**
        * Executes generic service commands included in criteria.
        *
        * @param aCmdId Command to be executed.
        * @param aInParamList Input parameters, can be an empty list.
        * @param aOutParamList Output parameters, can be an empty list.
        * @param aCmdOptions Options for the command, see KLiwOpt* in LiwCommon.hrh.
        * @param aCallback Callback for asynchronous command handling, parameter checking, etc.
        * @leave KErrArgument Callback is missing when required.
        * @leave KErrNotSupported No provider supports service.
        */
        virtual void HandleServiceCmdL(
            const TInt& aCmdId,
            const CLiwGenericParamList& aInParamList,
            CLiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MLiwNotifyCallback* aCallback = NULL) = 0;

        /**
        * Executes generic service command
        *
        * @param aCmdName The service command to be executed.
        * @param aInParamList Input parameters, can be an empty list.
        * @param aOutParamList Output parameters, can be an empty list.
        * @param aCmdOptions Options for the command, see KLiwOpt* in LiwCommon.hrh.
        * @param aCallback Callback for asynchronous command handling, parameter checking, etc.
        * @leave KErrArgument Callback is missing when required.
        * @leave KErrNotSupported No provider supports service.
        */
        void HandleServiceCmdL(                         
            const TDesC8& aCmdName,
            const CLiwGenericParamList& aInParamList,
            CLiwGenericParamList& aOutParamList,
            TUint aCmdOptions = 0,
            const MLiwNotifyCallback* aCallback = NULL)
          {
            _LIT8(KGenericParamID,"cmd");
          
            ((CLiwGenericParamList&)aInParamList).AppendL(
              TLiwGenericParam(KGenericParamID, TLiwVariant(aCmdName)));
            
            HandleServiceCmdL(KLiwCmdAsStr, aInParamList,
                              aOutParamList, aCmdOptions, aCallback);
          }

		

        // Rest of the class is for LIW's internal use and doesn't consern 
        // service providers.
        friend class CLiwServiceHandlerImpl;
        friend class CLiwBinding;
        friend class CLiwEcomMonitor;

    protected:
        IMPORT_C virtual void* ExtensionInterface(TUid aInterface);

		private:
        void SetDtorKeyId(TUid aDtorKeyId);
        TBool Match(CLiwCriteriaItem* aItem);   
        void AddCriteria(CLiwCriteriaItem* aItem);
        TUid ImplementationUid() const;
        void SetImplementationUid(TUid aUid);
        TBool HasCriteria(CLiwCriteriaItem& aItem) const;

    private:
        TUid iDtorKeyId;
        RCriteriaArray iCriterias;
        TUid iImplUid;

        // Reserved member        
        TAny* iReserved;        
    };


#endif  // LIW_SERVICE_IF_BASE_H

// End of file
