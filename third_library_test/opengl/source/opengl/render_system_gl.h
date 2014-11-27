#pragma once
namespace glmap
{
	class RenderSystemGL
	{
		friend class RenderSystem;
	public:
		RenderSystemGL();
		virtual ~RenderSystemGL(){};

		virtual VertexBuffer* newVertexBuffer(VertexFormat vf, size_t size);
		virtual IndexBuffer* newIndexBuffer(IndexFormat format, size_t size);
		virtual Texture* newTexture();

		//render
		virtual void clear(PixelType color, bool clearColor = true, bool clearDepth = true, bool clearStencil = false);

		//color
		virtual void setColor(PixelType c);
		virtual void setColorMask(bool writeRed, bool writeGreen, bool writeBlue, bool writeAlpha);  //default value: all are true

		virtual void enableBlend(bool toEnable);  ///< If false, disable it. Default value: true. If enable, the blend function are: (SRC_ALPHA, 1-SRC_ALPHA).

		//depth
		virtual void setDepthMask(bool writeDepth);   //default value: true
		virtual void enableDepthTest(bool toEnable);  ///< If false, disable it. Default value: false.
		virtual void setDepthRange(float minDepth, float maxDepth); ///< Default value: (0, 1.0), in which 0 is nearest and 1.0f is farthest.
		virtual void setDepthFunc(DepthFunc func);

		// texture
		virtual void setTexture(Texture* tex);
		//virtual void enableTexReplaceMode(bool toEnable); //Default: false, the texture is applied in modulate mode.

		//some configurations
		//virtual void enableCullBackFace(bool toEnable);  //default: false

		// vb & ib
		virtual void setVertexBuffer(VertexBuffer* vb);
		virtual void setIndexBuffer(IndexBuffer* ib);

		virtual void setTexCoordScale(float scale);   //default: 1.0f. Will affect the texture matrix.

		// matrix
		virtual void setViewport(const Rect* viewport);
		virtual void setModelViewMatrix(const Matrix4x4* mat);
		virtual void setProjectMatrix(const Matrix4x4* mat);

		virtual void useOrthoCamera();  ///< Used to draw something in screen coordinates.

		// other
		virtual void beginDraw();
		virtual void endDraw();
		virtual void present();
		void restoreStates();

	protected:
		virtual void _draw(bool useIB, ElementType elementType, size_t start, size_t number);
	};
}
