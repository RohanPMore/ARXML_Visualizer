#pragma once
#include "arxml_parser/referrable.hpp"

#include <memory>

namespace arx
{

class package: public referrable
{
    public:
    package(std::shared_ptr<armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid);
    
    protected:
    virtual bool integrate(std::shared_ptr<referrable> node) override
    {   
        bool success = true;
        // Add all children of the other referrable

        // Lock the foreign node for read access
        node->children_lock().lock_shared();

        for(auto foreign_child: node->children() )
        {
            if(add_or_integrate(foreign_child) == false)
            {
                // At least one of the children could not be added.
                success = false;
            }
        }
        node->children_lock().unlock_shared();
        
        return success;
    }
};

}