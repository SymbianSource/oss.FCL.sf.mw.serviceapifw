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
* Description:       Variant data type for LIW generic parameters.
*
*/







#ifndef LIW_VARIANT_H
#define LIW_VARIANT_H

// INCLUDES
#include <e32std.h>
#include <s32strm.h>
#include <f32file.h>
#include "liwvarianttype.hrh"

// FORWARD DECLARATIONS

class CLiwGenericParamList;
class MLiwNotifyCallback;

// New variant types
class CLiwContainer;
class CLiwIterable;
class CLiwList;
class CLiwMap;
class MLiwInterface;
class CLiwBuffer;


// CLASS DECLARATION

/**
* Variant data class to hold a value of a TLiwGenericParam instance.
* The variant contains a data type and a value. This class is attached to
* TLiwGenericParam instance which holds the semantic type of the value.
*
* @lib ServiceHandler.lib
* @since Series 60 2.6
* @see TLiwGenericParam
*/
class TLiwVariant
    {
    public:  // Constructors
        /**
        * Default constructor. Initializes this variant to be empty.
        *
        * @post IsEmpty()
        */
        inline TLiwVariant();
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */
        inline TLiwVariant(TInt32 aValue);
        
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */
        inline TLiwVariant(TInt64 aValue);
        
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */
        inline TLiwVariant(TUint aValue);
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */
        inline TLiwVariant(TBool aValue);
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const TUid& aValue);
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const TTime& aValue);
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const TDesC& aValue);
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const HBufC* aValue);
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const TDesC8& aValue);
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const RFile& aValue);

        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const CLiwList* aValue);

        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const CLiwMap* aValue);

        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const MLiwInterface* aValue);

        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */        
        inline TLiwVariant(const CLiwIterable* aValue);

		/**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */
        inline TLiwVariant(TReal aValue);
        
        /**
        * Constructor from a value.
        *
        * @param aValue The value to set for this variant object.
        */
        inline TLiwVariant(const CLiwBuffer* aValue);
        
        /**
        * Copy constructor. Does not take ownership of data.
        *
        * @param aSrc The source object.
        */
        IMPORT_C TLiwVariant(const TLiwVariant& aSrc);
                
        /**
        * Assignment operator. Does not take ownership of data.
        *
        * @param aValue The source object.
        */
        IMPORT_C TLiwVariant& operator=(const TLiwVariant& aValue);

        // This class does not need a destructor because memory allocation
        // policy for variant class has been implemented by TLiwGenericParam
        // class.

    public:  // Interface
        /**
        * Returns the type id of data this variant object is holding.
        */
        inline LIW::TVariantTypeId TypeId() const;

        /**
        * Returns ETrue if this variant is empty (it does not hold any value).
        */
        inline TBool IsEmpty() const;

        /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */
        IMPORT_C TBool Get(TInt32& aValue) const;
        
        /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */
        IMPORT_C TBool Get(TInt64& aValue) const;
        
		/**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */        
        IMPORT_C TBool Get(TUid& aValue)   const;

        /**
        * Retrieves the boolean value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */
        
        IMPORT_C TBool Get(TUint& aValue) const;
        /**
        * Retrieves the boolean value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */
        IMPORT_C TBool Get(TBool& aValue) const;
        
        
        /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */        
        IMPORT_C TBool Get(TPtrC& aValue)  const;

        //needed at least temporarily for data binding integration...
        //inline TBool Get(const TPtrC** aValue)  const;
          
        /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */        
        IMPORT_C TBool Get(TTime& aValue)  const;
        
        /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */        
        IMPORT_C TBool Get(TPtrC8& aValue) const;
        
        /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */        
        IMPORT_C TBool Get(RFile& aValue) const;

        /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */        
        IMPORT_C TBool Get(CLiwList& aValue) const;
        
        /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */        
        IMPORT_C TBool Get(CLiwMap& aValue) const;

        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */
        IMPORT_C TInt32 AsTInt32() const;

        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */
        IMPORT_C TInt64 AsTInt64() const;

        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */
        IMPORT_C TBool AsTBool() const;
        
        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */
        IMPORT_C TBool AsTUint() const;
        
        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */        
        IMPORT_C TUid AsTUid() const;
        
        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */        
        IMPORT_C TPtrC AsDes() const;
        
        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */        
        IMPORT_C TTime AsTTime() const;
        
        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */        
        IMPORT_C TPtrC8 AsData() const; 
        
        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */        
        IMPORT_C RFile AsFileHandle() const;

        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function default initialized data is returned.
        */        
        IMPORT_C const CLiwList* AsList() const;
        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function default initialized data is returned.
        */        
        IMPORT_C const CLiwMap* AsMap() const;
        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function default initialized data is returned.
        */        
        IMPORT_C MLiwInterface* AsInterface() const;
        
        /**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function default initialized data is returned.
        */        
        IMPORT_C CLiwIterable* AsIterable() const;

        /**
        * Deletes possibly set value and resets this variant to empty.
        *
        * @post IsEmpty()
        */
        IMPORT_C void Reset();

        /**
        * Sets integer value to this variant. The previous value is overwritten.
        *
        * @param aValue Value for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void Set(TInt32 aValue);
        
        /**
        * Sets integer value to this variant. The previous value is overwritten.
        *
        * @param aValue Value for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void Set(TInt64 aValue);
        
        /**
        * Sets Boolean value to this variant. The previous value is overwritten.
        *
        * @param aValue Value for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void Set(TBool aValue);
        
        /**
        * Sets Boolean value to this variant. The previous value is overwritten.
        *
        * @param aValue Value for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void Set(TUint aValue);
        /**
        * Sets unique ID value to this variant. The previous value is overwritten.
        *
        * @param aValue Value for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void Set(const TUid& aValue);
        
        /**
        * Sets date and time value to this variant. The previous value is overwritten.
        *
        * @param aValue Value for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void Set(const TTime& aValue);

        /**
        * Sets constant text reference to this variant. 
        *
        * @param aValue Text value for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void Set(const TDesC& aValue);

        /**
        * Sets constant text reference to this variant. 
        *
        * @param aValue Text value for this variant object to hold. 
        *               Ownership is not taken.
        */
        inline void Set(const HBufC* aValue);

        /**
        * Sets constant 8-bit text reference to this variant.
        * 
        * @param aValue Text value for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void Set(const TDesC8& aValue);

        /**
        * Sets RFile file handle to this variant.
        *
        * @param aValue File handle for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void Set(const RFile& aValue);

        /**
        * Sets list of variants to this variant.
        *
        * @param aValue Array of variants for this variant object to hold.
        * @postcond *this == aValue
        */
        IMPORT_C void Set(const CLiwList* aValue);

        /**
        * Sets map of variants to this variant.
        *
        * @param aValue Dictionary of variants for this variant object to hold.
        * @postcond *this == aValue
        */
        IMPORT_C void Set(const CLiwMap* aValue);

        /**
        * Sets interface pointer to this variant.
        *
        * @param aValue Interface pointer for this variant object to hold.
        * @postcond *this == aValue
        */
        IMPORT_C void Set(const MLiwInterface* aValue);

        /**
        * Sets list of variants to this variant.
        *
        * @param aValue Sequence of variants for this variant object to hold.
        * @postcond *this == aValue
        */
        IMPORT_C void Set(const CLiwIterable* aValue);

	    /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */ 
		IMPORT_C TBool Get(TDes& aValue)  const;
        
        /**
        * Sets the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */ 
		IMPORT_C TBool Get(TDes8& aValue)  const;
	    /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */
        inline TLiwVariant& operator=(const TTime& aValue);

        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */        
        inline TLiwVariant& operator=(const TUid& aValue);
        
        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */        
        inline TLiwVariant& operator=(TInt32 aValue);
        
        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */        
        inline TLiwVariant& operator=(TInt64 aValue);
        
        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */        
        inline TLiwVariant& operator=(TBool aValue);
        
        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */        
        inline TLiwVariant& operator=(TUint aValue);
        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */        
        inline TLiwVariant& operator=(const TDesC& aValue);
        
        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */        
        inline TLiwVariant& operator=(const HBufC* aValue);
        
        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */        
        inline TLiwVariant& operator=(const TDesC8& aValue);
        
        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source object.
        */        
        inline TLiwVariant& operator=(const RFile& aValue);

        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source.
        */        
        inline TLiwVariant& operator=(const CLiwList* aValue);

        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source.
        */        
        inline TLiwVariant& operator=(const CLiwMap* aValue);

        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source.
        */        
        inline TLiwVariant& operator=(const MLiwInterface* aValue);
        
        /**
        * Assignment operator for the variant.
        *
        * @param aValue The source.
        */        
        inline TLiwVariant& operator=(const CLiwIterable* aValue);
        
    public:
        /**
        * Data copying support for TLiwGenericParam.
        * Sets the copy of given variant value to this variant
        * @param aValue variant value for this variant object to hold.
        * @post *this == aValue
        */
        IMPORT_C void SetL(const TLiwVariant& aValue);
        /**
        * Retrieves the value held by this variant.
        *
        * @param aValue If this variant's type does not match the
        *               parameter type, the parameter will not be modified.
        * @return ETrue if aValue was set, EFalse if types did not match.
        */
        IMPORT_C TBool Get(TReal& aValue) const;
        
        /**
		* Sets integer value to this variant. The previous value is overwritten.
		*
		* @param aValue Value for this variant object to hold.
		* @post *this == aValue
		*/
		IMPORT_C void Set(TReal aValue);	
	
		/**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */
        IMPORT_C TReal AsTReal() const;

		 /**
		 * Assignment operator for the variant.
		 *
		 * @param aValue The source object.
		 */        
		 inline TLiwVariant& operator=(TReal aValue);
		 
		 /**
		* Sets integer value to this variant. The previous value is overwritten.
		*
		* @param aValue Value for this variant object to hold.
		* @post *this == aValue
		*/
		IMPORT_C void Set(const CLiwBuffer* aValue);	
	
		/**
        * Returns the value held by this variant.
        *
        * @return The value held by this variant. If the data type does not
        *         match the function, the default initialized data is returned.
        */
        IMPORT_C  CLiwBuffer* AsBuffer() const;

		/**
		* Assignment operator for the variant.
		*
		* @param aValue The source object.
		*/        
		inline TLiwVariant& operator=(const CLiwBuffer* aValue);
		
		/**
        * TLiwVariant cleanup method
        *
        * !!!!!!!!!!!!!!!!!!!!!!!!!!!! 		IMPORTANT NOTE  	!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	    * 
	    * Variants can be pushed into the CleanupStack using this method
	    * along with TCleanupItem
	    *
	    * @ param aObj - TLiwVariant Object but of type TAny* for usage with TCleanupItem
	    * 
	    * Example 1: Here the client explicitly pushes the variant into the CleanupStack using 
	    *  TCleanupItem
	    *
	    * @code
	    * 	 TLiwVariant var;
		*    
		*    //The below statement pushes the variant object "var" into the CleanupStack using TCleanupItem and VariantCleanup.
		*    CleanupStack::PushL( TCleanupItem( TLiwVariant::VariantCleanup , &var ) ); 
    	* 	  
		*    User::Leave(KErrGeneral);  //This calls the VariantCleanup method which cleans up TLiwVariant
		*    CleanupStack::Pop(&var);	//These statements are not executed due to "Leave"
		*    var.Reset(); 
	    * @endcode 
    	*  
	    *  
	    * Example 2: Here the client calls the PushL method of TLiwVariant which pushes the object into 
	    *  the CleanupStack using TCleanupItem
	    * 
	    * @code
	    *    TLiwVariant var;
		*    var.PushL();				//This pushes the variant object "var" into the CleanupStack using TCleanupItem and VariantCleanup.
		*    User::Leave(KErrGeneral);  //This calls the VariantCleanup method which cleans up TLiwVariant
		*    CleanupStack::Pop(&var);	//These statements are not executed due to "Leave"
		*    var.Reset(); 
	    * @endcode 
	    *
	    * The clients can use the convenient PushL method which is recommended (as shown in e.g 2) rather than 
	    *  explicitly pushing the variant object into the CleanupStack using TCleanupItem and VariantCleanup.
    	* 
	    * @ see PushL() method
	    */
        IMPORT_C static void VariantCleanup( TAny* aObj );
		 
		  
		/**
	    * A friendly and convenient method to push the TLiwVariant Object into the CleanupStack
	    * 
	    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! 		IMPORTANT NOTE  	!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	    *
	    * This method informs the CleanupStack that VariantCleanup method should be called incase of 
	    *  any Leave
	    *
	    * @code
	    * 	 TLiwVariant var;
		*    var.PushL();				//This pushes the variant object "var" into the CleanupStack using TCleanupItem and VariantCleanup.
		*    User::Leave(KErrGeneral);  //This calls the VariantCleanup method which cleans up TLiwVariant
		*    CleanupStack::Pop(&var);	//These statements are not executed
		*    var.Reset(); 		
	    * @endcode 
    	*  
	    * This method is recommended than explicitly pushing the parameter object into the CleanupStack 
    	*  using TCleanupItem and VariantCleanup.
    	*
    	* @see VariantCleanup() method
	    */
	    IMPORT_C void PushL();
        
    private:
        /**
        * Streaming support for TLiwGenericParam.
        * Internalizes this variant from a stream.
        */
        void InternalizeL(RReadStream& aStream);

        /**
        * Streaming support for TLiwGenericParam.
        * Externalizes this variant to a stream.
        */
        void ExternalizeL(RWriteStream& aStream) const;

        /**
        * Streaming support for TLiwGenericParam.
        * Returns the maximum externalized size of this variant in bytes.
        */
        TInt Size() const; 

        /**
        * Destroys any dynamic resource owned by this variant.
        */
        void Destroy();

        /// Debug invariant
        void __DbgTestInvariant() const;
        
        static void CleanupDestroy(TAny* aObj);
        
        operator TCleanupItem();
        
    private:  // Data
        /// Type of the data this variant is holding
        TUint8 iTypeId;

        /// POD storage for a 64-bit integer
        struct SInt64
            {
            TInt32 iLow;
            TInt32 iHigh;
            void InternalizeL(RReadStream& aStream);
            void ExternalizeL(RWriteStream& aStream) const;
            void Set(const TInt64& aTInt64);
            operator TInt64() const;
            TBool operator==(const SInt64& aRhs) const;
            };

        // Data value stored in this variant.
        // One of these are effective at a time depending on the iTypeId
        //
        union UData
            {
            TInt32 iInt32;      // 32-bit integer
			TInt64 iLong;      // 64-bit integer
            SInt64 iInt64;      // 64-bit integer
            HBufC* iBufC;       // owned string pointer 
            HBufC8* iBufC8;     // owned 8-bit data pointer 
            CLiwList* iList;    // List
            CLiwMap* iMap;      // Map
            MLiwInterface* iSession;     // Interface Type
            CLiwIterable* iIterable;    // Iterator
            CLiwBuffer* iBuffer;
            TBool  iBool;      //boolean
            TUint  iUint;      //Unsinged Integer
            TReal  iReal;
            };

        UData  iData;  // Data union
        TPtrC  iPtrC;  // Pointer to string
        TPtrC8 iPtrC8; // Pointer to 8-bit data 

    private:  // friend declarations
        friend IMPORT_C TBool operator==(const TLiwVariant& aLhs, const TLiwVariant& aRhs);
        /// TLiwGenericParam needs access to private streaming and copying methods.
        friend class TLiwGenericParam;
        friend class CLiwCriteriaItem;
        
    };

