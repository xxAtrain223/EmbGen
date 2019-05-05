#include "EmbGen/Loop.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Loop::Loop(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
            }

            std::vector<Code> Loop::getCode() const
            {
                return std::vector<Code>();
            }
        }
    }
}