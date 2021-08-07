#include <iostream>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <GL/glut.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__

#endif

#define FALSE 0
#define TRUE 1

// ----------------------------------------------------------
// Declarações de Funções
// ----------------------------------------------------------
void display();
void specialKeys();

// ----------------------------------------------------------
// Variáveis Globais
// ---------------------------------------------------------- 

GLuint texture[5];
double rotate_x=0;
double rotate_z=0;
GLfloat yRotated;
double orthoValues[2] = {110,-110};

// Parâmetros da luz da estrela
GLfloat light_ambient[] = { 10, 10, 10, 1.0 };
GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };

void desenhaCirculo (float cx, float cy, float r, int pontos) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < pontos; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(pontos); //faz o calculo do angulo theta
        float x = r * cosf(theta);//calcula componente x 
        float y = r * sinf(theta);//calcula componente y
        glVertex3f(x + cx,0, y + cy);//desenha os vertices
    }
    glEnd();
}
int LoadBitmap(const char *filename) // Função utilizada para carregar bitmap
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

void applyBitmaps();
//Determinando os parâmetros das esferas que serão desenhadas
void sun(){

  GLUquadricObj *sphere=NULL;
  sphere = gluNewQuadric();
  gluQuadricDrawStyle(sphere, GLU_FILL);
  gluQuadricTexture(sphere, true);
  gluQuadricNormals(sphere, GLU_SMOOTH);
  gluSphere(sphere, 12, 50, 50);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
 
}
void planet_1(){
  GLUquadricObj *sphere=NULL;
  sphere = gluNewQuadric();
  gluQuadricDrawStyle(sphere, GLU_FILL);
  gluQuadricTexture(sphere, true);
  gluQuadricNormals(sphere, GLU_SMOOTH);
  gluSphere(sphere, 0.7, 50, 50);
 
}
void planet_2(){
  GLUquadricObj *sphere=NULL;
  sphere = gluNewQuadric();
  gluQuadricDrawStyle(sphere, GLU_FILL);
  gluQuadricTexture(sphere, true);
  gluQuadricNormals(sphere, GLU_SMOOTH);
  gluSphere(sphere, 1.3, 50, 50);
 
}
void planet_3(){
  GLUquadricObj *sphere=NULL;
  sphere = gluNewQuadric();
  gluQuadricDrawStyle(sphere, GLU_FILL);
  gluQuadricTexture(sphere, true);
  gluQuadricNormals(sphere, GLU_SMOOTH);
  gluSphere(sphere, 2, 50, 50);
 
}
void planet_4(){
  GLUquadricObj *sphere=NULL;
  sphere = gluNewQuadric();
  gluQuadricDrawStyle(sphere, GLU_FILL);
  gluQuadricTexture(sphere, true);
  gluQuadricNormals(sphere, GLU_SMOOTH);
  gluSphere(sphere, 4.4, 50, 50);
 
}
 
void idle(void){ //Função para rotação continua da cena
	yRotated += 0.1;
  display();
}

void init(void){
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    applyBitmaps();
}