//definitions of new variant types

// CLASS DECLARATION
/**
* A type of variant class to execute a set of service command operations
* on an interface. The service provider should implement MLiwInterface to group
* set of related operations and offer them as service commands to the consumers.
*
* To maintain its internal state across the execution of service commands and 
* enable invocation of operations directly on the interface, service providers
* may offer an interface.
*
* @lib ServiceHandler.lib
*
* @see TLiwVariant
*/
class MLiwInterface
{
 public:
 
  /**
    * The consumer application should call this method to execute a service 
    * command directly on the interface. The service provider should
    * provide a concrete implementation for the supported service
    * command aCmdName. The consumer application should fill in the necessary
    * parameters in \em aInParamList. 
    *
    * @param aCmdName the name of the service command which the consumer wants to invoke
    * @param aInParamList the input parameter list, can be empty list
    * @param [in,out] aOutParamList the output parameter list, can be empty lis. The
    * service provider can use the output parameter list to fill in necessary return values
    * @param aCmdOptions Options for the command, see TLiwServiceCmdOptions in LiwCommon.hrh.
    * @param aCallback callback to be registered by consumer application
    *               
    * 
    *
    * @see TLiwServiceCmdOptions
    * @see CLiwGenericParamList
    * @see MLiwNotifyCallback
    *
   */
  virtual void ExecuteCmdL(const TDesC8& aCmdName,
                           const CLiwGenericParamList& aInParamList,
                           CLiwGenericParamList& aOutParamList,
                           TUint aCmdOptions = 0,
                           MLiwNotifyCallback* aCallback = 0) = 0;
  
  
  /**
    * The service provider should provide a concrete implementation
    * for this method. The internal state of the service provider
    * implemeting \c MLiwInterface can be reset within this method.
    * The consumer application should call this method if there 
    * are no more service commands to be executed on the interface.
    * 
    * 
    *               
    * <p align="center"></p>
    *
   */
  virtual void Close() = 0;
};

