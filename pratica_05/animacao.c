
/**
 * A função glutIdleFunc() é usada pelo GLUT para realizar operações em segundo plano ou animações, enquanto não recebe eventos de sistema. O seu protótipo é:

void glutIdleFunc(void *func(void));
Tomando como base o programa exemplos/projecoes.c, utilizando a função glutIdleFunc() e crie uma função de retorno idle(). Nesta função, os valores dos ângulos eixox e eixoy devem ser incrementados de valores constantes pequenos e diferentes, de modo a possibilitar uma animação. Utilize a função usleep() para introduzir retardos entre as apresentações dos quadros da animação, tornando mais agradável a visualização. Mantenhas as teclas o e p para chavear entre projeções ortogonais e de perspectiva.
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> // Para usleep()

#define AZUL 0.0, 0.0, 1.0
#define VERMELHO 1.0, 0.0, 0.0
#define AMARELO 1.0, 1.0, 0.0
#define VERDE 0.0, 1.0, 0.0
#define CYAN 0.0, 1.0, 1.0
#define LARANJA 0.8, 0.6, 0.1
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

  glDisableClientState(GL_VERTEX_ARRAY);

  glPopMatrix();
  glutSwapBuffers();
}

// Função de animação
void idle(void)
{
  eixox = (eixox + 1) % 360; // Incremento pequeno
  eixoy = (eixoy + 2) % 360; // Incremento diferente
  glutPostRedisplay();
  usleep(16000); // ~16ms ≈ 60 FPS
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:
    exit(0);
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
  glutIdleFunc(idle); // Ativa animação contínua
  glutMainLoop();
  return 0;
}
