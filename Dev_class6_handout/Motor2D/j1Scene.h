#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

enum CurrentMap
{
	NO_MAP = 0,
	MAP_1,
	MAP_2,
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	uint currentmap = 0;
	CurrentMap curr_map = NO_MAP;
private:
	bool is_fade = false;
	pugi::xml_document	map_file;
};

#endif // __j1SCENE_H__