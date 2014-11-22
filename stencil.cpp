#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//posicao e tamanho do quadrado
GLfloat x = 0.0f;
GLfloat y = 0.0f;
GLfloat tamanho_quadrado = 75;

// Step size in x and y directions
// (number of pixels to move each time)
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;

//mantem tamanho da janela
GLfloat windowWidth;
GLfloat windowHeight;

int iterando = 0;
int max_iterando=50;
int max_esconde=100;

//desenha na tela
void desenhar(void)
{
	GLdouble raio = 0.1; 
	GLdouble angulo;        

	// muda cor do fundo limpando a tela
	glClearColor(255, 255, 255, 0.0f);

	// Use 0 for clear stencil, enable stencil test
	glClearStencil(0.0f);
	glEnable(GL_STENCIL_TEST);

	// Clear color and stencil buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// All drawing commands fail the stencil test, and are not
	// drawn, but increment the value in the stencil buffer. 
	// glStencilFunc takes three arguments: the stencil function, the reference value, and the mask value. Whenever the stencil function is tested (for example GL_LESS), both the reference and the stencil value being tested from the buffer is bitwise ANDed with the mask: GL_LESS returns true if (ref & mask) < (stencil & mask).
	if(iterando >= max_iterando && iterando < max_esconde){
		// se eh para mostrar, usa GL_KEEP, ai ira mostrar a espiral vermelha
		glStencilFunc(GL_KEEP, 0x0, 0x0);
		iterando++;
	}
	else if(iterando >= max_esconde){
		//quando passar do max_esconde, reinicia para voltar o ciclo
		//na ultima vez ainda usa o GL_KEEP
		iterando = 0;
		glStencilFunc(GL_KEEP, 0x0, 0x0);
	}else{
		//enquanto esta iterando, usa GL_NEVER para esconder a espiral
		//como o quadrado verde vai passar na frente, ira poder ver
		//a espiral na cor branca, neste caso o quadrado esta na frente da espiral
		//por isso da para ver a espiral. O quadrado serve apenas para demonstrar isso
		iterando++;
		glStencilFunc(GL_NEVER, 0x0, 0x0);
	}
	// glStencilOp takes three arguments: the stencil operation to call when the stencil test fails, the stencil operation to call when the stencil test passes but the depth test fails, and the stenciloperation to call when the stencil test passes AND the depth test passes (or depth test is disabled or no depth buffer is allocated).
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);

	// Spiral pattern will create stencil pattern
	// Draw the spiral pattern with white lines. We 
	// make the lines  white to demonstrate that the 
	// stencil function prevents them from being drawn
	glBegin(GL_LINE_STRIP);
	for(angulo = 0; angulo < 400.0; angulo += 0.1)
	{
		glColor3f(255.0f, 0.0f, 0.0f);
		glVertex2d(raio * cos(angulo), raio * sin(angulo));
		raio *= 1.003;
	}
	glFlush();
	glEnd();

	// Permite desenhar na tela apenas onde nao for igual 0x1
	// Isso faz com q o retangulo nao seja desenhado onde tenha linha de espiral,
	// ou seja, as linhas de fundo irao aparecer
	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//desenha um quadrado
	glColor3f(0,5,0);
	glRectf(x, y, x + tamanho_quadrado, y - tamanho_quadrado);

	glutSwapBuffers();
}

//callback chamada a cada intervalo de tempo
////(quando a janela nao esta sendo redimensionada ou movida)
void tempo(int value)
{
	// Muda a direção quando chega na borda esquerda ou direita
	if(x > windowWidth-tamanho_quadrado || x < -windowWidth)
		xstep = -xstep;

	// Muda a direção quando chega na borda superior ou inferior
	if(y > windowHeight || y < -windowHeight + tamanho_quadrado)
		ystep = -ystep;


	// Check bounds 
	if(x > windowWidth-tamanho_quadrado)
		x = windowWidth-tamanho_quadrado-1;

	if(y > windowHeight)
		y = windowHeight-1; 

	//anda
	x += xstep;
	y += ystep;

	// Redesenha o quadrado com as novas coordenadas 
	glutPostRedisplay();
	glutTimerFunc(33,tempo, 1);
}


//chamada quando o tamanha da janela muda
void redimensiona(int w, int h)
{
	GLfloat aspectRatio;

	// Evita a divisao por zero
	if(h == 0) h = 1;

	// Especifica as dimensões da Viewport
	glViewport(0, 0, w, h);

	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Estabelece a janela de corte (left, right, bottom, top)     
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h) 
	{
		windowWidth = 100;
		windowHeight = 100 / aspectRatio;
		glOrtho (-100.0, 100.0, -windowHeight, windowHeight, 1.0, -1.0);
	}
	else 
	{
		windowWidth = 100 * aspectRatio;
		windowHeight = 100;
		glOrtho (-windowWidth, windowWidth, -100.0, 100.0, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_STENCIL);
	glutInitWindowSize(800,600);
	glutCreateWindow("CGR-2014/2 Stencil Buffer");
	glutReshapeFunc(redimensiona);
	glutDisplayFunc(desenhar);
	glutTimerFunc(33, tempo, 1);
	glutMainLoop();
	return 0;
}
