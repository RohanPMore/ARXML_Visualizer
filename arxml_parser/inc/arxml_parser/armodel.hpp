#pragma once

#include "arxml_parser/referrable.hpp"

#include <unordered_map>
#include <memory>
#include <string>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/any_range.hpp>
#include <iostream>

namespace arx
{

// Forward declaration to avoid cyclic includes
class reference;

class armodel
{
    public:
    inline bool add_package(std::shared_ptr<referrable> pkg)
    {
        _packages_lock.lock();
        auto it = _packages.find(std::string_view(pkg->shortname()));
        if(it == _packages.end())
        {
            // No element found. Add a new one.
            _packages.insert(std::make_pair(std::string_view(pkg->shortname()), pkg));
            _packages_lock.unlock();
            return true;
        }
        else
        {
            // Shortname already present in children. Do nothing. 
            _packages_lock.unlock();
            return false;
        }
    }

    bool integrate(std::shared_ptr<arx::armodel> other);

    void resolve_all_references();

    void resolve_references(size_t start, size_t count);

    size_t reference_count();

    void update_reference(reference* ref);

    void add_reference(std::shared_ptr<reference> ref);

    std::vector<std::shared_ptr<reference> > get_references_to(referrable* refbl);

    std::shared_ptr<reference> get_reference(rapidxml::xml_node<>* node);

    inline std::vector<std::shared_ptr<referrable> > filter_nodes_type(int type)
    {
        std::vector<std::shared_ptr<referrable> > output;

        std::function<void(std::shared_ptr<referrable>, std::vector<std::shared_ptr<referrable> >&)> find_nodes;
        find_nodes = [type, &find_nodes](std::shared_ptr<referrable> refbl, std::vector<std::shared_ptr<referrable> >& out)->void
        {
            if(refbl->type() == type)
            {
                out.push_back(refbl);
            }
            // Recurse
            for(auto child: refbl->children())
            {
                find_nodes(child, out);
            }
        };

        for(auto pkg : packages())
        {
            find_nodes(pkg, output);
        }

        return output;
    }

    inline std::shared_ptr<referrable> node_by_uuid(const std::string& uuid) const
    {
        // TODO : Recursivly search in referrables for the referrable.
        return nullptr;
    } 

    inline std::shared_ptr<referrable> node_by_path(const std::string_view& path) 
    {
        // Delimiter
        const char kDel = '/';
        std::string::size_type del_pos = path.find(kDel);
        std::string::size_type namestart = 0;
        std::string::size_type first_namestart = 0;

        // Ignore first slash at the beginning (if there is one)
        if(del_pos == 0)
        {
            namestart = 1;
            first_namestart = namestart;
            del_pos = path.find(kDel, 1);
        }
        
        std::string_view shortname = path.substr(namestart, del_pos-1);

        _packages_lock.lock_shared();
        auto it = _packages.find(shortname);
        if(it != _packages.end())
        {
            _packages_lock.unlock_shared();
            std::shared_ptr<referrable> refr = it->second;
            namestart = del_pos+1;

            while(del_pos < path.length() - 1)
            {
                del_pos = path.find(kDel, del_pos+1);
                if(del_pos != std::string_view::npos)
                {
                    shortname = path.substr(namestart, del_pos-namestart);
                    refr = refr->child(shortname);
                    if(refr == nullptr)
                    {
                        // Could not find node with that path!
                        return nullptr;
                    }
                    namestart = del_pos+1;
                }
                else
                {
                    // No more delimiters in the string.
                    shortname = path.substr(namestart);
                    return refr->child(shortname); 
                }
            }
        }
        else
        {
            // No package found.
            return nullptr;
        }
        
        return nullptr;
    }

    inline boost::any_range<std::shared_ptr<referrable>, boost::forward_traversal_tag, std::shared_ptr<referrable>&, std::ptrdiff_t> packages()
    {
        return _packages | boost::adaptors::map_values;
    }

    boost::any_range<std::shared_ptr<reference>, boost::forward_traversal_tag, std::shared_ptr<reference>&, std::ptrdiff_t> references();

    inline std::shared_ptr<referrable> package(const std::string& shortname)
    {
        _packages_lock.lock_shared();
        auto it = _packages.find(std::string_view(shortname));
        if(it != _packages.end())
        {
            _packages_lock.unlock_shared();
            return it->second;
        }
        else
        {
            // No package found.
            _packages_lock.unlock_shared();
            return nullptr;
        }
    }

    private:
    static constexpr int _min_cache_level = 2;
    static constexpr int _max_cache_level = 4;

     // Map from the shortname of a package to the package.
    std::unordered_map<std::string_view, std::shared_ptr<referrable> > _packages;

    // Map from the xml_node representing the reference (the node containing the path) to the parsed reference. 
    std::unordered_map<rapidxml::xml_node<>*, std::shared_ptr<reference> > _references;

    // Map from the target referrable to the references pointing to it
    std::unordered_map<referrable*, std::vector<rapidxml::xml_node<>*> > _reference_targets;

    std::shared_mutex _packages_lock;
    std::shared_mutex _references_lock;
    std::shared_mutex _reference_targets_lock;
};

}