/*
	File:				Emitter.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/21/2012
	Purpose:			Managing of all the Emitters happen here
*/

#include "Emitter.h"

CEmitter::CEmitter(void)
{
	m_pD3D					= CSGD_Direct3D::GetInstance();

	m_szEmitterName			= "1";

	m_nEmitterType			= RECTANGLE;
							
	m_nNoOfParticles		= 200;	
	
	m_bIsLooping			= true;
	m_bIsBlending			= true;

	m_fMinLife				= 100;
	m_fMaxLife				= 1000;	
							
	m_nMinScaleX			= 0;
	m_nMaxScaleX			= 10;
	m_nMinScaleY			= 0;
	m_nMaxScaleY			= 10;
							  
	m_fStartMinRotation		= 0.0f;
	m_fStartMaxRotation		= 90.0f;
	m_fEndMinRotation		= 45.0f;
	m_fEndMaxRotation		= 180.0f;

	m_fEmitterRotation		= 0.0f;

	m_nEmitterPosX			= 100;
	m_nEmitterPosY			= 100;
	m_nWidth				= 500;
	m_nHeight				= 500;
					
	m_nOuterRingRadius		= 0;

	m_fEmissionRate			= 1;
	m_fScaleRate			= 1;
	m_fColorRate			= 1;
	m_fRotationRate			= 1;
	m_fVelXRate				= 1;
	m_fVelYRate				= 1;

	m_nMinVelX				= 1;
	m_nMaxVelX				= 10;
	m_nMinVelY				= 1;
	m_nMaxVelY				= 10;	
							  
	m_fGravityX				= 0;
	m_fGravityY				= 0;
	m_fWind					= 0;

	m_nMinA					= 0;
	m_nMinR					= 0;
	m_nMinG					= 0;
	m_nMinB					= 0;
							  
	m_nMaxA					= 255;
	m_nMaxR					= 255;
	m_nMaxG					= 255;
	m_nMaxB					= 255;	

	m_nSourceBlend			= 4;
	m_nDestBlend			= 2;
							  
	m_szParticleImage		= "resource/graphics/A_Particle_Emission.png";
	m_szEmitterBgImage		= "";

	m_nParticleID			= -1;
	m_nBlendID				= -1;

	m_bFinished				= false;
	m_nTotalParticlesCreated= 0;

	m_vActiveParticles.clear();
	m_vDeadParticles.clear();
}

CEmitter::CEmitter(int noOfParticles,EMITTERTYPE type)
{
	
}

CEmitter::~CEmitter(void)
{
	m_vActiveParticles.clear();
	//delete &m_vActiveParticles;
	m_vDeadParticles.clear();
	//delete &m_vDeadParticles;
}

