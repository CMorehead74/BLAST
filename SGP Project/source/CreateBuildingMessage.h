/*
	File:				CreateBuildingMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Create Building Message to the Message System
*/

#pragma once
#include "message.h"
class CCreateBuildingMessage :
	public CMessage
{
	int			m_nType;
public:
	CCreateBuildingMessage(int nType);

	virtual ~CCreateBuildingMessage(void);

	int GetType() const				{ return m_nType; }
};