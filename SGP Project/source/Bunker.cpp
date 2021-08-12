#include "Bunker.h"
#include "MessageSystem.h"
#include "Bullet.h"
#include "DestroyBunkerMessage.h"
#include "DestroyBulletMessage.h"
#include "EventManager.h"
#include "Camera.h"
#include "BitmapFont.h"
#include "Ally.h"

#include "FollowState.h"

#include "../SGD Wrappers/SGD_String.h"

CBunker::CBunker(void)
{
	m_eType = OBJ_BUNKER;
	m_nNumAllies = 0;
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pOM = ObjectManager::GetInstance();
	
	m_nBunkerID = -1;
	SetHealth(200);
	m_nBunkerImgID = m_pTM->LoadTexture(_T("resource/graphics/Bunker.png"), D3DCOLOR_XRGB( 255, 255, 255 ));
	SetHeight(128);
	SetWidth(128);
	m_nHealthID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/HealthBarFill.jpg"));
	m_nMaxHealth = 200;
}


CBunker::~CBunker(void)
{
}

void CBunker::Update( float fElapsedTime )
{
	if( GetHealth() <= 0 )
	{
		list<IEntity*> objList = m_pOM->GetObjectList();
		for(std::list<IEntity*>::iterator iter = objList.begin();
			iter != objList.end(); ++iter)
		{
			if((*iter)->GetType() == OBJ_ALLY)
			{
				CAlly* pAlly = dynamic_cast<CAlly*>(*iter);
				if(pAlly->GetBunkeredState())
				{
					pAlly->SetBunkerID(-1);
					pAlly->LeaveBunker();
					pAlly->SetState(new CFollowState);
				}

			}
		}

		CDestroyBunkerMessage* pBunker = new CDestroyBunkerMessage( this );
		CMessageSystem::GetInstance()->SendMsg( pBunker );
		pBunker = nullptr;
	}
}

void CBunker::Render()
{
	RECT rsource = { 0, 0, 128, 128 };
	m_pTM->Draw(m_nBunkerImgID, (int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1.0, 1.0, &rsource );

	// Draw Health Bar
	float hp = (float)GetHealth()/(float)GetMaxHealth();

	ostringstream output;
	output << m_nNumAllies<<"/4";
	CBitmapFont::GetInstance()->PrintString(output.str().c_str(), (int)(GetPosX() - CCamera::GetInstance()->GetX() + GetWidth()*0.5f), (int)(GetPosY() - CCamera::GetInstance()->GetY() + GetHeight()*0.5f + 10), 0.75f, 255, 255, 0, 0);
	//if(hp != 1)
	//	hp = hp;
	if(hp > 0)
	{
		RECT rHealthBar = { 0, 0, (LONG)(256 * hp), 32 };
		if(hp > 0 && hp <= 0.15f)
			m_pTM->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.50f, 0.20f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
		else if(hp > 0.15f && hp < 0.3f)
			m_pTM->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.50f, 0.20f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 0));
		else if(hp > 0.3f && hp <= 1.f)
			m_pTM->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.50f, 0.20f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
	}
}


void CBunker::MiniMapRender()
{
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	RECT rSource = {0, 0, 128, 128};

	pTm->Draw(m_nBunkerImgID, (int)((GetPosX() * 0.0365f) +606), (int)((GetPosY()  * 0.0365f) +406), 0.08f, 0.08f, &rSource, 0,0,0, D3DCOLOR_RGBA(255,255,255,155));
}


bool CBunker::CheckCollision( IEntity* pOther )
{
	CMessageSystem* pMS = CMessageSystem::GetInstance();

	if(CEntity::CheckCollision(pOther) == false)
		return false;

	switch(pOther->GetType())
	{
	case OBJ_BULLET:
		{
			CBullet* pBullet = dynamic_cast<CBullet*>(pOther);
			CEventManager*	pEM	=CEventManager::GetInstance();


			if( pBullet->GetOwner()->GetType() == OBJ_ENEMY )
			{
				if(pBullet->GetBulletType() == ROCKET)
					pEM->sendEvent("collide_Rocket");
				else
					pEM->sendEvent("collide_Bullet");
				SetHealth( GetHealth()-pBullet->GetOwner()->GetDamage());


				CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(pBullet);
				pMS->SendMsg(pMsg);
				pMsg = nullptr;

			}
		}
		break;
	}
	return true;
}

void CBunker::Evacuate()
{

}
