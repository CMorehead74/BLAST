/*
	File:				ParticleManger.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/21/2012
	Purpose:			Particle manager to manage emitters
*/
#include "ParticleManager.h"
#include "Camera.h"

#include "../tinyxml/tinyxml.h"
#include <string>

/*static*/ CParticleManager* CParticleManager::GetInstance( void )
{
	// Local static variable:
	//	- stored in global memory
	//	- there can be only one!
	static CParticleManager s_Instance;

	return &s_Instance;
}

CParticleManager::CParticleManager(void)
{
	m_pTM = CSGD_TextureManager::GetInstance();

	//m_bDirectionChanged = false;
	//m_nParticleImageID  = -1;
	//m_coneRadius = 10;
	//m_Distance	 = 2.5;
	//m_Direction	 = 0.5;
}


CParticleManager::~CParticleManager(void)
{
	
	m_vEmitter.clear();
	//delete m_vEmitter;
	//m_vEmitter = nullptr;
}

CParticleManager::CParticleManager(float parentPosX, float parentPosY,float rotation,PARTICLETYPE pType)
{
	//m_bDirectionChanged = false;
	//m_nParticleImageID  = -1;
	//m_nParentX = parentPosX;
	//m_nParentY = parentPosY;
	//m_fRotation = rotation;
	//m_ePType	= pType;
	//
	//if(m_ePType == EMISSION)
	//	m_nParticleImageID = m_pTM->LoadTexture(_T("resource/graphics/Blast_Particle_Aiming.png"), D3DCOLOR_XRGB(0,0,0)); //D3DCOLOR_XRGB(0,0,0) color key to ignore that color
	//else if(m_ePType == STARS)
	//	m_nParticleImageID = m_pTM->LoadTexture(_T("resource/graphics/A_Particle_Star.png"), D3DCOLOR_XRGB(0,0,0)); //D3DCOLOR_XRGB(0,0,0) color key to ignore that color
	//
	//m_coneRadius = 180;
	//m_Distance	 = 1.5;
	//m_Direction	 = 0.5;
}

