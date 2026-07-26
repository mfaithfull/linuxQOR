
#ifndef QOR_PP_H_APP_ARGUMENTPARSER
#define QOR_PP_H_APP_ARGUMENTPARSER

#include "parsedargs.h"
#include "iargumented.h"

namespace qor { namespace app {

    /// @brief Main class of argument parser
    /// hold all user arguments from code and orchestrate other classes 
    /// in order to parse command line input
    class ArgumentParser
    {
    public:
        /// @brief Constructor for ArgumentParser
        /// @param name Program name which will appear in auto-generated help
        ArgumentParser(const std::string& name) noexcept
            : m_name(name)
        {}

        ArgumentParser(IArgumented& argumented)
        {
            m_name = argumented.Name();
            SetDescription(argumented.Description());
            SetAllowAbbrev(argumented.Config().allowAbbreviation);
            SetIgnoreUnknownArgs(argumented.Config().ignoreUnknownArgs);
            SetPrefixChars(argumented.Config().prefixChars);
            SetAddHelp(argumented.Config().addHelp);
            SetEpilogue(argumented.UsageEpilogue());
            const std::string& usageOverride = argumented.OverrideUsage();
            if(!usageOverride.empty())
            {
                SetUsage(usageOverride);
            }
            for(auto namedArg : argumented.NamedArguments())
            {
                AddArgument(CreateNamedArgument(namedArg));
            }
            for(auto positionalArg : argumented.PositionalArguments())
            {
                AddArgument(CreatePositionalArgument(positionalArg));
            }
        }

        /// @brief Overload default description for auto-generated command line
        /// @param description Text to display before the argument help ("" by default)
        /// @return reference to current parser
        ArgumentParser& SetDescription(const std::string& description) noexcept
        {
            m_description = description;
            return *this;
        }        

        /// @brief Allows long options to be abbreviated if the abbreviation is unambiguous.
        /// This has two effects when enabled:
        ///  * during parsing, an unambiguous prefix of a long option is accepted
        ///    on the command line (e.g. "--verb" for "--verbose");
        ///  * for every named argument that has a long name but no explicit short
        ///    name, a single-character short name is auto-generated (when a free
        ///    letter is available) and shown in the generated help.
        /// @param allowAbbrev bool value true for allow (true by default)
        /// @return reference to current parser
        ArgumentParser& SetAllowAbbrev(bool allowAbbrev) noexcept
        {
            m_allowAbbrev = allowAbbrev;
            return *this;
        }

        /// @brief Setter to ignore unknown argument while parsing.
        /// If false parsing will be failed if parser detected unknown argument.
        /// @param ignoreUnknownArgs bool value for ignore or not (false by default)
        /// @return reference to current parser
        ArgumentParser& SetIgnoreUnknownArgs(bool ignoreUnknownArgs) noexcept
        {
            m_ignoreUnknownArgs = ignoreUnknownArgs;
            return *this;
        }

        /// @brief Add a - h / --help option to the parser
        /// @param addHelp Flag to add or not (true by default)
        /// @return reference to current parser
        ArgumentParser& SetAddHelp(bool addHelp) noexcept
        {
            m_addHelp = addHelp;
            return *this;
        }



        /// @brief This function allows to set final message after program description and before
        /// argument list description
        /// @param epilogue string of epilogue. Automatically adjusting to screen size. (empty by default)
        /// @return reference to current parser
        ArgumentParser& SetEpilogue(const std::string& epilogue) noexcept
        {
            m_epilogue = epilogue;
            return *this;
        }

        /// @brief Program usage examples
        /// @param usage the string describing the program usage. (default: generated from arguments added to parser)
        /// @return reference to current parser
        ArgumentParser& SetUsage(const std::string& usage) noexcept
        {
            m_usage = usage;
            return *this;
        }

        /// @brief Function to override default prefix. Be careful when you choosing prefix.
        /// Single prefix will be used for short names of named argument. Double prefix - for long names.
        /// Positional arguments have no any prefix
        /// @param charSym character which will be used for prefix. ('-' by default)
        /// @return 
        ArgumentParser& SetPrefixChars(const char charSym) noexcept
        {
            m_prefix = charSym;
            return *this;
        }

