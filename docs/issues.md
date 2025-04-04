An error is an issue but not all issues are errors

The are all sorts of issues we may want to raise from code. Fatal errors, less serious errors, warnings etc are a severity axis but there may be other axes and other purposes

qor_issue provies a general issue template and allows for the collection of contextual information about when and where the issue was raised. The details of what the issue is are left to be added by extension.

qor_error extends qor_issue with severity based error categories and flyer based handlers. Errors can be ignored or escalated.

note        lowest level, can't be escalted
warning     nag level but can be escalted
continuable a real error but execution can carry on
serious     a serious error, execution may be able to conintue but you may prefer a quick exit
fatal       Unresolvable, will terminate even if 'handled' successfully.



