#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <Windows.h>
#include <MMSystem.h>
#include <iostream>
#include <glut.h>


#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

using namespace std;

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 1.0f, float eyeY = 1.0f, float eyeZ = 1.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;

	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		//cout << eye.x <<"eyeX "<< eye.y << "eyeY " << eye.z << "eyeZ " << center.x << "centerX " << center.y << "centerY " << center.z<< "centerZ " << up.x<<"upX "<<up.y<<"upY "<<up.z<<"upZ "<<  endl;
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

Camera camera;
bool init = true;
double goalX;
double goalY;
double playerX = -0.1;
double playerZ = 0.65;


bool rotChaircw = false;
bool rotChairAnti = false;
double rotChair = 0;

bool rotTablecw = false;
bool rotTableAnti = false;
double rotTable = 0;

bool trGardenRight = false;
bool trGardenLeft = false;
double trGarden = 0;

bool rotUmbrellacw = false;
bool rotUmbrellaAnti = false;
double rotUmbrella = 0;

bool ScaleUpTree1 = false;
bool ScaleDownTree1 = false;
double scaleTree1 = 1;

bool ScaleUpTree2 = false;
bool ScaleDownTree2 = false;
double scaleTree2 = 1;

bool scaleUpFence = false;
bool scaleDownFence = false;
double scaleFence = 1;

double colorR = 0;
double colorG = 0;
double colorB = 0;

bool gameOver = false;
bool gameLost = false;
int gameTime = 50;

void drawWall(double thickness) {
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslated(0.2, 0.5 * thickness, 0.2);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}
void drawTableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}


void drawTable(double topWid, double topThick, double legThick, double legLen) {

	glColor3f(0.4, 0.4, 0.4);
	glPushMatrix();
	glScaled(1.5, 2.6, 1.5);
	glTranslated(0.085, 0, 0.02);
	glRotated(rotTable, 0, 1, 0);
	glPushMatrix();

	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
	glPopMatrix();
}

void drawTable1(double topWid, double topThick, double legThick, double legLen) {
	glScaled(1, 1, 1);
	glColor3f(0.4, 0.4, 0.4);
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95 * topWid / 2.0 - legThick / 2.0;
	glPushMatrix();
	glTranslated(dist, 0, dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	drawTableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	drawTableLeg(legThick, legLen);
	glPopMatrix();
}
void drawBackFence() {
	glPushMatrix();
	glScaled(1, scaleFence, 1);
	glColor3f(colorR, colorG, colorB);
	for (double i = 0;i < 0.95;i += 0.05) {
		glPushMatrix();
		glTranslated(i - 0.25, 0.1, -0.3);
		glScaled(0.02, 0.2, 0.02);
		glutSolidCube(1.0);
		glPopMatrix();
	}
	glPopMatrix();
}
void drawRightFence() {
	glPushMatrix();
	glScaled(1, scaleFence, 1);
	glColor3f(colorR, colorG, colorB);
	for (double i = 0;i < 1.0;i += 0.05) {
		glPushMatrix();
		glTranslated(-0.3, 0.1, i - 0.3);
		glScaled(0.02, 0.2, 0.02);
		glutSolidCube(1.0);
		glPopMatrix();
	}
	glPopMatrix();
}
void drawLeftFence() {
	glPushMatrix();
	glScaled(1, scaleFence, 1);
	glColor3f(colorR, colorG, colorB);
	for (double i = 0;i < 0.95;i += 0.05) {
		glPushMatrix();
		glTranslated(-0.3 + 0.99, 0.1, i - 0.25);
		glScaled(0.02, 0.2, 0.02);
		glutSolidCube(1.0);
		glPopMatrix();
	}
	glPopMatrix();
}
void drawGarden() {
	glColor3f(0, 0.6, 0);
	glPushMatrix();
	glTranslated(0.2, 0.5 * 0.04 + 0.02, 0.2);
	glTranslated(trGarden, 0, 0);
	glScaled(1.0, 0.02, 1.0);
	glutSolidCube(1);
	glPopMatrix();
	glColor3f(0, 0.7, 0);
	glPushMatrix();
	glTranslated(trGarden, 0, 0);
	for (double i = 0.02;i < 1;i += 0.04) {
		for (double j = 0.02;j < 1;j += 0.04) {
			glPushMatrix();
			glTranslated(j - 0.3, 0.05, i - 0.3);
			glScaled(0.05, 0.05, 0.05);
			glRotated(-90, 1, 0, 0);
			glutSolidCone(0.2, 0.5, 10, 8);
			glPopMatrix();
		}
	}
	glPopMatrix();

}
void drawUmbrella() {
	glColor3f(0.3, 0.6, 0.6);
	glPushMatrix();

	glTranslated(0.55, 0.33, 0.2);
	glRotated(rotUmbrella, 0, 1, 0);
	glScaled(0.2, 0.3, 0.2);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(1, 0.5, 10, 8);
	glPushMatrix();
	glTranslated(0, 0, 0.53);
	glutSolidSphere(0.05, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, -0.95); // cylinder at (0,0,1)  
	glScaled(0.2, 0.2, 4);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.2, 0.2, 0.35, 8, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, -0.9);
	glScaled(0.5, 0.5, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();
	glPopMatrix();
}
void drawChair() {
	glColor3f(0.2, 0.2, 0.2);
	glPushMatrix();
	glScaled(1, 1, 2);
	glTranslated(0, 0, -0.23);
	glPushMatrix();

	glScaled(0.5, 0.5, 0.5);
	glTranslated(1.1, 0.0, 0.97);
	glRotated(rotChair, 0, 1, 0);
	drawTable1(0.2, 0.02, 0.02, 0.25);
	double dist = 0.95 * 0.2 / 2.0 - 0.02 / 2.0;
	glPushMatrix();
	glTranslated(dist, 0 + 0.2, dist);
	drawTableLeg(0.02, 0.25);
	glTranslated(0, 0, -0.17);
	drawTableLeg(0.02, 0.25);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.078, 0.4, 0);
	glScaled(0.01, 0.1, 0.2);

	glutSolidCube(1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void drawTrees() {
	//tree1
	glPushMatrix();
	glScaled(scaleTree1, scaleTree1, scaleTree1);
	glTranslated(0.6, 0, -0.2);
	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslated(0, 0.45, 0); // torus at (0,1,0)  
	glScaled(0.4, 0.4, 0.4);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.3, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslated(0, 0.55, 0); // torus at (0,1,0)  
	glScaled(0.3, 0.3, 0.3);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.3, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslated(0, 0.65, 0); // torus at (0,1,0)  
	glScaled(0.2, 0.2, 0.2);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.3, 10, 10);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.2, 0, 0);
	glTranslated(0, 0.48, 0); // cylinder at (0,0,1)  
	glRotated(90, 1, 0, 0);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj, 0.02, 0.02, 0.5, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.6, 0);
	glTranslated(0, 0.3, 0); // cone at (1,0,1) 
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.2, 0.5, 10, 8);
	glPopMatrix();

	glPopMatrix();
	//tree2



}