        /// @brief Function to add arguments specification to command line parser
        /// @param arg Argument instance
        void AddArgument(const Argument& arg)
        {
            if (arg.m_longName.empty() && arg.m_shortName.empty() && arg.m_positionalName.empty())
            {
                throw std::runtime_error("Short,long or positional names of argument are empty.\n"
                    "At least one name should been specified.");
            }
            else if (!(arg.m_longName.empty() || arg.m_shortName.empty()) && !arg.m_positionalName.empty())
            {
                throw std::runtime_error("Positional argument " + arg.m_positionalName + " declared aside with short/long name\n"
                    "Positional argument shouldn't have any short/long name.");
            }
            else if (arg.m_choicesDouble.size() + arg.m_choicesInt.size() + arg.m_choicesLongLong.size() + arg.m_choicesString.size())
            {
                if (arg.m_type == ArgTypeCast::e_bool)
                {
                    throw std::runtime_error("No need to declare choice for bool type");
                }
                else if (arg.m_type == ArgTypeCast::e_int && arg.m_choicesDouble.size() + arg.m_choicesLongLong.size() + arg.m_choicesString.size())
                {
                    throw std::runtime_error("Only int choices should been declared");
                }
                else if (arg.m_type == ArgTypeCast::e_longlong && arg.m_choicesDouble.size() + arg.m_choicesInt.size() + arg.m_choicesString.size())
                {
                    throw std::runtime_error("Only long long choices should been declared");
                }
                else if (arg.m_type == ArgTypeCast::e_double && arg.m_choicesInt.size() + arg.m_choicesLongLong.size() + arg.m_choicesString.size())
                {
                    throw std::runtime_error("Only double choices should been declared");
                }
                else if (arg.m_type == ArgTypeCast::e_String && arg.m_choicesInt.size() + arg.m_choicesLongLong.size() + arg.m_choicesDouble.size())
                {
                    throw std::runtime_error("Only string choices should been declared");
                }
            }
            _addArg(arg);
        }

