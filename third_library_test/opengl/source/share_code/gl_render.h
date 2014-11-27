#pragma once
#include "opengl_util.h"
#include "opengl_camera.h"

class RenderSystem
{
public:
	static RenderSystem* instance()
	{
		if (m_instance == NULL)
		{
			m_instance = new RenderSystem;
		}
		return m_instance;
	}

	void draw();
	void drawstr(GLuint x, GLuint y, char* format, ...);
	void drawCube();
	void drawCoordinate();

	void setCamera(Camera camera)
	{
		m_camera = camera;
	}

	const Camera* getCamera()
	{
		return &m_camera;
	}

	//void setModelViewMatrix(const Matrix4x4* mat);
	//void setProjectMatrix(const Matrix4x4* mat);

	//virtual void useOrthoCamera();  ///< Used to draw something in screen coordinates.

private:
	Size m_screenSize;
	Camera m_camera;
	static RenderSystem* m_instance;
};


