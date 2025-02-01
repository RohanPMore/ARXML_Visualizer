#pragma once

#include <QObject>
#include <QtGui/qcolor.h>
#include <QtQml/qqml.h>
#include <QString>
#include <QUrl>
#include "arxml_parser/arxml_file.hpp"
#include "projectinfo.h"

class CppInterface : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(CppInterface)
    QML_SINGLETON

public:
    explicit CppInterface(QObject *parent = nullptr);
    void openEcucFolder(QString path);
    void openEcucFile(QString path);
    void forwardProjectLoaded(ProjectInfo* project);
    Q_INVOKABLE void openDpaFile(QUrl path);


signals:
    void ecucFolderOpened(QString filepath);
    void ecucFileOpened(QString filepath);
    void dpaFileOpened(QString filepath);
    void projectLoaded(ProjectInfo* project);
};
