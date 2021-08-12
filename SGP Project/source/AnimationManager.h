/*
	File:				AnimationManager.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Nathan White
	Last Edited:		9/10/2012
	Purpose:			Load and manage animations
*/

#pragma once


#include "Animation.h"
#include "AnimationInfo.h"
#include "MovingObject.h"
#include <iostream>
#include <vector>
#include <string>
#include <Windows.h>
using std::vector;
using std::string;


#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"


class CAnimationManager 
{
private:


public:
	CAnimationManager(void);
	~CAnimationManager(void);

	static CAnimationManager* GetInstance( void );
	bool LoadAnimationFile( const char* szFile);

	void Shutdown();
		
	void Update(CAnimationInfo* pAnimInfo, float fElapsedTime);
	void Render(CAnimationInfo* pAnimInfo,int ImgID, int PosX, int PosY,  float fScaleX, float fScaleY, float fRotateX, float fRotateY, float fCurrRotate, DWORD color);

	CSGD_TextureManager*		m_pTM;

	vector <CAnimation*> Animations;

	//CAnimation* pAnimationObject;


		//CSGD_TextureManager*		m_pTM;

	//CAnimationManager* AniManager;

	int   imageID;

};

