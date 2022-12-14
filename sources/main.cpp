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
#include <QDateTime>

#include "povviewer.h"
#include "myconfig.h"
#include "glwidget.h"
#include "mainwindow.h"

static Config cfg;

/*
void verboseMessageHandler(QtMsgType type, const QMessageLogContext &context,
						   const QString &msg)
{
	static const char* typeStr[] = {"   DEBUG", " WARNING",
									"CRITICAL", "   FATAL"
								   };

	if(type <= QtFatalMsg) {

#ifdef MS_WINDOWS
		// Установка кодека для нормальной работы консоли
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP 866"));
#endif

		QByteArray localMsg = msg.toLocal8Bit();

		//~ QString contextString(QStringLiteral("%1:%2: %3")
		//~ .arg(context.file)
		//~ .arg(context.line)
		//~ .arg(context.function));
		QString contextString(QStringLiteral("%1:%2: ")
							  .arg(context.file)
							  .arg(context.line));

		QString timeStr(QDateTime::currentDateTime()
						.toString("dd-MM-yy HH:mm:ss:zzz"));

		//~ std::cerr << timeStr.toLocal8Bit().constData() << " - "
		//~ if (type>0 || cfg_debug) {
		if (type > 0) {
			std::cout << contextString.toLocal8Bit().constData()
					  << typeStr[type] << " "
					  //~ << timeStr.toLocal8Bit().constData() << " "
					  << localMsg.constData() << std::endl;
			//~ std::cout.flush();
		}
#ifdef MS_WINDOWS
		// Установка кодека для нормальной работы локали
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP 1251"));
#endif
		if(type == QtFatalMsg) {
			abort();
		}
	}
}
*/

void myMessageOutput(QtMsgType type, const QMessageLogContext &context
					 , const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
#ifdef DEBUG_CONTEXT
	const char *file = context.file ? context.file : "";
	const char *function = context.function ? context.function : "";
#endif
	const char* msg_type;
	switch (type) {
	case QtDebugMsg:
		msg_type = "   DEBUG";
		break;
	case QtInfoMsg:
		msg_type = "    INFO";
		break;
	case QtWarningMsg:
		msg_type = " WARNING";
		break;
	case QtCriticalMsg:
		msg_type = "CRITICAL";
		break;
	case QtFatalMsg:
		msg_type = "   FATAL";
		break;
	}
#ifdef DEBUG_CONTEXT
	fprintf(stderr, "%s:%u: %s\n%s: %s\n", file, context.line, function
			, msg_type, localMsg.constData());
#else
	fprintf(stderr, "%s: %s\n", msg_type, localMsg.constData());
#endif
}

int main(int argc, char *argv[])
{
#ifdef NDEBUG
	qDebug("DEBUG is OFF");
#else
	qDebug("DEBUG is ON");
	qInstallMessageHandler(myMessageOutput);
#endif

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

	// fixed: 1. get scene file folder
	qDebug() << "Scene file dir" << fi_scene.absolutePath();

	// todo: 2. chdir to scene file if workdir!=scenedir
	if (QDir::currentPath() != fi_scene.absolutePath()) {

	}

	qDebug() << "Working dir" << QDir::currentPath();
	qDebug() << "Home dir" << QDir::homePath();
	qDebug() << "Temporary dir" << QDir::tempPath();

	// todo: 3. check for local config

	// scan scene file
	cfg.scan_scene_file(fi_scene.absoluteFilePath());

	// todo: 3. create scene object
	//~ Scene* scene = new Scene(cfg, fi_scene.absoluteFilePath());

	//~ delete scene;
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
