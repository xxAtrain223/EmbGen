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

            std::string Parameter::getType() const
            {
                return std::string();
            }

            std::string Parameter::getName() const
            {
                return std::string();
            }

            bool Parameter::isCore() const
            {
                return false;
            }

            bool Parameter::read(const tinyxml2::XMLAttribute* attr, uint8_t& val) const
            {
                return false;
            }

            bool Parameter::read(const tinyxml2::XMLAttribute* attr, uint16_t& val) const
            {
                return false;
            }

            bool Parameter::read(const tinyxml2::XMLAttribute* attr, uint32_t& val) const
            {
                return false;
            }

            bool Parameter::read(const tinyxml2::XMLAttribute* attr, uint64_t& val) const
            {
                return false;
            }

            bool Parameter::read(const tinyxml2::XMLAttribute* attr, int8_t& val) const
            {
                return false;
            }

            bool Parameter::read(const tinyxml2::XMLAttribute* attr, int16_t& val) const
            {
                return false;
            }

            bool Parameter::read(const tinyxml2::XMLAttribute* attr, int32_t& val) const
            {
                return false;
            }

            bool Parameter::read(const tinyxml2::XMLAttribute* attr, int64_t& val) const
            {
                return false;
            }

            bool Parameter::read(const tinyxml2::XMLAttribute* attr, float& val) const
            {
                return false;
            }
        }
    }
}