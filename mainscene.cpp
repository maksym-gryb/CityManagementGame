#include "mainscene.h"

MainScene::MainScene() : Scene("The Town of Gauverre")
{
	m_update = NULL;
	m_user = NULL;
	m_console = NULL;
	
	m_update_timer = 2000;
	m_day_length = 5000;
	m_timer_next = 0;
	
	m_running = true;
	
	m_day = 0;
	
	/* Set Resources */
	Resource* food 			= createResource("food", 5000, 0);
	Resource* population 	= createResource("population", 87, 0);
	Resource* pop_growth 	= createResource("population growth", 0, 0);
	Resource* wood 			= createResource("wood", 500, 0);
	Resource* housing 		= createResource("house", 30, 0);
	Resource* farming 		= createResource("farm", 0, 0);
	ResourceRelationship rel;
	
	// Food Consumption
	Queue* food_consumption = new Queue("Food Consumption", 1, 1, 0);
	
	rel.m_intensity = 1;
	rel.m_resource = population;
	rel.m_resource_relation = QUANTITY_NEGOTIATOR;
	food_consumption->addResourceRelationship(rel);
	
	rel.m_intensity = 0;
	rel.m_resource = food;
	rel.m_resource_relation = TAKE_FROM_RESOURCE;
	food_consumption->addResourceRelationship(rel);
	
	rel.m_intensity = 1;
	rel.m_resource = pop_growth;
	rel.m_resource_relation = GIVE_TO_RESOURCE;
	food_consumption->addResourceRelationship(rel);
	
	// Population Growth
	Queue* population_growth = new Queue("Population Growth", 10, 0, 0);
	
	rel.m_intensity = 1;
	rel.m_resource = pop_growth;
	rel.m_resource_relation = COUNTER;
	population_growth->addResourceRelationship(rel);
	
	rel.m_intensity = 0.1;
	rel.m_resource = population;
	rel.m_resource_relation = QUANTITY_NEGOTIATOR;
	population_growth->addResourceRelationship(rel);
	
	rel.m_intensity = 0;
	rel.m_resource = population;
	rel.m_resource_relation = GIVE_TO_RESOURCE;
	population_growth->addResourceRelationship(rel);
	
	// Building Construction
	Queue* farm_construction = new Queue("farm", 5, 1, 0, 0, QueueType::CONSTRUCTION);
	
	rel.m_intensity = 4;
	rel.m_resource = wood;
	rel.m_resource_relation = TAKE_FROM_RESOURCE;
	farm_construction->addResourceRelationship(rel);
	
	rel.m_intensity = 1;
	rel.m_resource = farming;
	rel.m_resource_relation = GIVE_TO_RESOURCE;
	farm_construction->addResourceRelationship(rel);
	
	/* Create City */
	m_city = new City("Gauverre");
	m_city->addQueue(food_consumption);
	m_city->addQueue(population_growth);
	m_city->addQueue(farm_construction);
}

MainScene::~MainScene()
{
	delete m_update;
	delete m_user;
	delete m_console;
}

void MainScene::setup()
{
	struct {
		int w;
		int h;
	} m_window = {WindowInfo::getInstance()->getX(), WindowInfo::getInstance()->getY()};
	
	int x, w;
	x = w = m_window.w/2 + 10;
	m_update = new Tile(0, 0, w, m_window.h - 1);
	m_update->setBorder(0b0110);
	
	w = m_window.w - w - 1;
	m_user = new Tile(x, 0, w, m_window.h - 1);
	m_user->setBorder(0b0100);
	
	m_console = new Tile(0, m_window.h - 1, m_window.w, 1, 1);
	m_console->push("_");
}

int MainScene::run()
{
	update();
	
	print();
	
	handleInput();
	
	return m_running;
}

void MainScene::update()
{
	long now = time(NULL)*1000;
	
	if(now > m_timer_next)
	{
		m_timer_next = now + m_day_length;
		m_day++;
		m_update->push("Day: " + std::to_string(m_day));
		
		m_city->run(m_update);
	}
}
	
