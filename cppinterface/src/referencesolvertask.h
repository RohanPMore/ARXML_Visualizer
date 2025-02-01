#pragma once

#include "rapidxml/rapidxml.hpp"
#include "arxml_parser/armodel.hpp"
#include <QRunnable>
#include <future>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <atomic>

class ReferenceResolverTask : public QRunnable
{
public:
    ReferenceResolverTask(std::shared_ptr<arx::armodel> model, size_t start, size_t count);
    std::atomic_bool& finished() { return finished_; }
    void run() override;

private:
    std::shared_ptr<arx::armodel> model_;
    size_t start_;
    size_t count_;
    std::atomic_bool finished_;
};
