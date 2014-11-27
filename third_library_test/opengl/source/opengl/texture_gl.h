#pragma once

#include "cq_opengl.h"
#include "glmapX/texture.h"

namespace glmap
{
	class RenderSystemGL;

	class TextureGL : public Texture
	{
	public:
		TextureGL();
		virtual ~TextureGL();

		virtual void onContextLost();

		/** 
			@param area
				If NULL, assume that the whole texture is to be updated.
			@param data
				Image data.
		*/
		virtual void update(const Rect* area, PixelType* data);

		GLuint getTexObjId() { return m_glTexId;  };


	protected:
		GLuint m_glTexId;   //Taking 0 means the texture instance is invalid.
		GLint m_glFormat;
		GLint m_glComponentType;
		
	protected:
		virtual bool _createWithBuffer(PixelType* buffer, TextureType type, TextureFlag flags);
		virtual void _freeRes();
	};


}

#endif //#ifdef MAPBAR_USE_OPENGL
