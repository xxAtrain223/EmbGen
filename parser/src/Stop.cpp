#include "EmbGen/Stop.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Stop::Stop(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
            }
            
            std::string Stop::getCommand() const
            {
                return "";
            }
            
            std::shared_ptr<Code> Stop::getCode() const
            {
                return nullptr;
            }
        }
    }
}