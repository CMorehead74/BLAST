/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/5/2012
	Purpose:			Contain / manage the game code
*/

#pragma once
#include "ibaseaistate.h"
class CIdleState :
	public IBaseAIState
{
public:
	CIdleState(void);
	virtual ~CIdleState(void);

	void Update(float fElapsedTime, CMovingObject* pObject);
	void ProcessIdleState(float fElapsedTime, CMovingObject* pObject);
};

