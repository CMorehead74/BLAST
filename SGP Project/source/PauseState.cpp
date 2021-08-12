/*
File:				PauseState.cpp
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Michael Yatckoske
Last Edited:		9/12/2012
Purpose:			Pauses the gameplay state.
*/

#include "PauseState.h"
#include "Game.h"
#include "GameStateManager.h"
#include "OptionsState.h"
#include "../tinyxml/tinyxml.h"
#include "FileManagementState.h"

// SINGLETON!
CPauseState* CPauseState::GetInstance( void )
{
	// Lazy instantiation
	//	- created the first time the function is called
	static CPauseState s_Instance;

	return &s_Instance;
}

CPauseState::CPauseState(void)
{
	// Assign data members to clean values
	m_pD3D	= nullptr;
	m_pTM	= nullptr;
	m_pDI	= nullptr;
	m_pXA	= nullptr;
	m_pFont = nullptr;

	m_bSaveGame = false;
	m_bOptions	= false;

	m_nCursorPos = 0;
	m_nSelectionPauseSFXID=-1;
	m_nEnterPauseSFXID=-1;

}

void CPauseState::Enter(void)
{
	// Access the singletons of the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM  = CSGD_TextureManager::GetInstance();
	m_pDI  = CSGD_DirectInput::GetInstance();
	m_pXA  = CSGD_XAudio2::GetInstance();
	m_pFont = CBitmapFont::GetInstance();

	m_bSaveGame = false;
	m_bOptions	= false;

	// Load Textures
	m_nCursorImgID	=	m_pTM->LoadTexture(_T("resource/graphics/Blast_MenuCursorHover.png"), D3DCOLOR_XRGB(0,0,0));
	m_nCursorImgID2		= m_pTM->LoadTexture(_T("resource/graphics/Blast_Mouse_Cursor_PlacementEditor.png"), D3DCOLOR_XRGB(255,255,255));

	m_nMenuImgID	=	m_pTM->LoadTexture(_T("resource/graphics/Blast_Menu_Text_Bg.png"));

	// Load Bitmap Image
	m_nBitmapFontImgID		= m_pTM->LoadTexture(_T("resource/graphics/SansationFont_0.png"), D3DCOLOR_XRGB(0,0,0));
	m_pFont->SetImageID(m_nBitmapFontImgID);

	// Load sounds
	m_nSelectionPauseSFXID = m_pXA->SFXLoadSound(_T("resource/sounds/SFX_SELECTHIGHLIGHTPause.wav"));
	m_nEnterPauseSFXID	   = m_pXA->SFXLoadSound(_T("resource/sounds/SFX_SELECTENTERPause.wav"));

	// Set Up Cursor Position
	m_nCursorPos = 0;

	// Set Up Text Position
	m_nTextPos[0] = 240;
	m_nTextPos[1] = 270;
	m_nTextPos[2] = 300;
	m_nTextPos[3] = 330;
}

void CPauseState::Exit(void)
{
	// Unload Images:
	if(m_nCursorImgID != -1)
	{
		m_pTM->UnloadTexture(m_nCursorImgID);
		m_nCursorImgID = -1;
	}
	if(m_nMenuImgID != -1)
	{
		m_pTM->UnloadTexture(m_nMenuImgID);
		m_nMenuImgID = -1;
	}

	// Unload Audio:
	if(m_nSelectionPauseSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nSelectionPauseSFXID);
		m_pXA->SFXUnloadSound(m_nSelectionPauseSFXID);
		m_nSelectionPauseSFXID = -1;
	}

	if(m_nEnterPauseSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nEnterPauseSFXID);
		m_pXA->SFXUnloadSound(m_nEnterPauseSFXID);
		m_nEnterPauseSFXID = -1;
	}	

}

