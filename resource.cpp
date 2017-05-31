#include "resource.h"

Resource::Resource(std::string name, int amount, int max/*, int min*/)
{
	m_name = name;
	
	m_amount = amount;
	//m_min = min;
	m_max = max;
}

Resource::~Resource()
{
	//
}

std::string Resource::status()
{
	std::string response = "";

	response += m_name + " : " + std::to_string(m_amount);
	response += "/";
	response += m_max > 0 ? std::to_string(m_max) : "inf";

	return response;
}

/* Resources */
std::string Resource::getName()
{
	return m_name;
}

void Resource::add(int amount)
{
	m_amount += amount;
}

int Resource::take()
{
	int buf = m_amount;
	m_amount = 0;
	
	return buf;
}

int Resource::take(int amount)
{
	m_amount -= amount;
	
	// compare signs
	return m_amount;
}

void Resource::setAmount(int amount)
{
	m_amount = amount;
}

int Resource::getAmount()
{
	return m_amount;
}