void drawTrees2() {
	glPushMatrix();
	glScaled(scaleTree2, scaleTree2, scaleTree2);
	glTranslated(-0.2, 0, -0.2);
	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslated(0, 0.45, 0); // torus at (0,1,0)  
	glScaled(0.4, 0.4, 0.4);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.3, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslated(0, 0.55, 0); // torus at (0,1,0)  
	glScaled(0.3, 0.3, 0.3);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.3, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslated(0, 0.65, 0); // torus at (0,1,0)  
	glScaled(0.2, 0.2, 0.2);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.3, 10, 10);
	glPopMatrix();


	glPushMatrix();
	glColor3f(0.2, 0, 0);
	glTranslated(0, 0.48, 0); // cylinder at (0,0,1)  
	glRotated(90, 1, 0, 0);
	GLUquadricObj* qobj1;
	qobj1 = gluNewQuadric();
	gluQuadricDrawStyle(qobj1, GLU_FILL);  //GLU_FILL, GLU_SILHOUETTE ,GLU_POINT
	gluCylinder(qobj1, 0.02, 0.02, 0.5, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0.6, 0);
	glTranslated(0, 0.3, 0); // cone at (1,0,1) 
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.2, 0.5, 10, 8);
	glPopMatrix();
	glTranslated(-0.3, 0, 0);
	glPopMatrix();
}