        /// @brief Main function of parsing argument
        /// @param args vector of input tokens
        /// @return ParsedArgsObject, which contains valid ParsedArgument if parsing successful or
        /// information about errors if not
        ParsedArgsObject ParseArgs(const std::vector<std::string>& args)
        {
            std::string _pref{ m_prefix };
            std::string _doublePref{ m_prefix, m_prefix };

            // Fill in auto short names before building the internal lookup map so
            // the generated names participate in parsing.
            GenerateAbbreviations();

            for (auto& el : m_knownArgumentNames)
            {
                if (el.second.argNameType == KnownNameType::e_Short)
                {
                    m_knownArgumentNamesInternal[_pref + el.first] = el.second;
                }
                else
                {
                    m_knownArgumentNamesInternal[_doublePref + el.first] = el.second;
                }
            }

            if (m_addHelp)
            {
                bool shortHelpAlreadyExists = false, longHelpAlreadyExists = false;
                auto foundArgObject = m_knownArgumentNamesInternal.find(_pref + "h");
                shortHelpAlreadyExists = foundArgObject != m_knownArgumentNamesInternal.end();
                foundArgObject = m_knownArgumentNamesInternal.find(_doublePref + "help");
                longHelpAlreadyExists = foundArgObject != m_knownArgumentNamesInternal.end();
                if (!shortHelpAlreadyExists || !longHelpAlreadyExists)
                {
                    Argument arg = Argument::CreateNamedArgument(shortHelpAlreadyExists ? "" : "h", longHelpAlreadyExists ? "" : "help", 0);
                    arg.SetHelp("Show help!");
                    arg.SetRequired(false);
                    _addArg(arg);
                    if (!shortHelpAlreadyExists)
                    {
                        m_knownArgumentNamesInternal[_pref + arg.m_shortName] = { m_arguments.size()-1, KnownNameType::e_Short };
                    }
                    if (!longHelpAlreadyExists)
                    {
                        m_knownArgumentNamesInternal[_doublePref + arg.m_longName] = { m_arguments.size()-1, KnownNameType::e_Long };
                    }
                }
            }

            bool positionalArgsEndFlag = false;
            size_t currentArgumentObjectIndex = kSizeTypeEnd;
            std::vector<std::string> positionalArgs;
            ParsedArgsObject argObj;
            for (size_t i =0; i < args.size(); ++i)
            {
                const std::string& el = args[i];
                auto foundArgObject = m_knownArgumentNamesInternal.find(el);
                if (foundArgObject == m_knownArgumentNamesInternal.end() && m_allowAbbrev)
                {
                    // SetAllowAbbrev: accept an unambiguous prefix of a long option,
                    // e.g. "--verb" for "--verbose".
                    bool ambiguous = false;
                    std::string abbrev = ResolveAbbreviation(el, _doublePref, ambiguous);
                    if (ambiguous)
                    {
                        argObj.SetErrorString("Ambiguous option \"" + el
                            + "\" matches more than one argument");
                        return argObj;
                    }
                    if (!abbrev.empty())
                    {
                        foundArgObject = m_knownArgumentNamesInternal.find(abbrev);
                    }
                }
                if (foundArgObject != m_knownArgumentNamesInternal.end())
                {
                    currentArgumentObjectIndex = foundArgObject->second.position;

                    Argument& argument = m_arguments[currentArgumentObjectIndex];
                    if (argument.m_nargs == 0)
                    {
                        if (!argObj.Parse(argument, currentArgumentObjectIndex, el))
                        {
                            return argObj;
                        }
                    }
                    else
                    {
                        argObj.CreateParsingStub(argument, currentArgumentObjectIndex);
                    }

                    positionalArgsEndFlag = true;
                    continue;
                }
                else if (!positionalArgsEndFlag)
                {
                    if (isNumber(el))
                    {
                        positionalArgs.push_back(el);
                    }
                    else if (el.find(_pref) == 0 || el.find(_doublePref) == 0)
                    {
                        if (!_unknownArgumentHit(argObj, i+1, currentArgumentObjectIndex, positionalArgsEndFlag, el))
                        {
                            return argObj;
                        }
                    }
                    else
                    {
                        positionalArgs.push_back(el);
                    }
                    continue;
                }
                else if (el.find(_pref) == 0 || el.find(_doublePref) == 0)
                {
                    if (!_unknownArgumentHit(argObj, i+1, currentArgumentObjectIndex, positionalArgsEndFlag, el))
                    {
                        return argObj;
                    }
                    continue;
                }

                if (currentArgumentObjectIndex != kSizeTypeEnd)
                {
                    Argument& argument = m_arguments[currentArgumentObjectIndex];

                    if (!argObj.Parse(argument, currentArgumentObjectIndex, el))
                    {
                        return argObj;
                    }
                }
            }

            if (!positionalArgs.empty())
            {
                if (m_positionalArgumentNames.empty())
                {
                    argObj.SetErrorString("Unknown positional argument:" + positionalArgs.front());
                    return argObj;
                }
                size_t minimumRequiredPositionalCount = 0;
                size_t infiniteRequiredPositionalCount = 0;
                size_t optionalPositionalCount = 0;

                for (auto& el : m_positionalArgumentNames)
                {
                    if (m_arguments[el.positionInArguments].m_required)
                    {
                        minimumRequiredPositionalCount += m_arguments[el.positionInArguments].m_nargs == kFromOneToInfiniteArgCount ? 1 : m_arguments[el.positionInArguments].m_nargs;
                        infiniteRequiredPositionalCount = m_arguments[el.positionInArguments].m_nargs == kFromOneToInfiniteArgCount;
                    }
                    else
                    {
                        ++optionalPositionalCount;
                    }
                }
                if (minimumRequiredPositionalCount > positionalArgs.size())
                {
                    argObj.SetErrorString("Too few positional arguments: required " + std::to_string(minimumRequiredPositionalCount) + " got " + std::to_string(positionalArgs.size()));
                    return argObj;
                }

                size_t totalTokensForRequiredNargs = 1;
                size_t additionalTokensForFirstRequiredNarg = 0;
                size_t howMuchOptionalArgsCanBeParsed = positionalArgs.size() - minimumRequiredPositionalCount;
                if (howMuchOptionalArgsCanBeParsed > optionalPositionalCount)
                {
                    if (infiniteRequiredPositionalCount == 0)
                    {
                        argObj.SetErrorString("Too many positional arguments!");
                        return argObj;
                    }
                    else
                    {
                        totalTokensForRequiredNargs = (howMuchOptionalArgsCanBeParsed - optionalPositionalCount) / infiniteRequiredPositionalCount;
                        additionalTokensForFirstRequiredNarg = (howMuchOptionalArgsCanBeParsed - optionalPositionalCount) % infiniteRequiredPositionalCount;
                    }
                    howMuchOptionalArgsCanBeParsed = optionalPositionalCount;
                }


                size_t currentTokenPosition = 0;
                size_t optionalParsed = 0;

                for (auto& el : m_positionalArgumentNames)
                {
                    Argument& argument = m_arguments[el.positionInArguments];
                    argObj.CreateParsingStub(argument, el.positionInArguments);

                    if (m_arguments[el.positionInArguments].m_required)
                    {

                        if (m_arguments[el.positionInArguments].m_nargs != kFromOneToInfiniteArgCount)
                        {
                            for (size_t i = 0; i < static_cast<size_t>(m_arguments[el.positionInArguments].m_nargs); ++i)
                            {
                                if (!argObj.Parse(argument, el.positionInArguments, positionalArgs[currentTokenPosition]))
                                {
                                    return argObj;
                                }
                                ++currentTokenPosition;
                            }
                        }
                        else
                        {
                            size_t addtionalArg = 0;
                            if (additionalTokensForFirstRequiredNarg > 0)
                            {
                                ++addtionalArg;
                                --additionalTokensForFirstRequiredNarg;
                            }
                            for (size_t i = 0; i < totalTokensForRequiredNargs + addtionalArg; ++i)
                            {
                                if (!argObj.Parse(argument, el.positionInArguments, positionalArgs[currentTokenPosition]))
                                {
                                    return argObj;
                                }
                                ++currentTokenPosition;
                            }
                        }
                    }
                    else if (optionalParsed <= howMuchOptionalArgsCanBeParsed)
                    {
                        if (!argObj.Parse(argument, el.positionInArguments, positionalArgs[currentTokenPosition]))
                        {
                            return argObj;
                        }
                        ++currentTokenPosition;
                        ++optionalParsed;
                    }
                }
            }

            for (size_t i = 0; i < m_arguments.size(); ++i)
            {
                auto el = m_arguments[i];
                const std::string& name = el.m_longName.empty() ? (el.m_shortName.empty() ? el.m_positionalName : el.m_shortName) : el.m_longName;

                ParsedArgument parsedArg = argObj.GetArg(name);

                if (parsedArg.GetArgumentExists())
                {
                    if (static_cast<int>(parsedArg.GetArgumentCount()) == el.m_nargs
                        || el.m_nargs == kAnyArgCount
                        || (el.m_nargs == kFromOneToInfiniteArgCount && parsedArg.GetArgumentCount() >= 1))
                    {
                        continue;
                    }
                    else
                    {
                        argObj.SetErrorString("Wrong arguments count for argument with name \"" + name + "\" got = " + std::to_string(parsedArg.GetArgumentCount()));
                        return argObj;
                    }
                }
                else if (el.HasDefault())
                {
                    argObj.ParseDefault(el, i);
                }
                else if (el.m_required)
                {
                    argObj.SetErrorString("Required argument with name \"" + name + "\" does not exist");
                    return argObj;
                }
            }

            // Bindings: parsing and validation succeeded, so push values into
            // any variables the caller registered with Argument::BindTo(...).
            // Optional arguments that are absent (and have no default) do not
            // exist here, so their bound variables are left untouched.
            for (size_t i = 0; i < m_arguments.size(); ++i)
            {
                const Argument& el = m_arguments[i];
                if (!el.HasBinding())
                {
                    continue;
                }
                const std::string& name = el.m_longName.empty() ? (el.m_shortName.empty() ? el.m_positionalName : el.m_shortName) : el.m_longName;
                ParsedArgument parsedArg = argObj.GetArg(name);
                if (parsedArg.GetArgumentExists() && parsedArg.GetArgumentCount() >= 1)
                {
                    el.ApplyBinding(parsedArg);
                }
            }

            argObj.SetValid();
            return argObj;
        }

