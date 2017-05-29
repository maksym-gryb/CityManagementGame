#include "scene.h"

Scene::Scene(std::string p_scene_name)
{
	m_scene_name = p_scene_name;
}

Scene::~Scene() {}

std::string Scene::getName()
{
	return m_scene_name;
}
