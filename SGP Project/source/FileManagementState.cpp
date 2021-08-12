#include "FileManagementState.h"

CFileManagementState* CFileManagementState::GetInstance(void)
{
	// Lazy instantiation
	//	- created the first time the function is called
	static CFileManagementState s_Instance;

	return &s_Instance;
}

CFileManagementState::CFileManagementState(void)
{
	m_pD3D	= nullptr;
	m_pTM	= nullptr;
	m_pDI	= nullptr;
	m_pXA	= nullptr;
	m_pFont = nullptr;

	m_nBackgroundImgID	= -1;
	m_nMenuImgID		= -1;

	m_nSelectionFileSFXID = -1;
	m_nEnterFileSFXID	  = -1;

	m_nCursorPos		= 0;
	m_szFileName = "";
}

// Pure virtual methods
void CFileManagementState::Enter( void )
{
	//Access the singletons of the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM  = CSGD_TextureManager::GetInstance();
	m_pDI  = CSGD_DirectInput::GetInstance();
	m_pXA  = CSGD_XAudio2::GetInstance();

	m_pFont = CBitmapFont::GetInstance();

	m_nSavePos[0] = 235;
	m_nSavePos[1] = 265;
	m_nSavePos[2] = 295;

	m_nBackgroundImgID		= m_pTM->LoadTexture(_T("resource/graphics/instructions.png"));
	m_nMenuImgID			= m_pTM->LoadTexture(_T("resource/graphics/Blast_Menu_Text_Bg.png"));
	m_nCursorImgID			= m_pTM->LoadTexture(_T("resource/graphics/Blast_MenuCursorHover.png"), D3DCOLOR_XRGB(0,0,0));
	m_nCursorImgID2			= m_pTM->LoadTexture(_T("resource/graphics/Blast_Mouse_Cursor_PlacementEditor.png"), D3DCOLOR_XRGB(255,255,255));

		m_nSelectionFileSFXID = m_pXA->SFXLoadSound(_T("resource/sounds/SFX_SELECTHIGHLIGHTPause.wav"));
	m_nEnterFileSFXID	   = m_pXA->SFXLoadSound(_T("resource/sounds/SFX_SELECTENTERPause.wav"));


}

void CFileManagementState::Exit( void )
{
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
		// Unload Audio:
	if(m_nSelectionFileSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nSelectionFileSFXID);
		m_pXA->SFXUnloadSound(m_nSelectionFileSFXID);
		m_nSelectionFileSFXID = -1;
	}

	if(m_nEnterFileSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nEnterFileSFXID);
		m_pXA->SFXUnloadSound(m_nEnterFileSFXID);
		m_nEnterFileSFXID = -1;
	}	


}

bool CFileManagementState::Input( void )
{

	CGame* pGame = CGame::GetInstance();

	if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(6) )
	{
		CGameStateManager::GetInstance()->PreviousState();
		return true;
	}

	if(m_pDI->KeyPressed(DIK_UP) || m_pDI->JoystickGetLStickDirPressed(DIR_UP) )
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionFileSFXID)==false)
		m_pXA->SFXPlaySound(m_nSelectionFileSFXID);
		m_nCursorPos--;
		if(m_nCursorPos < 0)
			m_nCursorPos = 2 ;
	}
	if(m_pDI->KeyPressed(DIK_DOWN) || m_pDI->JoystickGetLStickDirPressed(DIR_DOWN) )
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionFileSFXID)==false)
		m_pXA->SFXPlaySound(m_nSelectionFileSFXID);
		m_nCursorPos++;
		if(m_nCursorPos > 2)
			m_nCursorPos = 0;
	}

	if(m_pDI->KeyPressed(DIK_RETURN) || m_pDI->JoystickButtonPressed(0))
	{
		switch(m_nCursorPos)
		{
			m_pXA->SFXPlaySound(m_nEnterFileSFXID);
		case 0:
			{
				m_szFileName = "game_save_1.xml";
				CGameStateManager::GetInstance()->PreviousState();
				return true;
			}
			break;

		case 1:
			{
				m_szFileName = "game_save_2.xml";
				CGameStateManager::GetInstance()->PreviousState();
				return true;
			}
			break;

		case 2:
			{
				m_szFileName = "game_save_3.xml";
				CGameStateManager::GetInstance()->PreviousState();
				return true;
			}
			break;
		};




	}


		if(m_nPosY > 230 && m_nPosY < 260 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -220)
		{

			m_nCursorPos = 0;
			
		if(m_pDI->MouseButtonReleased(0))
		{
			m_szFileName = "game_save_1.xml";
				CGameStateManager::GetInstance()->PreviousState();
				return true;
		}

		}

		if(m_nPosY > 260 && m_nPosY < 290 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -220)
		{
			m_nCursorPos = 1;
			if(m_pDI->MouseButtonReleased(0))
		{
			m_szFileName = "game_save_2.xml";
				CGameStateManager::GetInstance()->PreviousState();
				return true;
		}

		}
		
		if(m_nPosY > 290 && m_nPosY < 320 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -220)
		{
			m_nCursorPos = 2;
			if(m_pDI->MouseButtonReleased(0))
		{
			m_szFileName = "game_save_3.xml";
				CGameStateManager::GetInstance()->PreviousState();
				return true;
		}

		}

	return true;
}

void CFileManagementState::Update( float fElapsedTime )
{
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


void CFileManagementState::Render( void )
{
	CGame* pGame = CGame::GetInstance();
	// Render background
	RECT rSource = {0,0,800,600};
	m_pTM->Draw(m_nBackgroundImgID,0,0,1.0f,1.0f,&rSource,0.0f,0.0f,0.0f);

	// Render Menu Screen
	SetRect(&rSource, 0, 0, 256, 450);
	m_pTM->Draw(m_nMenuImgID, pGame->GetWidth() - 256, 50, 1.0f, 1.0f, &rSource);

	// Render Cursor
	SetRect(&rSource,0,0,200,40);
	m_pTM->Draw(m_nCursorImgID, 565 + 20, m_nSavePos[m_nCursorPos] ,1.0f,1.0f,&rSource,0.0f,0.0f,0.0f);

	m_pFont->PrintString("File 1", pGame->GetWidth() - 200, 240, 1.0f, 255, 255, 255, 255);
	m_pFont->PrintString("File 2", pGame->GetWidth() - 200, 270, 1.0f, 255, 255, 255, 255);
	m_pFont->PrintString("File 3", pGame->GetWidth() - 200, 300, 1.0f, 255, 255, 255, 255);

	m_pTM->Draw(m_nCursorImgID2, m_nPosX, m_nPosY);
}

CFileManagementState::~CFileManagementState(void)
{
}