        /// @brief function which resolves unknown arguments presence
        /// @param argObj
        /// @param positionInInput
        /// @param currentArgumentObjectIndex 
        /// @param positionalArgsEndFlag 
        /// @param el 
        /// @return bool - true for ignoring, false for stopping parse 
        bool _unknownArgumentHit(ParsedArgsObject& argObj, const size_t positionInInput, size_t& currentArgumentObjectIndex, bool& positionalArgsEndFlag, const std::string& el)
        {
            if (m_ignoreUnknownArgs)
            {
                currentArgumentObjectIndex = kSizeTypeEnd;
                positionalArgsEndFlag = true;
                return true;
            }
            std::stringstream ss;
            ss << "Unknown input argument: \"" << el << "\" at position " << positionInInput;
            argObj.SetErrorString(ss.str());
            return false;
        }

        /// @brief This function just converts argc and argv to vector of token
        /// @param argc count of arguments
        /// @param argv pointer to array of char*
        /// @return ParsedArgsObject, which contains valid ParsedArgument if parsing successful or
        /// information about errors if not
        ParsedArgsObject ParseArgs(const int argc, const char** argv)
        {

            std::vector<std::string> args;
            for (int i = 1; i < argc; ++i)
            {
                args.emplace_back(argv[i]);
            }

            return ParseArgs(args);
        }

