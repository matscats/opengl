/**
 * Utilizando o programa exemplos/teste-make.c como referência, implemente um programa quadrados4.c.
 * Este programa deverá desenhar numa janela com fundo branco, de dimensões 256x256 pixels, e quatro quadrados
 * de cores diferentes arranjados na janela.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

void display(void);

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(256, 256);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Quatro Quadrados");
  glClearColor(1.0, 1.0, 1.0, 0.0); // fundo branco
  glShadeModel(GL_FLAT);
  glOrtho(0, 1, 0, 1, -1, 1); // coordenadas normalizadas
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Quadrado 1 - canto inferior esquerdo (vermelho)
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_POLYGON);
  {
    glVertex2f(0.0, 0.0);
    glVertex2f(0.5, 0.0);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.0, 0.5);
  }
  glEnd();

  // Quadrado 2 - canto inferior direito (verde)
  glColor3f(0.0, 1.0, 0.0);
  glBegin(GL_POLYGON);
  {
    glVertex2f(0.5, 0.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(1.0, 0.5);
    glVertex2f(0.5, 0.5);
  }
  glEnd();

  // Quadrado 3 - canto superior direito (azul)
  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_POLYGON);
  {
    glVertex2f(0.5, 0.5);
    glVertex2f(1.0, 0.5);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.5, 1.0);
  }
  glEnd();

  // Quadrado 4 - canto superior esquerdo (amarelo)
  glColor3f(1.0, 1.0, 0.0);
  glBegin(GL_POLYGON);
  {
    glVertex2f(0.0, 0.5);
    glVertex2f(0.5, 0.5);
    glVertex2f(0.5, 1.0);
    glVertex2f(0.0, 1.0);
  }
  glEnd();

  glFlush();
}