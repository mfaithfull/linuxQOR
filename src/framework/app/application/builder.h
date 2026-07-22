// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#ifndef QOR_PP_H_APPLICATION_BUILDER
#define QOR_PP_H_APPLICATION_BUILDER

#include <type_traits>
#include "application.h"
#include "src/qor/flyers/error/error.h"

#define qor_pp_run_workflow(_WORKFLOW) return AppBuilder().Build(ThisModule().Name())(qor_unlocked).SetRole<qor::app::Role>().RunWorkflow<_WORKFLOW>();
#define qor_pp_run_role(_ROLE) return AppBuilder().Build(ThisModule().Name())(qor_unlocked).SetRole<_ROLE>().Run
#define qor_pp_run qor_pp_run_role(qor::app::Role)

namespace qor{

    class AppBuilder
    {
    public:
    
        qor_pp_module_interface(QOR_APPLICATION) AppBuilder();
        qor_pp_module_interface(QOR_APPLICATION) virtual ~AppBuilder() noexcept;

        qor_pp_module_interface(QOR_APPLICATION) qor_pp_noinline ref_of<Application>::type Build(const std::string& appName);
        qor_pp_module_interface(QOR_APPLICATION) qor_pp_noinline ref_of<Application>::type TheApplication();

        template< class AppClass > requires std::is_base_of_v<Application, AppClass>
        ref_of<AppClass>::type Build(const std::string& appName)
        {
            auto app = new_ref<AppClass>();
            auto application = app.template AsRef<Application>();
            AutoRedirect(application);            
            application.template AsRef<Application>()(qor_shared).Name() = appName;
            return app;
        }

        template<class AppClass, typename TConfigureApp> requires std::is_base_of_v<Application, AppClass>
        ref_of<AppClass>::type Build(const std::string& appName, TConfigureApp&& config_function)
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
            return app;
        }

    private:

        qor_pp_module_interface(QOR_APPLICATION) void AutoRedirect(ref_of<Application>::type application);
    };
    
}//qor

#endif//QOR_PP_H_APPLICATION_BUILDER
