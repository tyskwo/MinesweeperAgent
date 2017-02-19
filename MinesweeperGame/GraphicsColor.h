#pragma once

#include "..\gamelib\trackable.h"
#include <windows.h>

struct ALLEGRO_COLOR;

struct GraphicsColor : public Trackable
{
	friend class GraphicsSystem;
public:
	GraphicsColor( UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha = 255 );//default to solid

	UCHAR r, g, b, a;

private:
	ALLEGRO_COLOR mapGraphicsColorToAllegroColor() const;

};

const GraphicsColor COLOR_BLACK( 0, 0, 0, 255 );
const GraphicsColor COLOR_WHITE( 255, 255, 255, 255 );
const GraphicsColor COLOR_RED( 255, 0, 0, 255 );
const GraphicsColor COLOR_GREEN( 0, 255, 0, 255 );
const GraphicsColor COLOR_BLUE( 0, 0, 255, 255 );

