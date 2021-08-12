/*
	File:						CreateBunkerMessage.cpp
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/26/2012
	Purpose:				Sends a Create Building Message to the Message System
*/


#include "CreateBunkerMessage.h"


CCreateBunkerMessage::CCreateBunkerMessage(int X, int Y, int ID)
	:CMessage( MSG_CREATE_BUNKER )
{
	m_nID = ID;
	m_nPosX = X;
	m_nPosY = Y;
}


CCreateBunkerMessage::~CCreateBunkerMessage(void)
{
}
