/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/5/2012
	Purpose:			Contain / manage the game code
*/

#include "VoiceReco.h"
#include "Game.h"
#include "MainMenuState.h"
#include "TitleState.h"
#include "GameStateManager.h"

// SINGLETON:
//	- only allow ONE instance of the class
//	- use a STATIC method to access the instance
/*static*/ CGame* CGame::GetInstance( void )
{
	// Local static variable:
	//	- stored in global memory
	//	- there can be only one!
	static CGame s_Instance;

	return &s_Instance;
}

CGame::CGame( void )
{
	// Assign the data members to 'clean' values
	m_pD3D	= nullptr;
	m_pTM	= nullptr;
	m_pDI	= nullptr;
	m_pXA	= nullptr;
	m_pVoice = nullptr;
	m_pOM	= nullptr;

	m_nWidth = 1;
	m_nHeight = 1;
	m_bIsWindowed = true;

	m_dwCurrTime = 0;

	m_pCurrState = nullptr;
}


CGame::~CGame(void)
{
}

void CGame::Initialize(HWND hWnd, HINSTANCE hInstance,
						int nScreenWidth, int nScreenHeight,
						bool bIsWindowed)
{
	// store the hWnd
	m_hwnd = hWnd;

	// Store the parameters
	m_nWidth = nScreenWidth;
	m_nHeight = nScreenHeight;
	m_bIsWindowed = bIsWindowed;

	// Access the singletons of the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM  = CSGD_TextureManager::GetInstance();
	m_pDI  = CSGD_DirectInput::GetInstance();
	m_pXA  = CSGD_XAudio2::GetInstance();

	// Initialize the wrappers
	m_pD3D->InitDirect3D( hWnd, m_nWidth, m_nHeight, m_bIsWindowed, false );
	m_pTM->InitTextureManager( m_pD3D->GetDirect3DDevice(), m_pD3D->GetSprite() );
	m_pDI->InitDirectInput( hWnd, hInstance, DI_KEYBOARD | DI_MOUSE | DI_JOYSTICKS, DI_MOUSE );
	m_pXA->InitXAudio2();

	//initializing the voice recognition
// 	m_pVoice = CVoiceReco::GetInstance();
// 	m_pVoice->Init();

	m_dwCurrTime = GetTickCount();

	// Start in the MainMenuState
	//CGameStateManager::GetInstance()->NextState(CMainMenuState::GetInstance());
	CGameStateManager::GetInstance()->NextState(CMainMenuState::GetInstance());
}

bool CGame::Main( void )
{
	// Call the private helper functions
	if( Input() == false )
		return false;

	Update();
	Render();

	return true;
}

void CGame::Shutdown( void )
{
	// Exit the current state
	CGameStateManager::GetInstance()->ChangeState( nullptr );

	if(ObjectManager::GetInstance()->IsShutdown() == false)
	{
		ObjectManager::GetInstance()->Shutdown();
	}

	// Shutdown the wrappers
	if( m_pXA != nullptr )
	{
		m_pXA->ShutdownXAudio2();
		m_pXA = nullptr;
	}

	if( m_pDI != nullptr )
	{
		m_pDI->ShutdownDirectInput();
		m_pDI = nullptr;
	}

	if( m_pTM != nullptr )
	{
		m_pTM->ShutdownTextureManager();
		m_pTM = nullptr;
	}

	if( m_pD3D != nullptr )
	{
		m_pD3D->ShutdownDirect3D();
		m_pD3D = nullptr;
	}

// 	m_pVoice = CVoiceReco::GetInstance();
// 	m_pVoice->StopListening();
// 	m_pVoice->ShutDown();
}

bool CGame::Input( void )
{
	// Refresh the input wrapper
	m_pDI->ReadDevices();


	// Redirect the input to the current state
	return CGameStateManager::GetInstance()->Input();
}

void CGame::Update( void )
{
	// Refresh the audio wrapper
	m_pXA->Update(); 

	// Calculate the elapsed time between the previous frame and now
	DWORD dwNow = GetTickCount();

	//static float elapsed;
	float fElapsedSeconds = (dwNow - m_dwCurrTime) / 1000.0f;
	m_dwCurrTime = dwNow;
	
	//elapsed = fElapsedSeconds;
	if(fElapsedSeconds > 0.125f)
	{
		fElapsedSeconds = 0.125f;			
	}
	CGameStateManager::GetInstance()->Update(fElapsedSeconds);

	////static float elapsed;
	//static float fElapsedSeconds = 0.0f;// = (dwNow - m_dwCurrTime) / 1000.0f;
	//
	////elapsed = fElapsedSeconds;
	//if(fElapsedSeconds > 0.125f)
	//{
	//	fElapsedSeconds = 0.125f;	
	//	CGameStateManager::GetInstance()->Update(fElapsedSeconds);
	//	fElapsedSeconds = 0.0f;
	//}
	//else
	//	fElapsedSeconds += (dwNow - m_dwCurrTime) / 1000.0f;
	//m_dwCurrTime = dwNow;

	//if(elapsed > 0.125f)
	//{
	//	// Update the current state
	//	elapsed = 0.125;
	//	CGameStateManager::GetInstance()->Update(elapsed);
	//	//fElapsedSeconds = 0.125f;	
	//	elapsed = 0;
	//}
	//else
	//	elapsed += fElapsedSeconds;
	
}

void CGame::Render( void )
{
	// Clear the screen
	m_pD3D->Clear( 0, 255, 0 );

	// Start the D3D rendering
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	// Render the current state
	CGameStateManager::GetInstance()->Render();

	// End the D3D rendering (draw to the buffer)
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();

	// Present the buffer to the screen
	m_pD3D->Present();
}
