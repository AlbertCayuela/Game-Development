#include "j1App.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Module.h"
#include "p2Log.h"
#include "j1FadeToBlack.h"

j1Collisions::j1Collisions()
{
	//for (uint i = 0; i < MAX_COLLIDERS; ++i)
		//colliders[i] = nullptr;
	name.create("collisions");

	matrix[COLLIDER_DEATH][COLLIDER_DEATH] = false;
	matrix[COLLIDER_DEATH][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_DEATH][COLLIDER_MAX] = false;
	matrix[COLLIDER_DEATH][COLLIDER_PLAYER] = true;

	matrix[COLLIDER_FLOOR][COLLIDER_DEATH] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_MAX] = false;
	matrix[COLLIDER_FLOOR][COLLIDER_PLAYER] = true;

	matrix[COLLIDER_MAX][COLLIDER_DEATH] = false;
	matrix[COLLIDER_MAX][COLLIDER_FLOOR] = false;
	matrix[COLLIDER_MAX][COLLIDER_MAX] = false;
	matrix[COLLIDER_MAX][COLLIDER_PLAYER] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_DEATH] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_FLOOR] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_MAX] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
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

	//// Calculate collisions
	//Collider* c1;
	//Collider* c2;

	//for (uint i = 0; i < MAX_COLLIDERS; ++i)
	//{
	//	// skip empty colliders
	//	if (colliders[i] == nullptr)
	//		continue;

	//	c1 = colliders[i];

	//	// avoid checking collisions already checked
	//	//for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
	//	//{
	//	//	// skip empty colliders
	//	//	if (colliders[k] == nullptr)
	//	//		continue;

	//	//	c2 = colliders[k];

	//	//	if (c1->CheckCollision(c2->rect) == true)
	//	//	{
	//	//		if (matrix[c1->type][c2->type] && c1->callback)
	//	//			c1->callback->OnCollision(c1, c2);
	//	//		if (matrix[c2->type][c1->type] && c2->callback)
	//	//			c2->callback->OnCollision(c2, c1);
	//	//	}
	///*	}
	//}

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
	/*if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		if (debug == false) debug = true;
		else debug = false;
		LOG("Showing Colliders ");
	}

	if (debug == false)
		return;*/

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

	bool ret = true;

	if (r.x + r.w < rect.x) ret = false;
	else if (r.x > rect.x + rect.w) ret = false;
	else if (r.y + r.h < rect.y)ret = false;
	else if (r.y > rect.h + rect.y)ret = false;

	return ret;
}