#ifndef CITY_H
#define CITY_H

#include "inc.h"

#include "queue.h"
#include "tile.h"

class City{
public:
	/* Constructor */
	City(std::string name);
	~City();
	
	/* City */
	void run(Tile* display_tile);// simulate one day
	void addQueue(Queue* new_queue);
	std::string status();
	std::string getName();
	
	Queue* getQueueByTag(std::string tag);
	
private:
	/* Variables */
	std::string m_name;
	std::vector<Queue*> m_queues;
};

#endif/* CITY_H */
