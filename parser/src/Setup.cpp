#include "EmbGen/Setup.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Setup::Setup(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
            }

            std::vector<Code> Setup::getCode() const
            {
                return std::vector<Code>();
            }
        }
    }
}