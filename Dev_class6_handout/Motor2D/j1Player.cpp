#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Player.h"
#include "j1Textures.h"
#include "SDL/include/SDL.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"

j1Player::j1Player()
{
	
}
j1Player::~j1Player()
{
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");


	
	position.x = 150;
	position.y = 120;

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");


	return true;
}

// Update: draw background
bool j1Player::Update()
{
	position.x += 1; // Automatic movement

	int speed = 1;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		
	}

	
/*
	if (App->input->GetKey[SDL_SCANCODE_S] == KEY_IDLE
		&& App->input->GetKey[SDL_SCANCODE_W] == KEY_IDLE)
		*/


	// Draw everything --------------------------------------
	
	// Draw UI (score) --------------------------------------

	// TODO 3: Blit the text of the score in at the bottom of the screen

	return true;
}

//void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
//{
//	if (c1 == col && destroyed == false && App->fade->IsFading() == false)
//	{

//	}
