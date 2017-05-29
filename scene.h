/*
 * The scene/state of the game (e.g. normal, battle, city, etc.)
 */
#include "inc.h"

class Scene{
public:
	/** Constructors */
	Scene(std::string p_scene_name);
	~Scene();
    
    /** Scene */
    virtual void setup() = 0;
    virtual int run() = 0;
	
	/** Scene */
	std::string getName();
	
private:
	/** Variables */
	std::string m_scene_name;
};

class EmptyScene : public Scene{
public:
	/** Constructors */
	EmptyScene() : Scene("NULLSCENE") {};
	~EmptyScene() {};
    
    /** Scene */
    void setup() {};
    int run() { return 0; };
};
