#include "Angel.h"
#include "mat.h"
#include "vec.h"
#include "SOIL.h"
#include<vector>
#include<string.h>
#include "planet.h"
#include "camera.h"
#pragma comment(lib, "glew32.lib")

// camera

static int oldmy = -1, oldmx = -1; //du是视点绕y轴的角度,opengl里默认y轴是上方向

// 相机参数
float radius = 1000.0;
float rotateAngle = 0.0;
float upAngle = 0.0;
int camera_mode = 2;
Camera cam;


GLint matrixLocation;
//int currentTransform = TRANSFORM_ROTATE;    // 设置当前变换
int mainWindow;


//GLuint load_texture(const char *path)
//{
//	unsigned char* image;
//	int width, height;
//	GLuint name;
//	glGenTextures(1, &name);
//	glBindTexture(GL_TEXTURE_2D, name);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
//	SOIL_free_image_data(image);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	return name;
//}


////////////////////Load texture --Reduce memory usage ver.////////////////////////
typedef struct texture_data {
	unsigned char* data;
	int width, height;
} texture_data;

texture_data load_texture2(const char *path)
{
	texture_data t;
	//unsigned char* image;
	int width, height;
	t.data = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	t.width = width;
	t.height = height;
	//SOIL_free_image_data(image);
	return t;
}

GLUquadricObj* e_tex = gluNewQuadric();//texture
//GLuint sun_tex = load_texture("sun.jpg");
//GLuint earth_tex = load_texture("earth.jpg");
//GLuint moon_tex = load_texture("moon.jpg");
//GLuint sky_tex = load_texture("sky.png");

const int img_num = 5;
GLuint all_texture[img_num];
texture_data TextureImage[img_num];// 创建纹理的存储空间
//memset(TextureImage,0,sizeof(void *) * 4);
void add_textures() // 载入位图(调用上面的代码)并转换成纹理
{

	//int Status = FALSE; // 状态指示器
	
	memset(TextureImage, 0, sizeof(void *) * img_num); // 将指针设为 NULL
	TextureImage[0] = load_texture2("sun.jpg");
	TextureImage[1] = load_texture2("earth.jpg");
	TextureImage[2] = load_texture2("moon.jpg");
	TextureImage[3] = load_texture2("sky.png");
	TextureImage[4] = load_texture2("mars.jpg");

	glGenTextures(img_num, &all_texture[0]); // 创建纹理
	for (int i = 0; i < img_num; i++)
	{
		glBindTexture(GL_TEXTURE_2D, all_texture[i]);// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[i].width, TextureImage[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[i].data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // 线形滤波
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // 线形滤波
	}

	for (int i = 0; i < img_num; i++)
	{
		SOIL_free_image_data(TextureImage[i].data);
	}
	

}

/////////////////////////////////////////////////////////////////////////////




void init()
{
	//glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	add_textures();

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	
}

//static GLint imagewidth;
//static GLint imageheight;
//static GLint pixellength;
//static GLubyte* pixeldata;

POINT clickPos;
void Mouse(int button, int state, int x, int y) //处理鼠标点击
{
	if (button == 3 || button == 4) {
		//currentTransform = TRANSFORM_SCALE;
		if (button == 3)
			radius -= 10;

		if (button == 4)
			radius += 10;

	}

	//std::cout << radius << std::endl;
	
	if (state == GLUT_DOWN) {
		//currentTransform = TRANSFORM_ROTATE;
		GetCursorPos(&clickPos);
		oldmx = x, oldmy = y;
	}
	
}


GLfloat deltax = 0.0f;
GLfloat deltay = 0.0f;
void onMouseMove(int x, int y) //处理鼠标拖动
{

	deltax = x - oldmx; //鼠标在窗口x轴方向上的增量加到视点绕y轴的角度上，这样就左右转了
	
	deltay = y - oldmy; //鼠标在窗口y轴方向上的改变加到视点的y坐标上，就上下转了

	//std::cout << x << std::endl;
	rotateAngle += deltax;
	upAngle += deltay;

	oldmx = x, oldmy = y; //把此时的鼠标坐标作为旧值，为下一次计算增量做准备

}

// 追踪观察点的方向
static GLfloat s_eye[] = { 500,8.0,0 };
static GLfloat s_at[] = { 0.0,0.0,0.0 };
static GLfloat s_angle = -90.0;                    //如果初始角度设置为0则初始面向X轴正方向，设置为-90面向
												   //Z轴负方向，符合默认情况下的设计习惯。
int speed = 5;
float PI = 3.14159f;
float rad = float(PI*s_angle / 180.0f);


