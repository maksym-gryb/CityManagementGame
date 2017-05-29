#include "inc.h"

class WindowInfo{
	/* Singleton */
private:
	static WindowInfo* instance;
	WindowInfo();
	
public:
	static WindowInfo* getInstance();
	~WindowInfo();
	
	/* WindowInfo */
	int getX();
	int getY();
	
private:
	/* Variables */
	int m_x, m_y;
};
