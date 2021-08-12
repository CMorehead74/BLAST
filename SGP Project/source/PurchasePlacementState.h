/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Matthe Berry
	Last Edited:		10/08/2012
	Purpose:			Contain / manage the game code
*/

#pragma once
#include "igamestate.h"
#include "GameStateManager.h"
#include "TileManager.h"

#include "ObjectManager.h"
#include <list>

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"

class CBunker;
class CStoreHouse;
class CMine;

class CPurchasePlacementState :
	public IGameState
{
public:

	struct BuildingPlacedInfo
	{
		int buildingType;
		int tileX;
		int tileY;
		int bunkerID;
	};
	// SINGLETON:
	static CPurchasePlacementState* GetInstance( void );

	int m_nBunkerCount;
	// IGameState Interface
	virtual void Enter( void );					// enter the state
	virtual void Exit( void );					// exit the state

	virtual bool Input( void );					// handle user input
	virtual void Update( float fElapsedTime );	// update Purchase State
	virtual void Render( void );				// render Purchase Map

	//Accessors
	std::vector<BuildingPlacedInfo> GetBuildingInMapInfo() { return m_vBuildingInMapInfo; }
	std::vector<BuildingPlacedInfo> GetBuildingBoughtInfo() { return m_vBuildingBoughtInfo; }
	//member functions
	//Check if its a farm tile
	bool IsValidStoreHousePlacementTile(int x, int y);
	bool IsValidMineHousePlacementTile(int x, int y);
	bool IsValidBunkerPlacementTile(int x, int y);
	bool IsAFarmTile(int x, int y);
	bool IsAMountainTile(int x, int y);
	bool IsBunkerTile(int x, int y);
	bool IsANonBuildableTile(int x, int y);

	void CleanUp(void);
private:
	// SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
	CSGD_DirectInput*		m_pDI;
	CSGD_XAudio2*			m_pXA;

	ObjectManager* m_pOM;

	CBitmapFont*			m_pFont;

	//Tile Manager
	CTileManager*			m_pTileManager;

	// Image IDs:
	int						m_nCursorImgID;
	int						m_nBackgroundImgID;
	int						m_nBunkerImgID;
	int						m_nStoreHouseImgID;
	int						m_nMineHouseImgID;

	int						m_nNoPlacementZoneID;
	int						m_nPlacementZoneID;

	int						m_nBunkerSmallID;
	int						m_nMineHouseSmallID;
	int						m_nStoreHouseSmallID;

	int						m_nBitmapFontImgID;

	// Data Members
	int						m_nPosX;
	int						m_nPosY;

	int						m_nOffsetX;
	int						m_nOffsetY;

	int						m_nStartTileNumberX;
	int						m_nStartTileNumberY;
	int						m_nCurrentCord;

	bool					m_bIsBunkerSelected;
	bool					m_bIsStoreHouseSelected;
	bool					m_bIsMineHouseSelected;

	//Sound IDs
	int						m_nPlaceObjectSFXID;

	bool					m_bIsBunker;
	bool					m_bIsStoreHouse;
	bool					m_bIsFarm;


	CPlayer*						m_pPlayer;  

	//vector for storing the buildings placed;
	std::vector<BuildingPlacedInfo>	m_vBuildingInMapInfo;
	std::vector<BuildingPlacedInfo>	m_vBuildingBoughtInfo;

	CPurchasePlacementState(void);
	virtual ~CPurchasePlacementState(void);
	CPurchasePlacementState(const CPurchasePlacementState&);
	CPurchasePlacementState& operator=(const CPurchasePlacementState&);
};