bool CPauseState::Input(void)
{
		CGame* pGame = CGame::GetInstance();

	if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(6))
	{
	//	m_pXA->SFXPlaySound(m_nPauseLeaveSFXID);
		
		CGameStateManager::GetInstance()->PreviousState();
		return true;
	}

	// Move Up the Menu
	if(m_pDI->KeyPressed(DIK_UP) || m_pDI->JoystickGetLStickDirPressed(DIR_UP) )
	{
		m_pXA->SFXPlaySound(m_nSelectionPauseSFXID);
		m_nCursorPos--;
		if(m_nCursorPos < 0)
			m_nCursorPos = 3 ;
	}

	// Move Down the Menu
	if(m_pDI->KeyPressed(DIK_DOWN) || m_pDI->JoystickGetLStickDirPressed(DIR_DOWN) )
	{
		m_pXA->SFXPlaySound(m_nSelectionPauseSFXID);
		m_nCursorPos++;
		if(m_nCursorPos > 3)
			m_nCursorPos = 0;
	}

	// Select an Option
	if(m_pDI->KeyPressed(DIK_RETURN) || m_pDI->JoystickButtonPressed(0))
	{
	
		switch(m_nCursorPos)
		{
		case 0:
			{
				m_pXA->SFXPlaySound(m_nEnterPauseSFXID);
				CGameStateManager::GetInstance()->PreviousState();
				return true;
			}
			break;

		case 1:
			{
				m_pXA->SFXPlaySound(m_nEnterPauseSFXID);
				CGameStateManager::GetInstance()->NextState(COptionsState::GetInstance());
				m_bOptions = true;
				return true;
			}
			break;

		case 2:
			{
				if(CGamePlayState::GetInstance()->GetTutorialState() == false)
				{
					/*m_pXA->SFXPlaySound(m_nEnterPauseSFXID);
					SaveGame("game_save_1.xml");*/
					CGameStateManager::GetInstance()->NextState(CFileManagementState::GetInstance());
					/*SaveGame(CFileManagementState::GetInstance()->GetFileName());
					CFileManagementState::GetInstance()->SetFileName("");*/
					m_bSaveGame = true;
					return true;
				}
				else
				{
					CGamePlayState::GetInstance()->SetTutorialState(false);
					CGameStateManager::GetInstance()->PreviousState();
					return true;
				}
			}
			break;

		case 3:
			{
				m_pXA->SFXPlaySound(m_nEnterPauseSFXID);
				CGameStateManager::GetInstance()->PreviousState();
				CGameStateManager::GetInstance()->ChangeState(CMainMenuState::GetInstance());
				return true;
			}
			break;
		};
	}

	if(m_nPosY > 225 && m_nPosY < 255 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -220)
	{
				if(m_pXA->SFXIsSoundPlaying(m_nSelectionPauseSFXID)==false&&m_nCursorPos != 0)
		m_pXA->SFXPlaySound(m_nSelectionPauseSFXID);
		m_nCursorPos = 0;

		if(m_pDI->MouseButtonReleased(0))
		{
				m_pXA->SFXPlaySound(m_nEnterPauseSFXID);
				CGameStateManager::GetInstance()->PreviousState();
				return true;
		}


	}

	if(m_nPosY > 255 && m_nPosY < 285 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -220)
	{
				if(m_pXA->SFXIsSoundPlaying(m_nSelectionPauseSFXID)==false&&m_nCursorPos != 1)
		m_pXA->SFXPlaySound(m_nSelectionPauseSFXID);
		m_nCursorPos = 1;

		if(m_pDI->MouseButtonReleased(0))
		{
				m_pXA->SFXPlaySound(m_nEnterPauseSFXID);
				CGameStateManager::GetInstance()->NextState(COptionsState::GetInstance());
				m_bOptions = true;
				return true;

		}

	}
	if(m_nPosY > 285 && m_nPosY < 315 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -220)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionPauseSFXID)==false&&m_nCursorPos != 2)
			m_pXA->SFXPlaySound(m_nSelectionPauseSFXID);
		m_nCursorPos = 2;

		if(m_pDI->MouseButtonReleased(0))
		{	
			m_pXA->SFXPlaySound(m_nEnterPauseSFXID);
			if(CGamePlayState::GetInstance()->GetTutorialState() == false)
			{
				
				CGameStateManager::GetInstance()->NextState(CFileManagementState::GetInstance());
				m_bSaveGame = true;
				/*SaveGame(CFileManagementState::GetInstance()->GetFileName());
				CFileManagementState::GetInstance()->SetFileName("");*/
				return true;
			}
			else
			{
				CGamePlayState::GetInstance()->SetTutorialState(false);
				CGameStateManager::GetInstance()->PreviousState();
				return true;
			}
		}



	}
	if(m_nPosY > 315 && m_nPosX <= pGame->GetWidth() && m_nPosX >= pGame->GetWidth() -220)
	{
		if(m_pXA->SFXIsSoundPlaying(m_nSelectionPauseSFXID)==false&&m_nCursorPos != 3)
		m_pXA->SFXPlaySound(m_nSelectionPauseSFXID);
		m_nCursorPos = 3;

		if(m_pDI->MouseButtonReleased(0))
		{
				m_pXA->SFXPlaySound(m_nEnterPauseSFXID);
				CGameStateManager::GetInstance()->PreviousState();
				CGameStateManager::GetInstance()->ChangeState(CMainMenuState::GetInstance());
				return true;

		}
	}

	return true;
}

