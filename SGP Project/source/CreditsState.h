/*
	File:				CreditsState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/5/2012
	Purpose:			State to display the developer credits to the user
*/

#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "BitmapFont.h"

class CCreditsState : public IGameState
{
public:
	// Singleton
	static CCreditsState* GetInstance();

	// IGameState Interface
	virtual void Enter( void );					// enter the state
	virtual void Exit( void );					// exit the state

	virtual bool Input( void );					// handle user input
	virtual void Update( float fElapsedTime );	// update game objects
	virtual void Render( void );				// render game tokens

private:
	// SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pFont;

	// Image IDs:
	int						m_nBackgroundImgID;
	int						m_nBitmapFontImgID;

	// Audio IDs:
	int						m_nBackgroundMusID;

	// Extra Data Members
	float m_fPosX;
	float m_fTextPos[11];

	// Private functions for a secure singleton
	CCreditsState(void);
	virtual ~CCreditsState(void);
	CCreditsState(const CCreditsState&);
	CCreditsState& operator=(const CCreditsState&);
};