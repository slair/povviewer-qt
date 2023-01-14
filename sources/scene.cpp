// -*- coding: utf-8 -*-
/** @file scene.cpp
 ** Scene and related.
 **/

#include <qmath.h>

#include "scene.h"

Scene::Scene(Config& cfg, const QString& filename)
{
	// todo: 1. detect arch
	// todo: 2. detect platform
	// todo: 3. find povdump via system PATH
	m_povdumpbin = "povdump64";
}

Scene::~Scene()
{
}

