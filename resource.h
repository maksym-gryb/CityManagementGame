#include "inc.h"

enum ResourceRelation{
	GIVE_TO_RESOURCE = 0,
	SUSTAIN_RESOURCE,//not implemented
	TAKE_FROM_RESOURCE,
	QUANTITY_NEGOTIATOR,
	COUNTER
};

class Resource{
public:
	/* Constructors */
	Resource(std::string name, int amount = 0, int max = 100, int min = 0);
	~Resource();
	
	/* Resources */
	std::string status();
	std::string getName();
	void add(int amount);
	int take(int amount);
	int getAmount();
	
private:
	/* Variables */
	std::string m_name;
	int m_amount;
	int m_min;
	int m_max;// value of 0(zero) means no-limit
};
