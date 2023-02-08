// -*- coding: utf-8 -*-
/** @file myconfig.h
 ** Config for povviewer.
 **/

#ifndef MYCONFIG_H
#define MYCONFIG_H

#include <QObject>
#include <QMetaProperty>
#include <QVector4D>
#include <QTextStream>
#include <QFile>
#include <QDebug>

#include "v2s.h"

#define CFG_FILENAME "povviewer-qt.ini"

class Config : public QObject {
	Q_OBJECT
	Q_PROPERTY(bool change_window_pos READ change_window_pos
			   WRITE set_change_window_pos)
	Q_PROPERTY(int window_pos_x READ window_pos_x WRITE set_window_pos_x)
	Q_PROPERTY(int window_pos_y READ window_pos_y WRITE set_window_pos_y)

	Q_PROPERTY(bool change_window_size READ change_window_size
			   WRITE set_change_window_size)
	Q_PROPERTY(uint window_width READ window_width WRITE set_window_width)
	Q_PROPERTY(uint window_height READ window_height WRITE set_window_height)

	Q_PROPERTY(bool save_local_cfg MEMBER m_bSaveLocalCFG)
	Q_PROPERTY(bool load_local_cfg MEMBER m_bLoadLocalCFG)

	Q_PROPERTY(bool save_home_cfg MEMBER m_bSaveHomeCFG)

	Q_PROPERTY(QString path_to_povdump READ path_to_povdump
			   WRITE set_path_to_povdump)
	Q_PROPERTY(QString path_to_dump READ path_to_dump
			   WRITE set_path_to_dump)
	Q_PROPERTY(QString path_to_cfg READ path_to_cfg
			   WRITE set_path_to_cfg)

	Q_PROPERTY(QString path_to_debuglog READ path_to_debuglog
			   WRITE set_path_to_debuglog)
	Q_PROPERTY(QString path_to_fatallog READ path_to_fatallog
			   WRITE set_path_to_fatallog)
	Q_PROPERTY(QString path_to_renderlog READ path_to_renderlog
			   WRITE set_path_to_renderlog)
	Q_PROPERTY(QString path_to_statisticlog READ path_to_statisticlog
			   WRITE set_path_to_statisticlog)
	Q_PROPERTY(QString path_to_warninglog READ path_to_warninglog
			   WRITE set_path_to_warninglog)

	Q_PROPERTY(QString bg_color READ qs_bg_color
			   WRITE set_qs_bg_color)

	Q_PROPERTY(bool show_axis READ show_axis WRITE set_show_axis)
	Q_PROPERTY(bool is_perspective READ is_perspective WRITE set_perspective)
	Q_PROPERTY(float axis_size READ axis_size WRITE set_axis_size)

private:
	bool m_Changed;
	bool m_bChgWinPos;
	int m_iWinPosX;
	int m_iWinPosY;
	bool m_bChgWinSize;
	uint m_uiWinWidth;
	uint m_uiWinHeight;
	bool m_bSaveLocalCFG;
	bool m_bLoadLocalCFG;
	bool m_bSaveHomeCFG;
	QString m_povdumpbin;		// executable name: povdump64.exe or povdump64
	QString m_fp_povdumpbin;	// fullpath to povdump or povdump64
	QString m_fp_dump;			// fullpath to dump
	QString m_fp_cfg;			// fullpath to binary cfg
	QString m_fp_debug;			// fullpath to debug log
	QString m_fp_fatal;			// fullpath to fatal log
	QString m_fp_render;		// fullpath to render log
	QString m_fp_statistic;		// fullpath to statistic log
	QString m_fp_warning;		// fullpath to warning log
	QString m_qs_bg_color;		// window background color RGBA for Config
	QVector4D m_bg_color;		// window background color RGBA internal
	bool m_bShowAxis;			// show axis X, Y, Z
	float m_fAxisSize;			// axis size
	bool m_bIsPerspective;		// perspective camera, else orthographic

public:
	float axis_size() const {
		return m_fAxisSize;
	}
	void set_axis_size(float f) {
		m_fAxisSize = f;
		m_Changed = true;
	}

	bool is_perspective() const {
		return m_bIsPerspective;
	}
	void set_perspective(bool b) {
		m_bIsPerspective = b;
		m_Changed = true;
	}

