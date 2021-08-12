/*
	File:				TileManager.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Roberto Alvarado
	Last Edited:		9/15/2012
	Purpose:			Contain / manage the Tile code
*/

#pragma once

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>

#include <vector>
#include <string>
using namespace std;

#include "Camera.h"

enum Layers {LAYER_VISUAL=0, LAYER_COLLISION, LAYER_TYPE, NUM_LAYERS};
enum TileType {TILETYPE_FARM=0, TILETYPE_MOUNTAIN};

class CSGD_TextureManager;
class CSGD_Direct3D;

class CTileManager
{
public:

	/*
	LAYER LAYOUT
	Tile_LX is  the struct for tiles on layer X. All tiles 
	have an X and Y position.

	Layers:
	0	-	Aesthetic	:	Source rect
	1	-	Collision	:	Collidable bool, 16 char Event
	2	-	Type		:	Tile Type (follows the TileType enum)
	3	-	NoBuildZone	:	if you can build on this tile the bool == false
	
	*/

	struct Tile_L0
	{
		int		m_nX, m_nY;
		RECT	m_rSource;
	};
	struct Tile_L1
	{
		int		m_nX, m_nY;
		char	m_szEventName[16]; 
		bool	m_bIsCollidable;

		RECT GetCollisionRect() 
		{
			RECT rectCollision = {(LONG)(m_nX - CCamera::GetInstance()->GetX() ), (LONG)(m_nY - CCamera::GetInstance()->GetY() ), 
				(LONG)(m_nX - CCamera::GetInstance()->GetX() + CTileManager::GetInstance()->GetTileWidth()), 
				(LONG)(m_nY - CCamera::GetInstance()->GetY() + CTileManager::GetInstance()->GetTileHeight())};
			return rectCollision;
		}

	};
	struct Tile_L2
	{
		int		m_nX, m_nY;
		int		m_nTileType;
	};
	struct Tile_L3
	{
		int		m_nX, m_nY;
		bool	m_bIsNoBuildZone;	
	};

	//SINGLETON
	static CTileManager* GetInstance();

	
	//ACCESSORS

	int GetTileHeight()		{	return	m_nTileHeight;					}
	int GetTileWidth()		{	return	m_nTileWidth;					}
	int GetMapHeight()		{	return	m_nMapHeight;					}
	int GetMapWidth()		{	return	m_nMapWidth;					}
	int GetWorldHeight()	{	return	m_nTileHeight*	m_nMapHeight;	}
	int GetWorldWidth()		{	return	m_nTileWidth*	m_nMapWidth	;	}
	std::vector<Tile_L0> GetLayer0()	{	return	m_vLayer0;		}
	std::vector<Tile_L1> GetLayer1()	{	return	m_vLayer1;		}
	std::vector<Tile_L2> GetLayer2()	{	return	m_vLayer2;		}
	std::vector<Tile_L3> GetLayer3()	{	return	m_vLayer3;		}



	//MUTATORS

	void SetTileWidth	(int nX)	{ m_nTileHeight	= nX;	}
	void SetTileHeight	(int nY)	{ m_nTileWidth	= nY;	}
	void SetMapWidth	(int nX)	{ m_nMapHeight	= nX;	}
	void SetMapHeight	(int nY)	{ m_nMapWidth	= nY;	}


	//OTHER 

	//Returns true if tiles were successfully loaded in, false otherwise
	bool LoadTiles(const char* szFileName);
	//Takes in Camera TopLeft and renders only tiles within the cameras range
	void Render(RECT rCamera);
	void MiniMapRender(int left, int top, float scaleX, float scaleY);
	void PlacementEditorMapRender(int left, int top, float scaleX, float scaleY, int nTileNoX, int nTileNoY);

private:
	CSGD_TextureManager*	m_pTM;
	CSGD_Direct3D*			m_pD3D;

	std::vector<Tile_L0>	m_vLayer0;			//vector of Tiles in layer 0
	std::vector<Tile_L1>	m_vLayer1;			//vector of Tiles in layer 1
	std::vector<Tile_L2>	m_vLayer2;			//vector of Tiles in layer 2
	std::vector<Tile_L3>	m_vLayer3;			//vector of Tiles in layer 3

	int					m_nTileHeight;
	int					m_nTileWidth;
	int					m_nMapHeight;
	int					m_nMapWidth;
	int					m_nCurrentImageID;	//Current image ID

	CTileManager(void);
	~CTileManager(void);
	CTileManager(const CTileManager&);					//Copy Constructor
	CTileManager& operator=(const CTileManager&);		//Assignment Operator
};

