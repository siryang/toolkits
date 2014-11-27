#include "opengl_stdafx.h"
#include "texture_gl.h"

#include "../gluLib/glu_part.h"  //For mipmap functionality.

namespace glmap
{
	TextureGL::TextureGL()
	{
		m_glTexId = 0;
	}

	TextureGL::~TextureGL()
	{
		_freeRes();
	}

	void TextureGL::onContextLost()
	{
		m_glTexId = 0;
	}

	void TextureGL::_freeRes()
	{
		if(m_glTexId != 0)
		{
			glDeleteTextures(1, &m_glTexId);
			m_glTexId = 0;
		}
	}

	bool TextureGL::_createWithBuffer(PixelType* buffer, TextureType type, TextureFlag flags)
	{
		GLenum errNo = glGetError();

		switch(type)
		{
		case TextureType_RGB:
			m_glFormat = GL_RGB;
			m_glComponentType = GL_UNSIGNED_BYTE;
			break;
		case TextureType_RGBA:
			m_glFormat = GL_RGBA;
			m_glComponentType = GL_UNSIGNED_BYTE;
			break;
		case TextureType_RGBA4444:
			m_glFormat = GL_RGBA;
			m_glComponentType = GL_UNSIGNED_SHORT_4_4_4_4;
			break;
		case TextureType_RGB565:
			m_glFormat = GL_RGB;
			m_glComponentType = GL_UNSIGNED_SHORT_5_6_5;
			break;
		case TextureType_alphaOnly:
			m_glFormat = GL_ALPHA;
			m_glComponentType = GL_UNSIGNED_BYTE;
			break;
		default:
			return false;
		}

		glGenTextures(1, &m_glTexId);

		if(m_glTexId == 0)
			return false;

		GLint oldGLTexId;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &oldGLTexId);
		errNo = glGetError();
		glBindTexture(GL_TEXTURE_2D, m_glTexId);
		if(flags & TextureFlag_wrapModeClamp)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (flags & TextureFlag_generateMipmap)
		{
#	if defined(MAPBAR_ANDROID) || defined(MAPBAR_IPHONE)
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
#	endif
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
		}
		else
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		}		

		void* pixel = NULL;
		if (buffer != NULL)
		{
			// transfer buffer into texture
			pixel = _convertPixel((PixelType*)buffer);
		}

		//Commit pixel data, or allocate discard memory if pixel is NULL.
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

#if defined(MAPBAR_ANDROID) || defined(MAPBAR_IPHONE)
		glTexImage2D(GL_TEXTURE_2D, 0, m_glFormat, m_widthExtended, m_heightExtended, 0, m_glFormat, m_glComponentType, pixel);
#else
		if( (flags & TextureFlag_generateMipmap) && (pixel != NULL) )
			gluBuild2DMipmaps(GL_TEXTURE_2D, m_glFormat, m_widthExtended, m_heightExtended, m_glFormat, m_glComponentType, pixel);
		else //In windows when (pixel==NULL), mip-map isn't supported by TextureGL::update() in Windows at present.
			glTexImage2D(GL_TEXTURE_2D, 0, m_glFormat, m_widthExtended, m_heightExtended, 0, m_glFormat, m_glComponentType, pixel);
#endif

		if(pixel != NULL)
			_freePixel(pixel);

		glBindTexture(GL_TEXTURE_2D, oldGLTexId);

		errNo = glGetError();
		if(errNo != GL_NO_ERROR)
		{
			CQ_TRACE("[ERROR] Error occurred while glTexImage2D(). original height_width:", m_heightOriginal*10000+m_widthOriginal);
			glDeleteTextures(1, &m_glTexId);
			m_glTexId = 0;
			return false;
		}

		return true;
	}

	void TextureGL::update(const Rect* area, PixelType* data)
	{
		GLint oldGLTexId;
		int w = Rect_getWidth(*area);
		int h = Rect_getHeight(*area);
		 
		void* pixel = _convertPixel(data, w, h);
		if (pixel)
		{
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &oldGLTexId);
			glBindTexture(GL_TEXTURE_2D, m_glTexId);

			//Usually it's useless to call glFlush() here. However, in Huawei U8860 Android phone, glTexSubImage2D() has an error, which requires all 
			//read operations to a texture object come earlier than glTexSubImage2D() on it in the process of drawing a frame, or some areas in the 
			//texture other than the modified area by this function might be modified strangely with with some thin white vertical bars. 
			//To bypass this error, we insert a glFlush() here.
			glFlush();

			glTexSubImage2D(GL_TEXTURE_2D, 
				0, 
				area->left, area->top, w, h,
				m_glFormat, 
				m_glComponentType,
				pixel);

			glBindTexture(GL_TEXTURE_2D, oldGLTexId);

			_freePixel(pixel);
		}
	}

}	// namespace 

#endif  //#ifdef MAPBAR_USE_OPENGL
