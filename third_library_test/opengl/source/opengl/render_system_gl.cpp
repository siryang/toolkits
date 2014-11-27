#include "opengl_stdafx.h"
#include "render_system_gl.h"
#include "texture_gl.h"

/* 
	A pair of implementation-dependent thresholds of OpenGL.
	In some Motorola phones, the batch size of glDrawElements() has a limit which is roughly between 18390 and 50430 in ME811.
*/
#define MAX_VERTEX_BUFFER_SIZE 0x2000  /* the vertex number in glDrawArrays()  */
#define MAX_INDEX_BUFFER_SIZE 0x4000   /* the index number in glDrawElements() */

glmap::RenderSystem* RenderSystem_new() {
	return new glmap::RenderSystemGL();
}

namespace glmap
{

	//////////////////////////////////////////////////////////////////////////
	// VBO/IBO
#ifdef USE_VBO
	class GlVertexBuffer : public VertexBuffer
	{
		friend class RenderSystemGL;
	public:
		GlVertexBuffer(VertexFormat vf, size_t size) : VertexBuffer(vf, size) {
			m_handle = 0;
			glGenBuffers(1, &m_handle);
		}
		virtual ~GlVertexBuffer() {
			glDeleteBuffers(1, &m_handle);
		}
		virtual void update(void* data, size_t start, size_t length) {
			UNUSED_VAR(start);
			//GLenum errNo = glGetError();  //fixit: remove the debug codes
			glBindBuffer(GL_ARRAY_BUFFER, m_handle);
			glBufferData(GL_ARRAY_BUFFER, length * m_vertexSize, data, GL_STATIC_DRAW);
			//errNo = glGetError();
			//if(errNo != 0)
			//	CQ_TRACE("In GlVertexBuffer::update(), glBufferData() resulted in an error: ", errNo);
		}

		void onGLContextLost(){
			m_handle = 0;
		}

	private:
		GLuint m_handle;
	};

	class GlIndexBuffer : public IndexBuffer
	{
		friend class RenderSystemGL;
	public:
		GlIndexBuffer(IndexFormat format, size_t size) : IndexBuffer(format, size) {
			glGenBuffers(1, &m_handle);
		}
		virtual ~GlIndexBuffer() {
			glDeleteBuffers(1, &m_handle);
		}
		virtual void update(void* data, size_t start, size_t length) {
			UNUSED_VAR(start);
			//GLenum errNo = glGetError(); //fixit: remove the debug codes
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * m_indexSize, data, GL_STATIC_DRAW);
			//errNo = glGetError();
			//if(errNo != 0)
			//	CQ_TRACE("In GlVertexBuffer::update(), glBufferData() resulted in an error: ", errNo);
		}

		void onGLContextLost(){
			m_handle = 0;
		}

	private:
		GLuint m_handle;
	};
