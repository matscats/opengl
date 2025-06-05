/**
 * Implemente o algoritmo de Bresenham para traçado de linhas, utilizando GL_POINTS como parâmetro da função
 * glBegin(). Este parâmetro indica que cada vértice deve ser tratado como um ponto simples. Utilizando o algoritmo
 * implementado, desenhe uma reta verde do ponto (x, y)=(40, 200) ao ponto (x, y)=(200, 10).
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(256, 256);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Bresenham Line");
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

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0, 1.0, 0.0); // Cor verde

  int x1 = 40, y1 = 200;
  int x2 = 200, y2 = 10;

  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int err = dx - dy;

  glBegin(GL_POINTS);
  {
    while (1)
    {
      glVertex2i(x1, y1);
      if (x1 == x2 && y1 == y2)
        break;

      int e2 = 2 * err;
      if (e2 > -dy)
      {
        err -= dy;
        x1 += sx;
      }
      if (e2 < dx)
      {
        err += dx;
        y1 += sy;
      }
    }
  }
  glEnd();
  glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
  if (key == 27) // ESC
    exit(0);
}
