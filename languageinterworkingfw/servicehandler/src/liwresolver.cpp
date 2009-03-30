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
* Description:       Implementation of Custom ECom Resolver for LIW.
*
*/






#include "liwresolver.h"

const TInt KMaxDataItemSize = 150;
_LIT8(KContentTag, "<CONTENT>");
_LIT8(KOpaqueTag, "<OPAQUE>");


CLiwResolver* CLiwResolver::NewL(MPublicRegistry& aRegistry)
    {
    return new (ELeave) CLiwResolver(aRegistry);
    }


CLiwResolver::CLiwResolver(MPublicRegistry& aRegistry) : CResolver(aRegistry)
    {
    // Nothing to do.
    }


CLiwResolver::~CLiwResolver()
    {
    if (iImplementationInfoArray)
        {
        iImplementationInfoArray->Reset();  
        delete iImplementationInfoArray;
        }
    }



TUid CLiwResolver::IdentifyImplementationL(TUid aInterfaceUid, 
    const TEComResolverParams& aAdditionalParameters) const
    {
    RImplInfoArray& implementationsInfo = iRegistry.ListImplementationsL(aInterfaceUid);
    TUid found = KNullUid;

    if(implementationsInfo.Count())
        {
        found = Resolve(implementationsInfo, aAdditionalParameters);
        }

    return found;
    }



RImplInfoArray* CLiwResolver::ListAllL(TUid aInterfaceUid, 
    const TEComResolverParams& aAdditionalParameters) const
    {
    // Use the member var to create the array so that we get proper cleanup behaviour
    delete iImplementationInfoArray;
    iImplementationInfoArray = NULL;
    iImplementationInfoArray = new (ELeave) RImplInfoArray;
    RImplInfoArray* retList = iImplementationInfoArray;

    RImplInfoArray& fullList = iRegistry.ListImplementationsL(aInterfaceUid);

    const TBool useWildcards = aAdditionalParameters.IsWildcardMatch();
    TBuf8<KMaxDataItemSize> content;
    TBuf8<KMaxDataItemSize> opaque;

    ParseInput(aAdditionalParameters.DataType(), content, opaque);
    const TInt numImps = fullList.Count();

    for (TInt index = 0; index < numImps; ++index)
        {
        if (Match(fullList[index]->DataType(), content, useWildcards) &&
            MatchServiceCmd(fullList[index]->OpaqueData(), opaque))
            {
            User::LeaveIfError(retList->Append(fullList[index]));
            }
        }

    // Reset the member variable because we are passing ownership back
    iImplementationInfoArray = NULL;

    return retList;
    }



void CLiwResolver::ParseInput(const TDesC8& aParam, TDes8& aContent, TDes8& aOpaque) const
    {
    TInt cind = aParam.Find(KContentTag);
    TInt oind = aParam.Find(KOpaqueTag);
        
    if (cind != KErrNotFound)
        {
        if (oind != KErrNotFound)
            {
            aContent.Copy(aParam.Mid(cind + (&KContentTag)->Length(), 
                                     oind - (cind + (&KContentTag)->Length())));
            }
        else
            {
            aContent.Copy(aParam.Mid(cind + (&KContentTag)->Length()));
            }
        }

    if (oind != KErrNotFound)
        {
        aOpaque.Copy(aParam.Mid(oind + (&KOpaqueTag)->Length()));
        }
    }



TUid CLiwResolver::Resolve(const RImplInfoArray& aImplementationsInfo, 
    const TEComResolverParams& aAdditionalParameters) const
    {
    // Loop through the implementations matching on type
    const TInt count = aImplementationsInfo.Count();

    for (TInt index = 0; index < count; ++index)
        {
        const CImplementationInformation& impData = *aImplementationsInfo[index];
        // As soon as we get a match on the datatype then return uid of the
        // implementation found.
        if (Match(impData.DataType(),                   // The Datatype of this implementation
            aAdditionalParameters.DataType(),           // The type we are trying to find
            aAdditionalParameters.IsWildcardMatch()))   // If wildcards should be used
            {
            return impData.ImplementationUid();
            }
        }

    return KNullUid;
    }


