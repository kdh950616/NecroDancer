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
	_testScene = new testScene;
	_bossCoral = new boss_Coralriff;


	SOUNDMANAGER->addSound("mapTool", "sounds/mapTool/boss_9.ogg", true, true);
	SOUNDMANAGER->addSound("mapTool_shopkeeper", "sounds/mapTool/boss_9_vocal.ogg", true, true);
	SOUNDMANAGER->addSound("testScene", "sounds/zone/zone1_1.ogg", true, true);
	SOUNDMANAGER->addSound("testScene_shopkeeper", "sounds/zone/zone1_1_shopkeeper.ogg", true, true);

	//===============================================
	//				monster - Normal
	//===============================================
	//armadillo
	SOUNDMANAGER->addSound("armadillo_Attack", "sounds/monster/normal/armadillo/en_armadillo_attack.ogg", false, false);	//
	SOUNDMANAGER->addSound("armadillo_Rush", "sounds/monster/normal/armadillo/en_armadillo_cry.ogg", false, false);			//
	SOUNDMANAGER->addSound("armadillo_Death", "sounds/monster/normal/armadillo/en_armadillo_death.ogg", false, false);		//
	SOUNDMANAGER->addSound("armadillo_Hit", "sounds/monster/normal/armadillo/en_armadillo_hit.ogg", false, false);			//
	SOUNDMANAGER->addSound("armadillo_Block", "sounds/monster/normal/armadillo/en_armadillo_shielded.ogg", false, false);	//
	SOUNDMANAGER->addSound("armadillo_Wall", "sounds/monster/normal/armadillo/en_armadillo_wallImpact.ogg", false, false);	//
	//bat
	SOUNDMANAGER->addSound("bat_Attack", "sounds/monster/normal/bat/en_bat_attack.ogg", false, false);	//
	SOUNDMANAGER->addSound("bat_Death", "sounds/monster/normal/bat/en_bat_death.ogg", false, false);//
	SOUNDMANAGER->addSound("bat_Hit", "sounds/monster/normal/bat/en_bat_hit.ogg", false, false);//
	//clone
	SOUNDMANAGER->addSound("clone_Attack", "sounds/monster/normal/clone/en_clone_attack.ogg", false, false); //
	SOUNDMANAGER->addSound("clone_Death", "sounds/monster/normal/clone/en_clone_death.ogg", false, false); //
	//skeleton
	SOUNDMANAGER->addSound("skeleton_Attack", "sounds/monster/normal/skeleton/en_skel_attack_melee.ogg", false, false);//
	SOUNDMANAGER->addSound("skeleton_Death", "sounds/monster/normal/skeleton/en_skel_death.ogg", false, false);//
	SOUNDMANAGER->addSound("skeleton_HeadLess1", "sounds/monster/normal/skeleton/en_skel_head_loss_01.ogg", false, false);//
	SOUNDMANAGER->addSound("skeleton_HeadLess2", "sounds/monster/normal/skeleton/en_skel_head_loss_02.ogg", false, false);//
	SOUNDMANAGER->addSound("skeleton_HeadLess3", "sounds/monster/normal/skeleton/en_skel_head_loss_03.ogg", false, false);//
	SOUNDMANAGER->addSound("skeleton_Hit1", "sounds/monster/normal/skeleton/en_skel_hit.ogg", false, false);//
	SOUNDMANAGER->addSound("skeleton_Hit2", "sounds/monster/normal/skeleton/en_skel_hit_01.ogg", false, false);//
	SOUNDMANAGER->addSound("skeleton_Hit3", "sounds/monster/normal/skeleton/en_skel_hit_02.ogg", false, false);//
	SOUNDMANAGER->addSound("skeleton_Hit4", "sounds/monster/normal/skeleton/en_skel_hit_03.ogg", false, false);//
	//slime
	SOUNDMANAGER->addSound("slime_Attack", "sounds/monster/normal/slime/en_slime_attack.ogg", false, false);//
	SOUNDMANAGER->addSound("slime_Death1", "sounds/monster/normal/slime/en_slime_death.ogg", false, false);//
	SOUNDMANAGER->addSound("slime_Death2", "sounds/monster/normal/slime/en_slime_death_01.ogg", false, false);//
	SOUNDMANAGER->addSound("slime_Death3", "sounds/monster/normal/slime/en_slime_death_02.ogg", false, false);//
	SOUNDMANAGER->addSound("slime_Death4", "sounds/monster/normal/slime/en_slime_death_03.ogg", false, false);//
	SOUNDMANAGER->addSound("slime_Hit1", "sounds/monster/normal/slime/en_slime_hit.ogg", false, false);//
	SOUNDMANAGER->addSound("slime_Hit2", "sounds/monster/normal/slime/en_slime_hit_01.ogg", false, false);//
	SOUNDMANAGER->addSound("slime_Hit3", "sounds/monster/normal/slime/en_slime_hit_02.ogg", false, false);//
	SOUNDMANAGER->addSound("slime_Hit4", "sounds/monster/normal/slime/en_slime_hit_03.ogg", false, false);//
	//wraith
	SOUNDMANAGER->addSound("wraith_Attack", "sounds/monster/normal/wraith/en_wraith_attack.ogg", false, false);//
	SOUNDMANAGER->addSound("wraith_Cry", "sounds/monster/normal/wraith/en_wraith_cry.ogg", false, false);//
	SOUNDMANAGER->addSound("wraith_Death", "sounds/monster/normal/wraith/en_wraith_death.ogg", false, false);//
	//zombie
	SOUNDMANAGER->addSound("zombie_Attack", "sounds/monster/normal/zombie/en_zombie_attack.ogg", false, false);//
	SOUNDMANAGER->addSound("zombie_Death", "sounds/monster/normal/zombie/en_zombie_death.ogg", false, false);//
	//===============================================
	//				monster - Miniboss
	//===============================================
	//banshee
	SOUNDMANAGER->addSound("banshee_Loop", "sounds/monster/miniboss/banshee/en_banshee_loop.ogg", false, true);//
	SOUNDMANAGER->addSound("banshee_Attack", "sounds/monster/miniboss/banshee/en_banshee_attack.ogg", false,false);
	SOUNDMANAGER->addSound("banshee_Cry", "sounds/monster/miniboss/banshee/en_banshee_cry.ogg", false,false);//
	SOUNDMANAGER->addSound("banshee_Death", "sounds/monster/miniboss/banshee/en_banshee_death.ogg", false,false);//
	SOUNDMANAGER->addSound("banshee_Hit1", "sounds/monster/miniboss/banshee/en_banshee_hit_01.ogg", false,false);//
	SOUNDMANAGER->addSound("banshee_Hit2", "sounds/monster/miniboss/banshee/en_banshee_hit_02.ogg", false,false);//
	SOUNDMANAGER->addSound("banshee_Hit3", "sounds/monster/miniboss/banshee/en_banshee_hit_03.ogg", false,false);//
	//bat_Boss
	SOUNDMANAGER->addSound("bat_Boss_Attack", "sounds/monster/miniboss/bat/en_bat_miniboss_attack.ogg", false, false);//
	SOUNDMANAGER->addSound("bat_Boss_Death", "sounds/monster/miniboss/bat/en_bat_miniboss_death.ogg", false, false);//
	SOUNDMANAGER->addSound("bat_Boss_Hit", "sounds/monster/miniboss/bat/en_bat_miniboss_hit.ogg", false, false);//
	//dragon
	SOUNDMANAGER->addSound("dragon_Attack", "sounds/monster/miniboss/dragon/en_dragon_attack_melee.ogg", false, false);//
	SOUNDMANAGER->addSound("dragon_Cry", "sounds/monster/miniboss/dragon/en_dragon_cry.ogg", false, false);//
	SOUNDMANAGER->addSound("dragon_Death", "sounds/monster/miniboss/dragon/en_dragon_death.ogg", false, false);//
	SOUNDMANAGER->addSound("dragon_Hit1", "sounds/monster/miniboss/dragon/en_dragon_hit_01.ogg", false, false);//
	SOUNDMANAGER->addSound("dragon_Hit2", "sounds/monster/miniboss/dragon/en_dragon_hit_02.ogg", false, false);//
	SOUNDMANAGER->addSound("dragon_Hit3", "sounds/monster/miniboss/dragon/en_dragon_hit_03.ogg", false, false);//
	SOUNDMANAGER->addSound("dragon_Walk1", "sounds/monster/miniboss/dragon/en_dragon_walk_01.ogg", false, false);//
	SOUNDMANAGER->addSound("dragon_Walk2", "sounds/monster/miniboss/dragon/en_dragon_walk_02.ogg", false, false);//
	SOUNDMANAGER->addSound("dragon_Walk3", "sounds/monster/miniboss/dragon/en_dragon_walk_03.ogg", false, false);//


	//===============================================
	//				monster - boss
	//===============================================




	SCENEMANAGER->addScene("mapToolScene",_mapEditor);
	SCENEMANAGER->addScene("testScene", _testScene);
	SCENEMANAGER->addScene("bossCoral", _bossCoral);

	//맵툴
	//SCENEMANAGER->changeScene("mapToolScene");
	//테스트씬
	//SCENEMANAGER->changeScene("testScene");
	//코랄리프씬
	SCENEMANAGER->changeScene("bossCoral");
	
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
	EFFECTMANAGER->update();

}

void playGround::render()
{
	// Draw 시작 - 이 코드가 빠지면 D2D 출력 X
	D2DMANAGER->beginDraw();
	//===========================================================================
	//				##		여기에 코드 작성(Start)			##



	SCENEMANAGER->render();
	EFFECTMANAGER->render();



	//				##			여기에 코드 작성(End)			##
	//===========================================================================
	//				##카메라 정보 마우스 정보 시간정보 출력	##===================
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
	// Draw 끝 - 이 코드가 빠지면 D2D 출력 X
	D2DMANAGER->endDraw();
}