/**
* Base class for the framework supported container types.
* \c CLiwContainer is a self managing, reference counted container.
* This class offers minimal interface to manage the reference counter
* and to serialize the container data into the stream.
*
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*
* Since container objects are reference counted, they cannot go with CleanupStack::PushL.
* The issue is solved by using CleanupClosePushL for pushing the container objects into the CleanupStack.
* Hence CLiwContainer::Close method is introduced common to all container objects
*
* All the container data types supported by the framework are
* derived from \c CLiwContainer.
*
* @lib ServiceHandler.lib
*
* @see CLiwContainer::Close
* @see CLiwContainer::PushL
*
* @see CLiwList
* @see CLiwMap
* @see CLiwIterable
*/
class CLiwContainer : public CBase
{
 public:
 
  /**
    * Serializes the collection entries to an output stream.
    * The derving container types from \c CLiwContainer
    * should provide a concrete implementation to serialize the collection
    * data. The default implementation of this method just leaves with
    * an error code \c KErrNotSupported.
    * 
    * @param aOutputStream the output stream to which the data will be streamed
    *               
    * @leave KErrNotSupported whenever this method is called instead
    * of a dervied class implementation of this method
    *
    * @see RWiteStream
    *
   */
  IMPORT_C virtual void ExternalizeL(RWriteStream& aOutputStream) const;
  
