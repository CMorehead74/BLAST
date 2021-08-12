/*
File:				ShopState.h
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Matthew Berry
Last Edited:		10/08/2012
Purpose:			purchases items
*/

#include "ShopState.h"
#include "Game.h"
#include "GameStateManager.h"
#include "Player.h"
#include "CreateAllyMessage.h"
#include "MessageSystem.h"
#include "PurchasePlacementState.h"

//singleton definition
CShopState* CShopState::GetInstance()
{
	static CShopState s_Instance;

	return &s_Instance;
}

CShopState::CShopState(void)
{
	m_pFont		= nullptr;
	m_pTM		= nullptr;
	m_pDI		= nullptr;
	m_pXA		= nullptr;

	m_pPlayer	= nullptr;
	m_pAlly     = nullptr;

	m_nBGImgID = -1;
	m_nCursorImgID = -1;
	m_nMenuImgID = -1;
	m_nCursorPos = 0;

	m_nBuyItemSFXID = -1;


	m_nSelectionSFXID =-1;	

	m_bMaxHealth	=false;
	m_bHealthpack	=false;
	m_bDamage		=false;
	m_bROF			=false;
	m_bArmor		=false;
	m_bSpeed		=false;
	m_bAlly			=false;


	m_nDamagePrice        = 500;
	m_nFireRatePrice	  = 500;
	m_nArmorPrice		  = 400;
	m_nMoveSpeedPrice	  = 300;
	m_nUnitUpgradePrice   = 1000;
	m_nMaxHealthPrice     = 400;

}


CShopState::~CShopState(void)
{
}

void CShopState::Enter( void )
{
	m_pTM		= CSGD_TextureManager::GetInstance();
	m_pDI		= CSGD_DirectInput::GetInstance();
	m_pXA  = CSGD_XAudio2::GetInstance();
	m_pFont		= CBitmapFont::GetInstance();

	m_nBGImgID = m_pTM->LoadTexture(_T("resource/graphics/Blast_Shop_Bg.png"), D3DCOLOR_XRGB(0,0,0));
	m_nCursorImgID	=	m_pTM->LoadTexture(_T("resource/graphics/Blast_MouseOver_Button.png"), D3DCOLOR_XRGB(0,0,0));
	m_nCursorImgID2	=	m_pTM->LoadTexture(_T("resource/graphics/Blast_Mouse_Cursor_PlacementEditor.png"), D3DCOLOR_XRGB(0,0,0));

	m_nMenuImgID	=	m_pTM->LoadTexture(_T("resource/graphics/Blast_Menu_Text_Bg.png"));

	// Load Bitmap Image
	m_nBitmapFontImgID		= m_pTM->LoadTexture(_T("resource/graphics/SansationFont_0.png"), D3DCOLOR_XRGB(0,0,0));
	m_pFont->SetImageID(m_nBitmapFontImgID);

	// Load Sounds

	m_nBuyItemSFXID = m_pXA->SFXLoadSound(_T("resource/sounds/SFX_BUYITEM.wav"));
	m_nSelectionSFXID = m_pXA->SFXLoadSound(_T("resource/sounds/SFX_SELECTHIGHLIGHTPause.wav"));

	list<IEntity*> objList = ObjectManager::GetInstance()->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objList.begin();
		iter != objList.end(); ++iter)
	{
		if((*iter)->GetType() == CEntity::OBJ_PLAYER)
			m_pPlayer = dynamic_cast<CPlayer*>(*iter);		
	}
	for(std::list<IEntity*>::iterator iter1 = objList.begin();
		iter1 != objList.end(); ++iter1)
	{
		if((*iter1)->GetType() == CEntity::OBJ_ALLY)
			m_pAlly = dynamic_cast<CAlly*>(*iter1);		
	}

	if(m_pPlayer->GetMaxHealth()<325)	m_bMaxHealth	=false;
	else								m_bMaxHealth	=true;

	if(m_pPlayer->GetHealthPacks() < m_pPlayer->GetMaxHealthPacks())	m_bHealthpack	=false;
	else																m_bHealthpack	=true;

	if(m_pPlayer->GetDamage()<35)		m_bDamage		=false;
	else								m_bDamage		=true;

	if(m_pPlayer->GetFireRate()>0.1f)	m_bROF			=false;
	else								m_bROF			=true;

	if(m_pPlayer->GetArmor()<15)		m_bArmor		=false;
	else								m_bArmor		=true;

	if(m_pPlayer->GetMoveSpeed()<325)	m_bSpeed		=false;
	else								m_bSpeed		=true;

	if(m_pAlly!=nullptr)
	{
	if(m_pAlly->GetDamage()<35)			m_bAlly		=false;
	else								m_bAlly		=true;
	}

	//mouse position
	m_nCursorPos = 0;

	m_nTextPosX[0] = 138;
	m_nTextPosX[1] = 295;
	m_nTextPosX[2] = 452;
	m_nTextPosX[3] = 610;
	m_nTextPosX[4] = 52;
	m_nTextPosX[5] = 162;
	m_nTextPosX[6] = 270;
	m_nTextPosX[7] = 375;
	m_nTextPosX[8] = 490;
	m_nTextPosX[9] = 598;
	m_nTextPosX[10] = 707;
	m_nTextPosX[11] = 245;

	/*m_nTextPosX[0] = 150;
	m_nTextPosX[1] = 180;
	m_nTextPosX[2] = 210;
	m_nTextPosX[3] = 240;
	m_nTextPosX[4] = 310;
	m_nTextPosX[5] = 340;
	m_nTextPosX[6] = 370;
	m_nTextPosX[7] = 400;
	m_nTextPosX[8] = 430;
	m_nTextPosX[9] = 460;
	m_nTextPosX[10] = 490;
	m_nTextPosX[11] = 520;*/

	m_nTextPosY[0] = 200;
	m_nTextPosY[1] = 200;
	m_nTextPosY[2] = 200;
	m_nTextPosY[3] = 200;
	m_nTextPosY[4] = 395;
	m_nTextPosY[5] = 395;
	m_nTextPosY[6] = 395;
	m_nTextPosY[7] = 395;
	m_nTextPosY[8] = 395;
	m_nTextPosY[9] = 395;
	m_nTextPosY[10] = 395;
	m_nTextPosY[11] = 540;
}

