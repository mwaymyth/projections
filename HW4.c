/*
 *  HW4: Perspective
 *
 *  Creating a scene to be viewed in both orthogonal and perspective projections
 *  from multiple eye positions directed by user input
 *  Has 3 view modes:
 *  1) Overhead Orthogonal
 *  2) Overhead perspective
 *  3) First person
 *
 *  Key bindings:
 *  a          Toggle axes
 *  arrows     Change view angle
 *  0          Reset view angle
 *  ESC        Exit
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <stdarg.h>
 #include <math.h>
 //  OpenGL with prototypes for glext
 #define GL_GLEXT_PROTOTYPES
 #ifdef __APPLE__
 #include <GLUT/glut.h>
 #else
 #include <GL/glut.h>
 #endif

 int th=0;         //  Azimuth of view angle
 int ph=0;         //  Elevation of view angle
 int fov = 55;     //  Field of view (for perspective)
 double zh=0;      //  Rotation of teapot
 int axes=0;       //  Display axes
 int mode=0;       //  What to display
 double asp=1;     //  Aspect Ratio
 double dim=5.0;   //  Size of world
 char* text[] = {"Orthogonal", "Perpective", "First Person"};
 int fo=0;         //  Forward
 int hor=0;        //  Horizontal

 //  Macro for sin & cos in degrees
 #define Cos(th) cos(3.1415926/180*(th))
 #define Sin(th) sin(3.1415926/180*(th))

 /*
  *  Convenience routine to output raster text
  *  Use VARARGS to make this more flexible
  */
 #define LEN 8192  //  Maximum length of text string
 void Print(const char* format , ...)
 {
    char    buf[LEN];
    char*   ch=buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args,format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
 }

 /*
  *  Set projection
  */
 static void Project()
 {
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    if (mode == 1 | mode == 2)
       gluPerspective(fov,asp,dim/4,4*dim);
    //  Orthogonal projection
    else
       glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
 }

 /*
  *  Draw a house
  *     at (x,y,z)
  *     dimensions (dx,dy,dz)
  *     rotated th about the y axis
  */
 static void house(double x,double y,double z,
                  double dx,double dy,double dz,
                  double th)
 {
   // Dimensions for house
   double wd = 1;
   double ht = 1;

   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glVertex3f(-wd,-ht, 1);
   glVertex3f(wd,-ht, 1);
   glVertex3f(wd,ht, 1);
   glVertex3f(-wd,ht, 1);
   //  Back
   glColor3f(0,0,1);
   glVertex3f(wd,-ht,-1);
   glVertex3f(-wd,-ht,-1);
   glVertex3f(-wd,ht,-1);
   glVertex3f(wd,ht,-1);
   //  Right
   glColor3f(1,1,0);
   glVertex3f(wd,-ht,+1);
   glVertex3f(wd,-ht,-1);
   glVertex3f(wd,ht,-1);
   glVertex3f(wd,ht,+1);
   //  Left
   glColor3f(0,1,0);
   glVertex3f(-wd,-ht,-1);
   glVertex3f(-wd,-ht,+1);
   glVertex3f(-wd,ht,+1);
   glVertex3f(-wd,ht,-1);
   //  Roof Right
   glColor3f(0,1,1);
   glVertex3f(wd+0.5,ht-0.5,+1);
   glVertex3f(wd+0.5,ht-0.5,-1);
   glVertex3f(0,+2,-1);
   glVertex3f(0,+2,+1);
   //  Roof Left
   glColor3f(0,1,1);
   glVertex3f(-wd -0.5,ht-0.5,-1);
   glVertex3f(-wd -0.5,ht-0.5,+1);
   glVertex3f(0,+2,+1);
   glVertex3f(0,+2,-1);
   //  Bottom
   glColor3f(1,0,1);
   glVertex3f(-wd,-ht,-1);
   glVertex3f(wd,-ht,-1);
   glVertex3f(wd,-ht,+1);
   glVertex3f(-wd,-ht,+1);
   // Chimney Front
   glColor3f(1,0,1);
   glVertex3f(+0.75,+1, -0.25);
   glVertex3f(+1,+1, -0.25);
   glVertex3f(+1,+1.5,-0.25);
   glVertex3f(+0.75,+1.5, -0.25);
   // Chimney Right
   glColor3f(1,1,0);
   glVertex3f(+1,+1, -0.25);
   glVertex3f(+1,+1, -0.5);
   glVertex3f(+1,+1.5,-0.5);
   glVertex3f(+1,+1.5, -0.25);
   // Chimney Back
   glColor3f(0,0,1);
   glVertex3f(+1,+1, -0.5);
   glVertex3f(+0.75,+1, -0.5);
   glVertex3f(+0.75,+1.5, -0.5);
   glVertex3f(+1,+1.5,-0.5);
   // Chimney Left
   glColor3f(0,1,0);
   glVertex3f(+0.75,+1, -0.5);
   glVertex3f(+0.75,+1, -0.25);
   glVertex3f(+0.75,+1.5, -0.25);
   glVertex3f(+0.75,+1.5, -0.5);
   // Chimney Top
   glColor3f(0,1,1);
   glVertex3f(+0.75,+1.5, -0.25);
   glVertex3f(+1,+1.5,-0.25);
   glVertex3f(+1,+1.5,-0.5);
   glVertex3f(+0.75,+1.5, -0.5);
   //  End
   glEnd();
   glBegin(GL_TRIANGLES);
   // Front triangle
   glColor3f(1,0,0);
   glVertex3f(wd,ht, 1);
   glVertex3f(0,+2, 1);
   glVertex3f(-wd,ht, 1);
   // Back triangle
   glColor3f(0,0,1);
   glVertex3f(-wd,ht,-1);
   glVertex3f(0, +2, -1);
   glVertex3f(wd,ht,-1);

   glEnd();
   //  Undo transformations
   glPopMatrix();
 }

 /*
  *  Draw solid airplane
  *    at (x,y,z)
  *    nose towards (dx,dy,dz)
  *    up towards (ux,uy,uz)
  */
 static void SolidPlane(double x,double y,double z,
                        double dx,double dy,double dz,
                        double ux,double uy, double uz)
 {
    // Dimensions used to size airplane
    const double wid=0.05;
    const double nose=+0.50;
    const double cone= 0.20;
    const double wing= 0.00;
    const double strk=-0.20;
    const double tail=-0.50;
    //  Unit vector in direction of flght
    double D0 = sqrt(dx*dx+dy*dy+dz*dz);
    double X0 = dx/D0;
    double Y0 = dy/D0;
    double Z0 = dz/D0;
    //  Unit vector in "up" direction
    double D1 = sqrt(ux*ux+uy*uy+uz*uz);
    double X1 = ux/D1;
    double Y1 = uy/D1;
    double Z1 = uz/D1;
    //  Cross product gives the third vector
    double X2 = Y0*Z1-Y1*Z0;
    double Y2 = Z0*X1-Z1*X0;
    double Z2 = X0*Y1-X1*Y0;
    //  Rotation matrix
    double mat[16];
    mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
    mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
    mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
    mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;

    //  Save current transforms
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glMultMatrixd(mat);
    //  Nose (4 sided)
    glColor3f(0,0,1);
    glBegin(GL_TRIANGLES);
    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone, wid, wid);
    glVertex3d(cone,-wid, wid);

    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone, wid,-wid);
    glVertex3d(cone,-wid,-wid);

    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone, wid, wid);
    glVertex3d(cone, wid,-wid);

    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone,-wid, wid);
    glVertex3d(cone,-wid,-wid);
    glEnd();
    //  Fuselage (square tube)
    glBegin(GL_QUADS);
    glVertex3d(cone, wid, wid);
    glVertex3d(cone,-wid, wid);
    glVertex3d(tail,-wid, wid);
    glVertex3d(tail, wid, wid);

    glVertex3d(cone, wid,-wid);
    glVertex3d(cone,-wid,-wid);
    glVertex3d(tail,-wid,-wid);
    glVertex3d(tail, wid,-wid);

    glVertex3d(cone, wid, wid);
    glVertex3d(cone, wid,-wid);
    glVertex3d(tail, wid,-wid);
    glVertex3d(tail, wid, wid);

    glVertex3d(cone,-wid, wid);
    glVertex3d(cone,-wid,-wid);
    glVertex3d(tail,-wid,-wid);
    glVertex3d(tail,-wid, wid);

    glVertex3d(tail,-wid, wid);
    glVertex3d(tail, wid, wid);
    glVertex3d(tail, wid,-wid);
    glVertex3d(tail,-wid,-wid);
    glEnd();
    //  Wings (plane triangles)
    glColor3f(1,1,0);
    glBegin(GL_TRIANGLES);
    glVertex3d(wing, 0.0, wid);
    glVertex3d(tail, 0.0, wid);
    glVertex3d(tail, 0.0, 0.5);

    glVertex3d(wing, 0.0,-wid);
    glVertex3d(tail, 0.0,-wid);
    glVertex3d(tail, 0.0,-0.5);
    glEnd();
    //  Vertical tail (plane triangle)
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    glVertex3d(strk, 0.0, 0.0);
    glVertex3d(tail, 0.3, 0.0);
    glVertex3d(tail, 0.0, 0.0);
    glEnd();
    //  Undo transformations
    glPopMatrix();
 }

 /*
  *  OpenGL (GLUT) calls this routine to display the scene
  */
 void display()
 {
    const double len=1.5;  //  Length of axes
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();

     double Ex = 0;
     double Ey = 0;
     double Ez = 10;
     double Cx = 0;
     double Cy = 0;
     double Cz = 0;
     switch (mode)
     {
         case 0:
             glRotatef(ph,1,0,0);
             glRotatef(th,0,1,0);
             break;
         case 1:
             Ex = -2*dim*Sin(th)*Cos(ph);
             Ey = +2*dim        *Sin(ph);
             Ez = +2*dim*Cos(th)*Cos(ph);
             gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
             break;
         case 2:
            Ex = hor;
            Ey = .5;
            Ez += fo*1;
            Cx = Ex + th*.05;
            Cy = Ey - Cos(ph);
            Cz = 0 - Ez;
           gluLookAt(Ex,Ey,Ez , Cx,Cy,Cz , 0,1,0);
           break;
     }
    //  Draws 2 houses and a plane
    house(1,0,0 , 0.6,0.6,0.3 , 15);
    house(0,0,0.5 , 0.2,0.2,0.2 , 45);
    SolidPlane( -1, 1, 0 , 1,0,0 , 0, 5, 5);

    //  White
    glColor3f(1,1,1);
    //  Draw axes
    if (axes)
    {
       glBegin(GL_LINES);
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(len,0.0,0.0);
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(0.0,len,0.0);
       glVertex3d(0.0,0.0,0.0);
       glVertex3d(0.0,0.0,len);
       glEnd();
       //  Label axes
       glRasterPos3d(len,0.0,0.0);
       Print("X");
       glRasterPos3d(0.0,len,0.0);
       Print("Y");
       glRasterPos3d(0.0,0.0,len);
       Print("Z");
    }
    //  Five pixels from the lower left corner of the window
    glWindowPos2i(5,5);
    //  Print the text string
    Print("Angle=%d,%d   Dim=%.1f  FOV=%d   Projection=%s",th,ph,dim,fov,text[mode]);
    //  Render the scene
    glFlush();
    //  Make the rendered scene visible
    glutSwapBuffers();
 }

 /*
  *  GLUT calls this routine when an arrow key is pressed
  */
 void special(int key,int x,int y)
 {
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
       th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
       th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
       ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
       ph -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_UP)
       dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
       dim -= 0.1;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Update projection
    Project();
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
 }

 /*
  *  GLUT calls this routine when a key is pressed
  */
 void key(unsigned char ch,int x,int y)
 {
    //  Exit on ESC
    if (ch == 27)
       exit(0);
    //  Reset view angle
    else if (ch == '0')
       th = ph = 0;
    //  Switch display mode
    else if (ch == 'm')
       mode = (mode+1)%3;
    else if (ch == 'M')
      mode = (mode+2)%3;
    else if (ch == 'w')
      fo--;
    else if (ch == 's')
      fo++;
    else if (ch == 'a')
      hor--;
    else if (ch == 'd')
      hor++;
    //  Change field of view angle
    else if (ch == '-' && ch>1)
       fov--;
    else if (ch == '+' && ch<179)
       fov++;
    //  Reproject
    Project();

    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
 }

 /*
  *  GLUT calls this routine when the window is resized
  */
 void reshape(int width,int height)
 {
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project();
 }


 /*
  *  Start up GLUT and tell it what to do
  */
 int main(int argc,char* argv[])
 {
    //  Initialize GLUT
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Projections");
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
 }