void MainScene::print()
{
	clear();
	
	m_update->print();
	
	m_user->print();
	
	m_console->print();
	
	refresh();
}

void MainScene::handleInput()
{
	char ch;
	if( (ch = getch()) == ERR )
		return;
	
	std::string current_input = m_console->pop();
	if(current_input == "")
		current_input = "_";
	
	// process input
	if(ch == '\r')// press enter
	{
		current_input.erase(current_input.end() - 1, current_input.end());// remove the trailing '_'
		
		command(current_input);
		
		current_input = "_";
	}
	else if(ch >= 32 && ch <= 126)// pretty much any character
	{
		current_input.erase(current_input.end() - 1, current_input.end());
		current_input += ch;
		current_input += "_";
	}
	else if(ch == 127 && current_input.size() > 1)// delete character (backspace)
	{
		current_input.erase(current_input.end() - 2, current_input.end() - 1);
	}
	else if(ch == 27)// quit (escape)
	{
		m_running = false;
	}
	
	m_console->push(current_input);
}

void MainScene::command(std::string cmdstr)
{
	if(cmdstr.empty())
	{
		call("clear");
		return;
	}
	
	std::vector<std::string> cmdlst = split(cmdstr, ' ');
	
	std::string cmd = cmdlst[0];
	cmdlst.erase(cmdlst.begin());
	
	if(call(cmd, cmdlst) != 0)
	{
		m_user->clear();
		m_user->push("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		m_user->push("ERROR, command <" + cmd + "> not found!");
		m_user->push("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}
}

int MainScene::call(std::string cmd, std::vector<std::string> options)
{
	if(cmd == "status")
	{
		m_user->clear();
		
		// Resources
		m_user->push("****************************************");
		m_user->push("Resources:");
		for(Resource* r : m_resources)
			m_user->push(r->status());
		m_user->push("****************************************");
		
		// City Info
		for(std::string s : split(m_city->status(), '\n'))
			m_user->push(s);
		m_user->push("****************************************");
		
		return 0;
	}
	
	if(cmd == "add")
	{
		if(options.size() != 2)
		{
			m_user->clear();
			m_user->push("Command <add> requires 2 arguments, " + std::to_string(options.size()) + " given!");
			return 0;
		}
		
		std::string r_name = options[0];
		int r_amount = std::stoi(options[1]);//catch the exception in the future
		
		Resource* r = NULL;
		
		for(int i = 0; i < m_resources.size(); i++)
			if(m_resources[i]->getName() == r_name)
			{
				r = m_resources[i];
				break;
			}
			
		if(r != NULL)// if the resource is found
			r->add(r_amount);
		else
		{
			m_user->clear();
			m_user->push("Could not find Resource:" + r_name + "!");
		}
		
		return 0;
	}
	
	if(cmd == "build")
	{
		if(options.size() != 2)
		{
			m_user->clear();
			m_user->push("Command <build> requires 2 arguments, " + std::to_string(options.size()) + " given!");
			return 0;
		}
		
		std::string q_name = options[0];
		int amount = std::stoi(options[1]);//catch the exception in the future
		
		Queue* q = m_city->getQueueByName(q_name);
			
		if(q != NULL)// if the resource is found
			q->addTimes(amount);
		else
		{
			m_user->clear();
			m_user->push("Could not find Queue:" + q_name + "!");
		}
		
		return 0;
	}
	
	if(cmd == "clear")
	{
		m_user->clear();
		
		return 0;
	}
	
	if(cmd == "exit" || cmd == "quit")
	{
		m_running = false;
		
		return 0;
	}
	
	return -1;// command not found
}

Resource* MainScene::createResource(std::string name, int amount, int max)
{
	Resource* r = new Resource(name, amount, max);
	m_resources.push_back(r);
	return r;
}

std::vector<std::string> MainScene::split(std::string const & cmd, char delim)
{
	// convert input to lowercase
	//std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
	
	// split
	std::vector<std::string> cmd_lst;
	std::istringstream iss(cmd);

	for(std::string token; std::getline(iss, token, delim); )
		cmd_lst.push_back(std::move(token));

	return cmd_lst;
}