void CShopState::Exit( void )
{
	/*if( m_pPlayer != nullptr )
		m_pPlayer->SetPosY(m_pPlayer->GetPosY() + 64);
	m_pPlayer = nullptr;*/
	// cleaning up the Images:	

	//Cleaning the purchase placement state here since the message created in purchase placement are processed only exiting the shop.
	CPurchasePlacementState::GetInstance()->CleanUp();

	if(m_nCursorImgID != -1)
	{
		m_pTM->UnloadTexture(m_nCursorImgID);
		m_nCursorImgID = -1;
	}
	if(m_nBGImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBGImgID);
		m_nBGImgID = -1;
	}

	if(m_nBuyItemSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nBuyItemSFXID);
		m_pXA->SFXUnloadSound(m_nBuyItemSFXID);
		m_nBuyItemSFXID = -1;
	}	
	if(m_nSelectionSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nSelectionSFXID);
		m_pXA->SFXUnloadSound(m_nSelectionSFXID);
		m_nSelectionSFXID = -1;
	}	

}

bool CShopState::Input( void )
{
	CEventManager*	pEM	=CEventManager::GetInstance();
	CMessageSystem* pMsg = CMessageSystem::GetInstance();

	if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(6) )
	{
		pEM->sendEvent("leave_shop");

		CGameStateManager::GetInstance()->PreviousState();
		return true;
	}

	// Move Up the Menu
	if(m_pDI->KeyPressed(DIK_UP) || m_pDI->JoystickGetLStickDirPressed(DIR_UP) )
	{
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos--;
		if(m_nCursorPos < 0)
			m_nCursorPos = 11;
	}

	// Move Down the Menu
	if(m_pDI->KeyPressed(DIK_DOWN) || m_pDI->JoystickGetLStickDirPressed(DIR_DOWN))
	{
		m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos++;
		if(m_nCursorPos > 11)
			m_nCursorPos = 0;
	}

	// Select an Option
	if(m_pDI->KeyPressed(DIK_RETURN) || m_pDI->JoystickButtonPressed(0) )
	{
		//m_pXA->SFXPlaySound(m_nBuyItemSFXID);
		switch(m_nCursorPos)
		{
		case ALLY_GUNNER:		//Machine Gunner - $100
			{
				if(m_pPlayer->GetCash() >= 100)
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
						m_pXA->SFXPlaySound(m_nBuyItemSFXID);
					m_pPlayer->DecreaseCash( 100 );
					CCreateAllyMessage* pAllyMsg = new CCreateAllyMessage( ALLY_GUNNER );
					pMsg->SendMsg( pAllyMsg );
					pAllyMsg = nullptr;
					return true;
				}
			}
			break;

		case ALLY_SNIPER: // Sniper - $200
			{
				if(m_pPlayer->GetCash() >= 200)
				{
					m_pXA->SFXPlaySound(m_nBuyItemSFXID);
					m_pPlayer->DecreaseCash( 200 );
					CCreateAllyMessage* pAllyMsg = new CCreateAllyMessage( ALLY_SNIPER );
					pMsg->SendMsg( pAllyMsg );
					pAllyMsg = nullptr;
				}
				return true;

			}
			break;

		case ALLY_ROCKET: // Rocketeer - $300
			{
				if(m_pPlayer->GetCash() >= 300)
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
						m_pXA->SFXPlaySound(m_nBuyItemSFXID);
					m_pPlayer->DecreaseCash( 300 );
					CCreateAllyMessage* pAllyMsg = new CCreateAllyMessage( ALLY_ROCKET );
					pMsg->SendMsg( pAllyMsg );
					pAllyMsg = nullptr;
				}
				return true;
			}
			break;
		case ALLY_MEDIC: // Medic - $400
			{
				if(m_pPlayer->GetCash() >= 400)
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
						m_pXA->SFXPlaySound(m_nBuyItemSFXID);
					m_pPlayer->DecreaseCash( 400 );
					CCreateAllyMessage* pAllyMsg = new CCreateAllyMessage( ALLY_MEDIC );
					pMsg->SendMsg( pAllyMsg );
					pAllyMsg = nullptr;
				}
				return true;
			}
			break;

		case 4: // Health Boost - $1000
			{
				if(m_pPlayer->GetCash() >= 500)
				{
					if(m_pPlayer->GetHealthPacks() < m_pPlayer->GetMaxHealthPacks() )
					{
						if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
							m_pXA->SFXPlaySound(m_nBuyItemSFXID);
						m_pPlayer->DecreaseCash( 500 );
						m_pPlayer->IncreaseHealthPacks(1);
					}
				}
				return true;
			}
			break;
		case 5: // More Max HP - $500
			{	
				if(m_pPlayer->GetCash() >= m_nMaxHealthPrice)
				{
					if(m_pPlayer->GetMaxHealth() < 325 )
					{
						if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
							m_pXA->SFXPlaySound(m_nBuyItemSFXID);

						if(m_pPlayer->GetMaxHealth() < 325 )
						{
							m_pPlayer->DecreaseCash( m_nMaxHealthPrice );
							m_pPlayer->IncreaseMaxHealth( 25 );
							m_nMaxHealthPrice += 100;
						}
					}
				}
				return true;
			}
			break;
		case 6:
			{
				if(m_pPlayer->GetCash() >= m_nFireRatePrice)
				{					
					if(m_pPlayer->GetFireRate()  > 0.1f)
					{
						if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
							m_pXA->SFXPlaySound(m_nBuyItemSFXID);

						m_pPlayer->DecreaseCash( m_nFireRatePrice );
						m_pPlayer->IncreaseRateOfFire( -0.03f);
						m_nFireRatePrice += 100;
					}
				}
				return true;
			}
			break;
		case 7:
			{
				if(m_pPlayer->GetCash() >= m_nDamagePrice)
				{
					if( m_pPlayer->GetDamage() < 35 )
					{
						if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
							m_pXA->SFXPlaySound(m_nBuyItemSFXID);
						m_pPlayer->DecreaseCash( m_nDamagePrice );
						m_pPlayer->IncreaseDamage( 5 );
						m_nDamagePrice += 100;
					}
				}
				return true;
			}
			break;
		case 8:
			{
				if( m_pPlayer->GetCash() >= m_nArmorPrice)
				{
					if(m_pPlayer->GetArmor( ) < 15 )
					{
						if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
							m_pXA->SFXPlaySound(m_nBuyItemSFXID);
						m_pPlayer->DecreaseCash( m_nArmorPrice );
						m_pPlayer->IncreaseArmor( 3 );
						m_nArmorPrice += 100;
					}
				}
				return true;
			}
			break;
		case 9:
			{
				if( m_pPlayer->GetCash() >= m_nMoveSpeedPrice )
				{
					if(m_pPlayer->GetMoveSpeed() < 325 )
					{
						if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
							m_pXA->SFXPlaySound(m_nBuyItemSFXID);

						m_pPlayer->DecreaseCash( m_nMoveSpeedPrice );
						m_pPlayer->IncreaseMovementSpeed( 25 );
						m_nMoveSpeedPrice += 100;
					}
				}
				return true;
			}
			break;
		case 10:
			{
				if(m_pPlayer->GetCash() >= m_nUnitUpgradePrice)
				{
					if(m_pAlly != nullptr)
					{
						if(m_pAlly->GetDamage() < 35 )
						{
							if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
								m_pXA->SFXPlaySound(m_nBuyItemSFXID);

							m_pPlayer->DecreaseCash( m_nUnitUpgradePrice );
							m_pAlly->IncreaseDamage( 5 );
							m_pAlly->IncreaseArmor( 2 );
							m_nUnitUpgradePrice += 200;

						}
					}
				}
				return true;
			}
			break;
		case 11:
			{				
				//m_pXA->SFXStopSound(m_nBuyItemSFXID);
				CGameStateManager::GetInstance()->NextState(CPurchasePlacementState::GetInstance());
				return true;
			}
			break;
		};
	}



	if(m_nPosY > 225 && m_nPosY < 259 && m_nPosX <= 204 && m_nPosX >= 138)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos != 0)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 0;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_pPlayer->GetCash() >= 100)
			{
				if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
					m_pXA->SFXPlaySound(m_nBuyItemSFXID);
				m_pPlayer->DecreaseCash( 100 );
				CCreateAllyMessage* pAllyMsg = new CCreateAllyMessage( ALLY_GUNNER );
				pMsg->SendMsg( pAllyMsg );
				pAllyMsg = nullptr;
				return true;
			}
		}
	}

	if(m_nPosY > 225 && m_nPosY < 259 && m_nPosX <= 363 && m_nPosX >= 295)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos !=1)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 1;

		if(m_pDI->MouseButtonPressed(0))
		{
			m_pXA->SFXPlaySound(m_nBuyItemSFXID);
			m_pPlayer->DecreaseCash( 200 );
			CCreateAllyMessage* pAllyMsg = new CCreateAllyMessage( ALLY_SNIPER );
			pMsg->SendMsg( pAllyMsg );
			pAllyMsg = nullptr;
			return true;
		}
	}

	if(m_nPosY > 225 && m_nPosY < 259 && m_nPosX <= 520 && m_nPosX >= 452)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos != 2)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 2;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_pPlayer->GetCash() >= 200)
			{
				if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
					m_pXA->SFXPlaySound(m_nBuyItemSFXID);
				m_pPlayer->DecreaseCash( 200 );
				CCreateAllyMessage* pAllyMsg = new CCreateAllyMessage( ALLY_ROCKET );
				pMsg->SendMsg( pAllyMsg );
				pAllyMsg = nullptr;
			}
			return true;
		}
	}

	if(m_nPosY > 225 && m_nPosY < 259 && m_nPosX <= 678 && m_nPosX >= 610)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos != 3)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 3;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_pPlayer->GetCash() >= 300)
			{
				if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
					m_pXA->SFXPlaySound(m_nBuyItemSFXID);
				m_pPlayer->DecreaseCash( 300 );
				CCreateAllyMessage* pAllyMsg = new CCreateAllyMessage( ALLY_MEDIC );
				pMsg->SendMsg( pAllyMsg );
				pAllyMsg = nullptr;
			}
		}
		return true;
	}

	if(m_nPosY > 420 && m_nPosY < 454 && m_nPosX <= 120 && m_nPosX >= 52)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos != 4)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 4;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_pPlayer->GetCash() >= 500)
			{
				if(m_pPlayer->GetHealthPacks() < m_pPlayer->GetMaxHealthPacks())
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
						m_pXA->SFXPlaySound(m_nBuyItemSFXID);
					m_pPlayer->DecreaseCash( 500 );
					m_pPlayer->IncreaseHealthPacks(1);
				}
			}
		}
		return true;
	}

	if(m_nPosY > 420 && m_nPosY < 454 && m_nPosX <= 230 && m_nPosX >= 162)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos != 5)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 5;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_pPlayer->GetCash() >= m_nMaxHealthPrice)
			{
				if(m_pPlayer->GetMaxHealth() < 325 )
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
						m_pXA->SFXPlaySound(m_nBuyItemSFXID);

					m_pPlayer->DecreaseCash( m_nMaxHealthPrice );
					m_pPlayer->IncreaseMaxHealth( 25 );
					m_nMaxHealthPrice += 100;
				}
			}
		}
		return true;
	}

	if(m_nPosY > 420 && m_nPosY < 454 && m_nPosX <= 338 && m_nPosX >= 270)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false && m_nCursorPos != 6)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 6;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_pPlayer->GetCash() >= m_nFireRatePrice)
			{
				if(m_pPlayer->GetFireRate()  > 0.1f)
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
						m_pXA->SFXPlaySound(m_nBuyItemSFXID);
					m_pPlayer->DecreaseCash( m_nFireRatePrice );
					m_pPlayer->IncreaseRateOfFire( -0.03f);
					m_nFireRatePrice += 100;
				}
			}
		}
		return true;
	}

	if(m_nPosY > 420 && m_nPosY < 454 && m_nPosX <= 443 && m_nPosX >= 375)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos != 7)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 7;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_pPlayer->GetCash() >= m_nDamagePrice)
			{
				if( m_pPlayer->GetDamage() < 35 )
				{
				if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
							m_pXA->SFXPlaySound(m_nBuyItemSFXID);
					m_pPlayer->DecreaseCash( m_nDamagePrice );
					m_pPlayer->IncreaseDamage( 5 );
					m_nDamagePrice += 100;
				}
			}
		}
		return true;
	}

	if(m_nPosY > 420 && m_nPosY < 454 && m_nPosX <= 558 && m_nPosX >= 490)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos != 8)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 8;

		if(m_pDI->MouseButtonPressed(0))
		{
			if( m_pPlayer->GetCash() >= m_nArmorPrice)
			{
				if(m_pPlayer->GetArmor( ) < 15 )
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
						m_pXA->SFXPlaySound(m_nBuyItemSFXID);
					m_pPlayer->DecreaseCash( m_nArmorPrice );
					m_pPlayer->IncreaseArmor( 3 );
					m_nArmorPrice += 100;
				}
			}
		}
		return true;
	}

	if(m_nPosY > 420 && m_nPosY < 454 && m_nPosX <= 666 && m_nPosX >= 598)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos !=9)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 9;

		if(m_pDI->MouseButtonPressed(0))
		{
			if( m_pPlayer->GetCash() >= m_nMoveSpeedPrice )
			{
				if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
							m_pXA->SFXPlaySound(m_nBuyItemSFXID);
					m_pPlayer->DecreaseCash( m_nMoveSpeedPrice );
					m_pPlayer->IncreaseMovementSpeed( 25 );
					m_nMoveSpeedPrice += 100;
				}
			}
		}
		return true;	
	}

	if(m_nPosY > 420 && m_nPosY < 454 && m_nPosX <= 775 && m_nPosX >= 707)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos != 10)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 10;

		if(m_pDI->MouseButtonPressed(0))
		{
			if(m_pPlayer->GetCash() >= m_nUnitUpgradePrice)
			{
				if(m_pAlly != nullptr)
				{if(m_pAlly->GetDamage() < 35 )
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBuyItemSFXID)==false)
						m_pXA->SFXPlaySound(m_nBuyItemSFXID);

					m_pPlayer->DecreaseCash( m_nUnitUpgradePrice );
					m_pAlly->IncreaseDamage( 5 );
					m_pAlly->IncreaseArmor( 2 );
					m_nUnitUpgradePrice += 200;
				}
				}
			}
		}
		return true;
	}

	if(m_nPosY > 540 && m_nPosY < 574 && m_nPosX <= 605 && m_nPosX >= 245)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionSFXID)==false&&m_nCursorPos != 11)
			m_pXA->SFXPlaySound(m_nSelectionSFXID);
		m_nCursorPos = 11;

		if(m_pDI->MouseButtonPressed(0))
		{
			CGameStateManager::GetInstance()->NextState(CPurchasePlacementState::GetInstance());
		}
	}

	return true;
}

