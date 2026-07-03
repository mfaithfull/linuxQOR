
#ifndef QOR_PP_H_NULLPROFILER
#define QOR_PP_H_NULLPROFILER

#include "src/qor/tdd/profiling/iprofilereceiver.h"

namespace qor {
    
	class NullProfiler final
	{
	public:

		constexpr NullProfiler(IProfileReceiver*, bool) {}
		~NullProfiler() = default;
	};

}//qor

#endif//QOR_PP_H_NULLPROFILER
