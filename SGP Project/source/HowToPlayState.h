/*
	File:				HowToPlayState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/8/2012
	Purpose:			State to show the player how to play the game
*/

#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/IListener.h"
#include "MessageSystem.h"
#include "ObjectManager.h"
#include "EventManager.h"
#include "BitmapFont.h"

// Mini Map
#include "MiniMap.h"


#include "ParticleManager.h"
#include "AnimationManager.h"
//#include "AnimationInfo.h"

// Forward Declarations
//Tile Manager
class CTileManager;

#include "Entity.h"

//Bitmap Font class declaration
class CBitmapFont;

class CHowToPlayState :	public IGameState
{
public:
	// Singleton
	static CHowToPlayState* GetInstance(void);

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

	// Image IDs:
	int						m_nBackgroundImgID;

	// Enemy IDs
	int						m_nEnemyImgID;
	int						m_nHelicopterImgID;

	// Ally IDs
	int						m_nAllyMedicImgID;
	int						m_nAllyMGImgID;
	int						m_nAllySniperImgID;
	int						m_nAllyRocketImgID;

	// PickUp IDs
	int						m_nPickUpAmmoImgID;
	int						m_nPickUpCashImgID;
	int						m_nPickUpRocketImgID;
	int						m_nPickUpMGImgID;
	int						m_nPickUpShotgunImgID;
	int						m_nPickUpSniperImgID;

	// Bullet IDs
	int						m_nBulletImgID;

	// Bitmap Font ID
	int						m_nBitmapFontImgID;

	// Loading Img ID
	int						m_nLoadingLogoBgImgID;
	int						m_nLoadingBulletImgID;

	// Audio IDs:
	int						m_nBackgroundMusID;
	int						m_nDeath;

	// Private Functions for a secure Singleton
	CHowToPlayState(void);
	virtual ~CHowToPlayState(void);
	CHowToPlayState(const CHowToPlayState&);
	CHowToPlayState& operator=(const CHowToPlayState&);

	//Bitmap Font
	CBitmapFont*				m_pFont;

	// Gameplay objects
	CEntity*				m_pPlayer;
	CEntity*				m_pHelicopter;
};