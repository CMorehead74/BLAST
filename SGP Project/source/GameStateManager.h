/*
	File:				GameStateManager.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/6/2012
	Purpose:			Contain / manage the game code
*/

#pragma once
#include "igamestate.h"
#include "GamePlayState.h"
#include "MainMenuState.h"

#include <vector>

class CGameStateManager : public IGameState
{
public:
	// SINGLETON:
	static CGameStateManager* GetInstance( void );

	//Push into Stack
	void NextState(IGameState* pState);
	void PreviousState(void);

	// Game state machine access:
	void ChangeState(IGameState* pNextState);

	bool Input(void);
	void Update(float fElaspsedTime);
	void Render(void);
private:
	std::vector< IGameState* > m_vGameStateStack;
	// Current game state
	IGameState*				m_pCurrState;

	// IGameState Interface:
	virtual void Enter( void ){}		// Enters the state
	virtual void Exit( void ){}			// Exits the state

	CGameStateManager(void);
	virtual ~CGameStateManager(void);
	CGameStateManager( const CGameStateManager& );			
	CGameStateManager& operator=( const CGameStateManager& );
};

