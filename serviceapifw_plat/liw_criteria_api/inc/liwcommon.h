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
* Description:       Declares common constants, types, classes etc. to be used both
*                consumer and provider side.
*
*/







#ifndef LIW_COMMON_H
#define LIW_COMMON_H

// INCLUDES
#include <liwcommon.hrh>
#include <barsread.h> 
#include <liwgenericparam.h>

// CONSTANTS

// MACROS

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class CEikMenuPane;
class CLiwGenericParamList;

// CLASS DECLARATION

/**
 * Criteria item data.
 * This class encapsulates an LIW criteria item. Criteria items are building 
 * blocks for LIW interests, i.e. an interest is a list of criteria items. 
 * A criteria item can be defined dynamically (by using this class) or 
 * in a resource file.
 *
 * @lib ServiceHandler.lib
 * @since Series 60 2.6
 */
class CLiwCriteriaItem : public CBase
    {
    public:   // Constructors and destructor
        /** 
        * Constructs a criteria item instance with null items.
        *
        * @return A pointer to the new object.
        */
        IMPORT_C static CLiwCriteriaItem* NewL();

        /** 
        * Constructs a criteria item instance with null items. Leaves the 
        * pointer to the cleanup stack.
        *
        * @return A pointer to the new object.
        */
        IMPORT_C static CLiwCriteriaItem* NewLC();

        /** 
        * Constructs a criteria item instance.
        *
        * @param aCriteriaId The criteria ID.
        * @param aServiceCmd The service command.
        * @param aContentType The content type.
        * @return A pointer to the new object.
        */
        IMPORT_C static CLiwCriteriaItem* NewL(
            TInt aCriteriaId,
            TInt aServiceCmd,
            const TDesC8& aContentType);

        /** 
        * Constructs a criteria item instance. Leaves the pointer to the 
        * cleanup stack.
        *
        * @param aCriteriaId The criteria ID.
        * @param aServiceCmd The service command.
        * @param aContentType The content type.
        * @return A pointer to the new object.
        */
        IMPORT_C static CLiwCriteriaItem* NewLC(
            TInt aCriteriaId,
            TInt aServiceCmd,
            const TDesC8& aContentType);

        /** 
        * Constructs a criteria item instance.
        *
        * @param aCriteriaId The criteria ID.
        * @param aServiceCmd The service command.
        * @param aContentType Content type.
        * @return A pointer to the new object.
        */
        IMPORT_C static CLiwCriteriaItem* NewL(
            TInt aCriteriaId,
            const TDesC8& aServiceCmd,
            const TDesC8& aContentType);

        /** 
        * Constructs a criteria item instance.
        *
        * @param aCriteriaId The criteria ID.
        * @param aServiceCmd The service command.
        * @param aContentType Content type.
        * @return A pointer to the new object.
        */
        IMPORT_C static CLiwCriteriaItem* NewLC(
            TInt aCriteriaId,
            const TDesC8& aServiceCmd,
            const TDesC8& aContentType);

        /** 
        * Destructor.
        */        
        IMPORT_C virtual ~CLiwCriteriaItem();

    public: // New functions
        /** 
        * Sets the criteria ID.
        *
        * @param aId The criteria ID.
        */
        IMPORT_C void SetId(TInt aId);

        /** 
        * Sets the service class.
        *
        * @param aServiceUid The service class, see TLiwServiceClass.
        */
        IMPORT_C void SetServiceClass(const TUid& aServiceUid);
        
        /** 
        * Sets the service command UID.
        *
        * @param aServiceCmd The LIW service command, see TLiwServiceCommands.
        */
        IMPORT_C void SetServiceCmd(TInt aServiceCmd);

        /** 
        * Sets the service command name.
        *
        * @param aServiceCmd The LIW service command.
        */
        IMPORT_C void SetServiceCmdL(const TDesC8& aServiceCmd);

        /** 
        * Sets the content type. Makes a copy of the string data.
        *
        * @param aContentType Content MIME type, event type or any agreed one.
        */
        IMPORT_C void SetContentTypeL(const TDesC8& aContentType);

        /** 
        * Gets the criteria ID, 0 if not defined.
        *
        * @return The criteria ID.
        */
        IMPORT_C TInt Id() const;

        /** 
        * Gets the service class UID.
        *
        * @return The service class UID, see TLiwServiceClass.
        */
        IMPORT_C const TUid& ServiceClass() const;
        
        /** 
        * Gets the service command UID. KNullUid, if not defined.
        *
        * @return The service command UID, see TLiwServiceCommands.
        */
        IMPORT_C TInt ServiceCmd() const;

        /** 
        * Gets the service command name.
        *
        * @return The service command name.  
        */
        IMPORT_C const TDesC8& ServiceCmdStr() const;

        /** 
        * Gets the content type.
        *
        * @return The content type.
        */
        IMPORT_C const TDesC8& ContentType() const;

        /** 
        * Sets the options.
        *
        * @param aOptions Options.
        */
        IMPORT_C void SetOptions(TUint aOptions);

        /** 
        * Gets the options.
        * 
        * @return Options.
        */
        IMPORT_C TUint Options() const;

        /** 
        * Maximum number of providers allowed for this criteria item. 
        * 
        * @return Maximum number of providers allowed for this criteria item.
        */
        IMPORT_C TInt MaxProviders() const;

        /** 
        * Reads a criteria item from a resource.  
        *
        * @param aReader A resource reader pointing to a criteria item.
        */
        IMPORT_C void ReadFromResoureL(TResourceReader& aReader);

        /** 
        * Returns the default provider.
        * 
        * @return The default provider UID.        
        */
        IMPORT_C TUid DefaultProvider() const;

        /** 
        * Sets the default provider.
        *
        * @param aDefault The UID of a default provider.
        */
        IMPORT_C void SetDefaultProvider(TInt aDefault);

        /** 
        * Equality operator. Two criteria items are considered equal only if all
        * parameters match. The parameters are: criteria id, service command, 
        * content type, service class, default provider, max providers and options). 
        *
        * @param aItem Criteria item to compare.
        * @return ETrue if criteria items are equal, EFalse otherwise.
        */
        IMPORT_C TBool operator==(const CLiwCriteriaItem& aItem);

        /** 
        * Sets the maximum number of providers.
        *
        * @param aMaxProviders The maximum number of providers.
        */
        IMPORT_C void SetMaxProviders(TInt aMaxProviders);

    public:
        /** 
        * If set, only ROM based providers can be attached to this criteria.
        *
        * @return ETrue if LIW_OPTIONS_ROM_ONLY bit is set, EFalse otherwise.
        */    
        inline TBool RomOnly() const;
        
        IMPORT_C void SetMetaDataOptions(const TLiwVariant& aMetaDataOptions);

        IMPORT_C void GetMetaDataOptions(TLiwVariant& aMetaDataOptions) const;
        
	private:
        /**
        * C++ default constructor.
        */
        CLiwCriteriaItem();    
    
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(
            TInt aCriteriaId,
            TInt aServiceCmd,
            const TDesC8& aContentType);

        void ConstructL( 
            TInt aCriteriaId,
            const TDesC8& aServiceCmd,
            const TDesC8& aContentType);

    private:
        // Criteria ID.
        TInt iCriteriaId;
        // Service class UID
        TUid iServiceClass;
        // Service command UID
        TInt iServiceCmd;
        // Content type (MIME type). 
        HBufC8* iContentType;
        // Additional options for ROM
        TLiwVariant iOptions;        
        //Default provider implementation uid
        TUid iDefaultProvider;
        // Reserved member
        TLiwVariant iReserved;
        // Max providers
        TInt iMaxProviders;        
        // Metadata Options
        TLiwVariant iMetadataOptions;
    };


