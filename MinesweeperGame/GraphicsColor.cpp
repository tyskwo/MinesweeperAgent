#include "GraphicsColor.h"
#include <allegro5/allegro.h>


GraphicsColor::GraphicsColor(UCHAR red, UCHAR green, UCHAR blue, UCHAR alpha)
	:r(red)
	,g(green)
	,b(blue)
	,a(alpha)
{
}

ALLEGRO_COLOR GraphicsColor::mapGraphicsColorToAllegroColor() const
{
	return al_map_rgba( r, g, b, a );
}
