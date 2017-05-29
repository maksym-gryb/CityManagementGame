#include "inc.h"

#include "resource.h"
//#include "tile.h"

enum QueueReturn{
	EMPTY,
	NOTHING = 0,
	FULL
};

struct ResourceRelationship{
	double m_intensity;// if 0(zero), then get QUANTITY_NEGOTIATOR's amount
	Resource* m_resource;
	ResourceRelation m_resource_relation;
};

class Queue{
public:
	/* Constructors */
	Queue(std::string name, unsigned max = 100, int fill = 1, int current = 0);
	~Queue();
	
	/* Queue */
	int addToQueue();
	int addToQueue(int add);// return FULL or NOT_FULL
	void actOnResourceRelations(QueueReturn status);
	std::string status();
	std::string getName();
	
	/* Get/Set */
	void setFill(int fill);
	int getFill();
	
	void setCurrent(int current);
	int getCurrent();
	
	void setMax(int max);
	int getMax();
	
	/* Resource */
	void addResourceRelationship(ResourceRelationship rr);
	
private:
	/* Variables */
	// Queue
	std::string m_name;
	
	int m_fill;// if fill == 0, then use ResourceRelation::COUNTER
	int m_current;
	unsigned m_max;// value of 0(zero) means no_limit
	
	// Resource
	std::vector<ResourceRelationship> m_resources;
};
