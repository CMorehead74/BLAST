/*
File:				GamePlayState.cpp
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Matthew Berry
Last Edited:		10/08/2012
Purpose:			Contain / manage the game code
*/

#include "GamePlayState.h"

// State Machine Includes
#include "Game.h"
#include "GameStateManager.h"
#include "MainMenuState.h"
#include "PauseState.h"
#include "ShopState.h"
#include "FileManagementState.h"
#include "CreditsState.h"

// Game Object Includes
#include "Player.h"
#include "Helicopter.h"
#include "Ally.h"
#include "Enemy.h"
#include "PickUp.h"
#include "Bullet.h"
#include "Shop.h"
#include "Building.h"
#include "Bunker.h"
#include "StoreHouse.h"
#include "Mine.h"

// Message System Includes
#include "Message.h"
#include "CreateAllyMessage.h"
#include "CreateEnemyMessage.h"
#include "CreatePickUpMessage.h"
#include "CreateBulletMessage.h"
#include "DestroyBulletMessage.h"
#include "CreateShotgunBullet.h"
#include "DestroyShotgunBullet.h"
#include "CreateBuildingMessage.h"
#include "DestroyBuildingMessage.h"
#include "CreateBunkerMessage.h"
#include "DestroyBunkerMessage.h"
#include "CreateStoreHouseMessage.h"
#include "DestroyStoreHouseMessage.h"
#include "CreateMineMessage.h"
#include "DestroyMineMessage.h"

//#include "AnimationInfo.h"
#include "DestroyEnemyMessage.h"
#include "DestroyPickUpMessage.h"
#include "DestroyAllyMessage.h"

//SGD Wrappers
#include "../SGD Wrappers/SGD_Math.h"

//Particle Manager
#include "ParticleManager.h"
//Tile Manager
#include "TileManager.h"
//Camera
#include "Camera.h"
//Bitmap Font
#include "BitmapFont.h"

#include "../tinyxml/tinyxml.h"

#include <assert.h>

// SINGLETON!
CGamePlayState* CGamePlayState::GetInstance( void )
{
	static CGamePlayState s_Instance;
	return &s_Instance;
}

CGamePlayState::CGamePlayState(void)
{
	m_pD3D	= nullptr;
	m_pTM	= nullptr;
	m_pDI	= nullptr;
	m_pXA	= nullptr;
	m_pMS	= nullptr;
	m_pOM	= nullptr;
	m_pFont = nullptr;
	m_pEM	= nullptr;

	m_pPlayer		= nullptr;
	m_pHelicopter	= nullptr;
	m_pShop			= nullptr;
	m_pMiniMap		= nullptr;

	m_nCurrLevel	= -1;
	m_fLevelEndTimer = 0.0f;
	m_bLevelComplete = false;
	m_bPaused		 = false;

	m_nBackgroundImgID		= -1;
	m_nEnemyMedicImgID		= -1;
	m_nHelicopterImgID		= -1;
	m_nAllyMedicImgID		= -1;
	m_nAllyMGImgID			= -1;
	m_nAllySniperImgID		= -1;
	m_nAllyRocketImgID		= -1;
	m_nPickUpAmmoImgID		= -1;
	m_nPickUpCashImgID		= -1;
	m_nPickUpRocketImgID	= -1;
	m_nPickUpMGImgID		= -1;
	m_nPickUpShotgunImgID	= -1;
	m_nPickUpSniperImgID	= -1;
	m_nBulletImgID			= -1;
	m_nBitmapFontImgID		= -1;
	m_nRocketImgID			= -1;

	m_nLoadingLogoBgImgID	= -1;
	m_nLoadingBulletImgID	= -1;

	m_nPercentageLoaded		= 0;

	m_nBackgroundMusID[0]	= -1;
	m_nBackgroundMusID[1]	= -1;
	m_nBackgroundMusID[2]	= -1;
	m_nBackgroundMusID[3]	= -1;
	m_nBackgroundMusID[4]	= -1;

	m_nWinSFXID				= -1;
	m_nPauseSFXID			= -1;
	m_nShopEnterSFXID		= -1;
	m_nLostSFXID			= -1;
	m_nChopterSFXID			= -1;
	m_nEnemyDeathSFXID		= -1;
	m_nWinSFXID				= -1;
	m_nExitStoreSFXID		= -1;
	m_bChopExplode			= false;
	m_nBulletCollidedSFXID	= -1;
	m_nRocketCollidedSFXID	= -1;
	m_fPickUpSpawnTimer		= 0.0f;

	m_nHurtIndicator		= -1;	//cjm - 10/06/12

	m_bTutotial = false;

	m_nMusIndex = rand() % 5;
}

void CGamePlayState::Enter(void)
{
	m_nCurrLevel++;
	if(m_nCurrLevel == 0)
	{
		m_bTutotial = true;
		m_nMusIndex = rand() % 5;
	}
	if(m_nCurrLevel == 0)
	{
		m_pTM	= CSGD_TextureManager::GetInstance();

		//Load Images in advance for the loading screen
		m_nLoadingLogoBgImgID	= m_pTM->LoadTexture(_T("resource/graphics/Blast_Loading_Logo_Bg.png"));
		m_nLoadingBulletImgID	= m_pTM->LoadTexture(_T("resource/graphics/Blast_Loading_Logo_Bullet.png"));
		m_nBackgroundImgID	= m_pTM->LoadTexture(_T("resource/graphics/Blast_GamePlayState_Bg.png"), D3DCOLOR_XRGB(0,0,0)); 
		m_nCursorPos		= CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/Blast_Mouse_Cursor.png"));


		m_fLevelEndTimer = 0.0f;
		m_bLevelComplete = false;

		//Access the singletons of the SGD Wrappers
		m_pD3D	= CSGD_Direct3D::GetInstance();
		m_nPercentageLoaded += 1;
		RenderLoading();
		m_pDI	= CSGD_DirectInput::GetInstance();
		m_nPercentageLoaded += 1;
		RenderLoading();
		m_pXA	= CSGD_XAudio2::GetInstance();
		m_nPercentageLoaded += 1;
		RenderLoading();
		m_pMS	= CMessageSystem::GetInstance();
		m_nPercentageLoaded += 1;
		RenderLoading();
		m_pOM	= ObjectManager::GetInstance();
		m_nPercentageLoaded += 1;
		RenderLoading();
		m_pFont = CBitmapFont::GetInstance();
		m_nPercentageLoaded += 1;
		RenderLoading();
		m_pEM	= CEventManager::GetInstance();
		m_nPercentageLoaded += 1;
		RenderLoading();

		m_pOM->RegisterClassType<CPlayer>(_T("CPlayer"));
		m_pOM->RegisterClassType<CBullet>(_T("CBullet"));
		m_pOM->RegisterClassType<CEnemy>(_T("CEnemy"));
		m_pOM->RegisterClassType<CPickUp>(_T("CPickUp"));
		m_pOM->RegisterClassType<CAlly>(_T("CAlly"));
		m_pOM->RegisterClassType<CHelicopter>(_T("CHelicopter"));
		m_pOM->RegisterClassType<CShop>(_T( "CShop"));
		m_pOM->RegisterClassType<CBunker>(_T( "CBunker"));
		m_pOM->RegisterClassType<CStoreHouse>( _T("CStoreHouse"));
		m_pOM->RegisterClassType<CMine>( _T("CMine"));

		m_nPercentageLoaded += 10;
		RenderLoading();

		// Load Images:
		InitImages();

		// Load Audio:
		InitAudio();
		m_nPercentageLoaded += 10;
		RenderLoading();

#pragma region PARTICLES	
		//m_pPM   = CParticleManager::GetInstance();
#pragma endregion 
		//Calling the loading render function
		RenderLoading();
#pragma region TILES
		m_pTileManager = CTileManager::GetInstance();
		// 	if( m_bTutotial == false )
		// 		m_pTileManager->LoadTiles("Level01.xml");
		// 	else
		// 		m_pTileManager->LoadTiles( "Tutorial.xml" );

#pragma endregion

		m_nPercentageLoaded += 20;
		RenderLoading();

		CEventManager::GetInstance()->RegisterClient( "enter_shop", this );
		CEventManager::GetInstance()->RegisterClient( "leave_shop", this );
		CEventManager::GetInstance()->RegisterClient( "collide_Rocket", this );
		CEventManager::GetInstance()->RegisterClient( "collide_Bullet", this );

		//m_pMuzzleFlashEffect = CParticleManager::GetInstance()->CreateEmitter("muzzleFlash");

		m_DeathSound = false;
		m_bGotHighScore = false;

		m_nCurrId[0] = 'A';
		m_nCurrId[1] = 'A';
		m_nCurrId[2] = 'A';
		m_nCurrPos = 0;

		// Set Up the Message System
		m_pMS->Initialize(&MessageProc);

		InitAnimations();

		//TODO: if a camera pointer is ever added 
		//to the Gameplay State ask ROB to change this line
		CCamera::GetInstance()->InitCamera(0,0,(float)CGame::GetInstance()->GetWidth(),(float)CGame::GetInstance()->GetHeight());
		
		m_nPercentageLoaded += 5;
		RenderLoading();
	}

	if(m_pXA->MusicIsSongPlaying(m_nBackgroundMusID[m_nMusIndex]) == true)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusID[m_nMusIndex]);
		m_nMusIndex = rand() % 5;
	}
	m_pXA->MusicPlaySong(m_nBackgroundMusID[m_nMusIndex], false);
	m_bPaused = false;

	if(CMainMenuState::GetInstance()->GetLoadingGame() == false)
	{
		switch(m_nCurrLevel)
		{
		case 0:
			// Start Tutorial
			TutorialInit();
			break;

		case 1:
			// Start Level 1
			StartDefaultLv1();
			break;

		case 2:
			// Start Level 2
			StartLevel2();
			break;
		};
	}
	else
	{
		if(LoadGame(CFileManagementState::GetInstance()->GetFileName()) == false)
		{
			TutorialInit();
			InitHelicopter();
			CMainMenuState::GetInstance()->SetLoadingGame(false);
			CFileManagementState::GetInstance()->SetFileName("");
		}
	}

	m_pMiniMap = new CMiniMap();

	m_nPercentageLoaded += 10;
	RenderLoading();

	//Intializing the Direct Input Mouse Position
	m_pDI->MouseSetPosX((int)m_pPlayer->GetPosX());
	m_pDI->MouseSetPosY((int)m_pPlayer->GetPosY());

	m_fGameOverTimer = 0.0f;
	m_bGameOver = false;


	m_bRenderCrossHairs = true;
}

