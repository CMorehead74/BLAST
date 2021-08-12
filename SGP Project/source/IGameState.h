/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/5/2012
	Purpose:			Contain / manage the game code
*/

#pragma once
class IGameState
{
public:
	//Abstract 
	virtual ~IGameState(void) = 0
	{
	}

	// Pure virtual methods
	//	- MUST be overriden by the children
	virtual void Enter( void )	= 0;	// enter the state
	virtual void Exit( void )	= 0;	// exit the state

	virtual bool Input( void )	= 0;	// handle user input
	virtual void Update( float fElapsedTime )	= 0;	// update game objects
	virtual void Render( void ) = 0;	// render game tokens
};

