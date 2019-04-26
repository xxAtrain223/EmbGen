#include "EmbGen/Command.hpp"

namespace emb::gen::parser
{
    Command::Command(const tinyxml2::XMLElement* xml) :
        XmlElement(xml)
    {
    }
}