        /// @brief Function to get help string
        /// @param width current terminal width (80 by default)
        /// @param nameWidthPercent percentage of current width, for naming parameters (30 by default)
        /// @return help string with proper new lines
        std::string GetUsage(size_t width = kHelpWidth, size_t nameWidthPercent = kHelpNameWidthPercent)
        {
            // Ensure auto-generated short names appear in the help even when
            // GetUsage is called before ParseArgs.
            GenerateAbbreviations();

            width = width < kHelpWidth ? kHelpWidth : width;
            size_t nameWidthInHelp = nameWidthPercent * width / 100;
            width -= nameWidthInHelp;


            // NOTE: do not initialize the stream with "usage: " -- the first
            // insertion below would overwrite it (the put pointer starts at 0).
            std::stringstream usage;
            if (!m_usage.empty())
            {
                // Caller-provided usage line (SetUsage) overrides the auto-generated one.
                usage << m_usage;
            }
            else
            {
                usage << m_name << " ";
                if (m_positionalArgumentNames.size())
                {
                    for (auto& el : m_positionalArgumentNames)
                    {
                        Argument& arg = m_arguments[el.positionInArguments];
                        MakeUsageForName(arg, usage);
                    }
                }
                for (auto& el : m_arguments)
                {
                    if (el.m_positionalName.empty())
                    {
                        MakeUsageForName(el, usage);
                    }
                }
            }

            if (!m_description.empty())
            {
                AddAdditionalDescription(usage, m_description, width+nameWidthInHelp);
            }

            if (m_positionalArgumentNames.size())
            {
                usage << "\n\n" << "positional arguments:\n\n";
                for (auto& el : m_positionalArgumentNames)
                {
                    Argument& arg = m_arguments[el.positionInArguments];
                    MakeDescriptionForArg(arg, usage, nameWidthInHelp, width);
                }
            }

            if (m_arguments.size() > m_positionalArgumentNames.size())
            {
                usage << "\n\n" << "named arguments:\n\n";
                for (auto& el : m_arguments)
                {
                    if (el.m_positionalName.empty())
                    {
                        MakeDescriptionForArg(el, usage, nameWidthInHelp, width);
                    }
                }
            }

            if (!m_epilogue.empty())
            {
                AddAdditionalDescription(usage, m_epilogue, width+nameWidthInHelp);
            }

            return TrimTrailingSpacesPerLine(usage.str());
        }

        /// @brief Removes trailing spaces/tabs from every line of the help text.
        /// The column-based layout leaves padding at the end of many lines; this
        /// keeps the rendered help clean without touching the wrapping logic.
        /// @param text help text possibly containing trailing whitespace
        /// @return text with per-line trailing whitespace removed
        static std::string TrimTrailingSpacesPerLine(const std::string& text)
        {
            std::string result;
            result.reserve(text.size());
            size_t lineStart = 0;
            while (lineStart <= text.size())
            {
                size_t nl = text.find('\n', lineStart);
                size_t lineEnd = (nl == std::string::npos) ? text.size() : nl;
                size_t last = lineEnd;
                while (last > lineStart && (text[last - 1] == ' ' || text[last - 1] == '\t'))
                {
                    --last;
                }
                result.append(text, lineStart, last - lineStart);
                if (nl == std::string::npos)
                {
                    break;
                }
                result.push_back('\n');
                lineStart = nl + 1;
            }
            return result;
        }

    private:


        /// @brief Resolves an unambiguous long-option abbreviation (SetAllowAbbrev).
        /// Given a token like "--verb", finds the single long option whose
        /// prefixed name ("--verbose") begins with it.
        /// @param token the (double-prefixed) token typed on the command line
        /// @param doublePref the double-prefix string (e.g. "--")
        /// @param ambiguous set to true if the token is a prefix of more than one
        /// long option; in that case an empty string is returned.
        /// @return the full internal key of the unique match, or "" if none/ambiguous.
        std::string ResolveAbbreviation(
            const std::string& token, const std::string& doublePref, bool& ambiguous)
        {
            ambiguous = false;
            // Only long options (prefixed with the double prefix) can be abbreviated,
            // and the token must be a strict, non-empty prefix.
            if (token.size() <= doublePref.size()
                || token.compare(0, doublePref.size(), doublePref) != 0)
            {
                return std::string();
            }

            std::string match;
            for (auto it = m_knownArgumentNamesInternal.begin();
                it != m_knownArgumentNamesInternal.end(); ++it)
            {
                if (it->second.argNameType == KnownNameType::e_Long
                    && it->first.size() > token.size()
                    && it->first.compare(0, token.size(), token) == 0)
                {
                    if (!match.empty())
                    {
                        ambiguous = true;
                        return std::string();
                    }
                    match = it->first;
                }
            }
            return match;
        }

