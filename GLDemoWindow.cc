/*
 * GLDemoWindow.cc
 *
 *  Created on: 7 нояб. 2020 г.
 *      Author: unyuu
 */

#include <cmath>
#include "GLDemoWindow.h"

#include "Primitives.h"

static constexpr double Pi = acos(-1.);

static VertexVector s_house_vertices {
	{  2.0, 2.0, 0.0 },
	{  2.0, 2.0, 2.0 },
	{  2.0, 0.0, 3.0 },
	{  2.0, -2.0, 2.0 },
	{  2.0, -2.0, 0.0 },
	{  -2.0, 2.0, 2.0 },
	{  -2.0, 2.0, 0.0 },
	{  -2.0, 0.0, 3.0 },
	{  -2.0, -2.0, 2.0 },
	{  -2.0, -2.0, 0.0 },
};

static FaceVector s_house_faces {
	{ 5, 4, 3, 2, 1, 0 },
	{ 5, 9, 8, 7, 5, 6 },
	{ 4, 0, 1, 5, 6 },
	{ 4, 4, 3, 8, 9 },
	{ 4, 1, 2, 7, 5 },
	{ 4, 3, 2, 7, 8 }
};

static NormalVector s_house_normals {
	{  1.0, 0.0, 0.0 },
	{  -1.0, 0.0, 0.0 },
	{  0.0, 1.0, 0.0 },
	{  0.0, -1.0, 0.0 },
	{  0.0, 1.0, 0.5 },
	{  0.0, -1.0, 0.5 },
};
static ColorVector s_house_colors {	// белые грани
	{  1.0f, 1.0f, 0.9f },
	{  0.9f, 0.9f, 1.0f },
	{  0.9f, 1.0f, 1.0f },
	{  1.0f, 0.9f, 0.9f },
	{  1.0f, 0.9f, 1.0f },
	{  1.0f, 1.0f, 0.9f },
};

static TexCoordVector s_house_tex_coords {
	{  { 0.50, 1.00 }, { 0.50, 0.50 }, { 0.75, 0.25 }, { 1.00, 0.50 }, { 1.00, 1.00 } },
	{  { 0.50, 1.00 }, { 0.50, 0.50 }, { 0.75, 0.25 }, { 1.00, 0.50 }, { 1.00, 1.00 } },
	{  { 0.00, 1.00 }, { 0.00, 0.50 }, { 0.50, 0.50 }, { 0.50, 1.00 } },
	{  { 0.00, 1.00 }, { 0.00, 0.50 }, { 0.50, 0.50 }, { 0.50, 1.00 } },
	{  { 0.00, 0.50 }, { 0.00, 0.00 }, { 0.50, 0.00 }, { 0.50, 0.50 } },
	{  { 0.00, 0.50 }, { 0.00, 0.00 }, { 0.50, 0.00 }, { 0.50, 0.50 } },
};


Model House { s_house_vertices, s_house_faces, s_house_normals, s_house_colors, s_house_tex_coords };

void tree(int n)
{
	ColorVector tree_colors{
		{0.0f, 1.0f, 0.0f}
	};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &tree_colors[0][0]);
	make_cone_smooth(20);
	for (int i = 0; i < n; ++i){
		glScaled(0.7, 0.7, 0.7);
		glTranslated(0.0, 0.0, 1.1);
		make_cone_smooth(16);
	}
}
void snowman()
{
	ColorVector snowman_colors {
			{  0.5f, 0.95f, 0.95f },
			{  1.0f, 0.1f, 0.0f },
		};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &snowman_colors[0][0]);
	make_sphere_smooth(20,20);
	for (int i = 0; i < 2; ++i){
			glScaled(0.7, 0.7, 0.7);
			glTranslated(0.0, 0.0, 2.0);
			make_sphere_smooth(20,20);
		}
	glScaled(0.5, 0.5, 0.5);
	glTranslated(0.0, -3.0, -3.0);
	make_sphere_smooth(20,20);

	glTranslated(0.0, 6.0, 0.0);
	make_sphere_smooth(20,20);

	glTranslated(2.0, -3.0, 3.0);
	glRotated(90, 0.0, 1.0, 0.0);
	glScaled(0.3, 0.3, 0.8);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &snowman_colors[1][0]);
	make_cone_smooth(10);
}
GLDemoWindow::GLDemoWindow(int width, int height)
: Window(width, height), _crate_texture("texture.png")
{
	SDL_GL_SetSwapInterval(1); // VSync
//	SDL_GL_SetSwapInterval(0); // Немедленная отрисовка

	_turn_angle = 0.;
	_tilt_phase = 0.;
}

void GLDemoWindow::setup()
{
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE);

	glClearColor(0.3f, 0.46f, 1.0f, 1.f);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0, double(width()) / double(height()), 0.01, 30.0);
	glMatrixMode(GL_MODELVIEW);

//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CW);
}

void GLDemoWindow::render()
{
	double x = 1.1, y = -0.4, z = 0.0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity(); // Identity matrix - единичная матрица

	gluLookAt(4.2, 4.2, 1.0, 0.0, 0.0, 0.5, 0.0, 0.0, 2.0);

	glPushMatrix();

	_crate_texture.bind();//дом
	glEnable(GL_TEXTURE_2D);
	glRotated(80.0, 0.0, 0.0, 1.0);
	glTranslated(x, y, z);
	glScaled(0.7, 0.7, 0.7);
	draw_model(House);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPushMatrix();

	glTranslated(x + 1.5, -0.5, z);//елка
	glScaled(0.7, 0.7, 0.7);
	tree(5);

	glPopMatrix();
	glPushMatrix();

	glTranslated(x + 1.6, y + 0.9 , z);//снеговики
	glRotated(60.0, 0.0, 0.0, 1.0);
	glScaled(0.2, 0.2, 0.2);
	snowman();
	glPopMatrix();
	glPushMatrix();
	glRotated(60.0, 0.0, 0.0, 1.0);
	glTranslated(x + 1.2, y - 1.3, z);
	glScaled(0.15, 0.15, 0.15);
	snowman();


}

void GLDemoWindow::draw_model(Model &model)
{
	for (unsigned i = 0; i < model.faces.size(); ++i) {
		int count = model.faces[i][0];
		if (count == 3)
			glBegin(GL_TRIANGLES);
		else if (count == 4)
			glBegin(GL_QUADS);
		else
			glBegin(GL_POLYGON);

		glNormal3dv(&model.normals[i][0]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &model.colors[i][0]);
		for (int j = 1; j <= count; ++j) {
			glTexCoord2dv(&model.tex_coords[i][j-1][0]);
			glVertex3dv(&model.vertices[model.faces[i][j]][0]);
		}

		glEnd();
	}
}

void GLDemoWindow::do_logic()
{
	_turn_angle += 2.;
	if (_turn_angle >= 360.)
		_turn_angle -= 360.;

	_tilt_phase += Pi / 200.;
	if (_tilt_phase >= Pi * 2.)
		_tilt_phase -= Pi * 2.;
}


