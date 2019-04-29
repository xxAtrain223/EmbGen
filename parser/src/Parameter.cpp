#include "EmbGen/Parameter.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Parameter::Parameter(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
            }

            std::string Parameter::getType() const
            {
                return std::string();
            }

            std::string Parameter::getName() const
            {
                return std::string();
            }

            tinyxml2::XMLAttribute* Parameter::getMin() const
            {
                return nullptr;
            }

            tinyxml2::XMLAttribute* Parameter::getMax() const
            {
                return nullptr;
            }

            bool Parameter::isCore() const
            {
                return false;
            }
        }
    }
}