  /**
    * Returns the size of collection data values. This default implementation of 
    * this method leaves with an error code \c KErrNotSupported. Typically, the size
    * is calculated based on the size of data type(s) the container holds and the total
    * number of entries present in the collection.
    *
    * @return the size of collection data values
    *               
    * @leave KErrNotSupported whenever this method is called instead
    * of derived class implementation of this method
   */
  IMPORT_C virtual TInt Size() const;
  
  /**
    * Increments the container reference count by one. The reference
    * count gets incremented whenever a copy of the data values
    * is made.
    *
    */
  inline void IncRef();
  
  /**
    * Decrements the container reference count by one. If there
    * are no more references to the container type, this container
    * instance will be deleted.
    *
    */
  inline void DecRef();
  
  /**
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! 		IMPORTANT NOTE  	!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
	*
    * Since reference counting does not work with CleanupStack directly, 
    * CleanupStack::PushL cannot be used for container objects which are reference counted
    * 
    * So CleanupClosePushL method is used for container objects. Hence the need for this method
    * 
    * This method informs the CleanupStack that Close method should be called incase of any Leave
    * 
    * Close() inturn calls DecRef which ensures proper cleanup of container objects
    *
    *
    * Example 1: //Using PushL to push the container object into the CleanupStack using CleanupClosePushL
    * 
    * @code
    *	CLiwMap* map = CLiwDefaultMap::NewL();
    * 	map->PushL();
    * 	map->InsertL(KNullDesC8,variant);	//incase of any leave, CLiwContainer::Close is called
    * 	CleanupStack::PopAndDestroy(map);	//CLiwContainer::Close() is called
    * @endcode 
    *
    * Example 2: //Using NewLC to push the container object into the CleanupStack using CleanupClosePushL
    * 
    * @code
    *	CLiwMap* map = CLiwDefaultMap::NewLC();
    * 	map->InsertL(KNullDesC8,variant);	//incase of any leave, CLiwContainer::Close is called
    * 	CleanupStack::PopAndDestroy(map);   //CLiwContainer::Close() is called
    * @endcode
    *
    * @ see CLiwContainer::NewLC (pushes the container object using CleanupClosePushL)
    * @ see CLiwContainer::PushL (pushes the container object using CleanupClosePushL)
    */
  IMPORT_C void Close(); 
  
  /**
    * !!!!!!!!!!!!!!!!!!!!!!!!!!!! 		IMPORTANT NOTE  	!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
	*
	* A Friendly method to push the CLiwContainer Object into the CleanupStack using CleanupClosePushL
    * 
    * This method informs the CleanupStack that Close method should be called incase of any Leave
    *
    * @code
    * 	CLiwMap* map = CLiwDefaultMap::NewL();
    * 	map->PushL();
    * 	map->InsertL(KNullDesC8,variant);	//incase of any leave, CLiwContainer::Close is called
    * 	CleanupStack::PopAndDestroy(map);	//CLiwContainer::Close() is called
    * @endcode
    *
    * @ see CLiwContainer::Close
    */
  IMPORT_C void PushL();
  
 protected:
   
   /**
    * Protected default constructor. This method is available
    * only for the derived container types. The reference
    * count is set to its default vale.
    * 
    */
  CLiwContainer() : iRefCount(1) {}
  
  /**
    * Destructor.
    *
    */
  IMPORT_C virtual ~CLiwContainer();

 private:
 
  /* Reference counter of the container datatype.
   * During the construction, the counter is set to its
   * default value of 1. When the counter reaches the value
   * of zero (meaning that there are no more references to
   * the container data type), the container instance is deleted.   
   */
  TInt iRefCount;
};

/**
* Abstract interface to iterate over the collection entries.
* The service provider should provide a concrete implementation 
* for the iterator.
*
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Since container objects are reference counted, they cannot go with CleanupStack::PushL.
* The issue is solved by using CleanupClosePushL for pushing the container objects into the CleanupStack.
* Hence CLiwContainer::Close method is introduced common to all container objects
*
* This implies that the object of this concrete implementation class being created should be pushed into the 
* CleanupStack using CLiwContainer::PushL.
*
* @lib ServiceHandler.lib
*
* @see CLiwContainer
* 
* @see CLiwContainer::Close
* @see CLiwContainer::PushL
*
*/
class CLiwIterable : public CLiwContainer
{
 public:
 
 /**
    * Resets the iterator. The service provider should provide a concrete
    * implementation to reset the iterator.
    *
   */
  virtual void Reset() = 0;
  
  /**
    * Iterates over the collection entries to fetch the next data element.
    * The service provider should provide a concrete implementation
    * for this method.
    *
    * @param aEntry contains the next data element and its corresponding data type
    *
    * @return false if there are no more data elements to be fetched;
    * true otherwise
    * 
   */
  virtual TBool NextL(TLiwVariant& aEntry) = 0;
  
  /**
    * Default equality operator implementation
    * 
    * @param aIterable the iterator instance to be compared
    * 
   */
  IMPORT_C virtual TBool operator==(CLiwIterable& aIterable);
};

/**
* Abstract interface for an ordered collection or sequence. Typically, 
* the collection entries are of type \c TLiwVariant instances. 
* \c CLiwList offers abstract methods to:
*  <ul>
*     <li>access elements based on an index</li>
*     <li>remove an element from the collection</li>
*     <li>append to an element to the collection</li>
*     <li>remove a key-value pair based on a key</li>
*		 <li>get the total number of stored key-value pairs</li>
*   </ul>
* A default concrete implementation is provided in the form of specialized class 
* \c CLiwDefaultList
*
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Since container objects are reference counted, they cannot go with CleanupStack::PushL.
* The issue is solved by using CleanupClosePushL for pushing the container objects into the CleanupStack.
* Hence CLiwContainer::Close method is introduced common to all container objects.
* 
* This implies that the object of this concrete implementation class being created should be pushed into the 
* CleanupStack using CLiwContainer::PushL.
*
* @lib ServiceHandler.lib
*
* @see CLiwContainer
* 
* @see CLiwContainer::Close
* @see CLiwContainer::PushL
*
* @see CLiwDefaultList
*
*/
class CLiwList : public CLiwContainer
{
 public:
 
