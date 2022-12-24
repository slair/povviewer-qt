// -*- coding: utf-8 -*-
/****************************************************************************
**
**
**
****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

class Config : public QObject {
	Q_OBJECT
	Q_PROPERTY(int WinPosX READ WinPosX WRITE setWinPosX)
	Q_PROPERTY(int WinPosY READ WinPosY WRITE setWinPosY)
	Q_PROPERTY(size_t WinWidth READ WinWidth WRITE setWinWidth)
	Q_PROPERTY(size_t WinHeight READ WinHeight WRITE setWinHeight)

private:
	int m_iWinPosX;
	int m_iWinPosY;
	size_t m_uiWinWidth;
	size_t m_uiWinHeight;

public:
	Config(QObject* pobj = 0) : QObject(pobj)
		, m_iWinPosX(0)
		, m_iWinPosY(0)
		, m_uiWinWidth(320)
		, m_uiWinHeight(240) {
	}

	void DebugShowProperties() {
		const QMetaObject* pmo = pobj->metaObject();
		for (int i=0; i < pmo->propertyCount(); ++i) {
			const QMetaProperty mp = pmo->property(i);
			qDebug() << "Property#:" << i;
			qDebug() << "Туре:" << mp.typeName();
			qDebug() << "Name:" << mp. name() ;
			qDebug() << "Value:" << pobj->property(mp.name());
		}
	}

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

	void setWinWidth(size_t uiW) {
		m_uiWinWidth = uiW;
	}

	int WinWidth() const {
		return m_uiWinWidth;
	}

	void setWinHeight(size_t uiH) {
		m_uiWinHeight = uiH;
	}

	int WinHeight() const {
		return m_uiWinHeight;
	}

}

#endif // CONFIG_H

