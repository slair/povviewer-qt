// -*- coding: utf-8 -*-
/****************************************************************************
**
**
**
****************************************************************************/

#include "myconfig.h"

Config::~Config()
{
}

Config::Config() : m_bChgWinPos(false), m_iWinPosX(80), m_iWinPosY(80)
	, m_bChgWinSize(true), m_uiWinWidth(320), m_uiWinHeight(240)
	, m_bSaveLocalCFG(true), m_bLoadLocalCFG(true), m_bSaveHomeCFG(true)
{
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
			}
		}
	}
	//~ qDebug() << *this;
	return true;
}
