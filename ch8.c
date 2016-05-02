#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include "cpu.h"
#include "dbg.h"



chip8 cpu;

void initGL(); //initialise GL

void draw_square(float x_coord, float y_coord); //draw pixel

void render(); //update render

void idle(); //idling function

void handle_key_press(unsigned char key, int x, int y);

void handle_key_release(unsigned char key, int x, int y);

int main(int argc, char const *argv[])
{

	const char * game = argv[1];
	if(!game) {
		log_info("Usage: ./ch8 ./<name of game>");
		exit(1);
	}

	initialise_cpu(&cpu);
	load_rom(&cpu, game);


	glutInit(&argc, argv); //initialise glut

	//initialise display and window

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 320);
	glutCreateWindow("Chip-Emul8");

	initGL();

	//handle key presses and releases
	glutKeyboardFunc(handle_key_press);
	glutKeyboardUpFunc(handle_key_release);

	//draw function
	glutDisplayFunc(render);

	//idle function (to redraw)
	glutIdleFunc(idle);

	glutMainLoop();

	return 0;
}

void handle_key_press(unsigned char key, int x, int y) {
	unsigned char input;
	switch(key) {
		case 27:
			log_info("Exiting emulator...");
			exit(0);
		case '1':
			// only certain key values are passed to the chip
			input = 0x0001;
			break;
		case '2':
			input = 0x0002;
			break;
		case '3':
			input = 0x0003;
			break;
		case '4':
			input = 0x000C;
			break;
		case 'q':
			input = 0x0004;
			break;
		case 'w':
			input = 0x0005;
			break;
		case 'e':
			input = 0x0006;
			break;
		case 'r':
			input = 0x000D;
			break;
		case 'a':
			input = 0x0007;
			break;
		case 's':
			input = 0x0008;
			break;
		case 'd':
			input = 0x0009;
			break;
		case 'f':
			input = 0x000E;
			break;
		case 'z':
			input = 0x000A;
			break;
		case 'x':
			input = 0x0000;
			break;
		case 'c':
			input = 0x000B;
			break;
		case 'v':
			input = 0x000F;
			break;
		default:
			break;
	}
	handle_input(&cpu, input);

	
}

void handle_key_release(unsigned char key, int x, int y) {
	handle_input(&cpu, 0x00FF);
}

void draw_square(float x_coord, float y_coord) {
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex2f(x_coord, y_coord);
		glVertex2f(x_coord + 10, y_coord);
		glVertex2f(x_coord + 10, y_coord + 10);
		glVertex2f(x_coord, y_coord + 10);
	glEnd();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT); //clears screen

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	emulate_cycle(&cpu);

	int i, j;
	for(i = 0; i < 640; i++) {
		for(j = 0; j < 320; j++) {
			if(cpu.gfx[i][j] == 1) {
				draw_square((float)(i * 10), (float) (j * 10));
			}
		}
	}

	glutSwapBuffers(); //smooth animation
}

void idle() {
	glutPostRedisplay(); //calls to redraw screen
}

void initGL() {
	// sets up GLUT window for 2D drawing
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640, 320, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// clears screen color
	glClearColor(0.f, 0.f, 0.f, 1.f);
}


