#pragma once
#include <vector>
#include <string>

namespace NekoPackTools
{
	namespace Script
	{
		struct Script_Line
		{
			std::size_t uEncoding;   //Encoding
			std::wstring wsScript;   //Original Script Text
		};

		struct Text_Line
		{
			std::size_t uNumber;     //Line Number In Script
			std::wstring wsTraText;  //Translated Text
		};

		class Text
		{
		private:
			std::size_t m_uReadCodePage;
			std::size_t m_uInsetCodePage;
			std::vector<Text_Line> m_vecText;
			std::vector<Script_Line> m_vecScript;

		public:
			Text() : m_uReadCodePage(932), m_uInsetCodePage(932) { }

			bool Extract(const  std::wstring& wsScript);
			bool Insert(const std::wstring& wsScript);
			void SetCodePage(std::size_t uReadCodePage, std::size_t uInsetCodePage);

		private:
			bool ReadScript(const  std::wstring& wsScript, const size_t uCodePage);
			bool WriteScript(const std::wstring& wsScript);

			bool ReadText(const std::wstring& wsText);
			bool WriteText(const std::wstring& wsText);

			bool InsertText(std::size_t uCodePage);
			bool FilterCode();

			std::wstring RepCharaName(const std::wstring& wsLine, const std::wstring& wsName);
			std::wstring GetCharaName(const std::wstring& wsLine);
		};
	}
}