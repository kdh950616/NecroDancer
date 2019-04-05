#include "stdafx.h"
#include "playGround.h"

//x 27부터 에이스타 안쫓아오는거 + 사운드 일정량 이상 재생안되는거.


playGround::playGround()
{
}


playGround::~playGround()
{
}

HRESULT playGround::init()
{
	gameNode::init(true);

	imageInit();
	soundInit();

	_mapEditor = new mapEditor;
	_testScene = new testScene;
	_bossScene = new stageBoss;
	_stage1 = new stage1;
	_stage2 = new stage2;
	_loadingScene = new loadingScene;
	_lobbyScene = new lobbyScene;

	//맵툴
	SCENEMANAGER->addScene("mapToolScene",_mapEditor);
	//테스트씬
	SCENEMANAGER->addScene("testScene", _testScene);
	//보스씬
	SCENEMANAGER->addScene("bossScene", _bossScene);
	//스테이지1씬
	SCENEMANAGER->addScene("stage1", _stage1);
	//스테이지2씬
	SCENEMANAGER->addScene("stage2", _stage2);
	//로딩씬
	SCENEMANAGER->addScene("loadingScene", _loadingScene);
	//로비씬
	SCENEMANAGER->addScene("lobbyScene", _lobbyScene);

	SCENEMANAGER->changeScene("loadingScene");                  
	
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
	// Draw 시작 - 이 코드가 빠지면 D2D 출력 X
	D2DMANAGER->beginDraw();
	//===========================================================================
	//				##		여기에 코드 작성(Start)			##

	SCENEMANAGER->render();

	//				##			여기에 코드 작성(End)			##
	//===========================================================================
	//				##카메라 정보 마우스 정보 시간정보 출력	##===================
	//WCHAR str[128];
	//swprintf_s(str, L"cameraX : %d", CAMERA->getPosX());
	////D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 80);
	//D2DMANAGER->drawText(str, 0, 80,20,0xff00ff);
	//swprintf_s(str, L"cameraY : %d", CAMERA->getPosY());
	////D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 100);
	//D2DMANAGER->drawText(str, 0, 100, 20, 0xff00ff);
	//
	//swprintf_s(str, L"mouseX : %.2f", _ptMouse.x);
	////D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 140);
	//D2DMANAGER->drawText(str, 0, 140, 20, 0xff00ff);
	//swprintf_s(str, L"mouseY : %.2f", _ptMouse.y);
	////D2DMANAGER->drawText(str, CAMERA->getPosX(), CAMERA->getPosY() + 160);
	//D2DMANAGER->drawText(str, 0, 160, 20, 0xff00ff);

	TIMEMANAGER->render();
	// Draw 끝 - 이 코드가 빠지면 D2D 출력 X
	D2DMANAGER->endDraw();
}

