/*
	File:						TitleState.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/24/2012
	Purpose:				State to display the title of the game to the user
*/

#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "BitmapFont.h"

class CTitleState : public IGameState
{
private:
	//securing my singleton
	CTitleState(void);
	virtual ~CTitleState(void);
	CTitleState( const CTitleState& );
	CTitleState& operator=( const CTitleState& );

	//Wrappers
	CSGD_DirectInput*			m_pDI;
	CSGD_TextureManager*		m_pTM;

	//Images
	int										m_nBackgroundImgID;
	int										m_nTextImgID;
	int										m_nVortexImgID;

	// Bitmap Font ID
	int										m_nBitmapFontImgID;

	//rotation
	float									m_fRotation;

	//font
	CBitmapFont*							m_pFont;

public:
	//singleton
	static CTitleState* GetInstance();

	//functions that need to be overriden
	virtual void Enter();
	virtual void Exit();
	virtual bool Input();
	virtual void Update( float fElapsedTime );
	virtual void Render();
};

