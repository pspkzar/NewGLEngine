/*
 * Camera.h
 *
 *  Created on: 20/02/2015
 *      Author: cesar
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Camera {
public:
	Camera();
	virtual ~Camera();

	glm::vec3 getPosition();
	glm::vec3 getUp();
	glm::vec3 getLookDirection();

	glm::mat4 getViewMatrix(){
		return view_matrix;
	}

	glm::mat4 getProjectionMatrix(){
		return projection_matrix;
	}

	void update(glm::vec3 position, glm::vec3 up, glm::vec3 look_dir);
	void setProjectionFrustum(float fov, float ratio, float near, float far);
	void setProjectionOrtho(float left, float right, float bottom, float top);

private:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 look_dir;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
};

#endif /* CAMERA_H_ */
