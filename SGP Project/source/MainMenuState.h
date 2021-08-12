/*
	File:				MainMenuState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Matthew Berry
	Last Edited:		10/06/2012
	Purpose:			Display a main menu to the player, and have the ability to navigate between the different menus.
*/

#pragma once
#include "IGameState.h"
#include "GameStateManager.h"
#include "AnimationManager.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

class CBitmapFont;

class CMainMenuState : public IGameState
{
public:
	// SINGLETON:
	static CMainMenuState* GetInstance( void );

	// IGameState Interface
	virtual void Enter( void );					// enter the state
	virtual void Exit( void );					// exit the state

	virtual bool Input( void );					// handle user input
	virtual void Update( float fElapsedTime );	// update game objects
	virtual void Render( void );				// render game tokens

	bool GetLoadingGame(void)	const	{ return m_bLoadingGame; }
	void SetLoadingGame(bool bLoad)		{ m_bLoadingGame = bLoad; }

	void SetOptions( bool options )		{ m_bOptions = options; }

private:
	// SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_XAudio2*			m_pXA;

	bool					m_bOptions;
	CBitmapFont*			m_pFont;

	//Particle Manager
	CParticleManager*		m_pPM;

	// Animation Loading Object
	
	
	// Image IDs:
	int						m_nCursorImgID;
	int						m_nBackgroundImgID;
	int						m_nMenuImgID;
	int						m_nLogoImgID;
	int						m_nBitmapFontImageID;
	int						m_nCursorImgID2;

	// Audio IDs:
	int						m_nBackgroundMusID;
	int						m_nSelectionSFXID;
	int						m_nEnterSFXID;
	// Data Members
	int						m_nShadowPosY;
	int						m_nShadowPosX;
	int						m_nPosX;
	int						m_nPosY;
	bool					m_bTitleScreen;

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

	// Load Options ONCE at the start of the game
	bool LoadOptions(const char* szFileName);

	// Bool for Loading a Game
	bool	m_bLoadingGame;

	// Private Functions
	CMainMenuState(void);
	virtual ~CMainMenuState(void);	
	CMainMenuState(const CMainMenuState&);
	CMainMenuState& operator=(const CMainMenuState&);
};