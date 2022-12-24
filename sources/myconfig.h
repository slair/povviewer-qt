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

#include <QDebug>

class Config : public QObject {
	Q_OBJECT
	Q_PROPERTY(int WinPosX READ WinPosX WRITE setWinPosX)
	Q_PROPERTY(int WinPosY READ WinPosY WRITE setWinPosY)
	Q_PROPERTY(unsigned int WinWidth READ WinWidth WRITE setWinWidth)
	Q_PROPERTY(unsigned int WinHeight READ WinHeight WRITE setWinHeight)

private:
	int m_iWinPosX;
	int m_iWinPosY;
	unsigned int m_uiWinWidth;
	unsigned int m_uiWinHeight;

public:
	Config();
	~Config();

	void setWinPosX(int iX) {
		m_iWinPosX = iX;
	}

	int WinPosX() const {
		return m_iWinPosX;
	}

	void setWinPosY(int iY) {
		m_iWinPosY = iY;
	}

	int WinPosY() const {
		return m_iWinPosY;
	}

	void setWinWidth(unsigned int uiW) {
		m_uiWinWidth = uiW;
	}

	unsigned int WinWidth() const {
		return m_uiWinWidth;
	}

	void setWinHeight(unsigned int uiH) {
		m_uiWinHeight = uiH;
	}

	unsigned int WinHeight() const {
		return m_uiWinHeight;
	}

	//~ friend QTextStream &QTextStream::operator << (QTextStream &o, const Config &_cfg);
	friend QDebug operator << (QDebug d, const Config &cfg) {
		d << "Config" << endl;
		const QMetaObject* pmo = cfg.metaObject();
		for (int i=0; i < pmo->propertyCount(); ++i) {
			const QMetaProperty mp = pmo->property(i);
			d << "	Property#:" << i << endl;
			d << "	Type:" << mp.typeName() << endl;
			d << "	Name:" << mp. name() << endl;
			d << "	Value:" << cfg.property(mp.name()) << endl << endl;
		}
		return d;
	}

	//~ operator QString() const {
	//~ QString res;
	//~ res += "Config\x0d\x0a";

	//~ const QMetaObject* pmo = this->metaObject();
	//~ for (int i=0; i < pmo->propertyCount(); ++i) {
	//~ const QMetaProperty mp = pmo->property(i);
	//~ res += "Property#:" + QString::number(i);
	//~ res += "Туре:" + QString(mp.typeName());
	//~ res += "Name:" + QString(mp. name());
	//~ res += "Value:" + this->property(mp.name()).toString();
	//~ }
	//~ return res;
	//~ }
};

#endif // CONFIG_H

