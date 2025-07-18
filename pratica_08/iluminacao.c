/**
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define PI 3.1415

GLint WIDTH = 320;
GLint HEIGHT = 240;

GLint fatias = 30;
GLint pilhas = 30;
GLint raioEsfera = 1.5;

GLfloat obs[3] = {0.0, 7.0, 0.0};
GLfloat olho[3] = {0.0, 3.0, 0.0};

GLfloat plano_difusa[] = {0.5, 0.5, 0.0, 1.0};
GLfloat plano_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat plano_brilho[] = {50.0};

GLfloat mat_a_difusa[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_a_especular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_a_brilho[] = {50.0};

GLfloat mat_b_difusa[] = {0.7, 0.7, 0.7, 0.5};
GLfloat mat_b_especular[] = {1.0, 1.0, 1.0, 0.5};
GLfloat mat_b_brilho[] = {50.0};

GLfloat posicao_luz0[] = {0.0, 10.0, 0.0, 1.0};
GLfloat cor_luz0[] = {1.0, 1.0, 1.0, 1.0};
GLfloat cor_luz0_amb[] = {0.3, 0.3, 0.3, 1.0};

GLfloat posicao_luz1[] = {0.0, 10.0, 5.0, 1.0};
GLfloat cor_luz1[] = {0.0, 0.0, 1.0, 1.0};
GLfloat direcao_luz1[] = {0.0, -10.0, -5.0, 1.0};
GLint spot_luz1 = 30;

GLfloat sem_cor[] = {0.0, 0.0, 0.0, 1.0};

GLint gouraud = 0;

GLfloat tetaxz = 0;
GLfloat raioxz = 6;

int last_x = -1, last_y = -1;
int shift_pressed = 0;

void reshape(int width, int height)
{
  WIDTH = width;
  HEIGHT = height;
  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(70.0, width / (float)height, 0.1, 30.0);
  glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);

  glDepthMask(GL_TRUE);
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (gouraud)
  {
    glShadeModel(GL_SMOOTH);
  }
  else
  {
    glShadeModel(GL_FLAT);
  }

  glPushMatrix();

  obs[0] = raioxz * cos(2 * PI * tetaxz / 360);
  obs[2] = raioxz * sin(2 * PI * tetaxz / 360);
  gluLookAt(obs[0], obs[1], obs[2], olho[0], olho[1], olho[2], 0.0, 1.0, 0.0);

  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plano_difusa);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plano_especular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, plano_brilho);

  glNormal3f(0, 1, 0);
  glBegin(GL_QUADS);
  glVertex3f(-10, 0, 10);
  glVertex3f(10, 0, 10);
  glVertex3f(10, 0, -10);
  glVertex3f(-10, 0, -10);
  glEnd();

  glPushMatrix();
  glTranslatef(posicao_luz0[0], posicao_luz0[1], posicao_luz0[2]);
  glColor3f(1, 0, 0);
  glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz0);
  glutSolidSphere(0.3, 5, 5);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(posicao_luz1[0], posicao_luz1[1], posicao_luz1[2]);
  glMaterialfv(GL_FRONT, GL_EMISSION, cor_luz1);
  glutSolidSphere(0.3, 5, 5);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_EMISSION, sem_cor);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_a_difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_a_especular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_a_brilho);

  glPushMatrix();
  glTranslatef(0.0, 3.0, -3.0);
  glutSolidSphere(raioEsfera, fatias, pilhas);
  glPopMatrix();

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_b_difusa);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_b_especular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_b_brilho);

  glTranslatef(0.0, +3.0, +3.0);

  glutSolidSphere(raioEsfera, fatias, pilhas);

  glPopMatrix();
  glutSwapBuffers();
}

void special(int key, int x, int y)
{
  switch (key)
  {
  case GLUT_KEY_UP:
    obs[1] = obs[1] + 1;
    glutPostRedisplay();
    break;
  case GLUT_KEY_DOWN:
    obs[1] = obs[1] - 1;
    glutPostRedisplay();
    break;
  case GLUT_KEY_LEFT:
    tetaxz = tetaxz + 2;
    glutPostRedisplay();
    break;
  case GLUT_KEY_RIGHT:
    tetaxz = tetaxz - 2;
    glutPostRedisplay();
    break;
  }
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:
    exit(0);
    break;
  case 'g':
    gouraud = !gouraud;
    glutPostRedisplay();
    break;
  case 'r':
    raioxz = raioxz + 1;
    glutPostRedisplay();
    break;
  case 'R':
    if (raioxz > 1)
    {
      raioxz = raioxz - 1;
      glutPostRedisplay();
    }
    break;
  }
}

// GLUT não fornece callback direto para SHIFT, então usamos modificadores no mouse
void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN)
  {
    last_x = x;
    last_y = y;
    int mods = glutGetModifiers();
    shift_pressed = (mods & GLUT_ACTIVE_SHIFT) ? 1 : 0;
  }
  else if (state == GLUT_UP)
  {
    last_x = -1;
    last_y = -1;
    shift_pressed = 0;
  }
}

// Movimentação do mouse: ajusta transparência ou iluminação ambiente
void motion(int x, int y)
{
  if (last_x < 0 || last_y < 0)
    return;

  int dx = x - last_x;
  int dy = y - last_y;

  if (shift_pressed)
  {
    // Ajuste da iluminação ambiente
    float delta = (dx - dy) * 0.01f;
    for (int i = 0; i < 3; ++i)
    {
      cor_luz0_amb[i] += delta;
      if (cor_luz0_amb[i] < 0.0f)
        cor_luz0_amb[i] = 0.0f;
      if (cor_luz0_amb[i] > 1.0f)
        cor_luz0_amb[i] = 1.0f;
    }
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz0_amb);
  }
  else
  {
    // Ajuste da transparência do objeto B
    float delta = (dx - dy) * 0.005f;
    mat_b_difusa[3] += delta;
    mat_b_especular[3] += delta;
    if (mat_b_difusa[3] < 0.05f)
      mat_b_difusa[3] = 0.05f;
    if (mat_b_difusa[3] > 1.0f)
      mat_b_difusa[3] = 1.0f;
    if (mat_b_especular[3] < 0.05f)
      mat_b_especular[3] = 0.05f;
    if (mat_b_especular[3] > 1.0f)
      mat_b_especular[3] = 1.0f;
  }

  last_x = x;
  last_y = y;
  glutPostRedisplay();
}

void init()
{
  gouraud = 1;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz0_amb);
  glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);

  glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz1);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_luz1);
  glLightfv(GL_LIGHT1, GL_POSITION, posicao_luz1);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcao_luz1);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
}

void menu(int value)
{
  switch (value)
  {
  case 0:
    mat_a_especular[0] = mat_a_especular[1] = mat_a_especular[2] = 0.0;
    break;
  case 1:
    mat_a_especular[0] = mat_a_especular[1] = mat_a_especular[2] = 0.5;
    break;
  case 2:
    mat_a_especular[0] = mat_a_especular[1] = mat_a_especular[2] = 1.0;
    break;
  case 3:
    mat_a_difusa[0] = mat_a_difusa[1] = mat_a_difusa[2] = 0.0;
    break;
  case 4:
    mat_a_difusa[0] = mat_a_difusa[1] = mat_a_difusa[2] = 0.5;
    break;
  case 5:
    mat_a_difusa[0] = mat_a_difusa[1] = mat_a_difusa[2] = 1.0;
    break;
  }
  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  if (!glutCreateWindow("Modelos de iluminacao 2"))
  {
    fprintf(stderr, "Error opening a window.\n");
    exit(-1);
  }

  init();

  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  glutCreateMenu(menu);
  glutAddMenuEntry("-sem spec", 0);
  glutAddMenuEntry("-spec media", 1);
  glutAddMenuEntry("-spec alta", 2);
  glutAddMenuEntry("-sem difusa", 3);
  glutAddMenuEntry("-difusa media", 4);
  glutAddMenuEntry("-difusa alta", 5);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
  return (0);
}
