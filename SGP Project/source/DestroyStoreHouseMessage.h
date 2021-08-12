/*
	File:						DestroyStoreHouseMessage.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/27/2012
	Purpose:				Sends a Create Building Message to the Message System
*/

#pragma once
#include "message.h"
#include "StoreHouse.h"

class CDestroyStoreHouseMessage :
	public CMessage
{
private:
	CStoreHouse*		m_pStoreHouse;
public:
	CDestroyStoreHouseMessage(CStoreHouse* pStoreHouse );
	virtual ~CDestroyStoreHouseMessage(void);

	CStoreHouse* GetStoreHouse() const			{ return m_pStoreHouse; }
};

