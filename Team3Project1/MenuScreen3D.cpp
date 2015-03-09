#include "MenuScreen3D.h"


MenuScreen3D::MenuScreen3D(void) {

}

MenuScreen3D::~MenuScreen3D(void) {

}

void MenuScreen3D::LoadContent(void) {
	camera = new Camera(45.0f, 0.0f, 0.0f, T3Vector3(0.0f, -100.0f, 0.0f));
	SetCamera(camera);
}

void MenuScreen3D::Update(void) {
	
}

void MenuScreen3D::UnloadContent(void) {
	
}

void MenuScreen3D::DrawContent(void) {

}

void MenuScreen3D::UnDrawContent(void) {

}
