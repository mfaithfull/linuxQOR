

#ifndef QOR_PP_H_APP_PARSEDARGSOBJECT
#define QOR_PP_H_APP_PARSEDARGSOBJECT

#include "arguments.h"

namespace qor{ namespace app {

    /// @brief Class that carries result of real parsing
    /// Indicates if parsing is successful and allows to get ParsedArgument object in that case.
    /// In case of parsing failure provides error string for first error.
    class ParsedArgsObject
    {
    public:
        /// @brief Indicates if parsing was successful
        /// @return bool value
        bool IsArgValid() const
        {
            return m_isValid;
        }

        /// @brief Function to get error message in case of parsing failure.
        /// @return string with error message
        const std::string& GetErrorString()
        {
            return m_error;
        }

        /// @brief I don't know when you could need this info
        /// @return size_t count of successfully parsed arguments
        size_t ParsedArgsCount() const
        {
            return m_parsed.size();
        }

        /// @brief Getter function to get ParsedArgument object 
        /// @param name name of argument object, could be short name, long name or positional name
        /// @return Empty argument if argument with given name does not exist or real result if exists.
        ParsedArgument GetArg(const std::string& name)
        {
            ParsedArgument arg = ParsedArgument();
            arg.m_exists = false;
            arg.m_count = 0;

            const auto position = m_names.find(name);
            if (position == m_names.end())
            {
                return arg;
            }

            const auto argument = m_parsed.find(position->second);
            if (argument == m_parsed.end())
            {
                return arg;
            }

            return argument->second;
        }

    private:

        ParsedArgsObject() {}

        void SetValid()
        {
            m_isValid = true;
        }

        void SetErrorString(const std::string& error)
        {
            m_parsed.clear();
            m_error = error;
        }

        /// @brief Internal function to fill argument if default is argument absent
        /// @param argObj argument object in ArgumentParser::m_arguments
        /// @param position index of argument in ArgumentParser::m_arguments
        void ParseDefault(const Argument& argObj, const size_t position)
        {
            ParsedArgument arg = ParsedArgument();
            arg.m_exists = true;
            arg.m_type = argObj.m_type;

            if (argObj.m_type == ArgType::String)
            {
                arg.m_string = argObj.m_defaultString;
                arg.m_count = argObj.m_defaultString.size();
            }
            else if (argObj.m_type == ArgType::Bool)
            {
                arg.m_bool = argObj.m_defaultBool;
                arg.m_count = argObj.m_defaultBool.size();
            }
            else if (argObj.m_type == ArgType::Double)
            {
                arg.m_double = argObj.m_defaultDouble;
                arg.m_count = argObj.m_defaultDouble.size();
            }
            else if (argObj.m_type == ArgType::LongLong)
            {
                arg.m_longLong = argObj.m_defaultLongLong;
                arg.m_count = argObj.m_defaultLongLong.size();
            }
            else if (argObj.m_type == ArgType::Int)
            {
                arg.m_int = argObj.m_defaultInt;
                arg.m_count = argObj.m_defaultInt.size();
            }

            m_parsed.emplace(position, arg);
            if (!argObj.m_shortName.empty())
            {
                m_names[argObj.m_shortName] = position;
            }
            if (!argObj.m_longName.empty())
            {
                m_names[argObj.m_longName] = position;
            }
            if (!argObj.m_positionalName.empty())
            {
                m_names[argObj.m_positionalName] = position;
            }
        }