   /**
    * Appends an entry to the end of the collection. This is an abstract
    * method.
    *  
    * @param aEntry an Element to be added to the end of the collection
    *
    * @see CLiwDefaultList::AppendL
   */
  virtual void AppendL(const TLiwVariant& aEntry) = 0;
  
  /**
    * Gets an entry from the collection based on an integeral index.
    *  
    * @param aIndex an index to get an entry from the collection
    * @param aEntry the returned element from the collection
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    *
    * This is a copy of the value present in the list at the particular index. To ensure proper cleanup 
    * TLiwVariant::PushL method should be called beforehand.
	*
    * @return false if no element is found at the passed index;
    * true if an element is found 
    * 
    * @see CLiwDefaultList::AtL
    * @see TLiwVariant::PushL
   */
  virtual TBool AtL(TInt aIndex, TLiwVariant& aEntry) const = 0;
  
  /**
    * Returns the number of elements present in the collection. This
    * is an abstract method.
    *  
    * @return the number of elements; Zero if there are no elements stored
    * 
    * @see CLiwDefaultList::Count
   */
  virtual TInt Count() const = 0;
  
  /**
    * Removes an element located at the passed index from the collection. This
    * is an abstract method.
    *  
    * @param aIndex the index of the element to be removed from the collection
    *
    * @see CLiwDefaultList::Remove
   */
  virtual void Remove(TInt aIndex) = 0;
  
  /**
    * Equality operator implementation for collection container.
    *  
    * @param aList the list container to be compared
    *
    * @return true if the compared list collections are the same; false otherwise
    * 
    * @see CLiwDefaultList::operator==
   */
  IMPORT_C virtual TBool operator==(const CLiwList& aList) const;
};

/**
* Abstract interface for an associative array. The collection
* key entries are of type string (Descriptor type) and their  
* associated values can be an variant type (\c TLiwVariant). 
*
*  \c CLiwMap offers abstract methods to:
*  <ul>
*     <li>insert a key-value pair</li>
*     <li>find a stored value based on a key</li>
*     <li>get a key based on an index</li>
*     <li>remove a key-value pair based on a key</li>
*		 <li>get the total number of stored key-value pairs</li>
*   </ul>
* 
* A default concrete implementation is provided in the form of specialized class 
* \c CLiwDefaultMap
*
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Since container objects are reference counted, they cannot go with CleanupStack::PushL.
* The issue is solved by using CleanupClosePushL for pushing the container objects into the CleanupStack.
* Hence CLiwContainer::Close method is introduced common to all container objects.
* 
* This implies that the object of this concrete implementation class being created should be pushed into the 
* CleanupStack using CLiwContainer::PushL.
*
* @lib ServiceHandler.lib
*
* @see CLiwContainer
* 
* @see CLiwContainer::Close
* @see CLiwContainer::PushL
*
* @see TLiwVariant
* @see CLiwDefaultMap
*
*/
class CLiwMap : public CLiwContainer
{
 public:
 
 /**
    * Inserts a key-value pair element to the map collection. A default
    * implementation is provided in \c CLiwDefaultMap.
    *  
    * @param aKey the key to be stored
    * @param aValue the value associated with the key to be stored
    *
    * @see CLiwDefaultMap::InsertL
   */
  virtual void InsertL(const TDesC8& aKey, const TLiwVariant& aValue) = 0;
 
  /**
    * Finds a value stored in the map collection based on the key. A default
    * implementation is provided in \c CLiwDefaultMap.
    *  
    * @param aKey the key to be searched
    * @param aFndValue the value associated with the found key
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    *
    * This is a copy of the value present in the map for the particular key. To ensure proper cleanup 
    * TLiwVariant::PushL method should be called beforehand.
	*
    * @return false if there is no key stored; true otherwise
    *
    * @see CLiwDefaultMap::FindL
    * @see TLiwVariant::PushL
   */
  virtual TBool FindL(const TDesC8& aKey, TLiwVariant& aFndValue) const = 0;
  
  /**
    * Returns the number of key-value pair stored in the map collection. A default
    * implementation is provided in \c CLiwDefaultMap.
    *  
    * @return the number of key-value pair stored in the map collection
    *
    * @see CLiwDefaultMap::Count
   */
  virtual TInt Count() const = 0;
  
  /**
    * Returns the key stored at the passed index. A default
    * implementation is provided in \c CLiwDefaultMap.
    *  
    * @param aIndex the index of the key to be found
    * @param aFndKey the key found at the passed index
    *
    * @return true if a key-value entry is found at the passed index;
    * false otherwise
    *
    * @see CLiwDefaultMap::AtL
    * 
   */
  virtual TBool AtL(TInt aIndex, TDes8& aFndKey) const = 0;
  
  /**
    * Returns the key stored at the passed index. A default
    * implementation is provided in \c CLiwDefaultMap.
    *  
    * @param aIndex the index of the key to be found
    *
    * @return the key if a key-value entry is found at the passed index;
    * Leaves otherwise
    *
    * @see CLiwDefaultMap::AtL
    * 
   */
  // virtual const TDesC8& AtL(TInt aIndex) const = 0;
  
  /**
    * Removes a key from the map collection. A default
    * implementation is provided in \c CLiwDefaultMap.
    *  
    * @param aKey the key to be removed from the map
    *
    * @see CLiwDefaultMap::Remove
   */
  virtual void Remove(const TDesC8& aKey) = 0;
  
  /**
    * Default implementation of equality operator.
    *  
    * @param aMap the map to be compared
    *
    * @see CLiwDefaultMap::operator==
    * 
   */
  IMPORT_C virtual TBool operator==(const CLiwMap& aMap) const;
};

/**
* An ordered collection or sequence. \c CLiwDefaultList providers
* a default implementation for \c CLiwList. The collection entries are typically 
* of type \c TLiwVarian. 
*
*  This class provides concrete implementation to:
*  <ul>
*     <li>access elements based on index</li>
*     <li>remove an element</li>
*     <li>append an element to the end of the list collection</li>
*		 <li>get the total number of elements stored in the list collection</li>
*   </ul>
*
*
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Since container objects are reference counted, they cannot go with CleanupStack::PushL.
* The issue is solved by using CleanupClosePushL for pushing the container objects into the CleanupStack.
* Hence CLiwContainer::Close method is introduced common to all container objects.
* 
* This implies that the object of this concrete implementation class being created should be pushed into the 
* CleanupStack using CLiwContainer::PushL.
*
* @lib ServiceHandler.lib
*
* @see CLiwContainer
* 
* @see CLiwContainer::Close
* @see CLiwContainer::PushL
*
* @see CLiwList
* @see TLiwVariant
*
*/
class CLiwDefaultList : public CLiwList
{
 public:
  