void CGamePlayState::Exit(void)
{
	CCamera::GetInstance()->InitCamera(0,0,(float)CGame::GetInstance()->GetWidth(),(float)CGame::GetInstance()->GetHeight());
	//CCamera::GetInstance()->InitCamera(0,0,CGame::GetInstance()->GetWidth(),CGame::GetInstance()->GetHeight());
	// Unload Images	
	if(m_nBackgroundImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBackgroundImgID);
		m_nBackgroundImgID = -1;
	}

	// Unload Audio
	if(m_nBackgroundMusID[0] != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusID[0]);
		m_pXA->MusicUnloadSong(m_nBackgroundMusID[0]);
		m_nBackgroundMusID[0] = -1;
	}
	if(m_nBackgroundMusID[1] != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusID[1]);
		m_pXA->MusicUnloadSong(m_nBackgroundMusID[1]);
		m_nBackgroundMusID[1] = -1;
	}
	if(m_nBackgroundMusID[2] != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusID[2]);
		m_pXA->MusicUnloadSong(m_nBackgroundMusID[2]);
		m_nBackgroundMusID[2] = -1;
	}
	if(m_nBackgroundMusID[3] != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusID[3]);
		m_pXA->MusicUnloadSong(m_nBackgroundMusID[3]);
		m_nBackgroundMusID[3] = -1;
	}
	if(m_nBackgroundMusID[4] != -1)
	{
		m_pXA->MusicStopSong(m_nBackgroundMusID[4]);
		m_pXA->MusicUnloadSong(m_nBackgroundMusID[4]);
		m_nBackgroundMusID[4] = -1;
	}

	if(m_nPauseSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nPauseSFXID);
		m_pXA->SFXUnloadSound(m_nPauseSFXID);
		m_nPauseSFXID = -1;
	}
	if(m_nShopEnterSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nShopEnterSFXID);
		m_pXA->SFXUnloadSound(m_nShopEnterSFXID);
		m_nShopEnterSFXID = -1;
	}
	if(m_nLostSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nLostSFXID);
		m_pXA->SFXUnloadSound(m_nLostSFXID);
		m_nLostSFXID = -1;
	}			 

	if(m_nWinSFXID != -1)

	{
		m_pXA->SFXStopSound(m_nWinSFXID);
		m_pXA->SFXUnloadSound(m_nWinSFXID);
		m_nWinSFXID = -1;
	}
	if(m_nChopterSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nChopterSFXID);
		m_pXA->SFXUnloadSound(m_nChopterSFXID);
		m_nChopterSFXID = -1;
	}
	if(m_nEnemyDeathSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nEnemyDeathSFXID);
		m_pXA->SFXUnloadSound(m_nEnemyDeathSFXID);
		m_nEnemyDeathSFXID = -1;
	}
	if(m_nExitStoreSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nExitStoreSFXID);
		m_pXA->SFXUnloadSound(m_nExitStoreSFXID);
		m_nExitStoreSFXID = -1;
	}
	if(m_nBulletCollidedSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nBulletCollidedSFXID);
		m_pXA->SFXUnloadSound(m_nBulletCollidedSFXID);
		m_nBulletCollidedSFXID = -1;
	}
	if(m_nRocketCollidedSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nRocketCollidedSFXID);
		m_pXA->SFXUnloadSound(m_nRocketCollidedSFXID);
		m_nRocketCollidedSFXID = -1;
	}

	if(m_nCursorPos != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nCursorPos);
		m_nCursorPos = -1;
	}
	// Shutdown the Object Manager
	if(m_pOM != nullptr)
	{
		m_pOM->Shutdown();
		m_pOM = nullptr;
	}

	if(m_pPlayer != nullptr)
	{
		m_pPlayer->Release();
		m_pPlayer = nullptr;
	}
	if(m_pHelicopter != nullptr)
	{
		m_pHelicopter->Release();
		m_pHelicopter = nullptr;
	}
	if( m_pShop != nullptr )
	{
		m_pShop->Release();
		m_pShop = nullptr;
	}

	//unregistering the event
	m_pEM->UnregisterClient( "enter_shop", this );
	m_pEM->UnregisterClient( "leave_shop", this );
	m_pEM->UnregisterClient( "collide_Rocket", this );
	m_pEM->UnregisterClient( "collide_Bullet", this );

	if(m_pAnimInfo != nullptr)
	{
		delete m_pAnimInfo;
		m_pAnimInfo = nullptr;

	}

	m_nPercentageLoaded = 0;
	m_nCurrLevel = -1;

	//delete m_pMiniMap;
	m_pMiniMap->Release();
	m_pMiniMap = nullptr;

	m_bRenderCrossHairs = false;
}

bool CGamePlayState::Input(void)
{
	// Escape to Pause the Game
	if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(6) )
	{
		m_pXA->SFXPlaySound(m_nPauseSFXID);
		CGameStateManager::GetInstance()->NextState(CPauseState::GetInstance());
		m_bPaused = true;
		return true;
	}

	if(m_pDI->KeyDown(DIK_LALT))
	{
		if(m_pDI->KeyPressed(DIK_TAB))
		{
			CGameStateManager::GetInstance()->NextState(CPauseState::GetInstance());
			return true;
		}

		if(m_pDI->KeyPressed(DIK_F4))
		{
			//m_pOM->Shutdown();
			//CGameStateManager::GetInstance()->PreviousState();
			/*m_pPlayer->Release();
			m_pHelicopter->Release();
			m_pShop->Release();*/
			//m_pMiniMap->Release();

			/*delete m_pMiniMap;
			m_pMiniMap = nullptr;*/
			return false;
		}
	}
	if(m_nCurrLevel != 0)
	{
		if( m_pDI->KeyPressed(DIK_R))
		{
			m_pXA->SFXPlaySound(m_nShopEnterSFXID);
			m_pEM->sendEvent( "enter_shop" );
		}
	}

	if(m_bGotHighScore == true)
	{
		if(m_pDI->KeyPressed(DIK_UP) || m_pDI->JoystickGetLStickDirPressed(DIR_UP))
		{
			m_nCurrId[m_nCurrPos]++;
			if(m_nCurrId[m_nCurrPos] > 'Z')
				m_nCurrId[m_nCurrPos] = 'A';
		}

		if(m_pDI->KeyPressed(DIK_DOWN) || m_pDI->JoystickGetLStickDirPressed(DIR_DOWN) )
		{
			m_nCurrId[m_nCurrPos]--;
			if(m_nCurrId[m_nCurrPos] < 'A')
				m_nCurrId[m_nCurrPos] = 'Z';
		}

		if(m_pDI->KeyPressed(DIK_RETURN) || m_pDI->JoystickButtonPressed(0) )
		{
			m_nCurrPos++;
			if(m_nCurrPos > 2)
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
				tScores newScore;
				newScore.nScore = pPlayer->GetKills() * 100;

				//m_vScores[m_nScorePos].nScore = pPlayer->GetKills() * 100;
				std::ostringstream oss;
				oss << (char)m_nCurrId[0] << (char)m_nCurrId[1] << (char)m_nCurrId[2];
				strcpy_s(newScore.chName, 32, oss.str().c_str());

				m_vScores.insert(m_vScores.begin() + m_nScorePos, newScore);
				m_vScores.pop_back();

				SaveScores(m_vScores, "highscores.xml");

				CGameStateManager::GetInstance()->ChangeState(CCreditsState::GetInstance());
				return true;
			}
		}
	}

	if( m_pDI->KeyPressed(DIK_1) )
		m_bAttackActivate = true;
	if( m_pDI->KeyPressed(DIK_2) )
		m_bDefenseActivate = true;
	if( m_pDI->KeyPressed(DIK_3) )
		m_bFollowActivate = true;

	if( m_bTutotial == true )
	{
		if((m_pDI->MouseButtonPressed( (char)DIMOUSE_XAXISAB)) )
		{

			if (m_bTeacherMode == true )
			{
				switch( m_nCurrentLesson )
				{
				case 0:
					{
					}
					break;
				case 1:
					{
						CCreateEnemyMessage* pEnemy = new CCreateEnemyMessage();
						m_pMS->SendMsg( pEnemy );
						pEnemy = nullptr;

						m_nNumEnemies++;
						m_nNumPickups++;
					}
					break;
				case 2:
					{
						
					}
					break;
				case 3:
					{
						
					}
					break;
				case 4:
					{
					}
					break;
				case 5:
					{
						
					}
					break;
				case 6:
					{
					}
					break;
				case 7:
					{

					}
					break;
				case 8:
					{
					}
					break;
				case 9:
					{
						CCreateAllyMessage* pAlly = new CCreateAllyMessage( ALLY_MEDIC );
						m_pMS->SendMsg( pAlly );
					}
					break;
				case 10:
					{
						
					}
					break;
				}

				m_bTeacherMode = false;
			}
		}
	}
	return true;
}

