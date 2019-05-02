#include "EmbGen/Code.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Code::Code(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
            }

            Code::Insert Code::getInsert() const
            {
                return Code::Insert::Each;
            }

            std::string Code::getText() const
            {
                return "";
            }
        }
    }
}