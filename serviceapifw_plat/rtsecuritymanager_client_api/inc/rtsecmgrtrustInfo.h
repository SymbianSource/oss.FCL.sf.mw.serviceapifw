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
* Description:       Models trust information
 *
*/







#ifndef _CTRUSTINFO_H_
#define _CTRUSTINFO_H_

#include <rtsecmgrcommondef.h>

/**
 * CTrustInfo models trust data structure.
 * 
 * The runtime security manager evaluates trustedness of a 
 * runtime content/script/executable based on the trust information
 * passed by runtimes. The definition of this type is currently
 * incomplete, with state information such as
 *  
 * 	- Origin of the script
 * 	- Certificate chain of the signed trusted contents
 *  
 * @lib rtsecmgrclient.lib
 */
NONSHARABLE_CLASS(CTrustInfo) : public CBase
	{
public:
	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CTrustInfo instance
	 *
	 * @return pointer to an instance of CTrustInfo
	 */
	IMPORT_C static CTrustInfo* NewL();

	/**
	 * Two-phased constructor
	 * 
	 * Constructs a CTrustInfo instance and leaves the created instance
	 * on the cleanupstack
	 *
	 * @return pointer to an instance of CTrustInfo
	 */
	IMPORT_C static CTrustInfo* NewLC();

	/**
	 * Destructor
	 * 
	 */
	IMPORT_C ~CTrustInfo();
private:
	/*
	 * Default private constructor 
	 */
	CTrustInfo();
	};

#endif //_CTRUSTINFO_H_

// End of file
