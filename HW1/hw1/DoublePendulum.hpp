#ifndef _CG_HW1_DBLPEN_H_
#define _CG_HW1_DBLPEN_H_

#include "Vector.hpp"
#include <deque>

/**
 * This class is for representing a single double pendulum system abstractly.
 * 
*/

class DoublePendulum
{
public:

	DoublePendulum(Vector3f rootPosition, float topRodLength, float bottomRodLength, float topBobWeight, float bottomBobWeight, bool isLightSource, GLuint textureID)
	{
		this->rootPosition = rootPosition;
		this->topRodLength = topRodLength;
		this->bottomRodLength = bottomRodLength;
		this->topBobWeight = topBobWeight;
		this->bottomBobWeight = bottomBobWeight;
		this->isLightSource = isLightSource;
		this->textureID = textureID;
	}

	void render()
	{
		// reminder : the radius of a bob should be proportional to the weight of the bob ( r : weight^(0.33) )

		// you can use GLUquadricObj to draw sphere more easily, but please remember to Translate to the 
		// correct position.

		//GLUquadricObj* quad = gluNewQuadric();
		//gluSphere(quad, r, 20, 20);

		// write your code here ...
		positionRecord.push_back(GetBottomBobPosition());
		if (positionRecord.size() > 150)
			positionRecord.pop_front();

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		Vector3f topPos = GetTopBobPosition();
		Vector3f bottomPos = GetBottomBobPosition();
		glLineWidth(5);
		glBegin(GL_LINES);
		glColor3f(1, 0, 0);
		glVertex3f(this->rootPosition.x, this->rootPosition.y, this->rootPosition.z);
		glVertex3f(topPos.x, topPos.y, topPos.z);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(0, 10, 0);
		glVertex3f(topPos.x, topPos.y, topPos.z);
		glVertex3f(bottomPos.x, bottomPos.y, bottomPos.z);
		glEnd();

		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glLineWidth(2);
		glBegin(GL_LINE_STRIP);
		glColor3f(0, 0, 0.9);
		for (int i = 0; i < positionRecord.size(); ++i)
			glVertex3f(positionRecord[i].x, positionRecord[i].y, positionRecord[i].z);
		glEnd();
		
		glEnable(GL_LIGHTING);
		if (isLightSource) {
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glPushMatrix();
			glTranslatef(bottomPos.x, bottomPos.y, bottomPos.z);
			glColor3f(1, 1, 1);
			float r = powf(bottomBobWeight, 0.33) * 0.3;
			GLUquadric* quadratic = gluNewQuadric();         // Create A Pointer To The Quadric Object  
			gluQuadricTexture(quadratic, GL_TRUE);      // Create Texture Coords  
			gluQuadricNormals(quadratic, GLU_SMOOTH);   // Create Smooth Normals  
			gluSphere(quadratic, r, 20, 20);
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_LIGHTING);
		}
		else {
			glPushMatrix();
			glTranslatef(bottomPos.x, bottomPos.y, bottomPos.z);
			glColor3f(1, 1, 1);
			float r = powf(bottomBobWeight, 0.33) * 0.3;
			glutSolidSphere(r, 20, 20);
			glPopMatrix();
		}

		glPushMatrix();
		glTranslatef(topPos.x, topPos.y, topPos.z);		
		glRotatef(topAngle*180 * M_1_PI, 0, 0, -1);
		glColor3f(1, 1, 1);
		float r = powf(topBobWeight, 0.33) * 0.3;
		glutSolidCube(r);
		glPopMatrix();

		
		glEnable(GL_TEXTURE_2D);
	}

	void updatePhysics(float deltaTime)
	{
		// Reference : https://www.youtube.com/watch?v=uWzPe_S-RVE

		// this method should be erased when publishing template project
		const float g = 9.8;

		const float m1 = topBobWeight;
		const float m2 = bottomBobWeight;
		const float a1 = topAngle;
		const float a2 = bottomAngle;
		const float r1 = topRodLength;
		const float r2 = bottomRodLength;
		const float a1_v = topAngleVelocity;
		const float a2_v = bottomAngleVelocity;

		// calculate topAngleAcceleration
		// write your code here...
		// topAngleAcceleration = ...
		float num1 = -g * (2 * m1 + m2) * sin(a1);
		float num2 = -m2 * g * sin(a1 - 2 * a2);
		float num3 = -2 * sin(a1 - a2) * m2;
		float num4 = a2_v*a2_v*r2 + a1_v*a1_v*r1*cos(a1 - a2);
		float den = r1 * (2 * m1 + m2 - m2*cos(2 * a1 - 2 * a2));
		topAngleAcceleration = (num1 + num2 + num3*num4) / den;

		// calculate bottomAngleAcceleration
		// write your code here
		// bottomAngleAcceleration = ...
		num1 = 2 * sin(a1 - a2);
		num2 = (a1_v*a1_v*r1*(m1 + m2));
		num3 = g * (m1 + m2) * cos(a1);
		num4 = a2_v*a2_v*r2*m2*cos(a1 - a2);
		den = r2 * (2 * m1 + m2 - m2*cos(2 * a1 - 2 * a2));
		bottomAngleAcceleration = num1*(num2 + num3 + num4) / den;

		// apply and record
		topAngleVelocity += topAngleAcceleration * deltaTime;
		bottomAngleVelocity += bottomAngleAcceleration * deltaTime;

		topAngle += topAngleVelocity * deltaTime;
		bottomAngle += bottomAngleVelocity * deltaTime;
	}

public:
	float topRodLength;
	float topBobWeight;
	float bottomRodLength;
	float bottomBobWeight;

	// for rendering settings
	bool isLightSource = false;
	bool isUsingTexture = false;
	GLuint textureID = 0;

	Vector3f GetTopBobPosition()
	{
		// write your code here...
		return Vector3f(rootPosition.x - topRodLength*sinf(topAngle), 
						rootPosition.y - topRodLength*cosf(topAngle),
						rootPosition.z);
	}

	Vector3f GetBottomBobPosition()
	{
		// write your code here...
		Vector3f topPosition = GetTopBobPosition();
		return Vector3f(topPosition.x - bottomRodLength*sinf(bottomAngle),
						topPosition.y - bottomRodLength*cosf(bottomAngle), 
						topPosition.z);
	}

private:
	Vector3f rootPosition;
	std::deque<Vector3f> positionRecord;

	// for updating motion
	float topAngle = M_PI / 4; // in this template, this is represented in radian form
	float bottomAngle = 0; // in this template, this is represented in radian form

	float topAngleVelocity = 0;
	float bottomAngleVelocity = 0;

	float topAngleAcceleration = 0;
	float bottomAngleAcceleration = 0;
};

#endif