TBool CLiwResolver::Match(const TDesC8& aImplementationType, const TDesC8& aMatchType, 
    TBool aUseWildcards) const
    {
    TInt matchPos = KErrNotFound;

    _LIT8(dataSeparator, "||");
    const TInt separatorLength = dataSeparator().Length();

    // Look for the section separator marker '||'
    TInt separatorPos = aImplementationType.Find(dataSeparator);

    if (separatorPos == KErrNotFound)
        {
        // Match against the whole string
        if (aUseWildcards)
            {
            matchPos = aImplementationType.Match(aMatchType);
            }
        else
            {

            if (aImplementationType.Compare(aMatchType) == 0)
                {
                matchPos = KErrNone;    
                }
            }
        }
    else
        {
        // Find the first section, up to the separator
        TPtrC8 dataSection = aImplementationType.Left(separatorPos);
        TPtrC8 remainingData = aImplementationType.Mid(separatorPos + separatorLength);

        // Match against each section in turn
        while (separatorPos != KErrNotFound)
            {
            // Search this section
            if (aUseWildcards)
                {          
                matchPos = dataSection.Match(aMatchType);
                }
            else
                {
                matchPos = dataSection.Compare(aMatchType);
                }

            // If we found it then no need to continue, so return
            if (matchPos != KErrNotFound)
                {
                return ETrue;
                }

            // Move on to the next section
            separatorPos = remainingData.Find(dataSeparator);

            if (separatorPos != KErrNotFound)
                {
                dataSection.Set(remainingData.Left(separatorPos));
                remainingData.Set(remainingData.Mid(separatorPos + separatorLength));
                }
            else
                {
                dataSection.Set(remainingData);
                }   
            }

        // Check the final part
        if (aUseWildcards)
            {
            matchPos = dataSection.Match(aMatchType);
            }
        else
            {
            matchPos = dataSection.Compare(aMatchType);
            }

        }

    return matchPos != KErrNotFound;
    }




TBool CLiwResolver::MatchServiceCmd(const TDesC8& aOpaqueData, const TDesC8& aServiceCmd) const
   {
   _LIT8(KWild,"*");
   
   //check for wildcard character *
   //if yes, return immediatly
   if(0==aServiceCmd.Compare(KWild))
   	return ETrue;
   
   // Extract List Of service command from OpaQue Data
    _LIT8(MetadataSeparator, "::");
    TPtrC8 dataSection;
    TInt MetadataSeparatorPos = aOpaqueData.Find(MetadataSeparator);
    if (MetadataSeparatorPos != KErrNotFound)
    { 
        dataSection.Set(aOpaqueData.Left(MetadataSeparatorPos));
    }
    else
    {
        dataSection.Set(aOpaqueData);
    }
    
    _LIT8(dataSeparator, "||");
    const TInt separatorLength = dataSeparator().Length();

    // Look for the section separator marker '||'
    TInt separatorPos = dataSection.Find(dataSeparator);

    if (separatorPos == KErrNotFound)
        {
         if (aServiceCmd.Compare(dataSection) == 0)
            {
            return ETrue;   
            }
        }
    else
        {
         // Find the first section, up to the separator
        TPtrC8 remainingData = dataSection.Mid(separatorPos + separatorLength);
        dataSection.Set(dataSection.Left(separatorPos));

        // Match against each section in turn
        while (separatorPos != KErrNotFound)
            {
            if (dataSection.Compare(aServiceCmd) == 0)
                {
                return ETrue;
                }

            // Move on to the next section
            separatorPos = remainingData.Find(dataSeparator);

            if (separatorPos != KErrNotFound)
                {
                dataSection.Set(remainingData.Left(separatorPos));
                remainingData.Set(remainingData.Mid(separatorPos + separatorLength));
                }
            else
                {
                dataSection.Set(remainingData);
                }   
            }

        if (dataSection.Compare(aServiceCmd) == 0)
            {
            return ETrue;   
            }       
        }

    return EFalse;
    }

// Map the interface UIDs
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(KLiwResolverImplUidValue, CLiwResolver::NewL)
    };

// Exported proxy for instantiation method resolution
// ---------------------------------------------------------
//
//
// ---------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
    {
    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }

// End of file

