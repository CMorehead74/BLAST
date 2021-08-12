/*
	File:				Shop.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/14/2012
	Purpose:			Contain / manage the game code
*/

#include "Shop.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/SGD_String.h"

using namespace std;
#include "Camera.h"
CShop::CShop(void)
{
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();
	SetImageID(pTm->LoadTexture( _T("resource/graphics/building_SHOP.png"), D3DCOLOR_XRGB(255,255,255)));
	m_eType = OBJ_SHOP;

	//assigning data members
	m_bIsShopping = false;
	m_pFont = CBitmapFont::GetInstance();

	//image for the shop interface
	m_nBackgroundImgID	= pTm->LoadTexture(_T("resource/graphics/instructions.png"), D3DCOLOR_XRGB(0,0,0));
	m_nCursorImgID		= pTm->LoadTexture(_T("resource/graphics/Blast_MenuCursorHover.png"), D3DCOLOR_XRGB(0,0,0));
}

void CShop::Update(float fElapsedTime)
{
}

void CShop::Render()
{
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	pTm->Draw( GetImageID(), (int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1.0, 1.0 );

	//shop interface
	if( m_bIsShopping == true )
	{
		RECT rSource = { 0, 0, 800, 600 };
		pTm->Draw(m_nBackgroundImgID, 0, 0, 1.0f, 1.0f, &rSource);

		m_pFont->PrintString("Shop", 30, 80);

		// Render the Cursor
		//pTm->Draw(m_nCursorImgID, 20, m_nTextPos[m_nCursorPos] - 7.5, 2.0f);

		ostringstream output;

		output << "Buy";
		m_pFont->PrintString(output.str().c_str(), 10, 100);
		//output.str("");

		output << "Gunner - $100";
		m_pFont->PrintString(output.str().c_str(), 10, 140);
		//output.str("");
	}
	
}

bool CShop::CheckCollision(IEntity* pOther)
{
	return false;
}

CShop::~CShop(void)
{
}

bool CShop::Shop()
{
	if( m_bIsShopping == false )
		m_bIsShopping = true;
	if( m_bIsShopping == true )
		m_bIsShopping = false;

	return m_bIsShopping;
}

void CShop::MiniMapRender()
{
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	RECT rSource = {0, 0, 128, 128};

	pTm->Draw(GetImageID(), (int)((GetPosX() * 0.0365f) +606), (int)((GetPosY()  * 0.0365f) +406), 0.08f, 0.08f, &rSource, 0,0,0, D3DCOLOR_RGBA(255,255,255,155));
}