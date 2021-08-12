/*
	File:				CreateBulletMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Create Bullet Message to the Message System
*/

#include "CreateBulletMessage.h"
#include "Bullet.h"
#include <cassert>

CCreateBulletMessage::CCreateBulletMessage(CMovingObject* pOwner)
	: CMessage(MSG_CREATE_BULLET)
{
	assert(pOwner != nullptr && "CCreateBulletMessage ctor -- Cannot take in a null pointer for the owner.");
	m_pOwner = pOwner;
	m_pOwner->AddRef();
}

CCreateBulletMessage::~CCreateBulletMessage(void)
{
	m_pOwner->Release();
	m_pOwner = nullptr;
}