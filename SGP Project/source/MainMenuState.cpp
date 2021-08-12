/*
File:				MainMenuState.cpp
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Naveen Anand Gunalan
Last Edited:		9/10/2012
Purpose:			Display a main menu to the player, and have the ability to navigate between the different menus.
*/

#include "MainMenuState.h"

// State Machine Headers
#include "Game.h"
#include "GamePlayState.h"
#include "OptionsState.h"
#include "HowToPlayState.h"
#include "HighScoreState.h"
#include "CreditsState.h"
#include "TitleState.h"
#include "FileManagementState.h"

// Bitmap Font
#include "BitmapFont.h"

// Tiny XML Header
#include "../tinyxml/tinyxml.h"

#include "ParticleManager.h"

// SINGLETON!
CMainMenuState* CMainMenuState::GetInstance( void )
{
	// Lazy instantiation
	//	- created the first time the function is called
	static CMainMenuState s_Instance;

	return &s_Instance;
}

CMainMenuState::CMainMenuState(void)
{
	m_pD3D = nullptr;
	m_pTM  = nullptr;
	m_pDI  = nullptr;
	m_pXA  = nullptr;
	m_pFont = nullptr;

	m_bOptions = false;

	//Textures
	m_nCursorImgID		= -1;
	m_nBackgroundImgID	= -1;
	m_nMenuImgID		= -1;

	// Audio
	m_nBackgroundMusID	= -1;
	m_nSelectionSFXID	= -1;
	m_nEnterSFXID		= -1;

	m_bTitleScreen		= true;

	//Position of the staring menu
	m_nPosX = 565;
	m_nPosY = 235;
	m_nShadowPosY = 235;
	m_nShadowPosX = 565;
}

void CMainMenuState::Enter(void)
{
	//Access the singletons of the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM  = CSGD_TextureManager::GetInstance();
	m_pDI  = CSGD_DirectInput::GetInstance();
	m_pXA  = CSGD_XAudio2::GetInstance();

	m_pFont = CBitmapFont::GetInstance();

	//Using the texture manager to load the images
	m_nCursorImgID			= m_pTM->LoadTexture(_T("resource/graphics/Blast_MenuCursorHover.png"), D3DCOLOR_XRGB(0,0,0)); 
	m_nBackgroundImgID		= m_pTM->LoadTexture(_T("resource/graphics/instructions.png"));
	m_nMenuImgID			= m_pTM->LoadTexture(_T("resource/graphics/Blast_Menu_Text_Bg.png"));
	m_nLogoImgID			= m_pTM->LoadTexture(_T("resource/graphics/Blast_Logo.png"));

	m_nCursorImgID2			= m_pTM->LoadTexture(_T("resource/graphics/Blast_Mouse_Cursor_PlacementEditor.png"), D3DCOLOR_XRGB(255,255,255));

	// Load Bitmap Image
	m_nBitmapFontImageID	= m_pTM->LoadTexture(_T("resource/graphics/SansationFont_0.png"), D3DCOLOR_XRGB(0,0,0));
	m_pFont->SetImageID(m_nBitmapFontImageID);

	m_bOptions = false;

	// Load the Audio:
	m_nBackgroundMusID		= m_pXA->MusicLoadSong(_T("resource/sounds/MUS_MenuBackgroundMain.xwm"));
	m_nSelectionSFXID		= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_SELECTHIGHLIGHT.wav"));
	m_nEnterSFXID			= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_SELECTENTER.wav"));



	CGame* pGame = CGame::GetInstance();

#pragma region PARTICLES	
	m_pPM   = CParticleManager::GetInstance();
	m_pPM->RegisterEmitter("test2.xml");	
#pragma endregion 

	m_bLoadingGame = false;

	// Load the Options at the start of the game.
	if(LoadOptions("options.xml") == false)
	{
		pGame->SetWindowed(true);

		m_pXA->MusicSetMasterVolume((float)100/100);
		m_pXA->SFXSetMasterVolume((float)100/100);
	}
	else
	{
		pGame->SetWindowed(m_tOptions.isWindowed);
		m_pD3D->ChangeDisplayParam(pGame->GetWidth(), pGame->GetHeight(), pGame->IsWindowed());

		m_pXA->MusicSetMasterVolume((float)m_tOptions.music/100);
		m_pXA->SFXSetMasterVolume((float)m_tOptions.sfx/100);
	}

	m_pXA->MusicPlaySong(m_nBackgroundMusID, true);

	if(m_bTitleScreen == true)
	{
		CGameStateManager::GetInstance()->NextState(CTitleState::GetInstance());
		m_bTitleScreen = false;
	}
}

