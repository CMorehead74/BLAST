#include "AnimationManager.h"

#include "Bullet.h"
#include "MessageSystem.h"
#include "EventManager.h"
#include "CreateBulletMessage.h"

#include "../tinyxml/tinyxml.h"
#include "../tinyxml/tinystr.h"

CAnimationManager* CAnimationManager::GetInstance( void )
{
	// Lazy Instantiation
	static CAnimationManager s_Instance;

	return &s_Instance;
}

CAnimationManager::CAnimationManager(void)
{
	m_pTM	= CSGD_TextureManager::GetInstance();

	CAnimationInfo::CAnimationInfo();


	//CEventManager::GetInstance()->RegisterClient( "shoot", this );


	//imageID		= m_pTM->LoadTexture( _T("resource/graphics/PlayerWalking.png"  ));
}

CAnimationManager::~CAnimationManager(void)
{

	//for(unsigned int i = 0; i < Animations.size(); ++i)
	//{
	//	for(unsigned int j = 0; j < Animations[j]->frames.size(); ++j)
	//	{
	//		delete Animations[i]->frames[j];
	//		Animations[i]->frames[j] = NULL;
	//	}
	//	delete Animations[i];
	//	Animations[i] = NULL;
	//}
	//Animations.clear();

	Shutdown();

	//CEventManager::GetInstance()->UnregisterClient( "shoot", this );
}

void  CAnimationManager::Shutdown()
{


	if(Animations.size() > 0)
	{
		for(unsigned int i = 0; i < Animations.size() ; ++i)
		{
			for(unsigned int j = 0; j < Animations[i]->frames.size() ; ++j)
			{
				delete Animations[i]->frames[j];
				Animations[i]->frames[j] = NULL;

			}


			delete Animations[i];
			Animations[i] = NULL;
		}
		Animations.clear();
	}

}



void CAnimationManager::Update(CAnimationInfo* pAnimInfo, float fElapsedTime)
{
	if( pAnimInfo->m_bIsPlaying == false )
		return;


	pAnimInfo->m_fTimeWaited += fElapsedTime * 1.0f;


		if(pAnimInfo->m_fTimeWaited   > Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_fFrameDuration)
		{
			pAnimInfo->m_nCurrentFrame ++;
			pAnimInfo->m_fTimeWaited = 0;





			if( (unsigned)pAnimInfo->m_nCurrentFrame >= Animations[pAnimInfo->m_nCurrentAnimation]->frames.size() )
			{
				pAnimInfo->m_nCurrentFrame = 0;


				if(pAnimInfo->m_bIsLooping == true)
				{

					//pAnimInfo->m_nCurrentFrame ++;
					//pAnimInfo->m_nCurrentFrame = 0;
				}
				else
				{
					pAnimInfo->m_bIsPlaying = false;
					pAnimInfo->m_nCurrentFrame = 0;
				}

			}

		}


	


	//if(Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_szEventName == "Shoot")
	//{
	//	//CEventManager::GetInstance()->sendEvent("shoot");

	//	//pXA->SFXPlaySound(m_nTempSfxID);

	//	//CCreateBulletMessage* pBullet = new CCreateBulletMessage( this );
	//	//CMessageSystem::GetInstance()->SendMsg( pBullet );
	//	//pBullet = nullptr;

	//}



}
void CAnimationManager::Render(CAnimationInfo* pAnimInfo, int ImgID, int PosX, int PosY, float fScaleX, float fScaleY, float fRotateX, float fRotateY, float fCurrRotate, DWORD color)
{

	int TempX = (int)(PosX - (Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_rSourceRect.right - Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_rSourceRect.left) * 0.5f);
	int TempY = (int)(PosY - (Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_rSourceRect.bottom - Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_rSourceRect.top) * 0.5f);


	RECT temp = Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_rSourceRect;

	//fRotateX = (Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_rSourceRect.left + Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_rSourceRect.right) *0.5f;
	//fRotateY = (Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_rSourceRect.top + Animations[pAnimInfo->m_nCurrentAnimation]->frames[pAnimInfo->m_nCurrentFrame]->m_rSourceRect.bottom) *0.5f;



	CSGD_TextureManager::GetInstance()->Draw(ImgID,TempX,TempY, fScaleX , fScaleY, &temp,(temp.right-temp.left)*0.5f,  (temp.bottom-temp.top)*0.5f, fCurrRotate, color);

}







bool CAnimationManager::LoadAnimationFile(const char* szFile)
{
	TiXmlDocument doc;

	if(doc.LoadFile( szFile ) == false)
	{
		return false;
	}

	TiXmlElement* pRoot = doc.RootElement();

	if( pRoot == nullptr )
	{
		return false;
	}


	TiXmlElement* pAnimation = pRoot->FirstChildElement("Animations");

	CAnimation* pAnimationObject = new CAnimation;


	while( pAnimation != NULL)
	{
		//pAnimationObject = new CAnimation();

		TiXmlElement * pFrame = pAnimation->FirstChildElement("Frames");


		while( pFrame != NULL)
		{

			tFrame* frame = new tFrame;

			TiXmlElement* pNode = pFrame->FirstChildElement( "SourceRect" );


			pNode->Attribute("RenderLeft",			(int*)&frame->m_rSourceRect.left);
			pNode->Attribute("RenderTop",			(int*)&frame->m_rSourceRect.top);
			pNode->Attribute("RenderRight",			(int*)&frame->m_rSourceRect.right);
			pNode->Attribute("RenderBottom",		(int*)&frame->m_rSourceRect.bottom);


			pNode = pNode->NextSiblingElement( "CollisionRect" );

			pNode->Attribute("CollisionLeft",		(int*)&frame->m_rCollisionRect.left);
			pNode->Attribute("CollisionTop",		(int*)&frame->m_rCollisionRect.top);
			pNode->Attribute("CollisionRight",		(int*)&frame->m_rCollisionRect.right);
			pNode->Attribute("CollisionBottom",		(int*)&frame->m_rCollisionRect.bottom);


			pNode = pNode->NextSiblingElement( "ActiveRect" );

			pNode->Attribute("ActiveLeft",			(int*)&frame->m_rActiveRect.left);
			pNode->Attribute("ActiveTop",			(int*)&frame->m_rActiveRect.top);
			pNode->Attribute("ActiveRight",			(int*)&frame->m_rActiveRect.right);
			pNode->Attribute("ActiveBottom",		(int*)&frame->m_rActiveRect.bottom);


			pNode = pNode->NextSiblingElement( "AnchorPoint" );

			pNode->Attribute("X",					(int*)&frame->m_nAnchorPointX);
			pNode->Attribute("Y",					(int*)&frame->m_nAnchorPointY);

			pNode = pNode->NextSiblingElement( "Trigger" );

			frame->m_szEventName =					pNode->Attribute("Event");

			pNode = pNode->NextSiblingElement( "FrameDuration" );

			pNode->Attribute( "Duration",          (double*)&frame->m_fFrameDuration);



			pAnimationObject->frames.push_back(frame);

			pFrame = pFrame->NextSiblingElement( "Frames" );

			//delete frame;
			//frame = nullptr;

		}


		Animations.push_back( pAnimationObject );

		pAnimation = pAnimation->NextSiblingElement( "Animations" );

	}





	return false;
}