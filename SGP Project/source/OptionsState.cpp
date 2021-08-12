/*
File:				OptionsState.cpp
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Michael Yatckoske
Last Edited:		9/10/2012
Purpose:			A menu to alter various options of the game
*/

#include "OptionsState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "GameStateManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../TinyXML/tinyxml.h"
#include "BitmapFont.h"
#include "PauseState.h"

#include <iostream>

// Singleton
COptionsState* COptionsState::GetInstance(void)
{
	static COptionsState s_Instance;
	return &s_Instance;
}

COptionsState::COptionsState(void)
{
	// Assign data members to clean values
	m_pD3D	= nullptr;
	m_pTM	= nullptr;
	m_pDI	= nullptr;
	m_pXA	= nullptr;
	m_pFont	= nullptr;

	// Image IDs:
	m_nBackgroundImgID	= -1;
	m_nMenuImgID		= -1;
	m_nCursorImgID		= -1;

	// Audio IDs:
	m_nPreviewSfxID	= -1;
}

void COptionsState::Enter(void)
{
	// Access the singletons of the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM  = CSGD_TextureManager::GetInstance();
	m_pDI  = CSGD_DirectInput::GetInstance();
	m_pXA  = CSGD_XAudio2::GetInstance();
	m_pFont = CBitmapFont::GetInstance();

	// Load Images
	m_nBackgroundImgID	= m_pTM->LoadTexture(_T("resource/graphics/instructions.png"), D3DCOLOR_XRGB(0,0,0));
	m_nCursorImgID2		= m_pTM->LoadTexture(_T("resource/graphics/Blast_Mouse_Cursor_PlacementEditor.png"), D3DCOLOR_XRGB(255,255,255));
	m_nCursorImgID		= m_pTM->LoadTexture(_T("resource/graphics/Blast_MenuCursorHover.png"), D3DCOLOR_XRGB(0,0,0));
	m_nMenuImgID		= m_pTM->LoadTexture(_T("resource/graphics/Blast_Menu_Text_Bg.png"));


	// Load Bitmap Image
	m_nBitmapFontImgID		= m_pTM->LoadTexture(_T("resource/graphics/SansationFont_0.png"), D3DCOLOR_XRGB(0,0,0));
	m_pFont->SetImageID(m_nBitmapFontImgID);

	// Load Audio
	m_nPreviewMusID	= m_pXA->MusicLoadSong(_T("resource/sounds/MUS_TEMPMUSIC.xwm"));
	m_nPreviewSfxID	= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_OPTIONSTEMPSFX.wav"));

	if(LoadOptions("options.xml") == false)
	{
		m_tOptions.music		= 100.0;
		m_tOptions.sfx			= 100.0;
		m_tOptions.isWindowed	= true;
	}

	CGame* pGame = CGame::GetInstance();

	m_nTextPos[0] = 130;
	m_nTextPos[1] = 160;
	m_nTextPos[2] = 190;
	m_nCursorPos = 0;

	m_pXA->MusicSetMasterVolume((float)m_tOptions.music/100);
	m_pXA->SFXSetMasterVolume((float)m_tOptions.sfx/100);
}

void COptionsState::Exit(void)
{
	m_pXA->SFXSetMasterVolume((float)m_tOptions.sfx/100);
	m_pXA->MusicSetMasterVolume((float)m_tOptions.music/100);
	// Unload Images
	if(m_nBackgroundImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBackgroundImgID);
		m_nBackgroundImgID = -1;
	}
	if(m_nCursorImgID != -1)
	{
		m_pTM->UnloadTexture(m_nCursorImgID);
		m_nCursorImgID = -1;
	}
	if(m_nMenuImgID != -1)
	{
		m_pTM->UnloadTexture(m_nMenuImgID);
		m_nMenuImgID = -1;
	}

	// Unload Audio
	if(m_nPreviewSfxID != -1)
	{
		m_pXA->SFXStopSound(m_nPreviewSfxID);
		m_pXA->SFXUnloadSound(m_nPreviewSfxID);
		m_nPreviewSfxID = -1;
	}

	SaveOptions("options.xml");
	CMainMenuState::GetInstance()->SetOptions(false);
	CPauseState::GetInstance()->SetOptionsBool(false);
}