#endif


	VertexBuffer* RenderSystemGL::newVertexBuffer(VertexFormat vf, size_t size)
	{
#ifdef USE_VBO
		return new GlVertexBuffer(vf, size);
#else
		return new MemVertexBuffer(vf, size);
#endif
	}

	IndexBuffer* RenderSystemGL::newIndexBuffer(IndexFormat format, size_t size)
	{
#ifdef USE_VBO
		return new GlIndexBuffer(format, size);
#else
		return new MemIndexBuffer(format, size);
#endif
	}

	Texture* RenderSystemGL::newTexture()
	{
		return new TextureGL();
	}

	////////////////////////////////////////////////////////////////////////////
	//  RenderSystemGL
	RenderSystemGL::RenderSystemGL()
	{
		CQ_TRACE("[D3D]In RenderSystemGL::RenderSystemGL().  ", 0);

		Rect tmpViewport = {0, 0, 480, 800};
		setViewport(&tmpViewport);

		memcpy(&m_matProject.m, &m_matOrthoProj.m, sizeof(float)*16);

		m_matModelView = Matrix4x4::identity;

		m_texCoordScale = 1.0f;

		restoreStates();
		useOrthoCamera();
	}

	void RenderSystemGL::restoreStates()
	{
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		//glPolygonMode(GL_FRONT, GL_LINE);

		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glDepthFunc(GL_LEQUAL);
	}

	void RenderSystemGL::clear(PixelType color, bool clearColor, bool clearDepth, bool clearStencil)
	{
		GLbitfield bits = 0;
		if (clearColor)
			bits |= GL_COLOR_BUFFER_BIT;
		if (clearDepth)
			bits |= GL_DEPTH_BUFFER_BIT;
		if (clearStencil)
			bits |= GL_STENCIL_BUFFER_BIT;
		glClearColor((float)(Pixel_getR(color))/Pixel_channelMaxR, (float)(Pixel_getG(color))/Pixel_channelMaxG, (float)(Pixel_getB(color))/Pixel_channelMaxB, (float)(Pixel_getA(color))/Pixel_channelMaxA);

		glScissor(m_viewport.left,
			m_screenHeight == 0 ? 0 : (m_screenHeight - m_viewport.bottom),
			Rect_getWidth(m_viewport),
			Rect_getHeight(m_viewport));

		glEnable(GL_SCISSOR_TEST);

		glClear(bits);

		glDisable(GL_SCISSOR_TEST);
	}

	void RenderSystemGL::setColor(PixelType c)
	{
		//Note. In some low-end Android phones glColor4ub() isn't implemented. At runtime they complain "called unimplemented OpenGL ES API" on it.
		//
		//glColor4ub((GLubyte)Pixel_getR(c) * 255 / Pixel_channelMaxR, (GLubyte)Pixel_getG(c) * 255 / Pixel_channelMaxG, (GLubyte)Pixel_getB(c) * 255 / Pixel_channelMaxB, (GLubyte)Pixel_getA(c) * 255 / Pixel_channelMaxA);
		glColor4f((float)Pixel_getR(c) / (float)Pixel_channelMaxR, (float)Pixel_getG(c) / (float)Pixel_channelMaxG, (float)Pixel_getB(c) / (float)Pixel_channelMaxB, (float)Pixel_getA(c) / (float)Pixel_channelMaxA);
	}

	void RenderSystemGL::setColorMask(bool writeRed, bool writeGreen, bool writeBlue, bool writeAlpha)
	{
		glColorMask((GLboolean)writeRed, (GLboolean)writeGreen, (GLboolean)writeBlue, (GLboolean)writeAlpha);
	}

	void RenderSystemGL::setDepthMask(bool writeDepth)
	{
		glDepthMask((GLboolean)writeDepth);
	}

	void RenderSystemGL::enableDepthTest(bool toEnable)
	{
		if(toEnable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void RenderSystemGL::enableBlend(bool toEnable)
	{
		if(toEnable)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}

	void RenderSystemGL::setVertexBuffer(VertexBuffer* vb)
	{
#ifdef USE_VBO
		if (vb != NULL)
		{
			GlVertexBuffer* _vb = (GlVertexBuffer*)vb;
			glBindBuffer(GL_ARRAY_BUFFER, _vb->m_handle);
		}
#endif
		m_vb = vb;
	}

	void RenderSystemGL::setIndexBuffer(IndexBuffer* ib)
	{
#ifdef USE_VBO
		if (ib != NULL)
		{
			GlIndexBuffer* _ib = (GlIndexBuffer*)ib;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ib->m_handle);
		}
#endif
		m_ib = ib;
	}

	void RenderSystemGL::setTexture(Texture* tex)
	{
		TextureGL* glTex = (TextureGL*)tex;
		if (tex != NULL)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, glTex->getTexObjId());
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
	}

	//void RenderSystemGL::enableTexReplaceMode(bool toEnable)
	//{
	//	if(toEnable)
	//		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	//	else
	//		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	//}

	void RenderSystemGL::_draw(bool useIB, ElementType elementType, size_t start, size_t number)
	{
		GLenum drawType;
		int32  primitiveSize = 12;

		if (m_vb == NULL)
			return;

		switch(elementType)
		{
		case ElementType_points:
			drawType = GL_POINTS;
			primitiveSize = 1;
			break;
		case ElementType_lines:
			drawType = GL_LINES;
			primitiveSize = 2;
			break;
		case ElementType_lineStripe:
			drawType = GL_LINE_STRIP;
			break;
		case ElementType_lineLoop:
			drawType = GL_LINE_LOOP;
			break;
		case ElementType_triangles:
			drawType = GL_TRIANGLES;
			primitiveSize = 3;
			break;
		case ElementType_triangleStripe:
			drawType = GL_TRIANGLE_STRIP;
			break;
		case ElementType_triangleFan:
			drawType = GL_TRIANGLE_FAN;
			break;
#ifdef MAPBAR_WIN32
		case ElementType_quads:
			drawType = GL_QUADS; 
			primitiveSize = 4;
			CQ_ASSERT(0);
			break;
#endif
		default:
			return;
		}

#ifdef USE_VBO
		VertexBuffer* vb = m_vb;
#	define VB_OFFSET(i) ((char *)NULL + (i))
#	define IB_OFFSET(i) ((char *)NULL + (i))
#else
		MemVertexBuffer* vb = (MemVertexBuffer*)m_vb;
		MemIndexBuffer* ib = (MemIndexBuffer*)m_ib;
#	define VB_OFFSET(i) ((vbBuffer) + (i))
#	define IB_OFFSET(i) ((ibBuffer) + (i))
#endif

		//set vertex coordinate array
		uint32 vertexFormat = vb->format();
		size_t vertexSize = vb->vertexSize();
		char* vbBuffer = vb->buffer();
		size_t sizeVertex = 0;
		glEnableClientState(GL_VERTEX_ARRAY);
		if(vertexFormat & VertexFormat_xyz)
		{
			sizeVertex = 12;
			glVertexPointer(3, GL_FLOAT, vertexSize, VB_OFFSET(0));
		}
		else if(vertexFormat & VertexFormat_xy)
		{
			sizeVertex = 8;
			glVertexPointer(2, GL_FLOAT, vertexSize, VB_OFFSET(0));
		}
		else if(vertexFormat & VertexFormat_xyz_short)
		{
			glVertexPointer(3, GL_SHORT, vertexSize, VB_OFFSET(0));
			sizeVertex = 6;
		}
		else if(vertexFormat & VertexFormat_xy_short)
		{
			glVertexPointer(2, GL_SHORT, vertexSize, VB_OFFSET(0));
			sizeVertex = 4;
		}
		else
		{
			glDisableClientState(GL_VERTEX_ARRAY);
			CQ_ASSERT(FALSE);
		}

		//set texture coordinate array
		size_t sizeTexCoord = 0;
		if(vertexFormat & VertexFormat_uv)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, vertexSize, VB_OFFSET(sizeVertex));
			sizeTexCoord = 8;

			setTexCoordScale(1.0f);
		}
		else if(vertexFormat & VertexFormat_uv_short)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_SHORT, vertexSize, VB_OFFSET(sizeVertex));
			sizeTexCoord = 4;

			float texScale = 1.0f / (float)(1<<MAX_TEXTURE_SIZE_LOG);
			setTexCoordScale(texScale);
		}
		else
		{
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			setTexCoordScale(1.0f);
		}

		//set normal array
		size_t sizeNormal = 0;
		if(vertexFormat & VertexFormat_normal)
		{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, vertexSize, VB_OFFSET(sizeVertex+sizeTexCoord));
			sizeNormal = 12;
		}
		else
		{
			glDisableClientState(GL_NORMAL_ARRAY);
		}

		//set color array
		if(vertexFormat & VertexFormat_color)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_UNSIGNED_BYTE, vertexSize, VB_OFFSET(sizeVertex+sizeTexCoord+sizeNormal));
		}
		else
		{
			glDisableClientState(GL_COLOR_ARRAY);
		}


		//Really draw the wanted. 
