/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/5/2012
	Purpose:			Contain / manage the game code
*/

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
//#include "VoiceReco.h"
class CVoiceReco;

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"
#include "Entity.h"
#include "ObjectManager.h"

#include "IGameState.h"

typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CGame
{
public:

	// SINGLETON:
	//	- only allow ONE instance of the class
	//	- use a STATIC method to access the instance
	static CGame* GetInstance( void );

	// 3 actions for any game: Setup, Play, Cleanup
	void Initialize( HWND hWnd, HINSTANCE hInstance,
					 int nScreenWidth, int nScreenHeight,
					 bool bIsWindowed );
	bool Main( void );
	void Shutdown( void );

	// Window Accessors:
	HWND	GetHwnd(void) const { return m_hwnd; } 

	int		GetWidth( void ) const	{ return m_nWidth;		}
	int		GetHeight( void ) const	{ return m_nHeight;		}
	bool	IsWindowed( void ) const{ return m_bIsWindowed;	}
	void	SetWindowed(bool isWindowed)	{ m_bIsWindowed = isWindowed; }

private:
	//	- prevent outside access to the constructors
	CGame( void );
	~CGame( void );						//= default;
	CGame( const CGame& );				//= delete;
	CGame& operator=( const CGame& );	//= delete;

	// 3 actions each frame:
	bool Input( void );
	void Update( void );
	void Render( void );

	// SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_XAudio2*			m_pXA;
	ObjectManager*			m_pOM;

	//Handle to the window
	HWND					m_hwnd;

	// Window Attributes:
	int						m_nWidth;
	int						m_nHeight;
	bool					m_bIsWindowed;

	// Time
	DWORD					m_dwCurrTime;

	// Current game state
	IGameState*				m_pCurrState;

	//voice recognition
	CVoiceReco*				m_pVoice;
};