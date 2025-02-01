#include "arxml_parser/armodel.hpp"
#include "arxml_parser/reference.hpp"
#include <iostream>

namespace arx
{

void armodel::resolve_all_references()
{
    std::unordered_map<std::string_view, std::shared_ptr<referrable> > cache;
    for(auto& ref: _references)
    {
        ref.second->resolve();
    }
}

void armodel::resolve_references(size_t start, size_t count)
{
    auto it = std::next(std::begin(_references), start);
    for(int i=0; i < count; i++)
    {
        it->second->resolve();
        it++;
    }
}

size_t armodel::reference_count()
{
    return _references.size();
}

void armodel::update_reference(reference* ref)
{
    if(ref->is_resolved())
    {
        _reference_targets_lock.lock();
        auto it = _reference_targets.find(ref->target().get());
        if(it == _reference_targets.end())
        {
            // The reference target is not contained in the map. Add it
            _reference_targets.insert(std::make_pair(ref->target().get(), std::vector(1, ref->node())));
        }
        else
        {
            it->second.push_back(ref->node());
        }
        _reference_targets_lock.unlock();
    }
}

std::shared_ptr<reference> armodel::get_reference(rapidxml::xml_node<>* node)
{
    _references_lock.lock();
    auto it = _references.find(node);
    if(it == _references.end())
    {
        // No element found. Add a new reference.
        auto ref = std::make_shared<reference>(std::shared_ptr<armodel>(this), nullptr);
        ref->node(node);
        _references.insert(std::make_pair(node, ref));
        _references_lock.unlock();
        return ref;
    }
    else
    {
        _references_lock.unlock();
        return it->second;
    }
}

void armodel::add_reference(std::shared_ptr<reference> ref)
{
    if(ref->node() != nullptr)
    {
        _references_lock.lock();
        _references.insert(std::make_pair(ref->node(), ref));
        _references_lock.unlock();
    }
}

std::vector<std::shared_ptr<reference> > armodel::get_references_to(referrable* refbl)
{
    auto target_it = _reference_targets.find(refbl);
    if(target_it != _reference_targets.end())
    {
        std::vector<std::shared_ptr<reference> > return_value;
        auto refs = target_it->second;
        for(auto ref_node : refs)
        {
            auto ref_it = _references.find(ref_node);
            if(ref_it != _references.end())
            {
                return_value.push_back(ref_it->second);
            }
        }
        return return_value;
    }
    else
    {
        // No element found.
        return std::vector<std::shared_ptr<reference> >();
    }
}

boost::any_range<std::shared_ptr<reference>, boost::forward_traversal_tag, std::shared_ptr<reference>&, std::ptrdiff_t> armodel::references()
{
    return _references | boost::adaptors::map_values;
}

bool armodel::integrate(std::shared_ptr<arx::armodel> other)
{
    if(other == nullptr)
    {
        return false;
    }
    else
    {
        other->_packages_lock.lock_shared();
        for(auto& other_pkg : other->_packages)
        {
            auto own_package = _packages.find(other_pkg.first);
            if(own_package != _packages.end())
            {
                own_package->second->integrate(other_pkg.second);
            }
            else
            {
                _packages_lock.lock();
                _packages[other_pkg.first] = other_pkg.second;
                _packages_lock.unlock();
            }
        }
        other->_packages_lock.unlock_shared();
        
        _references_lock.lock();
        _references.insert(other->_references.begin(), other->_references.end());
        _references_lock.unlock();

        _reference_targets_lock.lock();
        _reference_targets.insert(other->_reference_targets.begin(), other->_reference_targets.end());
        _reference_targets_lock.unlock();

        return true;
    }
}

}