void CEmitter::AddParticle()
{	
	RECT dimension = {(LONG)m_nEmitterPosX,(LONG)m_nEmitterPosY,(LONG)m_nWidth,(LONG)m_nHeight};
	RECT minColor = {(LONG)m_nMinA,(LONG)m_nMinR,(LONG)m_nMinG,(LONG)m_nMinB};
	RECT maxColor = {(LONG)m_nMaxA,(LONG)m_nMaxR,(LONG)m_nMaxG,(LONG)m_nMaxB};
	if (m_nEmitterType == RECTANGLE)
	{
		//int test = rand()%((int)(abs(m_nMinVelY) + m_nMaxVelY)) + m_nMinVelY;
		CParticle* particle = new CParticle(
			0,
			0,
			dimension,
			m_nEmitterType,
			rand()%(m_nWidth+1 ) + m_nEmitterPosX,
			rand()%(m_nHeight+1) + m_nEmitterPosY,
			m_fWind,
			((m_nMinVelX + m_nMaxVelX) == 0)? 0 : ( (m_nMinVelX == m_nMaxVelX)?m_nMinVelX:(rand()%((int)(abs(m_nMinVelX) + m_nMaxVelX)) + m_nMinVelX) ),
			((m_nMinVelY + m_nMaxVelY) == 0)? 0 : ( (m_nMinVelY == m_nMaxVelY)?m_nMinVelY:(rand()%((int)(abs(m_nMinVelY) + m_nMaxVelY)) + m_nMinVelY) ),
			(m_nMaxScaleX == m_nMinScaleX)? m_nMaxScaleX : (rand()%((int)(m_nMaxScaleX - m_nMinScaleX+1))+m_nMinScaleX),
			(m_nMaxScaleY == m_nMinScaleY)? m_nMaxScaleY : (rand()%((int)(m_nMaxScaleY - m_nMinScaleY+1))+m_nMinScaleY),
			m_nMaxScaleX,
			m_nMaxScaleY,
			minColor,
			maxColor, 
			(m_fStartMaxRotation == m_fStartMinRotation)?m_fStartMaxRotation:(rand()%((int)(m_fStartMaxRotation - m_fStartMinRotation+1))+m_fStartMinRotation),
			(m_fEndMaxRotation == m_fEndMinRotation)?m_fEndMaxRotation:(rand()%((int)(m_fEndMaxRotation - m_fEndMinRotation+1))+m_fEndMinRotation),
			(m_fMaxLife == m_fMinLife)? m_fMinLife : (rand()%((int)(m_fMaxLife - m_fMinLife+1)) + m_fMinLife),
			m_fMaxLife,
			m_fScaleRate,
			m_fColorRate,
			m_fRotationRate,
			m_fVelXRate,
			m_fVelYRate,
			m_nParticleID
			);
		m_vActiveParticles.push_back(*particle);

		delete particle;
		particle = nullptr;
	}
	else if (m_nEmitterType == CIRCLE)
	{
		float a = m_nWidth * 0.5f;
		float b = m_nHeight * 0.5f;
		double x;
		double y;
		double angle = rand()%361;
		x = (a * b) / sqrt(b * b + a * a * tan(angle) * tan(angle));
		y = (a * b * tan(angle)) / sqrt(b * b + a * a * tan(angle) * tan(angle));
	
		if(angle > 90 && angle < 270)
		{
			x = m_nEmitterPosX-x;
			y = m_nEmitterPosY-y;
		}
		else
		{
			x = m_nEmitterPosX + x;
			y = m_nEmitterPosX + y;
		}

		CParticle* particle = new CParticle(
			(float)angle, 
			(float)m_nOuterRingRadius,
			dimension,
			m_nEmitterType, 
			(int)x,
			(int)y,
			m_fWind,
			((m_nMinVelX + m_nMaxVelX) == 0)? 0 : ( (m_nMinVelX == m_nMaxVelX)?m_nMinVelX:(rand()%((int)(abs(m_nMinVelX) + m_nMaxVelX)) + m_nMinVelX) ),
			((m_nMinVelY + m_nMaxVelY) == 0)? 0 : ( (m_nMinVelY == m_nMaxVelY)?m_nMinVelY:(rand()%((int)(abs(m_nMinVelY) + m_nMaxVelY)) + m_nMinVelY) ),
			(m_nMaxScaleX == m_nMinScaleX)? m_nMaxScaleX : (rand()%((int)(m_nMaxScaleX - m_nMinScaleX+1))+m_nMinScaleX),
			(m_nMaxScaleY == m_nMinScaleY)? m_nMaxScaleY : (rand()%((int)(m_nMaxScaleY - m_nMinScaleY+1))+m_nMinScaleY),
			m_nMaxScaleX,
			m_nMaxScaleY,
			minColor,
			maxColor, 
			(m_fStartMaxRotation == m_fStartMinRotation)?m_fStartMaxRotation:(rand()%((int)(m_fStartMaxRotation - m_fStartMinRotation+1))+m_fStartMinRotation),
			(m_fEndMaxRotation == m_fEndMinRotation)?m_fEndMaxRotation:(rand()%((int)(m_fEndMaxRotation - m_fEndMinRotation+1))+m_fEndMinRotation),
			(m_fMaxLife == m_fMinLife)? m_fMinLife : (rand()%((int)(m_fMaxLife - m_fMinLife+1)) + m_fMinLife),			
			m_fMaxLife,
			m_fScaleRate,
			m_fColorRate,
			m_fRotationRate,
			m_fVelXRate,
			m_fVelYRate,
			m_nParticleID
			);
		m_vActiveParticles.push_back(*particle);

		delete particle;
		particle = nullptr;
	}
	else if (m_nEmitterType == LINE)
	{
		CParticle* particle = new CParticle(
			0,
			0,
			dimension,
			m_nEmitterType,
			rand()%(m_nWidth) +m_nEmitterPosX,
			m_nEmitterPosY,
			m_fWind,
			((m_nMinVelX + m_nMaxVelX) == 0)? 0 : ( (m_nMinVelX == m_nMaxVelX)?m_nMinVelX:(rand()%((int)(abs(m_nMinVelX) + m_nMaxVelX)) + m_nMinVelX) ),
			((m_nMinVelY + m_nMaxVelY) == 0)? 0 : ( (m_nMinVelY == m_nMaxVelY)?m_nMinVelY:(rand()%((int)(abs(m_nMinVelY) + m_nMaxVelY)) + m_nMinVelY) ),
			(m_nMaxScaleX == m_nMinScaleX)? m_nMaxScaleX : (rand()%((int)(m_nMaxScaleX - m_nMinScaleX+1))+m_nMinScaleX),
			(m_nMaxScaleY == m_nMinScaleY)? m_nMaxScaleY : (rand()%((int)(m_nMaxScaleY - m_nMinScaleY+1))+m_nMinScaleY),
			m_nMaxScaleX,
			m_nMaxScaleY,
			minColor,
			maxColor, 
			(m_fStartMaxRotation == m_fStartMinRotation)?m_fStartMaxRotation:(rand()%((int)(m_fStartMaxRotation - m_fStartMinRotation+1))+m_fStartMinRotation),
			(m_fEndMaxRotation == m_fEndMinRotation)?m_fEndMaxRotation:(rand()%((int)(m_fEndMaxRotation - m_fEndMinRotation+1))+m_fEndMinRotation),
			(m_fMaxLife == m_fMinLife)? m_fMinLife : (rand()%((int)(m_fMaxLife - m_fMinLife+1)) + m_fMinLife),
			m_fMaxLife,
			m_fScaleRate,
			m_fColorRate,
			m_fRotationRate,
			m_fVelXRate,
			m_fVelYRate,
			m_nParticleID
			);

		m_vActiveParticles.push_back(*particle);

		delete particle;
		particle = nullptr;
	}
	
}