        /// @brief Auto-generates single-character short names for named arguments
        /// that have a long name but no explicit short name, when SetAllowAbbrev
        /// is enabled. For each such argument the first free alphanumeric letter of
        /// its long name is chosen; if every letter is already taken the argument
        /// simply keeps no short name. The generated names are registered so they
        /// work both for parsing and for the auto-generated help output.
        /// Runs at most once (guarded by m_abbrevGenerated).
        void GenerateAbbreviations()
        {
            if (!m_allowAbbrev || m_abbrevGenerated)
            {
                return;
            }
            m_abbrevGenerated = true;

            for (size_t i = 0; i < m_arguments.size(); ++i)
            {
                Argument& arg = m_arguments[i];
                // Only named arguments with a long name and no short name qualify.
                if (arg.m_longName.empty() || !arg.m_shortName.empty()
                    || !arg.m_positionalName.empty())
                {
                    continue;
                }

                for (size_t c = 0; c < arg.m_longName.size(); ++c)
                {
                    const char ch = arg.m_longName[c];
                    const bool isAlnum = (ch >= 'a' && ch <= 'z')
                        || (ch >= 'A' && ch <= 'Z')
                        || (ch >= '0' && ch <= '9');
                    if (!isAlnum)
                    {
                        continue;
                    }

                    const std::string candidate(1, ch);
                    // "h" is reserved for the auto-added -h/--help option.
                    if (m_addHelp && candidate == "h")
                    {
                        continue;
                    }
                    if (m_knownArgumentNames.find(candidate) != m_knownArgumentNames.end())
                    {
                        continue;
                    }

                    arg.m_shortName = candidate;
                    m_knownArgumentNames[candidate] = { i, KnownNameType::e_Short };
                    break;
                }
            }
        }

        /// @brief Private function which is called when AddArgument function
        /// succeed without errors
        /// @param arg Argument object after all validations check
        void _addArg(const Argument& arg)
        {
            size_t currentSize = m_arguments.size();
            if (!arg.m_shortName.empty())
            {
                if (m_knownArgumentNames.find(arg.m_shortName) == m_knownArgumentNames.end())
                {
                    m_knownArgumentNames[arg.m_shortName] = { currentSize, KnownNameType::e_Short };
                }
                else
                {
                    throw std::runtime_error("Short name \"" + arg.m_shortName + "\" already exists");
                }
            }
            if (!arg.m_longName.empty())
            {
                if (m_knownArgumentNames.find(arg.m_longName) == m_knownArgumentNames.end())
                {
                    m_knownArgumentNames[arg.m_longName] = { currentSize, KnownNameType::e_Long };
                }
                else
                {
                    throw std::runtime_error("Long name \"" + arg.m_longName + "\" already exists");
                }
            }
            if (!arg.m_positionalName.empty())
            {
                auto it = std::find_if(m_positionalArgumentNames.begin(), m_positionalArgumentNames.end(), [&arg](PositionalNamesStruct& posarg)->bool {return posarg.argName == arg.m_positionalName; });
                if (it == m_positionalArgumentNames.end())
                {
                    m_positionalArgumentNames.emplace_back(m_arguments.size(), arg.m_positionalName);
                }
                else if (arg.m_required && (arg.m_nargs == 0 || arg.m_nargs == kAnyArgCount))
                {
                    throw std::runtime_error("Required positional argument with name \"" + arg.m_positionalName + "\" cannot be with zero count");
                }
                else if (!arg.m_required && arg.m_nargs != 1)
                {
                    throw std::runtime_error("Non required positional argument with name \"" + arg.m_positionalName + "\" should be with count 1");
                }
                else
                {
                    throw std::runtime_error("Positional name \"" + arg.m_positionalName + "\" already exists");
                }
            }

            m_arguments.emplace_back(arg);
        }

