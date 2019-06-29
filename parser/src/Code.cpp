#include "EmbGen/Parser/Code.hpp"
#include <tinyxml2.h>
#include "EmbGen/Parser/Exceptions.hpp"

#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <limits>

namespace emb
{
    namespace gen
    {
        namespace parser
        {
            Code::Code(const tinyxml2::XMLElement* xml) :
                XmlElement(xml)
            {
                std::string insert = "";

                try
                {
                    insert = getAttribute("insert")->Value();
                }
                catch (AttributeException)
                {
                    m_insert = Insert::Each;
                }

                if (insert == "each")
                {
                    m_insert = Insert::Each;
                }
                else if (insert == "once")
                {
                    m_insert = Insert::Once;
                }
                else if (!insert.empty())
                {
                    throw AttributeException("Invalid 'insert' attribute on line " + std::to_string(getLineNum()));
                }

                m_text = processCode(XmlElement::getText());

                if (!isAttributesEmpty())
                {
                    throw AttributeException("Extra attributes for Code on line " + std::to_string(getLineNum()));
                }

                if (!isElementsEmpty())
                {
                    throw ElementException("Extra elements for Code on line " + std::to_string(getLineNum()));
                }
            }

            Code::Code(std::string text, Insert insert) :
                m_text(text), m_insert(insert)
            {
            }

            Code::Insert Code::getInsert() const
            {
                return m_insert;
            }

            std::string Code::getText() const
            {
                return m_text;
            }

            std::string Code::processCode(const std::string& text)
            {
                std::string rv = "";
                std::vector<std::string> lines;

                std::istringstream input(text);
                std::regex blankLine("^\\s*$"), leadingSpaces("^(\\s*)");
                std::smatch match;
                size_t whitespace_count = std::numeric_limits<size_t>::max();

                // Lambda for finding and replacing substrings in a string
                auto replace = [](std::string& str, const std::string& find_str, const std::string& replace_str) {
                    for (size_t find_pos = str.find(find_str); find_pos != std::string::npos; find_pos = str.find(find_str))
                    {
                        str.replace(find_pos, find_str.size(), replace_str);
                    }
                    return str;
                };

                for (std::string line; std::getline(input, line);)
                {
                    // If the string is not empty or whitespace
                    if (!std::regex_match(line, blankLine))
                    {
                        // Replace tabs with 4 spaces
                        lines.emplace_back(replace(line, "\t", "    "));

                        // Find the minimum number of whitspaces
                        std::regex_search(line, match, leadingSpaces);
                        size_t line_whitespace = match.str(1).size();

                        if (line_whitespace < whitespace_count)
                        {
                            whitespace_count = line_whitespace;
                        }
                    }
                }

                // If lines is empty, return empty string
                if (lines.empty())
                {
                    return "";
                }

                // Combine lines into one string
                for (std::string& line : lines)
                {
                    rv += "    " + line.substr(whitespace_count) + "\n";
                }

                rv.pop_back();

                return rv;
            }
        }
    }
}