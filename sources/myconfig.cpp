// -*- coding: utf-8 -*-
/** @file myconfig.cpp
 ** Config for povviewer.
 **/

#include <QSysInfo>
#include <QProcess>
#include <QDir>
#include <QSettings>

#include "myconfig.h"

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
//~ #pragma warning(disable : 4305)
	m_bg_color = QVector4D(0.854902f, 0.788235f, 0.749020f, 0);
//~ #pragma warning(default : 4305)

	m_bShowAxis = true;			// show axis X, Y, Z
	m_fAxisSize = 100.0;		// axis size
	m_bIsPerspective = true;	// perspective camera, else orthographic

	m_bShowBBOX = true;			// show bounding boxes

	m_bShowPOINTS = true;		// show points
	m_fPointsSize = 5;			// point size

	m_bShowLINES = true;		// show wireframes
	m_fLinesWidth = 2;			// line width

	m_bShowSOLID = true;		// show faces

	load_from_dir(".", 1);	// load from HOME
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
		QString _type = mp.typeName();
		QString _value = "";
		if (_name == "objectName") continue;
		if (_type == "float") {
			_value = QString("%1").arg(property(mp.name()).toFloat());
			settings->setValue(_name, _value);
		} else if (_type == "QVector4D") {
			QVector4D _v = property(mp.name()).value<QVector4D>();
			_value = QVector4D_to_QString(_v);
			settings->setValue(_name, _value);
		} else if (_type == "QVector3D") {
			QVector3D _v = property(mp.name()).value<QVector3D>();
			_value = QVector3D_to_QString(_v);
			settings->setValue(_name, _value);
		} else {
			qDebug() << _name << mp.typeName();
			settings->setValue(_name, property(mp.name()));
		}
	}
	delete settings;
	return res;
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
		QString _type = mp.typeName();
		if (_name == "objectName") continue;
		if (settings->contains(_name)) {
			if (_type == "QVector4D") {
				QString _s = settings->value(_name).toString();
				setProperty(_name.toLocal8Bit().constData()
							, QString_to_QVector4D(_s));
			} else if (_type == "QVector3D") {
				QString _s = settings->value(_name).toString();
				setProperty(_name.toLocal8Bit().constData()
							, QString_to_QVector3D(_s));
			} else {
				setProperty(_name.toLocal8Bit().constData()
							, settings->value(_name));
			}
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

bool Config::dump_cfg() const
{
	FILE* f = fopen(m_fp_cfg.toLocal8Bit().constData(), "w");
	fclose(f);
	return false;
}

bool Config::is_perspective() const
{
	return m_bIsPerspective;
}
void Config::set_perspective(bool b)
{
	m_bIsPerspective = b;
	m_Changed = true;
}

bool Config::change_window_pos() const
{
	return m_bChgWinPos;
}
void Config::set_change_window_pos(bool b)
{
	m_bChgWinPos = b;
	m_Changed = true;
}
int Config::window_pos_x() const
{
	return m_iWinPosX;
}
void Config::set_window_pos_x(int i)
{
	if (m_bChgWinPos) {
		m_iWinPosX = i;
		m_Changed = true;
	} else {
		qWarning() << "Can't change window_pos_x. "
				   "change_window_pos is false";
	}
}
int Config::window_pos_y() const
{
	return m_iWinPosY;
}
void Config::set_window_pos_y(int i)
{
	if (m_bChgWinPos) {
		m_iWinPosY = i;
		m_Changed = true;
	} else {
		qWarning() << "Can't change window_pos_y. "
				   "change_window_pos is false";
	}
}

bool Config::change_window_size() const
{
	return m_bChgWinSize;
}
void Config::set_change_window_size(bool b)
{
	m_bChgWinSize = b;
	m_Changed = true;
}
uint Config::window_width() const
{
	return m_uiWinWidth;
}
void Config::set_window_width(uint ui)
{
	if (m_bChgWinSize) {
		m_uiWinWidth = ui;
		m_Changed = true;
	} else {
		qWarning() << "Can't change window_width. "
				   "change_window_size is false";
	}
}
uint Config::window_height() const
{
	return m_uiWinHeight;
}
void Config::set_window_height(uint ui)
{
	if (m_bChgWinSize) {
		m_uiWinHeight = ui;
		m_Changed = true;
	} else {
		qWarning() << "Can't change window_height. "
				   "change_window_size is false";
	}
}

QVector4D Config::bg_color() const
{
	return m_bg_color;
}
void Config::set_bg_color(QVector4D& v)
{
	m_bg_color = v;
	m_Changed = true;
}

QString Config::path_to_debuglog() const
{
	return m_fp_debug;
}
void Config::set_path_to_debuglog(const QString& s)
{
	m_fp_debug = s;
	m_Changed = true;
}
QString Config::path_to_fatallog() const
{
	return m_fp_fatal;
}
void Config::set_path_to_fatallog(const QString& s)
{
	m_fp_fatal = s;
	m_Changed = true;
}
QString Config::path_to_renderlog() const
{
	return m_fp_render;
}
void Config::set_path_to_renderlog(const QString& s)
{
	m_fp_render = s;
	m_Changed = true;
}
QString Config::path_to_statisticlog() const
{
	return m_fp_statistic;
}
void Config::set_path_to_statisticlog(const QString& s)
{
	m_fp_statistic = s;
	m_Changed = true;
}
QString Config::path_to_warninglog() const
{
	return m_fp_warning;
}
void Config::set_path_to_warninglog(const QString& s)
{
	m_fp_warning = s;
	m_Changed = true;
}

QString Config::path_to_povdump() const
{
	return m_fp_povdumpbin;
}
void Config::set_path_to_povdump(const QString& s)
{
	m_fp_povdumpbin = s;
	m_Changed = true;
}

QString Config::path_to_dump() const
{
	return m_fp_dump;
}
void Config::set_path_to_dump(const QString& s)
{
	m_fp_dump = s;
	m_Changed = true;
}

QString Config::path_to_cfg() const
{
	return m_fp_cfg;
}
void Config::set_path_to_cfg(const QString& s)
{
	m_fp_cfg = s;
	m_Changed = true;
}

//~ bool m_bIsPerspective;		// perspective camera, else orthographic
//~ bool m_bShowAxis;			// show axis X, Y, Z
//~ float m_fAxisSize;			// axis size
//~ bool m_bShowBBOX;			// show bounding boxes
//~ bool m_bShowPOINTS;			// show points
//~ float m_fPointsSize;		// points size
//~ bool m_bShowLINES;			// show wireframes
//~ float m_fLinesWidth;		// line width
//~ bool m_bShowSOLID;			// show faces

bool Config::show_axis() const
{
	return m_bShowAxis;
}
void Config::set_show_axis(bool b)
{
	m_bShowAxis = b;
	m_Changed = true;
}
float Config::axis_size() const
{
	return m_fAxisSize;
}
void Config::set_axis_size(float f)
{
	m_fAxisSize = f;
	m_Changed = true;
}
bool Config::show_bbox() const
{
	return m_bShowBBOX;
}
void Config::set_show_bbox(bool b)
{
	m_bShowBBOX = b;
	m_Changed = true;
}
bool Config::show_points() const
{
	return m_bShowPOINTS;
}
void Config::set_show_points(bool b)
{
	m_bShowPOINTS = b;
	m_Changed = true;
}
float Config::points_size() const
{
	return m_fPointsSize;
}
void Config::set_points_size(float f)
{
	m_fPointsSize = f;
	m_Changed = true;
}
bool Config::show_lines() const
{
	return m_bShowLINES;
}
void Config::set_show_lines(bool b)
{
	m_bShowLINES = b;
	m_Changed = true;
}
float Config::line_width() const
{
	return m_fLinesWidth;
}
void Config::set_line_width(float f)
{
	m_fLinesWidth = f;
	m_Changed = true;
}
bool Config::show_solid() const
{
	return m_bShowSOLID;
}
void Config::set_show_solid(bool b)
{
	m_bShowSOLID = b;
	m_Changed = true;
}
