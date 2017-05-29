#include "app.h"

App* App::instance = NULL;

App* App::getInstance()
{
	if(instance == NULL)
		instance = new App;
	
	return instance;
}

App::App()
{
	m_running = true;
}

App::~App()
{
	//
}

bool App::isRunning()
{
	return m_running;
}

int App::init(Scene* p_scene)
{
	// Setup ncurses
	setlocale(LC_ALL, "");
	
	initscr();
	noecho();
	cbreak();
	nodelay(stdscr, true);
	curs_set(FALSE);
	
	nonl();
	
	start_color();
	//init_pair(1, COLOR_RED, COLOR_BLACK);
	
	//int w, h;
	//getmaxyx(stdscr, h, w);
	
	// Setup Scene
	m_current_scene = p_scene;
	//m_current_scene->setWindow(w, h);
	m_current_scene->setup();
	
	return 0;
}

int App::run(Scene* p_scene)
{
	if(init(p_scene) != 0)
		return -1;
	
	while(isRunning())
	{
		update();
		
		handleInput();
	}
	
	quit();
	
	return 0;
}

void App::update()
{
	m_running = m_current_scene->run();
}

void App::handleInput()
{
	//
}

void App::quit()
{
	endwin();
}
