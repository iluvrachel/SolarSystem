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

	/** ���㷽������ */
	vec3 view = at - eye;
	std::cout <<"x:"<< x << std::endl;
	//std::cout << "eye:" << eye << std::endl;
	/** ���� sin ��cosֵ */
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	/** ������ת������xֵ */
	newView.x = (cosTheta + (1 - cosTheta) * x * x)        * view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)    * view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)    * view.z;

	/** ������ת������yֵ */
	newView.y = ((1 - cosTheta) * x * y + z * sinTheta)    * view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)        * view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)    * view.z;

	/** ������ת������zֵ */
	newView.z = ((1 - cosTheta) * x * z - y * sinTheta)    * view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)    * view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)        * view.z;

	/** ����������ķ��� */
	at = eye + newView;
	

}

void Camera::setViewByMouse(int x, int y)
{
	POINT mousePos;                                   /**< ���浱ǰ���λ�� */
	
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /**< �õ���Ļ��ȵ�һ�� */
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /**< �õ���Ļ�߶ȵ�һ�� */
	float angleY = 0.0f;                              /**< �����������ת�Ƕ� */
	float angleZ = 0.0f;                              /**< �����������ת�Ƕ� */
	static float currentRotX = 0.0f;

	/** �õ���ǰ���λ�� */
	GetCursorPos(&mousePos);
	ShowCursor(TRUE);
	
	/** ������û���ƶ�,���ø��� */
	if ((mousePos.x == middleX) && (mousePos.y == middleY))
		return;

	/** �������λ������Ļ���� */
	SetCursorPos(middleX, middleY);

	/**< �õ�����ƶ����� */
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;      /**< ���ڱ�����ת�Ƕ� */
	lastRotX = currentRotX;

	/** ���������������ת�Ƕ� */
	currentRotX += angleZ;
	
	/** ���������ת���ȴ���1.0,���ǽ�ȡ��1.0����ת */
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		/** ���ݱ���ĽǶ���ת���� */
		if (lastRotX != 1.0f)
		{
			/** �ҵ�����ת����ֱ���� */
			vec3 vAxis = at - eye;
			vec3 vAxis2 = cross(vAxis, up);
			vec3 vAxis3 = normalize(vAxis2);

			///��ת
			rotateView(angleZ, vAxis3.x, vAxis3.y, vAxis3.z);
		}
	}
	/** �����ת����С��-1.0,��Ҳ��ȡ��-1.0����ת */
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;

		if (lastRotX != -1.0f)
		{

			/** �ҵ�����ת����ֱ���� */
			vec3 vAxis = at - eye;
			vec3 vAxis2 = cross(vAxis, up);
			vec3 vAxis3 = normalize(vAxis2);

			///��ת
			rotateView(angleZ, vAxis3.x, vAxis3.y, vAxis3.z);
		}
	}
	/** �������תangleZ�� */
	else
	{
		/** �ҵ�����ת����ֱ���� */
		vec3 vAxis = at - eye;
		vec3 temp = up;
		vec3 vAxis2 = cross(vAxis, up);
		vec3 vAxis3 = normalize(vAxis2);

		///��ת
		rotateView(angleZ, vAxis3.x, vAxis3.y, vAxis3.z);
	}

	/** ����������ת����� */
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