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
        }
    }
}