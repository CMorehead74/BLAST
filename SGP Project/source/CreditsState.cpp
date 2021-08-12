/*
	File:				CreditsState.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/10/2012
	Purpose:			Displays the credits to the user
*/

#include "CreditsState.h"

#include "Game.h"
#include "MainMenuState.h"

// Singleton
CCreditsState* CCreditsState::GetInstance(void)
{
	static CCreditsState s_Instance;
	return &s_Instance;
}

CCreditsState::CCreditsState(void)
{
	// Assign data members to clean values
	m_pD3D	= nullptr;
	m_pTM	= nullptr;
	m_pDI	= nullptr;
	m_pXA	= nullptr;
	m_pFont = nullptr;

	// Image IDs:
	m_nBackgroundImgID	= -1;
	
	// Audio IDs:
	m_nBackgroundMusID	= -1;
}

void CCreditsState::Enter(void)
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

	// Load Audio
	m_nBackgroundMusID		= m_pXA->MusicLoadSong(_T("resource/sounds/MUS_TEMPMUSIC.xwm"));

	// Set the Text to the Right side of the screen
	for(int i = 0; i < 13; ++i)
		m_fTextPos[i] = (float)CGame::GetInstance()->GetWidth();

	m_pXA->MusicPlaySong(m_nBackgroundMusID, true);
}

void CCreditsState::Exit(void)
{
	// Unload Images
	if(m_nBackgroundImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBackgroundImgID);
		m_nBackgroundImgID = -1;
	}

	// Unload Audio
	if(m_nBackgroundMusID != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusID);
		m_pXA->MusicUnloadSong(m_nBackgroundMusID);
		m_nBackgroundMusID = -1;
	}
}

bool CCreditsState::Input(void)
{
	if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(6) )
	{
		//CGameStateManager::GetInstance()->PreviousState();
		CGameStateManager::GetInstance()->ChangeState(CMainMenuState::GetInstance());
		return true;
	}
	return true;
}

void CCreditsState::Update(float fElapsedTime)
{
	for(int i = 0; i < 13; ++i)
	{
		if(m_fTextPos[i] == 50)
			continue;
		if(i == 0)
		{
			m_fTextPos[i] -= 850 * fElapsedTime;
		}
		else if(m_fTextPos[i-1] <= 50.0f)
		{
			m_fTextPos[i] -= 850 * fElapsedTime;
		}

		if(m_fTextPos[i] < 50.0f)
			m_fTextPos[i] = 50.0f;
	}

}

void CCreditsState::Render(void)
{	
	CGame* pGame = CGame::GetInstance();
	m_pTM->Draw(m_nBackgroundImgID, 0, 0);

	if(m_fTextPos[0] < pGame->GetWidth())
		m_pFont->PrintString("BLAST",	(int)m_fTextPos[0], 25);

	if(m_fTextPos[1] < pGame->GetWidth())
		m_pFont->PrintString("Executive Producer - John O'Leske",	(int)m_fTextPos[1], 50);
	if(m_fTextPos[2] < pGame->GetWidth())
		m_pFont->PrintString("Associate Producer - Sean Hathaway",	(int)m_fTextPos[2], 75);
	if(m_fTextPos[3] < pGame->GetWidth())
		m_pFont->PrintString("Artists - Chris Jahosky", (int)m_fTextPos[3], 100);

	if(m_fTextPos[4] < pGame->GetWidth())
		m_pFont->PrintString("Programmers", (int)m_fTextPos[4], 175);
	if(m_fTextPos[5] < pGame->GetWidth())
		m_pFont->PrintString("Roberto Alvarado",	(int)m_fTextPos[5], 200);
	if(m_fTextPos[6] < pGame->GetWidth())
		m_pFont->PrintString("Matthew Berry",	(int)m_fTextPos[6], 225);
	if(m_fTextPos[7] < pGame->GetWidth())
		m_pFont->PrintString("Schadrac Francois",		(int)m_fTextPos[7], 250);
	if(m_fTextPos[8] < pGame->GetWidth())
		m_pFont->PrintString("Naveen Gunalan",		(int)m_fTextPos[8], 275);
	if(m_fTextPos[9] < pGame->GetWidth())
		m_pFont->PrintString("Corey Morehead",	(int)m_fTextPos[9], 300);
	if(m_fTextPos[10] < pGame->GetWidth())
		m_pFont->PrintString("Nathan White",		(int)m_fTextPos[10], 325);
	if(m_fTextPos[11] < pGame->GetWidth())
		m_pFont->PrintString("Michael Yatckoske",		(int)m_fTextPos[11], 350);

	if(m_fTextPos[12] < pGame->GetWidth())
		m_pFont->PrintString("Thank you for playing!",	(int)m_fTextPos[12], 450);
}

CCreditsState::~CCreditsState(void)
{
}