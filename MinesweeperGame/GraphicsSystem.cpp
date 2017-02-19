#include "GraphicsSystem.h"
#include "GraphicsBuffer.h"
#include "GraphicsFont.h"
#include "GraphicsColor.h"
#include "GraphicsSprite.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <assert.h>

GraphicsSystem* GraphicsSystem::mspInstance = NULL;

GraphicsSystem::GraphicsSystem()
	:mWidth( 0 )
	,mHeight( 0 )
	,mFullScreen( false )
	,mpDisplay( NULL )
	,mpFont( NULL )
{
}

bool GraphicsSystem::init(UINT width, UINT height, bool fullscreen)
{
	if( mpDisplay != NULL )
	{
		cleanup();
	}

	mWidth = width;
	mHeight = height;
	mFullScreen = fullscreen;

	//initialize systems
	if (!al_init())
	{
		std::cout << "This could be a problem... Allegro didn't initialize properly.\n";
		return false;
	}

	if (!al_init_image_addon())
	{
		std::cout << "This could be a problem... Allegro couldn't initialize the image addon.\n";
		return false;
	}

	al_init_font_addon();

	if (!al_init_ttf_addon())
	{
		std::cout << "This could be a problem... Allegro couldn't initialize the ttf addon.\n";
		return false;
	}

	if (!al_init_primitives_addon())
	{
		std::cout << "This could be a problem... Allegro couldn't initialize the primitives addon.\n";
		return false;
	}

	if(!al_install_mouse())
	{
		std::cout << "This could be a problem... Allegro couldn't install the mouse.\n";
		return false;
	}

	if(!al_install_keyboard())
	{
		std::cout << "This could be a problem... Allegro couldn't install the mouse.\n";
		return false;
	}

	if( mFullScreen )
	{
		al_set_new_display_flags( ALLEGRO_FULLSCREEN );
	}
	else
	{
		al_set_new_display_flags( ALLEGRO_WINDOWED );
	}

	//create the display
	if ( !( mpDisplay = al_create_display( mWidth, mHeight ) ) )
	{
		std::cout << "This could be a problem... Allegro couldn't allocate a display.\n";
		return false;
	}

	al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

	mpFont = new GraphicsFont( "assets/cour.ttf", 24 );

	return true;
}

void GraphicsSystem::cleanup()
{
	if( mpDisplay != NULL )
	{
		delete mpFont;
		mpFont = NULL;

		al_destroy_display(mpDisplay);
		mpDisplay = NULL;
		al_uninstall_system();
	}
}

void GraphicsSystem::deleteGraphicsSystem()
{
	GraphicsBuffer::deleteAllGraphicsBuffers();
	
	GraphicsFont::deleteAllFonts();
	mspInstance->mpFont = NULL;

	delete mspInstance;
	mspInstance = NULL;
}

void GraphicsSystem::drawEntireBuffer(GraphicsBuffer* pSrc, UINT dx, UINT dy, int flags /*= 0 */, GraphicsBuffer* pDest /*= NULL */)
{
	cacheOrRestoreTargetBitmap( true );

	ALLEGRO_BITMAP* pDestBitmap = getAllegroBitmapOrBackbuffer( pDest );
	al_set_target_bitmap( pDestBitmap );

	//draw
	al_draw_bitmap( pSrc->mpBitmap, (float)dx, (float)dy, (int)flags );

	cacheOrRestoreTargetBitmap( false );
}

void GraphicsSystem::flipDisplay()
{
	al_flip_display();
}

void GraphicsSystem::clearBackBuffer(const GraphicsColor& theColor)
{
	cacheOrRestoreTargetBitmap( true );

	al_set_target_bitmap( getAllegroBitmapOrBackbuffer( NULL ) );

	al_clear_to_color( theColor.mapGraphicsColorToAllegroColor() );

	cacheOrRestoreTargetBitmap( false );

}

void GraphicsSystem::drawText(GraphicsFont* pFont, UINT dx, UINT dy, const GraphicsColor& theColor, const std::string& text, GraphicsBuffer* pDest /*= NULL*/)
{
	cacheOrRestoreTargetBitmap( true );

	al_set_target_bitmap( getAllegroBitmapOrBackbuffer( pDest ) );

	ALLEGRO_COLOR alColor = theColor.mapGraphicsColorToAllegroColor();

	al_draw_text( pFont->mpFont, alColor, dx, dy, ALLEGRO_ALIGN_CENTRE, text.c_str() );

	cacheOrRestoreTargetBitmap( false );
}

const float DEFAULT_THICKNESS = 2.0f;

void GraphicsSystem::drawRectangle(UINT dx, UINT dy, UINT width, UINT height, const GraphicsColor& theColor, bool filled /*= true*/, GraphicsBuffer* pDest /*= NULL */)
{
	cacheOrRestoreTargetBitmap( true );

	al_set_target_bitmap( getAllegroBitmapOrBackbuffer( pDest ) );

	if( filled )
	{
		al_draw_filled_rectangle( (float)dx, (float)dy, (float)(dx+width), (float)(dy+height), theColor.mapGraphicsColorToAllegroColor() );
	}
	else
	{
		al_draw_rectangle( (float)dx, (float)dy, (float)(dx+width), (float)(dy+height), theColor.mapGraphicsColorToAllegroColor(), DEFAULT_THICKNESS );
	}

	cacheOrRestoreTargetBitmap( false );
}

void GraphicsSystem::drawSprite(const GraphicsSprite& sprite, UINT dx, UINT dy, int flags /*= 0 */, GraphicsBuffer* pDest /*= NULL */)
{
	cacheOrRestoreTargetBitmap( true );

	al_set_target_bitmap( getAllegroBitmapOrBackbuffer( pDest ) );

	al_draw_bitmap_region( sprite.getBuffer()->mpBitmap, sprite.getX(), sprite.getY(), sprite.getWidth(), sprite.getHeight(), dx, dy, flags );

	cacheOrRestoreTargetBitmap( false );
}

ALLEGRO_BITMAP* GraphicsSystem::getAllegroBitmapOrBackbuffer(GraphicsBuffer* pBuffer /* = NULL */ )
{

	ALLEGRO_BITMAP* pBitmap = NULL;

	if( pBuffer == NULL )//no buffer - set bitmap to be the backbuffer
	{
		ALLEGRO_DISPLAY* pDisplay = getGraphicsSystem()->mpDisplay;
		assert( pDisplay != NULL );
		pBitmap = al_get_backbuffer( pDisplay );
	}
	else
	{
		pBitmap = pBuffer->mpBitmap;
	}
	return pBitmap;
}

void GraphicsSystem::cacheOrRestoreTargetBitmap( bool shouldCache )
{
	static ALLEGRO_BITMAP* pCache = NULL;
	if( shouldCache)
	{
		pCache = al_get_target_bitmap();
	}
	else
	{
		//restore from cache
		al_set_target_bitmap( pCache );
	}
}


