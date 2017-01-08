
/**
* @file   warpManager.h
* @brief  ワープゾーンマネージャ－
*
* @author f.naoto
* @date   2017.1.8
*/
#include "precompiled.h"


WarpManager::WarpManager() {
	name_ = "WarpManager";
	tag_ = WARP_MANAGER;

	player_ = nullptr;
	spawnPos_ = ofVec2f(ofRandom(spwPosXMin_, spwPosXMax_),	g_local->Height());
	destPos_ = ofVec2f(g_local->HalfWidth(), g_local->Height());
}

void WarpManager::setup() {
	warpZone_ = make_shared<WarpZone>();
	warpZone_->setPos(spawnPos_);
	warpZone_->setDistination(ofVec2f(destPos_.x, destPos_.y + warpZone_->getPos().y));
	AddActor(warpZone_);

	enableUpdate();
}

void WarpManager::update(float deltaTime) {
	//生成位置参照用のplayerを取得する
	if (!player_) {
		player_ = FindActor(PLAYER);
		return;
	}

	if (warpZone_->shouldDestroy()) {
		spawnWarp();
	}

	//プレイヤーと一定距離離れた場合、warpZoneを消す
	if ((player_->getPos().y - limitPos_) > warpZone_->getPos().y) {
		warpZone_->destroy();
	}
}

//ワープゾーンを生成する場所
void WarpManager::spawnWarp() {
	spawnPos_.x = ofRandom(spwPosXMin_, spwPosXMax_);

	warpZone_ = make_shared<WarpZone>();
	warpZone_->setPos(ofVec2f(spawnPos_.x, spawnPos_.y + player_->getPos().y));
	warpZone_->setDistination(ofVec2f(destPos_.x, destPos_.y + warpZone_->getPos().y));
	AddActor(warpZone_);
}