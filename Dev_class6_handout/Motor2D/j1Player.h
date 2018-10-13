#ifndef _j1Player_H_
#define _j1Player_H_

#include "Animation.h"
#include "j1Module.h"
#include "p2Point.h"
#include "j1Collisions.h"

struct SDL_Texture;
struct Collider;




class j1Player : public j1Module
{
	enum class STATE 
	{
		IDLE=1,
		WALKING_RIGHT,
		WALKING_LEFT,
		JUMPING,
		JUMPING_RIGHT,
		JUMPING_LEFT,
		FALLING,
		FALLING_RIGHT,
		FALLING_LEFT,
		NONE
	};

	enum class DIRECTION
	{

		IDLE,
		TO_RIGHT,
		TO_LEFT,
		TO_UP,
		TO_DOWN

	};

public:
	j1Player();
	~j1Player();

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void CalculatePosition();
	void SetPlayerActions();
	void CalculateTime();

public:

	SDL_Texture * graphics = nullptr;	
	//SDL_Rect player_rect;
	float sec;
	uint current_sec;
	uint prev_sec=0;
	fPoint speed;
	fPoint acc;
	fPoint position;
	fPoint next_pos;
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
	bool is_on_floor = false;
	bool is_in_air = false;
	
	float gravity = 30;

	STATE state = STATE::FALLING;
	DIRECTION direction = DIRECTION::IDLE;
};

#endif
