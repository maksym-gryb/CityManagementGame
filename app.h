#include "inc.h"

//#include "TestScene.h"
#include "mainscene.h"

class App{
	/* Singleton */
private:
	static App* instance;
	App();
	
public:
	static App* getInstance();
	~App();
	
	/* Status */
	bool isRunning();
	
	/* App */
	int init(Scene* p_scene);
	int run(Scene* p_scene);
	
	/* Loop */
	void update();
	void print();
	void handleInput();
	
	/* Exit */
	void quit();
	
private:
	/* Variables */
	bool m_running;
	
	Scene* m_current_scene;
};
