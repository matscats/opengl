/**
 * Utilizando as técnicas de seleção de objetos apresentadas no Capítulo [superficies-espaco], implemente um programa splineinter.c que proporcione um processo de modelagem interativa para este exemplo. Quando pressionar o botão esquerdo do mouse, o usuário deverá poder movimentar os pontos da malha de controle, podendo assim controlar livremente a forma da superfície.

Um método de seleção envolve lançar um raio através da localização do mouse e testar a proximidade com os objetos da cena. O raio pode ser gerado chamando duas vezes a função gluUnProject(), uma vez para o plano mais próximo do usuário (winz=0) e outra para o plano mais distante (winz=1). Subtraindo o resultado obtido para o plano mais distante do resultado obtido para o plano mais próximo será possível obter a direção do raio. A origem do raio ficará na posição do observador, no plano mais próximo.

A posição do ponto em uma reta que se encontra mais próxima de um dado ponto R no espaço pode obtida pela seguinte equação:

𝑃𝑜+(𝑅−𝑃𝑜)⋅𝑉𝑉⋅𝑉𝑉

Nesta equação, P_o é um ponto qualquer sobre a reta e pode ser obtido pela equação que a define, 𝑃(𝑡)=𝑃𝑜+𝑉𝑡
, onde V é o vetor que dá a direção da reta.
 */

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

GLint eixox = 0, eixoy = 0, eixoz = 0;
GLint nVertices = 4;
GLfloat vertices[4][4][3];

GLint largura = 500, altura = 500;

enum
{
  BEZIER,
  NURBS
};
GLint spline = NURBS;
GLUnurbsObj *nc;
GLfloat nos[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
GLint nNos = 8;

int pontoSelecionado = 0;
int selecionado_i = -1, selecionado_j = -1;
int movendo = 0;

void gera_superficie(void)
{
  int i, j;
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
    {
      vertices[i][j][0] = 2.0 * ((GLfloat)i - 1.5);
      vertices[i][j][1] = 2.0 * ((GLfloat)j - 1.5);
      if ((i == 1 || i == 2) && (j == 1 || j == 2))
        vertices[i][j][2] = 7.0;
      else
        vertices[i][j][2] = -3.0;
    }
  }
}

void display(void)
{
  int i, j;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();
  glRotatef(eixox, 1, 0, 0);
  glRotatef(eixoy, 0, 1, 0);
  glRotatef(eixoz, 0, 0, 1);
  glScalef(0.25, 0.25, 0.25);

  // Eixos
  glDisable(GL_LIGHTING);
  glPushMatrix();
  glTranslatef(-5, -5, -5);
  glColor3f(1, 1, 1);
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(1, 0, 0);
  glEnd();
  glRasterPos3f(1.5, 0, 0);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'x');
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 1, 0);
  glEnd();
  glRasterPos3f(0, 1.5, 0);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'y');
  glBegin(GL_LINES);
  glVertex3f(0, 0, 0);
  glVertex3f(0, 0, 1);
  glEnd();
  glRasterPos3f(0, 0, 1.5);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'z');
  glPopMatrix();
  glEnable(GL_LIGHTING);

  // Superfície
  switch (spline)
  {
  case BEZIER:
    glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0, 1, 3 * nVertices, 4, &vertices[0][0][0]);
    glEnable(GL_AUTO_NORMAL);
    glMapGrid2f(20, 0, 1.0, 20, 0, 1.0);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    break;
  case NURBS:
    gluBeginSurface(nc);
    gluNurbsSurface(nc, nNos, nos, nNos, nos, 4 * 3, 3, &vertices[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nc);
    break;
  }

  // Pontos de controle
  glPointSize(8.0);
  glDisable(GL_LIGHTING);
  glBegin(GL_POINTS);
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 4; j++)
    {
      if (i == selecionado_i && j == selecionado_j)
        glColor3f(0.0, 1.0, 0.0); // Selecionado: verde
      else
        glColor3f(1.0, 0.0, 0.0); // Normal: vermelho
      glVertex3fv(&vertices[i][j][0]);
    }
  }
  glEnd();
  glEnable(GL_LIGHTING);

  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

