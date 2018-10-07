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

	//animations

	//idle
	idle.PushBack({17,69,35,38});
	idle.PushBack({78,70,34,37});
	idle.PushBack({139,69,34,38});
	idle.PushBack({198,69,34,38});
	idle.speed = 0.05f;
	idle.loop = true;

	//walking to the right
	walking_right.PushBack({14,236,35,37});
	walking_right.PushBack({78,236,32,37});
	walking_right.PushBack({138,236,31,37});
	walking_right.PushBack({198,235,33,38});
	walking_right.speed = 0.05f;
	walking_right.loop = true;

	//walking to the left
	walking_left.PushBack({585,229,33,36});
	walking_left.PushBack({523,228,33,38});
	walking_left.PushBack({464,229,31,37});
	walking_left.PushBack({402,228,33,38});
	walking_left.speed = 0.05f;
	walking_left.loop = true;

	//jump to the right
	jump_right.PushBack({17,126,36,38});
	jump_right.loop = true;

	//jump to the left
	jump_left.PushBack({405,121,35,38});

	//attack to the right
	attack_right.PushBack({ 17,179,35,37 });
	attack_right.PushBack({ 77,179,35,36 });
	attack_right.PushBack({ 138,179,46,37 });
	attack_right.PushBack({ 198,179,38,37 });
	attack_right.speed = 0.05f;
	attack_right.loop = false;

	//attack to the left
	attack_left.PushBack({583,173,35,37});
	attack_left.PushBack({523,173,35,37});
	attack_left.PushBack({451,173,46,37});
	attack_left.PushBack({ 399,173,38,37 });
	attack_left.speed = 0.05f;
	attack_left.loop = false;

	//attack while jumping right
	jump_attack_right.PushBack({16,10,39,39});
	jump_attack_right.PushBack({75,10,37,39});
	jump_attack_right.PushBack({134,10,46,39});
	jump_attack_right.PushBack({195,10,38,39});
	jump_attack_right.speed = 0.1f;
	jump_attack_right.loop = true;

	//attack while jumping left
	jump_attack_left.PushBack({ 585,11,35,37 });
	jump_attack_left.PushBack({525,11,35,38});
	jump_attack_left.PushBack({457,11,46,38});
	jump_attack_left.PushBack({404,11,38,38});
	jump_attack_left.speed = 0.05f;
	jump_attack_left.loop = false;

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

	int speed = 1;

	current_animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) 
	{

		position.x +=1;
		current_animation = &walking_right;

	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{

		position.x -= 1;
		current_animation = &walking_left;

	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) 
	{
		current_animation = &attack_right;
	}
	
	
	

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

	//current_animation = &walking_right;
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