void playGround::imageInit()
{
	//==================================
	//			플레이어관련
	//==================================

	IMAGEMANAGER->addFrameImage("playerHead", L"images/player/head.png", 192, 96, 4, 2);
	IMAGEMANAGER->addFrameImage("playerBody", L"images/player/body.png", 192, 480, 4, 10);
	IMAGEMANAGER->addImage("playerShadow", L"images/player/shadow.png", 48, 54);
	//아이템 & 슬롯
	IMAGEMANAGER->addFrameImage("Item", L"images/item/item.png", 432, 576, 9, 12);
	IMAGEMANAGER->addImage("slot_Shovel", L"images/item/slot_Shovel.png", 60, 66);
	IMAGEMANAGER->addImage("slot_Weapon", L"images/item/slot_Weapon.png", 60, 66);
	IMAGEMANAGER->addImage("slot_Armor", L"images/item/slot_Armor.png", 60, 66);
	IMAGEMANAGER->addImage("slot_Torch", L"images/item/slot_Torch.png", 60, 66);
	IMAGEMANAGER->addImage("slot_Hp", L"images/item/slot_Hp.png", 60, 84);

	IMAGEMANAGER->addImage("ui_heart", L"images/ui/ui_heart.png", 48, 44);
	IMAGEMANAGER->addImage("ui_half_heart", L"images/ui/ui_heart_half.png", 48, 44);
	IMAGEMANAGER->addImage("ui_empty_heart", L"images/ui/ui_heart_empty.png", 48, 44);
	IMAGEMANAGER->addImage("ui_large_heart", L"images/ui/ui_heart_large.png", 57, 53);
	IMAGEMANAGER->addImage("ui_large_half_heart", L"images/ui/ui_half_heart_large.png", 57, 53);

	//이펙트 이미지
	IMAGEMANAGER->addFrameImage("effect_Dagger", L"images/effect/swipe_dagger.png", 144, 48, 3, 1);
	IMAGEMANAGER->addFrameImage("effect_Spear", L"images/effect/swipe_rapier.png", 384, 48, 4, 1);
	IMAGEMANAGER->addFrameImage("effect_Sword", L"images/effect/swipe_broadsword.png", 144, 144, 3, 1);


	//============================
	//			적관련
	//============================
	IMAGEMANAGER->addFrameImage("enemy_Attack", L"images/effect/swipe_enemy.png", 270, 48, 5, 1);
	IMAGEMANAGER->addImage("enemy_Heart", L"images/ui/TEMP_heart_small.png", 24, 24);
	IMAGEMANAGER->addImage("enemy_HeartEmpty", L"images/ui/TEMP_heart_empty_small.png", 24, 24);
	
	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
	IMAGEMANAGER->addImage("shadow_Large", L"images/monster/miniboss/large_shadow.png", 86, 28);
	IMAGEMANAGER->addFrameImage("dust", L"images/monster/normal/dust.png", 240, 48, 5, 1);

	IMAGEMANAGER->addFrameImage("zombie", L"images/monster/normal/zombie.png", 1152, 100, 24, 2);

	IMAGEMANAGER->addFrameImage("wraith", L"images/monster/normal/wraith.png", 144, 96, 3, 2);

	IMAGEMANAGER->addFrameImage("slime_Blue", L"images/monster/normal/slime_blue.png", 416, 100, 8, 2);

	IMAGEMANAGER->addFrameImage("slime_Normal", L"images/monster/normal/slime_green.png", 208, 104, 4, 2);

	IMAGEMANAGER->addFrameImage("skeleton_Black", L"images/monster/normal/skeleton_black.png", 432, 100, 9, 2);

	IMAGEMANAGER->addFrameImage("skeleton_Yellow", L"images/monster/normal/skeleton_yellow.png", 432, 100, 9, 2);

	IMAGEMANAGER->addFrameImage("skeleton_Normal", L"images/monster/normal/skeleton.png", 384, 100, 8, 2);

	IMAGEMANAGER->addFrameImage("clone", L"images/monster/normal/clone.png", 192, 96, 4, 2);

	IMAGEMANAGER->addFrameImage("bat_Red", L"images/monster/normal/bat_red.png", 192, 96, 4, 2);

	IMAGEMANAGER->addFrameImage("bat_Normal", L"images/monster/normal/bat.png", 192, 96, 4, 2);

	IMAGEMANAGER->addFrameImage("armadillo", L"images/monster/normal/armadillo.png", 528, 96, 11, 2);

	IMAGEMANAGER->addFrameImage("dragon", L"images/monster/miniboss/dragon.png", 216, 204, 2, 2);

	IMAGEMANAGER->addFrameImage("bat_Boss", L"images/monster/miniboss/bat_boss.png", 288, 96, 4, 2);

	IMAGEMANAGER->addFrameImage("banshee", L"images/monster/miniboss/banshee.png", 544, 156, 8, 2);

	IMAGEMANAGER->addFrameImage("rook", L"images/monster/boss/rook_red.png", 114, 104, 3, 2);

	IMAGEMANAGER->addFrameImage("queen", L"images/monster/boss/queen.png", 66, 156, 1, 2);

	IMAGEMANAGER->addFrameImage("pawn", L"images/monster/boss/pawn_red.png", 150, 92, 5, 2);

	IMAGEMANAGER->addFrameImage("knight", L"images/monster/boss/knight_red.png", 230, 114, 5, 2);
	IMAGEMANAGER->addFrameImage("knithg_Attack", L"images/effect/swipe_knight_red.png", 144, 96, 1, 1);
	IMAGEMANAGER->addFrameImage("knithg_Attack_R", L"images/effect/swipe_knight_red_reverse.png", 144, 96, 1, 1);

	IMAGEMANAGER->addFrameImage("king", L"images/monster/boss/king.png", 200, 180, 4, 2);

	IMAGEMANAGER->addFrameImage("bishop", L"images/monster/boss/bishop_red.png", 180, 128, 5, 2);

	//==============================
	//				맵관련
	//==============================

	IMAGEMANAGER->addImage("title", L"images/ui/mainmenu.png", 960, 540);
	IMAGEMANAGER->addImage("continue", L"images/ui/continue.png", 440, 36);

	IMAGEMANAGER->addImage("black", L"images/mapTool/black.png", 48, 48);

	//타일들 이미지
	IMAGEMANAGER->addFrameImage("tile", L"images/mapTool/tile.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("wall", L"images/mapTool/wall.png", 432, 576, 9, 6);
	IMAGEMANAGER->addFrameImage("item", L"images/mapTool/item.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("etc1", L"images/mapTool/etc1.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("etc2", L"images/mapTool/etc2.png", 432, 576, 3, 6);
	IMAGEMANAGER->addFrameImage("shopkeeper", L"images/item/shopkeeper2.png", 752, 152, 8, 2);

	//적 이미지
	IMAGEMANAGER->addFrameImage("enemy1", L"images/mapTool/mob1.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy2", L"images/mapTool/mob2.png", 432, 576, 9, 12);
	IMAGEMANAGER->addFrameImage("enemy3", L"images/mapTool/mob3.png", 432, 576, 3, 4);
	IMAGEMANAGER->addFrameImage("enemy4", L"images/mapTool/mob4.png", 432, 576, 3, 6);

	//비트 이미지
	IMAGEMANAGER->addFrameImage("beat_Heart", L"images/ui/beat_Heart.png", 160, 100, 2, 1);
	IMAGEMANAGER->addImage("beat_Green", L"images/ui/beat_Green.png", 12, 64);
	IMAGEMANAGER->addImage("beat_Red", L"images/ui/beat_Red.png", 12, 64);
	//코인배수
	IMAGEMANAGER->addImage("grooveChain", L"images/ui/game_coinmultiplier.png", 81, 18);
	IMAGEMANAGER->addFrameImage("number", L"images/ui/number.png", 40, 18, 4, 1);

	//아이템용 그림자
	IMAGEMANAGER->addImage("shadow_Standard", L"images/monster/normal/shadow_Standard.png", 48, 54);
}


void playGround::soundInit()
{
	SOUNDMANAGER->addSound("mapTool", "sounds/mapTool/boss_9.ogg", true, true);
	SOUNDMANAGER->addSound("mapTool_shopkeeper", "sounds/mapTool/boss_9_vocal.ogg", true, true);
	SOUNDMANAGER->addSound("stage1", "sounds/zone/zone1_1.ogg", true, true);
	SOUNDMANAGER->addSound("stage1_shopkeeper", "sounds/zone/zone1_1_shopkeeper.ogg", true, true);
	SOUNDMANAGER->addSound("stage2", "sounds/zone/zone2_1.ogg", true, true);
	SOUNDMANAGER->addSound("stage2_shopKeeper", "sounds/zone/zone2_1_shopkeeper.ogg", true, true);
	SOUNDMANAGER->addSound("bossScene", "sounds/zone/boss_3.ogg", true, true);
	
	SOUNDMANAGER->addSound("loadingScene", "sounds/zone/training.ogg", true,true);
	SOUNDMANAGER->addSound("lobbyScene", "sounds/zone/lobby.ogg", true,true);

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
	SOUNDMANAGER->addSound("banshee_Attack", "sounds/monster/miniboss/banshee/en_banshee_attack.ogg", false, false);
	SOUNDMANAGER->addSound("banshee_Cry", "sounds/monster/miniboss/banshee/en_banshee_cry.ogg", false, false);//
	SOUNDMANAGER->addSound("banshee_Death", "sounds/monster/miniboss/banshee/en_banshee_death.ogg", false, false);//
	SOUNDMANAGER->addSound("banshee_Hit1", "sounds/monster/miniboss/banshee/en_banshee_hit_01.ogg", false, false);//
	SOUNDMANAGER->addSound("banshee_Hit2", "sounds/monster/miniboss/banshee/en_banshee_hit_02.ogg", false, false);//
	SOUNDMANAGER->addSound("banshee_Hit3", "sounds/monster/miniboss/banshee/en_banshee_hit_03.ogg", false, false);//
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

	//piece
	SOUNDMANAGER->addSound("piece_Attack", "sounds/monster/boss/piece/en_blues_piece_attack.ogg", false, false);//
	SOUNDMANAGER->addSound("piece_death1", "sounds/monster/boss/piece/en_blues_piece_death_01.ogg", false, false);//
	SOUNDMANAGER->addSound("piece_death2", "sounds/monster/boss/piece/en_blues_piece_death_02.ogg", false, false);//
	SOUNDMANAGER->addSound("piece_death3", "sounds/monster/boss/piece/en_blues_piece_death_03.ogg", false, false);//

	//queen
	SOUNDMANAGER->addSound("queen_Attack1", "sounds/monster/boss/queen/en_blues_queen_sing_01.ogg", false, false);//
	SOUNDMANAGER->addSound("queen_Attack2", "sounds/monster/boss/queen/en_blues_queen_sing_02.ogg", false, false);//
	SOUNDMANAGER->addSound("queen_Attack3", "sounds/monster/boss/queen/en_blues_queen_sing_03.ogg", false, false);//
	SOUNDMANAGER->addSound("queen_Attack4", "sounds/monster/boss/queen/en_blues_queen_sing_04.ogg", false, false);//
	SOUNDMANAGER->addSound("queen_Attack5", "sounds/monster/boss/queen/en_blues_queen_sing_05.ogg", false, false);//
	SOUNDMANAGER->addSound("queen_Attack6", "sounds/monster/boss/queen/en_blues_queen_sing_06.ogg", false, false);//
	SOUNDMANAGER->addSound("queen_Death1", "sounds/monster/boss/queen/en_blues_queen_death_01.ogg", false, false);//
	SOUNDMANAGER->addSound("queen_Death2", "sounds/monster/boss/queen/en_blues_queen_death_02.ogg", false, false);//
	SOUNDMANAGER->addSound("queen_Death3", "sounds/monster/boss/queen/en_blues_queen_death_03.ogg", false, false);//

	//king
	SOUNDMANAGER->addSound("king_Cry", "sounds/monster/boss/king/en_blues_king_cry.ogg", false, false);
	SOUNDMANAGER->addSound("king_Death", "sounds/monster/boss/king/en_blues_king_death.ogg", false, false);//
	SOUNDMANAGER->addSound("king_Attack1", "sounds/monster/boss/king/en_blues_king_attack_01.ogg", false, false);
	SOUNDMANAGER->addSound("king_Attack2", "sounds/monster/boss/king/en_blues_king_attack_02.ogg", false, false);
	SOUNDMANAGER->addSound("king_Attack3", "sounds/monster/boss/king/en_blues_king_attack_03.ogg", false, false);
	SOUNDMANAGER->addSound("king_Attack4", "sounds/monster/boss/king/en_blues_king_attack_04.ogg", false, false);
	SOUNDMANAGER->addSound("king_Attack5", "sounds/monster/boss/king/en_blues_king_attack_05.ogg", false, false);
	SOUNDMANAGER->addSound("king_Attack6", "sounds/monster/boss/king/en_blues_king_attack_06.ogg", false, false);
	SOUNDMANAGER->addSound("king_Attack7", "sounds/monster/boss/king/en_blues_king_attack_07.ogg", false, false);
	SOUNDMANAGER->addSound("king_Hit1", "sounds/monster/boss/king/en_blues_king_hit_01.ogg", false, false);//
	SOUNDMANAGER->addSound("king_Hit2", "sounds/monster/boss/king/en_blues_king_hit_02.ogg", false, false);//
	SOUNDMANAGER->addSound("king_Hit3", "sounds/monster/boss/king/en_blues_king_hit_03.ogg", false, false);//
	SOUNDMANAGER->addSound("king_Hit4", "sounds/monster/boss/king/en_blues_king_hit_04.ogg", false, false);//



	//===============================================
	//				monster - boss
	//===============================================
	SOUNDMANAGER->addSound("bossStage_Battle", "sounds/zone/boss_zone1_walls.ogg", false, false);

	//===============================================
	//				obj
	//===============================================
	SOUNDMANAGER->addSound("doorOpen", "sounds/object/obj_door_open.ogg", false, false); //
	SOUNDMANAGER->addSound("dig1", "sounds/object/vo_cas_dig_01.ogg", false, false);//
	SOUNDMANAGER->addSound("dig2", "sounds/object/vo_cas_dig_02.ogg", false, false);//
	SOUNDMANAGER->addSound("dig3", "sounds/object/vo_cas_dig_03.ogg", false, false);//
	SOUNDMANAGER->addSound("dig4", "sounds/object/vo_cas_dig_04.ogg", false, false);//
	SOUNDMANAGER->addSound("dig5", "sounds/object/vo_cas_dig_05.ogg", false, false);//
	SOUNDMANAGER->addSound("dig6", "sounds/object/vo_cas_dig_06.ogg", false, false);//
	SOUNDMANAGER->addSound("wall1_Dig", "sounds/object/mov_dig_dirt.ogg", false, false);//
	SOUNDMANAGER->addSound("wall2_Dig", "sounds/object/mov_dig_stone.ogg", false, false);//
	SOUNDMANAGER->addSound("wall3_Dig", "sounds/object/mov_dig_brick.ogg", false, false);//
	SOUNDMANAGER->addSound("dig_Fail", "sounds/object/mov_dig_fail.ogg", false, false);//
	SOUNDMANAGER->addSound("pickup_Weapon", "sounds/object/sfx_pickup_weapon.ogg", false, false);//
	SOUNDMANAGER->addSound("pickup_Armor", "sounds/object/sfx_pickup_armor.ogg", false, false);//
	SOUNDMANAGER->addSound("pickup", "sounds/object/sfx_pickup_general_ST.ogg", false, false);//
	SOUNDMANAGER->addSound("chest", "sounds/object/obj_chest_open.ogg", false, false);
	SOUNDMANAGER->addSound("trap", "sounds/object/obj_trap_trapdoor_open.ogg", false, false);
	SOUNDMANAGER->addSound("trapdoor", "sounds/object/obj_trap_trapdoor_fall.ogg", false, false);
	//===============================================
	//				player
	//===============================================
	//atk
	SOUNDMANAGER->addSound("atk1_1", "sounds/player/atk/vo_cad_melee_1_01.ogg", false, false);//
	SOUNDMANAGER->addSound("atk1_2", "sounds/player/atk/vo_cad_melee_1_02.ogg", false, false);//
	SOUNDMANAGER->addSound("atk1_3", "sounds/player/atk/vo_cad_melee_1_03.ogg", false, false);//
	SOUNDMANAGER->addSound("atk1_4", "sounds/player/atk/vo_cad_melee_1_04.ogg", false, false);//
	SOUNDMANAGER->addSound("atk2_1", "sounds/player/atk/vo_cad_melee_2_01.ogg", false, false);//
	SOUNDMANAGER->addSound("atk2_2", "sounds/player/atk/vo_cad_melee_2_02.ogg", false, false);//
	SOUNDMANAGER->addSound("atk2_3", "sounds/player/atk/vo_cad_melee_2_03.ogg", false, false);//
	SOUNDMANAGER->addSound("atk2_4", "sounds/player/atk/vo_cad_melee_2_04.ogg", false, false);//
	SOUNDMANAGER->addSound("atk3_1", "sounds/player/atk/vo_cad_melee_3_01.ogg", false, false);//
	SOUNDMANAGER->addSound("atk3_2", "sounds/player/atk/vo_cad_melee_3_02.ogg", false, false);//
	SOUNDMANAGER->addSound("atk3_3", "sounds/player/atk/vo_cad_melee_3_03.ogg", false, false);//
	SOUNDMANAGER->addSound("atk3_4", "sounds/player/atk/vo_cad_melee_3_04.ogg", false, false);//
	SOUNDMANAGER->addSound("atk4_1", "sounds/player/atk/vo_cad_melee_4_01.ogg", false, false);//
	SOUNDMANAGER->addSound("atk4_2", "sounds/player/atk/vo_cad_melee_4_02.ogg", false, false);//
	SOUNDMANAGER->addSound("atk4_3", "sounds/player/atk/vo_cad_melee_4_03.ogg", false, false);//
	SOUNDMANAGER->addSound("atk4_4", "sounds/player/atk/vo_cad_melee_4_04.ogg", false, false);//
	//hit
	SOUNDMANAGER->addSound("hit1", "sounds/player/hit/vo_cad_hurt_01.ogg", false, false);//
	SOUNDMANAGER->addSound("hit2", "sounds/player/hit/vo_cad_hurt_02.ogg", false, false);//
	SOUNDMANAGER->addSound("hit3", "sounds/player/hit/vo_cad_hurt_03.ogg", false, false);//
	SOUNDMANAGER->addSound("hit4", "sounds/player/hit/vo_cad_hurt_04.ogg", false, false);//
	SOUNDMANAGER->addSound("hit5", "sounds/player/hit/vo_cad_hurt_05.ogg", false, false);//
	SOUNDMANAGER->addSound("hit6", "sounds/player/hit/vo_cad_hurt_06.ogg", false, false);//

	//heal
	SOUNDMANAGER->addSound("heal1", "sounds/player/heal/vo_cad_heal_01.ogg", false, false);//
	SOUNDMANAGER->addSound("heal2", "sounds/player/heal/vo_cad_heal_02.ogg", false, false);//
	SOUNDMANAGER->addSound("heal3", "sounds/player/heal/vo_cad_heal_03.ogg", false, false);//

	//item broke

	//===============================================
	//				ui & item
	//===============================================
	SOUNDMANAGER->addSound("hit", "sounds/player/hit/sfx_player_hit_ST.ogg", false, false);//
	SOUNDMANAGER->addSound("heal", "sounds/player/heal/sfx_item_food.ogg", false, false);//
	SOUNDMANAGER->addSound("brokeItem", "sounds/object/sfx_glass_break.ogg", false, false);//
	SOUNDMANAGER->addSound("grooveChainStart", "sounds/ui/sfx_chain_groove_ST.ogg", false, false);//
	SOUNDMANAGER->addSound("grooveChainFail", "sounds/ui/sfx_chain_break_ST.ogg", false, false);//
	SOUNDMANAGER->addSound("missBeat", "sounds/ui/sfx_missedbeat.ogg", false, false);//
}
