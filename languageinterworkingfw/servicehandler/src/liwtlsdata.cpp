/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:       TLS data object.
*
*/






// INCLUDE FILES
#include "liwtlsdata.h"

// CONSTANTS

const TInt KMenuLaunchObserversGranularity = 2;

// ================= MEMBER FUNCTIONS ==========================================

CLiwTlsData::CLiwTlsData() :
    iMenuLaunchObservers( KMenuLaunchObserversGranularity )
    {
    }

CLiwTlsData::~CLiwTlsData()
    {
    iMenuLaunchObservers.Reset();
    Dll::FreeTls();
    }
    
CLiwTlsData* CLiwTlsData::NewL()
    {
    CLiwTlsData* data = new( ELeave ) CLiwTlsData;
    CleanupStack::PushL( data );
    data->ConstructL();
    CleanupStack::Pop(data); // data
    return data;
    }
    
void CLiwTlsData::ConstructL()
    {
    User::LeaveIfError( Dll::SetTls( this ) );
    }

// -----------------------------------------------------------------------------
// CLiwTlsData::OpenL
// -----------------------------------------------------------------------------
//
CLiwTlsData* CLiwTlsData::OpenL()
    {
    CLiwTlsData* data = Instance();
    if ( !data )
        {
        data = NewL();
        }
    
    data->iRefCount++;
    return data;
    }

// -----------------------------------------------------------------------------
// CLiwTlsData::Close
// -----------------------------------------------------------------------------
//
void CLiwTlsData::Close()
    {
    CLiwTlsData* data = Instance();
    __ASSERT_DEBUG( data, User::Invariant() );
    if ( data )
        {
        if ( --data->iRefCount == 0 )
            {
            delete data; // also frees TLS
            }
        }
    }

// -----------------------------------------------------------------------------
// CLiwTlsData::AddMenuLaunchObserverL
// -----------------------------------------------------------------------------
//
void CLiwTlsData::AddMenuLaunchObserverL( MLiwMenuLaunchObserver* aObserver )
    {
    User::LeaveIfError( iMenuLaunchObservers.Append( aObserver ) );
    }

// -----------------------------------------------------------------------------
// CLiwTlsData::RemoveMenuLaunchObserver
// -----------------------------------------------------------------------------
//   
void CLiwTlsData::RemoveMenuLaunchObserver( MLiwMenuLaunchObserver* aObserver )
    {
    TInt index = iMenuLaunchObservers.Find( aObserver );
    if ( index >= 0 )
        {
        iMenuLaunchObservers.Remove( index );
        }
    }
    
// -----------------------------------------------------------------------------
// CLiwTlsData::ReportMenuLaunch
// -----------------------------------------------------------------------------
//
void CLiwTlsData::ReportMenuLaunch()
    {
    TInt count = iMenuLaunchObservers.Count();
    for ( TInt index = 0 ; index < count ; index++ )
        {
        iMenuLaunchObservers[index]->MenuLaunched();
        }
    }

// End of file