	bool change_window_pos() const {
		return m_bChgWinPos;
	}
	void set_change_window_pos(bool b) {
		m_bChgWinPos = b;
		m_Changed = true;
	}
	int window_pos_x() const {
		return m_iWinPosX;
	}
	void set_window_pos_x(int i) {
		if (m_bChgWinPos) {
			m_iWinPosX = i;
			m_Changed = true;
		} else {
			qWarning() << "Can't change window_pos_x. "
					   "change_window_pos is false";
		}
	}
	int window_pos_y() const {
		return m_iWinPosY;
	}
	void set_window_pos_y(int i) {
		if (m_bChgWinPos) {
			m_iWinPosY = i;
			m_Changed = true;
		} else {
			qWarning() << "Can't change window_pos_y. "
					   "change_window_pos is false";
		}
	}

	bool change_window_size() const {
		return m_bChgWinSize;
	}
	void set_change_window_size(bool b) {
		m_bChgWinSize = b;
		m_Changed = true;
	}
	uint window_width() const {
		return m_uiWinWidth;
	}
	void set_window_width(uint ui) {
		if (m_bChgWinSize) {
			m_uiWinWidth = ui;
			m_Changed = true;
		} else {
			qWarning() << "Can't change window_width. "
					   "change_window_size is false";
		}
	}
	uint window_height() const {
		return m_uiWinHeight;
	}
	void set_window_height(uint ui) {
		if (m_bChgWinSize) {
			m_uiWinHeight = ui;
			m_Changed = true;
		} else {
			qWarning() << "Can't change window_height. "
					   "change_window_size is false";
		}
	}

	QString qs_bg_color() {
		m_qs_bg_color = QVector4D_to_QString(m_bg_color);
		return m_qs_bg_color;
	}
	void set_qs_bg_color(QString& s) {
		m_qs_bg_color = s;
		m_bg_color = QString_to_QVector4D(m_qs_bg_color);
		m_Changed = true;
	}
	QVector4D bg_color() const {
		return m_bg_color;
	}
	void set_bg_color(QVector4D& v) {
		m_bg_color = v;
		m_Changed = true;
	}

	QString path_to_debuglog() const {
		return m_fp_debug;
	}
	void set_path_to_debuglog(const QString& s) {
		m_fp_debug = s;
		m_Changed = true;
	}
	QString path_to_fatallog() const {
		return m_fp_fatal;
	}
	void set_path_to_fatallog(const QString& s) {
		m_fp_fatal = s;
		m_Changed = true;
	}
	QString path_to_renderlog() const {
		return m_fp_render;
	}
	void set_path_to_renderlog(const QString& s) {
		m_fp_render = s;
		m_Changed = true;
	}
	QString path_to_statisticlog() const {
		return m_fp_statistic;
	}
	void set_path_to_statisticlog(const QString& s) {
		m_fp_statistic = s;
		m_Changed = true;
	}
	QString path_to_warninglog() const {
		return m_fp_warning;
	}
	void set_path_to_warninglog(const QString& s) {
		m_fp_warning = s;
		m_Changed = true;
	}

	QString path_to_povdump() const {
		return m_fp_povdumpbin;
	}
	void set_path_to_povdump(const QString& s) {
		m_fp_povdumpbin = s;
		m_Changed = true;
	}

	QString path_to_dump() const {
		return m_fp_dump;
	}
	void set_path_to_dump(const QString& s) {
		m_fp_dump = s;
		m_Changed = true;
	}

	QString path_to_cfg() const {
		return m_fp_cfg;
	}
	void set_path_to_cfg(const QString& s) {
		m_fp_cfg = s;
		m_Changed = true;
	}

	bool show_axis() const {
		return m_bShowAxis;
	}
	void set_show_axis(bool b) {
		m_bShowAxis = b;
		m_Changed = true;
	}

	Config();
	~Config();

	friend QDebug operator << (QDebug d, const Config& cfg);

	bool load_from_dir(const QString& dir, int load_user=0);
	bool save_to_dir(const QString& dir, int save_user=0) const;
	bool scan_scene_file(const QString& scenepath);
	bool find_povdump(const QString& povdump_name);
	bool dump_cfg() const;
};

#endif // MYCONFIG_H

