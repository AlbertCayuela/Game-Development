#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Map.h"

j1Collision::j1Collision()
{

}

j1Collision::~j1Collision()
{

}


bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion


	return true;
}