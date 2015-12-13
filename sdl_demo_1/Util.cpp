/*************************************************************************************

Program:			CS 4250 Final Project

Author:				Alexander Bagnall
Email:				ab667712@ohio.edu

Description:		Implementation file for the Util class.

Date:				December 9, 2015

*************************************************************************************/

#include "Util.h"

namespace Bagnall
{
	// PUBLIC

	std::vector<vec4> Util::TransformVertices(const std::vector<vec4>& vertices, const mat4& transform)
	{
		std::vector<vec4> transformedVertices;
		for (std::vector<vec4>::const_iterator it = vertices.begin(); it != vertices.end(); ++it)
		{
			transformedVertices.push_back(transform * *it);
		}
		return transformedVertices;
	}

	float Util::Distance(const vec4& p1, const vec4& p2)
	{
		//return length(p1 - p2);
		vec4 d = p2 - p1;
		return std::sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
	}

	float Util::Distance(const vec2& p1, const vec2& p2)
	{
		return length(p1 - p2);
	}

	float Util::DistanceSquared(const vec4& p1, const vec4& p2)
	{
		/*vec4 displacement = p2 - p1;
		return dot(displacement, displacement);*/
		vec4 d = p2 - p1;
		return d.x*d.x + d.y*d.y + d.z*d.z;
	}

	float Util::DistanceSquared(const vec2& p1, const vec2& p2)
	{
		vec2 displacement = p2 - p1;
		return dot(displacement, displacement);
	}

	std::string Util::TimeToString(int time)
	{
		time /= 1000;

		int seconds = time % 60;
		time /= 60;

		int minutes = time % 60;
		time /= 60;

		return (time != 0 ? ToStringWithLeadingZeroes(time, 2) + ":" : "")
			+ ToStringWithLeadingZeroes(minutes, 2) + ":"
			+ ToStringWithLeadingZeroes(seconds, 2);
	}

	mat4 Util::RotateX(float theta)
	{
		mat4 c;
		c[2][2] = c[1][1] = cos(theta);
		c[2][1] = sin(theta);
		c[1][2] = -c[2][1];
		return c;
	}

	mat4 Util::RotateY(float theta)
	{
		mat4 c;
		c[2][2] = c[0][0] = cos(theta);
		c[0][2] = sin(theta);
		c[2][0] = -c[0][2];
		return c;
	}

	mat4 Util::RotateZ(float theta)
	{
		mat4 c;
		c[0][0] = c[1][1] = cos(theta);
		c[1][0] = sin(theta);
		c[0][1] = -c[1][0];
		return c;
	}

	mat4 Util::LookAt(const vec3& eye, const vec3& at, const vec3& up)
	{
		vec3 n = normalize(eye - at);
		vec3 u = normalize(cross(up, n));
		vec3 v = normalize(cross(n, u));
		vec4 t = vec4(0.0, 0.0, 0.0, 1.0);
		mat4 c = mat4(vec4(u, 0.0f), vec4(v, 0.0f), vec4(n, 0.0f), t);
		return c * translate(-eye);
	}

	mat4 Util::Perspective(const GLfloat fovy, const GLfloat aspect, const GLfloat zNear, const GLfloat zFar)
	{
		GLfloat top = tan(fovy*DegreesToRadians / 2) * zNear;
		GLfloat right = top * aspect;

		mat4 c;
		c[0][0] = zNear / right;
		c[1][1] = zNear / -top;
		c[2][2] = -(zFar + zNear) / (zFar - zNear);
		c[2][3] = -2.0*zFar*zNear / (zFar - zNear);
		c[3][2] = -1.0;
		c[3][3] = 0.0;
		return c;
	}
}
