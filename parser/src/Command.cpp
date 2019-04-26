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
        }
    }
}