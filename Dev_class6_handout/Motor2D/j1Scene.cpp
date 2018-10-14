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
	

	App->map->Load("maps/cave_map.tmx");
	App->audio->PlayMusic(App->audio->musicmap1.GetString());
	App->audio->MusicVolume(App->audio->volume);
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


	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		volumechange = true;
		App->audio->VolumeChange(volumechange);
	}

	else if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		volumechange = false;
		App->audio->VolumeChange(volumechange);
	}



	//save
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame("save_game.xml");

	//load
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame("save_game.xml");
	//change to map1
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		//cleanup map colliders and reset player when map 1 is loaded
		if (currentmap == 1) {

			App->map->CleanUp();
			App->player->Disable();
			App->map->Load("maps/cave_map.tmx");
			pugi::xml_node colnode = map_file.child("map").child("objectgroup");
			App->map->loadcollision(colnode);
			App->player->Enable();
			App->player->position.x = 50.f;
			App->player->position.y = 600.f;
			currentmap = 2;
			
			
		}
		else if (currentmap == 2) {
	
			App->map->CleanUp();
			App->collision->ColliderCleanUp();
			App->player->Disable();
			App->map->Load("maps/cave_map.tmx");
			pugi::xml_node colnode = map_file.child("map").child("objectgroup");
			App->map->loadcollision(colnode);
			App->player->Enable();
			App->player->position.x = 50.f;
			App->player->position.y = 600.f;

			currentmap = 1;

			
		}

	}
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN || App->player->player_death==true)
	{
		//cleanup map colliders and reset player when map 1 and 2 are loaded and death condition detected
		App->player->player_death = false;
		if (currentmap == 1) {

			App->map->CleanUp();
			App->player->Disable();
			App->map->Load("maps/cave_map.tmx");
			pugi::xml_node colnode = map_file.child("map").child("objectgroup");
			App->map->loadcollision(colnode);
			App->player->Enable();
			App->player->position.x = 50.f;
			App->player->position.y = 600.f;
			currentmap = 1;

			
		}
		else if (currentmap == 2) {
			
			App->map->CleanUp();
			App->collision->ColliderCleanUp();
			App->player->Disable();
			App->map->Load("maps/dirt_map.tmx");
			pugi::xml_node colnode = map_file.child("map").child("objectgroup");
			App->map->loadcollision(colnode);
			App->player->Enable();
			App->player->position.x = 50.f;
			App->player->position.y = 800.f;

			currentmap = 2;

			
		}

	}
		//changemap
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN || App->player->change_map == true && currentmap == 1 || App->player->player_win == true && currentmap ==2)
		{
			App->player->change_map = false;
			App->player->player_win = false;
			if (currentmap == 1) {

				App->map->CleanUp();
				App->player->Disable();
				App->map->Load("maps/dirt_map.tmx");
				pugi::xml_node colnode = map_file.child("map").child("objectgroup");
				App->map->loadcollision(colnode);
				App->player->Enable();
				App->player->position.x = 50.f;
				App->player->position.y = 800.f;
				currentmap = 2;

				
			}
			else if (currentmap == 2) {
				
				currentmap = 1;
				App->map->CleanUp();
				App->collision->ColliderCleanUp();
				App->player->Disable();
				App->map->Load("maps/Cave_map.tmx");
				pugi::xml_node colnode = map_file.child("map").child("objectgroup");
				App->map->loadcollision(colnode);
				App->player->Enable();
				App->player->position.x = 50.f;
				App->player->position.y = 600.f;

				currentmap = 1;

				
			}

		}
		
	//camera start following player after he is at x+100
		if (App->render->camera.x + CAMERA_LEFT_MARGIN > App->player->position.x)
		{
			App->render->camera.x = 0;
		}
		else
		{
			App->render->camera.x = ((-App->player->position.x) + CAMERA_LEFT_MARGIN)*App->win->GetScale();
		}
		App->render->camera.y = (-App->player->position.y) - 200;

		App->map->Draw();

		int x, y;
		App->input->GetMousePosition(x, y);
		iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
		p2SString title("Slingshot Adventure");
			
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
