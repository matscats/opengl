/**
 * Utilizando o programa exemplos/splines2d.c como referência, implemente um programa splinesnurbs.c. Quando a tecla u for pressionada, a NURBS deverá ser desenhada utilizando um vetor de nós uniforme. Quando a tecla o for pressionada, a NURBS deverá ser desenhada utilizando um vetor de nós uniforme aberto. Quando a tecla n for pressionada, a NURBS deverá ser desenhada utilizando um vetor de nós não uniforme.
 */

#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

GLint nVertices = 6;
GLfloat vertices[6][3] = {
    {-4.0, 0.0, 0.0}, {-4.0, +4.0, 0.0}, {+4.0, -4.0, 0.0}, {-4.0, -4.0, 0.0}, {+4.0, +4.0, 0.0}, {+4.0, 0.0, 0.0}};

GLint largura, altura;
GLint mudaCurva = 0;
GLint verticeCorrente = 0;

GLfloat esquerda = -5;
GLfloat direita = +5;
GLfloat fundo = -5;
GLfloat topo = +5;
GLfloat longe = +5;
GLfloat perto = -5;

enum
{
  NURBS_UNIFORME,
  NURBS_UNIFORME_ABERTO,
  NURBS_NAO_UNIFORME
};
GLint tipoNurbs = NURBS_UNIFORME;

GLUnurbsObj *nc;

GLfloat nos_uniforme[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
GLfloat nos_uniforme_aberto[10] = {0, 0, 0, 0, 1, 2, 3, 4, 4, 4};
GLfloat nos_nao_uniforme[10] = {0, 0, 0, 1, 2, 4, 4.5, 5, 6, 6};

GLint nNos = 10;

GLint matrizViewport[4];
GLdouble matrizModelview[16], matrizProjecao[16];
GLint yreal;
GLdouble wx, wy, wz;

void display(void)
{
  int i;
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0.0, 1.0, 1.0);
  gluBeginCurve(nc);
  switch (tipoNurbs)
  {
  case NURBS_UNIFORME:
    gluNurbsCurve(nc, nNos, nos_uniforme, 3, &vertices[0][0], 4, GL_MAP1_VERTEX_3);
    break;
  case NURBS_UNIFORME_ABERTO:
    gluNurbsCurve(nc, nNos, nos_uniforme_aberto, 3, &vertices[0][0], 4, GL_MAP1_VERTEX_3);
    break;
  case NURBS_NAO_UNIFORME:
    gluNurbsCurve(nc, nNos, nos_nao_uniforme, 3, &vertices[0][0], 4, GL_MAP1_VERTEX_3);
    break;
  }
  gluEndCurve(nc);

  // Poligonal dos pontos de controle
  glPointSize(5.0);
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_LINE_STRIP);
  for (i = 0; i < nVertices; i++)
    glVertex3fv(&vertices[i][0]);
  glEnd();

  // Pontos de controle
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_POINTS);
  for (i = 0; i < nVertices; i++)
    glVertex3fv(&vertices[i][0]);
  glEnd();

  glColor3f(1.0, 1.0, 1.0);
  glFlush();
  glutSwapBuffers();
}

void init(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  nc = gluNewNurbsRenderer();
  gluNurbsProperty(nc, GLU_SAMPLING_TOLERANCE, 5.0);
  glEnable(GL_MAP1_VERTEX_3);
  display();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  largura = w;
  altura = h;
  glLoadIdentity();
  glOrtho(esquerda, direita, fundo, topo, perto, longe);
  glMatrixMode(GL_MODELVIEW);
  glGetIntegerv(GL_VIEWPORT, matrizViewport);
  glGetDoublev(GL_MODELVIEW_MATRIX, matrizModelview);
  glGetDoublev(GL_PROJECTION_MATRIX, matrizProjecao);
  glLoadIdentity();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'u':
    tipoNurbs = NURBS_UNIFORME;
    glutPostRedisplay();
    break;
  case 'o':
    tipoNurbs = NURBS_UNIFORME_ABERTO;
    glutPostRedisplay();
    break;
  case 'n':
    tipoNurbs = NURBS_NAO_UNIFORME;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  }
}

void proximidade()
{
  int i;
  double tam = 0, tamin = 32000;
  verticeCorrente = 0;
  for (i = 0; i < nVertices; i++)
  {
    tam = (wx - vertices[i][0]) * (wx - vertices[i][0]) +
          (wy - vertices[i][1]) * (wy - vertices[i][1]);
    if (tam < tamin)
    {
      tamin = tam;
      verticeCorrente = i;
    }
  }
  tamin = sqrt(tamin);
  if (tamin > 0.5)
  {
    mudaCurva = 0;
  }
}

void mouse(int button, int state, int x, int y)
{
  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
    {
      yreal = matrizViewport[3] - (GLint)y - 1;
      gluUnProject((GLdouble)x, (GLdouble)yreal, 0.0, matrizModelview,
                   matrizProjecao, matrizViewport, &wx, &wy, &wz);
      mudaCurva = 1;
      proximidade();
    }
    if (state == GLUT_UP)
    {
      mudaCurva = 0;
    }
    break;
  }
}

void motion(int x, int y)
{
  if (mudaCurva)
  {
    yreal = matrizViewport[3] - (GLint)y - 1;
    gluUnProject((GLdouble)x, (GLdouble)yreal, 0.0, matrizModelview,
                 matrizProjecao, matrizViewport, &wx, &wy, &wz);
    vertices[verticeCorrente][0] = wx;
    vertices[verticeCorrente][1] = wy;
    glutPostRedisplay();
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(motion);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
