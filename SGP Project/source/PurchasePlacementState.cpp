#include "PurchasePlacementState.h"
#include "ShopState.h"
#include "GamePlayState.h"
// Bitmap Font
#include "BitmapFont.h"

#include "StoreHouse.h"
#include "Mine.h"
#include "Bunker.h"

//Message System
#include "MessageSystem.h"

#include "CreateBunkerMessage.h"
#include "CreateStoreHouseMessage.h"
#include "CreateMineMessage.h"

#include <vector>

#define MAP_SCALE_FACTOR 0.25f
// SINGLETON!
CPurchasePlacementState* CPurchasePlacementState::GetInstance( void )
{
	// Lazy instantiation
	//	- created the first time the function is called
	static CPurchasePlacementState s_Instance;

	return &s_Instance;
}

CPurchasePlacementState::CPurchasePlacementState(void)
{
	//SGD Wrapper
	m_pD3D = nullptr;
	m_pTM  = nullptr;
	m_pDI  = nullptr;
	m_pXA  = nullptr;
	m_pFont = nullptr;

	//Textures
	m_nCursorImgID		= -1;
	m_nBackgroundImgID	= -1;
	m_nBunkerImgID		= -1;
	m_nStoreHouseImgID	= -1;
	m_nMineHouseImgID	= -1;
	m_nNoPlacementZoneID= -1;
	m_nPlacementZoneID	= -1;
	//Mouse Position
	m_nPosX = 0;
	m_nPosY = 0;
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	m_nBunkerCount = 0;
	m_nPlaceObjectSFXID =-1;
	m_nStartTileNumberX	= 0;
	m_nStartTileNumberY	= 0;

	m_nCurrentCord		= 0;


	m_pPlayer	= nullptr;
}

CPurchasePlacementState::~CPurchasePlacementState(void)
{
	m_vBuildingBoughtInfo.clear();
}

void CPurchasePlacementState::Enter(void)
{
	//Access the singletons of the SGD Wrappers
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM  = CSGD_TextureManager::GetInstance();
	m_pDI  = CSGD_DirectInput::GetInstance();
	m_pXA  = CSGD_XAudio2::GetInstance();

	m_pOM = ObjectManager::GetInstance();
	m_pFont = CBitmapFont::GetInstance();

	m_pTileManager = CTileManager::GetInstance();



	m_bIsBunkerSelected			= false;
	m_bIsStoreHouseSelected		= false;
	m_bIsMineHouseSelected		= false;

	m_bIsBunker					= false;
	m_bIsStoreHouse				= false;
	m_bIsFarm					= false;		




	m_nPlaceObjectSFXID = m_pXA->SFXLoadSound(_T("resource/sounds/SFX_PLACEMENT.wav"));

	//Using the texture manager to load the images
	m_nCursorImgID			= m_pTM->LoadTexture(_T("resource/graphics/Blast_Mouse_Cursor_PlacementEditor.png"), D3DCOLOR_XRGB(0,0,0)); 
	m_nBackgroundImgID		= m_pTM->LoadTexture(_T("resource/graphics/Blast_BuildingPlacementEditor_Bg.png"));
	m_nBunkerImgID			= m_pTM->LoadTexture(_T("resource/graphics/Bunker.png"));
	m_nStoreHouseImgID		= m_pTM->LoadTexture(_T("resource/graphics/StoreHouse.png"));
	m_nMineHouseImgID		= m_pTM->LoadTexture(_T("resource/graphics/Shop.png"));
	m_nNoPlacementZoneID	= m_pTM->LoadTexture(_T("resource/graphics/Blast_NoPlacement.png"));
	m_nPlacementZoneID		= m_pTM->LoadTexture(_T("resource/graphics/Blast_Placement.png"));

	m_nBunkerSmallID		= m_pTM->LoadTexture(_T("resource/graphics/Blast_Bunker_Small.png"));
	m_nMineHouseSmallID		= m_pTM->LoadTexture(_T("resource/graphics/Blast_MineHouse_Small.png"));
	m_nStoreHouseSmallID	= m_pTM->LoadTexture(_T("resource/graphics/Blast_StoreHouse_Small.png"));


	m_nBitmapFontImgID		= m_pTM->LoadTexture(_T("resource/graphics/SansationFont_0.png"), D3DCOLOR_XRGB(0,0,0));


	m_nStartTileNumberX	= 0;
	m_nStartTileNumberY	= 0;

	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{		
		BuildingPlacedInfo info;

		if( (*iter)->GetType() == CEntity::OBJ_BUNKER )
		{
			CBunker* pBunker = dynamic_cast<CBunker*>((*iter));
			info.buildingType = pBunker->GetType();
			info.tileX = (int)pBunker->GetPosX();
			info.tileY = (int)pBunker->GetPosY();
			m_vBuildingInMapInfo.push_back(info);
			continue;
		} 
		if((*iter)->GetType() == CEntity::OBJ_STOREHOUSE)
		{
			CStoreHouse* pStoreHouse = dynamic_cast<CStoreHouse*>((*iter));
			info.buildingType = pStoreHouse->GetType();
			info.tileX = (int)pStoreHouse->GetPosX();
			info.tileY = (int)pStoreHouse->GetPosY();			
			m_vBuildingInMapInfo.push_back(info);
			continue;
		}
		if((*iter)->GetType() == CEntity::OBJ_MINE )
		{
			CMine* pMine = dynamic_cast<CMine*>((*iter));
			info.buildingType = pMine->GetType();
			info.tileX = (int)pMine->GetPosX();
			info.tileY = (int)pMine->GetPosY();			
			m_vBuildingInMapInfo.push_back(info);
			continue;
		}
		if((*iter)->GetType() == CEntity::OBJ_PLAYER)
		{
			m_pPlayer = dynamic_cast<CPlayer*>(*iter);	
		}
	}
}

