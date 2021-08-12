/*
	File:				Animation.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Nathan White
	Last Edited:		9/10/2012
	Purpose:			Contain / manage the game code
*/

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
using std::vector;
using std::string;

#include "../SGD Wrappers/CSGD_TextureManager.h"

struct tFrame
{
	RECT	m_rSourceRect;
	RECT	m_rCollisionRect;
	RECT	m_rActiveRect;
	int		m_nAnchorPointX;
	int		m_nAnchorPointY;
	double	m_fFrameDuration;
	string	m_szEventName;
};



class CAnimation 
{
public:

	// friend CAnimationManager;

	CSGD_TextureManager*		m_pTM;

	
	int imageID;

	

	vector <tFrame*> frames;



	CAnimation(void);
	~CAnimation(void);






};

