#include "EmbGen/Parser/Appendage.hpp"
#include "EmbGen/Parser/Stop.hpp"
#include "EmbGen/Parser/Exceptions.hpp"
#include <tinyxml2.h>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Appendage::Appendage(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
                m_name = getAttribute("name")->Value();

                try
                {
                    m_version = getAttribute("version")->Value();
                }
                catch (AttributeException)
                {
                    m_version = "";
                }

                try
                {
                    m_lib_deps = getAttribute("lib_deps")->Value();
                }
                catch (AttributeException)
                {
                    m_lib_deps = "";
                }

                for (auto include : getElements("include"))
                {
                    m_includes.emplace_back(include);
                }

                for (auto variable : getElements("variable"))
                {
                    m_variables.emplace_back(variable);
                }

                auto setups = getElements("setup");
                if (setups.size() == 1)
                {
                    m_setup = std::make_shared<Setup>(setups.at(0));
                }
                else if (setups.size() > 1)
                {
                    throw ElementException("Too many Setup elements for Appendage '" + m_name + "' on line " + std::to_string(getLineNum()));
                }
                else
                {
                    m_setup = nullptr;
                }

                auto loops = getElements("loop");
                if (loops.size() == 1)
                {
                    m_loop = std::make_shared<Loop>(loops.at(0));
                }
                else if (loops.size() > 1)
                {
                    throw ElementException("Too many Loop elements for Appendage '" + m_name + "' on line " + std::to_string(getLineNum()));
                }
                else
                {
                    m_loop = nullptr;
                }

                for (auto command : getElements("command"))
                {
                    std::shared_ptr<Command> cmd = std::make_shared<Command>(command);
                    m_commands.emplace(cmd->getName(), cmd);
                }

                auto stops = getElements("stop");
                if (stops.size() == 1)
                {
                    std::shared_ptr<Stop> stop = std::make_shared<Stop>(stops.at(0));
                    std::shared_ptr<Code> code = stop->getCode();
                    if (code != nullptr)
                    {
                        m_commands.emplace("Stop", std::make_shared<Command>("Stop", std::vector<Parameter>(), code));
                    }
                    else if (m_commands.find(stop->getCommand()) != std::end(m_commands))
                    {
                        m_commands.emplace("Stop",
                            std::make_shared<Command>("Stop",
                                std::vector<Parameter>(),
                                std::make_shared<Code>("    " + stop->getCommand() + "(i);")));
                    }
                    else
                    {
                        throw AttributeException("Stop element command name '" + stop->getCommand() + "' not found in Appendage '" + m_name + "'");
                    }
                }
                else if (stops.size() > 1)
                {
                    throw ElementException("Too many Stop elements for Appendage '" + m_name + "' on line " + std::to_string(getLineNum()));
                }

                if (!isAttributesEmpty())
                {
                    throw AttributeException("Extra attributes for Appendage on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for Appendage on line " + std::to_string(getLineNum()));
                }
            }
            
            std::string Appendage::getName() const
            {
                return m_name;
            }
            
            std::string Appendage::getVersion() const
            {
                return m_version;
            }
            
            std::string Appendage::getLibDeps() const
            {
                return m_lib_deps;
            }

            std::vector<Include> Appendage::getIncludes() const
            {
                return m_includes;
            }

            std::vector<Variable> Appendage::getVariables() const
            {
                return m_variables;
            }

            std::shared_ptr<Setup> Appendage::getSetup() const
            {
                return m_setup;
            }

            std::shared_ptr<Loop> Appendage::getLoop() const
            {
                return m_loop;
            }

            std::map<std::string, std::shared_ptr<Command>> Appendage::getCommands() const
            {
                return m_commands;
            }
        }
    }
}
