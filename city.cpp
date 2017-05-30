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
	
	response += "Status for City " + m_name + "\n";
	response += "------------------------------\n";
	
	for(Queue* q : m_queues)
		response += q->status() + "\n";
	
	return response;
}

std::string City::getName()
{
	return m_name;
}

Queue* City::getQueueByName(std::string name)
{
	for(Queue* q : m_queues)
		if(q->getName() == name)
			return q;
		
	return NULL;
}
