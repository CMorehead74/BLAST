/*
	File:				CreateBulletMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Create Bullet Message to the Message System
*/

#include "CreateShotgunBullet.h"
#include "Bullet.h"
#include <cassert>

CCreateShotgunBullet::CCreateShotgunBullet(CMovingObject* pOwner)
	: CMessage(MSG_CREATE_SHOTGUN_BULLET)
{
	assert(pOwner != nullptr && "CCreateShotgunBullet ctor -- Cannot take in a null pointer for the owner.");
	m_pOwner = pOwner;
	m_pOwner->AddRef();
}


CCreateShotgunBullet::~CCreateShotgunBullet(void)
{
	m_pOwner->Release();
	m_pOwner = nullptr;
}