// -*- coding: utf-8 -*-
/** @file myconfig.h
 ** Config for povviewer.
 **/

#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QMetaProperty>
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

public:
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

#endif // CONFIG_H

