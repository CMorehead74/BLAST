/*
	File:				DestroyPickUpMessage.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy PickUp Message to the Message System
*/

#include "DestroyPickUpMessage.h"
#include "PickUp.h"
#include <cassert>

CDestroyPickUpMessage::CDestroyPickUpMessage(CPickUp* pPickUp)
	: CMessage(MSG_DESTROY_PICKUP)
{
	assert(pPickUp != nullptr && "CDestroyPickUpMessage ctor - Cannot take in a null pointer.");
	m_pPickUp = pPickUp;
	m_pPickUp->AddRef();
}

CDestroyPickUpMessage::~CDestroyPickUpMessage(void)
{
	m_pPickUp->Release();
	m_pPickUp = nullptr;
}