void CPurchasePlacementState::Exit(void)
{
	//Unload all the images
	if(m_nCursorImgID != -1)
	{
		m_pTM->UnloadTexture(m_nCursorImgID);
		m_nCursorImgID = -1;
	}

	if(m_nBackgroundImgID != -1)
	{
		m_pTM->UnloadTexture(m_nBackgroundImgID);
		m_nBackgroundImgID = -1;
	}

	if(m_nPlaceObjectSFXID != -1)
	{
		m_pXA->SFXStopSound(m_nPlaceObjectSFXID);
		m_pXA->SFXUnloadSound(m_nPlaceObjectSFXID);
		m_nPlaceObjectSFXID = -1;
	}	
	//Creating Building in the game from placement editor
	for(unsigned int count = 0; count < m_vBuildingBoughtInfo.size(); count++)
	{
		if(m_vBuildingBoughtInfo[count].buildingType == CEntity::OBJ_MINE)
		{
			CCreateMineMessage* pMsg = new CCreateMineMessage(m_vBuildingBoughtInfo[count].tileX,m_vBuildingBoughtInfo[count].tileY);
			CMessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;			
		}
		else if(m_vBuildingBoughtInfo[count].buildingType == CEntity::OBJ_STOREHOUSE)
		{
			CCreateStoreHouseMessage* pMsg = new CCreateStoreHouseMessage(m_vBuildingBoughtInfo[count].tileX,m_vBuildingBoughtInfo[count].tileY);
			CMessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
		else if(m_vBuildingBoughtInfo[count].buildingType == CEntity::OBJ_BUNKER)
		{
			CCreateBunkerMessage* pMsg = new CCreateBunkerMessage(m_vBuildingBoughtInfo[count].tileX,m_vBuildingBoughtInfo[count].tileY,m_vBuildingBoughtInfo[count].bunkerID);
			CMessageSystem::GetInstance()->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}


	//m_vBuildingBoughtInfo.clear();
	//m_vBuildingInMapInfo.clear();
}

bool CPurchasePlacementState::Input(void)
{
	if(m_pDI->KeyPressed(DIK_ESCAPE) || m_pDI->JoystickButtonPressed(6) )
	{
		CGameStateManager::GetInstance()->PreviousState();
		return true;
	}	

	if(m_pDI->MouseButtonDown( (char)DIMOUSE_XAXISAB ))
	{
		if( (m_pDI->MouseGetPosX() > 50 && m_pDI->MouseGetPosX() < 250) && (m_pDI->MouseGetPosY() > 245 && m_pDI->MouseGetPosY() < 290))
		{
			m_bIsBunkerSelected				= true;
			m_bIsStoreHouseSelected 		= false;
			m_bIsMineHouseSelected			= false;
		}
		if( (m_pDI->MouseGetPosX() > 50 && m_pDI->MouseGetPosX() < 250) && (m_pDI->MouseGetPosY() > 305 && m_pDI->MouseGetPosY() < 345))
		{
			m_bIsBunkerSelected			= false;
			m_bIsStoreHouseSelected 	= true;
			m_bIsMineHouseSelected		= false;
		}
		if( (m_pDI->MouseGetPosX() > 50 && m_pDI->MouseGetPosX() < 250) && (m_pDI->MouseGetPosY() > 355 && m_pDI->MouseGetPosY() < 400))
		{
			m_bIsBunkerSelected			= false;
			m_bIsStoreHouseSelected 	= false;
			m_bIsMineHouseSelected		= true;
		}
	}


	if( (m_pDI->MouseGetPosX() > 50 && m_pDI->MouseGetPosX() < 250) && (m_pDI->MouseGetPosY() > 245 && m_pDI->MouseGetPosY() < 290))
	{
		m_bIsBunker			= true;
		m_bIsStoreHouse		= false;
		m_bIsFarm			= false;
	}
	if( (m_pDI->MouseGetPosX() > 50 && m_pDI->MouseGetPosX() < 250) && (m_pDI->MouseGetPosY() > 305 && m_pDI->MouseGetPosY() < 345))
	{
		m_bIsBunker			= false;
		m_bIsStoreHouse		= true;
		m_bIsFarm			= false;
	}
	if( (m_pDI->MouseGetPosX() > 50 && m_pDI->MouseGetPosX() < 250) && (m_pDI->MouseGetPosY() > 355 && m_pDI->MouseGetPosY() < 400))
	{
		m_bIsBunker			= false;
		m_bIsStoreHouse		= false;
		m_bIsFarm			= true;
	}
	return true;
}

void CPurchasePlacementState::Update(float fElapsedTime)
{
	//if(m_pDI->MouseGetPosX()-250 > (MAP_SCALE_FACTOR*m_pTileManager->GetTileWidth()*m_pTileManager->GetMapWidth()*0.5f) && m_pDI->MouseGetPosX() < (MAP_SCALE_FACTOR*m_pTileManager->GetTileHeight()*m_pTileManager->GetMapHeight()*0.5f))

	switch(m_nCurrentCord)
	{
	case 0:
		if(( (m_pDI->MouseGetPosX()-250) > MAP_SCALE_FACTOR*m_pTileManager->GetTileWidth()*m_pTileManager->GetMapWidth()*0.5f + 32) && (m_pDI->MouseGetPosY() < MAP_SCALE_FACTOR*m_pTileManager->GetTileHeight()*m_pTileManager->GetMapHeight()*0.5f))
		{
			m_nStartTileNumberX = 30;
			m_nStartTileNumberY = 0;
			m_nOffsetX = m_nStartTileNumberX*m_pTileManager->GetTileWidth();
			m_nOffsetY = m_nStartTileNumberY*m_pTileManager->GetTileHeight();
			m_nCurrentCord = 1;
		}
		else if(( (m_pDI->MouseGetPosX()-250) < MAP_SCALE_FACTOR*m_pTileManager->GetTileWidth()*m_pTileManager->GetMapWidth()*0.5f + 32) && (m_pDI->MouseGetPosY() > MAP_SCALE_FACTOR*m_pTileManager->GetTileHeight()*m_pTileManager->GetMapHeight()*0.5f + 32) )
		{
			m_nStartTileNumberX = 0;
			m_nStartTileNumberY = 30;
			m_nOffsetX = m_nStartTileNumberX*m_pTileManager->GetTileWidth();
			m_nOffsetY = m_nStartTileNumberY*m_pTileManager->GetTileHeight();
			m_nCurrentCord = 2;
		}
		break;
	case 1:
		if(( (m_pDI->MouseGetPosX()-250) < 0) && (m_pDI->MouseGetPosY() < MAP_SCALE_FACTOR*m_pTileManager->GetTileHeight()*m_pTileManager->GetMapHeight()*0.5f))
		{
			m_nStartTileNumberX = 0;
			m_nStartTileNumberY = 0;
			m_nOffsetX = m_nStartTileNumberX*m_pTileManager->GetTileWidth();
			m_nOffsetY = m_nStartTileNumberY*m_pTileManager->GetTileHeight();
			m_nCurrentCord = 0;
		}
		else if(( (m_pDI->MouseGetPosX()-250) > 0) && (m_pDI->MouseGetPosY() > MAP_SCALE_FACTOR*m_pTileManager->GetTileHeight()*m_pTileManager->GetMapHeight()*0.5f + 32) )
		{
			m_nStartTileNumberX = 30;
			m_nStartTileNumberY = 30;
			m_nOffsetX = m_nStartTileNumberX*m_pTileManager->GetTileWidth();
			m_nOffsetY = m_nStartTileNumberY*m_pTileManager->GetTileHeight();
			m_nCurrentCord = 3;
		}
		break;
	case 2:
		if(( (m_pDI->MouseGetPosX()-250) < MAP_SCALE_FACTOR*m_pTileManager->GetTileWidth()*m_pTileManager->GetMapWidth()*0.5f + 32) && (m_pDI->MouseGetPosY() < 0))
		{
			m_nStartTileNumberX = 0;
			m_nStartTileNumberY = 0;
			m_nOffsetX = m_nStartTileNumberX*m_pTileManager->GetTileWidth();
			m_nOffsetY = m_nStartTileNumberY*m_pTileManager->GetTileHeight();
			m_nCurrentCord = 0;
		}
		else if( ((m_pDI->MouseGetPosX()-250) > MAP_SCALE_FACTOR*m_pTileManager->GetTileWidth()*m_pTileManager->GetMapWidth()*0.5f + 32) && (m_pDI->MouseGetPosY() > 0) )
		{
			m_nStartTileNumberX = 30;
			m_nStartTileNumberY = 30;
			m_nOffsetX = m_nStartTileNumberX*m_pTileManager->GetTileWidth();
			m_nOffsetY = m_nStartTileNumberY*m_pTileManager->GetTileHeight();
			m_nCurrentCord = 3;
		}
		break;
	case 3:
		if(( (m_pDI->MouseGetPosX()-250) < 0) && (m_pDI->MouseGetPosY() > 0))
		{
			m_nStartTileNumberX = 0;
			m_nStartTileNumberY = 30;
			m_nOffsetX = m_nStartTileNumberX*m_pTileManager->GetTileWidth();
			m_nOffsetY = m_nStartTileNumberY*m_pTileManager->GetTileHeight();
			m_nCurrentCord = 2;
		}
		else if(( (m_pDI->MouseGetPosX()-250) > 0) && (m_pDI->MouseGetPosY() < 0) )
		{
			m_nStartTileNumberX = 30;
			m_nStartTileNumberY = 0;
			m_nOffsetX = m_nStartTileNumberX*m_pTileManager->GetTileWidth();
			m_nOffsetY = m_nStartTileNumberY*m_pTileManager->GetTileHeight();
			m_nCurrentCord = 1;
		}
		break;
	}

	/*if(m_pDI->MouseGetPosX() > (MAP_SCALE_FACTOR*m_pTileManager->GetTileWidth()*m_pTileManager->GetMapWidth()*0.5f) && m_pDI->MouseGetPosX() < (MAP_SCALE_FACTOR*m_pTileManager->GetTileWidth()*m_pTileManager->GetMapWidth()*0.5f))

	{
	m_nStartTileNumberX = 32;
	m_nStartTileNumberY = 0;
	}*/
}

void CPurchasePlacementState::Render(void)
{
	// Render background
	RECT rSource = {0,0,750,550};
	m_pTM->Draw(m_nBackgroundImgID,25,25,1.0f,1.0f,&rSource,0.0f,0.0f,0.0f);

	m_pTileManager->PlacementEditorMapRender(250,0,MAP_SCALE_FACTOR,MAP_SCALE_FACTOR,m_nStartTileNumberX,m_nStartTileNumberY);

	//rendering the buildings in the map.
	for(unsigned int count = 0; count<m_vBuildingInMapInfo.size(); count++)
	{
		if(m_vBuildingInMapInfo[count].buildingType == CEntity::OBJ_MINE)
		{
			if( m_nCurrentCord == 0 )
				m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			else if( m_nCurrentCord == 1 )
			{
				if( 250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16)
					m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 2 )
			{
				if( m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR > 30*16)
					m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 3 )
			{
				if( 250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16 && m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR > 30*16 )
					m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			//m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
		}
		else if(m_vBuildingInMapInfo[count].buildingType == CEntity::OBJ_STOREHOUSE)
		{
			if( m_nCurrentCord == 0 )
				m_pTM->Draw(m_nStoreHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			else if( m_nCurrentCord == 1 )
			{
				if( 250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16)
					m_pTM->Draw(m_nStoreHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 2 )
			{
				if( m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR > 30*16)
					m_pTM->Draw(m_nStoreHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 3 )
			{
				if( 250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16 && m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR > 30*16 )
					m_pTM->Draw(m_nStoreHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			//m_pTM->Draw(m_nStoreHouseSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
		}
		else if(m_vBuildingInMapInfo[count].buildingType == CEntity::OBJ_BUNKER)
		{
			if( m_nCurrentCord == 0 )
				m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			else if( m_nCurrentCord == 1 )
			{
				if( 250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16)
					m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 2 )
			{
				if( m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR > 30*16)
					m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 3 )
			{
				if( 250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16 && m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR > 30*16 )
					m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			//m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
		}
	}
	//rendering the buildings bought now in the placement editor.
	for(unsigned int count = 0; count<m_vBuildingBoughtInfo.size(); count++)
	{
		if(m_vBuildingBoughtInfo[count].buildingType == CEntity::OBJ_MINE)
		{
			if( m_nCurrentCord == 0 )
				m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			else if( m_nCurrentCord == 1 )
			{
				if( 250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16)
					m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 2 )
			{
				if( m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR > 30*16)
					m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 3 )
			{
				if( 250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16 && m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR > 30*16 )
					m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			//m_pTM->Draw(m_nMineHouseSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
		}
		else if(m_vBuildingBoughtInfo[count].buildingType == CEntity::OBJ_STOREHOUSE)
		{
			if( m_nCurrentCord == 0 )
				m_pTM->Draw(m_nStoreHouseSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			else if( m_nCurrentCord == 1 )
			{
				if( 250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16)
					m_pTM->Draw(m_nStoreHouseSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 2 )
			{
				if( m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR > 30*16)
					m_pTM->Draw(m_nStoreHouseSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 3 )
			{
				if( 250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16 && m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR > 30*16 )
					m_pTM->Draw(m_nStoreHouseSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}

		}
		else if(m_vBuildingBoughtInfo[count].buildingType == CEntity::OBJ_BUNKER)
		{
			if( m_nCurrentCord == 0 )
				m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			else if( m_nCurrentCord == 1 )
			{
				if( 250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16)
					m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 2 )
			{
				if( m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR > 30*16)
					m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			else if( m_nCurrentCord == 3 )
			{
				if( 250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR > 250+30*16 && m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR > 30*16 )
					m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR - 30*16),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR - 30*16),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
			}
			//m_pTM->Draw(m_nBunkerSmallID,(int)(250+m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR),(int)(m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR),1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
		}
	}

	// Render Cursor
	SetRect(&rSource,0,0,50,50);
	if(m_bIsBunkerSelected)
		m_pTM->Draw(m_nBunkerImgID, m_pDI->MouseGetPosX() +32, m_pDI->MouseGetPosY() +32,0.5f,0.5f,nullptr,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
	else if(m_bIsStoreHouseSelected)
		m_pTM->Draw(m_nStoreHouseImgID, m_pDI->MouseGetPosX() +32, m_pDI->MouseGetPosY()  +32,0.5f,0.5f,nullptr,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
	else if(m_bIsMineHouseSelected)
		m_pTM->Draw(m_nMineHouseImgID, m_pDI->MouseGetPosX() +32, m_pDI->MouseGetPosY()  +32,0.5f,0.5f,nullptr,0,0,0,D3DCOLOR_ARGB(255,255,255,255));

	m_pTM->Draw(m_nCursorImgID, m_pDI->MouseGetPosX(), m_pDI->MouseGetPosY());
	//std::stringstream ss;//create a stringstream
	//   ss << m_pDI->MouseGetPosX() << " " << m_pDI->MouseGetPosY();//add number to the stream
	//m_pFont->PrintString(ss.str().c_str(), 100, 140, 1.0f, 255, 255, 255, 255);
	//ss.str("");


	m_pFont->PrintString(" Price: ", 100, 80 , 1.0f, 255,255,0,0);

	if(m_bIsBunker == true)
	{
		m_pFont->PrintString(" $400, 100 Ore ", 50, 120, 1.0f, 255,0,255,0);

	}
	if(m_bIsStoreHouse == true)
	{
		m_pFont->PrintString(" $200, 50 Ore ", 50, 120, 1.0f, 255,0,255,0);

	}
	if(m_bIsFarm == true)
	{
		m_pFont->PrintString(" $200, 50 Ore ", 50, 120, 1.0f, 255,0,255,0);

	}

	m_pD3D->GetSprite()->Flush();
	//m_pD3D->DrawText(_T("test"),200,100,255,255,255);

	//for(int count = 0; count<m_vBuildingInMapInfo.size(); count++)
	//{
	//	if(m_vBuildingInMapInfo[count].tileX == m_pDI->MouseGetPosX() && )
	//	{
	//		m_pTM->Draw(m_nMineHouseSmallID,250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR,m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR,1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
	//	}
	//	else if(m_vBuildingInMapInfo[count].buildingType == CEntity::OBJ_STOREHOUSE)
	//	{
	//		m_pTM->Draw(m_nStoreHouseSmallID,250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR,m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR,1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
	//	}
	//	else if(m_vBuildingInMapInfo[count].buildingType == CEntity::OBJ_BUNKER)
	//	{
	//		m_pTM->Draw(m_nBunkerSmallID,250+m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR,m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR,1.0f,1.0f,nullptr,0.0f,0.0f,0.0f);
	//	}
	//}


	if(m_bIsStoreHouseSelected)
	{
		if( m_pDI->MouseGetPosX() > 250 && m_pDI->MouseGetPosY() > 0 )
		{

			if(IsValidStoreHousePlacementTile(m_pDI->MouseGetPosX(), m_pDI->MouseGetPosY()))
			{
				SetRect(&rSource,0, 0, (int)(64*0.25f), (int)(64*0.25f));
				m_pTM->Draw(m_nPlacementZoneID, m_pDI->MouseGetPosX(), m_pDI->MouseGetPosY(), 1.0f, 1.0f, &rSource, 0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255));
				if( m_pDI->MouseButtonReleased((char)DIMOUSE_XAXISAB) )
				{

					if(m_pPlayer->GetCash() >= 200 && m_pPlayer->GetOre() >= 50)
					{
						m_pPlayer->DecreaseCash(200);
						m_pPlayer->DecreaseOre(50);

						m_pXA->SFXPlaySound(m_nPlaceObjectSFXID);
						BuildingPlacedInfo info;
						info.buildingType = CEntity::OBJ_STOREHOUSE;
						info.tileX = (int((m_pDI->MouseGetPosX()-250)/(m_pTileManager->GetTileWidth()*0.25f))) * m_pTileManager->GetTileWidth() + m_nOffsetX;
						info.tileY = (int((m_pDI->MouseGetPosY())/(m_pTileManager->GetTileHeight()*0.25f))) * m_pTileManager->GetTileHeight() + m_nOffsetY;
						m_vBuildingBoughtInfo.push_back(info);

					}
					else
					{
						ostringstream output;

						output << "Not enough resource";
						m_pFont->PrintString(output.str().c_str(), m_pDI->MouseGetPosX() - 20, m_pDI->MouseGetPosY() - 20, 0.75f, 255, 255, 255, 255);
						output.str("");
					}
				}
			}
			else
			{
				SetRect(&rSource,0, 0, (int)(64*0.25f), (int)(64*0.25f));
				m_pTM->Draw(m_nNoPlacementZoneID, (m_pDI->MouseGetPosX()), (m_pDI->MouseGetPosY()),1.0f,1.0f,&rSource,0,0,0,D3DCOLOR_ARGB(255,255,255,255));

				ostringstream output;

				output << "No surrounding farm tile";
				m_pFont->PrintString(output.str().c_str(), m_pDI->MouseGetPosX() - 20, m_pDI->MouseGetPosY() - 20, 0.75f, 255, 255, 255, 255);
				output.str("");
			}
		}
	}

	if(m_bIsMineHouseSelected)
	{
		if( m_pDI->MouseGetPosX() > 250 && m_pDI->MouseGetPosY() > 0 )
		{
			if(IsValidMineHousePlacementTile(m_pDI->MouseGetPosX(), m_pDI->MouseGetPosY()))
			{
				SetRect(&rSource,0, 0, (int)(64*0.25f), (int)(64*0.25f));
				m_pTM->Draw(m_nPlacementZoneID, (m_pDI->MouseGetPosX()), (m_pDI->MouseGetPosY()),1.0f,1.0f,&rSource,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
				if( m_pDI->MouseButtonReleased((char)(DIMOUSE_XAXISAB) ))
				{

					if(m_pPlayer->GetCash() >= 200 && m_pPlayer->GetOre() >= 50)
					{
						m_pPlayer->DecreaseCash(200);
						m_pPlayer->DecreaseOre(50);

						BuildingPlacedInfo info;
						info.buildingType = CEntity::OBJ_MINE;
						info.tileX = (int((m_pDI->MouseGetPosX()-250)/(m_pTileManager->GetTileWidth()*0.25f))) * m_pTileManager->GetTileWidth() + m_nOffsetX;
						info.tileY = (int((m_pDI->MouseGetPosY())/(m_pTileManager->GetTileHeight()*0.25f))) * m_pTileManager->GetTileHeight() + m_nOffsetY;
						m_vBuildingBoughtInfo.push_back(info);
						m_pXA->SFXPlaySound(m_nPlaceObjectSFXID);

					}
					else
					{
						ostringstream output;

						output << "Not enough resource";
						m_pFont->PrintString(output.str().c_str(), m_pDI->MouseGetPosX() - 20, m_pDI->MouseGetPosY() - 20, 0.75f, 255, 255, 255, 255);
						output.str("");
					}
				}
			}
			else
			{
				SetRect(&rSource,0, 0, (int)(64*0.25f), (int)(64*0.25f));
				m_pTM->Draw(m_nNoPlacementZoneID, (m_pDI->MouseGetPosX()), (m_pDI->MouseGetPosY()),1.0f,1.0f,&rSource,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
				ostringstream output;

				output << "No surrounding mountain";
				m_pFont->PrintString(output.str().c_str(), m_pDI->MouseGetPosX() - 20, m_pDI->MouseGetPosY() - 20, 0.75f, 255, 255, 255, 255);
				output.str("");
			}
		}
	}


	if(m_bIsBunkerSelected)
	{
		if( m_pDI->MouseGetPosX() > 250 && m_pDI->MouseGetPosY() > 0 )
		{
			if(IsValidBunkerPlacementTile(m_pDI->MouseGetPosX(), m_pDI->MouseGetPosY()))
			{
				SetRect(&rSource,0, 0, (int)(64*0.25f), (int)(64*0.25f));
				m_pTM->Draw(m_nPlacementZoneID, (m_pDI->MouseGetPosX()), (m_pDI->MouseGetPosY()),1.0f,1.0f,&rSource,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
				if( m_pDI->MouseButtonReleased((char)(DIMOUSE_XAXISAB) ))
				{

					if(m_pPlayer->GetCash() >= 400 && m_pPlayer->GetOre() >= 100)
					{
						m_pPlayer->DecreaseCash(400);
						m_pPlayer->DecreaseOre(100);

						BuildingPlacedInfo info;
						info.buildingType = CEntity::OBJ_BUNKER;
						info.tileX = (int((m_pDI->MouseGetPosX()-250)/(m_pTileManager->GetTileWidth()*0.25f))) * m_pTileManager->GetTileWidth() + m_nOffsetX;
						info.tileY = (int((m_pDI->MouseGetPosY())/(m_pTileManager->GetTileHeight()*0.25f))) * m_pTileManager->GetTileHeight() + m_nOffsetY;
						info.bunkerID = ++m_nBunkerCount;
						m_vBuildingBoughtInfo.push_back(info);
						m_pXA->SFXPlaySound(m_nPlaceObjectSFXID);
					}
					//else
					//{
					//	ostringstream output;

					//	output << "No enough resource";
					//	m_pFont->PrintString(output.str().c_str(), m_pDI->MouseGetPosX() - 20, m_pDI->MouseGetPosY() - 20, 0.75f, 255, 255, 255, 255);
					//	output.str("");
					//}
				}
			}
			else
			{
				SetRect(&rSource,0, 0, (int)(64*0.25f), (int)(64*0.25f));
				m_pTM->Draw(m_nNoPlacementZoneID, (m_pDI->MouseGetPosX()), (m_pDI->MouseGetPosY()),1.0f,1.0f,&rSource,0,0,0,D3DCOLOR_ARGB(255,255,255,255));

				ostringstream output;

				output << "Bunker needs 4x4 tile space";
				m_pFont->PrintString(output.str().c_str(), m_pDI->MouseGetPosX() - 20, m_pDI->MouseGetPosY() - 20, 0.75f, 255, 255, 255, 255);
				output.str("");
			}
		}
	}

	if(m_pPlayer->GetCash() <= 0 || m_pPlayer->GetOre() <= 0)
	{
		ostringstream output;

		output << "Not enough resource";
		m_pFont->PrintString(output.str().c_str(), m_pDI->MouseGetPosX() - 20, m_pDI->MouseGetPosY() + 20, 0.75f, 255, 255, 255, 255);
		output.str("");
	}

	SetRect(&rSource, 559, 64, 905, 93);

	m_pTM->Draw(m_pPlayer->GetHUDImgID(), 424, 19, 1.0f, 1.0f, &rSource);

	ostringstream output;

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

	//
}


bool CPurchasePlacementState::IsValidStoreHousePlacementTile(int x, int y)
{

	int TileCol = (int)( (x-250)/(m_pTileManager->GetTileWidth()*0.25f));
	int TileRow = (int)( y/(m_pTileManager->GetTileHeight()*0.25f));

	for(unsigned int count = 0; count<m_vBuildingInMapInfo.size(); count++)
	{
		if(TileCol*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR == m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR && TileRow*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR == m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR)
		{
			return false;
		}		
	}

	for(unsigned int count = 0; count<m_vBuildingBoughtInfo.size(); count++)
	{
		if(TileCol*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR && TileRow*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR)
		{
			return false;
		}		
	}

	//to check if its has a bunker
	if(IsBunkerTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;

	if(IsAFarmTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol* m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsAFarmTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol - 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAFarmTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol* m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAFarmTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAFarmTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol - 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAFarmTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAFarmTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol - 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAFarmTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol* m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAFarmTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;	

	return false;

}

bool CPurchasePlacementState::IsValidMineHousePlacementTile(int x, int y)
{

	int TileCol = (int)((x-250)/(m_pTileManager->GetTileHeight()*0.25f));
	int TileRow = (int)(y/(m_pTileManager->GetTileHeight()*0.25f));

			
	for(unsigned int count = 0; count<m_vBuildingInMapInfo.size(); count++)
	{		
		if(TileCol*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR == m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR && TileRow*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR == m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR)
		{
			return false;
		}		
	}

	for(unsigned int count = 0; count<m_vBuildingBoughtInfo.size(); count++)
	{
		if(TileCol*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR && TileRow*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR)
		{
			return false;
		}		
	}

	//to check if its has a bunker
	if(IsBunkerTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;


	//TileCol	+= 1;
	//TileRow	+= 1;	
	if(IsAMountainTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol* m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsAMountainTile((int)((TileRow - 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol - 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;																		   
	if(IsAMountainTile((int)((TileRow - 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAMountainTile((int)((TileRow - 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAMountainTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol - 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAMountainTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;
	if(IsAMountainTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol - 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;																		   
	if(IsAMountainTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol* m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;																		   
	if(IsAMountainTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return true;

	return false;

}

bool CPurchasePlacementState::IsValidBunkerPlacementTile(int x, int y)
{
	int TileCol = (int)((x-250)/(m_pTileManager->GetTileWidth()*0.25f));
	int TileRow = (int)(y/(m_pTileManager->GetTileHeight()*0.25f));

	//to check if its a no build zone
	vector<CTileManager::Tile_L3> tileTypes = m_pTileManager->GetLayer3();
	vector<CTileManager::Tile_L3>::iterator it = tileTypes.begin();

	for(unsigned int count = 0; count<m_vBuildingInMapInfo.size(); count++)
	{
		if(TileCol*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR == m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR && TileRow*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR == m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR)
		{
			return false;
		}		
	}

	switch(m_nCurrentCord)
	{
	case 0:			
		TileCol += 0;
		TileRow += 0;
		break;
	case 1:
		TileCol += 30;
		TileRow += 0;
		break;
	case 2:
		TileCol += 0;
		TileRow += 30;				
		break;
	case 3:
		TileCol += 30;
		TileRow += 30;			
		break;
	}

	/*for(it = tileTypes.begin();
		it!= tileTypes.end(); ++it)
	{
		if((*it).m_nX*MAP_SCALE_FACTOR == TileCol*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR && (*it).m_nY*MAP_SCALE_FACTOR == TileRow*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR && (*it).m_bIsNoBuildZone == true)
		{			
			return false;
		}	
	}*/

	if(IsANonBuildableTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsANonBuildableTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsANonBuildableTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsANonBuildableTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;


	if(IsAFarmTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsAFarmTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsAFarmTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;

	if(IsAMountainTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsAMountainTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsAMountainTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;

	if(IsBunkerTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)((TileRow-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)(TileRow * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol-1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)(TileCol * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;
	if(IsBunkerTile((int)((TileRow + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR), (int)((TileCol + 1) * m_pTileManager->GetTileHeight() * MAP_SCALE_FACTOR)))
		return false;



	//for(int count = 0; count<m_vBuildingBoughtInfo.size(); count++)
	//{
	//	if(TileCol*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR && TileRow*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR)
	//	{				
	//		return false;
	//	}	
	//	if((TileCol+1)*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR && TileRow*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR)
	//	{
	//		return false;
	//	}
	//	if(TileCol*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR && (TileRow+1)*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR)
	//	{
	//		return false;
	//	}
	//	if((TileCol+1)*m_pTileManager->GetTileWidth()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR && (TileRow+1)*m_pTileManager->GetTileHeight()*MAP_SCALE_FACTOR == m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR)
	//	{
	//		return false;
	//	}
	//}

	return true;
}

bool CPurchasePlacementState::IsANonBuildableTile(int y, int x)
{
	//to check if its a no build zone
	vector<CTileManager::Tile_L3> tileTypes = m_pTileManager->GetLayer3();
	vector<CTileManager::Tile_L3>::iterator it = tileTypes.begin();
	for(it = tileTypes.begin();
		it!= tileTypes.end(); ++it)
	{
		if((*it).m_nX*MAP_SCALE_FACTOR == x && (*it).m_nY*MAP_SCALE_FACTOR == y && (*it).m_bIsNoBuildZone == true)
		{			
			return true;
		}	
	}
	return false;
}


bool CPurchasePlacementState::IsBunkerTile(int y, int x)
{
	for(unsigned int count = 0; count<m_vBuildingInMapInfo.size(); count++)
	{
		if(x == m_vBuildingInMapInfo[count].tileX*MAP_SCALE_FACTOR && y == m_vBuildingInMapInfo[count].tileY*MAP_SCALE_FACTOR && m_vBuildingInMapInfo[count].buildingType == CEntity::OBJ_BUNKER)
		{				
			return true;
		}	
	}

	for(unsigned int count = 0; count<m_vBuildingBoughtInfo.size(); count++)
	{
		if(x == m_vBuildingBoughtInfo[count].tileX*MAP_SCALE_FACTOR && y == m_vBuildingBoughtInfo[count].tileY*MAP_SCALE_FACTOR && m_vBuildingBoughtInfo[count].buildingType == CEntity::OBJ_BUNKER)
		{				
			return true;
		}	
	}
	return false;
}


bool CPurchasePlacementState::IsAFarmTile(int y, int x)
{
	vector<CTileManager::Tile_L2> tileTypes = m_pTileManager->GetLayer2();
	vector<CTileManager::Tile_L2>::iterator it = tileTypes.begin();

	for(it = tileTypes.begin();
		it!= tileTypes.end(); ++it)
	{
		if((*it).m_nX*MAP_SCALE_FACTOR == x+m_nOffsetX*MAP_SCALE_FACTOR && (*it).m_nY*MAP_SCALE_FACTOR == y+m_nOffsetY*MAP_SCALE_FACTOR && (*it).m_nTileType == TILETYPE_FARM+1)
		{			
			return true;
		}			
	}
	return false;	
}

bool CPurchasePlacementState::IsAMountainTile(int y, int x)
{
	vector<CTileManager::Tile_L2> tileTypes = m_pTileManager->GetLayer2();
	vector<CTileManager::Tile_L2>::iterator it = tileTypes.begin();	

	for(it = tileTypes.begin();
		it!= tileTypes.end(); ++it)
	{
		if((*it).m_nX*MAP_SCALE_FACTOR == x+m_nOffsetX*MAP_SCALE_FACTOR && (*it).m_nY*MAP_SCALE_FACTOR == y+m_nOffsetY*MAP_SCALE_FACTOR && (*it).m_nTileType == TILETYPE_MOUNTAIN+1)
		{			
			return true;
		}			
	}
	return false;	
}

void CPurchasePlacementState::CleanUp(void)
{
	m_vBuildingBoughtInfo.clear();
	m_vBuildingInMapInfo.clear();
}

//bool CPurchasePlacementState::IsOnFarmTile(int x, int y)
//{
//	vector<CTileManager::Tile_L2> tileTypes = m_pTileManager->GetLayer2();
//	vector<CTileManager::Tile_L2>::iterator it = tileTypes.begin();
//
//	//for(it = tileTypes.begin();
//	//	it!= tileTypes.end(); ++it)
//	//{
//	//	RECT rSource = {0,0,64*0.25f,64*0.25f};
//	//	m_pTM->Draw(m_nStoreHouseImgID, 250 + (*it).m_nX*0.25f, (*it).m_nY*0.25f,1.0f,1.0f,&rSource,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
//	//}	
//
//	for(it = tileTypes.begin();
//		it!= tileTypes.end(); ++it)
//	{
//		if((*it).m_nTileType == TileType::TILETYPE_FARM+1)
//		{
//			//RECT rSource = {0,0,64*0.125f,64*0.125f};
//			//m_pTM->Draw(m_nStoreHouseImgID, 250 + (*it).m_nX*0.125f, 25 + (*it).m_nY*0.125f,1.0f,1.0f,&rSource,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
//
//			if( ( x > ((*it).m_nX*0.25 + 250 ) && x < ((*it).m_nX + 64)*0.25 + 250)  && 
//				( y > ((*it).m_nY*0.25) && y < ((*it).m_nY + 64)*0.25) )
//			{
//				return true;
//				//SetRect(&rSource,(*it).m_nX*0.125f,(*it).m_nY*0.125f,64*0.125f,64*0.125f);
//				//m_pD3D->DrawRect(rSource,0,255,0);
//			}				
//
//		}			
//	}
//
//	return false;
//	//SetRect(&rSource,220 + (*it).m_nX*0.125f, 50 + (*it).m_nY*0.125f, 220 + (*it).m_nX*0.125f + 64, 50 + (*it).m_nY*0.125f + 64);
//	//SetRect(&rSource,0, 0, 64*0.125f, 64*0.125f);
//	//m_pTM->Draw(m_nStoreHouseImgID, 257 + (*it).m_nX*0.125f, 50 + (*it).m_nY*0.125f,1.0f,1.0f,&rSource,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
//
//	//m_pD3D->DrawRect(rSource,255,0,0);
//				
//}

