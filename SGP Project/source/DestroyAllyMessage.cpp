/*
	File:				DestroyAllyMessage.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy Ally Message to the Message System
*/

#include "DestroyAllyMessage.h"
#include "Ally.h"
#include <cassert>

CDestroyAllyMessage::CDestroyAllyMessage(CAlly* pAlly)
	: CMessage(MSG_DESTROY_ALLY)
{
	assert(pAlly != nullptr && "CDestroyAllyMessage ctor -- Cannot take a null pointer.");
	m_pAlly = pAlly;
	m_pAlly->AddRef();
}

CDestroyAllyMessage::~CDestroyAllyMessage(void)
{
	m_pAlly->Release();
	m_pAlly = nullptr;
}