void CMainMenuState::Exit(void)
{

	//CAnimation::~CAnimation();

	//Unload all the images
	if(m_nCursorImgID != -1)
	{
		m_pTM->UnloadTexture(m_nCursorImgID);
		m_nCursorImgID = -1;
	}

	if(m_nCursorImgID2 != -1)
	{
		m_pTM->UnloadTexture(m_nCursorImgID2);
		m_nCursorImgID2 = -1;
	}

	if(m_nBackgroundImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBackgroundImgID);
		m_nBackgroundImgID = -1;
	}
	if(m_nMenuImgID != -1)
	{
		m_pTM->UnloadTexture(m_nMenuImgID);
		m_nMenuImgID = -1;
	}
	if(m_nLogoImgID != -1)
	{
		m_pTM->UnloadTexture(m_nLogoImgID);
		m_nLogoImgID = -1;
	}

	// Unload Audio
	if(m_nBackgroundMusID != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusID);
		m_pXA->MusicUnloadSong(m_nBackgroundMusID);
		m_nBackgroundMusID = -1;
	}
	if(m_nSelectionSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nSelectionSFXID);
		m_pXA->SFXUnloadSound(m_nSelectionSFXID);
		m_nSelectionSFXID = -1;
	}

	if(m_nEnterSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nEnterSFXID);
		m_pXA->SFXUnloadSound(m_nEnterSFXID);
		m_nEnterSFXID = -1;
	}

	CAnimationManager::GetInstance()->Shutdown();
}

bool CMainMenuState::Input(void)
{
	CGame* pGame = CGame::GetInstance();



	if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(6) )
	{
		m_nShadowPosY = 415;
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
	}
	if(m_pDI->KeyPressed(DIK_UP) || m_pDI->JoystickGetLStickDirPressed(DIR_UP) )
	{
		m_nShadowPosY = m_nShadowPosY - 30;
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
	}
	if(m_pDI->KeyPressed(DIK_DOWN) || m_pDI->JoystickGetLStickDirPressed(DIR_DOWN) )
	{
		m_nShadowPosY = m_nShadowPosY + 30;
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
	}
	if(m_pDI->KeyPressed(DIK_RETURN) || m_pDI->MouseButtonReleased((char)DIMOUSE_XAXISAB) || m_pDI->JoystickButtonPressed(0) )
	{

		m_pXA->SFXPlaySound(m_nEnterSFXID);

		if(m_nShadowPosY == 235)
		{
			CGameStateManager::GetInstance()->ChangeState(CGamePlayState::GetInstance());
			return true;
		}
		if(m_nShadowPosY == 265 )//&& m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
		{
			// Load Game
			// Set Load Game bool to true
			// Change to the Gameplay State
			//m_nShadowPosY = 265;
			m_bLoadingGame = true;
			CGameStateManager::GetInstance()->NextState(CFileManagementState::GetInstance());
			return true;
		}
		if(m_nShadowPosY == 295 )//&& m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
		{

			//m_nShadowPosY = 295;
			// PUT BACK TO NEXT STATE AFTER THE OPTIONS MENU IS FIXED!
			CGameStateManager::GetInstance()->NextState(COptionsState::GetInstance());
			m_bOptions = true;
			return true;
		}
		if(m_nShadowPosY == 325 )//&& m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
		{
			//m_nShadowPosY = 325;
			CGameStateManager::GetInstance()->NextState(CHowToPlayState::GetInstance());
			//CGameStateManager::GetInstance()->ChangeState(CHowToPlayState::GetInstance());
			return true;
		}
		if(m_nShadowPosY == 355 )//&& m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
		{

			m_nShadowPosY = 355;
			CGameStateManager::GetInstance()->NextState(CHighScoreState::GetInstance());
			//CGameStateManager::GetInstance()->ChangeState(CHighScoreState::GetInstance());
			return true;
		}
		if(m_nShadowPosY == 385 )// && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
		{
			m_nShadowPosY = 385;
			//CGameStateManager::GetInstance()->NextState(CCreditsState::GetInstance());
			CGameStateManager::GetInstance()->ChangeState(CCreditsState::GetInstance());
			return true;
		}
		if(m_nShadowPosY == 415 )// && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
		{

			m_nShadowPosY = 415;
			return false;
		}


	}

	if(m_nPosY < 250 && m_nPosY > 200 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nShadowPosY != 235)
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nShadowPosY = 235;

	}
	if(m_nPosY > 250 && m_nPosY < 280 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nShadowPosY != 265)
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nShadowPosY = 265;

	}
	if(m_nPosY > 280 && m_nPosY < 310 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nShadowPosY != 295)
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nShadowPosY = 295;
	}
	if(m_nPosY > 310 && m_nPosY < 340 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nShadowPosY != 325)
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nShadowPosY = 325;

	}
	if(m_nPosY > 340 && m_nPosY < 370 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nShadowPosY != 355)
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nShadowPosY = 355;

	}
	if(m_nPosY > 370 && m_nPosY < 400  && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nShadowPosY != 385)
		m_pXA->SFXPlaySound(m_nSelectionSFXID,false);
		m_nShadowPosY = 385;

	}
	if(m_nPosY > 400 && m_nPosY < 430  && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -200)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nShadowPosY != 415)
		m_pXA->SFXPlaySound(m_nSelectionSFXID,false);
		m_nShadowPosY = 415;

	}



	return true;
}

