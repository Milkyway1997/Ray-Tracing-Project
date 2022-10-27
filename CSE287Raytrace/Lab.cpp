#include "Lab.h"

//******************* GLOBALS **********************//

// The global variables are visible throughout this file.

// Frame buffer holding the color values for each pixel
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Some predefined colors.
const color LIGHT_BLUE(0.784, 0.784, 1.0, 1.0);

// Raytracer
RayTracer rayTrace(frameBuffer, LIGHT_BLUE );

// Light sources in the scene
shared_ptr<LightSource> ambientLight;
shared_ptr<PositionalLight> lightPos;
shared_ptr<DirectionalLight> lightDir;
shared_ptr<SpotLight> lightSpot;

BOOLEAN dayTime = true;

//*********** END OF GLOBAL DECLARATIONS **********//


/**
 * @fn	static void RenderSceneCB()
 *
 * @brief	Acts as the display function for the window.
 */
static void RenderSceneCB()
{
	int frameStartTime = glutGet( GLUT_ELAPSED_TIME ); // Get start time

	//// Clear the color buffer
	//frameBuffer.clearColorAndDepthBuffers( ); // Not necessary for ray tracing

	// Ray trace the scene to determine the color of all the pixels in the scene
	rayTrace.raytraceScene( );

	// Display the color buffer
	frameBuffer.showColorBuffer();

	// Calculate and display time required to render scene.
	int frameEndTime = glutGet( GLUT_ELAPSED_TIME ); // Get end time

	float totalTimeSec = (frameEndTime - frameStartTime) / 1000.0f;

	cout << "Render time: " << totalTimeSec << " sec." << endl;

} // end RenderSceneCB


/**
 * @fn	static void ResizeCB(int width, int height)
 *
 * @brief	Reset viewport limits for full window rendering each time the window 
 * 			is resized. This function is called when the program starts up and 
 * 			each time the window is resized.
 *
 * @param	width 	The window width in pixels.
 * @param	height	The window height in pixels.
 */