        /// @brief Private function which generates usage according to all argument of program
        /// @param arg Argument instance
        /// @param usage out parameter, which returns usage.
        void MakeUsageForName(Argument& arg, std::stringstream& usage)
        {
            if (!arg.m_required)
            {
                usage << "[";
            }
            std::stringstream showName;
            if (arg.m_choicesDouble.size() + arg.m_choicesInt.size() + arg.m_choicesLongLong.size() + arg.m_choicesString.size())
            {
                showName << "{";
                if (arg.m_type == ArgTypeCast::e_String)
                {
                    MakeChoicesToString<std::string>(showName, arg.m_choicesString);
                }
                else if (arg.m_type == ArgTypeCast::e_int)
                {
                    MakeChoicesToString<int>(showName, arg.m_choicesInt);
                }
                else if (arg.m_type == ArgTypeCast::e_double)
                {
                    MakeChoicesToString<double>(showName, arg.m_choicesDouble);
                }
                else if (arg.m_type == ArgTypeCast::e_longlong)
                {
                    MakeChoicesToString<long long>(showName, arg.m_choicesLongLong);
                }
                showName << "}";
            }
            else
            {
                if (!arg.m_positionalName.empty())
                {
                    showName << arg.m_positionalName;
                }
                else if (!arg.m_shortName.empty())
                {
                    showName << arg.m_shortName;
                }
                else
                {
                    showName << arg.m_longName;
                }
            }
            if (!arg.m_positionalName.empty())
            {
                usage << showName.str();
            }
            if (!arg.m_shortName.empty())
            {
                usage << m_prefix << arg.m_shortName;
            }

            if (!arg.m_longName.empty())
            {
                if (!arg.m_shortName.empty())
                {
                    usage << ",";
                }
                usage << m_prefix << m_prefix << arg.m_longName;
            }

            if (arg.m_nargs == kAnyArgCount)
            {
                usage << " [" << showName.str() << "[" << showName.str() << " ...]]";
            }
            else if (arg.m_nargs == kFromOneToInfiniteArgCount)
            {
                usage << " [" << showName.str() << " ...]";
            }
            else if (arg.m_nargs != 0)
            {
                usage << " [";
                usage << showName.str();
                for (size_t i = 1; i < static_cast<size_t>(arg.m_nargs); ++i)
                {
                    usage << " " << showName.str();
                }
                usage << "]";
            }
            if (!arg.m_required)
            {
                usage << "]";
            }
            // Always separate tokens with exactly one trailing space. This also
            // keeps flags (nargs == 0) from gluing onto the next token.
            usage << " ";
        }

        /// @brief Private function which generates description for every Argument instance which added to parser
        /// This function called from GetUsage functions chain in case if m_addHelp is true.
        /// @param arg Argument instance
        /// @param description string stream to which add description
        /// @param nameLen actual length of names column in usage. If names are longer then nameLen new line will be added before first line of description/
        /// nameLen is calculated according to terminal width and name nameWidthPercent from GetUsage input parameters
        /// @param descLen length of description column of usage.
        void MakeDescriptionForArg(Argument& arg, std::stringstream& description, size_t nameLen, size_t descLen)
        {
            std::stringstream showName;
            std::stringstream showDesc;
            if (!arg.m_positionalName.empty())
            {
                if (arg.m_choicesDouble.size() + arg.m_choicesInt.size() + arg.m_choicesLongLong.size() + arg.m_choicesString.size())
                {
                    showName << "{";
                    if (arg.m_type == ArgTypeCast::e_String)
                    {
                        MakeChoicesToString<std::string>(showName, arg.m_choicesString);
                    }
                    else if (arg.m_type == ArgTypeCast::e_int)
                    {
                        MakeChoicesToString<int>(showName, arg.m_choicesInt);
                    }
                    else if (arg.m_type == ArgTypeCast::e_double)
                    {
                        MakeChoicesToString<double>(showName, arg.m_choicesDouble);
                    }
                    else if (arg.m_type == ArgTypeCast::e_longlong)
                    {
                        MakeChoicesToString<long long>(showName, arg.m_choicesLongLong);
                    }
                    showName << "}";
                }
                else
                {
                    showName << arg.m_positionalName;
                }
            }
            else if (!arg.m_shortName.empty())
            {
                showName << m_prefix << arg.m_shortName;
                if (!arg.m_longName.empty())
                {
                    showName << "," << m_prefix << m_prefix << arg.m_longName;
                }
            }
            else
            {
                // Long name only: no short name, so no leading comma.
                showName << m_prefix << m_prefix << arg.m_longName;
            }

            showDesc << arg.m_help;
            if (arg.m_nargs)
            {
                showDesc << (arg.m_help.empty() ? "" : " ") << "Type: " << m_enumToString[arg.m_type] << ". ";
            }

            if ((!arg.m_shortName.empty() || !arg.m_longName.empty())
                && (arg.m_choicesDouble.size() + arg.m_choicesInt.size() + arg.m_choicesLongLong.size() + arg.m_choicesString.size()))
            {
                showDesc << " Choices:";
                if (arg.m_type == ArgTypeCast::e_String)
                {
                    MakeChoicesToString<std::string>(showDesc, arg.m_choicesString);
                }
                else if (arg.m_type == ArgTypeCast::e_int)
                {
                    MakeChoicesToString<int>(showDesc, arg.m_choicesInt);
                }
                else if (arg.m_type == ArgTypeCast::e_double)
                {
                    MakeChoicesToString<double>(showDesc, arg.m_choicesDouble);
                }
                else if (arg.m_type == ArgTypeCast::e_longlong)
                {
                    MakeChoicesToString<long long>(showDesc, arg.m_choicesLongLong);
                }
                showDesc << ". ";
            }

            showDesc << (arg.m_nargs ? "Args count: " : "");
            switch (arg.m_nargs)
            {
            case kAnyArgCount:
                showDesc << "any. ";
                break;
            case kFromOneToInfiniteArgCount:
                showDesc << " at least one. ";
                break;
            case 0:
                break;
            default:
                showDesc << arg.m_nargs << " ";
                break;
            }
            description << showName.str();
            size_t currentLen = getStringStreamLength(showName);

            size_t spaceFillerSize = nameLen - currentLen;
            if (currentLen >= nameLen - 1)
            {
                description << "\n";
                spaceFillerSize = nameLen;
            }

            // generate space after names printed
            std::string filler(spaceFillerSize, ' ');
            description << filler;

            // generate space for new line of description
            currentLen = 0;

            filler = std::string(nameLen, ' ');

            for (std::string s; showDesc >> s; )
            {
                if (currentLen > descLen)
                {
                    currentLen = 0;
                    description << "\n" << filler;
                }
                description << s << " ";
                currentLen += s.size() + 1;
            }

            description << "\n";
        }