void applyBitmaps(){ //Carrega cada bitmap em uma posição do array de textura
  texture[0] = LoadBitmap("Textura/Sun.bmp");
  texture[1] = LoadBitmap("Textura/planet_1.bmp");
  texture[2] = LoadBitmap("Textura/planet_2.bmp");
  texture[3] = LoadBitmap("Textura/planet_3.bmp");
  texture[4] = LoadBitmap("Textura/planet_4.bmp");
}
// ----------------------------------------------------------
// função display() 
// ----------------------------------------------------------
void display(){

  // Limpa a tela e o Z-Buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_PROJECTION);
  
  // Reinicia transformações
  glLoadIdentity();
  
  glOrtho(orthoValues[1], orthoValues[0], orthoValues[1], orthoValues[0], orthoValues[1], orthoValues[0]);//Define os limites para x,y e z
  
  // Desenha a trajetória das orbitas
  
  glPushMatrix();
  glColor3f(0,0,1);
  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  desenhaCirculo(0,0,16.00,200);
  desenhaCirculo(0,0,27.58,200);
  desenhaCirculo(0,0,51.22,200);
  desenhaCirculo(0,0,95.52,200);
  glPopMatrix();

  
  // Centro do sistema planetário  
  glPushMatrix();
  glutPostRedisplay();
  glEnable ( GL_TEXTURE_2D );
  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  sun();
  glPopMatrix();
  
  glRotatef( rotate_x, 1.0, 0.0, 0.0 );
  glRotatef( yRotated, 0.0, 1.0, 0.0 );


  
  // Objetos orbitando
  glPushMatrix();
  glTranslatef(16, 0, 0);
  glBindTexture( GL_TEXTURE_2D, texture[1]);
  planet_1();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-20, 0, 19);
  glBindTexture( GL_TEXTURE_2D, texture[2]);
  planet_2();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-40, 0, 32);
  glBindTexture( GL_TEXTURE_2D, texture[3]);
  planet_3();
  glPopMatrix();

   glPushMatrix();
   glTranslatef(70, 0, -65);
   glRotatef( 90, 1.0, 0.0, 0.0 );
   glBindTexture( GL_TEXTURE_2D, texture[4]);
   planet_4();
   glPopMatrix();


  glEnd();

  // Recuperar o estado antrior da fonte luminosa
  glPopAttrib();
  glFlush();
  glutSwapBuffers();
 
}

// ------------------------------------------------------------
// Função specialKeys(), controla a inclinação no eixo z para troca do angulo de visão 
// ------------------------------------------------------------
void specialKeys( int key, int x, int z) {
 
  
  // Seta para cima - gira em 5 graus em torno  do eixo x
  if (key == GLUT_KEY_UP)
    rotate_x += 5;
  // Seta para cima - gira em 5 graus em torno  do eixo x
  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 5;

  //  Requisitar atualização do display
  glutPostRedisplay();
 
}

void JumpToPlanet(int planetNumber){
  if(planetNumber == 1){
      orthoValues [0] = 16.7;
      orthoValues [1] = -16.7;
  }else if(planetNumber == 2){
      orthoValues [0] = 30;
      orthoValues [1] = -30;
  }else if(planetNumber == 3){
      orthoValues [0] = 55;
      orthoValues [1] = -55;
  }else if(planetNumber == 4){
      orthoValues [0] = 110;
      orthoValues [1] = -110;
  }

}
//Teclas que determinam quantas orbitas estarão visiveis
void KeyboardPressed(unsigned char key, int A, int B)
{
	switch (key)
	{

		case '1': JumpToPlanet(1); break;
		case '2': JumpToPlanet(2); break;
		case '3': JumpToPlanet(3); break;
		case '4': JumpToPlanet(4); break;
	
	}
}
// ----------------------------------------------------------
// Função main()
// ----------------------------------------------------------
int main(int argc, char* argv[]){
  std::cout<<"Utilize as teclas de 1 a 4 para definir quantas orbitas serão visiveis"<<std::endl;
  std::cout<<"Utilize as setas pra cima e pra baixo para rotacionar a cena no eixo z"<<std::endl;
 
  //Inicializa o GLUT e processa os parâmetros do usuário GLUT
  glutInit(&argc,argv);
  glutInitWindowSize(800,600);
  //  Requisita uma janela com buffer duplo e true color com um Z-buffer
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 
  //Cria a janela do programa
  glutCreateWindow("3D Design Planetario");

  //Habilita o teste de profundidade do Z-buffer
  glEnable(GL_DEPTH_TEST);
  yRotated = 180.0;
  // Funções
  init();
  glutDisplayFunc(display);
  glutSpecialFunc(specialKeys);
  glutKeyboardFunc(KeyboardPressed);
  glutIdleFunc(idle);
  //Passa o controle dos eventos para o GLUT
  glutMainLoop();
 
  //Retorna para o SO
  return 0;
 
}