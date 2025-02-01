#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

//#include "moduleeditor.h"
#include "moduleeditormodel.h"
#include "bswmd_parser.hpp"
#include "ecuc_parser.hpp"
#include "arxml_parser/armodel.hpp"
#include "arxml_parser/arxml_file.hpp"
#include "cppinterface.h"
#include "projectloadertask.h"
#include "dpa_file.h"
#include "projectinfo.h"
 
#include <QtQml/qqmlextensionplugin.h>
#include <iostream>
#include <filesystem>
#include <QThreadPool>
#include <QDateTime>
#include <chrono>


Q_IMPORT_QML_PLUGIN(ModuleEditorPlugin)

int main(int argc, char *argv[])
{
    ProjectLoaderTask* loader = nullptr;
    if(argc > 1)
    {
        std::filesystem::path path(argv[1]);
        if(std::filesystem::is_directory(path))
        {
            // Not supported at the moment.
        }
        else
        {
            QString filepath = QString::fromUtf8(argv[1]);
            if(filepath.endsWith("dpa", Qt::CaseInsensitive))
            {
                //singleton->openDpaFile(filepath);
                dpa_file dpa(filepath.toStdString());
                loader = new ProjectLoaderTask(dpa.name(), dpa.sip_path(), dpa.ecuc_files(), dpa.dpa_path());
                QThreadPool::globalInstance()->start(loader);
            }
            else
            {
                // Not supported at the moment.
            }
        }
    }
    
    
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    //engine.rootContext()->setContextProperty(QStringLiteral("moduleEditor"), &config);
    const QUrl url(QStringLiteral("qrc:/config_main/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.addImportPath(":/AsrConfModules/");
    engine.load(url);
    if(argc > 1)
    {
        std::filesystem::path path(argv[1]);
        if(std::filesystem::is_directory(path))
        {
            // Not supported at the moment.
        }
        else
        {
            QString filepath = QString::fromUtf8(argv[1]);
            if(filepath.endsWith("dpa", Qt::CaseInsensitive))
            {
                CppInterface *singleton = engine.singletonInstance<CppInterface*>("CppInterface", "CppInterface");
        
                using namespace std::chrono_literals;
                if(loader->result_future().wait_for(120s) == std::future_status::ready)
                {
                    ProjectInfo* proj = loader->result_future().get();
                    singleton->forwardProjectLoaded(proj);
                }
                else
                {
                    // Error - Loader did not finish in 2 minutes
                    std::cout << "Error: Could not load project" << std::endl;
                }
            }
            else
            {
                // Not supported at the moment.
            }
        }
    }
    return app.exec();
}
