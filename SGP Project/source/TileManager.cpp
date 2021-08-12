/*
File:				TileManager.cpp
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Roberto Alvarado
Last Edited:		9/15/2012
Purpose:			Contain / manage the Tile code
*/
#include "TileManager.h"
#include "../tinyxml/tinyxml.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "Camera.h"

/*static*/ CTileManager* CTileManager::GetInstance()
{
	static CTileManager pTileManager;
	return &pTileManager;
}

CTileManager::CTileManager(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_nCurrentImageID = -1;
	m_nMapHeight = m_nMapWidth = m_nTileHeight = m_nTileWidth = 0;

	m_vLayer0.clear();
	m_vLayer1.clear();
	m_vLayer2.clear();
	m_vLayer3.clear();
}
CTileManager::~CTileManager(void)
{
	m_vLayer0.clear();
	m_vLayer1.clear();
	m_vLayer2.clear();
	m_vLayer3.clear();
}

bool CTileManager::LoadTiles(const char* szFileName)
{
	TiXmlDocument txmldDoc;					
	if(!txmldDoc.LoadFile(szFileName))	
		return false;

	TiXmlElement* pRoot = txmldDoc.RootElement();	
	if(pRoot == nullptr)
		return false;

	m_vLayer0.clear();
	m_vLayer1.clear();
	m_vLayer2.clear();
	m_vLayer3.clear();

	TiXmlElement* pMap = pRoot->FirstChildElement("Map");	
	pMap->Attribute("MapWidth", &m_nMapWidth);						//Start Loading MAP variables
	pMap->Attribute("MapHeight", &m_nMapHeight);			
	pMap->Attribute("TileWidth", &m_nTileWidth);			
	pMap->Attribute("TileHeight", &m_nTileHeight);		

	string pText = pMap->GetText();
	TCHAR tChar[256] = {'\0'};
	for(UINT i=0; i<pText.size();i++ )
		tChar[i] = pText[i];
	m_nCurrentImageID = m_pTM->LoadTexture(tChar, D3DCOLOR_XRGB(255,255,255));				//End Loading MAP variables

	TiXmlElement* pLayer = pMap->NextSiblingElement("Layer");

	while(pLayer != nullptr)									//start loading LAYER variables
	{
		int nLayerLevel;
		pLayer->Attribute("LayerLevel", &nLayerLevel);
		TiXmlElement* pTile = pLayer->FirstChildElement("Tile");

		switch(nLayerLevel)
		{
#pragma region  Layer 0 :Aesthetic
		case 0:
			{
				while(pTile != nullptr)											
				{
					//load in position
					Tile_L0 tLayer = {};
					pTile->Attribute("PositionX",	&tLayer.m_nX			);
					pTile->Attribute("PositionY",	&tLayer.m_nY			);

					//load in source rect
					int nX;
					pTile->Attribute("RectLeft",	&nX);
					tLayer.m_rSource.left = nX;
					pTile->Attribute("RectTop",		&nX);
					tLayer.m_rSource.top = nX;
					pTile->Attribute("RectRight",	&nX);
					tLayer.m_rSource.right = nX;
					pTile->Attribute("RectBottom",	&nX);
					tLayer.m_rSource.bottom = nX;

					//push it back
					m_vLayer0.push_back(tLayer);
					pTile = pTile->NextSiblingElement();
				}		
				if(m_vLayer0.size() ==0)
					return false;
			}
			break;
#pragma endregion
#pragma region  Layer 1 :Collision & event
		case 1:
			{
				while(pTile != nullptr)											
				{
					//load in position
					Tile_L1 tLayer = {};
					pTile->Attribute("PositionX",	&tLayer.m_nX	);
					pTile->Attribute("PositionY",	&tLayer.m_nY	);

					//the tile exists therefore it can be COLLIDED with, no calculation necessary
					//int nBool;
					//pTile->Attribute("IsCollidable", &nBool);
					//nBool==0 ? tLayer.m_bIsCollidable=true :tLayer.m_bIsCollidable=false;
					tLayer.m_bIsCollidable = true;

					//read the EVENT string into a 16 char array
					const char* pText = pTile->GetText();		
					if(pText!=nullptr)
						strcpy_s(tLayer.m_szEventName, 16, pText);

					//push it back
					m_vLayer1.push_back(tLayer);
					pTile = pTile->NextSiblingElement();
				}																
			}
			break;
#pragma endregion
#pragma region  Layer 2 :Tile Type
		case 2:
			{
				while(pTile != nullptr)											
				{
					//load in position
					Tile_L2 tLayer = {};
					pTile->Attribute("PositionX",	&tLayer.m_nX	);
					pTile->Attribute("PositionY",	&tLayer.m_nY	);

					//Load in the tile type
					pTile->Attribute("TileType",	&tLayer.m_nTileType);

					//push it back
					m_vLayer2.push_back(tLayer);
					pTile = pTile->NextSiblingElement();
				}																
			}
			break;
#pragma endregion
#pragma region  Layer 3 :No Build Zone
		case 3:
			{
				while(pTile != nullptr)											
				{
					//load in position
					Tile_L3 tLayer = {};
					pTile->Attribute("PositionX",	&tLayer.m_nX	);
					pTile->Attribute("PositionY",	&tLayer.m_nY	);

					//the tile exists therefore it is a NOBUILDZONE, no calculation necessary
					//int nBool;
					//pTile->Attribute("IsNoBuildZone", &nBool);
					//nBool==0 ? tLayer.m_bIsNoBuildZone=true :tLayer.m_bIsNoBuildZone=false;
					tLayer.m_bIsNoBuildZone = true;

					//push it back
					m_vLayer3.push_back(tLayer);
					pTile = pTile->NextSiblingElement();
				}																
			}
			break;
#pragma endregion
		}
		pLayer = pLayer->NextSiblingElement();
	}
	return true;

}

