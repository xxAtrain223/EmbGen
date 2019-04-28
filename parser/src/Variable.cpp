#include "EmbGen/Variable.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Variable::Variable(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
            }
            std::string Variable::getType() const
            {
                return "";
            }
            std::string Variable::getName() const
            {
                return "";
            }
            bool Variable::isCore() const
            {
                return false;
            }
            std::vector<Parameter> Variable::getParameters() const
            {
                return std::vector<Parameter>();
            }
        }
    }
}