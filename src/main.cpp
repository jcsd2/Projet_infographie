#include "ofMain.h"
#include "application.h"


/*
 * brief: Point d'entre du programme 1024p x 1024p
 */

int main( ){

	ofSetupOpenGL(1024, 1024, OF_WINDOW);
	ofRunApp(new Application());
	ofRunMainLoop();
}