        /// @brief Function which creates holder for parsing various length arguments
        /// @param argObj argument object from ArgumentParser::m_arguments
        /// @param position index of argument in ArgumentParser::m_arguments
        /// @return true if the operation succeeded; false otherwise
        void CreateParsingStub(const Argument& argObj, const size_t position)
        {
            const auto argument = m_parsed.find(position);
            if (argument == m_parsed.end())
            {
                ParsedArgument arg = ParsedArgument();
                arg.m_exists = true;
                arg.m_count = 0;
                arg.m_type = argObj.m_type;
                m_parsed.emplace(position, arg);
                if (!argObj.m_shortName.empty())
                {
                    m_names[argObj.m_shortName] = position;
                }
                if (!argObj.m_longName.empty())
                {
                    m_names[argObj.m_longName] = position;
                }
                if (!argObj.m_positionalName.empty())
                {
                    m_names[argObj.m_positionalName] = position;
                }
            }
        }

        /// @brief Actual function which parses argument from command line
        /// @param argObj argument object from ArgumentParser::m_arguments
        /// @param position index of argument in ArgumentParser::m_arguments
        /// @param token token from command line input
        /// @return true if the operation succeeded; false otherwise
        bool Parse(const Argument& argObj, const size_t position, const std::string& token)
        {
            const auto argument = m_parsed.find(position);
            if (argument == m_parsed.end())
            {
                ParsedArgument arg = ParsedArgument();
                arg.m_exists = true;
                arg.m_count = 0;
                arg.m_type = argObj.m_type;
                m_parsed.emplace(position, arg);
                if (!argObj.m_shortName.empty())
                {
                    m_names[argObj.m_shortName] = position;
                }
                if (!argObj.m_longName.empty())
                {
                    m_names[argObj.m_longName] = position;
                }
                if (!argObj.m_positionalName.empty())
                {
                    m_names[argObj.m_positionalName] = position;
                }
                return true;
            }

            if (argument->second.m_type == ArgType::String)
            {
                if (argObj.m_nargs != 0)
                {
                    if (argObj.m_choicesString.size())
                    {
                        auto it = std::find_if(argObj.m_choicesString.begin(), argObj.m_choicesString.end(),
                            [&token](const std::string& str) -> bool
                            {
                                return token == str;
                            });
                        if (it == argObj.m_choicesString.end())
                        {
                            return InvalidateArgsOutOfChoice(argObj, token);
                        }
                    }
                    argument->second.m_string.push_back(token);                    
                }
                else
                {
                    return InvalidateArgsTooMany(argObj);
                }
            }
            else if (argument->second.m_type == ArgType::Bool)
            {
                if (argObj.m_nargs != 0)
                {
                    if (token == "True" || token == "TRUE" || token == "true")
                    {
                        argument->second.m_bool.push_back(true);
                    }
                    else if (token == "False" || token == "FALSE" || token == "false")
                    {
                        argument->second.m_bool.push_back(false);
                    }
                    else
                    {
                        return InvalidateArgsCannotParse(argObj, token);
                    }
                }
                else
                {
                    return InvalidateArgsTooMany(argObj);
                }
            }
            else if (argument->second.m_type == ArgType::Int)
            {
                if (argObj.m_nargs != 0)
                {
                    if (isNumber(token))
                    {
                        try
                        {
                            argument->second.m_int.push_back(std::stoi(token));

                            int value = argument->second.m_int.back();

                            if (argObj.m_choicesInt.size())
                            {
                                auto it = std::find_if(argObj.m_choicesInt.begin(), argObj.m_choicesInt.end(),
                                    [value](const int& integerValue) -> bool
                                    {
                                        return value == integerValue;
                                    });
                                if (it == argObj.m_choicesInt.end())
                                {
                                    return InvalidateArgsOutOfChoice(argObj, token);
                                }
                            }
                        }
                        catch (...)
                        {
                            return InvalidateArgsCannotParse(argObj, token);
                        }
                    }
                    else
                    {
                        return InvalidateArgsCannotParse(argObj, token);
                    }
                }
                else
                {
                    return InvalidateArgsTooMany(argObj);
                }
            }
            else if (argument->second.m_type == ArgType::LongLong)
            {
                if (argObj.m_nargs != 0)
                {
                    if (isNumber(token))
                    {
                        try
                        {
                            argument->second.m_longLong.push_back(std::stoll(token));

                            long long value = argument->second.m_longLong.back();

                            if (argObj.m_choicesLongLong.size())
                            {
                                auto it = std::find_if(argObj.m_choicesLongLong.begin(), argObj.m_choicesLongLong.end(),
                                    [value](const long long& longLongVal) -> bool
                                    {
                                        return value == longLongVal;
                                    });
                                if (it == argObj.m_choicesLongLong.end())
                                {
                                    return InvalidateArgsOutOfChoice(argObj, token);
                                }
                            }
                        }
                        catch (...)
                        {
                            return InvalidateArgsCannotParse(argObj, token);
                        }
                    }
                    else
                    {
                        return InvalidateArgsCannotParse(argObj, token);
                    }
                }
                else
                {
                    return InvalidateArgsTooMany(argObj);
                }
            }
            else if (argument->second.m_type == ArgType::Double)
            {
                if (argObj.m_nargs != 0)
                {
                    if (isNumber(token))
                    {
                        try
                        {
                            argument->second.m_double.push_back(std::stod(token));

                            double value = argument->second.m_double.back();

                            if (argObj.m_choicesDouble.size())
                            {
                                auto it = std::find_if(argObj.m_choicesDouble.begin(), argObj.m_choicesDouble.end(),
                                    [value](const double& doubleVal) -> bool
                                    {
                                        return std::numeric_limits<double>::epsilon() >= std::abs(doubleVal - value);
                                    });
                                if (it == argObj.m_choicesDouble.end())
                                {
                                    return InvalidateArgsOutOfChoice(argObj, token);
                                }
                            }
                        }
                        catch (...)
                        {
                            return InvalidateArgsCannotParse(argObj, token);
                        }
                    }
                    else
                    {
                        return InvalidateArgsCannotParse(argObj, token);
                    }
                }
                else
                {
                    return InvalidateArgsTooMany(argObj);
                }
            }

            argument->second.m_count += 1;
            return true;
        }

