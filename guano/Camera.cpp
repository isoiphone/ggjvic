#include "Camera.h"
#include "Gamepad.h"

Camera::Camera() {
	m_pos = vector2f(0,0);
	m_zoom = 1.0;
};

Camera::~Camera() {
}

vector2f Camera::getCenter() {
	return vector2f(m_pos.x+kScreenWidth*0.5, m_pos.y+kScreenHeight*0.5);
}

void Camera::update(uint32_t elapsedMs, Gamepad* gamepad) {
//	if (gamepad->isHeld(GAMEPAD_A)) {
//		if (gamepad->isHeld(GAMEPAD_UP))
//			m_pos.y -= kCameraMovePerTick;
//		else if (gamepad->isHeld(GAMEPAD_DOWN))
//			m_pos.y += kCameraMovePerTick;
//		
//		if (gamepad->isHeld(GAMEPAD_LEFT))
//			m_pos.x -= kCameraMovePerTick;
//		else if (gamepad->isHeld(GAMEPAD_RIGHT))
//			m_pos.x += kCameraMovePerTick;
//	}
}