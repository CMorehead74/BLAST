/*
	File:				CreatePickUpMessage.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Create PickUp Message to the Message System
*/

#include "CreatePickUpMessage.h"
#include "Enemy.h"
#include <cassert>

CCreatePickUpMessage::CCreatePickUpMessage(CEnemy* pOwner)
	: CMessage(MSG_CREATE_PICKUP)
{
	assert(pOwner != nullptr && "CCreatePickUpMessage ctor -- Cannot take in a null pointer for the owner.");
	m_pOwner = pOwner;
	m_pOwner->AddRef();
}

CCreatePickUpMessage::~CCreatePickUpMessage(void)
{
	m_pOwner->Release();
	m_pOwner = nullptr;
}