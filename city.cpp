#include "city.h"

City::City(std::string name)
{
	m_name = name;
}

City::~City()
{
	//
}

void City::run(Tile* display_tile)
{
	for(Queue* q : m_queues)
		if(q->addToQueue() == QueueReturn::FULL)
			display_tile->push("[*] City:" + getName() + " => " + "QueueCompleted:" + q->getName());
}

void City::addQueue(Queue* new_queue)
{
	m_queues.push_back(new_queue);
}

std::string City::status()
{
	std::string response = "";
	
	response += "Queues for City " + m_name + "\n";
	response += "------------------------------\n";
	
	for(Queue* q : m_queues)
		response += q->status() + "\n";
	
	return response;
}

std::string City::getName()
{
	return m_name;
}

Queue* City::getQueueByTag(std::string tag)
{
	for(Queue* q : m_queues)
		if(q->checkTag(tag))
			return q;
		
	return NULL;
}
