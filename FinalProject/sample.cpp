/*Name : Rahul Kumar Nalubandhu
email : nalubanr@oregonstate.edu
citations:
Some gl logics are refrenced from stack overflow and rest all from modules and ppt.*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "glut.h"

#include "bmptotexture.cpp"

//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Joe Graphics

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.


// title of these windows:

const char *WINDOWTITLE = { "Final Project" };
const char *GLUITITLE   = { "User Interface Window" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.025f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};

char * ColorNames[ ] =
{
	"Red",
	"Yellow",
	"Green",
	"Cyan",
	"Blue",
	"Magenta",
	"White",
	"Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees

int WhichView;

int visit_planet;

float move_axes_z = 0.;
float move_axes_x = 0.;
float move_axes_y = 0.;


GLuint gen_sun_list;
GLuint gen_mercury_list;
GLuint gen_venus_list;
GLuint gen_earth_List;
GLuint gen_moon_list;
GLuint gen_mars_list;
GLuint gen_jupiter_list;
GLuint gen_saturn_list;
GLuint gen_uranus_list;
GLuint gen_neptune_list;
GLuint gen_pluto_list;
GLuint gen_outer_box;
GLuint stars_sky_list;

bool  Freeze;

GLuint sun_texture;
GLuint mercury_texture;
GLuint venus_texture;
GLuint earth_texture;
GLuint moon_texture;
GLuint mars_texture;
GLuint jupiter_texture;
GLuint saturn_texture;
GLuint saturn_band_texture;
GLuint uranus_texture;
GLuint uranus_band_texture;
GLuint neptune_texture;
GLuint pluto_texture;
GLuint carboard_tex;
GLuint stars_sky_texture;


int center_point_mer;
int center_point_ven;
int center_point_ear;
int center_point_mars;
int center_point_jup;
int center_point_sat;
int center_point_uran;
int center_point_nep;
int center_point_pluto;

float White[] = { 1., 1., 1., 1. };

bool Light0On = 1;
bool Light1On = 1;
float angle = 0;
float Time;
float time;


#define MS_PER_CYCLE 100000


// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void	HsvRgb( float[3], float [3] );


struct point {
	float x, y, z;		// coordinates
	float nx, ny, nz;	// surface normal
	float s, t;		// texture coords
};

int		NumLngs, NumLats;
struct point *	Pts;

struct point * PtsPointer(int lat, int lng)
{
	if (lat < 0)	lat += (NumLats - 1);
	if (lng < 0)	lng += (NumLngs - 1);
	if (lat > NumLats - 1)	lat -= (NumLats - 1);
	if (lng > NumLngs - 1)	lng -= (NumLngs - 1);
	return &Pts[NumLngs*lat + lng];
}


void DrawPoint(struct point *p)
{
	glNormal3f(p->nx, p->ny, p->nz);
	glTexCoord2f(p->s, p->t);
	glVertex3f(p->x, p->y, p->z);
}

void OsuSphere(float radius, int slices, int stacks)
{
	struct point top, bot;		// top, bottom points
	struct point *p;

	// set the globals:

	NumLngs = slices;
	NumLats = stacks;

	if (NumLngs < 3)
		NumLngs = 3;

	if (NumLats < 3)
		NumLats = 3;


	// allocate the point data structure:

	Pts = new struct point[NumLngs * NumLats];


	// fill the Pts structure:

	for (int ilat = 0; ilat < NumLats; ilat++)
	{
		float lat = -M_PI / 2. + M_PI * (float)ilat / (float)(NumLats - 1);
		float xz = cos(lat);
		float y = sin(lat);
		for (int ilng = 0; ilng < NumLngs; ilng++)
		{
			float lng = -M_PI + 2. * M_PI * (float)ilng / (float)(NumLngs - 1);
			float x = xz * cos(lng);
			float z = -xz * sin(lng);
			p = PtsPointer(ilat, ilng);
			p->x = radius * x;
			p->y = radius * y;
			p->z = radius * z;
			p->nx = x;
			p->ny = y;
			p->nz = z;
			p->s = (lng + M_PI) / (2.*M_PI);
			p->t = (lat + M_PI / 2.) / M_PI;
		}
	}

	top.x = 0.;		top.y = radius;	    top.z = 0.;
	top.nx = 0.;	top.ny = 1.;		top.nz = 0.;
	top.s = 0.;		top.t = 1.;

	bot.x = 0.;		bot.y = -radius;	bot.z = 0.;
	bot.nx = 0.;	bot.ny = -1.;		bot.nz = 0.;
	bot.s = 0.;		bot.t = 0.;


	// connect the north pole to the latitude NumLats-2:

	glBegin(GL_QUADS);
	for (int ilng = 0; ilng < NumLngs - 1; ilng++)
	{
		p = PtsPointer(NumLats - 1, ilng);
		DrawPoint(p);

		p = PtsPointer(NumLats - 2, ilng);
		DrawPoint(p);

		p = PtsPointer(NumLats - 2, ilng + 1);
		DrawPoint(p);

		p = PtsPointer(NumLats - 1, ilng + 1);
		DrawPoint(p);
	}
	glEnd();

	// connect the south pole to the latitude 1:

	glBegin(GL_QUADS);
	for (int ilng = 0; ilng < NumLngs - 1; ilng++)
	{
		p = PtsPointer(0, ilng);
		DrawPoint(p);

		p = PtsPointer(0, ilng + 1);
		DrawPoint(p);

		p = PtsPointer(1, ilng + 1);
		DrawPoint(p);

		p = PtsPointer(1, ilng);
		DrawPoint(p);
	}
	glEnd();


	// connect the other 4-sided polygons:

	glBegin(GL_QUADS);
	for (int ilat = 2; ilat < NumLats - 1; ilat++)
	{
		for (int ilng = 0; ilng < NumLngs - 1; ilng++)
		{
			p = PtsPointer(ilat - 1, ilng);
			DrawPoint(p);

			p = PtsPointer(ilat - 1, ilng + 1);
			DrawPoint(p);

			p = PtsPointer(ilat, ilng + 1);
			DrawPoint(p);

			p = PtsPointer(ilat, ilng);
			DrawPoint(p);
		}
	}
	glEnd();

	delete[] Pts;
	Pts = NULL;
}

float *
Array3(float a, float b, float c)
{
	static float array[4];
	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = 1.;
	return array;
}

// utility to create an array from 4 separate values:
float * Array4(float r, float g, float b, float a)
{
	static float array[4];
	array[0] = r;
	array[1] = g;
	array[2] = b;
	array[3] = a;
	return array;
}

// utility to create an array from a multiplier and an array:
float *
MulArray3(float factor, float array0[3])
{
	static float array[4];
	array[0] = factor * array0[0];
	array[1] = factor * array0[1];
	array[2] = factor * array0[2];
	array[3] = 1.;
	return array;
}

void
SetMaterial(float r, float g, float b, float shininess)
{
	glMaterialfv(GL_BACK, GL_EMISSION, Array3(0., 0., 0.));
	glMaterialfv(GL_BACK, GL_AMBIENT, MulArray3(.4f, White));
	glMaterialfv(GL_BACK, GL_DIFFUSE, MulArray3(1., White));
	glMaterialfv(GL_BACK, GL_SPECULAR, Array3(0., 0., 0.));
	glMaterialf(GL_BACK, GL_SHININESS, 2.f);
	glMaterialfv(GL_FRONT, GL_EMISSION, Array3(0., 0., 0.));
	glMaterialfv(GL_FRONT, GL_AMBIENT, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Array3(r, g, b));
	glMaterialfv(GL_FRONT, GL_SPECULAR, MulArray3(.8f, White));
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void
SetPointLight(int ilight, float x, float y, float z, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));
	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);
}

void
SetSpotLight(int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array3(x, y, z));
	glLightfv(ilight, GL_SPOT_DIRECTION, Array3(xdir, ydir, zdir));
	glLightf(ilight, GL_SPOT_EXPONENT, 1.);
	glLightf(ilight, GL_SPOT_CUTOFF, 30.);
	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));
	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);
}

void
SetParallelLight(int ilight, float x, float y, float z, float r, float g, float b)
{
	glLightfv(ilight, GL_POSITION, Array4(x, y, z, 0.));
	glLightfv(ilight, GL_AMBIENT, Array3(0., 0., 0.));
	glLightfv(ilight, GL_DIFFUSE, Array3(r, g, b));
	glLightfv(ilight, GL_SPECULAR, Array3(r, g, b));
	glLightf(ilight, GL_CONSTANT_ATTENUATION, 1.);
	glLightf(ilight, GL_LINEAR_ATTENUATION, 0.);
	glLightf(ilight, GL_QUADRATIC_ATTENUATION, 0.);
	glEnable(ilight);
}


void axis_around_system(float a, float b)
{
	glColor3f(1., 1., 1.);
	glBegin(GL_LINE_STRIP);
	for (float i = 0; i <= 5*3.14; i += 0.01)
	{
		glVertex3f(a*cosf(i),0,b*sinf(i));
		
	}
	glEnd();
}

void band_around_planet(float r1, float r2, int n)
{
	float intial_angle = 0.;
	float sum_angle = 2 * M_PI / n;
	glBegin(GL_QUADS);
	for (int i = 0; i < n; i++)
	{
		glNormal3f(0, 1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(r1*cos(intial_angle), 0., r1*sin(intial_angle));
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 0);
		glVertex3f(r2*cos(intial_angle), 0., r2*sin(intial_angle));
		glNormal3f(0, 1, 0);
		glTexCoord2f(0, 1);
		glVertex3f(r2*cos(intial_angle+sum_angle), 0., r2*sin(intial_angle+sum_angle));
		glNormal3f(0, 1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(r1*cos(intial_angle + sum_angle), 0., r1*sin(intial_angle + sum_angle));
		intial_angle += sum_angle;
	}
	glEnd();
}

void window(float r1, float r2, float h)
{
	float angle = 2* M_PI / 500;
	float s0 = 1. / 50;
	point p1, p2, p3, p4;
	float nx, ny, nz;
	glBegin(GL_QUADS);
	for (int i = 0; i < 500; i++)
	{
		p1.x = r1*cos((float)i*angle);
		p1.y = h / 2;
		p1.z = r1*sin((float)i*angle);
		p1.s = (float)i * s0;
		p1.t = 1;
		
		p2.x = r1*cos((float)(i+1)*angle);
		p2.y = h / 2;
		p2.z = r1*sin((float)(i+1)*angle);
		p2.s = ((float)i + 1.) * s0;
		p2.t = 1;
		
		p3.x = r2*cos((float)(i+1)*angle);
		p3.y = - h / 2;
		p3.z = r2*sin((float)(i+1)*angle);
		p3.s = (float)(i+1) * s0;
		p3.t = 0;
		
		p4.x = r2*cos((float)i*angle);
		p4.y = - h / 2;
		p4.z = r2*sin((float)i*angle);
		p4.s = (float)i * s0;
		p4.t = 0;
		
		nx = ((p2.y - p1.y)*(p3.z - p1.z) - (p2.z - p1.z)*(p3.y - p1.y));
		ny = ((p2.z - p1.z)*(p3.x - p1.x) - (p2.x - p1.x)*(p3.z - p1.z));
		nz = ((p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x));

		glNormal3f(nx, ny, nz);
		glTexCoord2f(p1.s, p1.t);
		glVertex3f(p1.x, p1.y, p1.z);

		glTexCoord2f(p2.s, p2.t);
		glVertex3f(p2.x, p2.y, p2.z);

		glTexCoord2f(p3.s, p3.t);
		glVertex3f(p3.x, p3.y, p3.z);

		glTexCoord2f(p4.s, p4.t);
		glVertex3f(p4.x, p4.y, p4.z);
	}
	glEnd();

}

int generate_rand_origin(int lower_range, int top_range)
{
	int num;
	int temp;
	temp = top_range - lower_range;
	num = rand() % temp + lower_range;
	return num;
}


// utility to create an array from 3 separate values:

// main program:

int
main( int argc, char *argv[ ] )
{

	glutInit( &argc, argv );


	// setup all the graphics stuff:

	InitGraphics( );


	// create the display structures that will not change:

	InitLists( );


	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );


	// setup all the user interface stuff:

	InitMenus( );


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );

	glutMainLoop( );


	// this is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate( )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:

	// force a call to Display( ) next time it is convenient:
	int ms = glutGet(GLUT_ELAPSED_TIME);
	int msp = ms;
	ms %= MS_PER_CYCLE;
	Time = (float)ms / (float)(MS_PER_CYCLE - 1);
	time += 0.05;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}


	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );


	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glShadeModel(GL_SMOOTH);

	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.1, 2000. );
	else
		gluPerspective( 90., 1.,	0.1, 2000. );


	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );


	// set the eye position, look-at position, and up-vector:
	if (visit_planet == 0)
	{
		if (WhichView == 0)
		{
			gluLookAt(0., 0., 100., 0., 0., 0., 0., 1., 0.);
			glTranslatef((GLfloat)move_axes_x, (GLfloat)move_axes_y, (GLfloat)move_axes_z);
		}
		else if (WhichView == 1)
		{
			gluLookAt(0., 0., 41.5, 0., 0., 0., 0., 1., 0.);
		}
	}

	else if (visit_planet == 1)
	{
		gluLookAt((5.2 + (sin(2 * M_PI*Time) + 1) / 2 * 28.)*cos(2 * M_PI*Time), 4., (5.2 + (sin(2 * M_PI*Time) + 1) / 2 * 28.)*sin(2 * M_PI*Time), (5.2 + (sin(2 * M_PI*Time) + 1) / 2 * 28.) * cos(2 * M_PI*Time + M_PI / 3), 2., (5.2 + (sin(2 * M_PI*Time) + 1) / 2 * 28.) * sin(2 * M_PI*Time + M_PI / 3), sin(2 * M_PI*Time)*0.3, 1., 0.);
		WhichView = 1;
	}

	SetParallelLight(GL_LIGHT1, 1., 1., 0.75, 1., 1., 1.);


	//translate the scene:
	if (WhichView == 1)
	{
		glTranslatef((GLfloat)move_axes_x, (GLfloat)move_axes_y, (GLfloat)move_axes_z);
	}

	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0., 1., 0. );
	glRotatef( (GLfloat)Xrot, 1., 0., 0. );


	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );

	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}


	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3fv( &Colors[WhichColor][0] );
		glCallList( AxesList );
	}


	// since we are using glScalef( ), be sure normals get unitized:

	glEnable( GL_NORMALIZE );

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, MulArray3(.2, White));
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	//point light for sun 
	SetPointLight(GL_LIGHT0, 0., 0., 0., 1., 1., 1.);
	

	// draw the current object:

	glEnable(GL_LIGHTING);
	
	//outside
	if (WhichView == 0)
	{
		
		glEnable(GL_LIGHTING);
		glPushMatrix();
		glCallList(gen_outer_box);
		glPopMatrix();
		glDepthMask(GL_TRUE);
		glDisable(GL_LIGHTING);
		
	}
	
	else if (WhichView == 1)
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT1);
		glEnable(GL_LIGHT0);
		glPushMatrix();
		glRotatef(270,0,0,0);
		glCallList(stars_sky_list);
		glPopMatrix();
		// glEnable(GL_LIGHTING);
	}

	glPushMatrix();
	glRotatef(45., 0., 1., 0.);
	glRotatef(30., 0., 0., 1.);


	//sun
	glPushMatrix();
		glDisable(GL_LIGHTING);
		glRotatef(360 * Time, 0., 1., 0.);
		glCallList(gen_sun_list);
		glEnable(GL_LIGHTING);
	glPopMatrix();
	
	//mercury
	glDisable(GL_LIGHTING);
	axis_around_system(5.2,4);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(5.2*cosf(time),0,4*sinf(time));
	glRotated(center_point_mer, 0, 1, 0);
	glRotatef(360 * center_point_ven/10 * Time, 0., 1., 0.);
	SetMaterial(1.0, 1.0, 1.0, 2.);
	glCallList( gen_mercury_list );
	glPopMatrix();
	
	//venus
	glDisable(GL_LIGHTING);
	axis_around_system(8.2,5.5);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(8.2*cosf(time*0.5),0,5.5*sinf(time*0.5));
	glRotated(center_point_ear, 0, 1, 0);
	glRotatef(360 * center_point_ven/10 * Time, 0., 1., 0.);
	SetMaterial(1.0, 1.0, 1.0, 2.);
	glCallList( gen_venus_list );
	glPopMatrix();
	
	//earth 
	glDisable(GL_LIGHTING);
	axis_around_system(11.2, 7.0);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(11.2*cosf(time*0.35),0,7.0*sinf(time*0.35));
	glRotated(center_point_ear, 0, 1, 0);
	glRotatef(360 * center_point_ven/10 * Time, 0., 1., 0.);
	SetMaterial(1.0, 1.0, 1.0, 2.);
	glCallList( gen_earth_List );
	glDisable(GL_LIGHTING);
	glPopMatrix();
	
	//mars
	glDisable(GL_LIGHTING);
	axis_around_system(14.2, 8.5);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(14.2*cosf(time*0.3),0,8.5*sinf(time*0.3));
	glRotated(center_point_mars, 0, 1, 0);
	glRotatef(360 * center_point_ven/10 * Time, 0., 1., 0.);
	SetMaterial(1.0, 1.0, 1.0, 2.);
	glCallList( gen_mars_list );
	glPopMatrix();
	
	//jupiter
	glDisable(GL_LIGHTING);
	axis_around_system(17.2, 11.25);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(17.2*cosf(time*0.09),0,11.25*sinf(time*0.09));
	glRotated(center_point_jup, 0, 1, 0);
	glRotatef(360 * center_point_ven/10 * Time, 0., 1., 0.);
	SetMaterial(1.0, 1.0, 1.0, 2.);
	glCallList( gen_jupiter_list );
	glPopMatrix();
	
	//saturn
	glDisable(GL_LIGHTING);
	axis_around_system(22.45, 15.5);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(22.45*cosf(time*0.07),0,15.5*sinf(time*0.07));
	glRotatef(40., -1., 0., 1.);
	glRotated(center_point_sat, 0, 1, 0);
	glRotatef(360 * center_point_ven/10 * Time, 0., 1., 0.);
	SetMaterial(1.0, 1.0, 1.0, 2.);
	glCallList( gen_saturn_list );
	glPopMatrix();
	
	//uranus
	glDisable(GL_LIGHTING);
	axis_around_system(27.45, 19.5);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(27.45*cosf(time*0.06),0,19.5*sinf(time*0.06));
	glRotatef(-80., 1., 0.35, 0.25);
	glRotated(center_point_uran, 0, 1, 0);
	glRotatef(360 * center_point_ven/10 * Time, 0., 1., 0.);
	SetMaterial(1.0, 1.0, 1.0, 2.);
	glCallList( gen_uranus_list );
	glPopMatrix();
	
	//neptune
	glDisable(GL_LIGHTING);
	axis_around_system(33.0, 22.5);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(33.0*cosf(time*0.05),0,22.5*sinf(time*0.05));
	glRotated(center_point_nep, 0, 1, 0);
	glRotatef(360 * center_point_ven/10 * Time, 0., 1., 0.);
	SetMaterial(1.0, 1.0, 1.0, 2.);
	glCallList( gen_neptune_list );
	glPopMatrix();
	
	//pluto
	glDisable(GL_LIGHTING);
	axis_around_system(37.2, 26.0);
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(37.2*cosf(time*0.04),0,26.0*sinf(time*0.04));
	glRotated(center_point_pluto, 0, 1, 0);
	glRotatef(360 * center_point_ven/10 * Time, 0., 1., 0.);
	SetMaterial(1.0, 1.0, 1.0, 2.);
	glCallList( gen_pluto_list );
	glPopMatrix();
	

	

	glDisable(GL_LIGHTING);

	
	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0., 100.,     0., 100. );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glColor3f( 1., 1., 1. );
	DoRasterString( 5., 5., 0., "Solar System " );


	// swap the double-buffered framebuffers:

	glutSwapBuffers( );


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	WhichColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}

void DoViewMenu(int id)
{
	WhichView = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


void Do_visit(int id)
{
	visit_planet = id;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}

// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the num of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int viewmenu = glutCreateMenu( DoViewMenu );
	glutAddMenuEntry( "Outside",  0 );
	glutAddMenuEntry( "Inside",  1 );

	int visit_menu = glutCreateMenu( Do_visit );
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Colors",        colormenu);
	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddSubMenu(   "View",          viewmenu);
	glutAddSubMenu(   "Vist Planets",   visit_menu);
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
	unsigned char *Texture;
	int Width, Height;
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( Animate );

	/*Textures*/

	//Sun 
	Texture = BmpToTexture("sun.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &sun_texture);
	glBindTexture(GL_TEXTURE_2D, sun_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Mercury 
	Texture = BmpToTexture("mercurymap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &mercury_texture);
	glBindTexture(GL_TEXTURE_2D, mercury_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Venus 
	Texture = BmpToTexture("venusmap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &venus_texture);
	glBindTexture(GL_TEXTURE_2D, venus_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Earth 
	Texture = BmpToTexture("earthmap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &earth_texture);
	glBindTexture(GL_TEXTURE_2D, earth_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Moon 
	Texture = BmpToTexture("moonmap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &moon_texture);
	glBindTexture(GL_TEXTURE_2D, moon_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);
	//Mars Texture
	Texture = BmpToTexture("marsmap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &mars_texture);
	glBindTexture(GL_TEXTURE_2D, mars_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);


	//Jupiter 
	Texture = BmpToTexture("jupitermap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &jupiter_texture);
	glBindTexture(GL_TEXTURE_2D, jupiter_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Saturn 
	Texture = BmpToTexture("saturnmap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &saturn_texture);
	glBindTexture(GL_TEXTURE_2D, saturn_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Saturn Ring 
	Texture = BmpToTexture("saturnring.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &saturn_band_texture);
	glBindTexture(GL_TEXTURE_2D, saturn_band_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Uranus Texture
	Texture = BmpToTexture("uranusmap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &uranus_texture);
	glBindTexture(GL_TEXTURE_2D, uranus_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Uranus Ring 
	Texture = BmpToTexture("uranusring.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &uranus_band_texture);
	glBindTexture(GL_TEXTURE_2D, uranus_band_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Neptune 
	Texture = BmpToTexture("neptunemap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &neptune_texture);
	glBindTexture(GL_TEXTURE_2D, neptune_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Pluto 
	Texture = BmpToTexture("plutomap.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &pluto_texture);
	glBindTexture(GL_TEXTURE_2D, pluto_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//intial points
	center_point_mer = generate_rand_origin(0, 360);
	center_point_ven = generate_rand_origin(0, 360);
	center_point_ear = generate_rand_origin(0, 360);
	center_point_mars = generate_rand_origin(0, 360);
	center_point_jup = generate_rand_origin(0, 360);
	center_point_sat = generate_rand_origin(0, 360);
	center_point_uran = generate_rand_origin(0, 360);
	center_point_nep = generate_rand_origin(0, 360);
	center_point_pluto = generate_rand_origin(0, 360);

	//box Texture
	Texture = BmpToTexture("carboard_tex.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &carboard_tex);
	glBindTexture(GL_TEXTURE_2D, carboard_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	//Sky Texture
	Texture = BmpToTexture("stars_sky.bmp", &Width, &Height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &stars_sky_texture);
	glBindTexture(GL_TEXTURE_2D, stars_sky_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture);

	

	// init glew (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow( MainWindow );

	// Sun:
	gen_sun_list = glGenLists( 1 );
	glNewList( gen_sun_list, GL_COMPILE );
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, sun_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(3., 100, 100);
	glDisable(GL_TEXTURE_2D);
	glEndList( );

	//Mercury
	gen_mercury_list = glGenLists(1);
	glNewList(gen_mercury_list, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mercury_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(0.5, 100, 100);
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//Venus
	gen_venus_list = glGenLists(1);
	glNewList(gen_venus_list, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, venus_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(1., 100, 100);
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//Earth
	gen_earth_List = glGenLists(1);
	glNewList(gen_earth_List, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, earth_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(1., 100, 100);
	glDisable(GL_TEXTURE_2D);
	glEndList();
	
	//Mars
	gen_mars_list = glGenLists(1);
	glNewList(gen_mars_list, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mars_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(0.75, 100, 100);
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//Jupiter
	gen_jupiter_list = glGenLists(1);
	glNewList(gen_jupiter_list, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, jupiter_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(2.125, 100, 100);
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//Saturn
	gen_saturn_list = glGenLists(1);
	glNewList(gen_saturn_list, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, saturn_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(2., 100, 100);
	glBindTexture(GL_TEXTURE_2D, saturn_band_texture);
	glColor3f(1., 1., 1.);
	band_around_planet(2.25, 3.25, 50);
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//Uranus
	gen_uranus_list = glGenLists(1);
	glNewList(gen_uranus_list, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uranus_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(1.625, 100, 100);
	glBindTexture(GL_TEXTURE_2D, uranus_band_texture);
	glColor3f(1., 1., 1.);
	band_around_planet(1.75, 2.375, 50);
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//Neptune
	gen_neptune_list = glGenLists(1);
	glNewList(gen_neptune_list, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, neptune_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(1.625, 100, 100);
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//Pluto
	gen_pluto_list = glGenLists(1);
	glNewList(gen_pluto_list, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pluto_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(0.8, 100, 100);
	glDisable(GL_TEXTURE_2D);
	glEndList();

	//star_sky
	stars_sky_list = glGenLists( 1 );
	glNewList( stars_sky_list, GL_COMPILE );
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, stars_sky_texture);
	glColor3f(1., 1., 1.);
	OsuSphere(70., 100, 100); // giginatic sphere
	glDisable(GL_TEXTURE_2D);
	glEndList( );
	

	//wood stage
	gen_outer_box = glGenLists(1);
	glNewList(gen_outer_box, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, carboard_tex);
	glColor3f(1., 1., 1.);
	glutSolidCube(100);
	glRotatef(90,0,0,0);
	glTranslatef(0.,60,0);
	window(20., 20., 50.);
	glDepthMask(GL_FALSE);
	glDisable(GL_TEXTURE_2D);
	glEndList();
	glBegin(GL_POLYGON);
	glNormal3f(0., 1., 0.);

	// //sky box
	// glEnable(GL_LIGHTING);
	// stars_sky_list = glGenLists(1);
	// glNewList(stars_sky_list, GL_COMPILE);
	// glEnable(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE_2D, stars_sky_texture);
	// glColor3f(1., 1., 1.);
	// OsuSphere(100,100,100); //giginatic sphere
	// glDisable(GL_TEXTURE_2D);
	// glDisable(GL_LIGHTING);

	glEndList();
	
	// create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		case 'f':
		case 'F':
		        Freeze =! Freeze;
        if(Freeze)
            glutIdleFunc(NULL);
        else
            glutIdleFunc(Animate);
        break;

		case 'w':
		case 'W':
			move_axes_z += 0.75;
			if (move_axes_z >= 75.)
			{
				move_axes_z = 75.;
			}
			break;

		case 's':
		case 'S':
			move_axes_z -= 0.75;
			if (move_axes_z <= -75.)
			{
				move_axes_z = -75.;
			}
			break;

		case 'a':
		case 'A':
			move_axes_x += 0.5;
			if (move_axes_x >= 75.)
			{
				move_axes_x = 75.;
			}
			break;

		case 'd':
		case 'D':
			move_axes_x -= 0.5;
			if (move_axes_x <= -75.)
			{
				move_axes_x = -75.;
			}
			break;

		case 'x':
		case 'X':
			move_axes_y += 0.5;
			if (move_axes_y >= 75.)
			{
				move_axes_y = 75.;
			}
			break;

		case 'z':
		case 'Z':
			move_axes_y -= 0.5;
			if (move_axes_y <= -75.)
			{
				move_axes_y = -75.;
			}
			break;

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}


	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
	move_axes_x = move_axes_y = move_axes_z = 0.;
	WhichView = 0;
	Freeze = false;
	visit_planet = 0;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[ ] = {
		1, 2, -3, 4
		};

static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[ ] = {
		1, 2, 3, -2, 4
		};

static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r, g, b;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}
