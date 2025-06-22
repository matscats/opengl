/**
 * Repita o item anterior utilizando buffer simples e comente os resultados obtidos.
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>

#define N_POLIGONOS 4

// Buffer simples para armazenar as cores de preenchimento dos polígonos
float cores[N_POLIGONOS][3];

// Vértices dos polígonos (iguais ao exemplo)
int vx[N_POLIGONOS][4] = {
    {30, 113, 113, 30},   // Polígono 1
    {143, 226, 226, 143}, // Polígono 2
    {30, 113, 113, 30},   // Polígono 3
    {143, 226, 226, 143}  // Polígono 4
};

int vy[N_POLIGONOS][4] = {
    {226, 226, 143, 143}, // Polígono 1
    {226, 226, 143, 143}, // Polígono 2
    {113, 113, 30, 30},   // Polígono 3
    {113, 113, 30, 30}    // Polígono 4
};

int modo = 0; // 0: fundo, 1: borda (não implementado neste buffer simples)

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glOrtho(0, 256, 0, 256, -1, 1);
  srand(time(NULL));
  // Inicializa todas as cores para amarelo
  for (int i = 0; i < N_POLIGONOS; i++)
  {
    cores[i][0] = 1.0f; // R
    cores[i][1] = 1.0f; // G
    cores[i][2] = 0.0f; // B
  }
}

void draw_polygon(int i)
{
  // Preenchimento
  glColor3fv(cores[i]);
  glBegin(GL_POLYGON);
  {
    for (int j = 0; j < 4; j++)
    {
      glVertex2i(vx[i][j], vy[i][j]);
    }
  }
  glEnd();
  // Borda preta fixa
  glColor3f(0.0f, 0.0f, 0.0f);
  glLineWidth(2.0f);
  glBegin(GL_LINE_LOOP);
  {
    for (int j = 0; j < 4; j++)
    {
      glVertex2i(vx[i][j], vy[i][j]);
    }
  }
  glEnd();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  for (int i = 0; i < N_POLIGONOS; i++)
  {
    draw_polygon(i);
  }
  glFlush();
  glutSwapBuffers();
}

// Teste de ponto dentro do polígono (convexo)
int dentro(int x, int y, int i)
{
  int c = 0, j = 3;
  for (int k = 0; k < 4; k++)
  {
    if (((vy[i][k] > y) != (vy[i][j] > y)) &&
        (x < (vx[i][j] - vx[i][k]) * (y - vy[i][k]) / (vy[i][j] - vy[i][k]) + vx[i][k]))
      c = !c;
    j = k;
  }
  return c;
}

void random_color(float *c)
{
  c[0] = (float)rand() / (RAND_MAX + 1.0f);
  c[1] = (float)rand() / (RAND_MAX + 1.0f);
  c[2] = (float)rand() / (RAND_MAX + 1.0f);
}

void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    y = 256 - y; // Ajusta coordenada y
    for (int i = 0; i < N_POLIGONOS; i++)
    {
      if (dentro(x, y, i))
      {
        // Muda a cor do fundo (buffer simples) para cor aleatória
        random_color(cores[i]);
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
    // Modo borda não implementado no buffer simples
    break;
  case 'f':
  case 'F':
    // Modo fundo (padrão)
    break;
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(256, 256);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Selecao com Buffer Simples");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
