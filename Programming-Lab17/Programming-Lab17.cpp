#include <iostream>
#include <GL/glut.h>

float left;
float bottom;
float width;
float height;

float f(float x)
{
	return abs(sin(x)) + cos(abs(x));
}

void setScale()
{
	float max = -FLT_MAX;
	float min = FLT_MAX;

	size_t points_count = glutGet(GLUT_WINDOW_WIDTH);
	float x = left;
	float dx = width / points_count;
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
	}

	bottom = min;
	height = max - min;
}

void drawGraph()
{
	glBegin(GL_LINE_STRIP);
	glColor3f(255, 0, 0);

	size_t points_count = glutGet(GLUT_WINDOW_WIDTH);
	float x = left;
	float dx = width / points_count;
	for (size_t i = 0; i < points_count; ++i, x += dx)
	{
		glVertex2f(x, f(x));
	}

	glEnd();
}

const float unit_line_scale = 0.015F;

void plot()
{
	float right = left + width;
	float top = bottom + height;

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
	float unit_height = height * unit_line_scale;
	float unit_bottom = unit_y_pos - unit_height;
	float unit_top = unit_y_pos + unit_height;
	int right_unit = floor(right);
	for (int i = ceil(left); i <= right_unit; ++i)
	{
		glVertex2f(i, unit_bottom);
		glVertex2f(i, unit_top);
	}

	//draw y-axis unit lines
	float unit_x_pos = left > 0 ? left : right < 0 ? right : 0;
	float unit_width = width * unit_line_scale;
	float unit_left = unit_x_pos - unit_width;
	float unit_right = unit_x_pos + unit_width;
	int top_unit = floor(top);
	for (int i = ceil(bottom); i <= top_unit; ++i)
	{
		glVertex2f(unit_left, i);
		glVertex2f(unit_right, i);
	}

	glEnd();

	drawGraph();

	glutSwapBuffers();
}

const float move_speed_scale = 0.01F;

void move(int key, int x, int y)
{
	float step_x = width * move_speed_scale;
	float step_y = height * move_speed_scale;

	switch (key)
	{
	case GLUT_KEY_LEFT:
		left -= step_x;
		break;
	case GLUT_KEY_RIGHT:
		left += step_x;
		break;
	case GLUT_KEY_DOWN:
		bottom -= step_y;
		break;
	case GLUT_KEY_UP:
		bottom += step_y;
		break;
	default:
		return;
	}

	glutPostRedisplay();
}

const float zoom_in_scale = 0.9F;
const float zoom_out_scale = 1.1F;

void zoom(float scale)
{
	float zoom_constant = (1.0F - scale) / 2.0F;
	left += width * zoom_constant;
	bottom += height * zoom_constant;
	width *= scale;
	height *= scale;
}

void zoom(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		zoom(zoom_in_scale);
		break;
	case '-':
		zoom(zoom_out_scale);
		break;
	default:
		return;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	float right;
	do
	{
		std::cout << "Enter left bound: ";
		std::cin >> left;

		std::cout << "Enter right bound (it must be bigger than left bound): ";
		std::cin >> right;

		std::cout << '\n';
	} while (right <= left);
	width = right - left;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);

	glutInitWindowSize(600, 600);
	glutCreateWindow("Графік");

	glutDisplayFunc(plot);
	glutSpecialFunc(move);
	glutKeyboardFunc(zoom);

	setScale();

	glutMainLoop();
	return 0;
}