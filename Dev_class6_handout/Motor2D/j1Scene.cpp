#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Collisions.h"
#include "j1FadeToBlack.h"

#define CAMERA_LEFT_MARGIN 100

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	

	App->map->Load("maps/dirt_map.tmx");
	currentmap = 1;
	
	
	//App->map->Load("iso.tmx");
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		App->SaveGame();

	/*if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {

		if (currentmap == App->player->saved_map)
			App->LoadGame();
		else
		{
			switch (App->player->saved_map) {
			case 1:
				curr_map = MAP_1;
				App->collision->ColliderCleanUp();
				App->fadetoblack->FadeToBlack(this, this, 3.0f);
				App->map->CleanUp();
				App->map->Load("maps/dirt_map.tmx");
				App->player->player_pos.x = App->player->last_saved_pos.x;
				App->player->player_pos.y = App->player->last_saved_pos.y;
				is_fade = true;
				break;
			case 2:
				curr_map = MAP_1;
				App->fadetoblack->FadeToBlack(this, this, 3.0f);
				App->map->CleanUp();
				App->collision->CleanUp();
				App->map->Load("Cave_map.tmx");
				App->player->player_pos.x = App->player->last_saved_pos.x;
				App->player->player_pos.y = App->player->last_saved_pos.y;
				is_fade = true;
				break;
			default:
				break;
			}
		}

	}*/
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (currentmap == 1) {
			App->collision->ColliderCleanUp();
			App->fadetoblack->FadeToBlack(this, this, 3.0f);
			App->map->CleanUp();
			App->map->Load("maps/dirt_map.tmx");
			pugi::xml_node colnode = map_file.child("map").child("objectgroup");
			App->map->loadcollision(colnode);
			currentmap = 2;
			
			is_fade = true;
		}
		else if (currentmap == 2) {
			App->fadetoblack->FadeToBlack(this, this, 3.0f);
			App->map->CleanUp();
			App->collision->ColliderCleanUp();
			App->map->Load("maps/Cave_map.tmx");
			pugi::xml_node colnode = map_file.child("map").child("objectgroup");
			App->map->loadcollision(colnode);
			currentmap = 1;

			is_fade = true;
	}

	}
	//if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		//App->render->camera.y += 1;

	//if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		//App->render->camera.y -= 1;

	//if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		//App->render->camera.x += 1;

	//if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		//App->render->camera.x -= 1;
	if (App->render->camera.x + CAMERA_LEFT_MARGIN > App->player->position.x)
	{
		App->render->camera.x = 0;
	}
	else
	{
		App->render->camera.x = ((-App->player->position.x) + CAMERA_LEFT_MARGIN)*App->win->GetScale();
	}
	App->render->camera.y = (-App->player->position.y)-200;

	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count(),
					map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
