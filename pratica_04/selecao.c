/**
 * Utilizando o programa exemplos/preenchimento.c como referência, implemente um programa selecao.c. Este
 * programa deverá desenhar numa janela com fundo branco, de dimensões 256x256 pixels, quatro polígonos com as
 * mesmas coordenadas dos polígonos do programa de referência. Todos os polígonos devem estar inicialmente
 * preenchidos de amarelo e com bordas pretas. Quando o usuário clicar com o botão esquerdo do mouse dentro de um
 * dos polígonos, a cor de preechimento deste polígono deverá mudar para uma cor aleatória. Quando a tecla b for
 * pressionada, o programa passará a mudar as cores das bordas e não mais dos fundos dos polígonos. Quando a tecla f
 * for pressionada, o programa passará a mudar as cores dos fundos dos polígonos e não mais das bordas.
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

#define N_POLIGONOS 4

typedef struct
{
  int n;            // número de vértices
  int vx[4], vy[4]; // coordenadas dos vértices
  float fill[3];    // cor de preenchimento
  float border[3];  // cor da borda
} Poligono;

Poligono polys[N_POLIGONOS];
int modo = 0; // 0: fundo, 1: borda

void init_polys()
{
  // Polígono 1 (superior esquerdo)
  polys[0].n = 4;
  polys[0].vx[0] = 30;
  polys[0].vy[0] = 226;
  polys[0].vx[1] = 113;
  polys[0].vy[1] = 226;
  polys[0].vx[2] = 113;
  polys[0].vy[2] = 143;
  polys[0].vx[3] = 30;
  polys[0].vy[3] = 143;

  // Polígono 2 (superior direito)
  polys[1].n = 4;
  polys[1].vx[0] = 143;
  polys[1].vy[0] = 226;
  polys[1].vx[1] = 226;
  polys[1].vy[1] = 226;
  polys[1].vx[2] = 226;
  polys[1].vy[2] = 143;
  polys[1].vx[3] = 143;
  polys[1].vy[3] = 143;

  // Polígono 3 (inferior esquerdo)
  polys[2].n = 4;
  polys[2].vx[0] = 30;
  polys[2].vy[0] = 113;
  polys[2].vx[1] = 113;
  polys[2].vy[1] = 113;
  polys[2].vx[2] = 113;
  polys[2].vy[2] = 30;
  polys[2].vx[3] = 30;
  polys[2].vy[3] = 30;

  // Polígono 4 (inferior direito)
  polys[3].n = 4;
  polys[3].vx[0] = 143;
  polys[3].vy[0] = 113;
  polys[3].vx[1] = 226;
  polys[3].vy[1] = 113;
  polys[3].vx[2] = 226;
  polys[3].vy[2] = 30;
  polys[3].vx[3] = 143;
  polys[3].vy[3] = 30;

  // Inicializa cores (amarelo para fundo, preto para borda)
  for (int i = 0; i < N_POLIGONOS; i++)
  {
    polys[i].fill[0] = 1.0;
    polys[i].fill[1] = 1.0;
    polys[i].fill[2] = 0.0; // amarelo
    polys[i].border[0] = 0.0;
    polys[i].border[1] = 0.0;
    polys[i].border[2] = 0.0; // preto
  }
}

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glOrtho(0, 256, 0, 256, -1, 1);
  srand(time(NULL));
  init_polys();
}

void draw_polygon(Poligono *p)
{
  // Preenchimento
  glColor3fv(p->fill);
  glBegin(GL_POLYGON);
  for (int i = 0; i < p->n; i++)
    glVertex2i(p->vx[i], p->vy[i]);
  glEnd();

  // Borda
  glColor3fv(p->border);
  glLineWidth(2.0);
  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < p->n; i++)
    glVertex2i(p->vx[i], p->vy[i]);
  glEnd();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  for (int i = 0; i < N_POLIGONOS; i++)
    draw_polygon(&polys[i]);
  glFlush();
  glutSwapBuffers();
}

// Teste de ponto dentro de polígono convexo
int dentro(int x, int y, Poligono *p)
{
  int i, j, c = 0;
  for (i = 0, j = p->n - 1; i < p->n; j = i++)
  {
    if (((p->vy[i] > y) != (p->vy[j] > y)) &&
        (x < (p->vx[j] - p->vx[i]) * (y - p->vy[i]) / (p->vy[j] - p->vy[i]) + p->vx[i]))
      c = !c;
  }
  return c;
}

void random_color(float *c)
{
  c[0] = (float)rand() / (RAND_MAX + 1.0);
  c[1] = (float)rand() / (RAND_MAX + 1.0);
  c[2] = (float)rand() / (RAND_MAX + 1.0);
}

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    // Ajusta y pois GLUT tem origem no canto superior esquerdo
    y = 256 - y;
    for (int i = 0; i < N_POLIGONOS; i++)
    {
      if (dentro(x, y, &polys[i]))
      {
        if (modo == 0) // fundo
          random_color(polys[i].fill);
        else // borda
          random_color(polys[i].border);
        glutPostRedisplay();
        break;
      }
    }
  }
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27:
    exit(0);
    break;
  case 'b':
  case 'B':
    modo = 1;
    break;
  case 'f':
  case 'F':
    modo = 0;
    break;
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(256, 256);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Selecao de poligonos");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
