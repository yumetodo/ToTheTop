
#pragma once


class Brick : public Actor {
private:
	//落下地点
	ofVec2f fallPoint_;
public:
	Brick();
	void setup() override;
	void update(float deltaTime) override;
	void draw() override;
	void onCollision(Actor* c_actor) override;

	void fallSetup();

	//落下地点を指定する
	void moveTo(ofVec2f& pos);
};
