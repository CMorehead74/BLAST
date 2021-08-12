/*
	File:						CreateStoreHouseMessage.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/27/2012
	Purpose:				Sends a Create Building Message to the Message System
*/

#include "CreateStoreHouseMessage.h"


CCreateStoreHouseMessage::CCreateStoreHouseMessage(int X, int Y )
	: CMessage( MSG_CREATE_STOREHOUSE )
{
	m_nPosX = X;
	m_nPosY = Y;
}


CCreateStoreHouseMessage::~CCreateStoreHouseMessage(void)
{
}