bool CParticleManager::RegisterEmitter(std::string fileName)
{
	// Create a TinyXML Document
	TiXmlDocument doc;
	std::string temp = "Config/";
	fileName = temp.append(fileName);
	// Attempt to load the file
	if( doc.LoadFile( fileName.c_str() ) == false )
		return false;


	// Access the root node ("Particles_List")
	TiXmlElement* pRoot = doc.RootElement();

	if( pRoot == nullptr )
		return false;


	// Iterate through the nodes to load Emitter info
	TiXmlElement* pEmitter = pRoot->FirstChildElement( "Emitter" );

	while( pEmitter != nullptr )
	{		
		// Info structure
		CEmitter emitter;
		double fValue = 0.0f;
		int nValue = 0;
		std::string szCondition = "";

		szCondition = pEmitter->Attribute( "name");
		emitter.SetEmitterName(szCondition);
		//emitter.
		// Read the attributes
		pEmitter->Attribute( "xPos", &nValue );
		emitter.SetEmitterPosX(nValue);
		pEmitter->Attribute( "yPos", &nValue );
		emitter.SetEmitterPosY(nValue);
		pEmitter->Attribute( "width", &nValue );
		emitter.SetEmitterWidth(nValue);
		pEmitter->Attribute( "height", &nValue );
		emitter.SetEmitterHeight(nValue);

		pEmitter->Attribute( "OuterRadius", &nValue );
		emitter.SetOuterRingRadius(nValue);

		pEmitter->Attribute( "NoOfParticles", &nValue );
		emitter.SetNoOfParticles(nValue);
		pEmitter->Attribute( "minVelX", &fValue );
		emitter.SetMinVelX((float)fValue);
		pEmitter->Attribute( "maxVelX", &fValue );
		emitter.SetMaxVelX((float)fValue);
		pEmitter->Attribute( "minVelY", &fValue );
		emitter.SetMinVelY((float)fValue);
		pEmitter->Attribute( "maxVelY", &fValue );
		emitter.SetMaxVelY((float)fValue);
		pEmitter->Attribute( "Minlife", &fValue );
		emitter.SetMinLife((float)fValue);

		pEmitter->Attribute( "Maxlife", &fValue );
		emitter.SetMaxLife((float)fValue);

		pEmitter->Attribute( "ScaleMinX", &fValue );
		emitter.SetMinScaleX((int)fValue);
		pEmitter->Attribute( "ScaleMaxX", &fValue );
		emitter.SetMaxScaleX((int)fValue);
		pEmitter->Attribute( "ScaleMinY", &fValue );
		emitter.SetMinScaleY((int)fValue);
		pEmitter->Attribute( "ScaleMaxY", &fValue );
		emitter.SetMaxScaleY((int)fValue);
		pEmitter->Attribute( "RotationStartMin", &fValue );
		emitter.SetStartMinRotation((float)fValue);
		pEmitter->Attribute( "RotationStartMax", &fValue );
		emitter.SetStartMaxRotation((float)fValue);
		pEmitter->Attribute( "RotationEndMin", &fValue );
		emitter.SetEndMinRotation((float)fValue);
		pEmitter->Attribute( "RotationEndMax", &fValue );
		emitter.SetEndMaxRotation((float)fValue);
		
		pEmitter->Attribute( "ColorMinA", &nValue );
		emitter.SetMinA(nValue);
		pEmitter->Attribute( "ColorMinR", &nValue );
		emitter.SetMinR(nValue);
		pEmitter->Attribute( "ColorMinG", &nValue );
		emitter.SetMinG(nValue);
		pEmitter->Attribute( "ColorMinB", &nValue );
		emitter.SetMinB(nValue);
		pEmitter->Attribute( "ColorMaxA", &nValue );
		emitter.SetMaxA(nValue);
		pEmitter->Attribute( "ColorMaxR", &nValue );
		emitter.SetMaxR(nValue);
		pEmitter->Attribute( "ColorMaxG", &nValue );
		emitter.SetMaxG(nValue);
		pEmitter->Attribute( "ColorMaxB", &nValue );
		emitter.SetMaxB(nValue);		

		pEmitter->Attribute( "ScaleRate", &fValue );
		emitter.SetScaleRate((float)fValue);
		pEmitter->Attribute( "EmissionRate", &fValue );
		emitter.SetEmissionRate((float)fValue);
		pEmitter->Attribute( "ColorRate", &fValue );
		emitter.SetColorRate((float)fValue);
		pEmitter->Attribute( "RotationRate", &fValue );
		emitter.SetRotationRate((float)fValue);
		pEmitter->Attribute( "velocityXRate", &fValue );
		emitter.SetVelXRate((float)fValue);
		pEmitter->Attribute( "velocityYRate", &fValue );
		emitter.SetVelYRate((float)fValue);

		pEmitter->Attribute( "Type", &nValue );
		emitter.SetEmitterType(nValue);

		szCondition = pEmitter->Attribute("IsLooping");
		if( strcmp(szCondition.c_str(),"true") == 0 || strcmp(szCondition.c_str(),"TRUE") == 0 )
			emitter.SetIsLooping(true);
		else if( strcmp(szCondition.c_str(),"false") == 0 || strcmp(szCondition.c_str(),"FALSE") == 0 )
			emitter.SetIsLooping(false);
		
		szCondition = "";
		szCondition = pEmitter->Attribute("IsBlending");
		if( strcmp(szCondition.c_str(),"true") == 0 || strcmp(szCondition.c_str(),"TRUE") == 0 )
			emitter.SetIsBlending(true);
		else if( strcmp(szCondition.c_str(),"false") == 0 || strcmp(szCondition.c_str(),"FALSE") == 0 )
			emitter.SetIsLooping(false);

		
		pEmitter->Attribute( "SourceBlend", &nValue );
		emitter.SetSourceBlend(nValue);

		pEmitter->Attribute( "DestBlend", &nValue );
		emitter.SetDestBlend(nValue);

		emitter.SetParticleImageName(pEmitter->Attribute("imageID"));
		TCHAR temp[256];
		unsigned int count = 0;
		for(; count<emitter.GetParticleImageName().size(); count++)
		{
			temp[count] = emitter.GetParticleImageName()[count];
		}
		temp[count] = '\0';
		emitter.SetParticleImgID(m_pTM->LoadTexture(temp,D3DCOLOR_XRGB(0, 0, 0)));
		//m_vEmitter.push_back(emitter);

		//registering all the emitters in map
		m_mAllEmitter[emitter.GetEmitterName()] = emitter;
		m_vEmitter.push_back(emitter);
		// Move to the next "Enemy" node
		pEmitter = pEmitter->NextSiblingElement( "Emitter" );		
	}
	return true;
}

void CParticleManager::Update(float fElapsedTime)
{	
	for( int count = 0; count != m_vEmitter.size(); ++count )
	{
		m_vEmitter[count].Update(fElapsedTime);
	}
}

void CParticleManager::Render(std::string emitterName, int x, int y, float rotation)
{
	for( int count = 0; count != m_vEmitter.size(); ++count )
	{
		if(m_vEmitter[count].GetEmitterName() == emitterName)
		{
			m_vEmitter[count].SetEmitterPosX(x);
			m_vEmitter[count].SetEmitterPosY(y);
			m_vEmitter[count].SetEmitterRotation(rotation);

			if(m_vEmitter[count].GetParticleImgID() != -1)
				m_vEmitter[count].Render();
		}
		/*else
			m_pTM->GetInstance()->LoadTexture(m_vEmitter[count].m_nParticleImage,1);*/
	}
}

CEmitter* CParticleManager::CreateEmitter(std::string szRegistedEmitterName)
{
	std::map<std::string, CEmitter>::iterator iter = m_mAllEmitter.find(szRegistedEmitterName);
	CEmitter* emitter = &(*iter).second;
	//CEmitter* emitter = nullptr;
	m_vEmitter.push_back(*emitter);
	//return emitter;
	return emitter;
}


bool CParticleManager::StartMachineGunnerBulletParticle(void)
{
	return true;
}

bool CParticleManager::StartSniperBulletParticle(void)
{
	return true;
}

bool CParticleManager::StartRocketLauncherBulletParticle(void)
{
	return true;
}

bool CParticleManager::StartMedicBulletParticle(void)
{
	return true;
}

bool CParticleManager::StartSelectionParticle(void)
{
	return true;
}

bool CParticleManager::StartLevelUpParticle(void)
{
	return true;
}

bool CParticleManager::StartSnowParticle(void)
{
	return true;
}

bool CParticleManager::StartRainParticle(void)
{
	return true;
}

bool CParticleManager::StartHelicopterParticle(void)
{
	return true;
}
