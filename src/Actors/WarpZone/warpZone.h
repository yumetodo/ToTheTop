
#pragma once


class WarpZone : public Actor {
private:
	Actor* actor_;
	ofVec2f destPos_;

	ofxAnimatableFloat x_, y_;
public:
	WarpZone();
	virtual void setup() override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;

	virtual void onCollision(Actor* c_actor) override;

	void setDistination(ofVec2f& pos);
};