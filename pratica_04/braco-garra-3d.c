/**
 * Copie o programa exemplos/braco-garra.c que você criou com o nome braco-garra-3d.c. Neste novo programa, modifique o trecho da função main() que define o modo de apresentação no GLUT. Use a seguinte chamada para a função de inicialização: glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH), pois inclui o tratamento de profundidade e de superfícies escondidas no OpenGL . Além disso, ao invés de utilizar chamadas à função glutWireCube(), inclua chamadas apenas à função glutSolidCube(), que contém os mesmos argumentos. Acrescente à função init() as chamadas de função glEnable(GL_DEPTH_TEST); e glEnable(GL_CULL_FACE);, de modo que o tratamento de superfícies escondidas seja feito pelo OpenGL. Utilize cores diferentes para cada uma das partes do braço, de modo a obter um modelo semelhante ao da Braço robótico 3D. Possibilite também que a base do robô seja rotacionada em torno do eixo y, usando as teclas b e B, para girar nos sentidos horário e anti-horário.
 */

#include <GL/glut.h>
#include <stdlib.h>

static int base = 0, shoulder = 0, elbow = 0;
static int finger_index = 0, finger_middle = 0, finger_thumb = 0;

void init(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

void draw_finger(float angle, float x, float y, float z, float rot_axis_x, float rot_axis_y, float rot_axis_z, float r, float g, float b)
{
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(angle, rot_axis_x, rot_axis_y, rot_axis_z);
  glTranslatef(0.3, 0.0, 0.0);
  glColor3f(r, g, b);
  glScalef(0.6, 0.12, 0.18);
  glutSolidCube(1.0);
  glPopMatrix();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();

  // Base
  glRotatef((GLfloat)base, 0.0, 1.0, 0.0);
  glPushMatrix();
  glColor3f(0.4, 0.4, 0.4); // cinza escuro
  glScalef(0.6, 0.2, 0.6);
  glutSolidCube(2.0);
  glPopMatrix();

  // Ombro
  glTranslatef(-1.0, 0.0, 0.0);
  glRotatef((GLfloat)shoulder, 0.0, 0.0, 1.0);

  // Braço superior
  glTranslatef(1.0, 0.0, 0.0);
  glPushMatrix();
  glColor3f(0.8, 0.0, 0.0); // vermelho
  glScalef(2.0, 0.4, 1.0);
  glutSolidCube(1.0);
  glPopMatrix();

  // Cotovelo
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);

  // Braço inferior
  glTranslatef(1.0, 0.0, 0.0);
  glPushMatrix();
  glColor3f(0.0, 0.0, 0.8); // azul
  glScalef(2.0, 0.4, 1.0);
  glutSolidCube(1.0);
  glPopMatrix();

  // GARRA - três dedos
  // Indicador (em cima)
  draw_finger((GLfloat)finger_index, 0.7, 0.15, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0); // amarelo
  // Médio (meio)
  draw_finger((GLfloat)finger_middle, 0.7, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0); // verde
  // Polegar (em baixo)
  draw_finger((GLfloat)finger_thumb, 0.7, -0.15, 0.0, 0.0, 0.0, 1.0, 1.0, 0.5, 0.0); // laranja

  glPopMatrix();
  glutSwapBuffers();
}

void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
  case 'b':
    base = (base + 5) % 360;
    glutPostRedisplay();
    break;
  case 'B':
    base = (base - 5) % 360;
    glutPostRedisplay();
    break;
  case 's':
    shoulder = (shoulder + 5) % 360;
    glutPostRedisplay();
    break;
  case 'S':
    shoulder = (shoulder - 5) % 360;
    glutPostRedisplay();
    break;
  case 'e':
    elbow = (elbow + 5) % 360;
    glutPostRedisplay();
    break;
  case 'E':
    elbow = (elbow - 5) % 360;
    glutPostRedisplay();
    break;
  case 'i':
    finger_index = (finger_index + 5) % 360;
    glutPostRedisplay();
    break;
  case 'I':
    finger_index = (finger_index - 5) % 360;
    glutPostRedisplay();
    break;
  case 'm':
    finger_middle = (finger_middle + 5) % 360;
    glutPostRedisplay();
    break;
  case 'M':
    finger_middle = (finger_middle - 5) % 360;
    glutPostRedisplay();
    break;
  case 'p':
    finger_thumb = (finger_thumb + 5) % 360;
    glutPostRedisplay();
    break;
  case 'P':
    finger_thumb = (finger_thumb - 5) % 360;
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
