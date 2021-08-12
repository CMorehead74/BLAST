/*
	File:				HighScoreState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/24/2012
	Purpose:			Displays the high scores list to the player
*/

#pragma once
#include "IGameState.h"

// SGD Wrappers
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "BitmapFont.h"

class CHighScoreState : public IGameState
{
public:
	// Singleton
	static CHighScoreState* GetInstance(void);

	// IGameState Interface
	virtual void Enter( void );					// enter the state
	virtual void Exit( void );					// exit the state

	virtual bool Input( void );					// handle user input
	virtual void Update( float fElapsedTime );	// update game objects
	virtual void Render( void );				// render game tokens

private:
	// SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_XAudio2*			m_pXA;
	CBitmapFont*			m_pFont;

	// Score Structure
	struct tScores
	{
		int		nScore;
		char	szName[32];
	};

	void GenerateScores(vector<tScores>& vScores);

	bool LoadScores(vector<tScores>& vScores, const char* szFileName);
	void SaveScores(const vector<tScores>& vScores, const char* szFileName);

	// Image IDs:
	int						m_nBackgroundImgID;
	int						m_nMenuImgID;
	int						m_nBitmapFontImgID;

	// Audio IDs:

	// Extra Data Members
	vector<tScores>			m_vScores;

	// Private functions to keep the singleton secure
	CHighScoreState(void);
	virtual ~CHighScoreState(void);
	CHighScoreState(const CHighScoreState&);
	CHighScoreState& operator=(const CHighScoreState&);
};