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

#define CFG_FILENAME "povviewer-qt.ini"

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

public:
	void from_bg();		// from QVector4D to QString
	void to_bg();		// from QString to QVector4D
	QString qs_bg_color() {from_bg(); return m_qs_bg_color;}
	void set_qs_bg_color(QString& s) {m_qs_bg_color = s; to_bg();}
	QVector4D bg_color() const {return m_bg_color;}

	QString path_to_debuglog() const {
		return m_fp_debug;
	}
	void set_path_to_debuglog(const QString& s) {
		m_fp_debug = s;
	}
	QString path_to_fatallog() const {
		return m_fp_fatal;
	}
	void set_path_to_fatallog(const QString& s) {
		m_fp_fatal = s;
	}
	QString path_to_renderlog() const {
		return m_fp_render;
	}
	void set_path_to_renderlog(const QString& s) {
		m_fp_render = s;
	}
	QString path_to_statisticlog() const {
		return m_fp_statistic;
	}
	void set_path_to_statisticlog(const QString& s) {
		m_fp_statistic = s;
	}
	QString path_to_warninglog() const {
		return m_fp_warning;
	}
	void set_path_to_warninglog(const QString& s) {
		m_fp_warning = s;
	}

	QString path_to_povdump() const {
		return m_fp_povdumpbin;
	}
	void set_path_to_povdump(const QString& s) {
		m_fp_povdumpbin = s;
	}

	QString path_to_dump() const {
		return m_fp_dump;
	}
	void set_path_to_dump(const QString& s) {
		m_fp_dump = s;
	}

	QString path_to_cfg() const {
		return m_fp_cfg;
	}
	void set_path_to_cfg(const QString& s) {
		m_fp_cfg = s;
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

