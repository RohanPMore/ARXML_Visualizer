#include "arxml_parser/referrable.hpp"
#include "arxml_parser/armodel.hpp"

#include <string>

namespace arx
{

std::string referrable::path() const
{
    const referrable* node = this;
    std::string p;
    while(node != nullptr)
    {
        p.insert(0, node->shortname());
        p.insert(0, "/");
        node = node->_parent.lock().get();
    }
    return p;
}

bool referrable::is_descendant_of(std::shared_ptr<referrable> refbl)
{  
    std::shared_ptr<referrable> node = this->parent();
    while(node != nullptr)
    {
        if(node == refbl)
        {
            return true;
        }
        node = node->parent();
    }

    return false;
}

std::vector<std::shared_ptr<reference> > referrable::get_references()
{
    return _model->get_references_to(this);
}

}