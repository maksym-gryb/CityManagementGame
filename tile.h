#ifndef TILE_H
#define TILE_H

#include "inc.h"

#define TOP_BORDER    0b0001
#define RIGHT_BORDER  0b0010
#define BOTTOM_BORDER 0b0100
#define LEFT_BORDER   0b1000

class Tile{
public:
	/* Constructors */
	Tile(int p_x, int p_y, int p_w, int p_h, unsigned p_size = 100);
	~Tile();
	
	/* Tile */
	bool redraw();
	void print();
	void setBorder(int p_border = 0);
	void drawBorder();
	
	/* Memory Manipulation */
	void push(std::string p_str);
	std::string pop();
	std::string peek();
	void clear();
	
private:
	/* Variables */
	std::list<std::string> m_memory;
	unsigned m_memory_max_size;

	bool m_redraw;
	
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	
	int m_border;
	
	/*
	 * Add text-color properties
	 */
};

#endif /* TILE_H */