void drawPlayer() {
	glPushMatrix();
	glTranslated(playerX, 0, playerZ);
	glColor3f(0.1, 0.1, 0.1);
	glPushMatrix();
	glColor3f(0.8, 0, 0);
	glTranslated(0, 0.3, 0);
	glScaled(1, 1.4, 0.7);
	glutSolidCube(0.12);
	glPopMatrix();

	glColor3f(0.1, 0.1, 0.1);
	glPushMatrix();
	glColor3f(0.6, 0, 0);
	glTranslated(0.035, 0.2, 0);
	glScaled(0.3, 3, 0.3);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3f(0.1, 0.1, 0.1);
	glPushMatrix();
	glColor3f(0.6, 0, 0);
	glTranslated(-0.035, 0.2, 0);
	glScaled(0.3, 3, 0.3);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3f(0.1, 0.1, 0.1);
	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glTranslated(-0.035, 0.35, 0);
	glRotated(135, 0, 0, 1);
	glScaled(0.3, 2, 0.3);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.5, 0, 0);
	glTranslated(0.035, 0.35, 0);
	glRotated(45, 0, 0, 1);
	glScaled(0.3, 2, 0.3);
	glutSolidCube(0.1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8, 0.8, 0.6);
	glTranslated(0, 0.43, 0);
	glutSolidSphere(0.047, 10, 10);
	glPopMatrix();
	glPopMatrix();
}
void drawGoal() {
	glColor3d(1, 0, 0);
	glPushMatrix();
	glTranslated(goalX, 0.2, goalY);
	glScaled(0.4, 0.4, 0.4);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.2, 0.5, 10, 8);

	glPopMatrix();
	glPushMatrix();
	glColor3d(1, 1, 1);
	glTranslated(goalX, 0.4, goalY);
	glScaled(0.4, 0.4, 0.4);
	glutSolidSphere(0.047, 10, 10);
	glPopMatrix();
	//(float(srand()) / float(RAND_MAX)) * (0.7 - (-0.2)))  -0.2
}
double GenerateRandom(double min, double max)
{
	static bool first = true;
	if (first)
	{
		srand(time(0));
		first = false;
	}
	if (min > max)
	{
		std::swap(min, max);
	}
	return min + (double)rand() * (max - min) / (double)RAND_MAX;
}

