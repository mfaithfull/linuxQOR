// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_APP_ARGUMENT
#define QOR_PP_H_APP_ARGUMENT

#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include <initializer_list>
#include <functional>

#if __cplusplus > 201402L || _MSVC_LANG > 201402L
#include <any>
#endif

namespace qor { namespace app {

    /// @brief anonymous namespace for internal usage
    namespace 
    {
        const size_t kSizeTypeEnd = static_cast<size_t>(-1);
        const size_t kHelpWidth = 80;
        const size_t kHelpNameWidthPercent = 30;

        bool isNumber(const std::string& inStr)
        {
            const bool hasNegSign = inStr.at(0) == '-';
            size_t dotPos = 0, expPos = 0;
            size_t startPos = static_cast<size_t>(hasNegSign);
            for (size_t curPos = startPos; curPos < inStr.size(); ++curPos)
            {
                const char curChar = inStr.at(curPos);
                if (curChar >= '0' && curChar <= '9')
                {
                    continue;
                }
                else if (curChar == '.'
                    && !dotPos && curPos != startPos && curPos != inStr.size() - 1)
                {
                    dotPos = curPos;
                }
                else if ((curChar == 'e' || curChar == 'E')
                    && !expPos && curPos != startPos && curPos != inStr.size() - 1)
                {
                    expPos = curPos;
                    if (!dotPos)
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        size_t getStringStreamLength(std::stringstream& showDesc)
        {
            showDesc.seekp(0, std::ios::end);
            return showDesc.tellp();
        }
    }


    /// @brief Supported types for argument
    /// If needed type is not in this list, then just use e_String
    enum class ArgTypeCast : int
    {
        e_String,
        e_int,
        e_longlong,
        e_double,
        e_bool
    };

    /// @brief constant to indicate arguments with various
    /// count from 0 to infinite
    const int kAnyArgCount = -1;
    /// @brief constant to indicate arguments with various
    /// count from 1 to infinite
    const int kFromOneToInfiniteArgCount = -2;


    class ArgumentParser;
    class ParsedArgsObject;
    class ParsedArgument;

    /// @brief This class represents argument configuration
    /// which should be passed to ArgumentParser object instances
    class Argument
    {
        /// @brief Default constructor. Private!
        /// @param positionalName represents name for positional argument.
        /// @param shortName represents short name for named argument which should be passed with one prefix.
        /// @param longName represents long name for named argument which should be passed with double prefix.
        /// @param argsCount integer number. You can use "kAnyArgCount", "kFromOneToInfiniteArgCount" for non strict count or any int constant.
        /// @param argType type of argument. Defined via enum. Supported types are: int, long long, double and bool and string for all other cases.
        /// @param required Is argument required. Will fail parsing, if required argument are not present.
        /// @param help Your own custom help string start.
        Argument(const std::string& positionalName = "",
            const std::string& shortName = "",
            const std::string& longName = "",
            const int argsCount = 1,
            ArgTypeCast argType = ArgTypeCast::e_String,
            const bool required = true,
            const std::string& help = "")
            : m_required(required)
            , m_nargs(argsCount)
            , m_type(argType)
            , m_positionalName(positionalName)
            , m_shortName(shortName)
            , m_longName(longName)
            , m_help(help)
        {}
    public:

        /// @brief Default constructor positional arguments. You can use class Setters or pass your own values to public members directly.
        /// @param shortName represents short name for named argument which should be passed with one prefix.
        /// @param longName represents long name for named argument which should be passed with double prefix.
        /// @param argsCount integer number. You can use "kAnyArgCount", "kFromOneToInfiniteArgCount" for non strict count or any int constant.
        /// @param argType type of argument. Defined via enum. Supported types are: int, long long, double and bool and string for all other cases.
        /// @param required Is argument required. Will fail parsing, if required argument are not present.
        /// @param help Your own custom help string start.
        static Argument CreateNamedArgument(const std::string& shortName = "",
            const std::string& longName = "",
            const int argsCount = 1,
            ArgTypeCast argType = ArgTypeCast::e_String,
            const bool required = true,
            const std::string& help = "")
        {
            return Argument("", shortName, longName, argsCount, argType, required, help);
        }

        /// @brief Default function for named arguments. You can use class Setters or pass your own values to public members directly.
        /// @param positionalName represents name for positional argument.
        /// @param argsCount integer number. You can use "kAnyArgCount", "kFromOneToInfiniteArgCount" for non strict count or any int constant.
        /// @param argType type of argument. Defined via enum. Supported types are: int, long long, double and bool and string for all other cases.
        /// @param required Is argument required. Will fail parsing, if required argument are not present.
        /// @param help Your own custom help string start.
        static Argument CreatePositionalArgument(const std::string& positionalName = "",
            const int argsCount = 1,
            ArgTypeCast argType = ArgTypeCast::e_String,
            const bool required = true,
            const std::string& help = "")
        {
            return Argument(positionalName, "", "", argsCount, argType, required, help);
        }

        /// @brief required flag argument
        /// If required argument is not set in command line
        /// then parsing will fail.
        ///
        /// IMPORTANT: ALL arguments -- named AND positional -- are REQUIRED by
        /// default. This differs from Python's argparse, where named options are
        /// optional by default. To make an argument optional call
        /// SetRequired(false) (or pass required=false to the factory function).
        /// Optional flags in particular almost always want SetRequired(false).
        bool m_required = true;

        /// @brief Setter function to flag,
        /// @param required - bool value to indicate is argument required or not
        /// @return reference to current argument
        Argument& SetRequired(bool required)
        {
            m_required = required;
            return *this;
        }

        /// @brief variable that indicates count of argument in input
        /// use "kAnyArgCount" or "kFromOneToInfiniteArgCount" constants
        /// for arguments with variable count. Any other arguments count
        /// will be passed as strict arguments count.
        /// 0 is for flags (arguments that doesn't carry any data)
        /// set to 1 by default.
        int m_nargs = 1;

        /// @brief setter function for m_nargs with desired amount
        /// @param amount int value that indicates  amount of argument.
        /// Could be "kAnyArgCount" or "kFromOneToInfiniteArgCount", 0 or any other positive integer.
        /// @return reference to current argument
        Argument& SetNumberOfArguments(int amount)
        {
            m_nargs = amount;
            return *this;
        }

        /// @brief Handy setter for argument count with self declared name
        /// @return reference to current argument
        Argument& SetAnyNumberOfArgumentsButAtLeastOne()
        {
            m_nargs = kFromOneToInfiniteArgCount;
            return *this;
        }

        /// @brief Handy setter for argument count with self declared name
        /// @return reference to current argument
        Argument& SetAnyNumberOfArguments()
        {
            m_nargs = kAnyArgCount;
            return *this;
        }

        /// @brief Handy setter for argument which is actually a flag (e.g. has no any parameters)
        /// @return reference to current argument
        Argument& SetArgumentIsFlag()
        {
            m_nargs = 0;
            return *this;
        }

        /// @brief Variable that hold type of argument.
        /// string by default
        ArgTypeCast m_type = ArgTypeCast::e_String;

        /// @brief Setter function for type of current argument
        /// @param argType setter for type of current argument data.
        /// Any non string types will be casted while parsing.
        /// @return reference to current argument
        Argument& SetType(ArgTypeCast argType)
        {
            m_type = argType;
            return *this;
        }

        /// @brief name of positional argument
        /// positional arguments name used only to access desired argument from code
        std::string m_positionalName = "";

        /// @brief Handy setter for positional argument
        /// @param name name for positional argument. Empty by default
        /// @return reference to current argument
        Argument& SetPositionalName(const std::string& name)
        {
            m_positionalName = name;
            return *this;
        }

        /// @brief arguments short name
        /// for non positional argument only
        /// shot name used in input with 1 prefix
        std::string m_shortName = "";

        /// @brief Handy setter for short named argument. 
        /// Should been used with ordinary prefix in command line.
        /// Can be auto-generated if possible when m_allowAbbrev in ParsedArgument set to true.
        /// @param name name for positional argument. Empty by default
        /// @return reference to current argument
        Argument& SetShortName(const std::string& name)
        {
            m_shortName = name;
            return *this;
        }
        
        /// @brief arguments long name.
        /// for non positional arguments only
        /// argument long name start with double prefix
        std::string m_longName = "";

        /// @brief Handy setter for long named argument. 
        /// Should been used with double prefix in command line.
        /// Can be used for auto-generation of short name if it possible
        /// and m_allowAbbrev in ParsedArgument is "true".
        /// @param name name for positional argument. Empty by default
        /// @return reference to current argument
        Argument& SetLongName(const std::string& name)
        {
            m_longName = name;
            return *this;
        }

        /// @brief additional help info for argument
        /// Will be part of generated help
        std::string m_help = "";

        /// @brief Handy setter for additional help
        /// @param help string with additional help. Empty by default.
        /// @return reference to current argument
        Argument& SetHelp(const std::string& help)
        {
            m_help = help;
            return *this;
        }
        
        /// @brief vector of strings to validate arguments input data.
        /// Empty by default. Will fail parsing if string not is in input list
        std::vector<std::string> m_choicesString = {};

        /// @brief Handy setter of valid choices for arguments with string type
        /// @param choices vector or initializer list of valid strings
        /// @return reference to current argument
        Argument& SetChoices(const std::vector<std::string>& choices)
        {
            if (m_type != ArgTypeCast::e_String)
            {
                throw std::runtime_error("wrong type");
            }
            m_choicesString = choices;
            return *this;
        }

        /// @brief Overload so a braced list of string literals -- e.g.
        /// SetChoices({"+", "-"}) -- resolves unambiguously to the string
        /// choices instead of colliding with the int/double/long long overloads.
        /// @param choices initializer list of string literals
        /// @return reference to current argument
        Argument& SetChoices(std::initializer_list<const char*> choices)
        {
            return SetChoices(std::vector<std::string>(choices.begin(), choices.end()));
        }

        /// @brief vector of integers to validate arguments input data.
        /// Empty by default. Will fail parsing if ints not is in input list
        std::vector<int> m_choicesInt = {};

        /// @brief Handy setter of valid choices for arguments with int type
        /// @param choices vector or initializer list of valid ints
        /// @return reference to current argument
        Argument& SetChoices(const std::vector<int>& choices)
        {
            if (m_type != ArgTypeCast::e_int)
            {
                throw std::runtime_error("wrong type");
            }
            m_choicesInt = choices;
            return *this;
        }

        /// @brief vector of long longs to validate arguments input data.
        /// Empty by default. Will fail parsing if long longs not is in input list
        std::vector<long long> m_choicesLongLong = {};

        /// @brief Handy setter of valid choices for arguments with long long type
        /// @param choices vector or initializer list of valid long longs
        /// @return reference to current argument
        Argument& SetChoices(const std::vector<long long>& choices)
        {
            if (m_type != ArgTypeCast::e_longlong)
            {
                throw std::runtime_error("wrong type");
            }
            m_choicesLongLong = choices;
            return *this;
        }

        /// @brief vector of double to validate arguments input data.
        /// Empty by default. Will fail parsing if double not is in input list
        std::vector<double> m_choicesDouble = {};

        /// @brief Handy setter of valid choices for arguments with double type
        /// @param choices vector or initializer list of valid double
        /// @return reference to current argument
        Argument& SetChoices(const std::vector<double>& choices)
        {
            if (m_type != ArgTypeCast::e_double)
            {
                throw std::runtime_error("wrong type");
            }
            m_choicesDouble = choices;
            return *this;
        }

        /// @brief Handy setter for single default argument of bool type
        /// @param defaultArg default boolean value
        /// @return reference to current argument
        Argument& SetDefault(bool defaultArg)
        {
            if (m_type != ArgTypeCast::e_bool)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultBool.push_back(defaultArg);
            m_hasDefault = true;
            return *this;
        }

        /// @brief Handy setter for single default argument of int type
        /// @param defaultArg default int value
        /// @return reference to current argument
        Argument& SetDefault(int defaultArg)
        {
            if (m_type != ArgTypeCast::e_int)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultInt.push_back(defaultArg);
            m_hasDefault = true;
            return *this;
        }

        /// @brief Handy setter for single default argument of long long type
        /// @param defaultArg default long long value
        /// @return reference to current argument
        Argument& SetDefault(long long defaultArg)
        {
            if (m_type != ArgTypeCast::e_longlong)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultLongLong.push_back(defaultArg);
            m_hasDefault = true;
            return *this;
        }

        /// @brief Handy setter for single default argument of double type
        /// @param defaultArg default double value
        /// @return reference to current argument
        Argument& SetDefault(double defaultArg)
        {
            if (m_type != ArgTypeCast::e_double)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultDouble.push_back(defaultArg);
            m_hasDefault = true;
            return *this;
        }

        /// @brief Handy setter for single default argument of string type
        /// @param defaultArg default string value
        /// @return reference to current argument
        Argument& SetDefault(std::string defaultArg)
        {
            if (m_type != ArgTypeCast::e_String)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultString.push_back(defaultArg);
            m_hasDefault = true;
            return *this;
        }

        /// @brief Handy setter for vector of default arguments of bool type
        /// @param defaultArg vector of default boolean values
        /// @return reference to current argument
        Argument& SetDefault(const std::vector<bool>& defaultArg)
        {
            if (m_type != ArgTypeCast::e_bool)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultBool = defaultArg;
            m_hasDefault = true;
            return *this;
        }

        /// @brief Handy setter for vector of default arguments of int type
        /// @param defaultArg vector of default int values
        /// @return reference to current argument
        Argument& SetDefault(const std::vector<int>& defaultArg)
        {
            if (m_type != ArgTypeCast::e_int)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultInt = defaultArg;
            m_hasDefault = true;
            return *this;
        }

        /// @brief Handy setter for vector of default arguments of long long type
        /// @param defaultArg vector of default long values
        /// @return reference to current argument
        Argument& SetDefault(const std::vector<long long>& defaultArg)
        {
            if (m_type != ArgTypeCast::e_longlong)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultLongLong = defaultArg;
            m_hasDefault = true;
            return *this;
        }

        /// @brief Handy setter for vector of default arguments of double type
        /// @param defaultArg vector of default double values
        /// @return reference to current argument
        Argument& SetDefault(const std::vector<double>& defaultArg)
        {
            if (m_type != ArgTypeCast::e_double)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultDouble = defaultArg;
            m_hasDefault = true;
            return *this;
        }

        /// @brief Handy setter for vector of default arguments of string type
        /// @param defaultArg vector of default string values
        /// @return reference to current argument
        Argument& SetDefault(const std::vector<std::string>& defaultArg)
        {
            if (m_type != ArgTypeCast::e_String)
            {
                throw std::runtime_error("wrong type");
            }
            m_defaultString = defaultArg;
            m_hasDefault = true;
            return *this;
        }


        /// @brief Getter to indicate does argument has any default value
        /// @return reference to current argument
        bool HasDefault() const
        {
            return m_hasDefault;
        }

        /// @brief Bind a variable to this argument. After a successful
        /// ParseArgs(), the parsed value is written directly into *target, so
        /// you no longer have to pull it out with GetArg(name).GetAsX().
        ///
        /// BindTo also sets this argument's type to match the bound variable,
        /// so a separate SetType() call is not needed (and should not be used
        /// to contradict it).
        ///
        /// IMPORTANT: *target must outlive the ParseArgs() call. If the
        /// argument is optional and absent (with no default), the bound
        /// variable is left untouched -- initialize it yourself for a default.
        /// @param target pointer to the variable that receives the parsed value
        /// @return reference to current argument
        Argument& BindTo(bool* target);
        Argument& BindTo(int* target);
        Argument& BindTo(long long* target);
        Argument& BindTo(double* target);
        Argument& BindTo(std::string* target);
        Argument& BindTo(std::vector<bool>* target);
        Argument& BindTo(std::vector<int>* target);
        Argument& BindTo(std::vector<long long>* target);
        Argument& BindTo(std::vector<double>* target);
        Argument& BindTo(std::vector<std::string>* target);

        /// @brief Does this argument have a bound variable (see BindTo)
        /// @return true if BindTo(...) was called on this argument
        bool HasBinding() const
        {
            return static_cast<bool>(m_binding);
        }

        /// @brief Apply the binding (if any) from a parsed result. Called by
        /// ArgumentParser after a successful parse; a no-op when unbound.
        /// @param parsed the parsed values for this argument
        void ApplyBinding(const ParsedArgument& parsed) const
        {
            if (m_binding)
            {
                m_binding(parsed);
            }
        }

    private:
        /// @brief type-erased sink installed by BindTo(...); empty when unbound
        std::function<void(const ParsedArgument&)> m_binding = nullptr;

        bool                     m_hasDefault = false;
        std::vector<bool>        m_defaultBool = {};
        std::vector<int>         m_defaultInt = {};
        std::vector<long long>   m_defaultLongLong = {};
        std::vector<double>      m_defaultDouble = {};
        std::vector<std::string> m_defaultString = {};

        friend class ParsedArgsObject;
    };
}}//qor::app

#endif//QOR_PP_H_APP_ARGUMENT