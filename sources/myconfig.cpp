// -*- coding: utf-8 -*-
/** @file myconfig.cpp
 ** Config for povviewer.
 **/

#include <QSysInfo>
#include <QProcess>
#include <QDir>
#include <QSettings>

#include "myconfig.h"

bool Config::save_to_dir(const QString& dir, int save_user) const
{
	bool res = true;
	QDir d = QDir(dir);

	//~ QString filename = d.absoluteFilePath(CFG_FILENAME);
	QString filename = d.relativeFilePath(CFG_FILENAME);

	QSettings* settings;
	if (save_user == 0) {
		settings = new QSettings(filename, QSettings::IniFormat);
		qDebug() << "Saving" << filename << "to" << dir << "folder.";
	} else {
		settings = new QSettings(QSettings::IniFormat, QSettings::UserScope
								 , "povviewer-qt", "povviewer-qt");
		// folder, file
		qDebug() << "Saving" << CFG_FILENAME << "to HOME folder.";
	}
	const QMetaObject* pmo = metaObject();
	for (int i=0; i < pmo->propertyCount(); ++i) {
		const QMetaProperty mp = pmo->property(i);
		QString _name = mp.name();
		if (_name == "objectName") continue;
		settings->setValue(mp.name(), property(mp.name()));
	}
	delete settings;
	return res;
}

Config::~Config()
{
	qDebug() << "Config::~Config()";
	if (m_Changed) {
		if (m_bSaveLocalCFG) {
			save_to_dir(".");
		}
		if (m_bSaveHomeCFG) {
			save_to_dir(".", 1);
		}
	} else {
		qDebug() << "Config not changed";
	}
	qDebug() << "Config deleted";
}

Config::Config() : m_bChgWinPos(true), m_iWinPosX(80), m_iWinPosY(80)
	, m_bChgWinSize(true), m_uiWinWidth(320), m_uiWinHeight(240)
	, m_bSaveLocalCFG(false), m_bLoadLocalCFG(true), m_bSaveHomeCFG(true)
	, m_Changed(false), m_fp_dump("c:\\temp\\scene.dump")
	, m_fp_cfg("c:\\temp\\scene.cfg")
	, m_fp_debug("debug.log")
	, m_fp_fatal("fatal.log")
	, m_fp_render("render.log")
	, m_fp_statistic("statistic.log")
	, m_fp_warning("warning.log")
{
#pragma warning(disable : 4305)
	m_bg_color = QVector4D(0.854902, 0.788235, 0.749020, 0);
#pragma warning(default : 4305)
	m_qs_bg_color = QVector4D_to_QString(m_bg_color);

	m_bShowAxis = true;				// show axis X, Y, Z
	m_fAxisSize = 100.0;			// axis size
	m_bIsPerspective = true;		// perspective camera, else orthographic

	load_from_dir(".", 1);	// load from HOME
}

QDebug operator << (QDebug d, const Config& cfg)
{
	d << "Config" << endl;
	const QMetaObject* pmo = cfg.metaObject();
	for (int i=0; i < pmo->propertyCount(); ++i) {
		const QMetaProperty mp = pmo->property(i);
		d << " Property# :" << i << endl;
		d << "      Type :" << mp.typeName() << endl;
		d << "      Name :" << mp. name() << endl;
		d << "     Value :" << cfg.property(mp.name()) << endl << endl;
	}
	return d;
}

bool Config::find_povdump(const QString& povdump_name)
{
	// detect arch
	QString arch = QSysInfo::currentCpuArchitecture();
	qDebug() << "arch =" << arch;

	// detect platform
	QString kernel = QSysInfo::kernelType();
	qDebug() << "kernel =" << kernel;

	QChar sep;
	if (kernel == "winnt") {
		sep = ';';
		if (arch == "x86_64") {
			m_povdumpbin = "povdump64.exe";
		} else {
			m_povdumpbin = "povdump.exe";
		}
	} else {
		sep = ':';
		if (arch == "x86_64") {
			m_povdumpbin = "povdump64";
		} else {
			m_povdumpbin = "povdump";
		}
	}
	if (povdump_name != "") {
		m_povdumpbin = povdump_name;
	}
	qDebug() << "m_povdumpbin =" << m_povdumpbin;

	// find povdump via system PATH
	QStringList env = QProcess::systemEnvironment();
	//~ qDebug() << "env =" << env;

	QStringList path;
	for(int i = 0; i < env.size(); ++i) {
		if (env.at(i).startsWith("PATH=", Qt::CaseInsensitive)) {
			path = env.at(i).right(env.at(i).size() - 5).split(sep);
			break;
		}
	}
	if (path.size() == 0) {
		qWarning() << "Empty PATH environment variable";
	}
	//~ qDebug() << "path =" << path;

	for(int i = 0; i < path.size(); ++i) {
		if (path.at(i) != "") {
			if (QFile::exists(path.at(i) + QDir::separator() + m_povdumpbin)) {
				m_fp_povdumpbin = path.at(i) + QDir::separator() + m_povdumpbin;
				break;
			}
		}
	}
	qDebug() << "m_fp_povdumpbin =" << m_fp_povdumpbin;

	if (m_fp_povdumpbin.isEmpty()) {
		qWarning() << m_povdumpbin << "not found via PATH variable";
		return false;
	}
	return true;
}

