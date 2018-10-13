#ifndef _j1Player_H_
#define _j1Player_H_

#include "Animation.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Collisions.h"

struct SDL_Texture;
//struct Collider;

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

public:

	SDL_Texture * graphics = nullptr;	
	SDL_Rect player_rect;
	iPoint position;
	iPoint prev_pos;
	iPoint speed;
	Animation* current_animation;
	Animation idle;
	Animation jump_attack_right;
	Animation jump_attack_left;
	Animation walking_right;
	Animation walking_left;
	Animation attack_right;
	Animation jump_right;
	Animation jump_left;
	Animation attack_left;
	Collider* player_col=nullptr;
	bool destroyed = false;

	fPoint acc;
	float gravity = 250;
};

#endif