void CGamePlayState::Update(float fElapsedTime)
{

	//CAnimationManager::GetInstance()->Update(m_pAnimInfo, fElapsedTime);

	if(m_pXA->MusicIsSongPlaying(m_nBackgroundMusID[m_nMusIndex]) == false)
	{
		m_nMusIndex = rand() % 5;
		m_pXA->MusicPlaySong(m_nBackgroundMusID[m_nMusIndex], false);
	}

	if (m_bTutotial == false )
	{
		if(m_nCurrLevel == 0)
		{
			list<IEntity*> objList = m_pOM->GetObjectList();
			for(std::list<IEntity*>::iterator iter = objList.begin();
				iter != objList.end(); ++iter)
			{
				switch((*iter)->GetType())
				{
				case CEntity::OBJ_ALLY:
				case CEntity::OBJ_ENEMY:
				case CEntity::OBJ_BULLET:
				case CEntity::OBJ_BUNKER:
				case CEntity::OBJ_STOREHOUSE:
				case CEntity::OBJ_MINE:
				case CEntity::OBJ_SHOP:
					m_pOM->RemoveObject((*iter));
					break;
				};
			}
			m_pOM->RemoveObject(m_pPlayer);
			m_pPlayer->Release();
			m_pMiniMap->Release();
			m_pShop->Release();

			Enter();
		}	

		if(m_pPlayer->GetHealth() > 0 && m_pHelicopter->GetHealth() > 0 && m_bGameOver == false)
		{
			//Particles Update for Aiming
			//if(m_pPM)
			m_pMiniMap->Update(fElapsedTime);
			//	m_pPM->Update( fElapsedTime );

			m_pOM->UpdateObjects(fElapsedTime);
			m_pOM->CheckCollisions();

			list<IEntity*> objList = m_pOM->GetObjectList();
			for(std::list<IEntity*>::iterator iter = objList.begin();
				iter != objList.end(); ++iter)
			{
				if((*iter)->GetType() == CEntity::OBJ_ENEMY)
				{
					CEnemy* pEnemy = dynamic_cast<CEnemy*>((*iter));
					pEnemy->CheckTileCollision();
				}
				else if((*iter)->GetType() == CEntity::OBJ_ALLY)
				{
					CAlly* pAlly = dynamic_cast<CAlly*>((*iter));
					pAlly->CheckTileCollision();
				}
			}

			CPlayer* pPlayer = dynamic_cast< CPlayer* >( m_pPlayer );
			pPlayer->CheckTileCollision();			

			m_pMS->ProcessMessages();
			CEventManager::GetInstance()->ProcessEvents();

			//TODO: ROB change to a function call from the tile manager
			CCamera::GetInstance()->Update(m_pPlayer->GetPosX(), m_pPlayer->GetPosY(), 
				m_pTileManager->GetWorldWidth(), m_pTileManager->GetWorldHeight());
		}
		else
		{
			if(m_pPlayer->GetHealth() <= 0)
				m_bGameOver = true;

			if(m_pHelicopter->GetHealth() <= 0)
				m_bLevelComplete = true;

			if(m_bLevelComplete == true)
			{
				if(m_nCurrLevel == 2)
					m_bGameOver = true;

				m_fLevelEndTimer += fElapsedTime;

				if(m_fLevelEndTimer >= 3.0f && m_bGameOver == false)
				{
					m_fLevelEndTimer = 0.0f;
					m_bLevelComplete = false;

					list<IEntity*> objList = m_pOM->GetObjectList();
					for(std::list<IEntity*>::iterator iter = objList.begin();
						iter != objList.end(); ++iter)
					{
						switch((*iter)->GetType())
						{
						case CEntity::OBJ_ALLY:
						case CEntity::OBJ_ENEMY:
						case CEntity::OBJ_BULLET:
						case CEntity::OBJ_BUNKER:
						case CEntity::OBJ_STOREHOUSE:
						case CEntity::OBJ_MINE:
						case CEntity::OBJ_SHOP:
							m_pOM->RemoveObject((*iter));
							break;
						};
					}
					m_pOM->RemoveObject(m_pHelicopter);
					m_pHelicopter->Release();
					m_pMiniMap->Release();
					m_pShop->Release();

					Enter();
					m_bChopExplode = false;

					//return;

					/*CGameStateManager::GetInstance()->ChangeState(CGamePlayState::GetInstance());
					return;*/
				}
			}

			if(m_pPlayer->GetHealth()<=0)
			{
				if(m_DeathSound == false)
				{
					if(m_bGameOver == true && m_DeathSound == false)
					{

						//	m_pXA->SFXPlaySound(m_nDeath);
						m_DeathSound = true;
						if(m_pXA->SFXIsSoundPlaying(m_nLostSFXID)==false)
							m_pXA->SFXPlaySound(m_nLostSFXID);
					}
				}
				else
				{
					if(m_pHelicopter->GetHealth() <= 0&& m_pXA->SFXIsSoundPlaying(m_nChopterSFXID)==false&&m_bChopExplode==false)
					{
						m_bLevelComplete = true;
						m_bChopExplode=true;
						m_pXA->SFXPlaySound(m_nChopterSFXID); 

						if(m_pXA->SFXIsSoundPlaying(m_nWinSFXID)==false)
						{

							if(m_bGameOver == true && m_pXA->SFXIsSoundPlaying(m_nWinSFXID)==false)
							{

								m_pXA->SFXPlaySound(m_nWinSFXID);
							}
						}
					}
				}
			}
			// Check to see if a High Score was achieved.
			if(CheckScores() == true)
				m_bGotHighScore = true;

			if(m_bGotHighScore == false && m_bGameOver == true)
			{
				m_fGameOverTimer += fElapsedTime;

				if(m_fGameOverTimer >= 3.0f)
				{
					CGameStateManager::GetInstance()->ChangeState(CCreditsState::GetInstance());
					return;
				}

			}
		}
	} 
	else
	{
		TutorialUpdate(fElapsedTime );
		list<IEntity*> objList = m_pOM->GetObjectList();
		for(std::list<IEntity*>::iterator iter = objList.begin();
			iter != objList.end(); ++iter)
		{
			if((*iter)->GetType() == CEntity::OBJ_ENEMY)
			{
				CEnemy* pEnemy = dynamic_cast<CEnemy*>((*iter));
				pEnemy->CheckTileCollision();
			}
			else if((*iter)->GetType() == CEntity::OBJ_ALLY)
			{
				CAlly* pAlly = dynamic_cast<CAlly*>((*iter));
				pAlly->CheckTileCollision();
			}
		}
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

void CGamePlayState::RenderLoading(void)
{
	// Clear the screen
	m_pD3D->Clear( 0, 0, 0 );

	// Start the D3D rendering
	m_pD3D->DeviceBegin();
	m_pD3D->SpriteBegin();

	// Render the current state
	m_pTM->Draw(m_nBackgroundImgID, 0, 0, 1.0f, 1.0f, nullptr);
	m_pTM->Draw(m_nLoadingLogoBgImgID, 0, CGame::GetInstance()->GetHeight()-m_pTM->GetTextureHeight(m_nLoadingLogoBgImgID), 1.0f, 1.0f, nullptr);
	m_pTM->Draw(m_nLoadingBulletImgID, (int)(m_nPercentageLoaded*5.12f - 512), (int)(CGame::GetInstance()->GetHeight()-m_pTM->GetTextureHeight(m_nLoadingBulletImgID)-20), 1.0f, 1.0f, nullptr);

	// End the D3D rendering (draw to the buffer)
	m_pD3D->SpriteEnd();
	m_pD3D->DeviceEnd();

	// Present the buffer to the screen
	m_pD3D->Present();
}

void CGamePlayState::Render(void)
{	

	//Draw the map		
	//TODO: if a camera pointer is ever added 
	//to the Gameplay State ask ROB to change this line
	m_pTileManager->Render(CCamera::GetInstance()->GetCameraRect());
	CGame* pGame = CGame::GetInstance();
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
	ostringstream output;

	//Render Aiming Particle
	//m_pPM->Render("1");
	//m_pPM->Render("2");
	//m_pPM->Render("3");
	//m_pPM->Render("4");

	float alphaTimer = 255 * pPlayer->GetHurtTimer();															//cjm - 10/06/12
	m_pTM->Draw(m_nHurtIndicator,0, 0, 0.8f, 0.6f, nullptr,0,0,0,D3DCOLOR_ARGB((int)alphaTimer, 255,255,255));	//cjm - 10/06/12



	//m_pPM->Render()
	m_pOM->RenderObjects();

	if(m_bLevelComplete == true)
	{
		output << "Level " << m_nCurrLevel << " Complete!";
		m_pFont->PrintString(output.str().c_str(), /*pGame->GetWidth()/2*/(int)pGame->GetWidth() /2 - ((int)(strlen(output.str().c_str()) * 12) /2), (int)pGame->GetHeight()/2 - 100, 1.0f, 255, 255, 255, 255);	//cjm - 10/11/12
		output.str("");
	}

	//if(m_bGameOver == true)
	if ( m_bTutotial == false )
	{
		//CAnimationManager::GetInstance()->Render(m_pAnimInfo, 200,200);

		if(m_bGameOver == true)
		{
			if(m_pPlayer->GetHealth() <= 0)
				// Player Died! Game Over!
				m_pFont->PrintString("YOU LOSE!!", /*pGame->GetWidth()/2*/(int)pGame->GetWidth() /2 - ((int)(strlen("YOU LOSE!!") * 12) /2), (int)pGame->GetHeight()/2, 1.0f, 255, 255, 255, 255);	//cjm - 10/11/12
			else if(m_pHelicopter->GetHealth() <= 0)
			{
				// Player Won!
				m_pFont->PrintString("YOU WIN!!", /*pGame->GetWidth()/2*/(int)pGame->GetWidth() /2 - ((int)(strlen("YOU WIN!!") * 24) /2), (int)pGame->GetHeight()/2, 1.0f, 255, 255, 255, 255);	//cjm - 10/11/12

				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
				output << "Total Kills: " << pPlayer->GetKills() << " X 100";
				m_pFont->PrintString(output.str().c_str(), pGame->GetWidth()/2 - 100, pGame->GetHeight()/2 + 30, 1.0f, 255, 255, 0, 0);
				output.str("");

				output << "Final Score: " << pPlayer->GetKills() * 100;
				m_pFont->PrintString(output.str().c_str(), pGame->GetWidth()/2 - 100, pGame->GetHeight()/2 + 60, 1.0f, 255, 0, 0, 255);
				output.str("");
			}

			if(m_bGotHighScore == true)
			{
				m_pFont->PrintString("Enter Your Initials", pGame->GetWidth()/2 - 100, pGame->GetHeight()/2 + 100, 1.0f, 255, 255, 255, 255);	//cjm - 10/11/12

				char* ch = (char*)(&m_nCurrId[0]);
				if(m_nCurrPos == 0)
					m_pFont->PrintString(ch, pGame->GetWidth()/2 - 75, pGame->GetHeight()/2 + 130, 1.0f, 255, 0, 0, 255);
				else
					m_pFont->PrintString(ch, pGame->GetWidth()/2 - 75, pGame->GetHeight()/2 + 130, 1.0f, 255, 0, 255, 0);

				ch = (char*)(&m_nCurrId[1]);
				if(m_nCurrPos == 1)
					m_pFont->PrintString(ch, pGame->GetWidth()/2 - 50, pGame->GetHeight()/2 + 130, 1.0f, 255, 0, 0, 255);
				else
					m_pFont->PrintString(ch, pGame->GetWidth()/2 - 50, pGame->GetHeight()/2 + 130, 1.0f, 255, 0, 255, 0);

				ch = (char*)(&m_nCurrId[2]);
				if(m_nCurrPos == 2)
					m_pFont->PrintString(ch, pGame->GetWidth()/2 - 25, pGame->GetHeight()/2 + 130, 1.0f, 255, 0, 0, 255);
				else
					m_pFont->PrintString(ch, pGame->GetWidth()/2 - 25, pGame->GetHeight()/2 + 130, 1.0f, 255, 0, 255, 0);
			}
		}
	}

	// Render the HUD
#pragma region Render HUD
	RECT rSource;
	//ostringstream output;

	// Draw Health Bar
#pragma region HealthBar
	// Health Percentage
	float hp = (float)pPlayer->GetHealth()/(float)pPlayer->GetMaxHealth();

	// Health Bar Background
	SetRect(&rSource, 26, 153, 226, 173);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 36, 45, 1.0f, 1.0f, &rSource);

	// Green Health Bar
	SetRect(&rSource, 26, 123, (int)(226 * hp), 143);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 36, 45, 1.0f, 1.0f, &rSource);

	// Health Bar Frame
	SetRect(&rSource, 24, 40, 226, 85);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 34, 22, 1.0f, 1.0f, &rSource);
#pragma endregion

	// Draw Health Pack Counter
#pragma region Health Pack Counter
	// Health Pack Counter
	SetRect(&rSource, 298, 65, 333, 101);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 36, 72, 1.0f, 1.0f, &rSource);

	// Health Pack Text
	output << pPlayer->GetHealthPacks();
	m_pFont->PrintString(output.str().c_str(), 75, 98, 1.25f, 255, 0, 0, 0);
	output.str("");
#pragma endregion

	// Draw Food Rate Counter
#pragma region Food Rate Counter
	// Food Rate Counter
	SetRect(&rSource, 363, 62, 402, 101);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 101, 69, 1.0f, 1.0f, &rSource);

	// Food Rate Text
	output << pPlayer->GetFoodRate();
	m_pFont->PrintString(output.str().c_str(), 144, 98, 1.25f, 255, 0, 0, 0);
	output.str("");
#pragma endregion

	// Draw Ore Rate Counter
#pragma region Ore Rate Counter
	// Ore Rate Counter
	SetRect(&rSource, 436, 65, 473, 101);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 174, 72, 1.0f, 1.0f, &rSource);

	// Ore Rate Text
	output << pPlayer->GetOreRate();
	m_pFont->PrintString(output.str().c_str(), 215, 98, 1.25f, 255, 0, 0, 0);
	output.str("");
#pragma endregion

	// Draw Status Bars at Top of Map
#pragma region Status Bars
	// Draw the Top Status Bar (CASH, FOOD, ORE)
	SetRect(&rSource, 559, 64, 905, 93);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 424, 19, 1.0f, 1.0f, &rSource);

	// Cash Text
	output << pPlayer->GetCash();
	m_pFont->PrintString(output.str().c_str(), 488, 23, 0.75f, 255, 0, 0, 0);
	output.str("");

	// Ore Text
	output << pPlayer->GetOre();
	m_pFont->PrintString(output.str().c_str(), 602, 23, 0.75f, 255, 0, 0, 0);
	output.str("");

	// Food Text
	output << pPlayer->GetFood();
	m_pFont->PrintString(output.str().c_str(), 715, 23, 0.75f, 255, 0, 0, 0);
	output.str("");

	// Draw the Bottom Status Bar (BUILDING COUNT)
	SetRect(&rSource, 559, 93, 905, 124);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 424, 48, 1.0f, 1.0f, &rSource);

	// Bunker Text
	output << pPlayer->GetBunkers();
	m_pFont->PrintString(output.str().c_str(), 503, 53, 0.75f, 255, 0, 0, 0);
	output.str("");

	// StoreHouse Text
	output << pPlayer->GetStoreHouses();
	m_pFont->PrintString(output.str().c_str(), 619, 53, 0.75f, 255, 0, 0, 0);
	output.str("");

	// Mine Text
	output << pPlayer->GetMines();
	m_pFont->PrintString(output.str().c_str(), 715, 53, 0.75f, 255, 0, 0, 0);
	output.str("");

#pragma endregion

	// Draw Weapon Image and Ammo Counter
