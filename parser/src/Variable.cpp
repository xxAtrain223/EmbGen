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
        }
    }
}