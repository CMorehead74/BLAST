/*
	File:						CreateStoreHouseMessage.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/27/2012
	Purpose:				Sends a Create Building Message to the Message System
*/

#pragma once
#include "message.h"
class CCreateStoreHouseMessage :
	public CMessage
{
private:
	int			m_nPosX;
	int			m_nPosY;

public:
	CCreateStoreHouseMessage(int X, int Y );
	virtual ~CCreateStoreHouseMessage(void);

	int GetX()			{ return m_nPosX; }
	int GetY()			{ return m_nPosY; }
};