#pragma region Weapons and Ammo
	// Ammo Frame
	SetRect(&rSource, 112, 874, 203, 927);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 117, 519, 1.0f, 1.0f, &rSource);

	if(pPlayer->GetWeaponType() != 0)
	{
		output << 30 - (int)pPlayer->GetPickupTimer();
		m_pFont->PrintString(output.str().c_str(), 50, 529, 1.0f, 255, 255, 0, 0);
		output.str("");
	}

	// Ammo Text
	output << pPlayer->GetAmmo();
	m_pFont->PrintString( output.str().c_str(), 155, 544, 0.75f, 255, 0, 0, 0 );
	output.str("");

	// Weapon Image
	switch(pPlayer->GetWeaponType())
	{
	case 0:	// Pistol
		SetRect(&rSource, 82, 557, 266, 624);
		break;

	case 1:	// Machine Gun
		SetRect(&rSource, 341, 651, 547, 724);
		break;

	case 2:	// Shotgun
		SetRect(&rSource, 82, 665, 266, 724);
		break;

	case 3:	// Sniper Rifle
		SetRect(&rSource, 355, 550, 557, 615);
		break;

	case 4:	// Rocket Launcher
		SetRect(&rSource, 584, 653, 791, 724);
		break;
	};
	m_pTM->Draw(pPlayer->GetHUDImgID(), 24, 447, 1.0f, 1.0f, &rSource);
#pragma endregion

	// Draw Status Bars at Bottom of the Map
	SetRect(&rSource, 281, 874, 631, 927);
	m_pTM->Draw(pPlayer->GetHUDImgID(), 226, 519, 1.0f, 1.0f, &rSource);

	// Draw Population
	output << pPlayer->GetPopulation();
	m_pFont->PrintString(output.str().c_str(), 402, 523, 0.6f, 255, 0, 0, 0);
	output.str("");

	// Draw MG Total
	output << pPlayer->GetMGs();
	m_pFont->PrintString(output.str().c_str(), 267, 545, 0.75f, 255, 0, 0, 0);
	output.str("");

	// Draw Rocketeer Total
	output << pPlayer->GetRockets();
	m_pFont->PrintString(output.str().c_str(), 368, 545, 0.75f, 255, 0, 0, 0);
	output.str("");

	// Draw Sniper Total
	output << pPlayer->GetSnipers();
	m_pFont->PrintString(output.str().c_str(), 455, 545, 0.75f, 255, 0, 0, 0);
	output.str("");

	// Draw Medic Total
	output << pPlayer->GetMedics();
	m_pFont->PrintString(output.str().c_str(), 542, 545, 0.75f, 255, 0, 0, 0);
	output.str("");

#pragma endregion

	// Draw MiniMap Background
	if( m_bTutotial == false )
	{
		SetRect(&rSource, 737, 757, 907, 927);
		m_pTM->Draw(pPlayer->GetHUDImgID(), 602, 402, 1.0f, 1.0f, &rSource);

		// Render the Minimap
		m_pMiniMap->Render();
	}

	if( m_bTutotial == true )
	{
		RenderTutorial();
	}

#ifdef _DEBUG
	m_pD3D->GetSprite()->Flush();
	list<IEntity*> objList = m_pOM->GetObjectList();
	for(std::list<IEntity*>::iterator iter = objList.begin();
		iter != objList.end(); ++iter)
	{
		m_pD3D->DrawRect((*iter)->GetRect(), 255, 0, 0);
	}
#endif

	if(m_bPaused == false)
	{
		// Render the mouse cursor

		m_pTM->Draw(m_nCursorPos, m_pDI->MouseGetPosX() + 16, m_pDI->MouseGetPosY() + 16);

	}

	
	

}