void CEmitter::Update(float fElapsedTime)
{
	//int count = 0;
	for( std::vector< CParticle >::iterator it = m_vActiveParticles.begin(); it != m_vActiveParticles.end();  )
	{
		(*it).Update(fElapsedTime);
		if( (*it).GetCurrentLife() > m_fMaxLife)
		{
			it = m_vActiveParticles.erase(it);
		}
		else
			it++;
		
	//	POINT p;
	//	GetCursorPos(&p);
	//	ScreenToClient(CGame::GetInstance()->GetHwnd(), &p);

	//	//Set the current life to zero
	//	int degree = rand()%360;
	//	if ((*it).GetCurrentLife()<(*it).GetMyLife())
	//	{
	//		//p.x += 1;
	//		//p.y += 1;
	//		if(m_nEmitterType == RECTANGLE)
	//		{
	//			(*it).SetParticlePosX( (p.x+rand()%m_nWidth) - m_nWidth*0.5f);
	//			(*it).SetParticlePosY( (p.y+rand()%m_nHeight) - m_nHeight*0.5f);
	//		}
	//		else if(m_nEmitterType == CIRCLE)
	//		{
	//			(*it).SetParticlePosX(p.x+(float)std::cos(3.14f * 2 * degree/360)*50);
	//			(*it).SetParticlePosY(p.y+(float)((std::sin(3.14 * 2 * degree/360))-0.75f)*50);
	//		}
	//		(*it).SetCurrentLife((*it).GetCurrentLife()+1.0f);
	//	}
	//	else
	//	{
	//		it = m_vActiveParticles.erase( it );
	//	}
	//}
	////while( m_vActiveParticles.size() != 100 )
	////{
	////	
	////}
	//
	////else if(m_nEmitterType == STARS)
	////{
	////	while( m_vActiveParticles.size() != 100 )
	////	{
	////		m_vActiveParticles.erase( m_vActiveParticles.begin() );
	////	}
	////}
	//
	//CreateParticle( 1 );
	}
	
	if (!m_bIsLooping)
	{
		if (m_bFinished)
			return;
	}
	else
	{
		m_bFinished = false;
		m_nTotalParticlesCreated = 0;
	}

	if(m_vActiveParticles.size() < (unsigned int)m_nNoOfParticles)
	{
		for (int i = 0; i < m_fEmissionRate; ++i)
		{
			AddParticle();

			if (!m_bIsLooping)
			{
				m_nTotalParticlesCreated++;
				if(m_nTotalParticlesCreated >= m_nNoOfParticles)
					m_bFinished = true;	
			}

			if (m_vActiveParticles.size() >= (unsigned int)m_nNoOfParticles)
			{							
				break;
			}                    
		}
	}
}

