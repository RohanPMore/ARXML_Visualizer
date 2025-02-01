#pragma once

#include "arxml_parser/referrable.hpp"
#include "arxml_parser/armodel.hpp"
#include "rapidxml/rapidxml.hpp"

#include <memory>
#include <string>
#include <optional>


namespace arx
{
    
class reference
{
    public:

    reference(std::shared_ptr<armodel> model, std::shared_ptr<referrable> parent):
    _model(model),
    _parent(parent),
    _target(nullptr)
    {
    }

    inline std::string_view targetpath() const { return _targetpath; }

    inline void targetpath(const std::string& path) { _targetpath = path; }

    inline bool is_resolved() const
    {
        return _target != nullptr;
    }

    inline rapidxml::xml_node<>* node() const { return _node; }
    
    inline void node(rapidxml::xml_node<>* node)
    {
        _node = node;
        _targetpath = node->value();
    }
     
    inline std::shared_ptr<referrable> target() const { return _target; }

    inline void resolve()
    {
        _target = _model->node_by_path(_targetpath);
        _model->update_reference(this);
    }

    inline static bool is_reference(const rapidxml::xml_node<>* node)
    {
        // Check if the node has a child
        if(!node->first_node())
        {
            // Check if it has a value.
            if(node->value_size() > 0)
            {
                // Check if the first character is a "/"
                if(node->value()[0] == '/')
                {
                    return true;
                }
            }
        }

        return false;
    }

    private:
    rapidxml::xml_node<>* _node;
    std::string_view _targetpath;
    std::shared_ptr<armodel> _model;
    std::shared_ptr<referrable> _target;
    std::shared_ptr<referrable> _parent;
};

}
