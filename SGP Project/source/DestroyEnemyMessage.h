/*
	File:				DestroyEnemyMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy Enemy Message to the Message System
*/

#pragma once
#include "Message.h"

class CEnemy;

class CDestroyEnemyMessage : public CMessage
{
public:
	CDestroyEnemyMessage(CEnemy* pEnemy);
	virtual ~CDestroyEnemyMessage(void);

	CEnemy*	GetEnemy(void) const { return m_pEnemy; }

private:
	CEnemy*	m_pEnemy;
};