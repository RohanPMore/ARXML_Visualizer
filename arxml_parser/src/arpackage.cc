#include "arxml_parser/arpackage.hpp"
#include "arxml_parser/arxml_parser.hpp"

#include <memory>

namespace arx
{

package::package(std::shared_ptr<armodel> model, const std::string& shortname, std::unique_ptr<std::string> uuid):
        referrable(model, shortname, std::move(uuid), static_cast<int>(arx::kPackage))
{
}

}