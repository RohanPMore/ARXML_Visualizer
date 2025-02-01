#pragma once
#include "arxml_parser/referrable.hpp"
#include "arxml_parser/arpackage.hpp"
#include "arxml_parser/armodel.hpp"
#include "arxml_parser/reference.hpp"

#include "rapidxml/rapidxml.hpp"

#include <map>
#include <functional>
#include <memory>

namespace arx
{

enum type : int
{
    kUnknownTag = -2,
    kPackage =    -1
};

template<class T>
class arxml_parser
{
public:

    arxml_parser(std::shared_ptr<armodel> doc):
        _model(doc)
    {
    }

    void parse(std::shared_ptr<rapidxml::xml_node<> > root)
    {
        for(auto child = root->first_node(); child; child = child->next_sibling())
        {
            parse_node(child, nullptr);
        }
    }

    static void parse_longnames(referrable& ref, rapidxml::xml_node<>* node)
    {
        for(auto child = node->first_node(); child; child = child->next_sibling())
        {
            if(std::string("LONG-NAME") == child->name())
            {
                // Iterate over all L-4 nodes
                for(auto grandchild = child->first_node(); grandchild; grandchild = grandchild->next_sibling())
                {
                    if(std::string("L-4") == grandchild->name())
                    {
                        // Determine the language
                        for(auto attr = grandchild->first_attribute(); attr; attr = attr->next_attribute())
                        {
                            if(std::string("L") == attr->name())
                            {
                                ref.add_longname(grandchild->value(), attr->value());
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

protected:
    inline std::shared_ptr<referrable> create_default_referrable(std::shared_ptr<referrable> parent, const std::string& shortname, const std::string& tagname, std::unique_ptr<std::string> uuid, rapidxml::xml_node<>* node)
    {
        std::shared_ptr<referrable> ref = nullptr;

        if(tagname == std::string("AR-PACKAGE"))
        {
            ref = std::make_shared<package>(_model, shortname, std::move(uuid));
        }
        else
        {
            ref = std::make_shared<referrable>(_model, shortname, std::move(uuid), kUnknownTag);
        }
        
        return ref;
    }

    std::shared_ptr<armodel> _model;
    
private:
    void parse_node(rapidxml::xml_node<>* node, std::shared_ptr<referrable> parent)
    {
        // Check if this looks like a reference (does not have any children, and value starts with '/')
        if(reference::is_reference(node))
        {
            std::shared_ptr<reference> ref = std::make_shared<reference>(_model, parent);
            ref->node(node);
            _model->add_reference(ref);
        }
        else
        {
            // Get shortname of current node.
            std::string shortname;
            bool has_shortname = false;

            for(auto child = node->first_node(); child; child = child->next_sibling())
            {
                if(std::string("SHORT-NAME") == child->name())
                {
                    // This node has a shortname.
                    shortname = child->value();
                    has_shortname = true;
                }
            }

            // Check if this node has an uuid.
            std::unique_ptr<std::string> uuid = nullptr;
            for(auto attr = node->first_attribute(); attr; attr = attr->next_attribute())
            {
                if(std::string("UUID") == attr->name())
                {
                    // This node has an uuid.
                    uuid = std::make_unique<std::string>(attr->value());
                    break;
                }
            }

            std::shared_ptr<referrable> ref = nullptr;

            if(has_shortname)
            {
                // Call the handler to create a new referrable.
                ref = static_cast<T*>(this)->create_referrable(parent, shortname, std::string(node->name()), std::move(uuid), node);

                // Integrate it into the model.
                // If this is a toplevel package, add it to the model.
                if(parent == nullptr)
                {
                    std::shared_ptr<referrable> conflicting_ref = _model->package(shortname);
                    if(conflicting_ref == nullptr)
                    {
                        (void)_model->add_package(ref);
                    }
                    else
                    {
                        if(conflicting_ref->integrate(ref))
                        {
                            // Integrated successfully. Continue with the conflicting ref as the parent.
                            ref = conflicting_ref;
                        }
                    }
                    
                }
                else
                {
                    // Check whether there is a referrable with the same name:
                    std::shared_ptr<referrable> conflicting_ref = parent->child(shortname);
                    if(conflicting_ref == nullptr)
                    {
                        // Parent does not have a child with that name. Add the new one.
                        (void)parent->add_child(ref);
                        ref->parent(parent);
                    }
                    else
                    {
                        // A child with this name already exists. Try to integrate the new child.
                        (void)conflicting_ref->integrate(ref);
                    }
                }
            }

            // Iterate over children.
            for(auto child = node->first_node(); child; child = child->next_sibling())
            {
                // Recurse.
                if(ref == nullptr)
                {
                    parse_node(child, parent);
                }
                else
                {
                    if(std::string("SHORT-NAME") != child->name())
                    {
                        // A new referrable was created from this node, use it as the new parent.
                        parse_node(child, ref);
                    }
                }
            }
        }    
    }
};


}