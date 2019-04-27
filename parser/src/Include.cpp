#include "EmbGen/Include.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Include::Include(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
            }

            std::string Include::getValue() const
            {
                return "";
            }

            bool Include::isStandard() const
            {
                return false;
            }
        }
    }
}