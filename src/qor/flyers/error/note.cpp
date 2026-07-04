// Copyright Querysoft Limited 2008 - Present
// SPDX-License-Identifier: BSL-1.0

#include "src/configuration/configuration.h"
#include "error.h"
#include "handler.h"

namespace qor{

    Note::Note(const std::string& message) : SeverityTemplateIssue<Severity::Note>(message){ }

    Note& Note::operator = (const Note& src)
    {
        SeverityTemplateIssue<Severity::Note>::operator = (src);
        return *this;
    }

    void Note::Handle()
    {
        auto noteHandler = new_ref< IssueHandler<Note> >();
        if(noteHandler.IsNotNull())
        {
            noteHandler->Handle(*this);
        }
        else{ }//Notes without handlers disappear into oblivion.
    }
    
}//qor
