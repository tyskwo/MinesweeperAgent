#include "GraphicsFont.h"
#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsColor.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include <assert.h>

using namespace std;

FontID GraphicsFont::msNextID = 0;
FontContainer GraphicsFont::msFonts;

GraphicsFont::GraphicsFont(const std::string& filename, size_t fontSize)
	:mpFont( NULL )
	,mSize( fontSize )
	,mID(msNextID++)
{
	mpFont = al_load_ttf_font( filename.c_str(), fontSize, ALLEGRO_TTF_MONOCHROME );
	assert( mpFont != NULL );

	//add font to container if it doesn't exist yet
	assert( !fontExists( mID ) );
	msFonts[ mID ] = this;

}

GraphicsFont::~GraphicsFont()
{
	al_destroy_font( mpFont );

	//remove from container
	msFonts.erase( mID );
}

bool GraphicsFont::fontExists(const FontID& id)
{
	FontContainer::const_iterator iter = msFonts.find( id );
	if( iter == msFonts.end() )
	{
		return false;
	}
	else
	{
		return true;
	}
}

GraphicsFont* GraphicsFont::getFont(const FontID& id)
{
	if( fontExists( id ) )
	{
		return msFonts[ id ];
	}
	else
	{
		return NULL;
	}
}

void GraphicsFont::deleteAllFonts()
{
	while( msFonts.size() > 0 )
	{
		FontContainer::iterator iter = msFonts.begin();
		delete iter->second;
	}
}