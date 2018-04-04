#include "Camera.h"
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() {
	position = glm::vec3(0, 100, 0);
	up = glm::vec3(0, 1, 0);
	look_dir = glm::vec3(0, 0, -1);
	projection_matrix = glm::mat4(1);
	view_matrix = glm::lookAt(position, position + look_dir, up);
}

Camera::~Camera() {
	
}

glm::vec3 Camera::getPosition(){
	return position;
}

glm::vec3 Camera::getUp(){
	return up;
}

glm::vec3 Camera::getLookDirection(){
	return look_dir;
}

void Camera::update(glm::vec3 position, glm::vec3 up, glm::vec3 look_dir){
	this->position = position;
	this->up = up;
	this->look_dir = look_dir;

	view_matrix = glm::lookAt(position, position + look_dir, up);
}

void Camera::setProjectionFrustum(float fov, float ratio, float near, float far){
	projection_matrix = glm::perspective(fov, ratio, near, far);
}

void Camera::setProjectionOrtho(float left, float right, float bottom, float top){
	projection_matrix = glm::ortho(left, right, bottom, top);
}