        /// @brief Private function to add epilogue or program description for program help if
        /// auto-generated help is requested.
        /// @param description Current description stream
        /// @param additionalDesc Description to add
        /// @param descLen length of terminal
        void AddAdditionalDescription(std::stringstream& description, const std::string& additionalDesc, size_t descLen)
        {
            description << "\n";
            size_t currentLen = 0;
            std::stringstream buffer{ additionalDesc };
            for (std::string s; buffer >> s; )
            {
                if (currentLen > descLen)
                {
                    currentLen = 0;
                    description << "\n";
                }
                description << s << " ";
                currentLen += s.size() + 1;
            }
        }


        /// @brief Private function which generates choices for Argument instance description, if argument has
        /// any choices
        /// @tparam T type of choices vector
        /// @param outSstream output string stream
        /// @param choices vector of available choices.
        template<typename T>
        void MakeChoicesToString(std::stringstream& outSstream, std::vector<T>& choices)
        {
            outSstream << choices.front();
            for (size_t i = 1; i < choices.size(); ++i)
            {
                outSstream << ", " << choices[i];
            }
        }

    private:
        enum class KnownNameType :int
        {
            e_Short,
            e_Long
        };
        struct KnownNamesStruct
        {
            size_t position;
            KnownNameType argNameType;
        };

        struct PositionalNamesStruct
        {
            PositionalNamesStruct(const size_t position, const std::string& name)
                :positionInArguments(position)
                , argName(name)
            {}
            size_t positionInArguments;
            std::string argName;
        };

    private:
        /// @brief allow generate short names for named arguments, short name not preset
        bool        m_allowAbbrev = true;
        /// @brief guards GenerateAbbreviations so it runs at most once
        bool        m_abbrevGenerated = false;
        /// @brief generate help automatically
        bool        m_addHelp = true;
        /// @brief fail parsing if unknown argument is passed to command line
        bool        m_ignoreUnknownArgs = false;
        /// @brief prefix for short for named arguments
        char        m_prefix = '-';

        /// @brief name of program which would be occur in command line
        /// if auto generated help is required
        std::string m_name;
        /// @brief description of program which should be shown in help
        /// if auto generated help is required
        std::string m_description{ "" };
        /// @brief epilogue of program which should be placed after arguments list
        /// if auto generated help is required
        std::string m_epilogue{ "" };
        /// @brief usage examples of program
        /// if auto generated help is required
        std::string m_usage{ "" };
        /// @brief holder of defined arguments
        std::vector<Argument> m_arguments;
        /// @brief holder positional arguments with they position
        std::vector<PositionalNamesStruct>      m_positionalArgumentNames;
        /// @brief map with arguments which added explicitly
        std::map<std::string, KnownNamesStruct> m_knownArgumentNames;
        /// @brief generated map with arguments after add short names if needed and so on
        /// actual map which will be used for parsing
        std::map<std::string, KnownNamesStruct> m_knownArgumentNamesInternal;

        std::map<const ArgTypeCast, const std::string> m_enumToString
        {
            {ArgTypeCast::e_String,     "STRING" },
            {ArgTypeCast::e_int,        "INT" },
            {ArgTypeCast::e_longlong,   "LONG_LONG"},
            {ArgTypeCast::e_double,     "DOUBLE"},
            {ArgTypeCast::e_bool,       "BOOL"}
        };
    };

}}//qor::app

#endif//QOR_PP_H_APP_ARGUMENTPARSER