void setupLights() {
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat lightPosition[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

void Anim() {
	if (rotChaircw == true) {
		rotChair++;
	}
	if (rotChairAnti == true) {
		rotChair--;
	}
	if (trGardenRight == true) {
		trGarden += 0.001;
	}
	if (trGardenLeft == true) {
		trGarden -= 0.001;
	}
	if (rotTablecw == true) {
		rotTable++;
	}
	if (rotTableAnti == true) {
		rotTable--;
	}
	if (rotUmbrellacw == true) {
		rotUmbrella++;
	}
	if (rotUmbrellaAnti == true) {
		rotUmbrella--;
	}
	if (ScaleDownTree1 == true) {
		scaleTree1 -= 0.0003;
	}
	if (ScaleUpTree1 == true) {
		scaleTree1 += 0.0003;
	}
	if (ScaleDownTree2 == true) {
		scaleTree2 -= 0.0003;
	}
	if (ScaleUpTree2 == true) {
		scaleTree2 += 0.0003;
	}
	if (scaleDownFence == true) {
		scaleFence -= 0.0003;
	}
	if (scaleUpFence == true) {
		scaleFence += 0.0003;
	}
	glutPostRedisplay();
}
void gameTimer(int value) {
	gameTime--;
	if (gameOver && !gameLost) {
		return;
	}
	else {
		if (gameTime == 0) {
			gameLost = true;
			gameOver = true;
			glutPostRedisplay();
			return;

		}
		else {
			glutTimerFunc(1 * 1000, gameTimer, 0);
		}
	}

}
void Timer(int value) {
	colorR = GenerateRandom(0.2, 0.8);
	colorG = GenerateRandom(0.2, 0.8);
	colorB = GenerateRandom(0.2, 0.8);
	glutTimerFunc(2 * 1000, Timer, 0);
}

void Display() {
	setupCamera();
	setupLights();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (init) {
		goalX = GenerateRandom(0, 0.5);
		goalY = GenerateRandom(0, 0.5);
		init = false;
	}

	if (!gameOver) {

		glPushMatrix();
		glTranslated(0.4, 0.0, 0.5);
		drawTable(0.2, 0.02, 0.02, 0.2);
		glPopMatrix();
		drawWall(0.04);
		drawBackFence();
		drawRightFence();
		drawLeftFence();
		drawGarden();
		drawUmbrella();
		drawChair();
		drawTrees();
		drawTrees2();
		drawPlayer();
		drawGoal();
	}
	else {
		if (gameLost) {
			glPushMatrix();
			glColor3d(1, 0, 0);
			glTranslated(0, 0, 0.53);
			glutSolidSphere(50, 10, 10);
			glPopMatrix();
		}
		else {
			glPushMatrix();
			glColor3d(0, 1, 0);
			glTranslated(0, 0, 0.53);
			glutSolidSphere(50, 10, 10);
			glPopMatrix();
		}

	}



	glFlush();
}

void Keyboard(unsigned char key, int x, int y) {
	float d = 0.01;

	switch (key) {
		//bench
	case'q':
		if (rotChaircw == false) {
			rotChaircw = true;
			rotChairAnti = false;
		}
		else {
			rotChaircw = false;
		}
		break;
	case'w':
		if (rotChairAnti == false) {
			rotChairAnti = true;
			rotChaircw = false;
		}
		else {
			rotChairAnti = false;
		}
		break;

		//pergola
	case'r':
		if (rotTableAnti == false) {
			rotTableAnti = true;
			rotTablecw = false;
		}
		else {
			rotTableAnti = false;
		}
		break;
	case'e':
		if (rotTablecw == false) {
			rotTablecw = true;
			rotTableAnti = false;
		}
		else {
			rotTablecw = false;
		}
		break;

		//umbrella
	case'y':
		if (rotUmbrellaAnti == false) {
			rotUmbrellaAnti = true;
			rotUmbrellacw = false;
		}
		else {
			rotUmbrellaAnti = false;
		}
		break;
	case't':
		if (rotUmbrellacw == false) {
			rotUmbrellacw = true;
			rotUmbrellaAnti = false;
		}
		else {
			rotUmbrellacw = false;
		}
		break;

		//garden
	case'i':
		if (trGardenLeft == false) {
			trGardenLeft = true;
			trGardenRight = false;
		}
		else {
			trGardenLeft = false;
		}
		break;
	case'u':
		if (trGardenRight == false) {
			trGardenRight = true;
			trGardenLeft = false;
		}
		else {
			trGardenRight = false;
		}
		break;

		//tree1
	case'a':
		if (ScaleDownTree1 == false) {
			ScaleDownTree1 = true;
			ScaleUpTree1 = false;
		}
		else {
			ScaleDownTree1 = false;
		}
		break;
	case's':
		if (ScaleUpTree1 == false) {
			ScaleUpTree1 = true;
			ScaleDownTree1 = false;
		}
		else {
			ScaleUpTree1 = false;
		}
		break;

		//tree2
	case'd':
		if (ScaleDownTree2 == false) {
			ScaleDownTree2 = true;
			ScaleUpTree2 = false;
		}
		else {
			ScaleDownTree2 = false;
		}
		break;
	case'f':
		if (ScaleUpTree2 == false) {
			ScaleUpTree2 = true;
			ScaleDownTree2 = false;
		}
		else {
			ScaleUpTree2 = false;
		}
		break;


		//Fence
	case'g':
		if (scaleDownFence == false) {
			scaleDownFence = true;
			scaleUpFence = false;
		}
		else {
			scaleDownFence = false;
		}
		break;
	case'h':
		if (scaleUpFence == false) {
			scaleUpFence = true;
			scaleDownFence = false;
		}
		else {
			scaleUpFence = false;
		}
		break;
		//front
	case'x':
		camera.eye.x = 0.215098;
		camera.eye.y = 0.503863;
		camera.eye.z = 1.50493;
		camera.center.x = 0.199119;
		camera.center.y = 0.175175;
		camera.center.z = 0.560626;
		camera.up.x = -0.00556098;
		camera.up.y = 0.9444439;
		camera.up.z = -0.328641;
		break;

		//top
	case'z':
		camera.eye.x = 0.183288;
		camera.eye.y = 1.67526;
		camera.eye.z = 0.216134;
		camera.center.x = 0.181869;
		camera.center.y = 0.678788;
		camera.center.z = 0.132279;
		camera.up.x = -0.016859;
		camera.up.y = 0.0838673;
		camera.up.z = -0.996334;
		break;

		//side
	case'c':
		camera.eye.x = 1.80953;
		camera.eye.y = 0.262664;
		camera.eye.z = 0.160741;
		camera.center.x = 0.81052;
		camera.center.y = 0.223934;
		camera.center.z = 0.182576;
		camera.up.x = -0.0387419;
		camera.up.y = 0.999249;
		camera.up.z = -0.000134512;
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}


	glutPostRedisplay();
}
void Special(int key, int x, int y) {
	float a = 1.0;

	switch (key) {
	case GLUT_KEY_UP:
		if (playerZ >= -0.25) {
			playerZ -= 0.002;
		}
		else {
			PlaySound(TEXT("powerup.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		break;
	case GLUT_KEY_DOWN:
		if (playerZ <= 0.68) {
			playerZ += 0.002;
		}
		else {
			PlaySound(TEXT("powerup.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		break;
	case GLUT_KEY_LEFT:
		if (playerX >= -0.25) {
			playerX -= 0.002;
		}
		else {
			PlaySound(TEXT("powerup.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (playerX <= 0.64) {
			playerX += 0.002;
		}
		else {
			PlaySound(TEXT("powerup.wav"), NULL, SND_ASYNC | SND_FILENAME);
		}
		break;
	}
	if ((playerX <= goalX + 0.1 && playerX >= goalX - 0.1) && (playerZ <= goalY + 0.1 && playerZ >= goalY - 0.1)) {
		PlaySound(TEXT("solid.wav"), NULL, SND_ASYNC | SND_FILENAME);
		gameOver = true;
		glutPostRedisplay();
	}

	glutPostRedisplay();
}



void main(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(50, 50);

	glutCreateWindow("Assignment2");
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutIdleFunc(Anim);
	glutTimerFunc(0, Timer, 0);
	glutTimerFunc(0, gameTimer, 0);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}