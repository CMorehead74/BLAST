#include "MiniMap.h"


//#include "GamePlayState.h"

CMiniMap::CMiniMap(void)
{
	m_pD3D	= nullptr;
	m_pTM	= nullptr;

	//m_nBackgroundImgID = -1;

	m_pOM = ObjectManager::GetInstance();

	m_pPlayer		= nullptr;
	m_pHelicopter	= nullptr;
	m_pEnemy	    = nullptr;
	m_pAlly			= nullptr;

	m_pD3D	= CSGD_Direct3D::GetInstance();
	m_pTM	= CSGD_TextureManager::GetInstance();

	m_nBackgroundImgID = m_pTM->LoadTexture(_T("resource/graphics/MiniMap.png") );

	m_pTileManager = CTileManager::GetInstance();
	//m_pTileManager->LoadTiles("Map.xml");


	m_pHelicopter = nullptr;
	m_pAlly		  = nullptr;
	m_pEnemy	  = nullptr;
	m_pPlayer     = nullptr;
	m_pBunker	  = nullptr;
	m_pMine		  = nullptr;
	m_pFarm		  = nullptr;
	m_pShop		  = nullptr;



}


CMiniMap::~CMiniMap(void)
{

	//delete m_pHelicopter;
}


void CMiniMap::Update( float fElapsedTime )
{

}


void CMiniMap::Render()
{
	//CGamePlayState::GetInstance()->Render();

	typedef std::list<IEntity*>		ObjectList;
	typedef ObjectList::iterator	ObjListIter;

	ObjectList	m_lObjects = m_pOM->GetObjectList();



	m_pTileManager->MiniMapRender(200, 300, 0.5f,0.5f);



	for(ObjListIter iterI = m_lObjects.begin(); iterI != m_lObjects.end(); ++iterI)
	{
		if((*iterI)->GetType() == CEntity::OBJ_ALLY)
		{
			m_pAlly = dynamic_cast<CAlly*>(*iterI);
			
			(m_pAlly->MiniMapRender());
		}
		
		if((*iterI)->GetType() == CEntity::OBJ_HELICOPTER)
		{
			m_pHelicopter = dynamic_cast<CHelicopter*>(*iterI);

			m_pHelicopter->MiniMapRender();
		}
		
		if((*iterI)->GetType() == CEntity::OBJ_ENEMY)
		{

			m_pEnemy = dynamic_cast<CEnemy*>(*iterI);


			m_pEnemy->MiniMapRender();
		}

		if((*iterI)->GetType() == CEntity::OBJ_BUNKER)
		{

			m_pBunker = dynamic_cast<CBunker*>(*iterI);

			m_pBunker->MiniMapRender();
		}
	
		if((*iterI)->GetType() == CEntity::OBJ_MINE)
		{

			m_pMine = dynamic_cast<CMine*>(*iterI);

			m_pMine->MiniMapRender();
		}

		if((*iterI)->GetType() == CEntity::OBJ_STOREHOUSE)
		{

			m_pFarm = dynamic_cast<CStoreHouse*>(*iterI);

			m_pFarm->MiniMapRender();
		}

		if((*iterI)->GetType() == CEntity::OBJ_SHOP)
		{

			m_pShop = dynamic_cast<CShop*>(*iterI);

			m_pShop->MiniMapRender();
		}
	
		if((*iterI)->GetType() == CEntity::OBJ_PLAYER)
		{
			m_pPlayer = dynamic_cast<CPlayer*>(*iterI);


			m_pPlayer->MiniMapRender();
		}


	}


	//m_pHelicopter->SetImageID(m_pTM->LoadTexture(_T("resource/graphics/Blast_Helicopter.png"),D3DCOLOR_RGBA(255,255,255,155)));


	//m_pTM->Draw(m_nBackgroundImgID, 600, 500,0.25f,0.35f);
}