/*
	File:				HowToPlayState.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/10/2012
	Purpose:			State to show the player how to play the game
*/

#include "HowToPlayState.h"

// State Machine Includes
#include "Game.h"
#include "GameStateManager.h"
#include "GamePlayState.h"
#include "MainMenuState.h"
#include "PauseState.h"
#include "ShopState.h"

// Game Object Includes
#include "Player.h"
#include "Helicopter.h"
#include "Ally.h"
#include "Enemy.h"
#include "PickUp.h"
#include "Bullet.h"
#include "Shop.h"
#include "Building.h"
#include "Bunker.h"
#include "StoreHouse.h"
#include "Mine.h"

// Message System Includes
#include "Message.h"
#include "CreateAllyMessage.h"
#include "CreateEnemyMessage.h"
#include "CreatePickUpMessage.h"
#include "CreateBulletMessage.h"
#include "DestroyBulletMessage.h"
#include "CreateBuildingMessage.h"
#include "DestroyBuildingMessage.h"
#include "CreateBunkerMessage.h"
#include "DestroyBunkerMessage.h"
#include "CreateStoreHouseMessage.h"
#include "DestroyStoreHouseMessage.h"
#include "CreateMineMessage.h"
#include "DestroyMineMessage.h"

//#include "AnimationInfo.h"
#include "DestroyEnemyMessage.h"
#include "DestroyPickUpMessage.h"
#include "DestroyAllyMessage.h"

//SGD Wrappers
#include "../SGD Wrappers/SGD_Math.h"

//Particle Manager
#include "ParticleManager.h"
//Tile Manager
#include "TileManager.h"
//Camera
#include "Camera.h"
//Bitmap Font
#include "BitmapFont.h"

#include "../tinyxml/tinyxml.h"

#include <assert.h>

CHowToPlayState* CHowToPlayState::GetInstance(void)
{
	static CHowToPlayState s_Instance;
	return &s_Instance;
}

CHowToPlayState::CHowToPlayState(void)
{
	// Assign data members to clean values
	m_pD3D	= nullptr;
	m_pTM	= nullptr;
	m_pDI	= nullptr;
	m_pXA	= nullptr;


	// Image IDs:
	m_nBackgroundImgID	= -1;
	m_nEnemyImgID			= -1;
	m_nHelicopterImgID		= -1;
	m_nAllyMedicImgID		= -1;
	m_nAllyMGImgID			= -1;
	m_nAllySniperImgID		= -1;
	m_nAllyRocketImgID		= -1;
	m_nPickUpAmmoImgID		= -1;
	m_nPickUpCashImgID		= -1;
	m_nPickUpRocketImgID	= -1;
	m_nPickUpMGImgID		= -1;
	m_nPickUpShotgunImgID	= -1;
	m_nPickUpSniperImgID	= -1;
	m_nBulletImgID			= -1;
	m_nBitmapFontImgID		= -1;
}

void CHowToPlayState::Enter(void)
{
	// Access the singletons of the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM  = CSGD_TextureManager::GetInstance();
	m_pDI  = CSGD_DirectInput::GetInstance();
	m_pXA  = CSGD_XAudio2::GetInstance();

	m_pFont = CBitmapFont::GetInstance();

	// Load Images
	m_nBackgroundImgID	= m_pTM->LoadTexture(_T("resource/graphics/instructions.png"), D3DCOLOR_XRGB(0,0,0));

	// Load Bitmap Image
	m_nBitmapFontImgID		= m_pTM->LoadTexture(_T("resource/graphics/SansationFont_0.png"), D3DCOLOR_XRGB(0,0,0));
	m_pFont->SetImageID(m_nBitmapFontImgID);

	CGamePlayState::GetInstance()->SetTutorialState( true );
	CGameStateManager::GetInstance()->PreviousState();
	CGameStateManager::GetInstance()->ChangeState( CGamePlayState::GetInstance() );
}

void CHowToPlayState::Exit(void)
{
	// Unload Images
	if(m_nBackgroundImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBackgroundImgID);
		m_nBackgroundImgID = -1;
	}
	// Unload Audio

	//delete m_pFont;
}

bool CHowToPlayState::Input( void )
{
	if(m_pDI->KeyPressed(DIK_ESCAPE))
	{
		CGameStateManager::GetInstance()->PreviousState();
		//CGameStateManager::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return true;
	}
	return true;
}

void CHowToPlayState::Update( float fElapsedTime )
{

}

void CHowToPlayState::Render( void )
{
	//RECT rSource = { 0, 0, 800, 600 };
	//m_pTM->Draw(m_nBackgroundImgID, 0, 0, 1.0f, 1.0f, &rSource);

// 	m_pFont->PrintString("How to Play", (int)((CGame::GetInstance()->GetWidth() * 0.5f) - 100), 10, 1.5f );
// 
// 	m_pFont->PrintString("Controls", 0, 80 );
// 	m_pFont->PrintString("W - Move forward", 20, 120 );
// 	m_pFont->PrintString("A - Move to the left", 20, 160 );
// 	m_pFont->PrintString("D - Move to the right", 20, 200 );
// 	m_pFont->PrintString("S - Move down", 20, 240 );
// 	m_pFont->PrintString("Left Click - Shoot", 20, 280 );
}

CHowToPlayState::~CHowToPlayState(void)
{
}