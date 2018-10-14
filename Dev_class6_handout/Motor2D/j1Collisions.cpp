#include "j1App.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Module.h"
#include "p2Log.h"
#include "j1FadeToBlack.h"
#include "j1Player.h"

j1Collisions::j1Collisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;


	matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_DEATH] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_FLOOR][COLLIDER_END] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_WIN] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_WALLRIGHT] = false;

	matrix[COLLIDER_DEATH][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_DEATH][COLLIDER_END] = false;
	matrix[COLLIDER_DEATH][COLLIDER_WIN] = false;
	matrix[COLLIDER_DEATH][COLLIDER_WALLRIGHT] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_END] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WIN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_WALLRIGHT] = false;

	matrix[COLLIDER_END][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_END][COLLIDER_DEATH] = false;
	matrix[COLLIDER_END][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_END][COLLIDER_END] = false;
	matrix[COLLIDER_END][COLLIDER_WIN] = false;
	matrix[COLLIDER_END][COLLIDER_WALLRIGHT] = false;

	matrix[COLLIDER_WIN][COLLIDER_WIN] = false;
	matrix[COLLIDER_WIN][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_WIN][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WIN][COLLIDER_END] = false;
	matrix[COLLIDER_WIN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WIN][COLLIDER_WALLRIGHT] = false;

	matrix[COLLIDER_WALLRIGHT][COLLIDER_WALLRIGHT] = false;
	matrix[COLLIDER_WALLRIGHT][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_WALLRIGHT][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WALLRIGHT][COLLIDER_END] = false;
	matrix[COLLIDER_WALLRIGHT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALLRIGHT][COLLIDER_WALLLEFT] = true;



	matrix[COLLIDER_WALLLEFT][COLLIDER_WIN] = false;
	matrix[COLLIDER_WALLLEFT][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_WALLLEFT][COLLIDER_DEATH] = false;
	matrix[COLLIDER_WALLLEFT][COLLIDER_END] = false;
	matrix[COLLIDER_WALLLEFT][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALLLEFT][COLLIDER_WALLLEFT] = false;
	matrix[COLLIDER_WALLLEFT][COLLIDER_WALLRIGHT] = false;


}

// Destructor
j1Collisions::~j1Collisions()
{}

bool j1Collisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	// Calculate collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}
	
	

	return true;
}

// Called before render is available
bool j1Collisions::Update(float dt)
{
	DebugDraw();
	             

	

	return true;

	
}

void j1Collisions::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		if (debug== false) debug = true;
		else debug = false;
		LOG("Showing Colliders ");
	}

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_FLOOR: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_DEATH: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_PLAYER: //red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_END: //yellow
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_WIN://purple
			App->render->DrawQuad(colliders[i]->rect, 182, 102, 210, alpha);
			break;
		case COLLIDER_WALLRIGHT://orange
			App->render->DrawQuad(colliders[i]->rect, 255, 117, 020, alpha);
			break;
		case COLLIDER_WALLLEFT://black
			App->render->DrawQuad(colliders[i]->rect, 010, 010, 010, alpha);
			break;

		}
	}
}

// Called before quitting
bool j1Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}
bool j1Collisions::ColliderCleanUp()
{

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->type != COLLIDER_PLAYER)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool j1Collisions::EraseCollider(Collider * collider)
{
	if (collider != nullptr) 
	{
		for (uint i = 0; i < MAX_COLLIDERS; i++) 
		{
			collider->to_delete = true;
			break;

		}
	}

	return false;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	// TODO 0: Return true if there is an overlap
	// between argument "r" and property "rect"

	//bool ret = true;

	//if (r.x + r.w < rect.x) ret = false;
	//else if (r.x > rect.x + rect.w) ret = false;
	//else if (r.y + r.h < rect.y)ret = false;
	//else if (r.y > rect.h + rect.y)ret = false;

	return !(r.x + r.w < rect.x || r.x > rect.x + rect.w || r.y + r.h < rect.y || r.y > rect.h + rect.y);
}