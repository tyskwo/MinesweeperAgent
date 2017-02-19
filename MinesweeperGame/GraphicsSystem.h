#pragma once
#include "..\gamelib\trackable.h"
#include <assert.h>
#include <windows.h>

#include "InputSystem.h"

struct ALLEGRO_DISPLAY;
struct ALLEGRO_BITMAP;
class GraphicsBuffer;
class GraphicsFont;
struct GraphicsColor;
class GraphicsSprite;

class GraphicsSystem : public Trackable
{
	friend void InputSystem::init();
	friend void InputSystem::cleanup();
public:
	bool init( UINT width, UINT height, bool fullscreen );//return false if problem initting
	void cleanup();

	inline static GraphicsSystem* getGraphicsSystem(){ assert( mspInstance != NULL ); return mspInstance; };
	inline static void createGraphicsSystem(){ assert( mspInstance == NULL ); mspInstance = new GraphicsSystem; };
	static void deleteGraphicsSystem();

	static void drawEntireBuffer( GraphicsBuffer* pSrc, UINT dx, UINT dy, int flags = 0, GraphicsBuffer* pDest = NULL );
	static void drawText( GraphicsFont* pFont, UINT dx, UINT dy, const GraphicsColor& theColor, const std::string& text, GraphicsBuffer* pDest = NULL );
	static void drawRectangle( UINT dx, UINT dy, UINT width, UINT height, const GraphicsColor& theColor, bool filled = true, GraphicsBuffer* pDest = NULL );
	static void drawSprite( const GraphicsSprite& sprite, UINT dx, UINT dy, int flags = 0, GraphicsBuffer* pDest = NULL );
	static void flipDisplay();
	static void clearBackBuffer( const GraphicsColor& theColor );

	static GraphicsFont* getFont() { return mspInstance->mpFont; };

private:
	static GraphicsSystem* mspInstance;

	//private helper functions
	static ALLEGRO_BITMAP* getAllegroBitmapOrBackbuffer( GraphicsBuffer* pBuffer = NULL );
	static void cacheOrRestoreTargetBitmap( bool shouldCache );

	ALLEGRO_DISPLAY* mpDisplay;
	UINT mWidth;
	UINT mHeight;
	bool mFullScreen;
	GraphicsFont* mpFont;

	GraphicsSystem();
	~GraphicsSystem(){ cleanup(); };

};

