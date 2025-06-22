#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

// Padrão de preenchimento corrigido para "MC" (invertido verticalmente)
GLubyte mc_pattern[] = {
    // Linha 8 (agora primeira): M M   C
    0xC3, 0x3C, 0x00, 0x00,
    // Linha 7: M M  CC
    0xC3, 0x78, 0x00, 0x00,
    // Linha 6: M M  C
    0xC3, 0x60, 0x00, 0x00,
    // Linha 5: M M  C
    0xC3, 0x60, 0x00, 0x00,
    // Linha 4: M M  C
    0xC3, 0x60, 0x00, 0x00,
    // Linha 3: M M  C
    0xDB, 0x60, 0x00, 0x00,
    // Linha 2: MM   CC
    0xE7, 0x78, 0x00, 0x00,
    // Linha 1 (agora última): M    C
    0xC3, 0x3C, 0x00, 0x00,

    // Repetir o padrão invertido para completar 32x32
    0xC3, 0x3C, 0x00, 0x00,
    0xC3, 0x78, 0x00, 0x00,
    0xC3, 0x60, 0x00, 0x00,
    0xC3, 0x60, 0x00, 0x00,
    0xC3, 0x60, 0x00, 0x00,
    0xDB, 0x60, 0x00, 0x00,
    0xE7, 0x78, 0x00, 0x00,
    0xC3, 0x3C, 0x00, 0x00,

    0xC3, 0x3C, 0x00, 0x00,
    0xC3, 0x78, 0x00, 0x00,
    0xC3, 0x60, 0x00, 0x00,
    0xC3, 0x60, 0x00, 0x00,
    0xC3, 0x60, 0x00, 0x00,
    0xDB, 0x60, 0x00, 0x00,
    0xE7, 0x78, 0x00, 0x00,
    0xC3, 0x3C, 0x00, 0x00,

    0xC3, 0x3C, 0x00, 0x00,
    0xC3, 0x78, 0x00, 0x00,
    0xC3, 0x60, 0x00, 0x00,
    0xC3, 0x60, 0x00, 0x00,
    0xC3, 0x60, 0x00, 0x00,
    0xDB, 0x60, 0x00, 0x00,
    0xE7, 0x78, 0x00, 0x00,
    0xC3, 0x3C, 0x00, 0x00};

void init(void);
void display(void);
void keyboard(unsigned char key, int x, int y);
void drawHexagon(float centerX, float centerY, float radius);

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(300, 300);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Hexágono com Padrão MC");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0); // Fundo branco
  glOrtho(0, 300, 0, 300, -1, 1);   // Sistema de coordenadas 300x300
}

void drawHexagon(float centerX, float centerY, float radius)
{
  glBegin(GL_POLYGON);
  for (int i = 0; i < 6; i++)
  {
    float angle = i * M_PI / 3.0; // 60 graus em radianos
    float x = centerX + radius * cos(angle);
    float y = centerY + radius * sin(angle);
    glVertex2f(x, y);
  }
  glEnd();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Desenhar hexágono central com padrão de preenchimento "MC" corrigido
  glEnable(GL_POLYGON_STIPPLE);
  glColor3f(0.0, 0.0, 1.0); // Cor azul
  glPolygonStipple(mc_pattern);
  drawHexagon(150, 150, 100); // Hexágono centrado com bounding box 200x200

  glFlush();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 27: // ESC para sair
    exit(0);
    break;
  }
}
