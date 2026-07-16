// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_APPLICATION_BUILDER
#define QOR_PP_H_APPLICATION_BUILDER

#include "application.h"
#include "src/qor/flyers/error/error.h"

namespace qor{

    class AppBuilder
    {
    public:

        qor_pp_module_interface(QOR_APPLICATION) AppBuilder();
        qor_pp_module_interface(QOR_APPLICATION) virtual ~AppBuilder() noexcept;

        qor_pp_module_interface(QOR_APPLICATION) qor_pp_noinline ref_of<Application>::type Build(const std::string& appName/*, const int argc = 0, const char** argv = nullptr, const char** env = nullptr*/);
        qor_pp_module_interface(QOR_APPLICATION) qor_pp_noinline ref_of<Application>::type TheApplication();

        template< class AppClass >
        ref_of<Application>::type Build(const std::string& appName/*, const int argc = 0, const char** argv = nullptr, const char** env = nullptr*/)
        {
            auto application = new_ref<AppClass>().template AsRef<Application>();
            AutoRedirect(application);            
            application.template AsRef<Application>()(qor_shared).Name() = appName;
            return application;
        }

        template<class AppClass, typename TConfigureApp>
        ref_of<Application>::type Build(const std::string& appName, TConfigureApp&& config_function)
        {
            auto app = new_ref<AppClass>();
            AutoRedirect(app.template AsRef<Application>());                        
            app.template AsRef<Application>()(qor_shared).Name() = appName;
            try
            {
                config_function(app);
            }
            catch(const Serious& s)
            {
                std::cerr << s.what().Content() << std::endl;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << std::endl;
            }
            catch(...)
            {
                std::cerr << "Unhandled exception" << std::endl;
            }            
            return app.template AsRef<Application>();
        }

    private:

        qor_pp_module_interface(QOR_APPLICATION) void AutoRedirect(ref_of<Application>::type application);
    };
    
}//qor

#endif//QOR_PP_H_APPLICATION_BUILDER
