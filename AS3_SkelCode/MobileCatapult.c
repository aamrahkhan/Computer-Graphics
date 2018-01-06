#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <SOIL.h>
#include "MobileCatapult.h"

#define SW 1024
#define SH 720
#define NUMPTS 18097
#define MESHDIM 150
#define TERR_D MESHDIM*2.5
#define TERR_W MESHDIM*2.5

float cam_xrot=-190, cam_yrot=0, cam_zrot=0;
float tyre_rot = 1;
float car_move = 1;
float plat_rot = 1;
float arm1_rot = 0;
float arm2_rot = 0;
float drill_rot = 1;

GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 };

float terrainHeight[MESHDIM][MESHDIM] = {0,0};
int terrainH , initH;
int xNew, zNew;
float angleY=0 ,  angleZ;
GLdouble pos[3] = {75 , 3 , 75};
GLdouble vel[3] = {0 , 0 , 0};
static GLuint texName[6];
static GLuint texWood, texWall;

void gl_draw();
void gl_init(int w, int h);
void draw_Terrain();
void Normalize3(GLdouble *v);
void resizePointData();
void drawAxes();
void drawCatapult();
void drawPlatform(double l, double w, double h);
void drawWheel(double r, double h);
void drawArm(double r, double h);
void drawBowl(double r, double h);
void drawBase(double r, double h);

GLuint loadTexture(const char* path);
void findNormal(int x , int z , int id, GLdouble *res);

void keyboard (unsigned char key, int x, int y);
void camera();
void reshape (int w, int h);
void cleanup(){}