        /// @brief Helper function that puts error about argument is out of choices list
        /// @param argObj Argument for which parsing error is generated
        /// @param token token which is not in the list
        /// @return false
        bool InvalidateArgsOutOfChoice(const Argument& argObj, const std::string& token)
        {
            const std::string& name = argObj.m_longName.empty() ? (argObj.m_shortName.empty() ? argObj.m_positionalName : argObj.m_shortName) : argObj.m_longName;

            SetErrorString("Value '" + token + "' is out of choices for \"" + name + "\"");

            return false;
        }

        /// @brief Helper function that puts error about argument has too many inputs
        /// @param argObj Argument for which parsing error is generated
        /// @return false
        bool InvalidateArgsTooMany(const Argument& argObj)
        {
            const std::string& name = argObj.m_longName.empty() ? (argObj.m_shortName.empty() ? argObj.m_positionalName : argObj.m_shortName) : argObj.m_longName;

            SetErrorString("too many arguments for \"" + name + "\"");

            return false;
        }

        /// @brief Helper function that puts error about argument has too many inputs
        /// @param argObj Argument for which parsing error is generated
        /// @param token token which cannot be parsed by argument rules
        /// @return false
        bool InvalidateArgsCannotParse(const Argument& argObj, const std::string& token)
        {
            const std::string& name = argObj.m_longName.empty() ? (argObj.m_shortName.empty() ? argObj.m_positionalName : argObj.m_shortName) : argObj.m_longName;

            SetErrorString("cannot parse [\"" + token + "\"] for  argument \"" + name + "\"");

            return false;
        }

        bool m_isValid = false;
        std::string m_error;
        std::map<const size_t, ParsedArgument>  m_parsed;
        std::map<const std::string, size_t>     m_names;

        friend ArgumentParser;
    };

}}//qor::app

#endif//QOR_PP_H_APP_PARSEDARGSOBJECT