void CShopState::Update( float fElapsedTime )
{

	m_nPosX = m_pDI->MouseGetPosX();
	m_nPosY = m_pDI->MouseGetPosY();

	if(m_pPlayer->GetMaxHealth()<325)	m_bMaxHealth	=false;
	else								m_bMaxHealth	=true;
	
	if(m_pPlayer->GetHealthPacks() < m_pPlayer->GetMaxHealthPacks())	m_bHealthpack	=false;
	else																m_bHealthpack	=true;
	
	if(m_pPlayer->GetDamage()<35)		m_bDamage		=false;
	else								m_bDamage		=true;
	
	if(m_pPlayer->GetFireRate()>0.1f)	m_bROF			=false;
	else								m_bROF			=true;
	
	if(m_pPlayer->GetArmor()<15)		m_bArmor		=false;
	else								m_bArmor		=true;
	
	if(m_pPlayer->GetMoveSpeed()<325)	m_bSpeed		=false;
	else								m_bSpeed		=true;
	
	if(m_pAlly!=nullptr)
	{
	if(m_pAlly->GetDamage()<35)			m_bAlly		=false;
	else								m_bAlly		=true;
	}

	//mouse out of bounds check
	if( m_pDI->MouseGetPosX() < 0 )
		m_pDI->MouseSetPosX(0);
	if( m_pDI->MouseGetPosY() < 0 )
		m_pDI->MouseSetPosY(0);
	if( m_pDI->MouseGetPosX() > CGame::GetInstance()->GetWidth() - 50 )
		m_pDI->MouseSetPosX( CGame::GetInstance()->GetWidth() - 50);
	if( m_pDI->MouseGetPosY() > CGame::GetInstance()->GetHeight() - 50 )
		m_pDI->MouseSetPosY( CGame::GetInstance()->GetHeight() - 50);

}

