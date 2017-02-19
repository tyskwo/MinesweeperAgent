#include "GraphicsBuffer.h"
#include "GraphicsColor.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

#include <assert.h>

BufferID GraphicsBuffer::msNextID = 0;
BufferContainer GraphicsBuffer::msBuffers;

//create buffer from thin air
GraphicsBuffer::GraphicsBuffer(size_t width, size_t height)
	:mpBitmap(NULL)
	,mWidth(width)
	,mHeight(height)
	,mID(msNextID++)
{
	mpBitmap = al_create_bitmap( mWidth, mHeight );
	assert( mpBitmap != NULL );

	//add buffer to container if it doesn't exist yet
	assert( !bufferExists( mID ) );
	msBuffers[ mID ] = this;
}

GraphicsBuffer::GraphicsBuffer(const std::string& filename)
	:mpBitmap(NULL)
	,mWidth(0)
	,mHeight(0)
	,mID(msNextID++)
{
	mpBitmap = al_load_bitmap( filename.c_str() );
	assert( mpBitmap != NULL );
	mWidth = al_get_bitmap_width( mpBitmap );
	mHeight = al_get_bitmap_height( mpBitmap );

	//add buffer to container if it doesn't exist yet
	assert( !bufferExists( mID ) );
	msBuffers[ mID ] = this;
}

GraphicsBuffer::GraphicsBuffer(const GraphicsBuffer& rhs)
	:mpBitmap(NULL)
	,mWidth(0)
	,mHeight(0)
	,mID(msNextID++)
{
	assert( rhs.mpBitmap != NULL );
	mpBitmap = al_clone_bitmap( rhs.mpBitmap );
	assert( mpBitmap != NULL );
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;

	//add buffer to container if it doesn't exist yet
	assert( !bufferExists( mID ) );
	msBuffers[ mID ] = this;
}

GraphicsBuffer::~GraphicsBuffer()
{
	al_destroy_bitmap( mpBitmap );

	//remove from container
	msBuffers.erase( mID );
}

GraphicsBuffer& GraphicsBuffer::operator=(const GraphicsBuffer& rhs)
{
	//check for self assignment
	if( this == &rhs )
	{
		return *this;
	}

	assert( rhs.mpBitmap != NULL );

	//cleanup old bitmap
	al_destroy_bitmap( mpBitmap );

	//clone new bitmap
	mpBitmap = al_clone_bitmap( rhs.mpBitmap );

	//set height and width
	mHeight = rhs.mHeight;
	mWidth = rhs.mWidth;

	return *this;
}

void GraphicsBuffer::setToColor(const GraphicsColor& color)
{
	ALLEGRO_COLOR alColor = al_map_rgba( color.r, color.g, color.b, color.a );
	ALLEGRO_BITMAP* pOld = al_get_target_bitmap();

	al_set_target_bitmap(mpBitmap);
	al_clear_to_color(alColor);
	al_set_target_bitmap( pOld );
}

bool GraphicsBuffer::bufferExists(const BufferID& id)
{
	BufferContainer::const_iterator iter = msBuffers.find( id );
	if( iter == msBuffers.end() )
	{
		return false;
	}
	else
	{
		return true;
	}
}

GraphicsBuffer* GraphicsBuffer::getBuffer(const BufferID& id)
{
	if( bufferExists( id ) )
	{
		return msBuffers[ id ];
	}
	else
	{
		return NULL;
	}
}

void GraphicsBuffer::deleteAllGraphicsBuffers()
{
	while( msBuffers.size() > 0 )
	{
		BufferContainer::iterator iter = msBuffers.begin();
		delete iter->second;
	}
}


