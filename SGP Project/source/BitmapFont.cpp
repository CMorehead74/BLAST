/*
	File:						BitmapFont.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/14/2012
	Purpose:				display text using a bitmap font
*/

#include "BitmapFont.h"
#include "../tinyxml/tinyxml.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"

CBitmapFont* CBitmapFont::GetInstance()
{
	static CBitmapFont s_Instance;
	return &s_Instance;
}

CBitmapFont::CBitmapFont(void)
{
	m_szImageName = nullptr;
	LoadFont("SansationFont.xml");
}

CBitmapFont::~CBitmapFont(void)
{
	//for( int i = 0; i = m_vCharacterContainer.size(); i++ )
	//	delete m_vCharacterContainer[i];
	/*for( int i = 0; i == m_vKerningInfoContainer.size(); i++ )
		delete m_vKerningInfoContainer[i];*/

	m_vCharacterContainer.clear();
	m_vKerningInfoContainer.clear();

}

void CBitmapFont::PrintString( const char* szString, int nX, int nY, float fScale /*= 1.0f*/, short nAlpha /*= 255*/, short nRed /*= 255*/, short nGreen /*= 255*/, short nBlue /*= 255 */ )
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	if( szString == nullptr )
		return;
	if( m_vCharacterContainer.size() == 0 )
		return;
	
	int strLength = strlen( szString );

	for( int i = 0; i < strLength; i++ )
	{
		char Letters = szString[i];

		for( int i = 0; i < (int)m_vCharacterContainer.size() ; i++ )
		{
			if( m_vCharacterContainer[i].m_sCharID == Letters )
			{
				RECT rSource;
				rSource.left = m_vCharacterContainer[i].m_nLeft;
				rSource.right = m_vCharacterContainer[i].m_nWidth + rSource.left;
				rSource.top = m_vCharacterContainer[i].m_nTop;
				rSource.bottom = m_vCharacterContainer[i].m_nHeight + rSource.top;

				pTM->Draw( m_nImageID, (int)(nX + m_vCharacterContainer[i].m_nXOffset * fScale), (int)(nY +  m_vCharacterContainer[i].m_nYOffset * fScale), fScale, fScale, &rSource, 0.0f, 0.0f, 0.0, D3DCOLOR_ARGB( nAlpha, nRed, nGreen, nBlue) );

				nX += (int)(m_vCharacterContainer[i].m_nXAdvance * fScale);
			}
		}
	}
}

bool CBitmapFont::LoadFont( const char* szFileName )
{
	TiXmlDocument doc;

	if( doc.LoadFile( szFileName ) == false )
		return false;

	TiXmlElement* pRoot = doc.RootElement();

	if( pRoot == nullptr )
		return false;

	//info section of the xml file
	TiXmlElement* pFontElement = pRoot->FirstChildElement( "info" );

	while( pFontElement != nullptr )
	{
		m_szFontName = pFontElement->Attribute("face");
		pFontElement->Attribute("size", &m_nFontSize);

		pFontElement = pFontElement->NextSiblingElement( "info");
	}

	//page section of the xml
	TiXmlElement* pPageElement = pRoot->FirstChildElement( "pages" );
	TiXmlElement* pPageAttribute = pPageElement->FirstChildElement( "page" );

	while( pPageElement != nullptr )
	{
		m_szImageName = pPageAttribute->Attribute("file");
		
		pPageElement = pPageElement->NextSiblingElement( "pages" );
	}

	TiXmlElement* pCharElement = pRoot->FirstChildElement( "chars" );
	TiXmlElement* pCharAttributes = pCharElement->FirstChildElement( "char" );

	//TiXmlElement* pCharElement = pRoot->FirstChildElement( "char" )
	//chars section of the xml
	//looping through and filling in the data members
	while( pCharAttributes != nullptr )
	{
		Characters charset;

		pCharAttributes->Attribute("id", &charset.m_sCharID );
		pCharAttributes->Attribute("x", &charset.m_nLeft );
		pCharAttributes->Attribute("y", &charset.m_nTop );
		pCharAttributes->Attribute("width", &charset.m_nWidth );
		pCharAttributes->Attribute("height", &charset.m_nHeight );
		pCharAttributes->Attribute("xoffset", &charset.m_nXOffset );
		pCharAttributes->Attribute("yoffset", &charset.m_nYOffset );
		pCharAttributes->Attribute("xadvance", &charset.m_nXAdvance );

		//storing everything in the vector
		m_vCharacterContainer.push_back( charset );
		pCharAttributes = pCharAttributes->NextSiblingElement( "char" );
	}

	 	//kerning section of the xml
	 		//TiXmlElement* pKerningElement = pRoot->FirstChildElement( "kernings" );
	 		//TiXmlElement* pKerningAttributes = pKerningElement->FirstChildElement( "kerning" );
	 	
	 		//while( pKerningAttributes != nullptr )
	 		//{
	 		//	KerningInfo Kerning;
	 	
	 		//	pKerningAttributes->Attribute( "first", &Kerning.m_nFirst );
	 		//	pKerningAttributes->Attribute( "second", &Kerning.m_nSecond );
	 		//	pKerningAttributes->Attribute( "amount", &Kerning.m_nAmount );
	 	
	 		//	m_vKerningInfoContainer.push_back( &Kerning );
	 	
	 		//	//if( 75 == m_vKerningInfoContainer.size() )
	 		//		//break;
	 	
	 		//	pKerningAttributes = pKerningAttributes->NextSiblingElement( "kerning" );
	 		//}

	return true;
}