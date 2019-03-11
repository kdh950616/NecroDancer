#pragma once
#include "image.h"


class gameNode
{
private:

	//�Ŵ����� �̴��Ҳ��� ����
	bool _managerInit;

public:
	gameNode();
	virtual ~gameNode();

	//���� ���� -> ����� ��忡�� �ʱ�ȭ�� �ߵǾ����� ������ ������ ���� ����
	//S_OK, E_FAIL, SUCCEDED ������ �˷���.
	virtual HRESULT init();
	virtual HRESULT init(bool managerInit);
	virtual void release();
	virtual void update();
	virtual void render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	RECT  rectMake(D2D_RECT_F rc) { return RECT{ (LONG)rc.left,(LONG)rc.top,(LONG)rc.right,(LONG)rc.bottom }; }
	POINT pointMake(POINTF pt) { return POINT{ (int)pt.x,(int)pt.y }; }

	

};

