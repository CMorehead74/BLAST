/*
	File:				DestroyBulletMessage.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy Bullet Message to the Message System
*/

#include "DestroyBulletMessage.h"
#include "Bullet.h"
#include <cassert>

CDestroyBulletMessage::CDestroyBulletMessage(CBullet* pBullet)
	: CMessage(MSG_DESTROY_BULLET)
{
	assert(pBullet != nullptr && "CDestroyBulletMessage ctor - Cannot take in a null pointer.");
	m_pBullet = pBullet;
	m_pBullet->AddRef();
}

CDestroyBulletMessage::~CDestroyBulletMessage(void)
{
	m_pBullet->Release();
	m_pBullet = nullptr;
}