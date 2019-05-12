#include "EmbGen/Appendage.hpp"

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Appendage::Appendage(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
            }
            
            std::string Appendage::getName() const
            {
                return "";
            }
            
            std::string Appendage::getVersion() const
            {
                return "";
            }
            
            std::string Appendage::getLibDeps() const
            {
                return "";
            }

            std::vector<Include> Appendage::getIncludes() const
            {
                return std::vector<Include>{};
            }

            std::vector<Variable> Appendage::getVariables() const
            {
                return std::vector<Variable>{};
            }

            std::shared_ptr<Setup> Appendage::getSetup() const
            {
                return nullptr;
            }

            std::shared_ptr<Loop> Appendage::getLoop() const
            {
                return nullptr;
            }

            std::map<std::string, Command> Appendage::getCommands() const
            {
                return std::map<std::string, Command>{};
            }
        }
    }
}
