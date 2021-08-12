/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/5/2012
	Purpose:			Contain / manage the game code
*/

#pragma once

#include "MovingObject.h"
#include "Animation.h"
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
using std::vector;
using std::string;

#include "../SGD Wrappers/CSGD_TextureManager.h"





class CAnimationInfo
{
public:

	int m_nImageID;


	CSGD_TextureManager*		m_pTM;


	int			  	m_nCurrentAnimation;
	int				m_nCurrentFrame;
	float			m_fTimeWaited;
	bool			m_bIsLooping;
	bool			m_bIsPlaying;


	CAnimationInfo(void);
	~CAnimationInfo(void);
};

