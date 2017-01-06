﻿
/**
* @file   teleportState.h
* @brief  スキル状態
*
* @author wem
* @date   2016.1.2
*/

#include "precompiled.h"


//! TeleportStateに遷移した瞬間にフレームレートを下げる
void TeleportState::setup(Player* player) {
  setupTeleportCursor(player);

  currentAcc_ = g_local->FrameAcc();
  g_local->SetFrameAcc(player->getReduce());
}

void TeleportState::handleInput(Player* player, StateManager* stateMgr, ofxJoystick& input) {
  if (input.isRelease(Input::X)) {
    // カーソルがBrickと重なっていなければテレポート
    if (!cursor_->onBrick()) {
      player->setPos(cursor_->getPos());
      player->setCanTeleport(false);
    }
    // Brickとの判定に関わらず、ボタンを離したらフレームレートを元に戻して状態を遷移
    g_local->SetFrameAcc(currentAcc_);
    DeleteActors(TELEPORT_CURSOR);
    stateMgr->pop();
  }
}

//! 移動先の指定だけはフレームレート変更の影響を受けないようになっています
void TeleportState::update(float deltaTime, Player* player, ofxJoystick& input) {
  cursor_->setPlayerPos(player->getPos());
  cursor_->setPlayerVel(player->getVel());

  controlPlayerVel(player);
}

void TeleportState::draw(Player* player) {}


/**
* @brief Brickとの当たり判定がないと落下するので、ここにもBrickを貫通しない処理を追加
* @note  PlayerがActorに潰された場合の処理は不明なので後程追加します。
*/
void TeleportState::onCollision(Player* player, Actor* c_actor) {
  // プレイヤーと衝突判定を行うオブジェクトの必要パラメータを取得
  auto p_pos  = player->getPos();
  auto p_vel  = player->getVel();
  auto p_size = player->getSize();
  auto c_pos  = c_actor->getPos();
  auto c_size = c_actor->getSize();

  // Brick以外の物と判定しないように制限
  if (c_actor->getTag() == BRICK) {
    // Playerの上辺がBrickの底辺とCollisionした場合
    if (p_pos.y + p_vel.y < c_pos.y &&
        p_pos.y + p_size.y + p_vel.y > c_pos.y &&
        p_pos.x < c_pos.x + c_size.x &&
        p_pos.x + p_size.x > c_pos.x &&
        p_vel.y >= 0) {
      // 挟まれた状態
      if (player->onFloor()) {
        player->isDead(true); // 死亡判定をtrueに
        player->setVel(ofVec2f(p_vel.x, 0.0f));
        player->setPos(ofVec2f(p_pos.x, c_pos.y + c_size.y));
      }
      // 空中でぶつかった場合
      else {
        player->setVel(ofVec2f(p_vel.x, 0.0f));
        player->setPos(ofVec2f(p_pos.x, c_pos.y - p_size.y));
      }
    }

    // Brickに上からぶつかったら加速度を０に(左右への移動量はそのまま)
    // Brickの上にPlayerの位置を修正
    else if (p_pos.y + p_vel.y < c_pos.y + c_size.y &&
             (p_pos.y + p_size.y / 3) - p_vel.y > c_pos.y + c_size.y &&
             p_pos.x + (p_size.x / 10) <= c_pos.x + c_size.x &&
             p_pos.x + p_size.x - (p_size.x / 10) >= c_pos.x &&
             p_vel.y < 0) {
      player->onFloor(true);
      player->setVel(ofVec2f(0.0f, 0.0f));
      player->setPos(ofVec2f(p_pos.x, c_pos.y + c_size.y));
    }

    // Playerの左辺がBrickの右辺とCollisionした場合
    else if (p_pos.x < c_pos.x + c_size.x &&
             p_pos.x + p_size.x > c_pos.x + c_size.x &&
             p_pos.y - p_vel.y * 2 < c_pos.y + c_size.y &&
             p_pos.y + p_size.y > c_pos.y) {
      player->setVel(ofVec2f(0.0f, p_vel.y));
      // 画面外に押し出されないように調整
      if (c_pos.x + c_size.x >= ofGetWidth()) {
        player->setPos(ofVec2f(p_pos.x, c_pos.y + c_size.y));
      }
      else {
        player->setPos(ofVec2f(c_pos.x + c_size.x, p_pos.y));
      }
    }

    // Playerの右辺がBrickの左辺とCollisionした場合
    else if (p_pos.x + p_size.x > c_pos.x &&
             p_pos.x < c_pos.x &&
             p_pos.y - p_vel.y * 2 < c_pos.y + c_size.y &&
             p_pos.y + p_size.y > c_pos.y) {
      player->setVel(ofVec2f(0.0f, p_vel.y));
      // 画面外に押し出されないように調整
      if (c_pos.x - p_size.x < 0) {
        player->setPos(ofVec2f(p_pos.x, c_pos.y + c_size.y));
      }
      else {
        player->setPos(ofVec2f(c_pos.x - p_size.x, p_pos.y));
      }
    }
  }
}

// update()中のPlayer::vel_の制御
void TeleportState::controlPlayerVel(Player* player) {
  // スキル使用中の落下速度の限界値を一定の値にしてみる（試しに重力加速度の３倍に調整）
  if (player->getVel().y < -(player->getGravity() * 3)) { player->setVel(ofVec2f(player->getVel().x, -(player->getGravity() * 3))); }

  // スキル使用中のPlayerが画面外に移動出来ないよう制限
  if (player->getPos().x <= 0) {
    player->setVel(ofVec2f(0.0f, player->getVel().y));
  }
  else if (player->getPos().x + player->getSize().x >= ofGetWindowWidth()) {
    player->setVel(ofVec2f(0.0f, player->getVel().y));
  }
}

void TeleportState::setupTeleportCursor(Player* player) {
  cursor_ = make_shared<TeleportCursor>();
  cursor_->setPos(ofVec2f(player->getPos().x, player->getPos().y + player->getSize().y));
  cursor_->setSize(player->getSize());
  cursor_->setRound(player->getRound());
  AddActor(cursor_);
}
