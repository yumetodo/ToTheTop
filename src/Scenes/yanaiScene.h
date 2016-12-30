﻿
/**
 * @file   yanaiScene.h
 * @brief  テストシーン
 *
 * @author y.akira
 * @date   2016.12.21
 */

#pragma once


//! @brief テスト用シーンクラス
class YanaiScene : public ofxScene {
private:
  yCamera     cam_;
  BackGround  bg_;
  
  ofVec2f   originPos_;
  ofVec2f   destPos_;
  ofVec2f   brickSize_;
  float     time_;
  float     round_;
  int       curve_;
  
public:
  void setup() override;
  void update(float deltaTime) override;
  void draw() override;
  
  void keyPressed(int key) override;
};
