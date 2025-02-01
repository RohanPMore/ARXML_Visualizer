#pragma once
#include "arxml_parser/armodel.hpp"
#include "bswmodule.h"
#include <QObject>
#include <filesystem>

class ProjectInfo
{
    Q_GADGET
public:
    ProjectInfo() {}
    
    ProjectInfo(QString name, std::filesystem::path project_file, std::shared_ptr<arx::armodel> model):
        name_(name),
        project_file_(project_file),
        model_(model),
        modules_()
    {
    }

    QString name() const { return name_; };
    std::filesystem::path project_file() const { return project_file_; };
    std::vector<BswModule>& modules() { return modules_; }
    std::shared_ptr<arx::armodel> model() { return model_; }

private:
    std::shared_ptr<arx::armodel> model_;
    std::vector<BswModule> modules_;
    QString name_;
    std::filesystem::path project_file_;
};