#ifdef MAPBAR_DEBUG
		char _traceBuf[256];
#endif
		if (useIB)
		{
			char* ibBuffer = ib->buffer();

			if(drawType==GL_TRIANGLES || drawType==GL_LINES || drawType==GL_POINTS)
			{
				CQ_ASSERT(start%primitiveSize == 0);
				do{
					if(number <= MAX_INDEX_BUFFER_SIZE)
					{
						glDrawElements(drawType, number, GL_UNSIGNED_SHORT, IB_OFFSET(start * m_ib->indexSize()));
						number = 0;
					}
					else
					{
						size_t batchSize = (MAX_INDEX_BUFFER_SIZE - MAX_INDEX_BUFFER_SIZE%primitiveSize);
						glDrawElements(drawType, batchSize, GL_UNSIGNED_SHORT, IB_OFFSET(start * m_ib->indexSize()));
						start += batchSize;
						number -= batchSize;
					}
				}while (number > 0);
			}
			else
			{
#ifdef MAPBAR_DEBUG
				if(number > MAX_INDEX_BUFFER_SIZE)
				{
					cq_sprintf(_traceBuf, "[MapRender] WARN. An OpenGL drawing primitive of type 0x%x has a too big IB: %d //max: ", drawType, number);
					CQ_TRACE(_traceBuf, MAX_INDEX_BUFFER_SIZE);
				}
#endif
				glDrawElements(drawType, number, GL_UNSIGNED_SHORT, IB_OFFSET(start * m_ib->indexSize()));
			}
		}
		else
		{
			if(drawType==GL_TRIANGLES || drawType==GL_LINES || drawType==GL_POINTS)
			{
				CQ_ASSERT(start%primitiveSize == 0);
				do{
					if(number <= MAX_INDEX_BUFFER_SIZE)
					{
						glDrawArrays(drawType, start, number);
						number = 0;
					}
					else
					{
						size_t batchSize = (MAX_INDEX_BUFFER_SIZE - MAX_INDEX_BUFFER_SIZE%primitiveSize);
						glDrawArrays(drawType, start, batchSize);
						start += batchSize;
						number -= batchSize;
					}
				}while (number > 0);
			}
			else
			{
#ifdef MAPBAR_DEBUG
				if(number > MAX_VERTEX_BUFFER_SIZE)
				{
					cq_sprintf(_traceBuf, "[MapRender]WARN. An OpenGL drawing primitive of type 0x%x has a too big VB: %d //max: ", drawType, number);
					CQ_TRACE(_traceBuf, MAX_VERTEX_BUFFER_SIZE);
				}
#endif
				glDrawArrays(drawType, start, number);
			}
		}
	}

	void RenderSystemGL::beginDraw()
	{
	}

	void RenderSystemGL::endDraw()
	{
		glFlush();
	}

	void RenderSystemGL::present()
	{
		// fixit: A mysterious bug of openGL? We must enable depth write before SwapBuffer().
		// Or else the buildings will not be correctly rendered.
		glDepthMask(true);   //rs->setDepthMask(true);
		RenderSystem::present();
	}

	void RenderSystemGL::setViewport(const Rect* viewport)
	{
		m_viewport = *viewport;

		// if screen height is never set, use viewport.bottom
		glViewport(m_viewport.left,
			m_screenHeight == 0 ? 0 : (m_screenHeight - m_viewport.bottom),	
			Rect_getWidth(m_viewport),
			Rect_getHeight(m_viewport));

		cq_setOrthoMatrix(&m_matOrthoProj, (float)viewport->left, (float)viewport->right, (float)viewport->bottom, (float)viewport->top, 0.0f, 1.0f);
	}

	void RenderSystemGL::setModelViewMatrix(const Matrix4x4* mat)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf((GLfloat*)mat->m);

		m_matModelView = *mat;
	}

	void RenderSystemGL::setProjectMatrix(const Matrix4x4* mat)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf((GLfloat*)mat->m);

		m_matProject = *mat;
	}

	void RenderSystemGL::setTexCoordScale(float scale)
	{
		if(FLOAT_NOT_EQUAL(scale, m_texCoordScale))
		{
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			if(FLOAT_NOT_EQUAL(scale, 1.0f))
				glScalef(scale, scale, scale);

			m_texCoordScale = scale;
		}
	}

	void RenderSystemGL::useOrthoCamera()
	{
		memcpy(&m_matProject.m, (float*)m_matOrthoProj.m, sizeof(float)*16);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf((float*)m_matProject.m);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		m_matModelView = Matrix4x4::identity;
	}

	void RenderSystemGL::setDepthRange(float minDepth, float maxDepth)
	{
		glDepthRangef(minDepth, maxDepth);
	}

	void RenderSystemGL::setDepthFunc(DepthFunc func)
	{
		switch(func)
		{
		case DepthFunc_never:
			glDepthFunc(GL_NEVER);
			break;
		case DepthFunc_less:
			glDepthFunc(GL_LESS);
			break;
		case DepthFunc_lEqual:
			glDepthFunc(GL_LEQUAL);
			break;
		case DepthFunc_equal:
			glDepthFunc(GL_EQUAL);
			break;
		case DepthFunc_notEqual:
			glDepthFunc(GL_NOTEQUAL);
			break;
		case DepthFunc_gEqual:
			glDepthFunc(GL_GEQUAL);
			break;
		case DepthFunc_greater:
			glDepthFunc(GL_GREATER);
			break;
		case DepthFunc_always:
			glDepthFunc(GL_ALWAYS);
			break;
		}
	}

	//void RenderSystemGL::enableCullBackFace(bool toEnable)
	//{
	//	if(toEnable)
	//	{
	//		glEnable(GL_CULL_FACE);
	//		glCullFace(GL_BACK);
	//	}
	//	else
	//	{
	//		glDisable(GL_CULL_FACE);
	//	}
	//	;
	//}
}

#endif //#ifdef MAPBAR_USE_OPENGL