void CGamePlayState::MessageProc(IMessage* pMsg)
{
	CGamePlayState* self = CGamePlayState::GetInstance();
	CGame* pGame = CGame::GetInstance();


	switch(pMsg->GetMessageID())
	{
	case MSG_CREATE_ALLY:
		{
			CEntity* pAlly = self->m_pOM->CreateObject(_T("CAlly"));

			CCreateAllyMessage* pAllyMsg = dynamic_cast<CCreateAllyMessage* >(pMsg);

			CAlly* pObj = dynamic_cast<CAlly*>(pAlly);
			pObj->SetType(pAllyMsg->GetType());

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(self->m_pPlayer);

			switch( pObj->GetType() )
			{
			case ALLY_GUNNER:
				{
					pAlly->SetPosX( 140 );
					pAlly->SetPosY( 200 );
					pAlly->SetImageID(self->m_nAllyMGImgID);
					pAlly->SetHealth(100);
				//	pObj->SetDamage(10);
					pObj->SetAnimationID(7);
					pPlayer->IncreaseMGs(1);
				}
				break;
			case ALLY_MEDIC:
				{
					pAlly->SetPosX( 210 );
					pAlly->SetPosY( 200 );
					pAlly->SetImageID(self->m_nAllyMedicImgID);
					pAlly->SetHealth(100);
					pObj->SetDamage(10);
					pObj->SetAnimationID(6);
					pPlayer->IncreaseMedics(1);
				}
				break;
			case ALLY_ROCKET:
				{
					pAlly->SetPosX( 170 );
					pAlly->SetPosY( 400 );
					pAlly->SetImageID(self->m_nAllyRocketImgID);
					pAlly->SetHealth(100);
					pObj->SetDamage(10);
					pObj->SetAnimationID(9);
					pPlayer->IncreaseRockets(1);
				}
				break;
			case ALLY_SNIPER:
				{
					pAlly->SetPosX( 70 );
					pAlly->SetPosY( 200 );
					pAlly->SetImageID(self->m_nAllySniperImgID);
					pAlly->SetHealth(100);
					pObj->SetDamage(10);
					pObj->SetAnimationID(8);
					pPlayer->IncreaseSnipers(1);
				}
				break;
			}

			pPlayer->IncreasePopulation(1);
			pAlly->Release();
		}
		break;

	case MSG_CREATE_BUNKER:
		{
			CEntity* Bunker = self->m_pOM->CreateObject(_T("CBunker"));

			CCreateBunkerMessage* pBunkerMsg = dynamic_cast<CCreateBunkerMessage* >(pMsg);

			CBunker* pBunker = dynamic_cast<CBunker*>(Bunker);

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(self->m_pPlayer);

			pPlayer->IncreaseBunkers(1);

			pBunker->SetHeight(128);
			pBunker->SetWidth(128);
			pBunker->SetPosX( (float)pBunkerMsg->GetX() );
			pBunker->SetPosY( (float)pBunkerMsg->GetY() );
			pBunker->SetBunkerID(pBunkerMsg->GetID());
			pBunker->Release();

			if( self->m_nCurrLevel == 0 )
			{
				self->m_nNumBunkers++;
			}

		}
		break;

	case MSG_CREATE_STOREHOUSE:
		{
			CEntity* storehouse = self->m_pOM->CreateObject(_T("CStoreHouse"));

			CCreateStoreHouseMessage* pstorehouseMsg = dynamic_cast<CCreateStoreHouseMessage* >(pMsg);

			CStoreHouse* pStoreHouse = dynamic_cast<CStoreHouse*>(storehouse);

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(self->m_pPlayer);

			pPlayer->IncreaseStoreHouses(1);

			pStoreHouse->SetHeight(64);
			pStoreHouse->SetWidth(64);
			pStoreHouse->SetPosX( (float)pstorehouseMsg->GetX() );
			pStoreHouse->SetPosY( (float)pstorehouseMsg->GetY() );
			pStoreHouse->Release();

			if( self->m_nCurrLevel == 0 )
			{
				self->m_nNumStoreHouse++;
			}
		}
		break;

	case MSG_CREATE_MINE:
		{
			CEntity* Mine = self->m_pOM->CreateObject(_T("CMine"));

			CCreateMineMessage* pMineMsg = dynamic_cast<CCreateMineMessage* >(pMsg);

			CMine* pMine = dynamic_cast<CMine*>(Mine);

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(self->m_pPlayer);

			pPlayer->IncreaseMines(1);

			pMine->SetHeight(64);
			pMine->SetWidth( 64 );
			pMine->SetPosX( (float)pMineMsg->GetX() );
			pMine->SetPosY( (float)pMineMsg->GetY() );
			pMine->Release();

			if( self->m_nCurrLevel == 0 )
			{
				self->m_nNumMines++;
			}
		}
		break;

	case MSG_CREATE_BULLET:
		{

			//CEntity* pObj = self->m_pOM->CreateObject(_T("CBullet"));
			//pObj->SetImageID(self->m_nBulletImgID);


			//CCreateBulletMessage* pBulletMsg = dynamic_cast< CCreateBulletMessage* >( pMsg );

			//CBullet* pBullet = dynamic_cast<CBullet*>(pObj);
//----------------------------------------------------------------------------------------------------------------------------
			CCreateBulletMessage* pBulletMsg = dynamic_cast< CCreateBulletMessage* >( pMsg );		//corey
						
			CEntity* pObj = self->m_pOM->CreateObject(_T("CBullet"));
			CBullet* pBullet = dynamic_cast<CBullet*>(pObj);
			if (pBulletMsg->GetOwner()->GetType() == CEntity::OBJ_PLAYER)
			{
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(pBulletMsg->GetOwner());			
				if(pPlayer->GetWeaponType() == ROCKET)
					pBullet->SetImageID(self->m_nRocketImgID);
				else
					pBullet->SetImageID(self->m_nBulletImgID);
			}
			else if (pBulletMsg->GetOwner()->GetType() == CEntity::OBJ_ALLY)				
			{
				CAlly *pAlly = dynamic_cast<CAlly*>(pBulletMsg->GetOwner());
				if(pAlly->GetType() == ALLY_ROCKET)
					pBullet->SetImageID(self->m_nRocketImgID);
				else
					pBullet->SetImageID(self->m_nBulletImgID);
			}
			else if (pBulletMsg->GetOwner()->GetType() == CEntity::OBJ_ENEMY)				
			{
				CEnemy *pEnemy = dynamic_cast<CEnemy*>(pBulletMsg->GetOwner());
				if(pEnemy->GetUnitType() == eROCKET)
					pBullet->SetImageID(self->m_nRocketImgID);
				else
					pBullet->SetImageID(self->m_nBulletImgID);
			}
			else 
			pBullet->SetImageID(self->m_nBulletImgID);
//----------------------------------------------------------------------------------------------------------------------------

			//Rotating the bullet about the player center
			D3DXMATRIX rotation;
			D3DXMATRIX translate;
			D3DXMATRIX combined;

			// Initialize the Combined matrix.
			D3DXMatrixIdentity(&combined);

			// Translate the sprite
			D3DXMatrixTranslation(&translate, 15.f, -80.f, 0.0f);
			combined *= translate;

			D3DXMatrixRotationZ(&rotation, pBulletMsg->GetOwner()->GetRotation());
			combined *= rotation;

			pBullet->SetOwner( pBulletMsg->GetOwner() );
			pBullet->SetPosX( pBulletMsg->GetOwner()->GetPosX()  + combined._41);
			pBullet->SetPosY( pBulletMsg->GetOwner()->GetPosY()  + combined._42);
			pBullet->SetHeight(17);
			pBullet->SetWidth(7);
			pBullet->SetRotation( pBulletMsg->GetOwner()->GetRotation() );
			pBullet->SetDamage( pBulletMsg->GetOwner()->GetDamage() );		
			

			//creating a vector for the bullet's velocity
			tVector2D vBulletVelocity = { 0, -600 };
			vBulletVelocity = Vector2DRotate( vBulletVelocity, pBullet->GetRotation() );

			pBullet->SetVelX( vBulletVelocity.fX );
			pBullet->SetVelY( vBulletVelocity.fY );

			pObj->Release();
		}
		break;

	case MSG_CREATE_SHOTGUN_BULLET:
		{

			CEntity* pObj = self->m_pOM->CreateObject(_T("CBullet"));
			pObj->SetImageID(self->m_nBulletImgID);

			CCreateShotgunBullet* pBulletMsg1 = dynamic_cast< CCreateShotgunBullet* >( pMsg );

			CBullet* pBullets = dynamic_cast<CBullet*>(pObj);

			//Rotating the bullet about the player center
			D3DXMATRIX rotation;
			D3DXMATRIX translate;
			D3DXMATRIX combined;

			// Initialize the Combined matrix.
			D3DXMatrixIdentity(&combined);

			// Translate the sprite
			D3DXMatrixTranslation(&translate, 10.f, -80.f, 0.0f);
			combined *= translate;

			D3DXMatrixRotationZ(&rotation, pBulletMsg1->GetOwner()->GetRotation());
			combined *= rotation;

			pBullets->SetOwner( pBulletMsg1->GetOwner() );

			pBullets->SetPosX( pBulletMsg1->GetOwner()->GetPosX()  + combined._41 );
			pBullets->SetPosY( pBulletMsg1->GetOwner()->GetPosY()  + combined._42 );
			pBullets->SetHeight(17);
			pBullets->SetWidth(7);
			pBullets->SetRotation( pBulletMsg1->GetOwner()->GetRotation() );
			pBullets->SetDamage( pBulletMsg1->GetOwner()->GetDamage() );

			//creating a vector for the bullet's velocity
			tVector2D vBulletVelocity = { 0, -600 };
			vBulletVelocity = Vector2DRotate( vBulletVelocity, pBullets->GetRotation() );

			pBullets->SetVelX( vBulletVelocity.fX );
			pBullets->SetVelY( vBulletVelocity.fY );

			pObj->Release();
			
			CEntity* pObj2 = self->m_pOM->CreateObject(_T("CBullet"));
			pObj2->SetImageID(self->m_nBulletImgID);

			CBullet* pBullets2 = dynamic_cast<CBullet*>(pObj2);

			// Translate the sprite
			D3DXMatrixIdentity(&combined);

			D3DXMatrixTranslation(&translate, 18.f, -80.f, 0.0f);
			combined *= translate;

			D3DXMatrixRotationZ(&rotation, pBulletMsg1->GetOwner()->GetRotation());
			combined *= rotation;

			pBullets2->SetOwner( pBulletMsg1->GetOwner() );
			pBullets2->SetPosX( pBulletMsg1->GetOwner()->GetPosX()  + combined._41 );
			pBullets2->SetPosY( pBulletMsg1->GetOwner()->GetPosY()  + combined._42 );
			pBullets2->SetHeight(17);
			pBullets2->SetWidth(7);
			pBullets2->SetRotation( pBulletMsg1->GetOwner()->GetRotation() );
			pBullets2->SetDamage( pBulletMsg1->GetOwner()->GetDamage() );

			//creating a vector for the bullet's velocity
			tVector2D vBulletVelocity2 = { 0, -600 };
			vBulletVelocity2 = Vector2DRotate( vBulletVelocity2, pBullets->GetRotation() );

			pBullets2->SetVelX( vBulletVelocity2.fX );
			pBullets2->SetVelY( vBulletVelocity2.fY );

			pObj2->Release();
		}
		break;

	case MSG_CREATE_ENEMY:
		{
			CEntity* pObj = self->m_pOM->CreateObject(_T("CEnemy"));

			CEnemy* pEnemy = dynamic_cast<CEnemy*>(pObj);

			pEnemy->SetHeight(64);
			pEnemy->SetWidth(64);

			pEnemy->SetMovementXDirection(rand()%3 - 1);
			pEnemy->SetMovementYDirection(rand()%3 - 1);

			int type = rand() % 4;
			pEnemy->SetUnitType(type);
			pEnemy->SetTarget(nullptr);//self->m_pPlayer);

			if ( self->m_bTutotial == false )
			{
				pEnemy->SetPosX(self->m_pHelicopter->GetPosX());
				pEnemy->SetPosY(self->m_pHelicopter->GetPosY());
			}
			else
			{
				pEnemy->SetPosX( self->m_pPlayer->GetPosX() + 500 );
				pEnemy->SetPosY( self->m_pPlayer->GetPosY() - 50 );
			}

			switch(pEnemy->GetUnitType())
			{
			case 0:	// Machine Gunner
				{
					pEnemy->SetDamage(10 * self->m_nCurrLevel);
					pEnemy->SetImageID(self->m_nAllyMGImgID);
					pObj->SetAnimationID(11);
				}
				break;

			case 1:	// Sniper
				{
					pEnemy->SetDamage(10 * self->m_nCurrLevel);
					pEnemy->SetImageID(self->m_nEnemySniperImgID);
					pObj->SetAnimationID(12);
				}
				break;

			case 2:	// Medic
				{
					pEnemy->SetDamage(10 * self->m_nCurrLevel);
					pEnemy->SetImageID(self->m_nAllyMedicImgID);
					pObj->SetAnimationID(10);
				}
				break;

			case 3:	// Rocket
				{
					pEnemy->SetDamage(10 * self->m_nCurrLevel);
					pEnemy->SetImageID(self->m_nAllyRocketImgID);
					pObj->SetAnimationID(13);

				}
				break;
			}

			pEnemy->Release();
		}
		break;

	case MSG_CREATE_PICKUP:
		{
			CEntity* pPickUp = self->m_pOM->CreateObject(_T("CPickUp"));
			CCreatePickUpMessage* pPickUpMsg = dynamic_cast<CCreatePickUpMessage*>(pMsg);
			CPickUp* pickUp = dynamic_cast<CPickUp*>(pPickUp);

			pPickUp->SetHeight(32);
			pPickUp->SetWidth(32);

			pPickUp->SetPosX(pPickUpMsg->GetOwner()->GetPosX());
			pPickUp->SetPosY(pPickUpMsg->GetOwner()->GetPosY());
			switch(pickUp->GetPickUpType())
			{
			case 0:
			case 14:
			case 6:
			case 7:
			case 8:
				{
					pickUp->SetImageID(self->m_nPickUpAmmoImgID);
				}
				break;

			case 1:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
				{
					pickUp->SetImageID(self->m_nPickUpCashImgID);
				}
				break;

			case 2:
				{
					pickUp->SetImageID(self->m_nPickUpMGImgID);
				}
				break;

			case 3:
				{
					pickUp->SetImageID(self->m_nPickUpShotgunImgID);
				}
				break;

			case 4:
				{
					pickUp->SetImageID(self->m_nPickUpSniperImgID);
				}
				break;

			case 5:
				{
					pickUp->SetImageID(self->m_nPickUpRocketImgID);
				}
				break;
			};

			pPickUp->Release();

		}
		break;

	case MSG_DESTROY_ALLY:
		{
			CDestroyAllyMessage* pDestroyMsg = dynamic_cast<CDestroyAllyMessage*>(pMsg);

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(self->m_pPlayer);

			switch(pDestroyMsg->GetAlly()->GetType())
			{
			case 0:	// Machine Gunner
				{
					pPlayer->DecreaseMGs(1);
				}
				break;

			case 1:	// Sniper
				{
					pPlayer->DecreaseMedics(1);
				}
				break;

			case 2:	// Medic
				{
					pPlayer->DecreaseRockets(1);
				}
				break;

			case 3:	// Rocket
				{
					pPlayer->DecreaseSnipers(1);
				}
				break;
			}

			pPlayer->DecreasePopulation(1);
			self->m_pOM->RemoveObject(pDestroyMsg->GetAlly());
		}
		break;

	case MSG_DESTROY_BUNKER:
		{
			CDestroyBunkerMessage* pDestroyMsg = dynamic_cast<CDestroyBunkerMessage*>(pMsg);

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(self->m_pPlayer);
			pPlayer->DecreaseBunkers(1);

			self->m_pOM->RemoveObject(pDestroyMsg->GetBunker());
		}
		break;
	case MSG_DESTROY_STOREHOUSE:
		{
			CDestroyStoreHouseMessage* pDestroyMsg = dynamic_cast<CDestroyStoreHouseMessage*>(pMsg);

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(self->m_pPlayer);
			pPlayer->DecreaseStoreHouses(1);

			self->m_pOM->RemoveObject(pDestroyMsg->GetStoreHouse());
		}
		break;
	case MSG_DESTROY_MINE:
		{
			CDestroyMineMessage* pDestroyMsg = dynamic_cast<CDestroyMineMessage*>(pMsg);

			CPlayer* pPlayer = dynamic_cast<CPlayer*>(self->m_pPlayer);
			pPlayer->DecreaseMines(1);

			self->m_pOM->RemoveObject(pDestroyMsg->GetMine());
		}
		break;

	case MSG_DESTROY_BULLET:
		{
			CDestroyBulletMessage* pDestroyMessage = dynamic_cast< CDestroyBulletMessage*>( pMsg );
			assert( pDestroyMessage != nullptr );

			CBullet* pBullet = pDestroyMessage->GetBullet();

			self->m_pOM->RemoveObject( pBullet ); 
		}
		break;

	case MSG_DESTROY_ENEMY:
		{
			CDestroyEnemyMessage* pDestroyMsg = dynamic_cast<CDestroyEnemyMessage*>(pMsg);
			//if(self->m_pXA->SFXIsSoundPlaying(self->m_nEnemyDeathSFXID)==false)
			self->m_pXA->SFXPlaySound(self->m_nEnemyDeathSFXID);
			self->m_pOM->RemoveObject(pDestroyMsg->GetEnemy());

			self->m_pEM->sendEvent("get_kills");

			if( self->m_bTutotial == true )
				self->m_nNumEnemies--;
		}
		break;

	case MSG_DESTROY_PICKUP:
		{
			CDestroyPickUpMessage* pDestroyMsg = dynamic_cast<CDestroyPickUpMessage*>(pMsg);
			self->m_pOM->RemoveObject(pDestroyMsg->GetPickUp());

			if( self->m_bTutotial == true )
				self->m_nNumPickups--;
		}
		break;
	};
}

CGamePlayState::~CGamePlayState(void)
{
}

void CGamePlayState::HandleEvent( IEvent* pEvent )
{
	if( pEvent->GetEventID() == "enter_shop" )
	{
		//change to the shop state
		CGameStateManager::GetInstance()->NextState( CShopState::GetInstance() );
		return;
	}
	if( pEvent->GetEventID() == "leave_shop" )
	{
		m_pXA->SFXPlaySound(m_nExitStoreSFXID);
	}
	if( pEvent->GetEventID() == "collide_Rocket" )
	{
		if(m_pXA->SFXIsSoundPlaying(m_nRocketCollidedSFXID)==false)
		m_pXA->SFXPlaySound(m_nRocketCollidedSFXID);
	}
	if( pEvent->GetEventID() == "collide_Bullet" )
	{
		if(m_pXA->SFXIsSoundPlaying(m_nBulletCollidedSFXID)==false)
		m_pXA->SFXPlaySound(m_nBulletCollidedSFXID);
	}


}

bool CGamePlayState::CheckScores()
{
	if(LoadScores(m_vScores, "highscores.xml") == false)
		return false;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);

	m_nScorePos = 0;

	vector<tScores>::const_iterator iter;
	for(iter = m_vScores.begin(); iter != m_vScores.end(); ++iter)
	{
		if((pPlayer->GetKills() * 100) > (*iter).nScore)
		{
			return true;
		}
		++m_nScorePos;
	}
	return false;
}

bool CGamePlayState::LoadScores(vector<tScores>& vScores, const char* szFileName)
{
	// Create a TinyXML Document
	TiXmlDocument doc;

	// Attempt to load the file
	if(doc.LoadFile(szFileName) == false)
		return false;

	// Access the root node ("scores_list")
	TiXmlElement* pRoot = doc.RootElement();
	if(pRoot == nullptr)
		return false;

	// Clear the existing scores (if any)
	vScores.clear();

	// Iterate through the nodes to load scores
	TiXmlElement* pScore = pRoot->FirstChildElement("score_info");

	while(pScore != nullptr)
	{
		// Score Struct
		tScores info = { };

		// Read the Name
		const char* pText = pScore->GetText();
		if(pText != nullptr)
			strcpy_s(info.chName, 32, pText);
		else
			strcpy_s(info.chName, 32, "EMPTY");

		// Read the Score
		pScore->Attribute("score", &info.nScore);

		// Save the info into the vector
		vScores.push_back(info);

		// Move to the next "score_info" node
		pScore = pScore->NextSiblingElement("score_info");
	}

	return true;
}