static void ResizeCB(int width, int height)
{
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize( width, height );

	rayTrace.calculatePerspectiveViewingParameters(45.0);

	//rayTrace.calculateOrthographicViewingParameters(20.0);

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB


/**
 * @fn	static void KeyboardCB(unsigned char key, int x, int y)
 *
 * @brief	Responds to 'f' and escape keys. 'f' key allows toggling full 
 * 			screen viewing. Escape key ends the program. Allows lights to
 * 			be individually turned on and off by pressing the 'a,' 'p,' 
 * 			'd,' and 's' keys. and  
 *
 * @param	key	The key that was pressed.
 * @param	x  	The x coordinate of the mouse.
 * @param	y  	The y coordinate of the mouse.
 */
static void KeyboardCB(unsigned char key, int x, int y)
{
	switch(key) {

	case('f'): case('F') : // 'f' key to toggle full screen
		glutFullScreenToggle();
		break;
	case(27): // Escape key
		glutLeaveMainLoop();
		break;
	case('0') :
		rayTrace.setRecursionDepth( 0 );
		break;
	case('1') :
		rayTrace.setRecursionDepth( 1 );
		break;
	case('2') :
		rayTrace.setRecursionDepth( 2 );
		break;
	case('3') :
		rayTrace.setRecursionDepth( 3 );
		break;
	case('4') :
		rayTrace.setRecursionDepth( 4 );
		break;
	case( '5' ):
		rayTrace.setRecursionDepth( 5 );
		break;
	case( 'a' ):
		// toggle light on and off
		ambientLight->enabled = (ambientLight->enabled) ? false : true;
		std::cout << key << " key pressed." << std::endl;
		break;
	case( 'p' ):
		// toggle light on and off
		lightPos->enabled = ( lightPos->enabled ) ? false : true;
		std::cout << key << " key pressed." << std::endl;
		break;
	case( 'd' ):
		// toggle light on and off
		lightDir->enabled = ( lightDir->enabled ) ? false : true;
		std::cout << key << " key pressed." << std::endl;
		break;
	case( 's' ):
		// toggle light on and off
		lightSpot->enabled = ( lightSpot->enabled ) ? false : true;
		std::cout << key << " key pressed." << std::endl;
		break;
	case('y'):
		// toggle sky reflection on and off
		rayTrace.reflectSky = (rayTrace.reflectSky) ? false : true;
		std::cout << key << " key pressed." << std::endl;
		break;
	case('m'):
		// toggle day on
		if (dayTime == false) {
			rayTrace.setDefaultColor(LIGHT_BLUE);
			ambientLight->ambientLightColor = 10.0 * ambientLight->ambientLightColor / 4.0;
			lightPos->diffuseLightColor = 10.0 * lightPos->diffuseLightColor / 4.0;
			lightDir->diffuseLightColor = 10.0 * lightDir->diffuseLightColor / 4.0;
			lightSpot->diffuseLightColor = 10.0 * lightSpot->diffuseLightColor / 4.0;
			dayTime = true;
		}
		std::cout << key << " key pressed." << std::endl;
		break;
	case('n'):
		// toggle night on
		if (dayTime) {
			rayTrace.setDefaultColor(color(0.1, 0.1, 0.1, 1.0));
			ambientLight->ambientLightColor = 0.4 * ambientLight->ambientLightColor;
			lightPos->diffuseLightColor = 0.4 * lightPos->diffuseLightColor;
			lightDir->diffuseLightColor = 0.4 * lightDir->diffuseLightColor;
			lightSpot->diffuseLightColor = 0.4 * lightSpot->diffuseLightColor;
			dayTime = false;
		}
		std::cout << key << " key pressed." << std::endl;
		break;
	case('g'):
		// toggle new camera point
		rayTrace.setCameraFrame(dvec3(30, 0, -10), dvec3(-1, 0, 0), dvec3(0, 1, 0));
		std::cout << key << " key pressed." << std::endl;
		break;
	case('h'):
		// toggle new camera point
		rayTrace.setCameraFrame(dvec3(0, 8, -35), dvec3(0, -0.3, 1), dvec3(0, 1, 0));
		std::cout << key << " key pressed." << std::endl;
		break;
	case('j'):
		// toggle new camera point
		rayTrace.setCameraFrame(dvec3(0.0, 30, -10), dvec3(0, -35, 1), dvec3(0, 1, 0));
		std::cout << key << " key pressed." << std::endl;
		break;
	case('z'):
		// toggle Antialiasing on and off
		rayTrace.isAntialiasing = (rayTrace.isAntialiasing) ? false : true;
		std::cout << key << " key pressed." << std::endl;
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}

	glutPostRedisplay();

} // end KeyboardCB


/**
 * @fn	static void SpecialKeysCB(int key, int x, int y)
 *
 * @brief	Responds to presses of the arrow keys
 *
 * @param	key	The key that was pressed
 * @param	x  	The x coordinate of the mouse.
 * @param	y  	The y coordinate of the mouse.
 */
static void SpecialKeysCB(int key, int x, int y)
{
	switch(key) {
	
	case(GLUT_KEY_RIGHT):
		
		break;
	case(GLUT_KEY_LEFT):
		
		break;
	default:
		cout << key << " key pressed." << endl;
	}

	glutPostRedisplay();

} // end SpecialKeysCB


/**
 * @fn	static void animate()
 *
 * @brief Registered as the "idle" function to have the screen continuously
 * 		  repainted. Due to software rendering, the frame rate will not
 * 		  be fast enough to support motion simulation.
 */
static void animate()
{
	glutPostRedisplay();

} // end animate

/**
 * @fn	void buildScene()
 *
 * @brief	Builds the scene by dynamically allocating ImplicitShape
 * 			objects and Light objects. and adding them to the raytracer 
 * 			surfaces vector.
 * 			
 */
void buildScene()
{
	rayTrace.setRecursionDepth(1);

	// Initialize random seed - used to create random colors
	srand((unsigned int)time(NULL));

	Material earth("earth.ppm");
	Material flag("usflag.ppm");

	//The camera below is used for testing spot light.
	rayTrace.setCameraFrame(dvec3(0, 8, 15), dvec3(0, -0.3, -1), dvec3(0, 1, 0));

	// Create a red sphere and it to the list of surfaces in the scene
	shared_ptr<Sphere> redBall = make_shared<Sphere>(dvec3(-1.5, 0.0, -16.0), 1.5, RED);
	shared_ptr<Sphere> magBall = make_shared<Sphere>(dvec3(1.5, 0.0, -16.0), 1.5, MAGENTA);
	shared_ptr<Sphere> grayBall = make_shared<Sphere>(dvec3(-4.5, 0.0, -14.0), 1.5, GRAY);
	shared_ptr<Sphere> cyanBall = make_shared<Sphere>(dvec3(4.5, 0.0, -14.0), 1.5, CYAN);
	shared_ptr<Sphere> blueBall = make_shared<Sphere>(dvec3(-7.5, 0.0, -12.0), 1.5, BLUE);
	shared_ptr<Sphere> yellowBall = make_shared<Sphere>(dvec3(7.5, 0.0, -12.0), 1.5, YELLOW);
	shared_ptr<Sphere> greenBall = make_shared<Sphere>(dvec3(-10.5, 0.0, -10.0), 1.5, GREEN);
	shared_ptr<Sphere> pinkBall = make_shared<Sphere>(dvec3(10.5, 0.0, -10.0), 1.5, color(230 / 255.0, 120 / 255.0, 200 / 255.0, 1.0));
	shared_ptr<Sphere> orangeBall = make_shared<Sphere>(dvec3(-13.5, 0.0, -8.0), 1.5, color(1.0, 160 / 255.0, 90 / 255.0, 1.0));
	shared_ptr<Sphere> darkRedBall = make_shared<Sphere>(dvec3(13.5, 0.0, -8.0), 1.5, color(80 / 255.0, 0.0, 0.0, 1.0));
	shared_ptr<Sphere> greenBall2 = make_shared<Sphere>(dvec3(10.5, 0.0, -6.0), 1.5, GREEN);
	shared_ptr<Sphere> pinkBall2 = make_shared<Sphere>(dvec3(-10.5, 0.0, -6.0), 1.5, color(230 / 255.0, 120 / 255.0, 200 / 255.0, 1.0));
	shared_ptr<Sphere> blueBall2 = make_shared<Sphere>(dvec3(7.5, 0.0, -4.0), 1.5, BLUE);
	shared_ptr<Sphere> yellowBall2 = make_shared<Sphere>(dvec3(-7.5, 0.0, -4.0), 1.5, YELLOW);
	shared_ptr<Sphere> grayBall2 = make_shared<Sphere>(dvec3(4.5, 0.0, -2.0), 1.5, GRAY);
	shared_ptr<Sphere> cyanBall2 = make_shared<Sphere>(dvec3(-4.5, 0.0, -2.0), 1.5, CYAN);
	shared_ptr<Sphere> redBall2 = make_shared<Sphere>(dvec3(1.5, 0.0, 0.0), 1.5, RED);
	shared_ptr<Sphere> magBall2 = make_shared<Sphere>(dvec3(-1.5, 0.0, 0.0), 1.5, MAGENTA);

	shared_ptr<Sphere> earthBall = make_shared<Sphere>(dvec3(5, 2.5, -8.0), 5, BLUE);
	earthBall->material = earth;

	shared_ptr<Sphere> transBall = make_shared<Sphere>(dvec3(-5, 2.5, -8.0), 5, LIGHT_BLUE);
	transBall->material.isDielectric = true;

	shared_ptr<Cylinder> redCylinder = make_shared<Cylinder>(dvec3(-4.0, 4.5, -18.0), RED, 1, 12);
	redCylinder->setYCylinder();
	redCylinder->material = flag;

	shared_ptr<CapCylinder> capCylinder = make_shared<CapCylinder>(dvec3(-8.0, 0.0, 1.0), RED, 1, 3);
	shared_ptr<Ellipsoid> cyanEllipsoid = make_shared<Ellipsoid>(dvec3(8.0, 0.0, 1.0), CYAN, 2, 1, 1);

	shared_ptr<Plane> plane = make_shared<Plane>(dvec3(0.0, -1.5, 0.0), dvec3(0.0, 1.0, 0.0) , LIGHT_GRAY);

	rayTrace.surfaces.push_back(cyanBall);
	rayTrace.surfaces.push_back(redBall);
	rayTrace.surfaces.push_back(magBall);
	rayTrace.surfaces.push_back(grayBall);
	rayTrace.surfaces.push_back(blueBall);
	rayTrace.surfaces.push_back(yellowBall);
	rayTrace.surfaces.push_back(greenBall);
	rayTrace.surfaces.push_back(pinkBall);
	rayTrace.surfaces.push_back(orangeBall);
	rayTrace.surfaces.push_back(darkRedBall);
	rayTrace.surfaces.push_back(cyanBall2);
	rayTrace.surfaces.push_back(redBall2);
	rayTrace.surfaces.push_back(magBall2);
	rayTrace.surfaces.push_back(grayBall2);
	rayTrace.surfaces.push_back(blueBall2);
	rayTrace.surfaces.push_back(yellowBall2);
	rayTrace.surfaces.push_back(greenBall2);
	rayTrace.surfaces.push_back(pinkBall2);

	rayTrace.surfaces.push_back(plane);
	rayTrace.surfaces.push_back(redCylinder);
	rayTrace.surfaces.push_back(capCylinder);
	rayTrace.surfaces.push_back(cyanEllipsoid);
	rayTrace.surfaces.push_back(earthBall);
	rayTrace.surfaces.push_back(transBall);

	// Create light sources and add them to the scene.
	ambientLight = make_shared<LightSource>(BLACK);
	ambientLight->ambientLightColor = color(0.15, 0.15, 0.15, 1.0);
	lightPos = make_shared<PositionalLight>(dvec3(-10.0, 10.0, 10.0), color(0.9, 0.9, 0.9, 1));
	lightDir = make_shared<DirectionalLight>(dvec3(10.0, 10.0, -10.0), color(0.9, 0.9, 0.9, 1));
	lightSpot = make_shared<SpotLight>(dvec3(0, 8, 15), dvec3(0, -0.3, -1), PI / 4.0, color(0.8, 0.8, 0.8, 1));

	//LIGHT
	rayTrace.lights.push_back(lightPos);
	rayTrace.lights.push_back(lightDir);
	rayTrace.lights.push_back(lightSpot);
	rayTrace.lights.push_back(ambientLight);

}



/**
 * @fn	int main(int argc, char** argv)
 *
 * @brief	Main entry-point for this application. Creates a rendering window
 * 			and initializes OpenGL through Glut function calls. Registers
 * 			callback functions (functions that are called by the operating
 * 			system) for specific events. Builds a scene and then enters the 
 * 			rendering loop.
 *
 * @param	argc	The number of command-line arguments provided.
 * @param	argv	An array of command-line argument strings.
 *
 * @returns	Exit-code for the process - 0 for success, else an error code.
 */

int main(int argc, char** argv)
{
	// freeGlut and Window initialization ***********************

    // Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
    glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA );

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("Ray Trace");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor( BLACK );

	// Set the color to which pixels will be cleared if there is no intersection.
	rayTrace.setDefaultColor(LIGHT_BLUE);

	// Callbacks for window redisplay and other events
	glutDisplayFunc(RenderSceneCB);		
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);
	//glutIdleFunc( animate );

	// Create the objects and light sources.
	buildScene();

	// Enter the GLUT main loop. Control will not return until the window is closed.
    glutMainLoop();
 
	return 0;

} // end main