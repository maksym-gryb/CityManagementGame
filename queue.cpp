#include "queue.h"

Queue::Queue(std::string name, unsigned max, int fill, int current, int times, int type)
{
	m_name = name;
	
	m_fill = fill;
	m_current = current;
	m_max = max;
	
	m_times = times;
	m_type = type;
}

Queue::~Queue()
{
	//
}

void Queue::addTag(std::string tag)
{
	m_tags.push_back(tag);
}

bool Queue::checkTag(std::string tag)
{
	for(std::string str : m_tags)
		if(str == tag)
			return true;
		
	return false;
}

int Queue::addToQueue()
{	
	if(m_times == 0)
		return NOTHING;
	
	if(m_fill == 0)
	{
		int initial = m_current;
		QueueReturn ret = NOTHING;
		
		for(const ResourceRelationship& rel : m_resources)
		{
			if(rel.m_resource_relation == COUNTER)
			{
				int result = rel.m_resource->take(rel.m_intensity);
				
				if(result > 0)
					m_current += rel.m_intensity;
				else if (result < 0)
					m_current -= rel.m_intensity;
			}
		}
	
		if(initial == m_current)// i.e. nothing changed
			ret = NOTHING;
		
		else if(m_max != 0 && m_current >= m_max)
		{
			m_current -= m_max;
			ret = FULL;
		}
		else if(m_current <= 0)
		{
			m_current += m_max;
			ret = EMPTY;
		}
	
		if(ret != NOTHING)
			actOnResourceRelations(ret);
		
		return ret;
	}
	else
		return addToQueue(m_fill);
}

int Queue::addToQueue(int add)
{
	QueueReturn ret = NOTHING;
	
	m_current += add;
	
	if(m_max != 0 && m_current >= m_max)
	{
		m_current -= m_max;
		ret = FULL;
	}
	else if(m_current <= 0)
	{
		m_current += m_max;
		ret = EMPTY;
	}
	
	if(ret != NOTHING)
		actOnResourceRelations(ret);
	
	return ret;
}

void Queue::actOnResourceRelations(QueueReturn status)//change completely
{
	/* Negotiate Amount */
	int negotiated_amount = 0;

	for(const ResourceRelationship& rel : m_resources)
		if(rel.m_resource_relation == QUANTITY_NEGOTIATOR)
			negotiated_amount += rel.m_intensity * rel.m_resource->getAmount();
		
	if(status == EMPTY)
		negotiated_amount *= -1;

	for(const ResourceRelationship& rel : m_resources)
	{
		if(rel.m_resource_relation == GIVE_TO_RESOURCE)
		{
			if(rel.m_intensity == 0)
				rel.m_resource->add(negotiated_amount);
			else
				rel.m_resource->add(rel.m_intensity);
		}
		else if(rel.m_resource_relation == TAKE_FROM_RESOURCE)
		{
			if(rel.m_intensity == 0)
				rel.m_resource->take(negotiated_amount);
			else
				rel.m_resource->take(rel.m_intensity);
		}
	}
	
	// Decrement repetition counter
	if(m_times > 0)
		m_times --;
}

std::string Queue::status()
{
	std::string response = "";
	
	response += m_name + " : " + std::to_string(m_current) + "(";
	response += m_fill != 0 ? (m_fill > 0 ? "+" : "-") : ""; // is it a '+' OR '-' OR nothing for 0(zero)
	response += std::to_string(m_fill) + ")" + "/";
	response += m_max > 0 ? std::to_string(m_max) : "inf";
	response += " x " + std::to_string(m_times);
	
	return response;
}

std::string Queue::getName()
{
	return m_name;
}

void Queue::setFill(int fill)
{
	m_fill = fill;
}

int Queue::getFill()
{
	return m_fill;
}

void Queue::setCurrent(int current)
{
	m_current = current;
}

int Queue::getCurrent()
{
	return m_current;
}

void Queue::setMax(int max)
{
	m_max = max;
}

int Queue::getMax()
{
	return m_max;
}

void Queue::addTimes(int more)
{
	m_times += more;
}

void Queue::addResourceRelationship(ResourceRelationship rr)
{
	m_resources.push_back(rr);
}
