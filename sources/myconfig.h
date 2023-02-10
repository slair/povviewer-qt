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

	Q_PROPERTY(QVector4D bg_color READ bg_color
			   WRITE set_bg_color)

	Q_PROPERTY(bool is_perspective READ is_perspective WRITE set_perspective)

	Q_PROPERTY(bool show_axis READ show_axis WRITE set_show_axis)
	Q_PROPERTY(float axis_size MEMBER m_fAxisSize READ axis_size WRITE set_axis_size)
	Q_PROPERTY(bool m_bShowBBOX READ show_bbox WRITE set_show_bbox)
	Q_PROPERTY(bool m_bShowPOINTS READ show_points WRITE set_show_points)
	Q_PROPERTY(float m_fPointsSize READ points_size WRITE set_points_size)
	Q_PROPERTY(bool m_bShowLINES READ show_lines WRITE set_show_lines)
	Q_PROPERTY(float m_fLinesWidth READ line_width WRITE set_line_width)
	Q_PROPERTY(bool m_bShowSOLID READ show_solid WRITE set_show_solid)

	Q_PROPERTY(QVector3D camera_position READ cam_pos WRITE set_cam_pos)
	Q_PROPERTY(QVector3D camera_target READ cam_tgt WRITE set_cam_tgt)
	Q_PROPERTY(float camera_angle READ cam_angle WRITE set_cam_angle)

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
	QVector4D m_bg_color;		// window background color RGBA
	bool m_bIsPerspective;		// perspective camera, else orthographic
	bool m_bShowAxis;			// show axis X, Y, Z
	float m_fAxisSize;			// axis size
	bool m_bShowBBOX;			// show bounding boxes
	bool m_bShowPOINTS;			// show points
	float m_fPointsSize;		// points size
	bool m_bShowLINES;			// show wireframes
	float m_fLinesWidth;		// line width
	bool m_bShowSOLID;			// show faces
	QVector3D m_CamPos;			// camera position <0,0,0>
	QVector3D m_CamTgt;			// camera target <0,0,1>
	QVector3D m_CamDir;			// camera direction <0,0,1>
	QVector3D m_CamRgt;			// camera right 1.33*<1,0,0>
	QVector3D m_CamUp;			// camera right <0,1,0>
	QVector3D m_CamSky;			// camera right <0,1,0>
	float m_CamAngle;			// perspective camera angle	~67.380
	// ( direction_length=0.5*right_length/tan(angle/2) )

public:
	float cam_angle() const;
	void set_cam_angle(float f);
	QVector3D cam_pos() const;
	void set_cam_pos(QVector3D v);
	QVector3D cam_tgt() const;
	void set_cam_tgt(QVector3D v);

	bool change_window_pos() const;
	void set_change_window_pos(bool b);
	int window_pos_x() const;
	void set_window_pos_x(int i);
	int window_pos_y() const;
	void set_window_pos_y(int i);

	bool change_window_size() const;
	void set_change_window_size(bool b);
	uint window_width() const;
	void set_window_width(uint ui);
	uint window_height() const;
	void set_window_height(uint ui);

	QVector4D bg_color() const;
	void set_bg_color(QVector4D& v);

	QString path_to_debuglog() const;
	void set_path_to_debuglog(const QString& s);
	QString path_to_fatallog() const;
	void set_path_to_fatallog(const QString& s);
	QString path_to_renderlog() const;
	void set_path_to_renderlog(const QString& s);
	QString path_to_statisticlog() const;
	void set_path_to_statisticlog(const QString& s);
	QString path_to_warninglog() const;
	void set_path_to_warninglog(const QString& s);

	QString path_to_povdump() const;
	void set_path_to_povdump(const QString& s);

	QString path_to_dump() const;
	void set_path_to_dump(const QString& s);

	QString path_to_cfg() const;
	void set_path_to_cfg(const QString& s);

	bool is_perspective() const;
	void set_perspective(bool b);

	bool show_axis() const;
	void set_show_axis(bool b);
	float axis_size() const;
	void set_axis_size(float f);
	bool show_bbox() const;
	void set_show_bbox(bool b);
	bool show_points() const;
	void set_show_points(bool b);
	float points_size() const;
	void set_points_size(float f);
	bool show_lines() const;
	void set_show_lines(bool b);
	float line_width() const;
	void set_line_width(float f);
	bool show_solid() const;
	void set_show_solid(bool b);

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

