

/// Usage : 
/// $ g++ rubics.cpp cube.hpp solver.hpp animater.hpp -lglut -lGLU -lGL -lm -fPIC -I /usr/include/eigen3 -o rubics
/// $ ./rubics

#include <cmath>
#include <GL/glut.h>
#include <vector>
#include "solver.hpp"

Cube* my_cube;

void render();
void draw_cube();
void reshape_func(int w, int h);
void keyboard_func(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void solveCube();

int main(int argc, char **argv)
{

  my_cube = new Cube();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(600, 50);
	glutInitWindowSize(640, 640);
	glutCreateWindow("Rubics cube");

	glutDisplayFunc(render);
	glutReshapeFunc(reshape_func);
	glutKeyboardFunc(keyboard_func);

  glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}

void draw_cube()
{
  my_cube->render();
  
  glColor3f(1.0,1.0,1.0);
  glPopMatrix();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(300,200,300,0,0,0,0,1,0);

	// glTranslatef(0.0,50.0,0.0);
	glPushMatrix();
	// kinch.render();

	draw_cube();  

	glutSwapBuffers();
}

void reshape_func(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, float(1.0*w/h), 1.0, 1000.0);
	// glOrtho(0.0f, w, h, 0.0f, 0.0f, 1.0f);

	glutPostRedisplay();
}

void keyboard_func (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case '1':
		my_cube->Transition(X, anticlockwise, 0); break;
  case '2':
    my_cube->Transition(X, clockwise, 1); break;
  case '3':
    my_cube->Transition(X, clockwise, 2); break;
  case '4':
    my_cube->Transition(Y, clockwise, 0); break;
  case '5':
    my_cube->Transition(Y, clockwise, 1); break;
  case '6':
    my_cube->Transition(Y, clockwise, 2); break;
  case '7':
    my_cube->Transition(Z, clockwise, 0); break;
  case '8':
    my_cube->Transition(Z, clockwise, 1); break;
  case '9':
    my_cube->Transition(Z, clockwise, 2); break;
  case 's':
    solveCube();
	default:
		break;
	}
}

void mouse(int button, int state, int x, int y){
	// if (button)
}

void solveCube() {
    std::vector<moves_cube> mov;
  // if (!inTransition && !inSolveMode) {
    // inSolveMode = true;
    Cube initial(my_cube->n, my_cube->side, my_cube->faces);
    solutionFinder sf(initial);
    mov = sf.solveIt();// move_index = 0;
    // code here

    std::cout << "number of moves required: " << mov.size() << std::endl;
    // exit(0);
    // animateMoves();
  // }
}