void keyboard(unsigned char key, int x, int y)
{

	
	rad = float(PI*s_angle / 180.0f);                    //计算SIN和COS函数中需要的参数。
															   // 前进,后退请求
	switch (key) {
	case 'w':
		cam.moveCamera(10);
		break;
	case 's':
		cam.moveCamera(-10);
		break;
	case 'a':
		cam.yawCamera(-10);
		break;
	case 'd':
		cam.yawCamera(10);
		break;
	case 033:
		// Esc按键
		exit(EXIT_SUCCESS);
		break;
	}

	// 观察点
	s_at[0] = float(s_eye[0] + 100 * cos(rad));
	s_at[2] = float(s_eye[2] + 100 * sin(rad));
	s_at[1] = s_eye[1];
	//观察点可以设置的更远一些，如果设置的更小可能导致不能看到较远的物体，也是通过sin和cos函数实现
	//向前进方向去设置观察点。
	// 设置观察点
	//glLoadIdentity();
	glutPostWindowRedisplay(mainWindow);
}





void material_sun()                               
{
	GLfloat sun_mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //定义材质的环境光颜色，为0
	GLfloat sun_mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //定义材质的漫反射光颜色，为0
	GLfloat sun_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //定义材质的镜面反射光颜色，为0
	GLfloat sun_mat_emission[] = { 1.0f, 1.0f, 1.0f, 0.0f };   //定义材质的辐射广颜色，为偏红色
	GLfloat sun_mat_shininess = 0.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
	//glMaterialfv(GL_FRONT, GL_EMISSION, LightEmission);
}

void sun()
{
	glPushMatrix();
	//glEnable(GL_LIGHTING);
	//glTranslatef(0.0f, 0.0f, -300.0f);
	material_sun();
	/*glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_EMISSION, LightEmission);*/

	gluQuadricTexture(e_tex, GLU_TRUE);
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, all_texture[0]);
	gluSphere(e_tex, 100.0f, 30.0f, 30.0f);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	gluQuadricTexture(e_tex, GLU_FALSE);	
	glPopMatrix();
}

void material_planet()
{
	GLfloat earth_mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //定义材质的环境光颜色，骗蓝色
	GLfloat earth_mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //定义材质的漫反射光颜色，偏蓝色
	GLfloat earth_mat_specular[] = { 0.8f, 0.8f, 0.8f, 0.2f };   //定义材质的镜面反射光颜色，红色
	GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射光颜色，为0
	GLfloat earth_mat_shininess = 5.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);
	//GLfloat sun_light_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sun_light_ambient);
	//GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sun_light_specular);
	//glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 5);
}

void earth()
{

	glPushMatrix();
	material_planet();
	glEnable(GL_LIGHTING);

	static float aMoonRot = 0.0f;
	static float aEarthRot = 0.0f;
	static float sMoonRot = 0.0f;
	static float sEarthRot = 0.0f;

	glRotatef(aEarthRot, 0.0f, 1.0f, 0.0f);
	glTranslatef(150.0f, 0.0f, 0.0f);
	//glPushMatrix();

	glRotatef(90, 1.0f, 0.0f, 0.0f);
	//glPopMatrix();

	glRotatef(sEarthRot, 0.0f, 0.0f, 1.0f);

	gluQuadricTexture(e_tex, GLU_TRUE);
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_EVN_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, all_texture[1]);
	//glColor3ub(0, 0, 255);

	gluSphere(e_tex, 15.0f, 15.0f, 15.0f);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);
	
	glPopAttrib();
	gluQuadricTexture(e_tex, GLU_FALSE);

	glColor3ub(200, 200, 200);
	glRotatef(aMoonRot, 0.0f, 0.0f, 1.0f);
	glTranslatef(30.0f, 0.0f, 0.0f);
	aMoonRot += 3.0f;
	if (aMoonRot >= 360.0f)
		aMoonRot = 0.0f;
	gluQuadricTexture(e_tex, GLU_TRUE);
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_EVN_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, all_texture[2]);
	gluSphere(e_tex, 4.0f, 15.0f, 15.0f);
	glBindTexture(GL_TEXTURE_2D, 0);

	aEarthRot += 1.0f;
	if (aEarthRot >= 360.0f)
		aEarthRot = 0.0f;
	sEarthRot += 5.0f;
	//std::cout << sEarthRot << std::endl;
	if (sEarthRot >= 360.0f)
		sEarthRot = 0.0f;

	glPopMatrix();
}


//////////////////////////////Draw Normal Planets///////////////////////////////////
//typedef struct other_planet {
//	float srot;
//	float arot;
//	GLdouble radius;
//	GLuint texture_name;
//	float distance;
//	//GLdouble period; // Sidereal rotation period, in days.
//
//} other_planet;

// define planets
//other_planet mars = { 2.0f, 2.0f, 10.0f, all_texture[4], 200.0f };

