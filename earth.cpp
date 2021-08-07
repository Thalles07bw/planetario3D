//note you need a bitmap file named data/EarthTexture.bmp to run this
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h> //for malloc/free
#include <math.h>

float anglex = 35.0;
float angley =-35.0;
float anglez =  0.0;
float locZ = 0.0;
float locY = 0.0;
float locX = 0.0;

GLUquadric *earth;
GLuint earthTexture;

int LoadBitmap(char *filename)
{
    FILE * file;
    char temp;
    long i;

    // own version of BITMAPINFOHEADER from windows.h for Linux compile
    struct {
      int biWidth;
      int biHeight;
      short int biPlanes;
      unsigned short int biBitCount;
      unsigned char *data;
    } infoheader;

    GLuint num_texture;

    if( (file = fopen(filename, "rb"))==NULL) return (-1); // Open the file for reading

    fseek(file, 18, SEEK_CUR);  /* start reading width & height */
    fread(&infoheader.biWidth, sizeof(int), 1, file);

    fread(&infoheader.biHeight, sizeof(int), 1, file);

    fread(&infoheader.biPlanes, sizeof(short int), 1, file);
    if (infoheader.biPlanes != 1) {

      return 0;
    }

    // read the bpp
    fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
    if (infoheader.biBitCount != 24) {

      return 0;
    }

    fseek(file, 24, SEEK_CUR);

    // read the data
    if(infoheader.biWidth<0){
      infoheader.biWidth = -infoheader.biWidth;
    }
    if(infoheader.biHeight<0){
      infoheader.biHeight = -infoheader.biHeight;
    }
    infoheader.data = (unsigned char *) malloc(infoheader.biWidth * infoheader.biHeight * 3);
    if (infoheader.data == NULL) {
      printf("Error allocating memory for color-corrected image data");
      return 0;
    }

    if ((i = fread(infoheader.data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
      printf("Error reading image data from %s.", filename);
      return 0;
    }

    for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { // reverse all of the colors. (bgr -> rgb)
      temp = infoheader.data[i];
      infoheader.data[i] = infoheader.data[i+2];
      infoheader.data[i+2] = temp;
    }


    fclose(file); // Closes the file stream

    glGenTextures(1, &num_texture);
    glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);

    // And create 2d mipmaps for the minifying function
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);
    
    free(infoheader.data); // Free the memory we used to load the texture

    return (num_texture); // Returns the current texture OpenGL ID
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27: exit(0); break;
      case 'a': anglex+=5; break;
      case 'A': anglex-=5; break;
      case 's': angley-=5; break;
      case 'S': angley+=5; break;
      case 'd': anglez+=5; break;
      case 'D': anglez-=5; break;
      case 'u': locZ-=0.05; break;
      case 'U': locZ+=0.05; break;
      case 'k': locY+=0.05; break;
      case 'j': locY-=0.05; break;
      case 'l': locX+=0.05; break;
      case 'h': locX-=0.05; break;
      default: break; 
   }
   glutPostRedisplay();
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glEnable ( GL_TEXTURE_2D );
   glBindTexture ( GL_TEXTURE_2D, earthTexture);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   //rotate/translate entire scene with a,A,s,S,D,D and h,l,j,k,u,U keys
   glRotatef(anglez,0.0,0.0,1.0);
   glRotatef(angley,0.0,1.0,0.0);
   glRotatef(anglex,1.0,0.0,0.0);

   //draw textured sphere
   glPushMatrix();
      glTranslatef(locX,locY,locZ);
      glScalef(0.5,0.5,0.5);
      gluSphere( earth, 0.9, 36, 72);
   glPopMatrix();

   glDisable ( GL_TEXTURE_2D );

   glutSwapBuffers();
}

void init (void) 
{
  glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);
     
  earth = gluNewQuadric();
  gluQuadricTexture( earth, GL_TRUE);
  earthTexture = LoadBitmap("Textura/planet_4.bmp");
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutCreateWindow ("Testing Texture");
   init ();
   glutDisplayFunc(display);
   glutCreateWindow ("Testing Texture");
   init ();
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0; 
}
