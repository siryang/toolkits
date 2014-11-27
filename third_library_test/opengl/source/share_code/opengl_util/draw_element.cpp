#include "../opengl_util.h"

void drawTriangle(const GLPoint3F* a, const GLPoint3F* b, const GLPoint3F* c, size_t num)
{
	// Draw a filled rectangle with current color
	glBegin(GL_TRIANGLE_FAN);
	for (size_t i = 0; i < num; i++)
	{
		glVertex3f(a[i].x, a[i].y, a[i].z);
		glVertex3f(b[i].x, b[i].y, b[i].z);
		glVertex3f(c[i].x, c[i].y, c[i].z);
	}
	glEnd();
}

void drawModel()
{
	GLMmodel* g_model = NULL;
	if (!g_model) {
		g_model = glmReadOBJ("data/soccerball.obj");
		if (!g_model) exit(0);
		glmUnitize(g_model);
		glmFacetNormals(g_model);
		glmVertexNormals(g_model, 90.0);
	}

	glmDraw(g_model, GLM_SMOOTH | GLM_MATERIAL);
}