  /**
    * Creates and returns a new instace of \c CLiwDefaultList
    *
  	* @code
    * void CreateListL()
    * {
    * 	CLiwDefaultList* pList = CLiwDefaultList::NewL();
    *   pList->PushL();	//pushes the List into cleanupstack using CleanupClosePushL
	* 	
	*	CleanupStack::Pop(pList);
	*	pList->DecRef();
	* }			
    *
    * @endcode 
    *
   	* @return an instance of \c CLiwDefaultMap
   	* 
   	* @ see PushL
   	* @ see Close
   	*/
  	IMPORT_C static CLiwDefaultList* NewL();
  
  /**
    * Creates and returns a new instace of \c CLiwDefaultList
    *
  	* @code
    * void CreateListL()
    * {
    * 	CLiwDefaultList* pList = CLiwDefaultList::NewLC(); //pushes the List into cleanupstack using CleanupClosePushL
    *   
	*	CleanupStack::Pop(pList);
	*	pList->DecRef();
	* }			
    *
    * @endcode 
    *
   	* @return an instance of \c CLiwDefaultMap
   	* 
   	* @ see PushL
   	* @ see Close
   	*/
  	IMPORT_C static CLiwDefaultList* NewLC();
  
  /**
    * Creates and returns a new instace of \c CLiwDefaultList. The
    * collection elements will be generated from the data
    * values read from the input stream. This method leaves 
    * the created instance in the cleanup stack.
    *
    * @param aInputStream the input stream containing the data
    * values to append to the list collection
    *
    * @return an instance of \c CLiwDefaultList     
   */
  static CLiwDefaultList* NewLC(RReadStream& aInputStream);
  
  /**
    * Appends an entry to the end of the collection. The entry to be
    * added is of type \c TLiwVariant.
    *  
    * @param aEntry an Element to be added to the end of the collection
    *
    * 
    * A string content being added to the end of the collection
    *
    * @code
    * _LIT8(KLitPbkService,"PhoneBookService");
    * CLiwDefaultList* pList = CLiwDefaultList::NewL();
    * pList->PushL();
    * pList->AppendL(TLiwVariant(KLitPbkService));
    * CleanupStack::Pop(pList);
    * pList->DecRef();
    * @endcode
    *
    * An integer value being added to the end of the collection
    *
    * @code
    * CLiwDefaultList* pList = CLiwDefaultList::NewL();
    * pList->PushL();
    * TInt intVal=0; 
    * pList->AppendL(TLiwVariant(intVal));
    * CleanupStack::Pop(pList);
    * pList->DecRef();
    * @endcode
    * 
    */
  IMPORT_C void AppendL(const TLiwVariant& aEntry);
  
  /**
    * Gets an entry from the collection based on an integeral index.
    *  
    * @param aIndex to get an entry from the collection
    * @param aEntry the returned element from the collection
    *
    * @return false if no element is found at the passed index;
    * true if an element is found 
    *     
    * !!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    *
    * This is a copy of the value present in the list at the particular index. To ensure proper cleanup 
    * TLiwVariant::PushL method should be called beforehand.
	*
    *  
    * An integral value being retrieved from the collection
    * 
    * @code
    * CLiwDefaultList* pList = CLiwDefaultList::NewL();
    * pList->PushL();
    * TInt intVal=1100; //some magic number
    * pList->AppendL(TLiwVariant(intVal));
    *
    * TInt nIndex=0; //only one entry is stored..
    * TLiwVariant valFnd;
    * valFnd.PushL(); //Pushes the TLiwVariant into the CleanupStack using TCleanupItem
    * if(EFalse!=pList->AtL(nIndex,valFnd))
    * {
    * 	TInt intValFnd=-1;
    *   valFnd.Get(intValFnd);
    * }
    * 
    * CleanupStack::PopAndDestroy(valFnd); //Pops and Destroys the TLiwVariant object
    * CleanupStack::Pop(pList);
    * pList->DecRef();
    * @endcode
    *
    * @see TLiwVariant::PushL
    */
  IMPORT_C TBool AtL(TInt aIndex, TLiwVariant& aEntry) const;
  
  /**
    * Returns the number of elements present in the collection.
    *  
    * @return the number of elements; Zero if there are no elements stored
    *
   */
  IMPORT_C TInt Count() const;
  
  /**
    * Removes an element located at the passed index from the collection.
    *  
    * @param aIndex the index of the element to be removed from the collection
    *
    * 
    * An element being removed from the collection
    * 
    * @code
    * CLiwDefaultList* pList = CLiwDefaultList::NewL();
    * pList->PushL();
    * TInt intVal=1100; //some magic number
    * pList->AppendL(TLiwVariant(intVal));
    *
    * TInt nIndex=0; //only one entry is stored..
    * pList->Remove(nIndex);
    *
    * if(0==pList->Count())
    * {
    * 	//should be zero
    * }
    *  
    * CleanupStack::Pop(pList);
    * pList->DecRef();
    * @endcode
    * 
   */
  IMPORT_C void Remove(TInt aIndex);
  
  /**
    * Serializes the collection data entries to an output stream. 
    * 
    * @param aOutputStream the output stream to which the data will be streamed
    *               
    * @see RWiteStream
    */
  IMPORT_C void ExternalizeL(RWriteStream& aOutputStream) const;
  
  /**
    * Returns the size of the collection. The size is 
    * calculated based on the total numer of elements
    * and the size of each element in the collection
    *  
    * @return the size of the collection
   */
  IMPORT_C TInt Size() const;

 private:
 
  /*
   * Constructor
   */
  CLiwDefaultList(CLiwGenericParamList* aList) : iList(aList) {}
  
  /*
   * Destructor
   */
  IMPORT_C virtual ~CLiwDefaultList();
  
  /*
   * the underlying generic parameter list
   */
  CLiwGenericParamList* iList;
};

