#include "referenceresolvertask.h"

ReferenceResolverTask::ReferenceResolverTask(std::shared_ptr<arx::armodel> model, size_t start, size_t count):
    model_(model),
    start_(start),
    count_(count)
{
    setAutoDelete(false);
}

void ReferenceResolverTask::run()
{
    model_->resolve_references(start_, count_);
    finished_ = true;
}