Planet mars =  Planet(2.0f, 2.0f, 10.0f, all_texture[4], 200.0f);
//Planet venus = Planet(2.0f, 2.0f, 10.0f, all_texture[4], 250.0f);

float arot = 0.0f;
float srot = 0.0f;

void draw_planet(Planet &p)
{
	//std::cout << all_texture[1] << std::endl;


	glPushMatrix();
	material_planet();
	glEnable(GL_LIGHTING);

	glRotatef(arot, 0.0f, 1.0f, 0.0f);
	glTranslatef(p.distance, 0.0f, 0.0f);

	//lean
	//glRotatef(90, 1.0f, 0.0f, 0.0f);

	glRotatef(srot, 0.0f, 0.0f, 1.0f);

	gluQuadricTexture(e_tex, GLU_TRUE);
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_EVN_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, all_texture[4]);
	//std::cout << all_texture[4] << std::endl;
	
	//glColor3ub(0, 0, 255);

	gluSphere(e_tex, p.radius, 15.0f, 15.0f);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);

	glPopAttrib();
	gluQuadricTexture(e_tex, GLU_FALSE);


	arot += p.arot;
	if (arot >= 360.0f)
		arot = int(arot)%360;
	srot += p.srot;
	//std::cout << p.srot << std::endl;
	if (srot >= 360.0f)
		srot = int(arot) % 360;

	glPopMatrix();
}

//////////////////////////////////////////////////////////////////////////////



void draw_milky_way(float x, float y, float z, float width, float height, float len) 
{
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
	glDepthMask(GL_FALSE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, all_texture[3]);

	//back face
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, 1.0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y, z);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y + height, z);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, z);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z);
	glEnd();
	//front face
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z + len);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z + len);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y + height, z + len);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y, z + len);
	glEnd();
	//bottom face
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y, z);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y, z + len);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y, z + len);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y, z);
	glEnd();
	//top face
	glBegin(GL_QUADS);
	glNormal3f(0.0, -1.0, 0.0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y + height, z);


	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y + height, z + len);


	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, z + len);


	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y + height, z);
	glEnd();
	//left face
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0.0, 0.0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x, y + height, z);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x, y + height, z + len);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y, z + len);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z);
	glEnd();

	//right face
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0, -1.0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y, z);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y, z + len);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x + width, y + height, z + len);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x + width, y + height, z);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDepthMask(GL_TRUE);
	glPopAttrib();

}



void motion(int x, int y)
{
	//std::cout << x << std::endl;

	cam.setViewByMouse(clickPos.x, clickPos.y);

	glutPostRedisplay();
}

void RenderScene(void)
{
	float ex = radius * cos(upAngle * M_PI / 180.0) * sin(rotateAngle * M_PI / 180.0);
	float ey = radius * sin(upAngle * M_PI / 180.0);
	float ez = radius * cos(upAngle * M_PI / 180.0) * cos(rotateAngle * M_PI / 180.0);

	//s_at[0] = float(s_eye[0] + 100 * cos(rad));
	//s_at[2] = float(s_eye[2] + 100 * sin(rad));
	//s_at[1] = s_eye[1];
	glLoadIdentity();
	//if (camera_mode == 1) 
	//{
	//	gluLookAt(ex, ey, ez, 0, 0, 0, 0, 1, 0);
	//}
	//else
	//{

	//	gluLookAt(s_eye[0], s_eye[1], s_eye[2],
	//		s_at[0], s_at[1], s_at[2],
	//		0.0, 1.0, 0.0);
	//}
	//
	cam.view2();
	//cam.view1(radius, rotateAngle, upAngle);
	//gluLookAt(ex, ey, ez, 0, 0, 0, 0, 1, 0);
	static float fMoonRot = 0.0f;
	static float fEarthRot = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);

	//sun
	draw_milky_way(-500+ex, -500 + ey, -500 + ez, 1000.0f, 1000.0f, 1000.0f);
	sun();
	earth();
	draw_planet(mars);
	//draw_planet(venus);
	//earth 
	
	//glPopMatrix();


	glutPostWindowRedisplay(mainWindow);

	glutSwapBuffers();
}


void ChangeSize(GLsizei w, GLsizei h)
{
	GLfloat fAspect;
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, fAspect, 1.0, 4000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void printHelp() {

}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    // 窗口支持双重缓冲和深度测试
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	mainWindow = glutCreateWindow("3D Transfomations");

	glewExperimental = GL_TRUE;
	glewInit();

	init();
	//initskybox();
	//setupMenu();
	//glutDisplayFunc(display);
	//SetupRC();
	glutDisplayFunc(RenderScene);
	glutMouseFunc(Mouse);
	//glutMotionFunc(onMouseMove);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(ChangeSize);
	// 输出帮助信息
	printHelp();
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
	return 0;
}


