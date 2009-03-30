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
* Description:       A generic parameter class.
*
*/







#ifndef LIW_GENERIC_PARAM_H
#define LIW_GENERIC_PARAM_H

// INCLUDES
#include <e32base.h>
#include "liwgenericparam.hrh"
#include "liwvariant.h"

namespace LIW
{
 /*
 * LIW generic parameter id. This data type should always be used when dealing 
 * with LIW generic parameters. UIDs can be used as LIW generic parameter IDs. 
 * However, values from 0 to 131071 are reserved.
 * @see TLiwGenericParam 
 * @see TGenericParamIdValue
 */ 
typedef TInt TGenericParamId;
}
// CLASS DECLARATION

/**
* Generic parameter class for passing data between applications.
* A generic parameter is a pair of semantic ID and
* variant value. The semantic ID tells the purpose of the parameter,
* for example a file name, URL or phone number. The variant value contains
* the data format and actual value. This class does not implement any
* consistency checks between the semantic ID and value's data format.
* So one semantic ID can be expressed as alternative data formats.
*
* @lib ServiceHandler.lib
* @since Series 60 2.6
* @see TLiwVariant
* @see CLiwGenericParamList
*/
class TLiwGenericParam
    {
    public:  // Constructors and destructor
        /**
        * Constructs a generic parameter.
        */
        inline TLiwGenericParam();
        
        /**
        * Constructs a generic parameter.
        *
        * @param aSemanticId The semantic ID of the parameter, one of TGenericParamId values.
        */        
        inline TLiwGenericParam(LIW::TGenericParamId aSemanticId);
        
        /**
        * Constructs a generic parameter.
        *
        * @param aSemanticId The semantic ID of the parameter, one of TGenericParamId values.
        * @param aValue The parameter value.
        */        
        inline TLiwGenericParam(LIW::TGenericParamId aSemanticId, const TLiwVariant& aValue);

        /**
        * Constructs a generic parameter.
        *
        * @param aName Name of the generic parameter
        * @param aValue The parameter value.
        */ 
        inline TLiwGenericParam(const TDesC8& aName, const TLiwVariant& aValue);

    public:  // Interface
        /**
        * Sets the semantic ID. Possibly set previous ID is overwritten.
        *
        * @param aSemanticId The semantic ID of the parameter.
        */
        inline void SetSemanticId(LIW::TGenericParamId aSemanticId);

        /**
        * Set name. Possibly set previous name is overwritten.
        *
        * @param aName Parameter name.
        */
        inline void SetNameL(const TDesC8& aName);

        /**
        * Set name and value. Possibly set previous name and value is overwritten.
        *
        * @param aName the parameter name.
        *
        * @param aValue the parameter value
        */
        inline void SetNameAndValueL(const TDesC8& aName, const TLiwVariant& aValue);

        /**
        * Returns the semantic ID of this parameter.
        *
        * @return The semantic ID.
        */
        inline LIW::TGenericParamId SemanticId() const;

        /**
        * Returns the name of this parameter.
        *
        * @return The name of the parameter.
        */
        inline const TDesC8& Name() const;

        /**
        * Returns the value of this parameter.
        *
        * @return The value of the parameter.        
        */
        inline TLiwVariant& Value();

        /**
        * Returns the const value of this parameter.
        *
        * @return The const value of the parameter.        
        */
        inline const TLiwVariant& Value() const;

        /**
        * Resets the semantic ID and the value of this parameter.
        */
        inline void Reset();
		
		/**
        * TLiwGenericParam cleanup method
        *
        * !!!!!!!!!!!!!!!!!!!!!!!!!!!! 		IMPORTANT NOTE  	!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
        *
        * Parameters can be pushed into the CleanupStack using this method
        * along with TCleanupItem
        *
        * @ param aObj - TLiwGenericParam Object but of type TAny* for usage with TCleanupItem
        * 
        * Example 1: Here the client explicitly pushes the parameter into the CleanupStack using TCleanupItem
        *
        * @code
	    *	TLiwGenericParam param;
    	*	CleanupStack::PushL( TCleanupItem( TLiwGenericParam::ParamCleanup , &param ) ); 
    	*	User::Leave(KErrGeneral);  //This calls the ParamCleanup method which cleans up TLiwGenericParam 
    	*   CleanupStack::Pop(&param);
    	*	param.Reset();
    	* @endcode 
    	* 
    	* Example 2: Here the client calls the PushL method of TLiwGenericParam which pushes the object into 
    	* the CleanupStack using TCleanupItem
        *
        * @code
        *	TLiwGenericParam param;
    	*	param.PushL();				
    	*	User::Leave(KErrGeneral);  //This calls the ParamCleanup method which cleans up TLiwGenericParam 
    	*   CleanupStack::Pop(&param);
    	*	param.Reset();
    	* @endcode
    	*
    	* The clients can use the convenient PushL method which is recommended (as shown in e.g 2) rather than 
	    *  explicitly pushing the parameter object into the CleanupStack using TCleanupItem and ParamCleanup.
    	* 
	    * @ see PushL() method
        */
        IMPORT_C static void ParamCleanup(TAny* aObj);
        
        /**
        * A friendly and convenient method to push the TLiwGenericParam Object into the CleanupStack
        *
        * !!!!!!!!!!!!!!!!!!!!!!!!!!!! 		IMPORTANT NOTE  	!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
        *
        * This method informs the CleanupStack that ParamCleanup method should be called incase of any Leave
        *
        * Example: 
        *
        * @code
        *	TLiwGenericParam param;
    	*	param.PushL();				//This pushes the parameter object "param" into the CleanupStack using TCleanupItem and ParamCleanup.
    	*	User::Leave(KErrGeneral);  	//This calls the ParamCleanup method which cleans up TLiwGenericParam
    	*   CleanupStack::Pop(&param);	//These statements are not executed
    	*	param.Reset();
    	* @endcode
    	* 
    	* This method is recommended than explicitly pushing the parameter object into the CleanupStack 
    	*  using TCleanupItem and ParamCleanup.
    	*
    	* @see ParamCleanup() method
        */
        IMPORT_C void PushL();

    private:  // Interface for friend classes
        void Destroy();
        void CopyLC(const TLiwGenericParam& aParam);
        static void CleanupDestroy(TAny* aObj);
        operator TCleanupItem();
        void InternalizeL(RReadStream& aStream);
        void ExternalizeL(RWriteStream& aStream) const;
        TInt Size() const; 

    private:  // Data
        /// Own: semantic ID of this parameter
        LIW::TGenericParamId iSemanticId;
        /// Own: value of this parameter
        TLiwVariant iValue;
        /// Reserved member
        TAny* iReserved;

    private:  // friend declarations
        friend class CLiwGenericParamList;
        friend class CLiwDefaultList; 
        friend class CLiwDefaultMap;  
    };


