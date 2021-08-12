/*
	File:						CreateMineMessage.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/27/2012
	Purpose:				Sends a Create Building Message to the Message System
*/

#include "CreateMineMessage.h"


CCreateMineMessage::CCreateMineMessage(int X, int Y )
	: CMessage( MSG_CREATE_MINE )
{
	m_nPosX = X;
	m_nPosY = Y;
}


CCreateMineMessage::~CCreateMineMessage(void)
{
}
