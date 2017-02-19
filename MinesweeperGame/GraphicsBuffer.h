#pragma once

#include "..\gamelib\trackable.h"
#include <windows.h>
#include <string>
#include <map>

struct ALLEGRO_BITMAP;
struct GraphicsColor;
class GraphicsBuffer;

typedef UINT BufferID;
typedef std::map<BufferID, GraphicsBuffer*> BufferContainer;

class GraphicsBuffer :	public Trackable
{
	friend class GraphicsSystem;
public:
	GraphicsBuffer( size_t width, size_t height );
	GraphicsBuffer( const std::string& filename );
	GraphicsBuffer( const GraphicsBuffer& rhs );
	~GraphicsBuffer();

	GraphicsBuffer& operator=( const GraphicsBuffer& rhs );

	void setToColor( const GraphicsColor& color );
	inline BufferID getID() const { return mID; };

	static GraphicsBuffer* getBuffer( const BufferID& id );
	static bool bufferExists( const BufferID& id );
	static void deleteAllGraphicsBuffers();

private:
	static BufferID msNextID;
	static BufferContainer msBuffers;

	UINT mID;
	ALLEGRO_BITMAP* mpBitmap;
	size_t mWidth;
	size_t mHeight;
};

