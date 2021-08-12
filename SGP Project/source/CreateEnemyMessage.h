/*
	File:				CreateEnemyMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Create Enemy Message to the Message System
*/

#pragma once
#include "Message.h"
class CCreateEnemyMessage : public CMessage
{
public:
	CCreateEnemyMessage(void);
	virtual ~CCreateEnemyMessage(void);
};