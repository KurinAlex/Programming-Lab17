#include <iostream>
#include <GL/glut.h>

#include "point.h"

float left;
float right;
float top;
float bottom;

const size_t points_count = 2000;
point points[points_count];

float f(float x)
{
	return abs(sin(x)) + cos(x);
}

void initPoints()
{
	float max = -FLT_MAX;
	float min = FLT_MAX;

	float x = left;
	float dx = (right - left) / points_count;
	for (size_t i = 0; i < points_count; ++i, x += dx)
	{
		float y = f(x);

		if (y < min)
		{
			min = y;
		}
		if (y > max)
		{
			max = y;
		}

		points[i].x = x;
		points[i].y = y;
	}

	top = max;
	bottom = min;
}

const float unit_line_scale = 0.015F;

void plot()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);

	//draw axis
	glBegin(GL_LINES);
	glColor3f(255, 255, 255);

	//draw x-axis
	glVertex2f(0, top);
	glVertex2f(0, bottom);

	//draw y-axis
	glVertex2f(left, 0);
	glVertex2f(right, 0);

	//draw x-axis unit lines
	float unit_y_pos = bottom > 0 ? bottom : top < 0 ? top : 0;
	float unit_x_height = (top - bottom) * unit_line_scale;
	for (int i = ceil(left); i <= floor(right); ++i)
	{
		glVertex2f(i, unit_y_pos - unit_x_height);
		glVertex2f(i, unit_y_pos + unit_x_height);
	}

	//draw y-axis unit lines
	float unit_x_pos = left > 0 ? left : right < 0 ? right : 0;
	float unit_y_width = (right - left) * unit_line_scale;
	for (int i = ceil(bottom); i <= floor(top); ++i)
	{
		glVertex2f(unit_x_pos - unit_y_width, i);
		glVertex2f(unit_x_pos + unit_y_width, i);
	}

	glEnd();

	//draw graph
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(255, 0, 0);
	glVertexPointer(2, GL_FLOAT, sizeof(point), points);
	glDrawArrays(GL_LINE_STRIP, 0, points_count);
	glDisableClientState(GL_VERTEX_ARRAY);

	glFlush();
	glutSwapBuffers();
}

const float move_speed_scale = 0.01F;

void move(int key, int x, int y)
{
	float step_x = (right - left) * move_speed_scale;
	float step_y = (top - bottom) * move_speed_scale;
	switch (key)
	{
	case GLUT_KEY_LEFT:
		left -= step_x;
		right -= step_x;
		break;
	case GLUT_KEY_RIGHT:
		left += step_x;
		right += step_x;
		break;
	case GLUT_KEY_DOWN:
		top -= step_y;
		bottom -= step_y;
		break;
	case GLUT_KEY_UP:
		top += step_y;
		bottom += step_y;
		break;
	}
	glutPostRedisplay();
}

const float zoom_in_scale = 0.9F;
const float zoom_out_scale = 1.1F;

void zoom(unsigned char key, int x, int y)
{
	float zoom_scale;

	switch (key)
	{
	case '+':
		zoom_scale = zoom_in_scale;
		break;
	case '-':
		zoom_scale = zoom_out_scale;
		break;
	default:
		return;
	}

	float half_width = (right - left) / 2.0F;
	float half_height = (top - bottom) / 2.0F;
	float offset_x = left + half_width;
	float offset_y = bottom + half_height;
	float new_half_width = half_width * zoom_scale;
	float new_half_height = half_height * zoom_scale;

	left = offset_x - new_half_width;
	right = offset_x + new_half_width;
	top = offset_y + new_half_height;
	bottom = offset_y - new_half_height;

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	do
	{
		std::cout << "Enter left bound: ";
		std::cin >> left;

		std::cout << "Enter right bound (it must be bigger than left bound): ";
		std::cin >> right;

		std::cout << '\n';
	} while (right <= left);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(600, 600);
	glutCreateWindow("Графік");

	glutDisplayFunc(plot);
	glutSpecialFunc(move);
	glutKeyboardFunc(zoom);

	initPoints();

	glutMainLoop();
	return 0;
}