﻿
/**
* @file   jumpingState.h
* @brief  ジャンプ状態
*
* @author wem
* @date   2016.12.21
*/

#include "precompiled.h"


void JumpingState::setup(Player* player) {
  ofVec2f n_vel = player->getVel();
  n_vel.y = player->getJumpPow();
  player->setVel(n_vel);
}

void JumpingState::handleInput(Player* player, StateManager* stateMgr, ofxJoystick& input) {
  // 着地したらジャンプ状態を終了
  if (player->onFloor()) {
    landing(player);
    stateMgr->pop();
  }
}

void JumpingState::update(float deltaTime, Player* player, ofxJoystick& input) {}

void JumpingState::draw(Player* player) {}

void JumpingState::onCollision(Player* player, Actor* c_actor){}

// 着地後の座標と加速度をデフォルトに戻す処理
void JumpingState::landing(Player* player) {
  auto c_pos = player->getPos();
  c_pos.y    = 100;  // 仮の値
  player->setPos(c_pos);

  auto c_vel = player->getVel();
  c_vel.y    = -player->getGravity();
  player->setVel(c_vel);
}