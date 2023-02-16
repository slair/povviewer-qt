// -*- coding: utf-8 -*-
/** @file main.cpp
 ** Main source. Entry point.
 **/

#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDir>
#include <QDateTime>

#include <vld.h>

#include "povviewer.h"
#include "myconfig.h"
#include "pov_scene.h"
#include "glwidget.h"
#include "mainwindow.h"

static QString ATTENTION_CHARS {"-+!><"};
static QString AC;

#pragma warning(disable : 4100)
void myMessageOutput(QtMsgType type, const QMessageLogContext &context
					 , const QString &msg)
#pragma warning(default : 4100)
{
	QByteArray localMsg = msg.toLocal8Bit();
#ifdef DEBUG_CONTEXT
	const char *file = context.file ? context.file : "";
#pragma warning(disable : 4189)
	const char *function = context.function ? context.function : "";
#pragma warning(default : 4189)
#endif
	if (ATTENTION_CHARS.contains(msg[0])) {
		AC = msg[0];
		localMsg = msg.right(msg.length() - 1).toLocal8Bit();
	} else {
		AC = " ";
	}
	QString msg_type;
	switch (type) {
	case QtDebugMsg:
		msg_type = AC + "   DEBUG";
		break;
	case QtInfoMsg:
		msg_type = AC + "    INFO";
		break;
	case QtWarningMsg:
		msg_type = "!WARNING";
		break;
	case QtCriticalMsg:
		msg_type = "!CRITICAL";
		break;
	case QtFatalMsg:
		msg_type = "!   FATAL";
		break;
	}
	if (localMsg != "QOpenGLFramebufferObject: Unsupported "
		"framebuffer format.") {
//~ #ifdef DEBUG_CONTEXT
#if DEBUG_CONTEXT == 1
		fprintf(stderr, "%s: %s\t\t\t\t\t\t\t\t\t\t\t\tFile \"%s\", line %u\n"
				, msg_type.toLocal8Bit().constData()
				, localMsg.constData(), file, context.line);
#elif DEBUG_CONTEXT == 2
		fprintf(stderr, "%s: %s\n", msg_type.toLocal8Bit().constData()
				, localMsg.constData());
		fprintf(stderr, "%s:%u: %s\n%s: %s\n\n", file, context.line, function
				, msg_type.toLocal8Bit().constData(), localMsg.constData());
#else
		fprintf(stderr, "%s: %s\n", msg_type.toLocal8Bit().constData()
				, localMsg.constData());
#endif
//~ #endif
	}
}

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

int main(int argc, char *argv[])
{
#ifdef NDEBUG
	qDebug("DEBUG is OFF");
#else
	qDebug("DEBUG is ON");
	qInstallMessageHandler(myMessageOutput);
#endif

	qDebug() << "Working dir" << QDir::currentPath();
	qDebug() << "Home dir" << QDir::homePath();
	qDebug() << "Temporary dir" << QDir::tempPath();

	QApplication* app = new QApplication(argc, argv);

	Config* cfg = new Config();

	QCoreApplication::setApplicationName("povviewer-qt");
	QCoreApplication::setOrganizationName("povviewer");
	QCoreApplication::setApplicationVersion(POVVIEWER_VERSION_STR);

	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();

	parser.addPositionalArgument("filename"
								 , QCoreApplication::translate("main"
										 , "The file to open(.pov / .dump)."));

	parser.process(*app);

	QString filename;

	const QStringList args = parser.positionalArguments();
	if (args.isEmpty()) {
		if (QFile::exists(POVVIEWER_DEFAULT_POV_SCENE_NAME)) {
			filename = POVVIEWER_DEFAULT_POV_SCENE_NAME;
		} else {
			qCritical().nospace() << "No '"
								  << POVVIEWER_DEFAULT_POV_SCENE_NAME
								  << "' found";
			return 0;
		}
	} else {
		filename = args.first();
	}
	QFileInfo fi_scene{filename};
	qDebug() << "Scene file name" << fi_scene.absoluteFilePath();

	// done:   1. get scene file folder
	qDebug() << "Scene file dir" << fi_scene.absolutePath();

	// done:   2. chdir to scene file if workdir!=scenedir
	if (QDir::currentPath() != fi_scene.absolutePath()) {
		if (!QDir::setCurrent(fi_scene.absolutePath())) {
			qCritical() << "Cannot change directory to '"
						<< fi_scene.absolutePath() << "'";
		}
	}

	// done:   5. create scene object
	pov_Scene* scene = new pov_Scene(cfg, fi_scene.fileName());
	qDebug() << "scene really readed";
	qDebug() << *scene;
	qDebug() << "scene printed";

	QSurfaceFormat fmt;
	fmt.setDepthBufferSize(24);
	//~ fmt.setSamples(4);
	//~ fmt.setVersion(3, 2);
	//~ fmt.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(fmt);

	// done:   6. create window
	MainWindow* main_window = new MainWindow(scene);

	// done:   7. show window
	main_window->show();

	int exitcode = app->exec();

	delete main_window;
	main_window = nullptr;
	qDebug() << "main_window deleted";

	delete scene;
	scene = nullptr;
	qDebug() << "scene deleted";

	delete cfg;
	cfg = nullptr;
	qDebug() << "cfg deleted";

	//~ qDebug() << "delete app";
	//~ delete app;
	//~ qDebug() << "app deleted";
	//~ _CrtDumpMemoryLeaks();
	return exitcode;

	//~ QSurfaceFormat fmt;
	//~ fmt.setDepthBufferSize(24);
	//~ if (parser.isSet(multipleSampleOption))
	//~ fmt.setSamples(4);
	//~ if (parser.isSet(coreProfileOption)) {
	//~ fmt.setVersion(3, 2);
	//~ fmt.setProfile(QSurfaceFormat::CoreProfile);
	//~ }
	//~ QSurfaceFormat::setDefaultFormat(fmt);

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

	//~ return app.exec();
}