bool COptionsState::Input(void)
{
	CGame* pGame = CGame::GetInstance();

	// Escape to return to the previous menu (since the options can be accessed from the pause menu)
	if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(6) )
	{
		// CHANGE BACK TO PREVIOUS STATE AFTER THE OPTIONS MENU IS FIXED!
		CGameStateManager::GetInstance()->PreviousState();
		return true;
	}

	// Move the Cursor Up
	if(m_pDI->KeyPressed(DIK_UP) || m_pDI->JoystickGetLStickDirPressed(DIR_UP))
	{
		if(m_nCursorPos == 1)
		{
			m_pXA->SFXStopSound(m_nPreviewSfxID);
		}
		else if(m_nCursorPos == 2)
		{
			m_pXA->SFXPlaySound(m_nPreviewSfxID, true);
		}

		m_nCursorPos--;
		if(m_nCursorPos < 0)
			m_nCursorPos = 2;
	}

	// Move the Cursor Down
	if(m_pDI->KeyPressed(DIK_DOWN) || m_pDI->JoystickGetLStickDirPressed(DIR_DOWN) )
	{
		if(m_nCursorPos == 0)
		{
			m_pXA->SFXPlaySound(m_nPreviewSfxID, true);
		}
		else if(m_nCursorPos == 1)
		{
			m_pXA->SFXStopSound(m_nPreviewSfxID);
		}

		m_nCursorPos++;
		if(m_nCursorPos > 2)
			m_nCursorPos = 0;
	}

	// Change Options
	if(m_pDI->KeyPressed(DIK_LEFT) || m_pDI->JoystickGetLStickDirPressed(DIR_LEFT))
	{
		switch(m_nCursorPos)
		{
		case 0:
			{	// Change Music Volume
				if(m_tOptions.music > 0)
				{
					m_tOptions.music -= 5;
					m_pXA->MusicSetMasterVolume((float)m_tOptions.music/100);
				}
			}
			break;

		case 1:
			{	// Change SFX Volume
				if(m_tOptions.sfx > 0)
				{
					m_tOptions.sfx -= 5;
					m_pXA->SFXSetMasterVolume((float)m_tOptions.sfx/100);
				}
			}
			break;

		case 2:
			{	// Toggle Full Screen
				if(m_tOptions.isWindowed == false)
					m_tOptions.isWindowed = true;
				else
					m_tOptions.isWindowed = false;

				pGame->SetWindowed(m_tOptions.isWindowed);

				m_pD3D->ChangeDisplayParam(pGame->GetWidth(), pGame->GetHeight(), pGame->IsWindowed());
			}
			break;
		};
	}
	if(m_pDI->KeyPressed(DIK_RIGHT) || m_pDI->JoystickGetLStickDirPressed(DIR_RIGHT) )
	{
		switch(m_nCursorPos)
		{
		case 0:
			{	// Change Music Volume
				if(m_tOptions.music < 100)
				{
					m_tOptions.music += 5;
					m_pXA->MusicSetMasterVolume((float)m_tOptions.music/100);
				}
			}
			break;

		case 1:
			{	// Change SFX Volume
				if(m_tOptions.sfx < 100)
				{
					m_tOptions.sfx += 5;
					m_pXA->SFXSetMasterVolume((float)m_tOptions.sfx/100);
				}
			}
			break;

		case 2:
			{	// Toggle Full Screen
				if(m_tOptions.isWindowed == false)
					m_tOptions.isWindowed = true;
				else
					m_tOptions.isWindowed = false;

				pGame->SetWindowed(m_tOptions.isWindowed);

				m_pD3D->ChangeDisplayParam(pGame->GetWidth(), pGame->GetHeight(), pGame->IsWindowed());
			}
			break;
		};
	}


	if(m_nPosY > 115 && m_nPosY < 145 && m_nPosX <= 260 && m_nPosX >= 60)
	{
		if(m_nCursorPos==1)
		{
			m_pXA->SFXStopSound(m_nPreviewSfxID);
		}
		m_nCursorPos = 0;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_tOptions.music < 100)
			{
				m_tOptions.music += 5;
				m_pXA->MusicSetMasterVolume((float)m_tOptions.music/100);
			}
		}
		if(m_pDI->MouseButtonPressed(1))
		{
			if(m_tOptions.music > 0)
			{
				m_tOptions.music -= 5;
				m_pXA->MusicSetMasterVolume((float)m_tOptions.music/100);
			}

		}
	}
	if(m_nPosY > 145 && m_nPosY < 175 && m_nPosX <= 260 && m_nPosX >= 60)
	{
		
		if(m_pXA->SFXIsSoundPlaying(m_nPreviewSfxID)==false)
		m_pXA->SFXPlaySound(m_nPreviewSfxID, true);
		m_nCursorPos = 1;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_tOptions.sfx < 100)
			{
				m_tOptions.sfx += 5;
				m_pXA->SFXSetMasterVolume((float)m_tOptions.sfx/100);
			}
		}
		
		if(m_pDI->MouseButtonPressed(1))
		{
			if(m_tOptions.sfx > 0)
			{
				m_tOptions.sfx -= 5;
				m_pXA->SFXSetMasterVolume((float)m_tOptions.sfx/100);
				
			}
		}

	}
	if(m_nPosY > 175 && m_nPosY < 205 && m_nPosX <= 260 && m_nPosX >= 60)
	{
		if(m_nCursorPos==1)
		{
			m_pXA->SFXStopSound(m_nPreviewSfxID);
		}
		m_nCursorPos = 2;

			if(m_pDI->MouseButtonPressed(0))
		{

		// Toggle Full Screen
				if(m_tOptions.isWindowed == false)
					m_tOptions.isWindowed = true;
				else
					m_tOptions.isWindowed = false;

				pGame->SetWindowed(m_tOptions.isWindowed);

				m_pD3D->ChangeDisplayParam(pGame->GetWidth(), pGame->GetHeight(), pGame->IsWindowed());

			}
	}






	return true;
}

