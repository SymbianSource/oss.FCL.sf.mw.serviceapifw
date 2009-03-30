#ifndef LIW_BUFFEREXTENSION_H
#define LIW_BUFFEREXTENSION_H/*
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
* Description:       Abstract bitmap buffer representation.
*
*/







#include <fbs.h>

#include "LiwVariant.h"
class CFbsBitmap;

class CLiwBitmapBuffer : public CLiwBuffer
	{
	public:
		/*
		 * The concrete implementation of buffer types should perform equality operation
		 * check. This is a platform specifc extension class to support bitmap
		 * buffer to abstract CFbsBitmap type.
		 
		 *
		 * @return platform specific bitmap representation. By default, this method
		 * returns NULL
		 */
		virtual CFbsBitmap* AsBitmap()
			{
			return NULL;	// return NULL by default
			} 
	};

/**
 	* Abstract file buffer representation.
	*
**/
class CLiwFileBuffer : public CLiwBuffer
{
	public:
		virtual RFile& AsFile() = 0;
};

#endif