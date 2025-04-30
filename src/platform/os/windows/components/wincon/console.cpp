

#include "src/configuration/configuration.h"

#include "console.h"
#include "src/qor/error/error.h"
#include "src/platform/os/windows/winqapi/kernel/kernel32.h"

using namespace qor::winapi;

namespace qor {
	bool qor_pp_module_interface(QOR_WINCONSOLE) ImplementsIConsole() //Implement this trivial function so the linker will pull in this library to fulfil the ImplementsIConsole requirement. 
	{
		return true;
	}
}//qor

namespace qor { namespace nsWindows {

	Console::Console() : m_redirected(false), m_allocated(false)
	{
		auto hstdin = m_helper.GetStdHandle(ConsoleHelper::hStdIn);
		if (hstdin.IsNull() || hstdin.IsInvalid())
		{
			m_helper.Alloc();
			m_allocated = true;
			ResetIn();
		}

		auto hstdout = m_helper.GetStdHandle(ConsoleHelper::hStdOut);
		if (hstdout.IsNull() || hstdout.IsInvalid())
		{
			if (!m_allocated)
			{
				m_helper.Alloc();
				ResetIn();
				ResetErr();
			}
			ResetOut();
		}

		DWORD mode = 0;
		Kernel32::GetConsoleMode(hstdin.Use(), &mode);
		mode &= ~(ENABLE_LINE_INPUT);
		Kernel32::SetConsoleMode(hstdin.Use(), mode);

	}

	string_t Console::ReadLine()
	{
		if (!m_redirected)
		{
			DWORD mode = 0;
			auto hstdin = m_helper.GetStdHandle(ConsoleHelper::hStdIn);
			Kernel32::GetConsoleMode(hstdin.Use(), &mode);
			string_t str;
			DWORD charsRead = 0;
			str.reserve(1024);
			str.resize(1024);
			CONSOLE_READCONSOLE_CONTROL readControl;
			readControl.nLength = sizeof(CONSOLE_READCONSOLE_CONTROL);
			readControl.nInitialChars = 0;
			readControl.dwCtrlWakeupMask = 0x0D;
			readControl.dwControlKeyState = 0;
			bool result = false;
			do
			{
				DWORD newCharsRead = 0;				
				result = Kernel32::ReadConsole(m_helper.GetStdHandle(ConsoleHelper::hStdIn).Use(), str.data()+charsRead, 1023, &newCharsRead, &readControl);
				if (result && newCharsRead > 0)
				{
					charsRead += newCharsRead;
					if (mode & ENABLE_ECHO_INPUT)
					{	//echo is broken with line input turned off so we have to do it ourselves
						WriteChar(str.data()[charsRead - 1]);
					}
					if (charsRead >= 1023 || str.data()[charsRead - 1] == '\r')
					{
						str.resize(charsRead-1);
						return str;
					}
				}
			} while (result);
			return str;
		}
		return string_t();
	}

	char_t Console::ReadChar()//get next key press including key combo stuff - echo by default except it doesn't
	{
		if (!m_redirected)
		{
			char_t c;
			DWORD charsRead = 0;
			CONSOLE_READCONSOLE_CONTROL readControl;
			readControl.nLength = sizeof(CONSOLE_READCONSOLE_CONTROL);
			readControl.nInitialChars = 0;
			readControl.dwCtrlWakeupMask = 0x0D;
			readControl.dwControlKeyState = 0;
			bool result = Kernel32::ReadConsole(m_helper.GetStdHandle(ConsoleHelper::hStdIn).Use(), &c, 1, &charsRead, &readControl);
			if (result && charsRead == 1)
			{
				return c;
			}
		}
		return 0;
	}

	void Console::WriteChar(char_t c)
	{
		if (!m_redirected)
		{
			DWORD charsWritten = 0;
			bool result = Kernel32::WriteConsole(m_helper.GetStdHandle(ConsoleHelper::hStdOut).Use(), &c, 1, &charsWritten, nullptr);
			if (!(result && charsWritten == 1))
			{
				continuable("Call to Kernel32::WriteConsole failed");
			}
		}
	}

	void Console::WriteLine(string_t& output)
	{
		if (!m_redirected)
		{
			DWORD charsWritten = 0;
			bool result = Kernel32::WriteConsole(m_helper.GetStdHandle(ConsoleHelper::hStdOut).Use(), output.data(), static_cast<DWORD>(output.size()), &charsWritten, nullptr);
			if (!(result && charsWritten == output.size()))
			{
				continuable("Call to Kernel32::WriteConsole failed");
			}
		}
	}

}}//qor::nsWindows
