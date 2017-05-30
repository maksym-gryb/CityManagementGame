#ifndef MAIN_SCENE_H
#define MAIN_SCENE_H

#include "scene.h"

#include "windowinfo.h"
#include "city.h"
#include "tile.h"

class MainScene : public Scene{
public:
	/* Constructors */
	MainScene();
	~MainScene();
	
	/* MainScene */
	void setup();
	int run();
	
	void update();
	void print();
	void handleInput();
	void command(std::string cmdstr);
	int call(std::string cmd, std::vector<std::string> options = {});
	
	/* Resources */
	Resource * createResource(std::string name, int amount = 0, int max = 100);
	
	/* Utilities */
	std::vector<std::string> split(std::string const & cmd, char delim);
	
private:
	/* Variables */
	Tile* m_update;
	Tile* m_user;
	Tile* m_console;
	
	//int m_update_timer;
	double m_day_length;
	high_resolution_clock::time_point m_timer_begin;
	
	bool m_running;
	
	int m_day;
	
	// fast forward info
	int m_fast_forward_date;
	double m_ff_timer_diff;
	int m_fast_forward_speed;
	
	/* City */
	City* m_city;
	
	std::vector<Resource*> m_resources;
	
	/* Commands */
	/** Console Commands */
	/*
	struct{
		std::function< void( std::vector< std::string >, Console_City * cc ) > fn;
		const char* key[];
	}function_lookup_table[] =
		{
 			{ [](std::vector<std::string> cmd, Console_City * cc ){// Status
				cc->showStatus();
				}, ["status"] },
			
			//{ &help, {"help", "?", NULL} },
			
			{ NULL, {NULL} }
		};
	*/
};

#endif /* MAIN_SCENE_H */
