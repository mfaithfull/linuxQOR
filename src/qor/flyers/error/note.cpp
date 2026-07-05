// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "error.h"
#include "handler.h"

namespace qor{

    Note::Note(const std::string& message) : ErrorBase(message, Severity::Note){ }
    Note::Note(const Note& src) : ErrorBase(src){ }
    Note::~Note(){ }

    Note& Note::operator = (const Note& src)
    {
        ErrorBase::operator = (src);
        return *this;
    }

    void Note::Handle()
    {
        auto noteHandler = new_ref< IssueHandler<Note> >();
        if(noteHandler.IsNotNull())
        {
            noteHandler->Handle(*this);
        }
        else
        {
            auto handler = new_ref< IssueHandler<ErrorBase> >();
            Resolve(handler.IsNotNull() ? handler->Handle(*this) : true);
        }
    }
    
    void Note::Escalate() const {}
}//qor
