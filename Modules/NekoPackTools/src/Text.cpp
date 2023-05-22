#include "Text.h"
#include "Rxx/include/Str.h"
#include "Rxx/include/Console.h"

#include <fstream>
#include <iomanip>

using namespace Rut::StrX;
using namespace Rut::ConsoleX;


namespace NekoPackTools
{
	namespace Script
	{
		bool Text::Extract(const std::wstring& wsScript)
		{
			ReadScript(wsScript, m_uReadCodePage);
			FilterCode();
			WriteText(wsScript + L".txt");

			return true;
		}

		bool Text::Insert(const std::wstring& wsScript)
		{
			ReadScript(wsScript, m_uReadCodePage);
			ReadText(wsScript + L".txt");
			InsertText(m_uInsetCodePage);
			WriteScript(wsScript + L".new");

			return true;
		}

		void Text::SetCodePage(std::size_t uReadCodePage, std::size_t uInsetCodePage)
		{
			m_uReadCodePage = uReadCodePage;
			m_uInsetCodePage = uInsetCodePage;
		}

		bool Text::WriteScript(const std::wstring& wsScript)
		{
			std::ofstream ofs_Script(wsScript, std::ios::binary);
			if (!ofs_Script.is_open()) { return false; }

			for (auto& line : m_vecScript)
			{
				ofs_Script << WStrToStr(line.wsScript, line.uEncoding) << '\n';
			}

			ofs_Script.flush();

			return true;
		}

		bool Text::ReadScript(const std::wstring& wsScript, const size_t uCodePage)
		{
			m_vecScript.clear();

			std::ifstream ifs_Script(wsScript);
			if (!ifs_Script.is_open()) { return false; }

			for (std::string line; getline(ifs_Script, line);)
			{
				m_vecScript.emplace_back(Script_Line{ uCodePage, StrToWStr(line, uCodePage) });
			}

			return false;
		}

		bool Text::ReadText(const std::wstring& wsText)
		{
			m_vecText.clear();

			std::wifstream ifs_Text(wsText); ifs_Text.imbue(GetCVT_UTF8());
			if (!ifs_Text.is_open()) { return false; }

			Text_Line text_line = { 0 };
			for (std::wstring line; getline(ifs_Text, line);)
			{
				if (line.find(L"INF:") != 0) { continue; }

				text_line.uNumber = std::stoi(line.substr(4));

				getline(ifs_Text, line);
				getline(ifs_Text, line);

				if (line.find(L"Tra:") != 0) { return false; }

				text_line.wsTraText = line.substr(4);

				m_vecText.emplace_back(text_line);
			}

			return true;
		}

		bool Text::WriteText(const std::wstring& wsText)
		{
			std::wofstream ofs_Text(wsText); ofs_Text.imbue(GetCVT_UTF8());
			if (!ofs_Text.is_open()) { return false; }

			for (auto& line : m_vecText)
			{
				ofs_Text
					<< L"INF:"
					<< std::setw(8) << std::setfill(L'0')
					<< line.uNumber << '\n';

				ofs_Text
					<< L"Raw:" << line.wsTraText << '\n'
					<< L"Tra:" << line.wsTraText << "\n\n";
			}

			ofs_Text.flush();

			return true;
		}

		bool Text::InsertText(std::size_t uCodePage)
		{
			for (auto& line : m_vecText)
			{
				std::size_t& number = line.uNumber;
				std::wstring& text = line.wsTraText;

				std::size_t& encoding = m_vecScript[number - 1].uEncoding;
				std::wstring& script = m_vecScript[number - 1].wsScript;

				if (text.size() == 0) { continue; }
				if (script.size() == 0) { continue; }

				encoding = uCodePage;

				if (script[0] == L':')
				{
					script = RepCharaName(script, text);
				}
				else
				{
					script = text;
				}
			}

			return true;
		}

		bool Text::FilterCode()
		{
			std::size_t number = 0;
			Text_Line text_line = { 0 };
			for (auto& line : m_vecScript)
			{
				number++;

				std::wstring& script = line.wsScript;

				if (script.size() == 0) continue;

				switch (script[0])
				{
				case L'#': break;
				case L'.': break;
				case L';': break;
				case L'\t': break;
				case L':': 
				{
					text_line.uNumber = number;
					text_line.wsTraText = GetCharaName(script);
					m_vecText.emplace_back(text_line);
				} 
				break;

				default: 
				{
					text_line.uNumber = number;
					text_line.wsTraText = script;
					m_vecText.emplace_back(text_line);
				} 
				break;
				}
			}

			return true;
		}

		std::wstring Text::RepCharaName(const std::wstring& wsLine, const std::wstring& wsName)
		{
			if (wsLine[0] != L':')
			{
				PutConsoleW(L"Not Match Character Name:%s\n", wsLine);
				return wsLine;
			}

			std::wstring newLine = wsLine;
			newLine.erase(1, wsLine.find(L":", 1) - 1);
			newLine.insert(1, wsName);
			return newLine;
		}

		std::wstring Text::GetCharaName(const std::wstring& wsLine)
		{
			if (wsLine[0] != L':')
			{
				PutConsoleW(L"Not Match Character Name:%s\n", wsLine);
				return wsLine;
			}

			return wsLine.substr(1, wsLine.find(L":", 1) - 1);
		}
	}
}
