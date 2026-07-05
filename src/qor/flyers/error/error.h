// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_ERROR_ERROR
#define QOR_PP_H_ERROR_ERROR

#include <format>
#include "errorbase.h"
#include "src/qor/essentials/text/strings/strings.h"

namespace qor{ 

    class qor_pp_module_interface(QOR_ERROR) Fatal : public ErrorBase
    {
    public:
        Fatal(const std::string& message);
        Fatal(const Fatal& src);
        virtual ~Fatal() noexcept;
        Fatal& operator = (const Fatal& src);
        virtual void Handle();
        virtual void Escalate() const;
        virtual void Ignore() const;
    };

    template< typename... _p >
    void fatal(const std::string& message, _p&&... p1)
    {
        issue<Fatal, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
    }

    inline void fatal(const std::string& message)
    {
        issue<Fatal, const std::string&>(message);
    }    

    class qor_pp_module_interface(QOR_ERROR) Serious : public ErrorBase
    {
    public:
        Serious(const std::string& message);
        Serious(const Serious& src);
        virtual ~Serious() noexcept;
        Serious& operator = (const Serious& src);
        virtual void Handle();
        virtual void Escalate() const;
        virtual void Ignore() const;
    };
    
    template< typename... _p >
    void serious(const std::string& message, _p&&... p1)
    {
        issue<Serious, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
    }

    inline void serious(const std::string& message)
    {
        issue<Serious, const std::string&>(message);
    }    

    class qor_pp_module_interface(QOR_ERROR) Continuable : public ErrorBase
    {
    public:
        Continuable(const std::string& message);
        Continuable(const Continuable& src);
        virtual ~Continuable() noexcept;
        Continuable& operator = (const Continuable& src);
        virtual void Handle();
        virtual void Escalate() const;
        virtual void Ignore() const;
    };
    
    template< typename... _p >
    void continuable(const std::string& message, _p&&... p1)
    {
        issue<Continuable, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
    }

    inline void continuable(const std::string& message)
    {
        issue<Continuable, const std::string&>(message);
    }    


    class qor_pp_module_interface(QOR_ERROR) Warning : public ErrorBase
    {
    public:
        Warning(const std::string& message);
        Warning(const Warning& src);
        virtual ~Warning() noexcept;        
        Warning& operator = (const Warning& src);
        virtual void Handle();
        virtual void Escalate() const;
    };
    
    template< typename... _p >
    void warning(const std::string& message, _p&&... p1)
    {
        issue<Warning, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
    }

    inline void warning(const std::string& message)
    {
        issue<Warning, const std::string&>(message);
    }    

    class qor_pp_module_interface(QOR_ERROR) Note : public ErrorBase
    {
    public:
        Note(const std::string& message);
        Note(const Note& src);
        virtual ~Note() noexcept;
        Note& operator = (const Note& src);
        virtual void Handle();
        virtual void Escalate() const;
    };

    template< typename... _p >
    void note(const std::string& message, _p&&... p1)
    {
        issue<Note, const std::string&>(std::vformat(std::string_view(message), std::make_format_args(p1...)));
    }

    inline void note(const std::string& message)
    {
        issue<Note, const std::string&>(message);
    }

}//qor

#endif//QOR_PP_H_ERROR_ERROR
