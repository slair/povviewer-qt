// -*- coding: utf-8 -*-
/****************************************************************************
**
**
**
****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QMetaProperty>
#include <QTextStream>
#include <QFile>

#include <QDebug>

class Config : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool change_window_pos MEMBER m_bChgWinPos)
	Q_PROPERTY(int window_pos_x MEMBER m_iWinPosX)
	Q_PROPERTY(int window_pos_y MEMBER m_iWinPosY)

	Q_PROPERTY(bool change_window_size MEMBER m_bChgWinSize)
	Q_PROPERTY(uint window_width MEMBER m_uiWinWidth)
	Q_PROPERTY(uint window_height MEMBER m_uiWinHeight)

	Q_PROPERTY(bool save_local_cfg MEMBER m_bSaveLocalCFG)
	Q_PROPERTY(bool load_local_cfg MEMBER m_bLoadLocalCFG)

	Q_PROPERTY(bool load_home_cfg MEMBER m_bSaveHomeCFG)

private:
	bool m_bChgWinPos;
	int m_iWinPosX;
	int m_iWinPosY;
	bool m_bChgWinSize;
	uint m_uiWinWidth;
	uint m_uiWinHeight;
	bool m_bSaveLocalCFG;
	bool m_bLoadLocalCFG;
	bool m_bSaveHomeCFG;

public:
	Config();
	~Config();

	friend QDebug operator << (QDebug d, const Config& cfg);

	bool scan_scene_file(const QString &scenepath);
};

#endif // CONFIG_H

