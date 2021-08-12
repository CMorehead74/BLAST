/*
	File:				CreateEnemyMessage.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Create Enemy Message to the Message System
*/

#include "CreateEnemyMessage.h"

CCreateEnemyMessage::CCreateEnemyMessage(void)
	: CMessage(MSG_CREATE_ENEMY)
{
}

CCreateEnemyMessage::~CCreateEnemyMessage(void)
{
}