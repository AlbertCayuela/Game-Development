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
	name.create("player");

	//idle
	idle.PushBack({17,69,35,37});
	idle.PushBack({78,69,35,37});
	idle.PushBack({139,69,35,37});
	idle.PushBack({198,69,35,37});
	idle.speed = 0.05f;
	idle.loop = true;

	//attack while jumping
	jump_attack.PushBack({16,10,39,39});
	jump_attack.PushBack({75,10,37,39});
	jump_attack.PushBack({134,10,46,39});
	jump_attack.PushBack({195,10,38,39});
	jump_attack.speed = 0.1f;
	jump_attack.loop = true;
}
j1Player::~j1Player()
{
}

// Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	graphics = App->tex->Load("maps/spritesheet.png");

	

	//destroyed=false;
	position.x = 0;
	position.y = 839;

	return true;
}

// Unload assets
bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);

	return true;
}

// Update: draw background
bool j1Player::Update(float dt)
{
	//position.y += 1; // Automatic movement

	//int speed = 1;

	//if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//{
	//	
	//}

	//if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//{
	//	position.x += speed;
	//}

	//if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//{
	//	
	//}

	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//{
	//	
	//}

	
/*
	if (App->input->GetKey[SDL_SCANCODE_S] == KEY_IDLE
		&& App->input->GetKey[SDL_SCANCODE_W] == KEY_IDLE)
		*/

	current_animation = &idle;
	// Draw everything --------------------------------------
	
	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	// Draw UI (score) --------------------------------------

	// TODO 3: Blit the text of the score in at the bottom of the screen

	return true;
}

//void ModulePlayer::OnCollision(Collider* c1, Collider* c2)
//{
//	if (c1 == col && destroyed == false && App->fade->IsFading() == false)
//	{

//	}
