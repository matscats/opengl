/**
 * Utilizando o programa exemplos/projecoes.c como referência, implemente um programa tiposdeprojecoes.c. Este programa deverá conter mais sete opções de teclado: t e f, para exibir o topo e o fundo do objeto, F e T, para mostrar a frente e a face traseira, e e d, para mostrar a faces esquerda e direita, respectivamente, e a tecla c, para mostrar o triângulo do canto.
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#define AZUL 0.0, 0.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0
#define AMARELO 1.0, 1.0, 0.0
#define VERDE 0.0, 1.0, 0.0
#define CYAN 0.0, 1.0, 1.0
#define LARANJA 0.8, 0.6, 0.1
#define ROSEO 0.7, 0.1, 0.6
#define CINZA 0.6, 0.6, 0.6

static GLfloat vertices[30] = {
    0.0, 30.0, 30.0,  /* 0 */
    20.0, 30.0, 30.0, /* 1 */
    30.0, 20.0, 30.0, /* 2 */
    30.0, 0.0, 30.0,  /* 3 */
    0.0, 0.0, 30.0,   /* 4 */
    0.0, 30.0, 0.0,   /* 5 */
    30.0, 30.0, 0.0,  /* 6 */
    30.0, 0.0, 0.0,   /* 7 */
    0.0, 0.0, 0.0,    /* 8 */
    30.0, 30.0, 20.0  /* 9 */
};

static GLubyte frenteIndices[] = {0, 4, 3, 2, 1};
static GLubyte trasIndices[] = {5, 6, 7, 8};
static GLubyte esquerdaIndices[] = {0, 5, 8, 4};
static GLubyte direitaIndices[] = {2, 3, 7, 6, 9};
static GLubyte topoIndices[] = {0, 1, 9, 6, 5};
static GLubyte fundoIndices[] = {3, 4, 8, 7};
static GLubyte trianguloIndices[] = {1, 2, 9};

static int eixoy = 0, eixox = 0;
int largura = 256, altura = 256;

enum
{
  EXIBE_TODAS,
  EXIBE_TOPO,
  EXIBE_FUNDO,
  EXIBE_FRENTE,
  EXIBE_TRAS,
  EXIBE_ESQ,
  EXIBE_DIR,
  EXIBE_TRIANGULO
};
static int modo_exibicao = EXIBE_TODAS;

enum
{
  PROJ_ORTHO,
  PROJ_PERSPECTIVE
};
static int modo_projecao = PROJ_ORTHO;

void atualizaProjecao()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (modo_projecao == PROJ_ORTHO)
    glOrtho(-50, 50, -50, 50, -50, 50);
  else
    gluPerspective(65.0, (GLfloat)largura / (GLfloat)altura, 20.0, 120.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  if (modo_projecao == PROJ_PERSPECTIVE)
    gluLookAt(0, 0, -90, 0, 0, 0, 0, 1, 0);
}

void init(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  atualizaProjecao();
}

void reshape(int w, int h)
{
  largura = w;
  altura = h;
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  atualizaProjecao();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef((GLfloat)eixoy, 0.0, 1.0, 0.0);
  glRotatef((GLfloat)eixox, 1.0, 0.0, 0.0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices);

  switch (modo_exibicao)
  {
  case EXIBE_TODAS:
    glColor3f(AZUL);
    glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_BYTE, frenteIndices);
    glColor3f(AMARELO);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, esquerdaIndices);
    glColor3f(VERMELHO);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, trasIndices);
    glColor3f(VERDE);
    glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_BYTE, direitaIndices);
    glColor3f(CYAN);
    glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_BYTE, topoIndices);
    glColor3f(LARANJA);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, fundoIndices);
    glColor3f(CINZA);
    glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_BYTE, trianguloIndices);
    break;
  case EXIBE_TOPO:
    glColor3f(CYAN);
    glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_BYTE, topoIndices);
    break;
  case EXIBE_FUNDO:
    glColor3f(LARANJA);
    glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, fundoIndices);
    break;
  case EXIBE_FRENTE:
    glColor3f(AZUL);
    glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_BYTE, frenteIndices);
    break;
  case EXIBE_TRAS:
    glColor3f(VERMELHO);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, trasIndices);
    break;
  case EXIBE_ESQ:
    glColor3f(AMARELO);
    glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, esquerdaIndices);
    break;
  case EXIBE_DIR:
    glColor3f(VERDE);
    glDrawElements(GL_POLYGON, 5, GL_UNSIGNED_BYTE, direitaIndices);
    break;
  case EXIBE_TRIANGULO:
    glColor3f(CINZA);
    glDrawElements(GL_POLYGON, 3, GL_UNSIGNED_BYTE, trianguloIndices);
    break;
  }

  glDisableClientState(GL_VERTEX_ARRAY);

  glPopMatrix();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:
    exit(0);
    break;
  case 'a':
    printf("%d, %d\n", x, y);
    break;
  case 'y':
    eixoy = (eixoy + 5) % 360;
    glutPostRedisplay();
    break;
  case 'Y':
    eixoy = (eixoy - 5) % 360;
    glutPostRedisplay();
    break;
  case 'x':
    eixox = (eixox + 5) % 360;
    glutPostRedisplay();
    break;
  case 'X':
    eixox = (eixox - 5) % 360;
    glutPostRedisplay();
    break;
  case 'p':
    modo_projecao = PROJ_PERSPECTIVE;
    atualizaProjecao();
    glutPostRedisplay();
    break;
  case 'o':
    modo_projecao = PROJ_ORTHO;
    atualizaProjecao();
    glutPostRedisplay();
    break;
  case 't': // Topo
    modo_exibicao = EXIBE_TOPO;
    glutPostRedisplay();
    break;
  case 'f': // Fundo
    modo_exibicao = EXIBE_FUNDO;
    glutPostRedisplay();
    break;
  case 'F': // Frente
    modo_exibicao = EXIBE_FRENTE;
    glutPostRedisplay();
    break;
  case 'T': // Traseira
    modo_exibicao = EXIBE_TRAS;
    glutPostRedisplay();
    break;
  case 'e': // Esquerda
    modo_exibicao = EXIBE_ESQ;
    glutPostRedisplay();
    break;
  case 'd': // Direita
    modo_exibicao = EXIBE_DIR;
    glutPostRedisplay();
    break;
  case 'c': // Triângulo do canto
    modo_exibicao = EXIBE_TRIANGULO;
    glutPostRedisplay();
    break;
  case 'u': // Volta a mostrar todas as faces
    modo_exibicao = EXIBE_TODAS;
    glutPostRedisplay();
    break;
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(256, 256);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
