
Link time dependency for qor base libraries

qor_module          :
qor_host            : qor_module
qor_injection       : qor_module, qor_host
qor_reporting       : qor_module, qor_host
qor_sync            : qor_module, qor_host
qor_text            : qor_module, qor_host, qor_reporting
qor_current         : qor_module, qor_host, qor_injection
qor_interception    : qor_module, qor_host, qor_current
qor_issue           : qor_module, qor_host, qor_current
qor_property        : qor_module, qor_host, qor_reporting, qor_current
qor_error           : qor_module, qor_host, qor_reporting, qor_current, qor_text, qor_interception, qor_issue 
qor_thread          : qor_module, qor_host, qor_current, qor_sync, qor_injection
qor_signals         : qor_module, qor_host, qor_thread
qor_log             : qor_module, qor_host, qor_current, qor_interception, qor_issue, qor_error, qor_signals
qor_threadmemory    : qor_module, qor_host, qor_thread, qor_sync

