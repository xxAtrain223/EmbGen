#include "EmbGen/Command.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Command::Command(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
            }

            std::string Command::getName() const
            {
                return "";
            }

            std::vector<Parameter> Command::getParameters() const
            {
                return std::vector<Parameter>();
            }
            
            std::shared_ptr<Code> Command::getCode() const
            {
                return nullptr;
            }
        }
    }
}