#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>
#include "j1Collisions.h"

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}
TileSet::~TileSet() {
	App->tex->UnLoad(texture);
}
// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

//blit
void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	p2List_item<TileSet*>* itemtileset;
	itemtileset = data.tilesets.end;//get last tileset
	p2List_item<MapLayer*>* itemlayer;
	itemlayer = data.maplayers.start;

	while (itemtileset != NULL) {

		itemlayer = data.maplayers.start;

		while (itemlayer != NULL) {

			for (uint x = 0; x < itemlayer->data->width; x++) {

				for (uint y = 0; y < itemlayer->data->height; y++) {

					SDL_Rect rect = itemtileset->data->GetTileRect(itemlayer->data->Get(x, y));
					iPoint coordd = MapToWorld(x, y);

					if (itemlayer->data->type == LAYER_MAINGROUND) {
						App->render->Blit(itemtileset->data->texture, coordd.x, coordd.y, &rect, 1.0f);
					}
					else if (itemlayer->data->type == LAYER_BACKGROUND) {
						App->render->Blit(itemtileset->data->texture, coordd.x, coordd.y, &rect, 1.0f);
					}
					else if (itemlayer->data->type == LAYER_MAINGROUND2) {
						App->render->Blit(itemtileset->data->texture, coordd.x, coordd.y, &rect, 1.0f);
					}
					else if (itemlayer->data->type == LAYER_BACKGROUND2) {
						App->render->Blit(itemtileset->data->texture, coordd.x, coordd.y, &rect, itemlayer->data->speed);
					}
					else if (itemlayer->data->type == LAYER_BACKGROUNDMOUNT ) {
						App->render->Blit(itemtileset->data->texture, coordd.x, coordd.y, &rect, itemlayer->data->speed);
					}
					
				}

			}
			//next layer
			itemlayer = itemlayer->next;
		}
		//previous tileset(start from the end)
		itemtileset = itemtileset->prev;
	}

}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret(0,0);
	
	if (data.type == MAPTYPE_ORTHOGONAL) 
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;

		return ret;
	}
	
	else if (data.type == MAPTYPE_ISOMETRIC) 
	{


		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
		

		return ret;
	}
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;

		return ret;
	}
	
	else if (data.type == MAPTYPE_ISOMETRIC)
	{


		ret.x = (x - y) / (data.tile_width * 0.5f);
		ret.y = (x + (y/2)) / (data.tile_height * 0.5f);
		

		return ret;
	}
	
	
	
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

//Load and Add colliders
void j1Map::loadcollision(pugi::xml_node &node) {

	for (pugi::xml_node col = node.child("object"); col; col = col.next_sibling("object")) {
		SDL_Rect collision;
		p2SString checkcol;
		checkcol = col.attribute("name").as_string();
		collision.x = col.attribute("x").as_uint();
		collision.y = col.attribute("y").as_uint();
		collision.h = col.attribute("height").as_uint();
		collision.w = col.attribute("width").as_uint();

		if (checkcol == "floor")
			App->collision->AddCollider(collision, COLLIDER_FLOOR);

		if (checkcol == "death")
			App->collision->AddCollider(collision, COLLIDER_DEATH);

		if (checkcol == "player")
			App->collision->AddCollider(collision, COLLIDER_PLAYER);

		if (checkcol == "end")
			App->collision->AddCollider(collision, COLLIDER_END);

		if (checkcol == "win")
			App->collision->AddCollider(collision, COLLIDER_WIN);

		if (checkcol == "wallr")
			App->collision->AddCollider(collision, COLLIDER_WALLRIGHT);

		if (checkcol == "walll")
			App->collision->AddCollider(collision, COLLIDER_WALLLEFT);
	}
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	App->collision->ColliderCleanUp();//clean collider on cleanup map
	// Remove all layers

	p2List_item<MapLayer*>* item_layer;
	item_layer = data.maplayers.start;

	while (item_layer != NULL)
	{
		RELEASE(item_layer->data);
		item_layer = item_layer->next;
	}
	data.maplayers.clear();


	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(file_name);

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.maplayers.add(lay);
	}
	//load collision 
	pugi::xml_node colnode=map_file.child("map").child("objectgroup");


	loadcollision(colnode);



	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.maplayers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

//load layers
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{

	const int size = layer->height*layer->width;

	pugi::xml_node layer_data = node.child("data");

	const char* aux = node.child("properties").child("property").attribute("name").as_string();

	if (strcmp(aux, "speed") == 0)
		layer->speed = node.child("properties").child("property").attribute("value").as_float();

	layer->name = node.attribute("name").as_string();

	if (layer->name == "MAINGROUND")
		layer->type = LAYER_MAINGROUND;
	else if (layer->name == "fondo")
		layer->type = LAYER_BACKGROUND;
	else if (layer->name == "MAINGROUNDDIRT")
		layer->type = LAYER_MAINGROUND2;
	else if (layer->name == "BACKGROUNDMOUNT")
		layer->type = LAYER_BACKGROUNDMOUNT;
	else if (layer->name == "BACKGROUNDIRT")
		layer->type = LAYER_BACKGROUND2;



	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();


	layer->data = new uint[layer->height * layer->width];
	memset(layer->data, 0, sizeof(uint)*layer->height * layer->width);

	uint i = 0;
	for (pugi::xml_node tile_gid = node.child("data").child("tile"); tile_gid; tile_gid = tile_gid.next_sibling("tile")) {
		layer->data[i++] = tile_gid.attribute("gid").as_uint();
	}
	return true;
}

inline uint MapLayer::Get(int x, int y) const {

	return data[width * y + x];
}