///////////////////////////////////////////////////////////
void initLight(void)
{
  GLfloat ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat position[] = { 0.0, 3.0, 2.0, 0.0 };
  GLfloat lmodel_ambient[] = { 0.4, 0.8, 0.4, 1.0 };
  GLfloat local_view[] = { 0.0 };

  glClearColor(0.0, 0.1, 0.1, 0.0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

///////////////////////////////////////////////////////////
// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(const char* path)
{
  //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    int width, height;
    
    // Load texture
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    printf("%d,%d\n", width, height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); 
    SOIL_free_image_data(image);
    
    // Parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glShadeModel(GL_SMOOTH);
    //glBindTexture(GL_TEXTURE_2D, 0);   

    return textureID;
}


///////////////////////////////////////////////////////////
void drawSkybox()
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   
  for(int j =0; j<6;j++){
       glBindTexture(GL_TEXTURE_2D, texName[j]);
       glBegin(GL_QUADS);
         for(int i=0; i<4; i++){
          glTexCoord2f(cV[i+j*4][3], 1-cV[i+j*4][4]); glVertex3f(TERR_W*cV[i+j*4][0],TERR_W*cV[i+j*4][1],TERR_D*cV[i+j*4][2]);
       }
       glEnd();
       glBindTexture(GL_TEXTURE_2D, 0); 
    }  

    glEnd();       
    glFlush();
    glDisable(GL_TEXTURE_2D); 
}

///////////////////////////////////////////////////////////
void updateTerrainH(){

  texName[0] = loadTexture("textures/skybox/others/front.jpg");
  texName[1] = loadTexture("textures/skybox/others/back.jpg");
  texName[2] = loadTexture("textures/skybox/others/left.jpg");
  texName[3] = loadTexture("textures/skybox/others/right.jpg");
  texName[4] = loadTexture("textures/skybox/others/bottom.jpg");
  texName[5] = loadTexture("textures/skybox/others/top.jpg");
  texWood = loadTexture("textures/wood.png");
  texWall = loadTexture("textures/bbrickwall.jpg");
}

///////////////////////////////////////////////////////////
void resizePointData(){
  float minx=9999,miny=9999,minz=9999,maxx=-9999,maxy=-9999,maxz=-9999;

  for(int t=0; t<NUMPTS; t++){
    minx = minx > dat[t][0] ? dat[t][0] : minx;
    miny = miny > dat[t][1] ? dat[t][1] : miny;
    minz = minz > dat[t][2] ? dat[t][2] : minz;

    maxx = maxx <= dat[t][0] ? dat[t][0] : maxx;
    maxy = maxy <= dat[t][1] ? dat[t][1] : maxy;
    maxz = maxz <= dat[t][2] ? dat[t][2] : maxz;    
  }
  printf("min:(%f,%f,%f)\n max:(%f,%f,%f)\n",minx,miny,minz,maxx,maxy,maxz);

  for(int t=0; t<NUMPTS; t++){
    dat[t][0] = MESHDIM*(dat[t][0]-minx)/(maxx-minx);
    dat[t][1] = MESHDIM*(dat[t][1]-miny)/(maxy-miny);
    dat[t][2] = MESHDIM*(dat[t][2]-minz)/(maxz-minz);
  } 
}

///////////////////////////////////////////////////////////
void findNormal(int x , int z , int id, GLdouble *res){
  GLdouble line1[3] ;
  GLdouble line2[3] ;
  if (id == 1){
    line1[0] = 1; line1[1] = (GLdouble)terrainHeight[x+1][z] - (GLdouble)terrainHeight[x][z+1]; line1[2] = -1;
    line2 [0] = -1; line2[1] = (GLdouble)terrainHeight[x][z] - (GLdouble)terrainHeight[x+1][z]; line2[2] = 0 ;
  }else{
    line1[0] = 0; line1[1] = (GLdouble)terrainHeight[x+1][z] - (GLdouble)terrainHeight[x+1][z+1]; line1[2] = -1;
    line2 [0] = -1; line2[1] = (GLdouble)terrainHeight[x][z+1] - (GLdouble)terrainHeight[x+1][z]; line2[2] = 1 ;    
  }
  res[0] = line1[1]*line2[2] - line1[2]*line2[1];
  res[1] = line1[2]*line2[0] - line1[0]*line2[2];
  res[2] = line1[0]*line2[1] - line1[1]*line2[0];

  Normalize3(res);
}

///////////////////////////////////////////////////////////
GLdouble dotProd(GLdouble *a , GLdouble *b){
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

///////////////////////////////////////////////////////////
void Normalize3(GLdouble *v){
   GLdouble len = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
   v[0] /= len;
   v[1] /= len;
   v[2] /= len;
}


//////////////////////////////////////////////////////////
void drawAxes(){
   glColor3d(1,0,0);
   glBegin(GL_LINES);
     glVertex3f(0,0,0);
     glVertex3f(3,0,0);
   glEnd();

   glColor3d(0,1,0);
   glBegin(GL_LINES);
     glVertex3f(0,0,0);
     glVertex3f(0,3,0);
   glEnd();

   glColor3d(0,0,1);
   glBegin(GL_LINES);
     glVertex3f(0,0,0);
     glVertex3f(0,0,3);
   glEnd();
}

///////////////////////////////////////////////////////////
void gl_draw(){
  //printf("In gl_draw \n");

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLfloat lightpos[] = {0, 50, 85, 0};
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  
  glColor3f(0.0, 1.0, 0.0);
  camera();
  drawSkybox();
  drawAxes();
  drawCatapult();

  glutSwapBuffers();
}

///////////////////////////////////////////////////////////
void gl_init(int w, int h){
  printf("In gl_init \n");

  // Setup Camera
  glClearColor(0.0, 0.0, 1.0, 1.0);
  initLight();  

  glViewport(0, 0, w, h);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);

  texName[0] = loadTexture("textures/skybox/front.jpg");
  texName[1] = loadTexture("textures/skybox/back.jpg");
  texName[2] = loadTexture("textures/skybox/left.jpg");
  texName[3] = loadTexture("textures/skybox/right.jpg");
  texName[4] = loadTexture("textures/skybox/bottom.jpg");
  texName[5] = loadTexture("textures/skybox/top.jpg");
  texWood = loadTexture("textures/wood.png");
  texWall = loadTexture("textures/bbrickwall.jpg");
    
  resizePointData();
  updateTerrainH();
}

///////////////////////////////////////////////////////////
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);   
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(120.0, 1.0, 0.0001, MESHDIM*8);
   glMatrixMode(GL_MODELVIEW);
   camera();
}

