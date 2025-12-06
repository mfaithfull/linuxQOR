
#ifndef QOR_PP_H_RETROPAD_MODEL
#define QOR_PP_H_RETROPAD_MODEL

#include "src/platform/os/windows/common/structures.h"
#include "src/platform/os/windows/gui/window.h"
#include "src/framework/pipeline/podbuffer.h"
#include "src/platform/os/windows/api_layer/kernel/kernel32.h"

constexpr int MAX_PATH_BUFFER  = 1024;
constexpr int FIND_SIZE  = 128;
constexpr int REPLACE_SIZE  = 128;

extern const stdstring APP_TITLE;
extern const stdstring UNTITLED_NAME;

enum TextEncoding 
{
    ENC_UTF8 = 1,
    ENC_UTF16LE = 2,
    ENC_UTF16BE = 3,
    ENC_ANSI = 4
};

class ModelState 
{
public:

    void SetCurrentPath(const stdstring& path);
    stdstring& GetCurrentPath();
    size_t GetCurrentPathLength();
    void SetModified();
    void ClearModified();
    bool GetModified();
    void SetEncoding(TextEncoding enc);
    TextEncoding GetEncoding();
    void SetFindFlags(unsigned int);
    unsigned int GetFindFlags();
    void SetFindText(const stdstring& find);
    stdstring& GetFindText();
    size_t GetFindTextLength();
    void SetReplaceText(const stdstring& replace);
    stdstring& GetReplaceText();
    size_t GetReplaceTextLength();
    qor::pipeline::PODBuffer<qor::platform::nswindows::TCHAR>& Document();
    void InsertTimeDate();
    void DoFileNew();    
    bool LoadDocumentFromPath(stdstring path);
    bool FindInText(const qor::platform::nswindows::TCHAR* needle, bool matchCase, bool searchDown, unsigned long startPos, unsigned long *outStart, unsigned long *outEnd);
    int ReplaceAllOccurrences(bool matchCase);
    bool Save(const stdstring& path);
    bool SaveTextFile(const stdstring& path, const qor::platform::nswindows::TCHAR* text, size_t length, TextEncoding encoding);

private:

    stdstring m_currentPath;
    bool m_modified;
    TextEncoding m_encoding;
    unsigned int m_findFlags;
    stdstring m_findText;
    stdstring m_replaceText;
    qor::pipeline::PODBuffer<qor::platform::nswindows::TCHAR> m_document;
};

#endif//QOR_PP_H_RETROPAD_MODEL