#include "mainscene.h"

MainScene::MainScene() : Scene("The Town of Gauverre")
{
	m_update = NULL;
	m_user = NULL;
	m_console = NULL;
	
	//m_update_timer = 2000;
	m_day_length = 5.0;
	m_timer_begin = high_resolution_clock::now();
	
	m_running = true;
	
	m_day = 0;
	
	/* Set Resources */
	Resource* food 			= createResource("food", 1000, 0);
	Resource* population 	= createResource("population", 87, 0);
	Resource* pop_growth 	= createResource("pop growth counter", 0, 0);
	Resource* wood 			= createResource("wood", 500, 0);
	Resource* housing 		= createResource("house", 30, 0);
	Resource* farms 		= createResource("farm", 0, 0);
	ResourceRelationship rel;
	
	// Food Consumption
	Queue* food_consumption = new Queue("Food Consumption", 1, 1, 0);
	
	rel.m_intensity = 1;
	rel.m_resource = population;
	rel.m_resource_relation = QUANTITY_NEGOTIATOR;
	food_consumption->addResourceRelationship(rel);
	
	rel.m_intensity = 0;
	rel.m_resource = food;
	rel.m_resource_relation = CONTRIBUTOR;
	food_consumption->addResourceRelationship(rel);
	
	rel.m_intensity = 1;
	rel.m_resource = pop_growth;
	rel.m_resource_relation = RECEIVER;
	food_consumption->addResourceRelationship(rel);
	
	// Population Growth
	Queue* population_growth = new Queue("Population Growth", 10, 0, 0);
	
	rel.m_intensity = 1;
	rel.m_resource = pop_growth;
	rel.m_resource_relation = TALLY;
	population_growth->addResourceRelationship(rel);
	
	rel.m_intensity = 0.1;
	rel.m_resource = population;
	rel.m_resource_relation = QUANTITY_NEGOTIATOR;
	population_growth->addResourceRelationship(rel);
	
	rel.m_intensity = 0;
	rel.m_resource = population;
	rel.m_resource_relation = RECEIVER;
	population_growth->addResourceRelationship(rel);
	
	// Building Construction
	Queue* farm_construction = new Queue("Farm Construction", 5, 1, 0, 0, QueueType::CONSTRUCTION);
	farm_construction->addTag("farm");
	
	rel.m_intensity = 20;
	rel.m_resource = wood;
	rel.m_resource_relation = CONTRIBUTOR;
	farm_construction->addResourceRelationship(rel);
	
	rel.m_intensity = 1;
	rel.m_resource = farms;
	rel.m_resource_relation = RECEIVER;
	farm_construction->addResourceRelationship(rel);
	
	// Building Construction
	Queue* food_production = new Queue("Food Production", 10, 1, 0);
	
	rel.m_intensity = 80;
	rel.m_resource = farms;
	rel.m_resource_relation = QUANTITY_NEGOTIATOR;
	food_production->addResourceRelationship(rel);
	
	rel.m_intensity = 0;
	rel.m_resource = food;
	rel.m_resource_relation = RECEIVER;
	food_production->addResourceRelationship(rel);
	
	/* Create City */
	m_city = new City("Gauverre");
	m_city->addQueue(food_consumption);
	m_city->addQueue(population_growth);
	m_city->addQueue(farm_construction);
	m_city->addQueue(food_production);
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
	x = w = m_window.w/2 + 5;
	m_update = new Tile(0, 0, w, m_window.h - 1);
	m_update->setBorder(0b0110);
	
	w = m_window.w - w - 1;
	m_user = new Tile(x, 0, w, m_window.h - 1);
	m_user->setBorder(0b0100);
	
	m_console = new Tile(0, m_window.h - 1, m_window.w, 1, 1);
	m_console->push("_");
	
#ifdef DEBUG
	command("ff 100 3");
#endif /* DEBUG */
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
	auto now = high_resolution_clock::now();
	double difference = duration_cast<duration <double> >(now - m_timer_begin).count();
	
	bool fast_forward = m_fast_forward_date > m_day && difference > m_ff_timer_diff;
	
	if(difference > m_day_length || fast_forward)
	{
		m_timer_begin = high_resolution_clock::now();
		m_day++;
		m_update->push("Day: " + std::to_string(m_day));
		
		m_city->run(m_update);
		
#ifdef DEBUG
		call("status");
#endif /* DEBUG */
	}
}
	
void MainScene::print()
{
	bool redraw = m_update->redraw()
		   || m_user->redraw()
		   || m_console->redraw();

	if(redraw)
	{
		clear();
	
		m_update->print();
	
		m_user->print();
	
		m_console->print();
	
		refresh();
	}
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
		m_user->push("ERROR, command <" + cmd + "> not found!");
	}
}

int MainScene::call(std::string cmd, std::vector<std::string> options)
{
	if(cmd == "status" || cmd == "s")
	{
		m_user->clear();
		
		// Resources
		m_user->push("*************************");
		m_user->push("Resources:");
		for(Resource* r : m_resources)
			m_user->push(r->status());
		m_user->push("*************************");
		
		// City Info
		for(std::string s : split(m_city->status(), '\n'))
			m_user->push(s);
		m_user->push("*************************");
		
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
		
		std::string q_tag = options[0];
		int amount = std::stoi(options[1]);//catch the exception in the future
		
		Queue* q = m_city->getQueueByTag(q_tag);
			
		if(q != NULL)// if the resource is found
			q->addTimes(amount);
		else
		{
			m_user->clear();
			m_user->push("Could not find Queue:" + q_tag + "!");
		}
		
		return 0;
	}
	
	if(cmd == "ff" || cmd == "fastforward")// fast forward
	{
		if(options.size() < 1 || options.size() > 2)
		{
			m_user->clear();
			m_user->push("Command <fastforward> requires 1-2 arguments, " + std::to_string(options.size()) + " given!");
			return 0;
		}
		
		int ff_days = std::stoi(options[0]);//catch the exception in the future
		m_fast_forward_date = ff_days + m_day;
		
		m_ff_timer_diff = options.size() < 2 ? /*DEFAULT_SPEED=*/1.0/3.0 : 1.0/std::stod(options[1]);//catch the exception in the future
		
		m_user->clear();
		m_user->push("Fast forwarding to day: " + std::to_string(m_fast_forward_date));
		m_user->push("Speed: " + std::to_string(m_ff_timer_diff*1000) + "ms / day");
		
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