void CShopState::Render( void )
{
	CGame* pGame = CGame::GetInstance();

	RECT rSource = { 0, 0, 800, 600 };
	m_pTM->Draw(m_nBGImgID, 0, 0, 1.0f, 1.0f, &rSource);

	// Render the Menu Background
	RECT rMenuSource = { 0, 0, 256, 450 };
	//m_pTM->Draw(m_nMenuImgID, 10, 100, 1.0f, 1.1f, &rMenuSource);

	//m_pFont->PrintString("Shop", (int)(pGame->GetWidth() * 0.5f - 100), 10, 1.5f );

	//cursor position
	SetRect(&rSource, 0, 0, 68, 33);
	if(m_nCursorPos != 11)
		m_pTM->Draw(m_nCursorImgID, m_nTextPosX[m_nCursorPos], m_nTextPosY[m_nCursorPos]+25, 1.0f, 1.0f, &rSource);

	//buying units
	//m_pFont->PrintString("Units", 80, 120, 0.75f);
	m_pFont->PrintString("$100", m_nTextPosX[0], m_nTextPosY[0], 0.75f, 255, 255, 0, 0);
	m_pFont->PrintString("$200", m_nTextPosX[1], m_nTextPosY[1], 0.75f, 255, 255, 0, 0);
	m_pFont->PrintString("$300", m_nTextPosX[2], m_nTextPosY[2], 0.75f, 255, 255, 0, 0);
	m_pFont->PrintString("$400", m_nTextPosX[3], m_nTextPosY[3], 0.75f, 255, 255, 0, 0);

	//Buying buying Upgrades

	ostringstream output;

	if(!m_bMaxHealth){	output << "$" << m_nMaxHealthPrice;}
	else			 {	output << "Max";				}
	m_pFont->PrintString(output.str().c_str(), m_nTextPosX[5], m_nTextPosY[5], 0.75, 255, 255, 0, 0); 
	output.str("");

	if(!m_bROF)		{	output << "$" << m_nFireRatePrice;}
	else			{	output << "Max";			 }
	m_pFont->PrintString(output.str().c_str(), m_nTextPosX[6], m_nTextPosY[6], 0.75, 255, 255, 0, 0); 
	output.str("");

	if(!m_bDamage)	{	output << "$" << m_nDamagePrice;}
	else			{	output << "Max";			 }
	m_pFont->PrintString(output.str().c_str(), m_nTextPosX[7], m_nTextPosY[7], 0.75, 255, 255, 0, 0); 
	output.str("");

	if(!m_bArmor)	{	output << "$" << m_nArmorPrice;}
	else			{	output << "Max";		   }
		m_pFont->PrintString(output.str().c_str(), m_nTextPosX[8], m_nTextPosY[8], 0.75, 255, 255, 0, 0); 
		output.str("");

	if(!m_bSpeed)	{	output << "$" << m_nMoveSpeedPrice;}
	else			{	output << "Max";			   }
		m_pFont->PrintString(output.str().c_str(), m_nTextPosX[9], m_nTextPosY[9], 0.75, 255, 255, 0, 0); 
		output.str("");

	if(!m_bAlly)	{	output << "$" << m_nUnitUpgradePrice;}
	else			{	output << "Max";				 }
		m_pFont->PrintString(output.str().c_str(), m_nTextPosX[10], m_nTextPosY[10], 0.75, 255, 255, 0, 0); 
		output.str("");

	//m_pFont->PrintString("Upgrades", 80, 280, 0.75f);

	if(!m_bHealthpack){	m_pFont->PrintString("$500", m_nTextPosX[4], m_nTextPosY[4], 0.75f, 255, 255, 0, 0);	  }
	else			  {	m_pFont->PrintString("Max", m_nTextPosX[4], m_nTextPosY[4], 0.75f, 255, 255, 0, 0);}

	//	m_pFont->PrintString("Max Health - $500", 40, m_nTextPos[5], 0.75f);
	//	m_pFont->PrintString("Rate Of Fire - $300", 40, m_nTextPos[6], 0.75f);
	//	m_pFont->PrintString("Damage  - $500", 40, m_nTextPos[7], 0.75f);
	//	m_pFont->PrintString("Armor  - $250", 40, m_nTextPos[8], 0.75f);
	//	m_pFont->PrintString("Speed  - $500", 40, m_nTextPos[9], 0.75f);
	//	m_pFont->PrintString("Units  - $1000", 40, m_nTextPos[10], 0.75f);
	//m_pFont->PrintString("Buildings", m_nTextPosX[11], m_nTextPosY[11], 0.75f);

	//	ostringstream output;
	/*
	output << "Cash: " << m_pPlayer->GetCash();
	m_pFont->PrintString(output.str().c_str(), 400, 53, 1.5f, 255, 255, 0, 0);
	output.str("");*/


	// Draw the Top Status Bar (CASH, FOOD, ORE)
	SetRect(&rSource, 559, 64, 905, 93);
	m_pTM->Draw(m_pPlayer->GetHUDImgID(), 424, 19, 1.0f, 1.0f, &rSource);

	// Cash Text
	output << m_pPlayer->GetCash();
	m_pFont->PrintString(output.str().c_str(), 488, 23, 0.75f, 255, 255, 255, 255);
	output.str("");

	// Ore Text
	output << m_pPlayer->GetOre();
	m_pFont->PrintString(output.str().c_str(), 602, 23, 0.75f, 255, 255, 255, 255);
	output.str("");

	// Food Text
	output << m_pPlayer->GetFood();
	m_pFont->PrintString(output.str().c_str(), 715, 23, 0.75f, 255, 255, 255, 255);
	output.str("");

	m_pTM->Draw(m_nCursorImgID2, m_nPosX, m_nPosY, 1.0f, 1.0f);

}