void CEmitter::Render()
{	
	DWORD temp1 = 0, temp2 = 0;
	m_pD3D->GetDirect3DDevice()->GetRenderState(D3DRS_SRCBLEND, &temp1 );
	m_pD3D->GetDirect3DDevice()->GetRenderState(D3DRS_DESTBLEND, &temp2 );
	m_pD3D->SpriteEnd();

	m_pD3D->SpriteBegin();
	m_pD3D->GetDirect3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE,  TRUE );
	m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	if(m_bIsBlending)
	{
		if (m_nSourceBlend != -1)
		{
			switch (m_nSourceBlend)
			{
			case 0:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
				break;
			case 1:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
				break;
			case 2:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
				break;
			case 3:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCCOLOR);
				break;
			case 4:
				m_pD3D->GetDirect3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
				break;
			case 5:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCALPHA);
				break;
			case 6:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHASAT);
				break;
			case 7:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BOTHSRCALPHA);
				break;
			case 8:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BOTHINVSRCALPHA);
				break;
			case 9:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BLENDFACTOR);
				break;
			case 10:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVBLENDFACTOR);
				break;
			}
		}

		if (m_nDestBlend != -1)
		{
			switch (m_nDestBlend)
			{
			case 0:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTCOLOR);
				break;
			case 1:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTCOLOR);
				break;
			case 2:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTALPHA);
				break;
			case 3:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVDESTALPHA);
				break;
			case 4:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ZERO);
				break;
			case 5:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
				break;
			case 6:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
				break;
			case 7:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCCOLOR);
				break;
			case 8:
				m_pD3D->GetDirect3DDevice()->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
				break;
			case 9:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVSRCALPHA);
				break;
			case 10:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHASAT);
				break;
			case 11:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BOTHSRCALPHA);
				break;
			case 12:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BOTHINVSRCALPHA);
				break;
			case 13:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_BLENDFACTOR);
				break;
			case 14:
				m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_INVBLENDFACTOR);
				break;
			}
		}

	}

	for( std::vector< CParticle >::iterator it = m_vActiveParticles.begin(); it != m_vActiveParticles.end(); it++ )
	{
		if(m_fEmitterRotation == 0)
			(*it).Render();
		else
		{
			(*it).RenderWithRotation(m_fEmitterRotation);
			//(*it).RenderRotation( );
		}
	}
	

	m_pD3D->SpriteEnd();
	
	m_pD3D->SpriteBegin();
	/*m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_SRCBLEND, temp1 );
		m_pD3D->GetDirect3DDevice()->SetRenderState(D3DRS_DESTBLEND, temp2 );*/
	//m_pD3D->GetDirect3DDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE,  FALSE );


	
}
