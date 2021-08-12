/*
	File:						TitleState.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/24/2012
	Purpose:				State to display the title of the game to the user
*/

#include "TitleState.h"
#include "GameStateManager.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "Game.h"

CTitleState* CTitleState::GetInstance()
{
	static CTitleState s_Instance;
	return &s_Instance;
}

CTitleState::CTitleState(void)
{
	m_pTM = nullptr;
	m_pDI = nullptr;
	m_pFont = nullptr;

	m_nBackgroundImgID = -1;
	m_nTextImgID = -1;
	m_nVortexImgID = -1;
	m_fRotation = 5.0f;
}


CTitleState::~CTitleState(void)
{
}

void CTitleState::Enter()
{
	m_pDI = CSGD_DirectInput::GetInstance();
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pFont = CBitmapFont::GetInstance();

	m_nBackgroundImgID = m_pTM->LoadTexture( _T("resource/graphics/Blast_Intro_bg.png"), D3DCOLOR_XRGB(255,255,255));
	m_nTextImgID = m_pTM->LoadTexture( _T("resource/graphics/Blast_Vertex_Text_Only.png"), D3DCOLOR_XRGB(255,255,255));
	m_nVortexImgID = m_pTM->LoadTexture( _T("resource/graphics/Blast_Vertex_Logo_Bg.png"), D3DCOLOR_XRGB(255,255,255));

	// Load Bitmap Image
	m_nBitmapFontImgID		= m_pTM->LoadTexture(_T("resource/graphics/SansationFont_0.png"), D3DCOLOR_XRGB(0,0,0));
	m_pFont->SetImageID(m_nBitmapFontImgID);
}

void CTitleState::Exit()
{
	//unloading the texture
	if( m_nBackgroundImgID != -1 )
	{
		m_pTM->UnloadTexture(m_nBackgroundImgID);
		m_nBackgroundImgID = -1;
	}

	if( m_nTextImgID != -1 )
	{
		m_pTM->UnloadTexture(m_nTextImgID);
		m_nTextImgID = -1;
	}

	if( m_nVortexImgID != -1 )
	{
		m_pTM->UnloadTexture(m_nVortexImgID);
		m_nVortexImgID = -1;
	}
}

bool CTitleState::Input()
{

	if( m_pDI->CheckKeys() )//|| m_pDI->JoystickCheckBufferedButtons() )
	{
		CGameStateManager::GetInstance()->PreviousState();
		return true;
	}
	if( m_pDI->JoystickButtonReleased(0) || m_pDI->JoystickButtonReleased(1) || m_pDI->JoystickButtonReleased(2) ||
		m_pDI->JoystickButtonReleased(3) || m_pDI->JoystickButtonReleased(4) || m_pDI->JoystickButtonReleased(5) || m_pDI->JoystickButtonReleased(6) )
	{
		CGameStateManager::GetInstance()->PreviousState();
		return true;
	}
	if(m_pDI->MouseButtonReleased(0))
		{
			CGameStateManager::GetInstance()->PreviousState();
		return true;
	}
	return true;
}

void CTitleState::Update( float fElapsedTime )
{
	m_fRotation += fElapsedTime;
}

void CTitleState::Render()
{
	CGame* pGame = CGame::GetInstance();

	m_pTM->Draw(m_nBackgroundImgID, 0, 0);

	RECT rSource = { 0, 0, 512, 256 };
	m_pTM->Draw(m_nVortexImgID, 0, 0, 1.0f, 1.0f, &rSource, (float)(rSource.left + 126.0f), (float)(rSource.bottom * 0.5f), m_fRotation );
	m_pTM->Draw(m_nTextImgID, 0, 0);

	m_pFont->PrintString( "Press any key to continue...", 40, pGame->GetHeight() - 40 );
}
