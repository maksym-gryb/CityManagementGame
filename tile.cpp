#include "tile.h"

Tile::Tile(int p_x, int p_y, int p_w, int p_h, unsigned p_size)
{
	m_x = p_x;
	m_y = p_y;
	m_width = p_w - 1;// the -1 is a temporary solution
	m_height = p_h - 1;// the -1 is a temporary solution
	
	m_memory_max_size = p_size;
}

Tile::~Tile()
{
	//DESTROY
}

void Tile::print()
{
	int y = m_y + m_height;
	
	for(std::list<std::string>::reverse_iterator itt = m_memory.rbegin(); itt != m_memory.rend(); itt++)
	{
		int line = (*itt).size() / m_width;
		int cursor[] = { m_x, y - line };
		
		for(const char c : (*itt))
		{
			if(cursor[0] > m_x + m_width)
			{
				cursor[0] = m_x;
				cursor[1]++;
			}
			mvaddch(cursor[1], cursor[0], c);
			cursor[0]++;
		}
		
		y = y - line - 1;
	}
	
	drawBorder();
}

void Tile::setBorder(int p_border)
{
	m_border = p_border;
	
	if(m_border & TOP_BORDER)
		m_y++;
	
	if(m_border & RIGHT_BORDER)
		m_width--;
	
	if(m_border & BOTTOM_BORDER)
		m_height--;
	
	if(m_border & LEFT_BORDER)
		m_x++;
}

void Tile::drawBorder()
{
	if(m_border & TOP_BORDER)
	{
		for(int x = m_x - 1, y = m_y - 1; x <= m_x + m_width + 1; x++)
			mvaddch(y, x, ACS_BLOCK);
			//mvaddch(y, x, ACS_HLINE);
	}
	
	if(m_border & RIGHT_BORDER)
	{
		for(int x = m_x + m_width + 1, y = m_y - 1; y <= m_y + m_height + 1; y++, x = m_x + m_width + 1)
			mvaddch(y, x, ACS_BLOCK);
			//mvaddch(y, x, ACS_VLINE);
	}
	
	if(m_border & BOTTOM_BORDER)
	{
		for(int x = m_x - 1, y = m_y + m_height + 1; x <= m_x + m_width + 1; x++)
			mvaddch(y, x, ACS_BLOCK);
			//mvaddch(y, x, '#');
	}
	
	if(m_border & LEFT_BORDER)
	{
		for(int x = m_x - 1, y = m_y - 1; y <= m_y + m_height + 1; y++)
			mvaddch(y, x, ACS_BLOCK);
			//mvaddch(y, x, ACS_VLINE);
	}
}

void Tile::push(std::string str)
{
	if(m_memory.size() >= m_memory_max_size)
		m_memory.pop_front();
	
	m_memory.push_back(str);
}

std::string Tile::pop()
{
	std::string str = "";
	
	if(m_memory.size() > 0)
	{
		str = m_memory.back();
		
		m_memory.pop_back();
	}
	
	return str;
}

std::string Tile::peek()
{
	std::string str = "";
	
	if(m_memory.size() > 0)
		str = m_memory.back();
	
	return str;
}

void Tile::clear()
{
	m_memory.clear();
}
