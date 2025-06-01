/**
 * Utilizando o programa exemplos/teste-make.c como referência, implemente um programa quadradoscolor.c.
 * Este programa deverá desenhar numa janela com fundo branco, de dimensões 256x256 pixels, um quadrado com as
 * mesmas dimensões do quadrado original. Entretanto, cada uma das cores determinadas para os quatro vértices do quadrado
 * deverão ser determinadas a partir de valores calculados via modelo HSI.
 * Implemente um temporizador para os valores de H mudarem com o tempo para os vértices, produzindo uma pequena animação.
 * Comente a linha glShadeModel(GL_FLAT) para que o feito de degradê possa ser visualizado.
 */

#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Parâmetros HSI para os quatro vértices
float H[4] = {0.0, 0.25, 0.5, 0.75}; // valores iniciais de H
float S = 1.0, I = 1.0;              // Saturação e Intensidade fixas

// Conversão de HSI para RGB
void HSItoRGB(float H, float S, float I, float *r, float *g, float *b)
{
  float R, G, B;
  float h = H * 360.0f; // H em graus

  if (h < 0.0f)
    h += 360.0f;
  if (h >= 360.0f)
    h -= 360.0f;

  if (h < 120.0f)
  {
    B = I * (1 - S);
    R = I * (1 + (S * cos(h * M_PI / 180.0f)) / cos((60.0f - h) * M_PI / 180.0f));
    G = 3 * I - (R + B);
  }
  else if (h < 240.0f)
  {
    h = h - 120.0f;
    R = I * (1 - S);
    G = I * (1 + (S * cos(h * M_PI / 180.0f)) / cos((60.0f - h) * M_PI / 180.0f));
    B = 3 * I - (R + G);
  }
  else
  {
    h = h - 240.0f;
    G = I * (1 - S);
    B = I * (1 + (S * cos(h * M_PI / 180.0f)) / cos((60.0f - h) * M_PI / 180.0f));
    R = 3 * I - (G + B);
  }
  // Clamp para 0..1
  *r = fmax(0.0, fmin(1.0, R));
  *g = fmax(0.0, fmin(1.0, G));
  *b = fmax(0.0, fmin(1.0, B));
}

void display(void)
{
  float rgb[4][3];
  int i;
  // Calcula as cores RGB para os quatro vértices
  for (i = 0; i < 4; i++)
  {
    HSItoRGB(H[i], S, I, &rgb[i][0], &rgb[i][1], &rgb[i][2]);
  }

  glClear(GL_COLOR_BUFFER_BIT);

  // Quadrado central (como no exemplo original)
  glBegin(GL_POLYGON);
  glColor3f(rgb[0][0], rgb[0][1], rgb[0][2]);
  glVertex2f(0.25, 0.25);

  glColor3f(rgb[1][0], rgb[1][1], rgb[1][2]);
  glVertex2f(0.75, 0.25);

  glColor3f(rgb[2][0], rgb[2][1], rgb[2][2]);
  glVertex2f(0.75, 0.75);

  glColor3f(rgb[3][0], rgb[3][1], rgb[3][2]);
  glVertex2f(0.25, 0.75);
  glEnd();

  glFlush();
}

void timer(int value)
{
  // Animação: faz H girar para cada vértice
  for (int i = 0; i < 4; i++)
  {
    H[i] += 0.01f * (i + 1); // Cada vértice com velocidade diferente
    if (H[i] > 1.0f)
      H[i] -= 1.0f;
  }
  glutPostRedisplay();
  glutTimerFunc(33, timer, 1); // 30fps
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(256, 256);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Quadrado Colorido com HSI");
  glutTimerFunc(33, timer, 1);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  // glShadeModel(GL_FLAT); // Comentar para ver o degradê
  glShadeModel(GL_SMOOTH); // Degradê ativado
  glOrtho(0, 1, 0, 1, -1, 1);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}