/**
* An associative array or dictionary class. The collection
* key entries are of type string (Descriptor type) and their  
* associated values can be an variant type.
* 
*  \c CLiwDefaultMap provides default map implementation to
*  <ul>
*     <li>insert a key-value pair</li>
*     <li>find a stored value based on a key</li>
*     <li>get a key based on an index</li>
*     <li>remove a key-value pair based on a key</li>
*		 <li>get the total number of stored key-value pairs</li>
*   </ul>
* 
*
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Since container objects are reference counted, they cannot go with CleanupStack::PushL.
* The issue is solved by using CleanupClosePushL for pushing the container objects into the CleanupStack.
* Hence CLiwContainer::Close method is introduced common to all container objects.
* 
* This implies that the object of this concrete implementation class being created should be pushed into the 
* CleanupStack using CLiwContainer::PushL.
*
* @lib ServiceHandler.lib
*
* @see CLiwContainer
* 
* @see CLiwContainer::Close
* @see CLiwContainer::PushL
*
* @see TLiwVariant
* @see CLiwMap
*
*/
class CLiwDefaultMap : public CLiwMap
{
 public:
  
   /*
   	* Creates an instance of \c CLiwDefaultMap
   	* 
   	* Instance of the map created, can be pushed into the CleanupStack using PushL()
   	* to ensure proper cleanup of this container object
   	*
   	* @code
    * void CreateMapL()
    * {
    * 	CLiwDefaultMap* pMap = CLiwDefaultMap::NewL();
    *   pMap->PushL();	//pushes the map into cleanupstack using CleanupClosePushL
	* 	
	*	CleanupStack::Pop(pMap);
	*	pMap->DecRef();
	* }			
    *
    * @endcode 
    *
   	* @return an instance of \c CLiwDefaultMap
   	* 
   	* @ see PushL
   	* @ see Close
   	*/
    IMPORT_C static CLiwDefaultMap* NewL();
   
   /*
   	* Creates an instance of \c CLiwDefaultMap
   	*
   	* Pushes the map into the CleanupStack using CleanupClosePushL
   	* 
   	* @code
    * void CreateMapL()
    * {
    * 	CLiwDefaultMap* pMap = CLiwDefaultMap::NewLC(); //pushes the map into cleanupstack using CleanupClosePushL
    * 	
	*	CleanupStack::Pop(pMap);
	*	pMap->DecRef();
	* }			
    *
    * @endcode 
    * @return an instance of \c CLiwDefaultMap
    *
    * @ see Close
   	*/
    IMPORT_C static CLiwDefaultMap* NewLC();
  
   /*
   	* Creates an instance of \c CLiwDefaultMap and 
   	* fills up the collection entries from the input stream.
   	* The created instance is left in the cleanupstack.
   	*
   	* @return an instance of \c CLiwDefaultMap   
   	*/
    static CLiwDefaultMap* NewLC(RReadStream& aInputStream);
  
  /**
    * Inserts a key-value pair element to the map collection. If
    * the specified key already exists, it will be removed from
    * the collection and the new key-value pair will be added to the
    * map.
    *  
    * @param aKey the key to be stored
    * @param aValue the value associated with the key to be stored
    *
    * 
    * @code
    * void AddMapEntryL(const TDesC* ptrFirstNameVal)
    * {
    * 	_LIT8(KKey,"firstName");
    * 	TBuf8<128> pbkFieldKey(KKey);
    *
    *	CLiwDefaultMap* pMap = CLiwDefaultMap::NewL();
    *   pMap->PushL();	//pushes the map into cleanupstack using CleanupClosePushL
	* 	pMap->InsertL(pbkFieldKey, TLiwVariant(*ptrFirstNameVal));  
	*   CleanupStack::Pop(pMap);
	*	pMap->DecRef();
	* }			
    *@endcode 
    *
    */
  IMPORT_C void InsertL(const TDesC8& aKey, const TLiwVariant& aValue);
  
  /**
    * Finds a value stored in the map collection based on the key.
    *  
    * @param aKey the key to be searched
    * @param aFndValue the value associated with the found key
    *
    * !!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    *
    * This is a copy of the value present in the map for the particular key. To ensure proper cleanup 
    * TLiwVariant::PushL method should be called beforehand.
	*
    * @return false if there is no key stored; true otherwise
    *
    * 
    *
    * @code
    * void FindAnEntryL()
    * {
    * 	_LIT8(KKey,"firstName");
    * 	TBuf8<128> pbkFieldKey(KKey);
    *   TLiwVariant firstNameVal;
    *	firstNameVal.PushL();
    *
    *	CLiwDefaultMap* pMap = CLiwDefaultMap::NewL();
	*	pMap->PushL();
	* 	if(EFalse!=pMap->FindL(pbkFieldKey, firstNameVal))
	*   {
	*			//key value has been found
	*   }  
	*	
	*	CleanupStack::PopAndDestroy(pMap);
	*	CleanupStack::PopAndDestroy(firstNameVal);
	* }			
    * @endcode 
    *
    * @see TLiwVariant::PushL 
   */
  IMPORT_C TBool FindL(const TDesC8& aKey, TLiwVariant& aValue) const;
  
  /**
    * Returns the number of key-value pair stored in the map collection.
    *  
    * @return the number of key-value pair stored in the map collection
   */
  IMPORT_C TInt Count() const;
  
  /**
    * Returns the key stored at a specified index. 
    *  
    * @param aIndex the index of the key to be found
    * @param aFndKey the key found at the passed index
    *
    * @return true if a key-value entry is found at the passed index;
    * false otherwise
    *
    * 
    *
    * @code
    * void FindAnEntryL()
    * {
    * 	_LIT8(KKey,"firstName");
    * 	TBuf8<128> pbkFieldKeyVal(KKey);
    *   
	* 	TInt nIndex=0;
	*	CLiwDefaultMap* pMap = CLiwDefaultMap::NewL();
	*	pMap->PushL();
	*
	*	if(EFalse!=pMap->AtL(nIndex, pbkFieldKeyVal))
	*   {
	*			//key value has been found
	*   }
	*
	*	CleanupStack::Pop(pMap); 
	*	pMap->DecRef(); 
	* }			
    * @endcode 
    * 
   */
  IMPORT_C TBool AtL(TInt aIndex, TDes8& aFndKey) const;
  
