#include "scene.h"

#include "tile.h"
#include "windowinfo.h"

class TestScene : public Scene{
public:
	/* Constructors */
	TestScene();
	~TestScene();
	
	/* TestScene */
	void setup();
	int run();
	
	void print();
	void handleInput();
	
private:
	/* Variables */
	Tile* m_update;
	Tile* m_user;
	Tile* m_console;
	
	int m_update_timer;
	long m_timer_next;
	
	bool m_running;
};
