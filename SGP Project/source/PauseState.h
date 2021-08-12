/*
	File:				PauseState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Matthew Berry
	Last Edited:		10/08/2012
	Purpose:			Pauses the gameplay state.
*/

#pragma once
#include "IGameState.h"

// Wrappers
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "BitmapFont.h"

class CPauseState : public IGameState
{
public:
	// Singleton
	static CPauseState* GetInstance(void);

	// IGameState Interface
	virtual void Enter( void );					// enter the state
	virtual void Exit( void );					// exit the state

	virtual bool Input( void );					// handle user input
	virtual void Update( float fElapsedTime );	// update game objects
	virtual void Render( void );				// render game tokens

	void SetOptionsBool(bool options)	{ m_bOptions = options; }

private:
	// SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pFont;

	// Asset IDs:
	// --  Images:
	int						m_nCursorImgID;
	int						m_nCursorImgID2;

	int						m_nMenuImgID;
	int						m_nBitmapFontImgID;
	// -- Sounds:
	int						m_nSelectionPauseSFXID;
	int						m_nEnterPauseSFXID;

	// Data Members
	int						m_nCursorPos;
	int						m_nTextPos[4];

	int						m_nPosX;
	int						m_nPosY;

	bool					m_bSaveGame;
	bool					m_bOptions;

	void SaveGame(const char* szFileName);

	// Private Functions for a Secure Singleton
	CPauseState(void);
	virtual ~CPauseState(void);
	CPauseState(const CPauseState&);
	CPauseState& operator=(const CPauseState&);
};