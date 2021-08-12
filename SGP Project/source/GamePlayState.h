/*
	File:				GamePlayState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/14/2012
	Purpose:			Contain / manage the game code
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

//typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CGamePlayState : public IGameState, public IListener
{
public:
	// SINGLETON:
	static CGamePlayState* GetInstance( void );

	// Pure virtual methods
	virtual void Enter( void );	// enter the state
	virtual void Exit( void );	// exit the state

	virtual bool Input( void );	// handle user input
	virtual void Update( float fElapsedTime );	// update game objects
	virtual void Render( void );	// render game tokens

	static void MessageProc(IMessage* pMsg);
	void HandleEvent(IEvent* pEvent);

	//Accessor that checks if we are in the tutorial
	bool GetTutorialState()	const	{ return m_bTutotial; }

	//manipulator for the tutorial state boolean
	void SetTutorialState( bool bTutorialState )			{ m_bTutotial = bTutorialState; }

	int GetCurrentLevel() const		{ return m_nCurrLevel; }
	void SetCurrentLevel(int nLvl)	{ m_nCurrLevel = nLvl; }

	//initializing the tutorial level
	void TutorialInit();
	//rendering the tutorial
	void RenderTutorial();
	//updating the tutorial
	void TutorialUpdate( float fElapsedTime );

private:
	int						m_nHurtIndicator;

	// SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_XAudio2*			m_pXA;
	CMessageSystem*			m_pMS;
	ObjectManager*			m_pOM;
	CEventManager*			m_pEM;
	CBitmapFont*			m_pFont;


	CEmitter*			m_pMuzzleFlashEffect;
	// Image IDs:
	int						m_nBackgroundImgID;

	// Enemy IDs
	int						m_nEnemyMedicImgID;
	int						m_nEnemyMGImgID;
	int						m_nEnemyRocketImgID;
	int						m_nEnemySniperImgID;

	int						m_nHelicopterImgID;


	int						m_nCursorPos;

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
	int						m_nRocketImgID;

	// Bitmap Font ID
	int						m_nBitmapFontImgID;

	// Loading Img ID
	int						m_nLoadingLogoBgImgID;
	int						m_nLoadingBulletImgID;

	// Audio IDs:
	int						m_nBackgroundMusID[5];
	int						m_nMusIndex;
	int						m_nDeath;

	int						m_nPercentageLoaded;
	int						m_nPauseSFXID;
	int						m_nShopEnterSFXID;
	int						m_nLostSFXID;
	int						m_nWinSFXID;
	int						m_nChopterSFXID;
	int						m_nEnemyDeathSFXID;
	int						m_nExitStoreSFXID;
	bool					m_bChopExplode;
	int						m_nBulletCollidedSFXID;
	int						m_nRocketCollidedSFXID;


	 bool					m_bRenderCrossHairs;

	//Particle Manager
	CParticleManager*			m_pPM;

	//Tile Manager
	CTileManager*				m_pTileManager;

	//Animation
	CAnimationInfo*				m_pAnimInfo;

	// Data Members
	// Gameplay objects
	CEntity*				m_pPlayer;
	CEntity*				m_pHelicopter;
	CEntity*				m_pShop;

	CMiniMap*				m_pMiniMap;

	float					m_fPickUpSpawnTimer;
	bool					m_bGameOver;
	float					m_fGameOverTimer;

	bool					m_DeathSound;
	bool					m_bGotHighScore;
	bool					m_bPaused;

	// Entering the High Scores
	int	m_nCurrId[3];
	int m_nCurrPos;
	int m_nScorePos;

	int m_nCurrLevel;
	float m_fLevelEndTimer;
	bool m_bLevelComplete;

	struct tScores
	{
		int		nScore;
		char	chName[32];
	};

	vector<tScores>			m_vScores;

	bool CheckScores();

	bool LoadGame(const char* szFileName);
	
	bool LoadScores(vector<tScores>& vScores, const char* szFileName);
	void SaveScores(const vector<tScores>& vScores, const char* szFileName);

	void RenderLoading();

	//Setting up the tutorial level
	bool		m_bTutotial;
	bool		m_bTeacherMode;
	bool		m_bAttackActivate;
	bool		m_bDefenseActivate;
	bool		m_bFollowActivate;
	int			m_nCurrentLesson;
	int			m_nNumEnemies;
	int			m_nNumPickups;
	int			m_nNumStoreHouse;
	int			m_nNumBunkers;
	int			m_nNumMines;

	// Private Functions to keep the Singleton secure
	CGamePlayState(void);
	virtual ~CGamePlayState(void);
	CGamePlayState(const CGamePlayState&);
	CGamePlayState& operator=(const CGamePlayState&);

	void StartDefaultLv1(void);
	void StartLevel2(void);
	void InitAnimations(void);
	void InitImages(void);
	void InitAudio(void);

	void InitPlayer(void);
	void InitHelicopter(void);
};