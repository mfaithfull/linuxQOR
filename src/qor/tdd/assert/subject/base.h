// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

//Derived from assertcc
//Copyright 2021 Sean Nash
//under BSD 3 clause license

#ifndef QOR_PP_H_ASSERT_SUBJECT_BASE
#define QOR_PP_H_ASSERT_SUBJECT_BASE

namespace qor{ namespace assert{ namespace subject{

    /*Base class that holds shared information between the subject classes and the
    proposition mixins.

    The subject class will intilize the Base superclass and the proposition
    classes will use the functions to retrive the data. Virtual inheritance is
    used to avoid the diamond issue.
    */
    class Base 
    {
        private:
    
            const char* d_file; //The source file of the assert_that statement was called in.
            const int d_line;/// The line number in the file the assert_that statement is on.
            const bool d_failOnError;/// A flag to determine whether a test stops at the error or continues.

        protected:

            const char* getFile() const 
            { 
                return d_file; 
            }

            int getLine() const 
            { 
                return d_line; 
            }

            bool getFailOnError() const 
            { 
                return d_failOnError; 
            }

        public:

            Base(const bool failOnError, const char* file, int line): d_file(file), d_line(line), d_failOnError(failOnError) {}
    };

}}}//qor::assert::subject

#endif//QOR_PP_H_ASSERT_SUBJECT_BASE