///////////////////////////////////////////////////////////
void camera(){
   glLoadIdentity();
   glTranslatef (0, 0.0,-MESHDIM);
   glRotatef(cam_xrot,1,0,0);
   glRotatef(cam_yrot,0,1,0);
   glRotatef(cam_zrot,0,0,1);
}

/////////////////////////////////////////////////
void drawCatapult()
{
  //TYPE YOUR CODE HERE
  double l1, w1, h1, wheelheight, wheelrad, baseheight, baserad, armheight, armrad;
  l1 = 20;
  w1 = 10;
  h1 = 30;
  wheelheight = 3;
  wheelrad = 8;
  baseheight = 5;
  baserad = 4;
  armheight = 30;
  armrad = 0.8;
  glPushMatrix();
    glTranslatef(0, 0, car_move);
    glRotatef(plat_rot, 0, 1, 0);
    drawPlatform(20, 10, 30);
    glPushMatrix();
      glTranslatef(0, w1+10, h1/4);
      glRotatef(drill_rot, 0, 1, 0);
      drawBase(baserad, baseheight);
      glPushMatrix();
        glRotatef(arm1_rot, 1, 0, 0);
        drawArm(armrad, armheight);
      glPopMatrix();
    glPopMatrix();


    glPushMatrix();
      glTranslatef(-l1/2 - wheelheight, 10, -h1/2);
      glRotatef(tyre_rot, 1, 0, 0);
      drawWheel(wheelrad, wheelheight);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-l1/2 - wheelheight, 10, h1/2);
      glRotatef(tyre_rot, 1, 0, 0);
      drawWheel(wheelrad, wheelheight);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(l1/2, 10, -h1/2);
      glRotatef(tyre_rot, 1, 0, 0);
      drawWheel(wheelrad, wheelheight);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(l1/2, 10, h1/2);
      glRotatef(tyre_rot, 1, 0, 0);
      drawWheel(wheelrad, wheelheight);
    glPopMatrix();
  glPopMatrix();
}

void drawPlatform(double l1, double w1, double h1)
{
glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

glBindTexture(GL_TEXTURE_2D, texWood);

  glBegin(GL_QUADS);
    //TYPE YOUR CODE HERE
    glVertex3f(-200, 200, -300);
    glVertex3f(200, 200, 300);
    glVertex3f(-200, 200, 300);
    glVertex3f(200, 200, -300);

  glEnd();
  glPushMatrix();
    glTranslatef(0.0, 10.0, 0.0);
    glScalef (l1, w1, h1);
    glutSolidCube(1);
  glPopMatrix();
glBindTexture(GL_TEXTURE_2D, 0);

glFlush();
glDisable(GL_TEXTURE_2D); 


}

void drawWheel(double r, double h)
{
  //TYPE YOUR CODE HERE
  GLUquadricObj *obj = gluNewQuadric();
  glPushMatrix();
    glRotatef(90, 0, 1, 0);
    //glTranslatef(0.0, 40, 0.0);
    gluCylinder(obj, r, r, h, 32, 32);
    gluDisk(obj, 0.5, r, 32, 32);
    glTranslatef(0.0, 0.0, h);
    gluDisk(obj, 0.5, r, 32, 32);
  glPopMatrix();
}

void drawBase(double r, double h)
{
  GLUquadricObj *obj3 = gluNewQuadric();
  glPushMatrix();
    //glTranslatef(0.0, 40, 0.0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(obj3, r, r, h, 32, 32);
    gluDisk(obj3, 0, r, 32, 32);
    glTranslatef(0.0, 0.0, h);
    gluDisk(obj3, 0, r, 32, 32);
  glPopMatrix();
}

