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
#include "j1Collisions.h"
#define SPEED 3


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

bool j1Player::Awake(pugi::xml_node& node)
{
	
	position.x = node.child("initial_attributes").attribute("x").as_float();
	position.y = node.child("initial_attributes").attribute("y").as_float();
	

	return true;
}
// Load assets
bool j1Player::Start()
{
	LOG("Loading player");

	graphics = App->tex->Load("maps/spritesheet.png");

	App->audio->LoadFx(App->audio->fxJump.GetString());
	//App->audio->LoadFx(App->audio->fxDeath.GetString());
	
	speed.x = 0;
	speed.y = 0;


	player_col = App->collision->AddCollider({(int)position.x,(int)position.y,20,4 }, COLLIDER_PLAYER, this);

	current_animation = &idle;

	return true;
}

// Unload assets
bool j1Player::Update(float dt)
{

	position.x += speed.x;
	position.y += speed.y+4;
	
	falling = false;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		//speed.x += 3;
		position.x += SPEED;
		current_animation = &walking_right;
		
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		position.x -= SPEED;
		current_animation = &walking_left;
		
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && is_on_floor==true) 
	{
		
		jumping = true;
		is_on_floor = false;
		App->audio->PlayFx(1);
	}
	if (jumping_time == 0.0f) prev_pos.y = -1 * position.y;
	if (jumping) 
	{
		
		jumping_time += 0.1f;
		position.y -= 8;
		if (position.y > prev_pos.y&&jumping_time >= 1.f) 
		{
			position.y += 1;
			jumping_time = 0.0f;
			jumping = false;
			falling = true;
		}
	}

	if(falling)current_animation = &jump_right;
	
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {

			godmode = !godmode;
		}
		if (godmode == true) {
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				//speed.x += 3;
				position.x += SPEED;
				current_animation = &walking_right;

			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			{
				position.x -= SPEED;
				current_animation = &walking_left;

			}
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			{
				//speed.x += 3;
				position.y += SPEED;
				current_animation = &jump_right;

			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			{
				position.y -= SPEED;
				current_animation = &jump_right;

			}

		}

	
	
	
	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));;
	

	player_col->SetPos(position.x+4, position.y + 33);

	return true;
}

bool j1Player::PostUpdate()
{

	player_col->SetPos(position.x+4, position.y + 33);

	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	App->collision->EraseCollider(player_col);
	player_col->SetPos(player_pos.x, player_pos.y);

	return true;
}
// Update: draw background
//save and load
bool j1Player::Load(pugi::xml_node& save)
{
	if (save.child("position") != NULL)
	{
		position.x = save.child("position").attribute("x").as_float();
		position.y = save.child("position").attribute("y").as_float();
	}

	return true;
}

bool j1Player::Save(pugi::xml_node& save) const
{
	if (save.child("position") == NULL)
	{
		save.append_child("position").append_attribute("x") = position.x;
		save.child("position").append_attribute("y") = position.y;
	}
	else {
		save.child("position").attribute("x") = position.x;
		save.child("position").attribute("y") = position.y;
	}

	return true;
}
//
void j1Player::OnCollision(Collider* c1, Collider* c2)
{


	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_FLOOR)
	{
		
			position.y = (c2->rect.y - c1->rect.h)-36;
			speed.y = 0;
			is_on_floor = true;
		
	}

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALLRIGHT) 
	{
		position.x = (c2->rect.x+c2->rect.w);
		speed.x = 0;
	}

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALLLEFT) 
	{
		position.x = (c2->rect.x - c1->rect.w);
		speed.x = 0;
	}
	

	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_END)
	{
			
			
		change_map = true;

		
			
	}
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WIN)
	{


		player_win = true;



	}
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_DEATH) {

		player_death = true;
		//App->audio->PlayFx(2);

	}
}
	

