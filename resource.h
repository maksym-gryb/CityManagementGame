#include "inc.h"

/*
 * RECEIVER/CONTRIBUTOR : give/take - respectively - on every Queue Completion
 * SUSTAINED : give/take, on every tick (i.e. in-game day)
 * QUANTITY_NEGOTIATOR : Use to get dynamic "intensity" value, based on resource quantity
 * TALLY : Counts for the Queue
 */
enum ResourceRelation{
	RECEIVER = 0,
	RECEIVER_SUSTAINED,
	
	CONTRIBUTOR,
	CONTRIBUTOR_SUSTAINED,
	
	QUANTITY_NEGOTIATOR,
	
	TALLY // https://en.wikipedia.org/wiki/Tally_counter
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
