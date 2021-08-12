/*
	File:				OptionsState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/8/2012
	Purpose:			A menu to alter various options of the game
*/

#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

class CBitmapFont;

class COptionsState : public IGameState
{
public:
	// Singleton
	static COptionsState* GetInstance(void);

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
	int						m_nCursorImgID;
	int						m_nMenuImgID;
	int						m_nBitmapFontImgID;
	int						m_nCursorImgID2;

	// Audio IDs:
	int						m_nPreviewMusID;
	int						m_nPreviewSfxID;

	// Extra Data Members
	// Struct to have the Volume Settings
	struct tOptions
	{
		// Volume Settings
		double	music;
		double	sfx;

		// Full Screen Setting
		bool	isWindowed;
	};

	tOptions				m_tOptions;

	// Cursor Position
	int						m_nCursorPos;

	int						m_nPosX;
	int						m_nPosY;


	// Position of the Text
	int						m_nTextPos[3];

	// Save/Load Settings
	bool LoadOptions(const char* szFileName);
	void SaveOptions(const char* szFileName);

	// Private functions for a secure Singleton
	COptionsState(void);
	virtual ~COptionsState(void);
	COptionsState(const COptionsState&);
	COptionsState& operator=(const COptionsState&);
};