// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_MODULE
#define QOR_PP_H_MODULE

#include "library.h"

namespace qor{

    class ModuleRegistry;

	class Module : public Library
    {
    public:

        Module(const char* name, const char* version, bool _register = true);
        virtual ~Module() noexcept;

		bool RegisterLibrary(Library* library);         //Register a static library as part of this module
		void UnregisterLibrary(Library* library);       //Unregister a static library from this module

        inline void VisitLibraries(void(f)(Library*))
        {
            for(Library* it = m_StaticLibraryList; it != nullptr; it = const_cast<Library*>(it->Next()))
            {
                f(it);
            }
        }

        virtual void RegisterModule(Module* /*module*/){ };
        virtual void UnregisterModule(Module* /*module*/){ };

        ModuleRegistry* Modules();

    protected:

        Library* m_StaticLibraryList{nullptr};
        ModuleRegistry* m_ModuleReg{nullptr};

    private:

        Module() = delete;
        Module(const Module&) = delete;
        Module& operator = (const Module&) = delete;
    };
}//qor

extern "C" qor::Module& ThisModule();       //Each and every module that includes this header must provide a ThisModule function returning its module representation object.

#define qor_pp_implement_module(_X)\
extern "C"{\
    qor::Module& ThisModule(void)\
    {\
	    static qor::Module QORModule(_X, \
            qor_pp_stringize(qor_pp_ver_major) "." qor_pp_stringize(qor_pp_ver_minor) "." qor_pp_stringize(qor_pp_ver_patch));\
	    return QORModule;\
    }\
}

#endif//QOR_PP_H_MODULE
