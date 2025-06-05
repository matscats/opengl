/**
 * Implemente o algoritmo de Bresenham para traçado de circunferências, utilizando GL_POINTS como parâmetro da
 * função glBegin(). Utilizando o algoritmo implementado, desenhe uma circunferência azul de raio r=50, centrada no
 * ponto (x, y) = (128, 128).
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void plot_points(int xc, int yc, int x, int y);

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(256, 256);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Bresenham Circle");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0); // Fundo branco
  glOrtho(0, 256, 0, 256, -1, 1);   // Sistema de coordenadas
}

void plot_points(int xc, int yc, int x, int y)
{
  // Plota os 8 pontos simétricos usando GL_POINTS
  glVertex2i(xc + x, yc + y);
  glVertex2i(xc + x, yc - y);
  glVertex2i(xc - x, yc + y);
  glVertex2i(xc - x, yc - y);
  glVertex2i(xc + y, yc + x);
  glVertex2i(xc + y, yc - x);
  glVertex2i(xc - y, yc + x);
  glVertex2i(xc - y, yc - x);
}

void bresenham_circle(int xc, int yc, int r)
{
  int x = 0;
  int y = r;
  int d = 3 - 2 * r; // Fórmula inicial do parâmetro de decisão [2]

  while (x <= y)
  {
    plot_points(xc, yc, x, y);

    if (d < 0)
    {
      d += 4 * x + 6; // Atualização para movimento horizontal [2]
    }
    else
    {
      d += 4 * (x - y) + 10; // Atualização para movimento diagonal [2]
      y--;
    }
    x++;
  }
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0, 0.0, 1.0); // Cor azul

  glBegin(GL_POINTS);
  {
    bresenham_circle(128, 128, 50); // Centro (128,128) e raio 50
  }
  glEnd();

  glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
  if (key == 27) // ESC para sair
    exit(0);
}