void CTileManager::Render(RECT rCamera)
{

	float middleScreenX = (float)(((rCamera.right - rCamera.left) * .5f) + rCamera.left);
	float middleScreenY = (float)(((rCamera.bottom - rCamera.top) * .5f) + rCamera.top);

	int col = (int)middleScreenX;
	int row = (int)middleScreenY;

	col /= 64;
	row /= 64;

	if(col < 8)
		col = 8;
	if(row < 6)
		row = 6;

	if(col > 56)
		col = 56;
	if(row > 56 )
		row = 56;

	int drawIndex = col * 64 + row;
	drawIndex = drawIndex - ((8 * 64) + 6);

	//int drawIndex = col * 64 + row;
	//drawIndex = drawIndex - ((8 * 64) + 6);

	//for(int x = 0; x < 16; x++)
	//{
	// for(int y = 0; y < 14; y++)
	// {
	//  int camX = (int)CCamera::GetInstance()->GetX();
	//  int camY = (int)CCamera::GetInstance()->GetY();
	//  m_pTM->Draw(m_nCurrentImageID, m_vLayer0[drawIndex].m_nX - (int)CCamera::GetInstance()->GetX(),
	//   m_vLayer0[drawIndex].m_nY - (int)CCamera::GetInstance()->GetY(),
	//   1.0f, 1.0f, &m_vLayer0[drawIndex].m_rSource);
	//  drawIndex ++;
	// }

	// drawIndex += 50;
	//}

	vector<Tile_L0>::iterator layerIter;
	for(layerIter = m_vLayer0.begin();
		layerIter!=m_vLayer0.end(); ++layerIter)
	{
		/*vector<Tile>::iterator tileIter;
		for( tileIter = layerIter->m_vTiles.begin();
		tileIter!=layerIter->m_vTiles.end(); ++tileIter)
		{*/
		//If any part of the tile is within camera range, draw it entirely
		if((layerIter->m_nX >= rCamera.left &&					//if tile left is b/t
			layerIter->m_nX <= rCamera.right)||					//camera left & right
			(layerIter->m_nY >= rCamera.top &&					//if tile top is b/t
			layerIter->m_nY<= rCamera.bottom)||					//camera top & bot
			(layerIter->m_nX+m_nTileWidth < rCamera.right &&		//if tile right is b/t
			layerIter->m_nX+m_nTileWidth > rCamera.left)||		//camera left & right
			(layerIter->m_nY+m_nTileHeight < rCamera.bottom &&	//if tile bot is b/t
			layerIter->m_nY+m_nTileHeight > rCamera.top))		//camera top & bot
		{
			m_pTM->Draw(m_nCurrentImageID,
				layerIter->m_nX - (int)CCamera::GetInstance()->GetX(), 
				layerIter->m_nY - (int)CCamera::GetInstance()->GetY(), 
				1.0f,1.0f,&layerIter->m_rSource);
		}

		//}
	}

#ifdef _DEBUG
	m_pD3D->GetSprite()->Flush();
	vector<Tile_L1>::iterator layerIter1;
	for(layerIter1 = m_vLayer1.begin();
		layerIter1!=m_vLayer1.end(); ++layerIter1)
	{

		//If any part of the tile is within camera range, draw it entirely
		if((layerIter1->m_nX >= rCamera.left &&					//if tile left is b/t
			layerIter1->m_nX <= rCamera.right)||					//camera left & right
			(layerIter1->m_nY >= rCamera.top &&					//if tile top is b/t
			layerIter1->m_nY<= rCamera.bottom)||					//camera top & bot
			(layerIter1->m_nX+m_nTileWidth < rCamera.right &&		//if tile right is b/t
			layerIter1->m_nX+m_nTileWidth > rCamera.left)||		//camera left & right
			(layerIter1->m_nY+m_nTileHeight < rCamera.bottom &&	//if tile bot is b/t
			layerIter1->m_nY+m_nTileHeight > rCamera.top))		//camera top & bot
		{
			m_pD3D->DrawRect(layerIter1->GetCollisionRect(), 255, 0, 0);			
		}

		//}
	}
#endif

}

void CTileManager::MiniMapRender(int left, int top, float scaleX, float scaleY)
{

	vector<Tile_L0>::iterator layerIter;
	for(layerIter = m_vLayer0.begin();
		layerIter!=m_vLayer0.end(); ++layerIter)
	{

		m_pTM->Draw(m_nCurrentImageID,
		(int)(layerIter->m_nX *0.0395f) + 606, 
		(int)(layerIter->m_nY *0.0395f) + 406,
		0.044f,0.044f,&layerIter->m_rSource,0,0,0, D3DCOLOR_RGBA(255,255,255,255) );
	}
}

void CTileManager::PlacementEditorMapRender(int left, int top, float scaleX, float scaleY, int nTileNoX, int nTileNoY)
{

	vector<Tile_L0>::iterator layerIter;
	for(layerIter = m_vLayer0.begin();
		layerIter!=m_vLayer0.end(); ++layerIter)
	{
		if((layerIter->m_nX/m_nTileWidth) >= nTileNoX && (layerIter->m_nY/m_nTileHeight) >= nTileNoY )
		{
			m_pTM->Draw(m_nCurrentImageID,
				(int)((layerIter->m_nX *scaleX) + left - (nTileNoX)*m_nTileWidth *scaleX), 
				(int)((layerIter->m_nY *scaleY) + top - (nTileNoY)*m_nTileHeight *scaleY),
				scaleX,scaleY,&layerIter->m_rSource,0,0,0, D3DCOLOR_RGBA(255,255,255,255) );
		}
	}
}