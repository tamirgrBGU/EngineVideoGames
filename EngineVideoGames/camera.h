#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Viewport.h"

struct Camera 
{
public:
	Camera(const glm::vec3& pos,const glm::vec3& forward, float fov, float zNear, float zFar, const Viewport &view)
	{
		this->pos = pos;
		this->forward = forward; 
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov,GetWHRelation() , zNear, zFar);
		this->projection = this->projection * glm::lookAt(pos, pos + forward, up);
		this->fov = fov;
		this->near = zNear;
		this->far = zFar;
		this->vp = view;
	}

	void setProjection(float zNear, float zFar, Viewport &view)
	{
		this->vp = view;
		this->projection = glm::perspective(fov,view.GetWHRelation(), zNear, zFar)* glm::lookAt(pos, pos + forward, up);
		this->near = zNear;
		this->far = zFar;		
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection ;
	}

	int GetWidth()
	{
		return vp.GetWidth();
	}

	int GetHeight()
	{
		return vp.GetHeight();
	}

	int GetLeft()
	{
		return vp.GetLeft();
	}

	int GetBottom()
	{
		return vp.GetBottom();
	}

	void MoveForward(float amt)
	{
		pos += forward * amt;
	}

	void MoveRight(float amt)
	{
		pos += glm::cross(up, forward) * amt;
	}

	void Pitch(float angle)
	{
		glm::vec3 right = glm::normalize(glm::cross(up, forward));

		forward = glm::vec3(glm::normalize(glm::rotate(angle, right) * glm::vec4(forward, 0.0)));
		up = glm::normalize(glm::cross(forward, right));
	}

	void RotateY(float angle)
	{
		static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

		glm::mat4 rotation = glm::rotate(angle, UP);

		forward = glm::vec3(glm::normalize(rotation * glm::vec4(forward, 0.0)));
		up = glm::vec3(glm::normalize(rotation * glm::vec4(up, 0.0)));
	}
	inline float GetAngle()
	{
		return fov;
	}

	inline float GetNear()
	{
		return near;
	}
	inline float GetFar()
	{
		return far;
	}
	inline float GetWHRelation()
	{
		return vp.GetWHRelation();
	}
protected:
private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
	float fov;
	float far,near;
	Viewport vp;
};

#endif
