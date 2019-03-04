#include "stdafx.h"
#include "playGround.h"

playGround::playGround()
{
}


playGround::~playGround()
{
}

HRESULT playGround::init()
{
	gameNode::init(true);

	_mapEditor = new mapEditor;

	SCENEMANAGER->addScene("mapToolScene",_mapEditor);
	SCENEMANAGER->changeScene("mapToolScene");
	

	
	return S_OK;
}

void playGround::release()
{
	gameNode::release();
	
}

void playGround::update()
{
	gameNode::update();

	SCENEMANAGER->update();

}

void playGround::render()
{
	// Draw ���� - �� �ڵ尡 ������ D2D ��� X
	D2DMANAGER->beginDraw();
	//===========================================================================
	//				##		���⿡ �ڵ� �ۼ�(Start)			##



	SCENEMANAGER->render();




	//				##			���⿡ �ڵ� �ۼ�(End)			##
	//===========================================================================
	//				##ī�޶� ���� ���콺 ���� �ð����� ���	##===================
	WCHAR str[128];
	swprintf_s(str, L"cameraX : %d", CAMERA->getPosX());
	//D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 80);
	D2DMANAGER->drawText(str, 0, 80,20,0xff00ff);
	swprintf_s(str, L"cameraY : %d", CAMERA->getPosY());
	//D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 100);
	D2DMANAGER->drawText(str, 0, 100, 20, 0xff00ff);

	swprintf_s(str, L"mouseX : %.2f", _ptMouse.x);
	//D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 140);
	D2DMANAGER->drawText(str, 0, 140, 20, 0xff00ff);
	swprintf_s(str, L"mouseY : %.2f", _ptMouse.y);
	//D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 160);
	D2DMANAGER->drawText(str, 0, 160, 20, 0xff00ff);

	TIMEMANAGER->render();
	// Draw �� - �� �ڵ尡 ������ D2D ��� X
	D2DMANAGER->endDraw();
}