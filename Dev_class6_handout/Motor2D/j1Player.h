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
	

public:
	j1Player();
	~j1Player();


	bool Awake(pugi::xml_node&);

	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	
	bool Load(pugi::xml_node&);
	// Save
	bool Save(pugi::xml_node&) const;

public:

	SDL_Texture * graphics = nullptr;	
	fPoint speed;
	
	
	fPoint position;
	fPoint prev_pos;
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
	bool jumping = false;
	bool falling = false;

	float jumping_time = 0;
	float gravity = 30;

public:

	
	fPoint last_saved_pos = { 0.0f,0.0f };
	fPoint player_pos = { 50.0f,600.0f };
	int saved_map = 1;
	iPoint collider_offset = { 0,0 };
	bool player_death = false;
	bool player_win = false;
	bool change_map = false;
	bool godmode = false;
	
private:

	pugi::xml_document	map_file;

};

#endif