void CMainMenuState::Update(float fElapsedTime)
{
	//update Particles
	//m_pPM->Update(fElapsedTime);

	if(CFileManagementState::GetInstance()->GetFileName() != "")
	{
		CGameStateManager::GetInstance()->ChangeState(CGamePlayState::GetInstance());
		return;
	}

	m_nPosX = m_pDI->MouseGetPosX();
	m_nPosY = m_pDI->MouseGetPosY();

	//m_pEM->ProcessEvents();

	if(m_nShadowPosY >415)
	{
		m_nShadowPosY = 235;
	}
	if(m_nShadowPosY <235)
	{
		m_nShadowPosY = 415;
	}


	//mouse out of bounds check
	if( m_pDI->MouseGetPosX() < 0 )
		m_pDI->MouseSetPosX(0);
	if( m_pDI->MouseGetPosY() < 0 )
		m_pDI->MouseSetPosY(0);
	if( m_pDI->MouseGetPosX() > CGame::GetInstance()->GetWidth() - 50 )
		m_pDI->MouseSetPosX( CGame::GetInstance()->GetWidth() - 50);
	if( m_pDI->MouseGetPosY() > CGame::GetInstance()->GetHeight() - 50 )
		m_pDI->MouseSetPosY( CGame::GetInstance()->GetHeight() - 50);
}

void CMainMenuState::Render(void)
{
	//CParticleManager::GetInstance()->Render();

	CGame* pGame = CGame::GetInstance();
	// Render background
	RECT rSource = {0,0,800,600};
	m_pTM->Draw(m_nBackgroundImgID,0,0,1.0f,1.0f,&rSource,0.0f,0.0f,0.0f);


	// Render Menu Screen
	SetRect(&rSource, 0, 0, 256, 450);
	m_pTM->Draw(m_nMenuImgID, pGame->GetWidth() - 256, 50, 1.0f, 1.0f, &rSource);

	// Render Logo
	m_pTM->Draw(m_nLogoImgID, pGame->GetWidth() - 260, 40);

	// Render Cursor
	SetRect(&rSource,0,0,200,40);
	m_pTM->Draw(m_nCursorImgID, m_nShadowPosX + 20, m_nShadowPosY,1.0f,1.0f,&rSource,0.0f,0.0f,0.0f);

	//Render Particles
	//m_pPM->Render("1");
	//m_pPM->Render("2");
	//m_pPM->Render("3");
	//m_pPM->Render("4");

	// Render Text
	m_pFont->PrintString("New Game", pGame->GetWidth() - 200, 240, 1.0f, 255, 255, 255, 255);
	m_pFont->PrintString("Load Game", pGame->GetWidth() - 200, 270, 1.0f, 255, 255, 255);
	m_pFont->PrintString("Options", pGame->GetWidth() - 200, 300, 1.0f, 255, 255, 255, 255);
	m_pFont->PrintString("How to Play", pGame->GetWidth() - 200, 330, 1.0f, 255, 255, 255, 255);
	m_pFont->PrintString("High Scores", pGame->GetWidth() - 200, 360, 1.0f, 255, 255, 255, 255);
	m_pFont->PrintString("Credits", pGame->GetWidth() - 200, 390, 1.0f, 255, 255, 255, 255);
	m_pFont->PrintString("Exit", pGame->GetWidth() - 200, 420, 1.0f, 255, 255, 255, 255);

	if(m_bOptions == false)
		m_pTM->Draw(m_nCursorImgID2, m_pDI->MouseGetPosX() + 16, m_pDI->MouseGetPosY() + 16);
}

bool CMainMenuState::LoadOptions(const char* szFileName)
{
	TiXmlDocument doc;

	if(doc.LoadFile(szFileName) == false)
		return false;

	TiXmlElement* pRoot = doc.RootElement();

	if(pRoot == nullptr)
		return false;

	TiXmlElement* pOptions = pRoot->FirstChildElement("options_info");

	int nWindowed = 0;

	while(pOptions != nullptr)
	{
		pOptions->Attribute("music_volume", &m_tOptions.music);
		pOptions->Attribute("sfx_volume", &m_tOptions.sfx);

		pOptions->Attribute("windowed_mode", &nWindowed);

		if(nWindowed == 0)
			m_tOptions.isWindowed = false;
		else if(nWindowed == 1)
			m_tOptions.isWindowed = true;
		else	// If an invalid number is in the XML, default to windowed mode.
			m_tOptions.isWindowed = true;

		pOptions = pOptions->NextSiblingElement("options_info");
	}

	return true;
}

CMainMenuState::~CMainMenuState(void)
{
}