// Função para calcular distância ponto-raio
float distancia_ponto_raio(float *Po, float *V, float *R)
{
  float Rx = R[0] - Po[0];
  float Ry = R[1] - Po[1];
  float Rz = R[2] - Po[2];
  float Vx = V[0], Vy = V[1], Vz = V[2];
  float t = (Rx * Vx + Ry * Vy + Rz * Vz) / (Vx * Vx + Vy * Vy + Vz * Vz);
  float Px = Po[0] + Vx * t;
  float Py = Po[1] + Vy * t;
  float Pz = Po[2] + Vz * t;
  return sqrt((Px - R[0]) * (Px - R[0]) + (Py - R[1]) * (Py - R[1]) + (Pz - R[2]) * (Pz - R[2]));
}

// Seleção de ponto de controle via raio do mouse
void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON)
  {
    if (state == GLUT_DOWN)
    {
      // Obtenha as matrizes atuais
      GLint viewport[4];
      GLdouble modelview[16], projection[16];
      glGetIntegerv(GL_VIEWPORT, viewport);
      glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
      glGetDoublev(GL_PROJECTION_MATRIX, projection);

      // Calcule o raio
      GLdouble winX = (double)x;
      GLdouble winY = (double)(viewport[3] - y - 1);
      GLdouble nearX, nearY, nearZ, farX, farY, farZ;
      gluUnProject(winX, winY, 0.0, modelview, projection, viewport, &nearX, &nearY, &nearZ);
      gluUnProject(winX, winY, 1.0, modelview, projection, viewport, &farX, &farY, &farZ);

      float Po[3] = {nearX, nearY, nearZ};
      float V[3] = {farX - nearX, farY - nearY, farZ - nearZ};

      // Encontre o ponto mais próximo do raio
      float minDist = 1e9;
      int minI = -1, minJ = -1;
      int i, j;
      for (i = 0; i < 4; i++)
      {
        for (j = 0; j < 4; j++)
        {
          float d = distancia_ponto_raio(Po, V, vertices[i][j]);
          if (d < minDist)
          {
            minDist = d;
            minI = i;
            minJ = j;
          }
        }
      }
      // Se estiver suficientemente próximo, selecione
      if (minDist < 0.5)
      {
        movendo = 1;
        selecionado_i = minI;
        selecionado_j = minJ;
      }
      else
      {
        movendo = 0;
        selecionado_i = selecionado_j = -1;
      }
    }
    if (state == GLUT_UP)
    {
      movendo = 0;
      selecionado_i = selecionado_j = -1;
    }
  }
}

// Movimentação do ponto selecionado
void motion(int x, int y)
{
  if (movendo && selecionado_i >= 0 && selecionado_j >= 0)
  {
    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    GLdouble winX = (double)x;
    GLdouble winY = (double)(viewport[3] - y - 1);

    // Obtenha a profundidade do ponto selecionado
    GLfloat winZ;
    glReadPixels(x, viewport[3] - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    GLdouble objX, objY, objZ;
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &objX, &objY, &objZ);

    // Atualize o ponto de controle
    vertices[selecionado_i][selecionado_j][0] = objX;
    vertices[selecionado_i][selecionado_j][1] = objY;
    vertices[selecionado_i][selecionado_j][2] = objZ;

    glutPostRedisplay();
  }
}

void init(void)
{
  GLfloat mat_diffuse[] = {0.7, 0.7, 0.7, 1.0};
  GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
  GLfloat mat_shininess[] = {100.0};

  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);

  gera_superficie();

  nc = gluNewNurbsRenderer();
  gluNurbsProperty(nc, GLU_SAMPLING_TOLERANCE, 5.0);
  gluNurbsProperty(nc, GLU_DISPLAY_MODE, GLU_FILL);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);

  spline = NURBS;
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_MAP2_VERTEX_3);
  display();
}

void reshape(int w, int h)
{
  largura = w;
  altura = h;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 3.0, 8.0);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'x':
    eixox = (eixox + 5) % 360;
    glutPostRedisplay();
    break;
  case 'y':
    eixoy = (eixoy + 5) % 360;
    glutPostRedisplay();
    break;
  case 'z':
    eixoz = (eixoz + 5) % 360;
    glutPostRedisplay();
    break;
  case 'X':
    eixox = (eixox - 5) % 360;
    glutPostRedisplay();
    break;
  case 'Y':
    eixoy = (eixoy - 5) % 360;
    glutPostRedisplay();
    break;
  case 'Z':
    eixoz = (eixoz - 5) % 360;
    glutPostRedisplay();
    break;
  case 'b':
    spline = BEZIER;
    glutPostRedisplay();
    break;
  case 'n':
    spline = NURBS;
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
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(largura, altura);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutMainLoop();
  return 0;
}
