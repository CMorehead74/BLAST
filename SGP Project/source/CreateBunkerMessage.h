/*
	File:						CreateBunkerMessage.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/26/2012
	Purpose:				Sends a Create Building Message to the Message System
*/

#pragma once
#include "message.h"
class CCreateBunkerMessage :
	public CMessage
{
private:
	int				m_nPosX;
	int				m_nPosY;
	int				m_nID;

public:
	CCreateBunkerMessage(int X, int Y, int ID);
	virtual ~CCreateBunkerMessage(void);

	int GetX()		{ return m_nPosX;}
	int GetY()		{ return m_nPosY; }
	int GetID()	{ return m_nID; }
};

