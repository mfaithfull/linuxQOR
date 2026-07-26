// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_APP_PARSEDARGUMENT
#define QOR_PP_H_APP_PARSEDARGUMENT

#include "argument.h"

namespace qor { namespace app {

    /// @brief Class which represent actual parsed argument in case of successfully parsing
    class ParsedArgument
    {
    public:
        /// @brief Does argument exists. Needed to check in case when argument is not required
        /// @return bool value
        bool GetArgumentExists()
        {
            return m_exists;
        }

        /// @brief get actual count of argument, in case of various arguments count.
        /// @return 
        size_t GetArgumentCount()
        {
            return m_count;
        }

        /// @brief Get result as single bool for bool type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return bool value of argument
        /// @throws std::out_of_range if the argument holds no value
        bool GetAsBool() const
        {
            ThrowIfEmpty(m_bool.empty(), "GetAsBool");
            return m_bool.front();
        }

        /// @brief Get result as single int for int type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return int value of argument
        /// @throws std::out_of_range if the argument holds no value
        int GetAsInt() const
        {
            ThrowIfEmpty(m_int.empty(), "GetAsInt");
            return m_int.front();
        }

        /// @brief Get result as single long long for long long type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return long long value of argument
        /// @throws std::out_of_range if the argument holds no value
        long long GetAsLongLong() const
        {
            ThrowIfEmpty(m_longLong.empty(), "GetAsLongLong");
            return m_longLong.front();
        }


        /// @brief Get result as single double for double type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return double value of argument
        /// @throws std::out_of_range if the argument holds no value
        double GetAsDouble() const
        {
            ThrowIfEmpty(m_double.empty(), "GetAsDouble");
            return m_double.front();
        }


        /// @brief Get result as single string for string type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return string value of argument (returned by value so it never dangles)
        /// @throws std::out_of_range if the argument holds no value
        std::string GetAsString() const
        {
            ThrowIfEmpty(m_string.empty(), "GetAsString");
            return m_string.front();
        }

        /// @brief Get result as vector bool for bool type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return vector bool value of argument (by value: safe to store even when
        /// called on a temporary ParsedArgument returned by GetArg())
        std::vector<bool> GetAsVecBool() const
        {
            return m_bool;
        }

        /// @brief Get result as vector int for int type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return vector int value of argument (by value, see GetAsVecBool)
        std::vector<int> GetAsVecInt() const
        {
            return m_int;
        }

        /// @brief Get result as vector long long for long long type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return vector long long value of argument (by value, see GetAsVecBool)
        std::vector<long long> GetAsVecLongLong() const
        {
            return m_longLong;
        }

        /// @brief Get result as vector double for double type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return vector double value of argument (by value, see GetAsVecBool)
        std::vector<double> GetAsVecDouble() const
        {
            return m_double;
        }

        /// @brief Get result as vector string for string type arguments. Added for c++11 support.
        /// Starting from c++17 you can use Get()
        /// @return vector string value of argument (by value, see GetAsVecBool)
        std::vector<std::string> GetAsVecString() const
        {
            return m_string;
        }

#if __cplusplus > 201402L || _MSVC_LANG > 201402L
        /// @brief Function to get actual argument value
        /// @return std::any: could be bool, int, long long, double, string 
        /// and vector variants of same types regarding of arguments type.
        std::any Get()
        {
            switch (m_type)
            {
            case ArgTypeCast::e_String:
                if (m_count == 1)
                {
                    return m_string.front();
                }
                return m_string;
                break;
            case ArgTypeCast::e_int:
                if (m_count == 1)
                {
                    return m_int.front();
                }
                return m_int;
                break;
            case ArgTypeCast::e_longlong:
                if (m_count == 1)
                {
                    return m_longLong.front();
                }
                return m_longLong;
                break;
            case ArgTypeCast::e_double:
                if (m_count == 1)
                {
                    return m_double.front();
                }
                return m_double;
                break;
            case ArgTypeCast::e_bool:
            default:
                if (m_count == 1)
                {
                    return m_bool.front();
                }
                return m_bool;
                break;
            }
        }

#endif // __cplusplus >= 


    protected:

        ParsedArgument() {}

        /// @brief Guard for the scalar getters. Turns an out-of-bounds front()
        /// (undefined behavior) into a clear, catchable exception. Call
        /// GetArgumentExists()/GetArgumentCount() first to avoid it.
        static void ThrowIfEmpty(bool empty, const char* getter)
        {
            if (empty)
            {
                throw std::out_of_range(std::string("ParsedArgument::") + getter
                    + "() called on an argument that holds no value");
            }
        }

        /// @brief flag about is argument exists
        bool        m_exists{ false };
        /// @brief type of argument
        ArgTypeCast m_type{ ArgTypeCast::e_String };
        /// @brief count of arguments properties
        size_t      m_count{ 0 };

        /// @brief container of parsed bool arguments
        std::vector<bool>        m_bool = {};
        /// @brief container of parse int arguments
        std::vector<int>         m_int = {};
        /// @brief container of parsed long long arguments
        std::vector<long long>   m_longLong = {};
        /// @brief container of parsed double arguments
        std::vector<double>      m_double = {};
        /// @brief container of parsed string arguments
        std::vector<std::string> m_string = {};

        friend ParsedArgsObject;
    };

    // ---- Argument::BindTo definitions -------------------------------------
    // Defined out-of-line (but still inline) because they read values through
    // ParsedArgument's typed getters, which only become complete right here.
    // Each overload also fixes the argument type to match the bound variable.

    inline Argument& Argument::BindTo(bool* target)
    {
        m_type = ArgTypeCast::e_bool;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsBool(); };
        return *this;
    }
    
    inline Argument& Argument::BindTo(int* target)
    {
        m_type = ArgTypeCast::e_int;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsInt(); };
        return *this;
    }

    inline Argument& Argument::BindTo(long long* target)
    {
        m_type = ArgTypeCast::e_longlong;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsLongLong(); };
        return *this;
    }

    inline Argument& Argument::BindTo(double* target)
    {
        m_type = ArgTypeCast::e_double;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsDouble(); };
        return *this;
    }

    inline Argument& Argument::BindTo(std::string* target)
    {
        m_type = ArgTypeCast::e_String;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsString(); };
        return *this;
    }

    inline Argument& Argument::BindTo(std::vector<bool>* target)
    {
        m_type = ArgTypeCast::e_bool;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsVecBool(); };
        return *this;
    }

    inline Argument& Argument::BindTo(std::vector<int>* target)
    {
        m_type = ArgTypeCast::e_int;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsVecInt(); };
        return *this;
    }

    inline Argument& Argument::BindTo(std::vector<long long>* target)
    {
        m_type = ArgTypeCast::e_longlong;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsVecLongLong(); };
        return *this;
    }

    inline Argument& Argument::BindTo(std::vector<double>* target)
    {
        m_type = ArgTypeCast::e_double;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsVecDouble(); };
        return *this;
    }

    inline Argument& Argument::BindTo(std::vector<std::string>* target)
    {
        m_type = ArgTypeCast::e_String;
        m_binding = [target](const ParsedArgument& parsed) { *target = parsed.GetAsVecString(); };
        return *this;
    }

}}//qor::app

#endif//QOR_PP_H_APP_PARSEDARGUMENT