// FUNCTIONS

/**
* Returns ETrue if two generic params are equal.
*
* @param aLhs Left hand side.
* @param aRhs Right hand side.
* @return ETrue if equal, EFalse otherwise.
*/ 
IMPORT_C TBool operator==(const TLiwGenericParam& aLhs, const TLiwGenericParam& aRhs);

/**
* Returns ETrue if two generic params are not equal.
*
* @param aLhs Left hand side.
* @param aRhs Right hand side.
* @return ETrue if not equal, EFalse otherwise.
*/
inline TBool operator!=(const TLiwGenericParam& aLhs, const TLiwGenericParam& aRhs);
        

/**
 * Generic parameter list.
 * A list containing TLiwGenericParam objects. Used for passing parameters 
 * between consumers and providers.
 *
 * @lib ServiceHandler.lib
 * @since Series 60 2.6
 */
NONSHARABLE_CLASS(CLiwGenericParamList): public CBase
    {
    public:   // Constructors and destructor
        /**
        * Creates an instance of this class.
        *
        * @return A pointer to the new instance.
        */
        IMPORT_C static CLiwGenericParamList* NewL();
        
        /**
        * Creates an instance of this class.
        *
        * @param aReadStream A stream to initialize this parameter list from.
        * @return A pointer to the new instance.
        */        
        IMPORT_C static CLiwGenericParamList* NewL(RReadStream& aReadStream);

        /**
        * Creates an instance of this class. Leaves the created instance on the
        * cleanup stack.
        *
        * @return A pointer to the new instance.
        */
        IMPORT_C static CLiwGenericParamList* NewLC();
        
        /**
        * Creates an instance of this class. Leaves the created instance on the
        * cleanup stack.
        *
        * @param aReadStream A stream to initialize this parameter list from.
        * @return A pointer to the new instance.
        */        
        IMPORT_C static CLiwGenericParamList* NewLC(RReadStream& aReadStream);

        /**
        * Destructor.
        */
        virtual ~CLiwGenericParamList();
        
    public:  // Interface
        /**
        * Returns the number of parameters in the list. 
        *
        * @return The number of parameters in the list. 
        */
        IMPORT_C TInt Count() const;
        
        /**
        * Returns the number of the parameters in the list by semantic id and datatype.
        *
        * @param aSemanticId The semantic ID of the parameter.
        * @param aDataType The type id of data. Default is any type.
        * @return The number of parameters in the list by semantic id and datatype.
        */
        IMPORT_C TInt Count(LIW::TGenericParamId aSemanticId, 
                LIW::TVariantTypeId aDataType = LIW::EVariantTypeAny) const;  
            
        /**
        * Returns a parameter from this list.
        *
        * @param aIndex Index of the parameter.
        * @return The parameter at the aIndex position.
        * @pre aIndex>=0 && aIndex<Count()
        */
        IMPORT_C const TLiwGenericParam& operator[](TInt aIndex) const;
        
        /**
	    * Gets an entry from the collection based on an integeral index.
	    * Note ! This method allocates memory. To ensure proper cleanup you shall call PushL method beforehand.
	    *  
	    * @param aIndex an index to get an entry from the collection
	    * @param aEntry the returned element from the collection.  
	    *
	    * !!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	    *
	    * This is a copy of TLiwGenericParam present in the list at the particular index. To ensure proper cleanup 
	    * TLiwGenericParam::PushL method should be called beforehand.
	    *
	    * @code
	    * CLiwGenericParamList* param = CLiwGenericParamList::NewLC();
		* //... param is filled with some semantic Id and TLiwVariant value ...
		*
		* TLiwGenericParam outParam;
 	    * outParam.PushL();			
 	    * //This pushes the parameter object "outParam" into the CleanupStack using TCleanupItem and ParamCleanup.
		*
		* param->AtL(0,outParam);	
		* //This fills the "outParam" with the parameter value present in the 0th position of parameter list, but
		* //if there is any Leave in AtL, then ParamCleanup method is called for proper cleanup
		*
		* CleanupStack::Pop(&outParam);	
		* outParam.Reset();
		* CleanupStack::PopAndDestroy(param);
		* @endcode
		*
	    * @return false if no element is found at the passed index;
	    * true if an element is found 
	    * 
	    * @see CLiwDefaultList::AtL
	    * @see TLiwGenericParam::PushL
	    */
		IMPORT_C void AtL(TInt aIndex, TLiwGenericParam& aParam) const;

        /**
        * Appends a parameter to this list.
        *
        * @param aParam The parameter to append to this list. This object takes
        *               an own copy of the data in aParam.
        */
        IMPORT_C void AppendL(const TLiwGenericParam& aParam);

        /**
        * Copies the given list and appends it to end of this list.
        *
        * @since Series 60 2.8        
        * @param aList A list to be copied and appended.
        */
        IMPORT_C void AppendL(const CLiwGenericParamList& aList);
        
        /**
        * Removes the first found item with given semantic id from the list.
        * 
        * @param aSemanticId Semantic id for the item to be removed.
        * @return  ETrue  if an item for the given semantic id was found and removed.
        *          EFalse otherwise.
        */
        IMPORT_C TBool Remove(TInt aSemanticId);
        
        /**
        * Removes the first found item with given string based semantic id from the list.
        * 
        * @param aSemanticId Semantic id for the item to be removed.
        * @return  ETrue  if an item for the given semantic id was found and removed.
        *          EFalse otherwise.
        */
        IMPORT_C void Remove(const TDesC8& aName);

        /**
        * Deletes all parameters in the list and resets the list.
        */
        IMPORT_C void Reset();

        /**
        * Returns the first item matching the given semantic ID.
        *
        * @param aIndex Position in which to start searching. On return it contains 
        *               the position of the found parameter. It is set to KErrNotFound, 
        *               if no matching items were found.
        * @param aSemanticId The semantic ID of the parameter.
        * @param aDataType The type id of data. Default is any type.
        * @return The first matching item.
        */
        IMPORT_C const TLiwGenericParam* FindFirst(
            TInt& aIndex, 
            LIW::TGenericParamId aSemanticId,
            LIW::TVariantTypeId aDataType = LIW::EVariantTypeAny) const;
        /**
        * Returns the next item matching the given semantic ID.
        * 
        * @param aIndex Position after which to start searching. On return it contains 
        *               the position of the found parameter. It is set to KErrNotFound, 
        *               if no matching items were found.
        * @param aSemanticId The semantic ID of the parameter.
        * @param aDataType The type id of data. Default is any type. 
        * @return The next matching item.       
        */
        IMPORT_C const TLiwGenericParam* FindNext(
            TInt& aIndex, 
            LIW::TGenericParamId aSemanticId,
            LIW::TVariantTypeId aDataType = LIW::EVariantTypeAny) const;
        
        /**
		* Returns item matching the given name.
        *
        * @param aIndex Position in which to start searching, on return contains 
        *               position of the found parameter. Is set to KErrNotFound, 
        *               if no matching items were found.
        * @param aName Parameter name.
        * @param aDataType The type id of data. Default is any type.
        * @return The first matching item.
        */
        IMPORT_C const TLiwGenericParam* FindFirst(
            TInt& aIndex, 
            const TDesC8& aName,
            LIW::TVariantTypeId aDataType = LIW::EVariantTypeAny) const;
            
        /**
        * Returns item matching the given name.
        *
        * @param aIndex Position in which to start searching, on return contains 
        *               position of the found parameter. Is set to KErrNotFound, 
        *               if no matching items were found.
        * @param aName Parameter name.
        * @param aDataType The type id of data. Default is any type.
        * @return The first matching item.
        */
        IMPORT_C const TLiwGenericParam* FindNext(
            TInt& aIndex, 
            const TDesC8& aName,
            LIW::TVariantTypeId aDataType = LIW::EVariantTypeAny) const;

        /**
        * Externalizes this parameter list to a stream.
        *
        * @param aStream The stream. 
        * @see NewL(RReadStream& aStream)
        * @see NewLC(RReadStream& aStream)
        */
        IMPORT_C void ExternalizeL(RWriteStream& aStream) const;

        /**
        * Returns the externalized size of the parameter list in bytes.
        *
        * @return The size.
        */
        IMPORT_C TInt Size() const;

        /**
        * Internalizes the parameter list from a stream.
        *
        * @since Series60 2.8
        * @param aStream The stream.
        */
        IMPORT_C void InternalizeL(RReadStream& aStream);

        /**
        * Packs the parameter list to TIpcArgs structure for
        * passing the generic param list to server over process boundary.
        * Only one RFile handle parameter can be passed over process boundary.
        *
        * @since Series60 3.0
        * @param aArgs Inter process call arguments.
        * @return Packed parameter list.
        */
        IMPORT_C HBufC8* PackForServerL(TIpcArgs& aArgs);

        /**
        * Unpacks the list from client message structure.
        *
        * @since Series60 3.0
        * @param aArgs The list to be unpacked. 
        */
        IMPORT_C void UnpackFromClientL(const RMessage2& aArgs);
        
    private:  // Implementation
        CLiwGenericParamList();
        void ConstructL();
        void AppendL(RReadStream& aReadStream);
        
    private:  // Data
        /// Own: the parameter list
        RArray<TLiwGenericParam> iParameters;
        friend class CLiwDefaultList;  
        friend class CLiwDefaultMap;   
     };


// INLINE FUNCTIONS
#include "liwgenericparam.inl"

#endif // LIW_GENERIC_PARAM_H

// End of File