inline TBool CLiwCriteriaItem::RomOnly() const
    {
    return (Options() & LIW_OPTIONS_ROM_ONLY) != 0;
    }


/**
* Interest is an array of criteria items.
*/
typedef RPointerArray<CLiwCriteriaItem> RCriteriaArray;

/**
* Abstact callback interface to handle callbacks or events
* from providers. This callback is needed when
* - Consumer wants to pre-check output parameters before returning
*   synchronous Handle*Cmd.
* - Asynchronous Handle*Cmd is used.
* - An asynchronous event occurs in the system.
* 
*  @lib ServiceHandler.lib
*  @since Series 60 2.6
*/ 
class MLiwNotifyCallback                                  // needs to handle this as well?
    {
    public:
        /**
        * Handles notifications caused by an asynchronous Execute*CmdL call
        * or an event.
        *
        * @param aCmdId The service command associated to the event.
        * @param aEventId Occured event, see LiwCommon.hrh.
        * @param aEventParamList Event parameters, if any, as defined per
        *        each event.
        * @param aInParamList Input parameters, if any, given in the
        *        related HandleCommmandL.
        * @return Error code for the callback.
        */
        virtual TInt HandleNotifyL(
            TInt aCmdId,
            TInt aEventId,
            CLiwGenericParamList& aEventParamList,
            const CLiwGenericParamList& aInParamList) = 0;
    	IMPORT_C static TInt32 GetTransactionID();
    };

#endif // LIW_COMMON_H

// End of File