void CGamePlayState::SaveScores(const vector<tScores>& vScores, const char* szFileName)
{
	// Create a TinyXML Document
	TiXmlDocument doc;

	// Create the TinyXML declaration (version info)
	TiXmlDeclaration* pDec = new TiXmlDeclaration("1.0", "utf-8", "");

	// Write the declaration to the end of the document
	doc.LinkEndChild(pDec);

	// Create a root node for the "players_list"
	TiXmlElement* pRoot = new TiXmlElement("scores_list");

	// Add the node to the document
	doc.LinkEndChild(pRoot);

	// Iterate through the vector,
	// saving each score's info
	vector<tScores>::const_iterator iter;
	for(iter = vScores.begin(); iter != vScores.end(); ++iter)
	{
		// Create a node for this score
		TiXmlElement* pScore = new TiXmlElement("score_info");

		// Fill the node with score data
		pScore->SetAttribute("score", (*iter).nScore);

		// Create a Text node for the name
		TiXmlText* pText = new TiXmlText((*iter).chName);
		pScore->LinkEndChild(pText);

		// Write the score node to the root "scores_list"
		pRoot->LinkEndChild(pScore);
	}

	// Save the document to the file
	doc.SaveFile(szFileName);
}

bool CGamePlayState::LoadGame(const char* szFileName)
{
	// Create the doc
	TiXmlDocument doc;

	if(doc.LoadFile(szFileName) == false)
		return false;

	TiXmlElement* pRoot = doc.RootElement();

	if(pRoot == nullptr)
		return false;

	InitAnimations();

	// Load Level
	TiXmlElement* pLevel = pRoot->FirstChildElement("level_info");
	pLevel->Attribute("current_level", &m_nCurrLevel);

	switch(m_nCurrLevel)
	{
	case 1:
		StartDefaultLv1();
		m_bTutotial = false;
		break;

	case 2:
		StartLevel2();
		m_bTutotial = false;
		break;
	};

	// Load Player
	TiXmlElement* pPlayer = pRoot->FirstChildElement("player_info");

	while(pPlayer != nullptr)
	{
		// Create A Player
		m_pPlayer = m_pOM->CreateObject(_T("CPlayer"));
		m_pPlayer->SetImageID(m_pTM->LoadTexture( _T("resource/graphics/Barney.png"), D3DCOLOR_XRGB(255,255,255)));
		// Set Position
		double x, y;
		pPlayer->Attribute("pos_x", &x);
		m_pPlayer->SetPosX((float)x);
		pPlayer->Attribute("pos_y", &y);
		m_pPlayer->SetPosY((float)y);

		// Set Health
		int hp;
		pPlayer->Attribute("health", &hp);
		m_pPlayer->SetHealth(hp);

		// Set PickUpTime
		CPlayer* obj = dynamic_cast<CPlayer*>(m_pPlayer);
		double PickUpTimer;
		pPlayer->Attribute("pickup_timer", &PickUpTimer);
		obj->SetPickUpTimer((float)PickUpTimer);

		// Set Current Weapon
		int weapon;
		pPlayer->Attribute("curr_weapon", &weapon);
		obj->SetWeaponType(weapon);

		// Set Food
		double food;
		pPlayer->Attribute("food_total", &food);
		obj->SetFood((float)food);

		// Set Fire Rate
		double fireRate;
		pPlayer->Attribute("fire_rate", &fireRate);
		obj->SetFireRate((float)fireRate);

		// Set Ore Rate
		int oreRate;
		pPlayer->Attribute("ore_rate", &oreRate);
		obj->SetOreRate(oreRate);

		// Set Food Rate
		int foodRate;
		pPlayer->Attribute("food_rate", &foodRate);
		obj->SetFoodRate(foodRate);

		// Set Damage
		int damage;
		pPlayer->Attribute("damage", &damage);
		obj->SetDamage(damage);

		// Set Ammo
		int ammo;
		pPlayer->Attribute("ammo", &ammo);
		obj->SetAmmo(ammo);

		// Set Cash
		int cash;
		pPlayer->Attribute("cash", &cash);
		obj->SetCash(cash);

		// Set Ore
		int ore;
		pPlayer->Attribute("ore", &ore);
		obj->SetOre(ore);

		// Set Healthpacks
		int healthpacks;
		pPlayer->Attribute("health_packs", &healthpacks);
		obj->SetHealthPacks(healthpacks);

		// Set Max Health
		int maxhealth;
		pPlayer->Attribute("max_health", &maxhealth);
		obj->SetMaxHealth(maxhealth);

		// Set Armor
		int armor;
		pPlayer->Attribute("armor", &armor);
		obj->SetArmor(armor);

		// Set Kills
		int kills;
		pPlayer->Attribute("kills", &kills);
		obj->SetKills(kills);

		switch(weapon)
		{
		case 0:
			obj->SetAnimationID(0);
			obj->SetLoadFireRate(0.05f);
			break;

		case 1:
			obj->SetAnimationID(4);
			obj->SetLoadFireRate(0.05f);
			break;

		case 2:
			obj->SetAnimationID(1);
			obj->SetLoadFireRate(1.5f);
			break;

		case 3:
			obj->SetAnimationID(2);
			obj->SetLoadFireRate(1.75f);
			break;

		case 4:
			obj->SetAnimationID(3);
			obj->SetLoadFireRate(2.5f);
			break;
		};

		pPlayer = pPlayer->NextSiblingElement("player_info");
	}

	// Load Allies
	TiXmlElement* pAlly = pRoot->FirstChildElement("ally_info");

	while(pAlly != nullptr)
	{
		CEntity* obj = m_pOM->CreateObject(_T("CAlly"));
		// Set Position
		double x,y;
		pAlly->Attribute("pos_x", &x);
		pAlly->Attribute("pos_y", &y);
		obj->SetPosX((float)x);
		obj->SetPosY((float)y);

		// Set Health
		int hp;
		pAlly->Attribute("health", &hp);
		obj->SetHealth(hp);

		// Set Type
		int UnitType;
		pAlly->Attribute("type", &UnitType);
		CAlly* pObj = dynamic_cast<CAlly*>(obj);
		pObj->SetType(UnitType);

		switch(UnitType)
		{
		case ALLY_GUNNER:
			pObj->SetImageID(m_nAllyMGImgID);
			pObj->SetAnimationID(7);
			break;

		case ALLY_MEDIC:
			pObj->SetImageID(m_nAllyMedicImgID);
			pObj->SetAnimationID(6);
			break;

		case ALLY_ROCKET:
			pObj->SetImageID(m_nAllyRocketImgID);
			pObj->SetAnimationID(9);
			break;

		case ALLY_SNIPER:
			pObj->SetImageID(m_nAllySniperImgID);
			pObj->SetAnimationID(8);
			break;
		};

		obj->Release();
		pAlly = pAlly->NextSiblingElement("ally_info");
	}

	// Load Helicopter
	TiXmlElement* pHelicopter = pRoot->FirstChildElement("helicopter_info");

	while(pHelicopter != nullptr)
	{
		m_pHelicopter = m_pOM->CreateObject(_T("CHelicopter"));
		//m_pHelicopter->SetImageID(m_pTM->LoadTexture(_T("resource/graphics/Blast_Helicopter.png"), D3DCOLOR_XRGB(0,0,0)));

		CHelicopter* obj = dynamic_cast<CHelicopter*>(m_pHelicopter);
		// Set Position
		double x,y;
		pHelicopter->Attribute("pos_x", &x);
		pHelicopter->Attribute("pos_y", &y);
		obj->SetPosX((float)x);
		obj->SetPosY((float)y);

		// Set Health
		int hp;
		pHelicopter->Attribute("health", &hp);
		obj->SetHealth(hp);

		// Set Bools
		int pf, otg, isfly, isspawn, island;
		pHelicopter->Attribute("player_found", &pf);
		pHelicopter->Attribute("on_the_ground", &otg);
		pHelicopter->Attribute("is_flying", &isfly);
		pHelicopter->Attribute("is_spawning", &isspawn);
		pHelicopter->Attribute("is_landing", &island);

		if(pf == 1)
			obj->SetPlayerFound(true);
		else
			obj->SetPlayerFound(false);

		if(otg == 1)
			obj->SetOnTheGround(true);
		else
			obj->SetOnTheGround(false);

		if(isfly == 1)
			obj->SetIsFlying(true);
		else
			obj->SetIsFlying(false);

		if(isspawn == 1)
			obj->SetIsSpawning(true);
		else
			obj->SetIsSpawning(false);

		if(island == 1)
			obj->SetIsLanding(true);
		else
			obj->SetIsLanding(false);

		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
		m_pHelicopter->SetHeight(194);
		m_pHelicopter->SetWidth(256);
		m_pHelicopter->SetImageID(m_pTM->LoadTexture(_T("resource/graphics/Blast_Helicopter.png"), D3DCOLOR_XRGB(0,0,0)));

		CHelicopter* pHeli = dynamic_cast<CHelicopter*>(m_pHelicopter);
		pHeli->SetPlayerPointer(pPlayer);

		pHelicopter = pHelicopter->NextSiblingElement("helicopter_info");
	}

	// Load Enemies
	TiXmlElement* pEnemy = pRoot->FirstChildElement("enemy_info");

	while(pEnemy != nullptr)
	{
		CEntity* obj = m_pOM->CreateObject(_T("CEnemy"));
		// Set Position
		double x,y;
		pEnemy->Attribute("pos_x", &x);
		pEnemy->Attribute("pos_y", &y);
		obj->SetPosX((float)x);
		obj->SetPosY((float)y);

		// Set Health
		int hp;
		pEnemy->Attribute("health", &hp);
		obj->SetHealth(hp);

		// Set Type
		int UnitType;
		pEnemy->Attribute("type", &UnitType);
		CEnemy* pObj = dynamic_cast<CEnemy*>(obj);
		pObj->SetUnitType(UnitType);

		switch(pObj->GetUnitType())
			{
			case 0:	// Machine Gunner
				{
					pObj->SetDamage(10);
					pObj->SetImageID(m_nAllyMGImgID);
					pObj->SetAnimationID(11);
				}
				break;

			case 1:	// Sniper
				{
					pObj->SetDamage(10);
					pObj->SetImageID(m_nEnemySniperImgID);
					pObj->SetAnimationID(12);
				}
				break;

			case 2:	// Medic
				{
					pObj->SetDamage(10);
					pObj->SetImageID(m_nAllyMedicImgID);
					pObj->SetAnimationID(10);
				}
				break;

			case 3:	// Rocket
				{
					pObj->SetDamage(10);
					pObj->SetImageID(m_nAllyRocketImgID);
					pObj->SetAnimationID(13);
				}
				break;
			}

		//pObj->SetImageID(m_nEnemyImgID);
		obj->Release();

		pEnemy = pEnemy->NextSiblingElement("enemy_info");
	}

	// Load Bunkers
	TiXmlElement* pBunker = pRoot->FirstChildElement("bunker_info");

	while(pBunker != nullptr)
	{
		CEntity* obj = m_pOM->CreateObject(_T("CBunker"));

		// Set Position
		double x,y;
		pBunker->Attribute("pos_x", &x);
		pBunker->Attribute("pos_y", &y);
		obj->SetPosX((float)x);
		obj->SetPosY((float)y);

		// Set Health
		int hp;
		pBunker->Attribute("health", &hp);
		obj->SetHealth(hp);

		// Set Curr Units In Bunker

		obj->Release();

		pBunker = pBunker->NextSiblingElement("bunker_info");
	}

	// Load Storehouses
	TiXmlElement* pStorehouse = pRoot->FirstChildElement("storehouse_info");

	while(pStorehouse != nullptr)
	{
		CEntity* obj = m_pOM->CreateObject(_T("CStoreHouse"));

		// Set Position
		double x,y;
		pStorehouse->Attribute("pos_x", &x);
		pStorehouse->Attribute("pos_y", &y);
		obj->SetPosX((float)x);
		obj->SetPosY((float)y);

		// Set Health
		int hp;
		pStorehouse->Attribute("health", &hp);
		obj->SetHealth(hp);

		obj->Release();

		pStorehouse = pStorehouse->NextSiblingElement("storehouse_info");
	}

	// Load Mines
	TiXmlElement* pMine = pRoot->FirstChildElement("mine_info");

	while(pMine != nullptr)
	{
		CEntity* obj = m_pOM->CreateObject(_T("CMine"));

		// Set Position
		double x,y;
		pMine->Attribute("pos_x", &x);
		pMine->Attribute("pos_y", &y);
		obj->SetPosX((float)x);
		obj->SetPosY((float)y);

		// Set Health
		int hp;
		pMine->Attribute("health", &hp);
		obj->SetHealth(hp);

		obj->Release();

		pMine = pMine->NextSiblingElement("mine_info");
	}

	CFileManagementState::GetInstance()->SetFileName("");

	return true;
}

