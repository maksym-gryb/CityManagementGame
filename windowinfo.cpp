#include "windowinfo.h"

WindowInfo* WindowInfo::instance = NULL;

WindowInfo* WindowInfo::getInstance()
{
	if(instance == NULL)
		instance = new WindowInfo;
	
	return instance;
}

WindowInfo::WindowInfo()
{
	getmaxyx(stdscr, m_y, m_x);
}

WindowInfo::~WindowInfo()
{
	//
}

int WindowInfo::getX()
{
	return m_x;
}

int WindowInfo::getY()
{
	return m_y;
}
