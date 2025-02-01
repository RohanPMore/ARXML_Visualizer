#pragma once

#include "parameter.h"

#include <QObject>
//#include <QtQml/qqmlregistration.h>

#include <memory>
#include <vector>

class ModuleEditor : public QObject
{
    Q_OBJECT
public:
    explicit ModuleEditor(QObject *parent = nullptr);

    std::vector<std::unique_ptr<Parameter> >& items();

    bool setItemAt(int index, const Parameter &item);
signals:

public slots:

private:
    std::vector<std::unique_ptr<Parameter> > _items;
};