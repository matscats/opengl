/**
 * Utilizando o programa exemplos/linha.c como referência, implemente um programa quadrado.c. Este programa
 * deverá desenhar numa janela com fundo branco, de dimensões 256x256 pixels, um quadrado vermelho, com vértice
 * superior esquerdo de coordenadas (x, y)= (30, 226) e vértice inferior direito de coordenadas (x, y) = (226, 30). Quando a
 * tecla a for pressionada, o quadrado deverá ficar com a cor azul. Quando a tecla v for pressionada, o quadrado
 * deverá voltar à cor vermelha.
 */

#include <GL/glut.h>
#include <stdlib.h>

int cor = 0;

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0); // Fundo branco
  glOrtho(0, 256, 0, 256, -1, 1);   // Sistema de coordenadas
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  if (cor == 0)
    glColor3f(1.0, 0.0, 0.0); // Vermelho
  else
    glColor3f(0.0, 0.0, 1.0); // Azul

  glBegin(GL_POLYGON);
  {
    glVertex2i(30, 226);  // Superior esquerdo
    glVertex2i(226, 226); // Superior direito
    glVertex2i(226, 30);  // Inferior direito
    glVertex2i(30, 30);   // Inferior esquerdo
  }
  glEnd();

  glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'a':
    cor = 1;
    glutPostRedisplay();
    break;
  case 'v':
    cor = 0;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(256, 256);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Quadrado Colorido");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
