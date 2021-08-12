/*
	File:				DestroyEnemyMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy Enemy Message to the Message System
*/

#include "DestroyEnemyMessage.h"
#include "Enemy.h"
#include <cassert>

CDestroyEnemyMessage::CDestroyEnemyMessage(CEnemy* pEnemy)
	: CMessage(MSG_DESTROY_ENEMY)
{
	assert(pEnemy != nullptr && "CDestroyEnemyMessage ctor - Cannot take in a null pointer.");
	m_pEnemy = pEnemy;
	m_pEnemy->AddRef();
}

CDestroyEnemyMessage::~CDestroyEnemyMessage(void)
{
	m_pEnemy->Release();
	m_pEnemy = nullptr;
}