/*
	File:				Shop.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/14/2012
	Purpose:			Contain / manage the game code
*/

#pragma once
#include "Building.h"
#include "BitmapFont.h"

class CShop : public CBuilding
{
private:
	bool				m_bIsShopping;

	//img ID
	int					m_nBackgroundImgID;
	int					m_nCursorImgID;

	//font
	CBitmapFont* m_pFont;
public:
	CShop(void);
	virtual ~CShop(void);

	// IEntity Interface functions
	virtual void Update( float fElapsedTime );
	virtual void Render(void);

	virtual bool CheckCollision( IEntity *pOther);

	bool Shop();

	void MiniMapRender();
};