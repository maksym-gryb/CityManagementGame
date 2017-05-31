#include "queue.h"

Queue::Queue(std::string name, int max, int fill, int current, int times, int type)
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
		bool changed = false;
		QueueReturn ret = QueueReturn::NOTHING;
		
		for(const ResourceRelationship& rel : m_resources)
		{
			if(rel.m_resource_relation == TALLY)
			{
				int result = rel.m_resource->take();
				
				if(result != 0)
				{
					m_current += result;
					changed = true;
				}
				else
					changed = false;
			}
		}
		
		if(!changed)
			ret = QueueReturn::NOTHING;
		else if(m_max != 0 && m_current >= m_max)
		{
			//m_current -= m_max;
			m_current = 0;
			ret = QueueReturn::FULL;
		}
		else if(m_current < 0)
		{
			//m_current += m_max;
			m_current = m_max - 1;
			ret = QueueReturn::EMPTY;
		}
	
		if(ret != QueueReturn::NOTHING)
			actOnResourceRelations(ret);
		
		return ret;
	}
	else
		return addToQueue(m_fill);
}

int Queue::addToQueue(int add)
{
	QueueReturn ret;
	
	m_current += add;
	
	if(add == 0)
		ret = QueueReturn::NOTHING;
	else if(m_max != 0 && m_current >= m_max)
	{
		//m_current -= m_max;
		m_current = 0;
		ret = QueueReturn::FULL;
	}
	else if(m_current < 0)
	{
		//m_current += m_max;
		m_current = m_max - 1;
		ret = QueueReturn::EMPTY;
	}
	
	if(ret != QueueReturn::NOTHING)
		actOnResourceRelations(ret);
	
	return ret;
}

void Queue::actOnResourceRelations(QueueReturn status)//change completely
{
	// set slope
	//int slope = status == FULL ? 1 : -1;
	int slope = 1;
	
	if(status == QueueReturn::FULL)
	{
		for(const ResourceRelationship& rel : m_resources)
		{
			if(rel.m_resource_relation == CONTRIBUTOR)
			{
				slope = rel.m_resource->getAmount() >= 0 ? 1 : -1;
			}
		}
		
		//slope = slope >= 0 ? 1 : -1;
	}
	
	if(status == QueueReturn::EMPTY)
		slope = -1;

	// Negotiate Amount
	int negotiated_amount = 0;
	
	for(const ResourceRelationship& rel : m_resources)
	{
		if(rel.m_resource_relation == QUANTITY_NEGOTIATOR && rel.m_resource->getAmount() > 0)
		{
			int tmp = rel.m_intensity * rel.m_resource->getAmount();
			negotiated_amount += (tmp == 0 ? 1 : tmp);
		}
	}
	
	// Receivers / Constributors
	for(const ResourceRelationship& rel : m_resources)
	{
		if(rel.m_resource_relation == RECEIVER)
		{
			if(rel.m_intensity == 0)
				rel.m_resource->add(negotiated_amount * slope);
			else
				rel.m_resource->add(rel.m_intensity * slope);
		}
		else if(rel.m_resource_relation == CONTRIBUTOR)
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
	response += " x " + (m_times == -1 ? "always" : std::to_string(m_times));
	
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
