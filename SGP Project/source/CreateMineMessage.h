/*
	File:						CreateMineMessage.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/27/2012
	Purpose:				Sends a Create Building Message to the Message System
*/

#pragma once
#include "message.h"
class CCreateMineMessage :
	public CMessage
{
private:
	int					m_nPosX;
	int					m_nPosY;

public:
	CCreateMineMessage(int X, int Y );
	virtual ~CCreateMineMessage(void);

	int GetX()				{ return m_nPosX; }
	int GetY()				{ return m_nPosY; }
};

