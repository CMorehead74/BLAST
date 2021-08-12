#include "AnimationInfo.h"


CAnimationInfo::CAnimationInfo(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();

	//m_nImageID		= m_pTM->LoadTexture( _T("resource/graphics/PlayerWalking.png" ) );
	

	m_bIsPlaying = true;
	m_nCurrentFrame = 0;
	m_fTimeWaited = 0.0f;
}


CAnimationInfo::~CAnimationInfo(void)
{

}

