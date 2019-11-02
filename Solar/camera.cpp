#include "camera.h"


void Camera::view1(float radius, float rotateAngle, float upAngle)
{
	eye[0] = radius * cos(upAngle * M_PI / 180.0) * sin(rotateAngle * M_PI / 180.0);
	eye[1] = radius * sin(upAngle * M_PI / 180.0);
	eye[2] = radius * cos(upAngle * M_PI / 180.0) * cos(rotateAngle * M_PI / 180.0);
	glLoadIdentity();
	//std::cout << eye << std::endl;
	gluLookAt(eye[0],eye[1],eye[2], 0, 0, 0, 0, 1, 0);
}



void Camera::rotateView(float angle, float x, float y, float z)
{
	vec3 newView;

	/** 计算方向向量 */
	vec3 view = at - eye;
	std::cout <<"x:"<< x << std::endl;
	//std::cout << "eye:" << eye << std::endl;
	/** 计算 sin 和cos值 */
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	/** 计算旋转向量的x值 */
	newView.x = (cosTheta + (1 - cosTheta) * x * x)        * view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)    * view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)    * view.z;

	/** 计算旋转向量的y值 */
	newView.y = ((1 - cosTheta) * x * y + z * sinTheta)    * view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)        * view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)    * view.z;

	/** 计算旋转向量的z值 */
	newView.z = ((1 - cosTheta) * x * z - y * sinTheta)    * view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)    * view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)        * view.z;

	/** 更新摄像机的方向 */
	at = eye + newView;
	

}

void Camera::setViewByMouse(int x, int y)
{
	POINT mousePos;                                   /**< 保存当前鼠标位置 */
	
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< 得到屏幕宽度的一半 */
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /**< 得到屏幕高度的一半 */
	float angleY = 0.0f;                              /**< 摄像机左右旋转角度 */
	float angleZ = 0.0f;                              /**< 摄像机上下旋转角度 */
	static float currentRotX = 0.0f;

	/** 得到当前鼠标位置 */
	GetCursorPos(&mousePos);
	ShowCursor(TRUE);
	
	/** 如果鼠标没有移动,则不用更新 */
	if ((mousePos.x == middleX) && (mousePos.y == middleY))
		return;

	/** 设置鼠标位置在屏幕中心 */
	SetCursorPos(middleX, middleY);

	/**< 得到鼠标移动方向 */
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;      /**< 用于保存旋转角度 */
	lastRotX = currentRotX;

	/** 跟踪摄像机上下旋转角度 */
	currentRotX += angleZ;
	
	/** 如果上下旋转弧度大于1.0,我们截取到1.0并旋转 */
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		/** 根据保存的角度旋转方向 */
		if (lastRotX != 1.0f)
		{
			/** 找到与旋转方向垂直向量 */
			vec3 vAxis = at - eye;
			vec3 vAxis2 = cross(vAxis, up);
			vec3 vAxis3 = normalize(vAxis2);

			///旋转
			rotateView(angleZ, vAxis3.x, vAxis3.y, vAxis3.z);
		}
	}
	/** 如果旋转弧度小于-1.0,则也截取到-1.0并旋转 */
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;

		if (lastRotX != -1.0f)
		{

			/** 找到与旋转方向垂直向量 */
			vec3 vAxis = at - eye;
			vec3 vAxis2 = cross(vAxis, up);
			vec3 vAxis3 = normalize(vAxis2);

			///旋转
			rotateView(angleZ, vAxis3.x, vAxis3.y, vAxis3.z);
		}
	}
	/** 否则就旋转angleZ度 */
	else
	{
		/** 找到与旋转方向垂直向量 */
		vec3 vAxis = at - eye;
		vec3 temp = up;
		vec3 vAxis2 = cross(vAxis, up);
		vec3 vAxis3 = normalize(vAxis2);

		///旋转
		rotateView(angleZ, vAxis3.x, vAxis3.y, vAxis3.z);
	}

	/** 总是左右旋转摄像机 */
	rotateView(angleY, 0, 1, 0);

}

void Camera::yawCamera(float speed)
{
}

void Camera::moveCamera(float speed)
{
}

void Camera::view2()
{
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
}


/*******Reference:
https://blog.csdn.net/u010223072/article/details/53379231
*****************/