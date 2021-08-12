#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"
#include "BitmapFont.h"

#include "Game.h"
#include "GameStateManager.h"

class CFileManagementState : public IGameState
{
public:
	static CFileManagementState* GetInstance(void);

	// Pure virtual methods
	virtual void Enter( void );	// enter the state
	virtual void Exit( void );	// exit the state

	virtual bool Input( void );	// handle user input
	virtual void Update( float fElapsedTime );	// update game objects
	virtual void Render( void );	// render game tokens

	char* GetFileName(void) const	{ return m_szFileName; }
	void  SetFileName(char* szName)	{ m_szFileName = szName; }

private:

	// SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pFont;

	// Image IDs:
	int						m_nBackgroundImgID;
	int						m_nMenuImgID;
	int						m_nCursorImgID;
	int						m_nCursorImgID2;
	
	char*					m_szFileName;
	int						m_nCursorPos;
	int						m_nSavePos[3];
	// Sounds
		// -- Sounds:
	int						m_nSelectionFileSFXID;
	int						m_nEnterFileSFXID;

	
	int						m_nPosX;
	int						m_nPosY;

	// Secure Singleton
	CFileManagementState(void);
	virtual ~CFileManagementState(void);
	CFileManagementState(const CFileManagementState&);
	CFileManagementState& operator=(const CFileManagementState&);
};