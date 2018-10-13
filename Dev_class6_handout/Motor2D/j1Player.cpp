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


	prev_sec = current_sec = SDL_GetTicks();

	//destroyed=false;
	position.x = 50.f;
	position.y = 600.f;

	acc.x = 0;
	acc.y = gravity;
	
	speed.x = 0;
	speed.y = 0;

	player_col = App->collision->AddCollider({(int)position.x,(int)position.y,30,35 }, COLLIDER_PLAYER, this);

	return true;
}

// Unload assets
bool j1Player::Update(float dt)
{
	bool right_down = (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT);
	bool left_down= (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT);
	bool space_down= (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT);
	bool right_up= (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP);
	bool left_up = (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP);
	bool falling = (speed.y >= 0);

	switch (state)
	{
	case STATE::IDLE:
		if (right_down && !left_down) state = STATE::WALKING_RIGHT;
		if (left_down && !right_down)state = STATE::WALKING_LEFT;
		if (space_down)state = STATE::JUMPING;
		break;
	case STATE::WALKING_RIGHT:
		if (right_up || left_down)state = STATE::IDLE;
		if (space_down)state = STATE::JUMPING;
		break;
	case STATE::WALKING_LEFT:
		if (left_up || right_down)state = STATE::IDLE;
	
		if (space_down)state = STATE::JUMPING;
		break;

	case STATE::JUMPING:
		if (right_down && !left_down)state = STATE::JUMPING_RIGHT;
		if (left_down && !right_down)state = STATE::JUMPING_LEFT;
		if (falling)state = STATE::FALLING;
		break;
	case STATE::JUMPING_RIGHT:
		if (right_up || left_down)state = STATE::JUMPING;
		if (falling)state = STATE::FALLING;
		break;
	case STATE::JUMPING_LEFT:
		if (left_up || right_down)state = STATE::JUMPING;
		if (falling)state = STATE::FALLING;
		break;
	case STATE::FALLING:
		if (right_down && !left_down)state = STATE::FALLING_RIGHT;
		if (left_down && !right_down)state = STATE::FALLING_LEFT;
		if (is_on_floor)state = STATE::IDLE;
	case STATE::FALLING_RIGHT:
		if (right_up || left_down)state = STATE::FALLING;
		if (is_on_floor)state = STATE::IDLE;
		break;
	case STATE::FALLING_LEFT:
		if (left_up || right_down)state = STATE::FALLING;
		if (is_on_floor)state = STATE::IDLE;
		break;

	}

	SetPlayerActions();
	CalculateTime();
	CalculatePosition();

	
	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));;
	

	player_col->SetPos(position.x, position.y);

	return true;
}

bool j1Player::PostUpdate()
{
	
	player_col->SetPos(position.x, position.y);
	
	
	//App->render->DrawQuad(player_col->rect,255,0,0);
	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);
	App->collision->EraseCollider(player_col);

	return true;
}
// Update: draw background

void j1Player::CalculatePosition()
{

	speed.x = speed.x + acc.x * sec;
	speed.y = speed.y + acc.y*sec;
	position.x = position.x + speed.x * sec + acc.x * sec*sec*0.5f;
	position.y = position.y + speed.y * sec + acc.y * sec*sec*0.5f;
	next_pos.x = position.x + speed.x * (sec + 0.1);
	next_pos.y = position.y + speed.y*(sec + 0.1);

	if (speed.x > 0)direction = DIRECTION::TO_RIGHT;
	if (speed.x < 0)direction = DIRECTION::TO_LEFT;
	if (speed.y > 0)direction = DIRECTION::TO_DOWN;
	if (speed.y < 0)direction = DIRECTION::TO_UP;
}

void j1Player::CalculateTime()
{
	current_sec = SDL_GetTicks();
	sec = current_sec - prev_sec;
	sec = sec / 1000;
	prev_sec = current_sec;
}

void j1Player::SetPlayerActions() 
{

	switch (state)
	{
	case STATE::IDLE:
		speed.x = 0;
		current_animation = &idle;
		is_in_air = false;
		break;
	case STATE::WALKING_RIGHT:
		speed.x = 40;
		current_animation = &walking_right;
		break;
	case STATE::WALKING_LEFT:
		speed.x = -40;
		current_animation = &walking_left;
		break;
	case STATE::JUMPING:
		speed.x = 2;
		current_animation = &jump_right;
		if (is_in_air == false)
		{
			speed.y = -40;
			acc.y = gravity;
			is_in_air = true;
			is_on_floor = false;
		}
		break;
	case STATE::JUMPING_RIGHT:
		speed.x = 20;
		break;
	case STATE::JUMPING_LEFT:
		speed.x = -20;
		break;
	case STATE::FALLING:
		current_animation = &jump_right;
		speed.x = 0;
		break;
	case STATE::FALLING_RIGHT:
		speed.x = 20;
	case STATE::FALLING_LEFT:
		current_animation = &jump_left;
		break;
	}

}



void j1Player::OnCollision(Collider* c1, Collider* c2)
{	
	
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_FLOOR) 
	{
		if (speed.y >= 0)
		{
			position.y = c2->rect.y - c1->rect.h;
			acc.y = 0;
			is_on_floor = true;
		}
	}
	
}
