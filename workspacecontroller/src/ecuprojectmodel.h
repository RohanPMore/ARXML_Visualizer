#pragma once
#include <QtQml/qqml.h>
#include <QObject>
#include <QString>
#include <QVector>

#include "bswmodule.h"

#include "bswmd_parser.hpp"
#include "ecuc_parser.hpp"
#include "arxml_parser/armodel.hpp"
#include "arxml_parser/arxml_file.hpp"
#include "projectinfo.h"

#include <vector>

class EcuProjectModel
{
public:
    explicit EcuProjectModel(ProjectInfo* projectInfo);
    QString name() const;
    std::vector<BswModule>& modules();
    std::shared_ptr<arx::armodel> model();
private:
    ProjectInfo projectInfo_;
    std::shared_ptr<arx::armodel> model_;
};