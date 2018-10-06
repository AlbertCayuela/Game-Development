#ifndef _j1Player_H_
#define _j1Player_H_

#include "j1Module.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Start();
	bool Update();
	bool CleanUp();
	/*void OnCollision(Collider* c1, Collider* c2);*/

public:

	SDL_Texture * graphics = nullptr;	
	iPoint position;
	
};

#endif
