
/* Derived from scene.c in the The OpenGL Programming Guide */

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

	/* flags used to control the appearance of the image */
int lineDrawing = 1;	// draw polygons as solid or lines
int lighting = 0;	// use diffuse and specular lighting
int smoothShading = 0;  // smooth or flat shading
int textures = 0;

GLubyte  Image[64][64][4];
GLuint   textureID[1];

char str[256];
int maxDepth = 1;
float angle = 25.0f;

typedef struct camera {
   float x;
   float y;
   float z;
} Camera;

Camera cam;

/*
   Drawing tree
*/
void runTree(char* str, int i, int depth, int maxDepth)
{
   while (str[i] != '\0')
   {
      if (str[i] == 'F')
      {
         //move and draw sphere
         glTranslatef(0, 1, 0);
         glutSolidSphere (0.25, 10, 10);
         //next layer
         if (depth < maxDepth)
         {  
            runTree(str, 0, depth + 1, maxDepth);
         }
      }
      else if (str[i] == '[')
      {
         glPushMatrix();
      }
      else if (str[i] == ']')
      {
         glPopMatrix();
      }
      else if (str[i] == '-')
      {
         glRotatef(-angle, 0, 0, 1);
      }
      else if (str[i] == '+')
      {
         glRotatef(angle, 0, 0, 1);
      }
      i++;
   }
   
}

/*  Initialize material property and light source.
 */
void init (void)
{
   GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_full_off[] = {0.0, 0.0, 0.0, 1.0};
   GLfloat light_full_on[] = {1.0, 1.0, 1.0, 1.0};

   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

	/* if lighting is turned on then use ambient, diffuse and specular
	   lights, otherwise use ambient lighting only */
   if (lighting == 1) {
      glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
      glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
      glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
   } else {
      glLightfv (GL_LIGHT0, GL_AMBIENT, light_full_on);
      glLightfv (GL_LIGHT0, GL_DIFFUSE, light_full_off);
      glLightfv (GL_LIGHT0, GL_SPECULAR, light_full_off);
   }
   glLightfv (GL_LIGHT0, GL_POSITION, light_position);
   
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
}

void display (void)
{
GLfloat blue[]  = {0.0, 0.0, 1.0, 1.0};
GLfloat red[]   = {1.0, 0.0, 0.0, 1.0};
GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0, 1.0};

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw surfaces as either smooth or flat shaded */
   if (smoothShading == 1)
      glShadeModel(GL_SMOOTH);
   else
      glShadeModel(GL_FLAT);

	/* draw polygons as either solid or outlines */
   if (lineDrawing == 1)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else 
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   
	/* give all objects the same shininess value */
   glMaterialf(GL_FRONT, GL_SHININESS, 30.0);

	/* set colour of sphere */
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
   glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	/* move to initial location */
   glPushMatrix ();
   glTranslatef(cam.x, cam.y, cam.z);
   
	/* turn texturing on */
   if (textures == 1) {
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, textureID[0]);
	/* if textured, then use white as base colour */
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
   }

   //drawing tree
   runTree(str, 0, 1, maxDepth);

   if (textures == 1) 
      glDisable(GL_TEXTURE_2D);
   glPopMatrix ();

   glFlush ();
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 10000.0);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
      case 'q':
         exit(0);
         break;
      case '1':		// draw polygons as outlines
         lineDrawing = 1;
         lighting = 0;
         smoothShading = 0;
         textures = 0;
         init();
         display();
         break;
      case '2':		// draw polygons as filled
         lineDrawing = 0;
         lighting = 0;
         smoothShading = 0;
         textures = 0;
         init();
         display();
         break;
      case '3':		// diffuse and specular lighting, flat shading
         lineDrawing = 0;
         lighting = 1;
         smoothShading = 0;
         textures = 0;
         init();
         display();
         break;
      case '4':		// diffuse and specular lighting, smooth shading
         lineDrawing = 0;
         lighting = 1;
         smoothShading = 1;
         textures = 0;
         init();
         display();
         break;
      case '5':		// texture with  smooth shading
         lineDrawing = 0;
         lighting = 1;
         smoothShading = 1;
         textures = 1;
         init();
         display();
         break;
      case 'w':		// move cam up
         cam.y -= 1.0f;
         display();
         break;
      case 's':		// move cam down
         cam.y += 1.0f;
         display();
         break;
   }
}

void mouse (int button, int state, int x, int y)
{
   switch (button) {
      case 3:  //Zoom in
         cam.z += 1.0f;
         display();
         break;
      case 4:  //Zoom out
         cam.z -= 1.0f;
         display();
         break;
   }
}

void loadTexture() {
FILE *fp;
int  i, j;
int  red, green, blue;

   if ((fp = fopen("image.txt", "r")) == 0) {
      printf("Error, failed to find the file named image.txt.\n");
      exit(0);
   } 

   for(i=0; i<64; i++) {
      for(j=0; j<64; j++) {
         fscanf(fp, "%d %d %d", &red, &green, &blue);
         Image[i][j][0] = red;
         Image[i][j][1] = green;
         Image[i][j][2] = blue;
         Image[i][j][3] = 255;
      }
   }

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glGenTextures(1,textureID);
   glBindTexture(GL_TEXTURE_2D, textureID[0]);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, Image);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glEnable(GL_TEXTURE_GEN_S);
   glEnable(GL_TEXTURE_GEN_T);

   fclose(fp);
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
   //checking for file argument
   if (argc != 2)
   {
      printf("Incorrect number of arguments.\n");
      return 1;
   }

   //opening file
   FILE* f = fopen(argv[1], "r");

   //couldn't open file
   if (f == NULL)
   {
      printf("File not found: %s\n", argv[1]);
      return 1;
   }

   //reading in values
   int num = fscanf(f, "%d %f %255s", &maxDepth, &angle, str);

   //closing file
   fclose(f);

   //checking for correct amount of data
   if (num != 3)
   {
      printf("File has incorect fields.\n");
      return 1;
   }

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (1440, 960);
   glutCreateWindow (argv[0]);
   init();

   //setting initial camera position
   cam.x = 0;
   cam.y = -10;
   cam.z = -50.0;

   loadTexture();
   glutReshapeFunc (reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc (keyboard);
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0; 
}