void drawArm(double r, double h)
{
  GLUquadricObj *obj3 = gluNewQuadric();
  double bowlheight = 3.5;
  double bowlrad = 2;
  glPushMatrix();
    //glTranslatef(0.0, 40, 0.0);
    glRotatef(-45, 1, 0, 0);
    glPushMatrix();
      glTranslatef(0.0, 0, h + bowlheight/2);
      glRotatef(arm2_rot, 0, 0, 1);
      drawBowl(bowlrad, bowlheight);
    glPopMatrix();
    gluCylinder(obj3, r, r, h, 32, 32);
    gluDisk(obj3, 0, r, 32, 32);
    glTranslatef(0.0, 0.0, h);
    gluDisk(obj3, 0, r, 32, 32);
  glPopMatrix();
}

void drawBowl(double r, double h)
{
  GLUquadricObj *obj3 = gluNewQuadric();
  glPushMatrix();
    //glTranslatef(0.0, 10, 0.0);
    glRotatef(-90, 1, 0, 0);
    gluCylinder(obj3, r, r+1, h, 32, 32);
    gluCylinder(obj3, r-1, r, h, 32, 32);
    gluDisk(obj3, 0, r, 32, 32);
    glTranslatef(0.0, 0.0, h);
    gluDisk(obj3, r, r+1, 32, 32);
  glPopMatrix();
}
///////////////////////////////////////////////////////////
void specialkeys(int key, int x, int y)
{
  switch (key) {
  case GLUT_KEY_UP:
    car_move -= 3;
    tyre_rot -= 10;
    break;

  case GLUT_KEY_DOWN:
    car_move += 3;
    tyre_rot -= 10;
    break;
  case GLUT_KEY_RIGHT:
    drill_rot -=10;  
    break;
  case GLUT_KEY_LEFT:
    drill_rot += 10;
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

///////////////////////////////////////////////////////////
void keyboard (unsigned char key, int x, int y)
{   
   // Camera controls - Rotation along principle axis
   switch (key) {
      case 'q':
       cam_xrot += 1;
       if (cam_xrot >360) cam_xrot -= 360;
         break;
      case 'z':
         cam_xrot -= 1;
       if (cam_xrot < -360) cam_xrot += 360;
         break;
      case 'a':
       cam_yrot += 1;
       if (cam_yrot >360) cam_yrot -= 360;
         break;
      case 'd':
         cam_yrot -= 1;
       if (cam_yrot < -360) cam_yrot += 360;
         break;
      case 'w':
       cam_zrot += 1;
       if (cam_zrot >360) cam_zrot -= 360;
         break;
      case 'x':
       cam_zrot -= 1;
       if (cam_zrot < -360) cam_zrot += 360;
         break;
        //TYPE YOUR CODE HERE
      case 'k':
        if (arm1_rot > -135)
          arm1_rot -= 10;
          break;
      case 'm':
        if (arm1_rot < 45)
          arm1_rot += 10;
        break;
      case 'g':
        //if (arm1_rot > -180 && arm1_rot < 180)
          arm2_rot += 10;
          break;
      case 'h':
        //if (arm1_rot > -180 && arm1_rot < 180)
          arm2_rot -= 10;
          break;
      case 'f':
        //if (arm1_rot > -180 && arm1_rot < 180)
          plat_rot += 10;
          break;
      case 'j':
        //if (arm1_rot > -180 && arm1_rot < 180)
          plat_rot -= 10;
          break;
    default:
         break;
   }
   glutPostRedisplay();
}

///////////////////////////////////////////////////////////
int main(int argc, char **argv){
  printf("In main \n");

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
   glutInitWindowSize(SW, SH);
   glutInitWindowPosition(100, 50);
   glutCreateWindow(argv[0]);
   glutDisplayFunc(gl_draw);
   glutReshapeFunc(reshape);
   glutSpecialFunc(specialkeys);
   glutKeyboardFunc(keyboard);   
   gl_init(SW, SH);

   glutMainLoop();

   return EXIT_SUCCESS;
}