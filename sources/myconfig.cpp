// -*- coding: utf-8 -*-
/****************************************************************************
**
**
**
****************************************************************************/

#include "myconfig.h"

//~ static cfg;
Config::~Config()
{
}

Config::Config() : m_bChgWinPos(false), m_iWinPosX(80), m_iWinPosY(80)
	,m_bChgWinSize(true), m_uiWinWidth(320), m_uiWinHeight(240)
{
}
