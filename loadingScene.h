#pragma once
#include "gameNode.h"

class loadingScene :public gameNode
{
private:
public:
	loadingScene();
	~loadingScene();

	HRESULT init();
	void release();
	void update();
	void render();
};