void COptionsState::Update(float fElapsedTime)
{
	CGame* pGame = CGame::GetInstance();

	if(m_nCursorPos == 1)
	{
		m_pXA->MusicSetMasterVolume(0);

	}
	else
		m_pXA->MusicSetMasterVolume((float)m_tOptions.music/100);


		m_pXA->SFXSetMasterVolume((float)m_tOptions.sfx/100);

	m_nPosX = m_pDI->MouseGetPosX();
	m_nPosY = m_pDI->MouseGetPosY();


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

void COptionsState::Render(void)
{
	// Instance of the game for positioning
	CGame* pGame = CGame::GetInstance();

	// Render Menu Img Background
	RECT rSource = { 0, 0, 256, 450 };
	m_pTM->Draw(m_nMenuImgID, 0, 50, 2.0f, 0.5f, &rSource);

	m_pFont->PrintString("OPTIONS", 30, 80);

	// Render the Cursor
	m_pTM->Draw(m_nCursorImgID, 20, (int)(m_nTextPos[m_nCursorPos] - 7.5f), 2.0f);
	
	std::ostringstream output;
	// Music Volume
	output << "Music Volume = " << m_tOptions.music;
	m_pFont->PrintString(output.str().c_str(), 60, m_nTextPos[0]);
	output.str("");

	// SFX Volume
	output << "SFX Volume = " << m_tOptions.sfx;
	m_pFont->PrintString(output.str().c_str(), 60, m_nTextPos[1]);
	output.str("");

	// Toggle Full Screen
	if(m_tOptions.isWindowed == true)
		m_pFont->PrintString("Windowed Mode = ON", 60, m_nTextPos[2]);
	else if(m_tOptions.isWindowed == false)
		m_pFont->PrintString("Windowed Mode = OFF", 60, m_nTextPos[2]);

	m_pTM->Draw(m_nCursorImgID2, m_nPosX, m_nPosY, 1.0f);
}

bool COptionsState::LoadOptions(const char* szFileName)
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

void COptionsState::SaveOptions(const char* szFileName)
{
	TiXmlDocument doc;

	TiXmlDeclaration* pDec = new TiXmlDeclaration("1.0", "utf-8", "");

	doc.LinkEndChild(pDec);

	TiXmlElement* pRoot = new TiXmlElement("OPTIONS");

	doc.LinkEndChild(pRoot);

	TiXmlElement* pOptions = new TiXmlElement("options_info");
	pOptions->SetDoubleAttribute("music_volume", m_tOptions.music);
	pOptions->SetDoubleAttribute("sfx_volume", m_tOptions.sfx);

	// Set Windowed Mode
	if(m_tOptions.isWindowed == true)
	{
		pOptions->SetAttribute("windowed_mode", 1);
	}
	else if(m_tOptions.isWindowed == false)
	{
		pOptions->SetAttribute("windowed_mode", 0);
	}

	pRoot->LinkEndChild(pOptions);

	doc.SaveFile(szFileName);
}

COptionsState::~COptionsState(void)
{
}