   /**
    * Returns the key stored at a specified index. 
    *  
    * @param aIndex the index of the key to be found
    * @param aFndKey the key found at the passed index
    *
    *
    * @return the key if a key-value entry is found at the passed index;
    * Leaves otherwise
    *
    * 
    *
    * @code
    * void FindAnEntryL()
    * {
    * 	_LIT8(KKey,"firstName");
    * 	TBuf8<128> pbkFieldKeyVal(KKey);
    *   
	* 	TInt nIndex=0;
	*	CLiwDefaultMap* pMap = CLiwDefaultMap::NewL();
	*	pMap->PushL();
	*
	*	pbkFieldKeyVal = pMap->AtL(nIndex);
	*	if(pbkFieldKeyVal)			
	*   {
	*		//key value has been found
	*  	}
	*
	*	CleanupStack::Pop(pMap); 
	*	pMap->DecRef(); 
	* }			
    * @endcode 
    * 
   */
   IMPORT_C const TDesC8& AtL(TInt aIndex) const;
  
  /**
    * Removes a key from the map collection.
    *  
    * @param aKey the key to be removed from the map
    *
    * 
    *
    * @code
    * void RemoveMapEntryL(const TDesC* ptrFirstNameVal)
    * {
    * 	_LIT8(KKey,"firstName");
    * 	TBuf8<128> pbkFieldKey(KKey);
    *	CLiwDefaultMap* pMap = CLiwDefaultMap::NewL();
	*	pMap->PushL();
	*
	* 	pMap->InsertL(pbkFieldKey, TLiwVariant(*ptrFirstNameVal));  
	*	pMap->Remove(pbkFieldKey);
	*
	*	if(0==pMap->Count())
	*	{
	*		//no entries stored; removal successful
	*	}
	*	
	*	CleanupStack::Pop(pMap);
	*	pMap->DecRef();
	* }			
    * @endcode 
    * 
   */
  IMPORT_C void Remove(const TDesC8& aKey);
  
  /**
    * The container managed data values can be serialized to an output
    * stream. 
    * 
    * @param aOutputStream the output stream to which the data will be streamed
    *               
    * 
    *
    * @see RWiteStream
    *
   */
  void ExternalizeL(RWriteStream& aOutputStream) const;
  
  /**
    * Returns the size of the collection. The size is 
    * calculated based on the total numer of key-value elements
    * and the size of each pair in the collection
    *  
    * @return the size of the collection
   */
   IMPORT_C TInt Size() const;

 private:
 
 	/*
 	 * Constructor
 	 */
  CLiwDefaultMap(CLiwGenericParamList* aMap) : iMap(aMap) {}
  
  /*
 	 * Destructor
 	 */
 	 IMPORT_C virtual ~CLiwDefaultMap();
  
  /*
   * the underlying generic parameter list
   */
  CLiwGenericParamList* iMap;
};

/**
* Abstract generic buffer representation. Class used for passing generic buffer data between 
* service provider and the consumers.

* The class offers APIs for the consumers to access the buffer data in a generic manner. Depending on 
* the data agreement between the service provider and its consumers, one 
* or more of the API operations may be inappropriate e.g. a service provider may not 
* support write operation on the buffer
*
* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
* Since container objects are reference counted, they cannot go with CleanupStack::PushL.
* The issue is solved by using CleanupClosePushL for pushing the container objects into the CleanupStack.
* Hence CLiwContainer::Close method is introduced common to all container objects.
* 
* This implies that the object of this concrete implementation class being created should be pushed into the 
* CleanupStack using CLiwContainer::PushL.
*
* @lib ServiceHandler.lib
*
* @see CLiwContainer
* 
* @see CLiwContainer::Close
* @see CLiwContainer::PushL
*
*/
class CLiwBuffer : public CLiwContainer
{
	public:		
		enum TLiwBufferAccess
		{
			KLiwBufferNonReadable = 0,
			KLiwBufferReadOnly = 1
		};


		/*
		 * Returns the pointer to the underlying buffer
		 *
		 * @return the pointer to the underlying buffer
		 */
		virtual TUint8* Buf() const =0;
		
		/*
		 * Returns the length of the buffer data
		 *
		 * @return the length of the buffer data
		 */
		virtual TInt Len() =0;
		
		/*
		 * Reads the specified number of bytes of buffer data 
		 * 
		 * @param aPtr the pointer to the area into which buffer data is read
		 * @param aLength the number of bytes to be read from the buffer
		 *
		 * @return KErrNone if the operation is successful; Otherwise
		 *		   TLiwBufferAccess::KLiwBufferNonReadable
		 *        
		 */
		inline virtual TInt Read(TUint8* /*aPtr*/,TInt /*aLength*/)
		{
			return KLiwBufferNonReadable;
		}
		
		/*
		 * Writes the specified number of bytes into the buffer 
		 * 
		 * @param aPtr the pointer to the data to be written into buffer
		 * @param aLength the number of bytes to be written into buffer
		 *
		 * @return KErrNone if the operation is successful; Otherwise
		 *		   TLiwBufferAccess::KLiwBufferReadOnly if writing is not supported
		 *        
		 */
		inline virtual TInt Write(const TUint8 * /*aPtr*/, TInt /*aLength*/)
		{
			return KLiwBufferReadOnly;
		}
			
		/*
		 * Releases the buffer. The service provider implementing concrete buffer
		 * type should provide appropriate implementation
		 */
		virtual void Release() =0;
		
		/*
		 * Returns the appropriate type of buffer.
		 */
		virtual TInt TypeID()=0;
		
		/*
		 * The concrete implementation of buffer types should perform equality operation
		 * check
		 */
		virtual TBool operator==(CLiwBuffer& aBuffer) =0;
};

/**
* Returns ETrue if two variants are equal.
*
* @param aLhs Left hand side.
* @param aRhs Right hand side.
* @return ETrue if equal, EFalse otherwise.
*/
IMPORT_C TBool operator==(const TLiwVariant& aLhs, const TLiwVariant& aRhs);

/**
* Returns ETrue if two variants are not equal.
*
* @param aLhs Left hand side.
* @param aRhs Right hand side.
* @return ETrue if not equal, EFalse otherwise.
*/
inline TBool operator!=(const TLiwVariant& aLhs, const TLiwVariant& aRhs);

/**
* Dumps the contents of aVariant to debug output. Available only in debug builds.
*
* @param aVariant The object to be dumped.
*/
IMPORT_C void Dump(const TLiwVariant& aVariant);


// INLINE FUNCTIONS
#include "liwvariant.inl"

#endif // LIW_VARIANT_H

// End of File
