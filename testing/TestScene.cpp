#include "TestScene.h"

TestScene::TestScene() : Scene("TEST")
{
	m_update = NULL;
	m_user = NULL;
	m_console = NULL;
	
	m_update_timer = 2000;
	m_timer_next   = 0;
	
	m_running = true;
}

TestScene::~TestScene()
{
	delete m_update;
	delete m_user;
	delete m_console;
}

void TestScene::setup()
{
	struct {
		int w;
		int h;
	} m_window = {WindowInfo::getInstance()->getX(), WindowInfo::getInstance()->getY()};
	
	int w = m_window.w/2;
	m_update = new Tile(0, 0, w, m_window.h - 2);
	/* TEST */
	/*
	m_update->push("Could it be that the legendary jabawocky has finally risen to the challenge of building a device so devisive that even the devices of Good and Evil cannot eat the lemons produced from the apothecarian's brimming startlight?");
	*/
	/* END TEST */
	
	w = m_window.w - w - 1;
	m_user = new Tile(w, 0, w, m_window.h - 2);
	/* TEST */
	/*
	m_update->push("Could it be that the legendary jabawocky has finally risen to the challenge of building a device so devisive that even the devices of Good and Evil cannot eat the lemons produced from the apothecarian's brimming startlight?");
	*/
	/* END TEST */
	
	m_console = new Tile(0, m_window.h - 1, m_window.w, 1, 1);
	m_console->push("_");
}

int TestScene::run()
{
	long now = time(NULL)*1000;
	
	if(now > m_timer_next)
	{
		m_timer_next = now + m_update_timer;
		m_update->push("Moo moo");
	}
	
	print();
	
	handleInput();
	
	return m_running;
}

void TestScene::print()
{
	clear();
	
	m_update->print();
	
	m_user->print();
	
	m_console->print();
	
	refresh();
}

void TestScene::handleInput()
{
	char ch;
	if( (ch = getch()) == ERR )
		return;
	
	std::string current_input = m_console->pop();
	if(current_input == "")
		current_input = "_";
	
	// process input
	if(ch == '\r')// press enter
	{
		current_input.erase(current_input.end() - 1, current_input.end());// remove the trailing '_'
		
		m_user->push(current_input);
		current_input = "_";
	}
	else if(ch >= 32 && ch <= 126)// pretty much any character
	{
		current_input.erase(current_input.end() - 1, current_input.end());
		current_input += ch;
		current_input += "_";
	}
	else if(ch == 127 && current_input.size() > 1)// delete character (backspace)
	{
		current_input.erase(current_input.end() - 2, current_input.end() - 1);
	}
	else if(ch == 27)// quit (escape)
	{
		m_running = false;
	}
	
	m_console->push(current_input);
}
