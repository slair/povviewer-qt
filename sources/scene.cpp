/****************************************************************************
**
**
**
****************************************************************************/

#include "scene.h"
#include <qmath.h>

Scene::Scene()
{
	// todo: detect arch
	// todo: find povdump via system PATH
	m_povdumpbin = "povdump64";
}

Scene::~Scene()
{
}