void CGamePlayState::StartDefaultLv1(void)
{
	m_pTileManager->LoadTiles("Level01.xml");			//findme		load tiles "Tutorial.xml");				//

	if(CMainMenuState::GetInstance()->GetLoadingGame() == false)
	{
		InitPlayer();
		InitHelicopter();

		//delete this when shop is implemented
#pragma region ALLIES
	//	CCreateAllyMessage* pAlly1 = new CCreateAllyMessage( ALLY_GUNNER );
	//	m_pMS->SendMsg( pAlly1 );
	//	pAlly1 = nullptr;

	//	CCreateAllyMessage* pAlly2 = new CCreateAllyMessage( ALLY_MEDIC );
	//	m_pMS->SendMsg( pAlly2 );
	//	pAlly2 = nullptr;

	//	CCreateAllyMessage* pAlly3 = new CCreateAllyMessage( ALLY_ROCKET );
	//	m_pMS->SendMsg( pAlly3 );
	//	pAlly3 = nullptr;

	//	CCreateAllyMessage* pAlly4 = new CCreateAllyMessage( ALLY_SNIPER );
	//	m_pMS->SendMsg( pAlly4 );
	//	pAlly4 = nullptr;
	
#pragma endregion
	}
	else
	{
		InitPlayer();
		InitHelicopter();
	}
	//Setting up the Shop
	m_pShop = m_pOM->CreateObject( _T("CShop"));
	m_pShop->SetPosX( 384 );
	m_pShop->SetPosY( 3456 );
	m_pShop->SetHeight(192);
	m_pShop->SetWidth(384);

	//InitPlayer();
}

void CGamePlayState::StartLevel2(void)
{
	m_pTileManager->LoadTiles("Level02.xml");			//findme		load tiles "Tutorial.xml");				//

	if(CMainMenuState::GetInstance()->GetLoadingGame() == false)
	{
		//InitPlayer();
		InitHelicopter();
		m_pPlayer->SetPosX( 400.0f );
		m_pPlayer->SetPosY( 3456.0f );
	}
	else
	{
		InitPlayer();
		InitHelicopter();
	}

	//Setting up the Shop
	m_pShop = m_pOM->CreateObject( _T("CShop"));
	m_pShop->SetPosX( 384 );
	m_pShop->SetPosY( 3456 );
	m_pShop->SetHeight(192);
	m_pShop->SetWidth(384);
}

void CGamePlayState::InitAnimations(void)
{
	CAnimationManager::GetInstance()->LoadAnimationFile("Player_Moving_Pistol.xml");		//0
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_player_shotgun.xml");		//1	
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_player_sniper.xml");		//2
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_player_rocketeer.xml");		//3
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_player_machinegunner.xml");	//4
	CAnimationManager::GetInstance()->LoadAnimationFile("Helicopter.xml");					//5
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_Medic_ally.xml");			//6	
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_MG_ally.xml");				//7	
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_sniper_green.xml");			//8
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_rocket_green.xml");			//9
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_medic_red.xml");			//10
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_MG_red.xml");				//11
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_sniper_red.xml");			//12
	CAnimationManager::GetInstance()->LoadAnimationFile("sprite_rocket_red.xml");			//13
}

void CGamePlayState::InitImages(void)
{
	//m_nEnemyMedicImgID		= m_pTM->LoadTexture(_T("resource/graphics/sprites_player2.png"), D3DCOLOR_XRGB(255,255,255));
	//m_nEnemyMGImgID			= m_pTM->LoadTexture(_T("resource/graphics/sprites_player2.png"), D3DCOLOR_XRGB(255,255,255));	
	//m_nEnemyRocketImgID		= m_pTM->LoadTexture(_T("resource/graphics/sprites_player2.png"), D3DCOLOR_XRGB(255,255,255));
	m_nEnemySniperImgID			= m_pTM->LoadTexture(_T("resource/graphics/sprite_red_sniper.png")/*, D3DCOLOR_XRGB(255,255,255)*/);

	m_nHelicopterImgID	= -1;
	m_nAllyMedicImgID	= m_pTM->LoadTexture(_T("resource/graphics/sprites_npc_medic.png"), D3DCOLOR_XRGB(255,255,255));
	m_nAllyMGImgID		= m_pTM->LoadTexture(_T("resource/graphics/sprites_npc_MG.png"), D3DCOLOR_XRGB(255,255,255));
	m_nAllySniperImgID	= m_pTM->LoadTexture(_T("resource/graphics/sprites_npc_sniper.png"), D3DCOLOR_XRGB(255,255,255));
	m_nAllyRocketImgID	= m_pTM->LoadTexture(_T("resource/graphics/sprites_npc_rocket.png"), D3DCOLOR_XRGB(255,255,255));
	m_nHurtIndicator	= m_pTM->LoadTexture(_T("resource/graphics/hurtIndicator800x600.png"), D3DCOLOR_XRGB (255, 255, 255));	//cjm - 10/06/12

	m_nPercentageLoaded += 10;
	RenderLoading();
	// Load Pick Up Images
	m_nPickUpAmmoImgID		= m_pTM->LoadTexture(_T("resource/graphics/Bullets.png"));		//cjm - 10/08/12
	m_nPickUpCashImgID		= m_pTM->LoadTexture(_T("resource/graphics/cashBag.png"));		//cjm - 10/08/12
	m_nPickUpRocketImgID	= m_pTM->LoadTexture(_T("resource/graphics/crate.png"));		//cjm - 10/08/12
	m_nPickUpMGImgID		= m_pTM->LoadTexture(_T("resource/graphics/crate.png"));		//cjm - 10/08/12
	m_nPickUpShotgunImgID	= m_pTM->LoadTexture(_T("resource/graphics/crate.png"));		//cjm - 10/08/12
	m_nPickUpSniperImgID	= m_pTM->LoadTexture(_T("resource/graphics/crate.png"));		//cjm - 10/08/12
	m_nBulletImgID			= m_pTM->LoadTexture( _T("resource/graphics/sprites_player2.png"), D3DCOLOR_XRGB(255,255,255));
	m_nRocketImgID			= m_pTM->LoadTexture( _T("resource/graphics/RocketBullet.png"), D3DCOLOR_XRGB(255,255,255));

	m_nPercentageLoaded += 10;
	RenderLoading();

	// Load Bitmap Image
	m_nBitmapFontImgID		= m_pTM->LoadTexture(_T("resource/graphics/SansationFont_0.png"), D3DCOLOR_XRGB(0,0,0));
	m_pFont->SetImageID(m_nBitmapFontImgID);
}

void CGamePlayState::InitAudio(void)
{
	//m_nBackgroundMusID		= m_pXA->MusicLoadSong(_T("resource/sounds/MUS_GameBackgroundMain.xwm"));

	m_nBackgroundMusID[0]	= m_pXA->MusicLoadSong(_T("resource/sounds/MUS_HitTheFloor.xwm"));
	m_nBackgroundMusID[1]	= m_pXA->MusicLoadSong(_T("resource/sounds/MUS_ByMyself.xwm"));
	m_nBackgroundMusID[2]	= m_pXA->MusicLoadSong(_T("resource/sounds/MUS_PlaceForMyHead.xwm"));
	m_nBackgroundMusID[3]	= m_pXA->MusicLoadSong(_T("resource/sounds/MUS_NewDivide.xwm"));
	m_nBackgroundMusID[4]	= m_pXA->MusicLoadSong(_T("resource/sounds/MUS_OneStepCloser.xwm"));

	m_nDeath				= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_DEATH.wav"));
	m_nPauseSFXID			= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_PAUSE.wav"));
	m_nShopEnterSFXID		= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_ENTERSTORE.wav"));
	m_nLostSFXID			= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_LOSEB.wav"));
	m_nWinSFXID				= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_WINB.wav"));
	m_nChopterSFXID			= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_ROCKETCOLLIDE.wav"));
	m_nEnemyDeathSFXID		= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_DEATH.wav"));
	m_nExitStoreSFXID		= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_EXITSTORE.wav"));
	m_bChopExplode			= false;
	m_nBulletCollidedSFXID	= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_BULLETCOLLIDE_OTHER.wav"));
	m_nRocketCollidedSFXID	= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_ROCKETCOLLIDE_OTHER.wav"));
}

void CGamePlayState::InitPlayer(void)
{
	//Setting up the player
	m_pPlayer = m_pOM->CreateObject(_T("CPlayer"));
	m_pPlayer->SetPosX( 400.0f );
	m_pPlayer->SetPosY( 3456.0f );
	m_pPlayer->SetHeight(64);
	m_pPlayer->SetWidth(64);

	m_pPlayer->SetImageID(m_pTM->LoadTexture( _T("resource/graphics/Barney.png"), D3DCOLOR_XRGB(255,255,255)));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
	pPlayer->SetMaxHealthPacks(3);
	pPlayer->SetHealthPacks(1);

	if( m_bTutotial )
	{
		pPlayer->SetOre(500);
		pPlayer->SetFood( 500 );
		pPlayer->SetOreRate (200 );
		pPlayer->SetFoodRate( 200 );
		pPlayer->SetCash( 10000 );
		m_pPlayer->SetPosX( 300.0f );
		m_pPlayer->SetPosY( 300.0f );
	}
	//Creating Particles for the player
	//pPlayer->SetHealingParticleEffect(m_pPM->GetInstance()->CreateEmitter("1"));
}

void CGamePlayState::InitHelicopter(void)
{
	// Set up the Helicopter
	m_pHelicopter = m_pOM->CreateObject(_T("CHelicopter"));
	m_pHelicopter->SetPosX(800.0f);
	m_pHelicopter->SetPosY(800.0f);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
	m_pHelicopter->SetHeight(194);
	m_pHelicopter->SetWidth(256);
	m_pHelicopter->SetImageID(m_pTM->LoadTexture(_T("resource/graphics/Blast_Helicopter.png"), D3DCOLOR_XRGB(0,0,0)));
	
	CHelicopter* pHeli = dynamic_cast<CHelicopter*>(m_pHelicopter);
	pHeli->SetPlayerPointer(pPlayer);
}

