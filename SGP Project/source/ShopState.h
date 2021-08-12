/*
	File:						ShopState.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Matthew Berry
	Last Edited:			10/08/2012
	Purpose:				purchases items
*/

#pragma once
#include "IGameState.h"

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/IListener.h"
#include "MessageSystem.h"
#include "ObjectManager.h"
#include "EventManager.h"
#include "BitmapFont.h"
#include "Entity.h"
#include "Ally.h"

typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CPlayer;

class CShopState : public IGameState
{
private:

	//securing our singleton class
	CShopState(void);
	~CShopState(void);
	CShopState( const CShopState& );
	CShopState& operator=( const CShopState& );

	//img IDs
	int						m_nBGImgID;
	int						m_nCursorImgID;
	int						m_nMenuImgID;
	int						m_nBitmapFontImgID;
	int						m_nCursorImgID2;

	//Sound IDs
	
	int						m_nBuyItemSFXID;
	int						m_nSelectionSFXID;

	// max bools
	bool					m_bMaxHealth;
	bool					m_bHealthpack;
	bool					m_bDamage;
	bool					m_bROF;
	bool					m_bArmor;
	bool					m_bSpeed;
	bool					m_bAlly;


	// Shop Prices
	int						m_nDamagePrice;
	int						m_nFireRatePrice;
	int						m_nArmorPrice;
	int						m_nMoveSpeedPrice;
	int						m_nUnitUpgradePrice;
	int						m_nMaxHealthPrice;


	//necessary Wrapper pointers
	CSGD_TextureManager*			m_pTM;
	CSGD_DirectInput*				m_pDI;
	CSGD_XAudio2*					m_pXA;

	//Bitmap Font
	CBitmapFont*					m_pFont;

	//player object that will be manipulated
	CPlayer*						m_pPlayer;      
	CAlly*							m_pAlly;

	//cursor
	int						m_nCursorPos;
	int						m_nTextPosX[12];
	int						m_nTextPosY[12];


	int						m_nPosX;
	int						m_nPosY; 

public:
	//singleton getinstance() declatation
	static CShopState* GetInstance();

	//functions that must be overridden
	virtual void Enter( void );	// enter the state
	virtual void Exit( void );	// exit the state

	virtual bool Input( void );	// handle user input
	virtual void Update( float fElapsedTime );	// update game objects
	virtual void Render( void );	// render game tokens

};

