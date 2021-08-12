/*
	File:				DestroyBuildingMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy Building Message to the Message System
*/

#pragma once
#include "Message.h"

class CBuilding;

class CDestroyBuildingMessage :	public CMessage
{
public:
	CDestroyBuildingMessage(CBuilding* pBuilding);
	virtual ~CDestroyBuildingMessage(void);

	CBuilding*	GetBuilding(void) const { return m_pBuilding; }

private:
	CBuilding* m_pBuilding;
};