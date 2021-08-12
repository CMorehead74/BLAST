/*
File:				DestroyMineMessage.cpp
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Michael Yatckoske
Last Edited:		9/27/2012
Purpose:			Contain / manage the game code
*/
#pragma once
#include "message.h"
#include "Mine.h"

class CDestroyMineMessage :
	public CMessage
{
private:
	CMine*			m_pMine;

public:
	CDestroyMineMessage(CMine* pMine);
	virtual ~CDestroyMineMessage(void);

	CMine* GetMine()				{ return m_pMine; }
};