void CGamePlayState::RenderTutorial()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
	RECT rSource;

	if (m_bTeacherMode == true )
	{
		switch( m_nCurrentLesson )
		{
		case 0:
			{
				SetRect(&rSource, 737, 757, 907, 927);
				m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.5f, 2.0f, &rSource);
				m_pFont->PrintString("Tutorial - Introduction", 220, 160, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("This tutorial will teach you", 240, 190, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("all of the basics of Blast.", 240, 220, 1.0, 255, 255, 255, 255 );

				m_pFont->PrintString("If you are already familiar with Blast", 240, 310, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("Press Escape to skip tutorial.", 240, 340, 1.0, 255, 255, 255, 255 );

				m_pFont->PrintString("Click anywhere to continue...", 240, 440, 1.0f, 255, 255, 255, 255);
			}
			break;
		case 1:
			{
				SetRect(&rSource, 737, 757, 907, 927);
				m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.0f, 2.0f, &rSource);
				m_pFont->PrintString("Tutorial 1 - Movement and Shooting", 220, 160, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("W - Move Up", 240, 190, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("S - Move Down", 240, 220, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("A - Move Left", 240, 250, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("D - Move Right", 240, 280, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("F - Use a health pack", 240, 310, 1.0, 255, 255, 255, 255);
				m_pFont->PrintString("Left-Click - Shoot", 240, 340, 1.0, 255, 255, 255, 255 );

				m_pFont->PrintString("Find the enemy and kill him!", 240, 390, 1.0f, 255, 255, 255, 255);
				m_pFont->PrintString("Shoot to continue...", 240, 440, 1.0f, 255, 255, 255, 255);
			}
			break;
		case 2:
			{
				SetRect(&rSource, 737, 757, 907, 927);
				m_pTM->Draw(pPlayer->GetHUDImgID(), 180, 150, 3.5f, 2.0f, &rSource);
				m_pFont->PrintString("Tutorial 2 - Pickups", 220, 160, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("After you kill each enemy you have", 240, 190, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("a random chance of getting a pickup.", 240, 220, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("Pickups can give you money,.", 240, 250, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("ammo, or a random weapon.", 240, 280, 1.0, 255, 255, 255, 255 );

				m_pFont->PrintString("Get the pickup!", 240, 370, 1.0f, 255, 255, 255, 255);
				m_pFont->PrintString("Shoot to continue...", 240, 440, 1.0f, 255, 255, 255, 255);

			}
			break;
			case 3:
			{
				SetRect(&rSource, 737, 757, 907, 927);
				m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.0f, 2.0f, &rSource);
				m_pFont->PrintString("Tutorial 3 - Bunker Placement", 220, 160, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("Find the shop", 240, 190, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("Select buildings at the bottom", 240, 220, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("Then Select the Bunker", 240, 250, 1.0, 255, 255, 255, 255 );
				m_pFont->PrintString("Click on the map to place it", 240, 280, 1.0, 255, 255, 255, 255 );

				m_pFont->PrintString("Place a Bunker", 240, 370, 1.0f, 255, 255, 255, 255);
				m_pFont->PrintString("Shoot to continue...", 240, 440, 1.0f, 255, 255, 255, 255);
			}
			break;
			case 4:
				{
					SetRect(&rSource, 737, 757, 907, 927);
					m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.0f, 2.0f, &rSource);
					m_pFont->PrintString("Tutorial 3 - Bunker Placement (cont.)", 220, 160, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Bunkers can hold units.", 240, 190, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("While units are in bunkers they", 240, 220, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Don't take any damage", 240, 250, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("but cannot move", 240, 280, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("and cannot take any orders", 240, 310, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("until the bunker is destroyed.", 240, 340, 1.0, 255, 255, 255, 255);

					m_pFont->PrintString("Shoot to continue...", 240, 440, 1.0f, 255, 255, 255, 255);
				}
				break;
			case 5:
				{
					SetRect(&rSource, 737, 757, 907, 927);
					m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.0f, 2.0f, &rSource);
					m_pFont->PrintString("Tutorial 4 - Storehouse Placement", 220, 160, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Find the shop", 240, 190, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Select buildings at the bottom", 240, 220, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Then Select the Storehouse", 240, 250, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Click on the map to place it", 240, 280, 1.0, 255, 255, 255, 255 );

					m_pFont->PrintString("Place a Storehouse", 240, 370, 1.0f, 255, 255, 255, 255);
					m_pFont->PrintString("Shoot to continue...", 240, 440, 1.0f, 255, 255, 255, 255);
				}
				break;
			case 6:
				{
					SetRect(&rSource, 737, 757, 907, 927);
					m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.0f, 2.0f, &rSource);
					m_pFont->PrintString("Tutorial 4 - Storehouse Placement (cont.)", 220, 160, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Storehouses collect food for you.", 240, 190, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Food is necessary to", 240, 220, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("keep your units alive.", 240, 250, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Storehouses can only be", 240, 280, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("placed near farm tiles.", 240, 310, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Careful, the enemy", 240, 340, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("can and will destroy", 240, 340, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("the storehouse if not places carefully.", 240, 370, 1.0, 255, 255, 255, 255 );

					m_pFont->PrintString("Shoot to continue...", 240, 440, 1.0f, 255, 255, 255, 255);
				}
				break;
			case 7:
				{
					SetRect(&rSource, 737, 757, 907, 927);
					m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.0f, 2.0f, &rSource);
					m_pFont->PrintString("Tutorial 5 - Mine Placement", 220, 160, 1.0, 255, 255, 255, 255);
					m_pFont->PrintString("Find the shop", 240, 190, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Select buildings at the bottom", 240, 220, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Then Select the Mine", 240, 250, 1.0, 255,255, 255, 255 );
					m_pFont->PrintString("Click on the map to place it", 240, 280, 1.0, 255, 255, 255, 255 );

					m_pFont->PrintString("Place a Mine", 240, 370, 1.0f, 255, 255, 255, 255);
					m_pFont->PrintString("Shoot to continue...", 240, 440, 1.0f, 255, 255, 255, 255);
				}
				break;
			case 8:
				{
					SetRect(&rSource, 737, 757, 907, 927);
					m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.0f, 2.0f, &rSource);
					m_pFont->PrintString("Tutorial 5 - Mine Placement (cont.)", 220, 160, 1.0, 255, 255, 255, 255);
					m_pFont->PrintString("Mines collect ore for you.", 240, 190, 1.0, 255, 255, 255, 255);
					m_pFont->PrintString("ore is necessary to", 240, 220, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("build buildings.", 240, 250, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Mines can only be", 240, 280, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("placed near mountain tiles.", 240, 310, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Careful, the enemy", 240, 340, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("can and will destroy", 240, 340, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("the Mine if not places carefully.", 240, 370, 1.0, 255, 255, 255, 255 );

					m_pFont->PrintString("Shoot to continue...", 240, 440, 1.0f, 255, 255, 255, 255);
				}
				break;
			case 9:
				{
					SetRect(&rSource, 737, 757, 907, 927);
					m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.0f, 2.0f, &rSource);
					m_pFont->PrintString("Tutorial 6 - Unit Management", 220, 160, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Press 1 - Switches units to attack", 240, 190, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("Press 2 - Switches units to defend", 240, 220, 1.0, 255,255, 255, 255 );
					m_pFont->PrintString("Press 3 - Switches units to follow", 240, 250, 1.0, 255, 255, 255, 255 );

					m_pFont->PrintString("More units can be bought from the shop.", 240, 290, 1.0, 255, 255, 255, 255 );

					m_pFont->PrintString("Use all 3 commands!", 240, 370, 1.0f, 255, 255, 255, 255);
					m_pFont->PrintString("Click anywhere to continue...", 240, 440, 1.0f, 255, 255, 255, 255);

				}
				break;
			case 10:
				{
					SetRect(&rSource, 737, 757, 907, 927);
					m_pTM->Draw(pPlayer->GetHUDImgID(), 200, 150, 3.0f, 2.0f, &rSource);
					m_pFont->PrintString("Tutorial 7 -Winning the game", 220, 160, 1.0, 255, 255, 255, 255 );
					m_pFont->PrintString("To win simply destroy the helicopter.", 240, 190, 1.0, 255, 255, 255, 255 );

					m_pFont->PrintString("Destroy the Helicopter!", 240, 370, 1.0f, 255, 255, 255, 255);
					m_pFont->PrintString("Click anywhere to continue...", 240, 440, 1.0f, 255, 255, 255, 255);

				}
				break;
		}
	}

}

void CGamePlayState::TutorialInit()
{
	m_pTileManager->LoadTiles("Tutorial64x64.xml");
	InitPlayer();

	m_bTeacherMode = true;
	m_bAttackActivate = false;
	m_bDefenseActivate = false;
	m_bFollowActivate = false;

	m_nCurrentLesson = 0;
	m_nNumEnemies = 0;
	m_nNumPickups = 0;
	m_nNumMines = 0;
	m_nNumStoreHouse = 0;
	m_nNumBunkers = 0;

	//Setting up the Shop
	m_pShop = m_pOM->CreateObject( _T("CShop"));
	m_pShop->SetPosX( 256 );
	m_pShop->SetPosY( 928 );
	m_pShop->SetHeight(192);
	m_pShop->SetWidth(384);
}

void CGamePlayState::TutorialUpdate( float fElapsedTime )
{
	if (m_bTeacherMode == false )
	{
		m_pMiniMap->Update(fElapsedTime);

		m_pOM->UpdateObjects(fElapsedTime);
		m_pOM->CheckCollisions();
		CPlayer* pPlayer = dynamic_cast< CPlayer* >( m_pPlayer );
		pPlayer->CheckTileCollision();

		m_pMS->ProcessMessages();
		CEventManager::GetInstance()->ProcessEvents();

		//TODO: ROB change to a function call from the tile manager
		CCamera::GetInstance()->Update(m_pPlayer->GetPosX(), m_pPlayer->GetPosY(), 
			m_pTileManager->GetWorldWidth(), m_pTileManager->GetWorldHeight());

		switch( m_nCurrentLesson )
		{
		case 0:
			{
				m_bTeacherMode = true;
				m_nCurrentLesson++;
			}
			break;
		case 1:
			{
				if( m_nNumEnemies == 0 )
				{
					m_bTeacherMode = true;
					m_nCurrentLesson++;
				}
			
			}
			break;
		case 2:
			{
				if( m_nNumPickups == 0)
				{
					m_bTeacherMode = true;
					m_nCurrentLesson++;
				}
				
			}
			break;
		case 3:
			{
				if( m_nNumBunkers > 0 )
				{
					m_bTeacherMode = true;
					m_nCurrentLesson++;
				}
			}
			break;
		case 4:
			{
				m_bTeacherMode = true;
				m_nCurrentLesson++;
			}
			break;
		case 5:
			{
				if( m_nNumStoreHouse > 0 )
				{
					m_bTeacherMode = true;
					m_nCurrentLesson++;
				}
			}
			break;
		case 6:
			{
				m_bTeacherMode = true;
				m_nCurrentLesson++;
			}
			break;
		case 7:
			{
				if( m_nNumMines > 0 )
				{
					m_bTeacherMode = true;
					m_nCurrentLesson++;
				}
			}
			break;
		case 8:
			{
				m_bTeacherMode = true;
				m_nCurrentLesson++;
			}
			break;
		case 9:
			{
				if( m_bAttackActivate == true && m_bDefenseActivate == true && m_bFollowActivate == true )
				{
					m_bTeacherMode = true;
					m_nCurrentLesson++;
				}
			}
			break;
		case 10:
			{

			}
			break;
		}

	}

	if(m_bTutotial == false)
	{
		list<IEntity*> objList = m_pOM->GetObjectList();
		for(std::list<IEntity*>::iterator iter = objList.begin();
			iter != objList.end(); ++iter)
		{
			switch((*iter)->GetType())
			{
			case CEntity::OBJ_ALLY:
			case CEntity::OBJ_ENEMY:
			case CEntity::OBJ_BULLET:
			case CEntity::OBJ_BUNKER:
			case CEntity::OBJ_STOREHOUSE:
			case CEntity::OBJ_MINE:
			case CEntity::OBJ_SHOP:
				m_pOM->RemoveObject((*iter));
				break;
			};
		}
		m_pOM->RemoveObject(m_pHelicopter);
		m_pOM->RemoveObject(m_pPlayer);
		m_pPlayer->Release();
		m_pHelicopter->Release();
		m_pMiniMap->Release();
		m_pShop->Release();

		Enter();
	}
	if(m_nCurrLevel == 0)
	{
		if(m_pPlayer->GetHealth() <= 0)
		{
			m_pPlayer->SetHealth(200);
		}
	}
}