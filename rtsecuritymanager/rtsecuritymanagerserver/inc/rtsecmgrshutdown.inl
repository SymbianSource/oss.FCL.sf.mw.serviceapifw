/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:      
*
*/






inline CShutDown::CShutDown() :
	CTimer(-1)
	{
	CActiveScheduler::Add (this);
	}
inline void CShutDown::ConstructL()
	{
	CTimer::ConstructL ();
	}
inline void CShutDown::Start()
	{
	After (KShutDownDelay);
	}

inline CShutDown::~CShutDown()
	{
	}

inline void CShutDown::RunL()
	{
	CActiveScheduler::Stop ();
	CActiveScheduler::Install (NULL);
	}