/**
 * Utilizando o programa exemplos/braco.c como referência, implemente um programa braco-garra.c. Este programa deverá acrescente ao braço robótico uma garra com três dedos, sendo um indicador, um médio e um polegar, como mostra a Figura Braço robótico com garra. O usuário deverá poder rotacionar o dedo indicador com as teclas i e I, nos sentidos horário e anti-horário. Da mesma forma, as teclas p P deverão rotacionar o polegar, e teclas m M, o dedo médio do robô.
 */

#include <GL/glut.h>
#include <stdlib.h>

static int shoulder = 0, elbow = 0;
static int finger_index = 0, finger_middle = 0, finger_thumb = 0;

void init(void) { glClearColor(0.0, 0.0, 0.0, 0.0); }

void draw_finger(float angle, float x, float y, float z, float rot_axis_x, float rot_axis_y, float rot_axis_z)
{
  glPushMatrix();
  glTranslatef(x, y, z);
  glRotatef(angle, rot_axis_x, rot_axis_y, rot_axis_z);
  glTranslatef(0.3, 0.0, 0.0);
  glScalef(0.6, 0.12, 0.18);
  glutWireCube(1.0);
  glPopMatrix();
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  glPushMatrix();

  // Ombro
  glTranslatef(-1.0, 0.0, 0.0);
  glRotatef((GLfloat)shoulder, 0.0, 0.0, 1.0);

  // Braço superior
  glTranslatef(1.0, 0.0, 0.0);
  glPushMatrix();
  glScalef(2.0, 0.4, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  // Cotovelo
  glTranslatef(1.0, 0.0, 0.0);
  glRotatef((GLfloat)elbow, 0.0, 0.0, 1.0);

  // Braço inferior
  glTranslatef(1.0, 0.0, 0.0);
  glPushMatrix();
  glScalef(2.0, 0.4, 1.0);
  glutWireCube(1.0);
  glPopMatrix();

  // GARRA - três dedos
  // Indicador (em cima)
  draw_finger((GLfloat)finger_index, 0.7, 0.15, 0.0, 0.0, 0.0, 1.0);
  // Médio (meio)
  draw_finger((GLfloat)finger_middle, 0.7, 0.0, 0.0, 0.0, 0.0, 1.0);
  // Polegar (em baixo, levemente inclinado)
  draw_finger((GLfloat)finger_thumb, 0.7, -0.15, 0.0, 0.0, 0.0, 1.0);

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
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
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
