
#ifndef QOR_PP_H_RETROPAD_MODEL
#define QOR_PP_H_RETROPAD_MODEL

#include "src/platform/os/windows/common/structures.h"
#include "src/platform/os/windows/gui/window.h"
#include "src/framework/pipeline/podbuffer.h"

constexpr int MAX_PATH_BUFFER  = 1024;
constexpr int FIND_SIZE  = 128;
constexpr int REPLACE_SIZE  = 128;

extern const qor::platform::nswindows::TCHAR* APP_TITLE;// = TEXT("Retropad");
extern const qor::platform::nswindows::TCHAR* UNTITLED_NAME;// = TEXT("Untitled");

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

    void SetCurrentPath(qor::platform::nswindows::TCHAR* path, int charCount);
    qor::platform::nswindows::TCHAR* GetCurrentPath();
    int GetCurrentPathLength();
    void SetModified();
    void ClearModified();
    bool GetModified();
    void SetEncoding(TextEncoding enc);
    TextEncoding GetEncoding();
    void SetFindFlags(unsigned int);
    unsigned int GetFindFlags();
    void SetFindText(qor::platform::nswindows::TCHAR* path, int charCount);
    qor::platform::nswindows::TCHAR* GetFindText();
    int GetFindTextLength();
    void SetReplaceText(qor::platform::nswindows::TCHAR* path, int charCount);
    qor::platform::nswindows::TCHAR* GetReplaceText();
    int GetReplaceTextLength();
    qor::pipeline::PODBuffer<qor::platform::nswindows::TCHAR>& Document();
    void InsertTimeDate();
    void DoFileNew();    
    bool LoadDocumentFromPath(const qor::platform::nswindows::TCHAR* path);
    bool FindInText(const qor::platform::nswindows::TCHAR* needle, bool matchCase, bool searchDown, unsigned long startPos, unsigned long *outStart, unsigned long *outEnd);
    int ReplaceAllOccurrences(bool matchCase);
    bool SaveTextFile(const qor::platform::nswindows::TCHAR* path, const qor::platform::nswindows::TCHAR* text, size_t length, TextEncoding encoding);

private:

    qor::platform::nswindows::TCHAR currentPath[MAX_PATH_BUFFER];
    bool modified;
    TextEncoding encoding;
    unsigned int findFlags;
    qor::platform::nswindows::TCHAR findText[FIND_SIZE];
    qor::platform::nswindows::TCHAR replaceText[REPLACE_SIZE];
    qor::pipeline::PODBuffer<qor::platform::nswindows::TCHAR> m_document;
};

#endif//QOR_PP_H_RETROPAD_MODEL