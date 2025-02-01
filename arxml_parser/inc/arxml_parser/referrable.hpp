#pragma once

#include "rapidxml/rapidxml.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/any_range.hpp>
#include <shared_mutex>
#include <mutex>
namespace arx
{

// Forward declaration to avoid cyclic includes
class armodel;
class reference;

class referrable
{
    public:
    referrable(std::shared_ptr<armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid, int type):
        _model(model),
        _shortname(shortname),
        _type(type),
        _uuid(std::move(uuid))
    {
    }

    inline int type() { return _type; }

    inline std::shared_ptr<armodel> model() { return _model; }
    
    inline std::string_view shortname() const { return _shortname; }

    std::string path() const;

    bool is_descendant_of(std::shared_ptr<referrable> refbl);

    // Return references to this referrable.
    std::vector<std::shared_ptr<reference> > get_references();

    inline bool has_uuid()
    {
       return _uuid != nullptr;
    }

    inline const std::string& uuid()
    {
        static const std::string empty("");
        if(has_uuid())
        {
            return *_uuid;
        }
        else
        {
            return empty;
        }
    }

    inline void uuid(const std::string& id)
    {
        _uuid = std::make_unique<std::string>(id);
    }

    inline void add_longname(const std::string_view longname, const std::string_view lang)
    {
        _longnames.insert(std::make_pair(lang, longname));
    }

    inline bool has_longname(const std::string_view lang)
    {
        auto it = _longnames.find(lang);
        return it != _longnames.end();
    }

    inline bool has_longname()
    {
        return _longnames.size();
    }

    inline const std::string& longname(const std::string_view lang)
    {
        static const std::string empty("");
        auto it = _longnames.find(lang);
        if(it != _longnames.end())
        {
            return it->second;
        }
        else
        {
            return empty;
        }
    }

    inline const std::string& longname()
    {
        static const std::string empty("");
        if(has_longname())
        {
            return _longnames.begin()->second;
        }
        else
        {
            return empty;
        }
    }

    inline std::shared_ptr<referrable> parent() const
    {
        return _parent.lock();
    }

    inline void parent(std::weak_ptr<referrable> par) 
    {
        _parent = par;
    }

    inline std::shared_mutex& children_lock() { return _children_lock; }

    bool add_or_integrate(std::shared_ptr<referrable> child)
    {
        if(child != nullptr)
        {
            // Acquire shared lock for read access.
            _children_lock.lock_shared();
            auto it = _children.find(child->shortname());
            if(it == _children.end())
            {
                // No element found. Add a new one.
                _children_lock.unlock_shared();
                // Acquire exclusive lock for write access.
                _children_lock.lock();
                _children.insert(std::make_pair(child->shortname(), child));
                _children_lock.unlock();
                return true;
            }
            else
            {
                // Shortname already present in children. Try to integrated (only successful for splittable referrables).
                _children_lock.unlock_shared();
                auto conflicting_ref = it->second;
                return conflicting_ref->integrate(child);
            }
        }
        else
        {
            return false;
        }
    }

    bool add_child(std::shared_ptr<referrable> child)
    {
        if(child != nullptr)
        {
            // Acquire shared lock for read access.
            _children_lock.lock_shared();
            auto it = _children.find(child->shortname());
            if(it == _children.end())
            {
                // No element found. Add a new one.
                _children_lock.unlock_shared();
                // Acquire exclusive lock for write access.
                _children_lock.lock();
                _children.insert(std::make_pair(child->shortname(), child));
                _children_lock.unlock();
                return true;
            }
            else
            {
                // Shortname already present in children. Do nothing. 
                _children_lock.unlock_shared();
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    virtual bool integrate(std::shared_ptr<referrable> node)
    {   
        // By default, a referrable does not integrate the children (this applies only to packages.)
        return false;
    }

    inline size_t children_count() { return _children.size(); }
    
    inline boost::any_range<std::shared_ptr<referrable>, boost::forward_traversal_tag, std::shared_ptr<referrable>&, std::ptrdiff_t> children()
    {
        return _children | boost::adaptors::map_values;
    }

    inline std::shared_ptr<referrable> child(std::string_view shortname)
    {
        auto it = _children.find(shortname);
        if(it != _children.end())
        {
            // Child found
            return it->second;
            
        }
        else
        {
            // No element found.
            return nullptr;
        }
    }

    inline std::shared_ptr<referrable> child(const std::string& shortname)
    {
        auto it = _children.find(std::string_view(shortname));
        if(it != _children.end())
        {
            // Child found
            return it->second;
            
        }
        else
        {
            // No element found.
            return nullptr;
        }
    }
private:
    int _type;
    std::shared_ptr<armodel> _model;
    std::weak_ptr<referrable> _parent;
    std::unordered_map<std::string_view, std::shared_ptr<referrable> > _children;
    std::string _shortname;
    /**
     * Map from language to longname.
     */
    std::unordered_map<std::string_view, std::string > _longnames;
    std::unique_ptr<std::string> _uuid;
    std::shared_mutex _children_lock;
    
};

}