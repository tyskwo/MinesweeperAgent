#pragma once

#include "..\gamelib\trackable.h"
#include <windows.h>
#include <map>
#include <string>

struct ALLEGRO_FONT;
class GraphicsFont;

typedef UINT FontID;
typedef std::map<FontID, GraphicsFont*> FontContainer;

class GraphicsFont : public Trackable
{
	friend class GraphicsSystem;
public:
	GraphicsFont( const std::string& filename, size_t fontSize );
	~GraphicsFont();

	inline FontID getID() const { return mID; };
	inline size_t getSize() const { return mSize; };

	static GraphicsFont* getFont( const FontID& id );
	static bool fontExists( const FontID& id );
	static void deleteAllFonts();

private:
	static FontID msNextID;
	static FontContainer msFonts;

	ALLEGRO_FONT* mpFont;
	FontID mID;
	size_t mSize;
};