void CPauseState::Update(float fElapsedTime)
{
	m_nPosX = m_pDI->MouseGetPosX();
	m_nPosY = m_pDI->MouseGetPosY();

	if(m_bSaveGame == true)
	{
		SaveGame(CFileManagementState::GetInstance()->GetFileName());
		CFileManagementState::GetInstance()->SetFileName("");
		m_bSaveGame = false;
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

void CPauseState::Render(void)
{
	CGame* pGame = CGame::GetInstance();

	// Render the Menu Background
	RECT rSource = { 0, 0, 256, 450 };
	m_pTM->Draw(m_nMenuImgID, pGame->GetWidth() - 256, 50, 1.0f, 1.0f, &rSource);

	// Tell the Player that the game is Paused
	m_pFont->PrintString("PAUSED", pGame->GetWidth() - 220, 200, 1.0f);

	m_pTM->Draw(m_nCursorImgID, pGame->GetWidth() - 240, m_nTextPos[m_nCursorPos] - 10, 1.25f);
	
	m_pFont->PrintString("Resume Game", pGame->GetWidth() - 220, m_nTextPos[0], 0.75f);
	m_pFont->PrintString("Options", pGame->GetWidth() - 220, m_nTextPos[1], 0.75f);

	if(CGamePlayState::GetInstance()->GetTutorialState() == false)
		m_pFont->PrintString("Save Game", pGame->GetWidth() - 220, m_nTextPos[2], 0.75f);
	else
		m_pFont->PrintString("Skip Tutorial", pGame->GetWidth() - 220, m_nTextPos[2], 0.75f);

	m_pFont->PrintString("Back to Main Menu", pGame->GetWidth() - 220, m_nTextPos[3], 0.75f);

	if(m_bOptions == false)
		m_pTM->Draw(m_nCursorImgID2, m_nPosX, m_nPosY, 1.0f);
}

CPauseState::~CPauseState(void)
{
}

void CPauseState::SaveGame(const char* szFileName)
{
	// Create a pointer to the object manager
	ObjectManager* pOM = ObjectManager::GetInstance();

	// Create/Open Document
	TiXmlDocument doc;

	TiXmlDeclaration* pDec = new TiXmlDeclaration("1.0", "utf-8", "");

	doc.LinkEndChild(pDec);

	TiXmlElement* pRoot = new TiXmlElement("GAME_SAVE");
	doc.LinkEndChild(pRoot);

	// Save the Current Level
	TiXmlElement* pLevel = new TiXmlElement("level_info");
	pLevel->SetAttribute("current_level", CGamePlayState::GetInstance()->GetCurrentLevel());
	pRoot->LinkEndChild(pLevel);

	// Save the Player
	TiXmlElement* pPlayer = new TiXmlElement("player_info");
	// Loop through ObjectManager and find the player
	list<IEntity*> objectsList = pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		// If the player is found, save the data
		if((*iter)->GetType() == CEntity::OBJ_PLAYER)
		{
			CPlayer* obj = dynamic_cast<CPlayer*>((*iter));
			// Save Position
			pPlayer->SetDoubleAttribute("pos_x", obj->GetPosX());
			pPlayer->SetDoubleAttribute("pos_y", obj->GetPosY());
			// Save Health
			pPlayer->SetAttribute("health", obj->GetHealth());
			// Save Pick Up Time
			pPlayer->SetDoubleAttribute("pickup_timer", obj->GetPickupTimer());
			// Save Weapon Type
			pPlayer->SetAttribute("curr_weapon", obj->GetWeaponType());
			// Save Food
			pPlayer->SetDoubleAttribute("food_total", obj->GetFood());
			// Save Fire Rate
			pPlayer->SetDoubleAttribute("fire_rate", obj->GetFireRate());
			// Save Ore Rate
			pPlayer->SetAttribute("ore_rate", obj->GetOreRate());
			// Save Food Rate
			pPlayer->SetAttribute("food_rate", obj->GetFoodRate());
			// Save Damage
			pPlayer->SetAttribute("damage", obj->GetDamage());
			// Save Ammo
			pPlayer->SetAttribute("ammo", obj->GetAmmo());
			// Save Cash
			pPlayer->SetAttribute("cash", obj->GetCash());
			// Save Ore
			pPlayer->SetAttribute("ore", obj->GetOre());
			// Save Healthpacks
			pPlayer->SetAttribute("health_packs", obj->GetHealthPacks());
			// Save Max Health
			pPlayer->SetAttribute("max_health", obj->GetMaxHealth());
			// Save Armor
			pPlayer->SetAttribute("armor", obj->GetArmor());
			// Save Kills
			pPlayer->SetAttribute("kills", obj->GetKills());

			break;
		}
	}
	pRoot->LinkEndChild(pPlayer);

	// Save the Allies
	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		if((*iter)->GetType() == CEntity::OBJ_ALLY)
		{
			TiXmlElement* pAllies = new TiXmlElement("ally_info");

			CAlly* obj = dynamic_cast<CAlly*>((*iter));
			// Save Position
			pAllies->SetDoubleAttribute("pos_x", obj->GetPosX());
			pAllies->SetDoubleAttribute("pos_y", obj->GetPosY());
			// Save Health
			pAllies->SetAttribute("health", obj->GetHealth());
			// Save Type
			pAllies->SetAttribute("type", obj->GetType());

			pRoot->LinkEndChild(pAllies);
		}
	}

	// Save the Helicopter
	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		if((*iter)->GetType() == CEntity::OBJ_HELICOPTER)
		{
			TiXmlElement* pHelicopter = new TiXmlElement("helicopter_info");

			CHelicopter* obj = dynamic_cast<CHelicopter*>((*iter));
			// Save Position
			pHelicopter->SetDoubleAttribute("pos_x", obj->GetPosX());
			pHelicopter->SetDoubleAttribute("pos_y", obj->GetPosY());
			// Save Health
			pHelicopter->SetAttribute("health", obj->GetHealth());
			// Save Booleans
			pHelicopter->SetAttribute("player_found", obj->GetPlayerFound());
			pHelicopter->SetAttribute("on_the_ground", obj->GetOnGround());
			pHelicopter->SetAttribute("is_flying", obj->GetFlying());
			pHelicopter->SetAttribute("is_spawning", obj->GetSpawning());
			pHelicopter->SetAttribute("is_landing", obj->GetLanding());

			pRoot->LinkEndChild(pHelicopter);
		}
	}

	// Save the Enemies
	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		if((*iter)->GetType() == CEntity::OBJ_ENEMY)
		{
			TiXmlElement* pEnemy = new TiXmlElement("enemy_info");

			CEnemy* obj = dynamic_cast<CEnemy*>((*iter));
			// Save Position
			pEnemy->SetDoubleAttribute("pos_x", obj->GetPosX());
			pEnemy->SetDoubleAttribute("pos_y", obj->GetPosY());
			// Save Health
			pEnemy->SetAttribute("health", obj->GetHealth());
			// Save Type
			pEnemy->SetAttribute("type", obj->GetUnitType());

			pRoot->LinkEndChild(pEnemy);
		}
	}

	// Save the Bunkers
	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		if((*iter)->GetType() == CEntity::OBJ_BUNKER)
		{
			TiXmlElement* pBunker = new TiXmlElement("bunker_info");

			CBunker* obj = dynamic_cast<CBunker*>((*iter));
			// Save Position
			pBunker->SetDoubleAttribute("pos_x", obj->GetPosX());
			pBunker->SetDoubleAttribute("pos_y", obj->GetPosY());
			// Save Health
			pBunker->SetAttribute("health", obj->GetHealth());
			// Save Current Units inside bunker
			pBunker->SetAttribute("curr_units_inside", obj->GetNumAllies());

			pRoot->LinkEndChild(pBunker);
		}
	}

	// Save the Storehouses
	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		if((*iter)->GetType() == CEntity::OBJ_STOREHOUSE)
		{
			TiXmlElement* pStorehouse = new TiXmlElement("storehouse_info");

			CStoreHouse* obj = dynamic_cast<CStoreHouse*>((*iter));
			// Save Position
			pStorehouse->SetDoubleAttribute("pos_x", obj->GetPosX());
			pStorehouse->SetDoubleAttribute("pos_y", obj->GetPosY());
			// Save Health
			pStorehouse->SetAttribute("health", obj->GetHealth());

			pRoot->LinkEndChild(pStorehouse);
		}
	}

	// Save the Mines
	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		if((*iter)->GetType() == CEntity::OBJ_MINE)
		{
			TiXmlElement* pMine = new TiXmlElement("mine_info");

			CMine* obj = dynamic_cast<CMine*>((*iter));
			// Save Position
			pMine->SetDoubleAttribute("pos_x", obj->GetPosX());
			pMine->SetDoubleAttribute("pos_y", obj->GetPosY());
			// Save Health
			pMine->SetAttribute("health", obj->GetHealth());

			pRoot->LinkEndChild(pMine);
		}
	}

	doc.SaveFile(szFileName);
}