bool Config::scan_scene_file(const QString &scenepath)
{
	//~ qDebug() << *this;
	qDebug() << "Scan file" << scenepath;

	QFile f{scenepath};

	if (!f.open(QIODevice::ReadOnly)) {
		qWarning() << "Cannot open file" << scenepath << "for reading";
		return false;
	}

	QTextStream in{&f};

	while (!in.atEnd()) {
		QString line = in.readLine().simplified().toLower();
		if (line.startsWith("// povviewer:")) {
			//~ qDebug() << line;
			QStringList linelist = line.split(" ");
			//~ qDebug() << linelist;
			const QMetaObject* pmo = this->metaObject();
			for (int i=0; i < pmo->propertyCount(); ++i) {

				const QMetaProperty mp = pmo->property(i);
				QString _name = mp.name();
				QString _typename = mp.typeName();

				if (_name == "objectName") continue;

				QString _s(_name);
				_s += "(=.*)?";
				QRegExp rx_s(_s);

				int kw_index = linelist.indexOf(rx_s);
				if (kw_index == -1) continue;

				QString _value;

				//~ qDebug() << _name << kw_index << linelist.size();

				int eq_pos = linelist[kw_index].indexOf("=");

				//~ qDebug() << linelist[kw_index] << eq_pos
				//~ << linelist[kw_index].length();

				if (eq_pos == -1) {	// no equal sign
					if (kw_index + 2 < linelist.size()) {
						_value = linelist[kw_index + 2];
					}
				} else {
					if (linelist[kw_index].length() - eq_pos - 1 > 0) {
						_value = linelist[kw_index].right(
									 linelist[kw_index].length()
									 - eq_pos - 1);
					} else {
						if (kw_index + 1 < linelist.size()) {
							_value = linelist[kw_index + 1];
						}
					}
				}
				if (_value == "") continue;
				if (_typename == "int") {
					bool ok;
					_value.toInt(&ok);
					if (!ok) {
						qDebug() << "Cannot change" << _name << "to"
								 << _value;
						continue;
					}
				}
				if (_typename == "float") {
					bool ok;
					_value.toFloat(&ok);
					if (!ok) {
						qDebug() << "Cannot change" << _name << "to"
								 << _value;
						continue;
					}
				}
				if (_name.startsWith("window_pos_") && !m_bChgWinPos) {
					qWarning() << "Cannot change" << _name << "to"
							   << _value;
					qWarning() << "Because change_window_pos is false";
					continue;
				}
				if ((_name =="window_width" || _name =="window_height")
					&& !m_bChgWinSize) {
					qWarning() << "Cannot change" << _name << "to"
							   << _value;
					qWarning() << "Because change_window_size is false";
					continue;
				}
				qDebug() << "Change" << _name << "to" << _value;
				this->setProperty(_name.toLocal8Bit().constData(), _value);
				m_Changed = true;
			}
		}
	}
	//~ qDebug() << *this;
	return true;
}

bool Config::load_from_dir(const QString& dir, int load_user)
{
	bool empty_home_config = false;
	QDir d = QDir(dir);
	QSettings* settings;

	if (load_user == 0) {

		//~ QString filename = d.absoluteFilePath(CFG_FILENAME);
		QString filename = d.relativeFilePath(CFG_FILENAME);

		if (!QFile::exists(filename)) {
			qDebug() << "No" << filename << "file found at" << dir
					 << "folder.";
			return false;
		}

		settings = new QSettings(filename, QSettings::IniFormat);
		qDebug() << "Loading" << filename << "from" << dir << "folder.";

	} else {
		settings = new QSettings(QSettings::IniFormat, QSettings::UserScope
								 , "povviewer-qt", "povviewer-qt");
		qDebug() << "Loading" << CFG_FILENAME << "from HOME folder.";
		//~ qDebug() << settings->allKeys().size();
		if (settings->allKeys().size() == 0) {
			empty_home_config = true;
		}
	}

	const QMetaObject* pmo = metaObject();
	for (int i=0; i < pmo->propertyCount(); ++i) {
		const QMetaProperty mp = pmo->property(i);
		QString _name = mp.name();
		if (_name == "objectName") continue;
		if (settings->contains(_name)) {
			setProperty(_name.toLocal8Bit().constData()
						, settings->value(_name));
			m_Changed = true;
		}
	}

	delete settings;

	if (load_user != 0) {
		// load from HOME
		if (empty_home_config) {
			m_Changed = true;
		} else {
			m_Changed = false;
		}
		if (m_fp_povdumpbin.isEmpty()) {
			find_povdump("");
		} else {
			if (!QFile::exists(m_fp_povdumpbin)) {
				qCritical() << m_fp_povdumpbin << "not found";
				find_povdump("");
			}
		}
	}

	return true;
}

bool Config::dump_cfg() const
{
	FILE* f = fopen(m_fp_cfg.toLocal8Bit().constData(), "w");
	fclose(f);
	return false;
}
