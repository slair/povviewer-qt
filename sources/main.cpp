/****************************************************************************
**
**
**
****************************************************************************/

#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDir>
#include <QFile>

#include "povviewer.h"
#include "myconfig.h"
#include "glwidget.h"
#include "mainwindow.h"

static Config cfg;

bool parse_scene_for_config(const QString &scenepath, Config &_cfg)
{
	qDebug() << scenepath;
	qDebug() << _cfg;
	return false;
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QCoreApplication::setApplicationName("POV-Ray Viewer");
	QCoreApplication::setOrganizationName("slairium");
	QCoreApplication::setApplicationVersion(POVVIEWER_VERSION_STR);

	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	//~ QCommandLineOption multipleSampleOption("multisample", "Multisampling");
	//~ parser.addOption(multipleSampleOption);
	//~ QCommandLineOption coreProfileOption("coreprofile", "Use core profile");
	//~ parser.addOption(coreProfileOption);

	parser.addPositionalArgument("filename"
								 , QCoreApplication::translate("main"
										 , "The file to open(.pov / .dump)."));

	parser.process(app);

	qDebug() << "Working dir" << QDir::currentPath();

	QString filename;

	const QStringList args = parser.positionalArguments();
	if (args.isEmpty()) {
		if (QFile::exists(POVVIEWER_DEFAULT_POV_SCENE_NAME)) {
			filename = POVVIEWER_DEFAULT_POV_SCENE_NAME;
		} else {
			return 0;
		}
	} else {
		filename = args.first();
	}
	QFileInfo fi_scene{filename};
	qDebug() << "Scene file name" << fi_scene.absoluteFilePath();

	parse_scene_for_config(fi_scene.absoluteFilePath(), cfg);

	return 0;
	QSurfaceFormat fmt;
	fmt.setDepthBufferSize(24);
	//~ if (parser.isSet(multipleSampleOption))
	//~ fmt.setSamples(4);
	//~ if (parser.isSet(coreProfileOption)) {
	//~ fmt.setVersion(3, 2);
	//~ fmt.setProfile(QSurfaceFormat::CoreProfile);
	//~ }
	QSurfaceFormat::setDefaultFormat(fmt);

	//~ MainWindow mainWindow;

	//~ GLWidget::setTransparent(parser.isSet(transparentOption));
	//~ if (GLWidget::isTransparent()) {
	//~ mainWindow.setAttribute(Qt::WA_TranslucentBackground);
	//~ mainWindow.setAttribute(Qt::WA_NoSystemBackground, false);
	//~ }
	//~ mainWindow.resize(mainWindow.sizeHint());
	//~ int desktopArea = QApplication::desktop()->width() *
					  //~ QApplication::desktop()->height();
	//~ int widgetArea = mainWindow.width() * mainWindow.height();
	//~ if (((float)widgetArea / (float)desktopArea) < 0.75f)
	//~ mainWindow.show();
	//~ else
	//~ mainWindow.showMaximized();

	return app.exec();
}
