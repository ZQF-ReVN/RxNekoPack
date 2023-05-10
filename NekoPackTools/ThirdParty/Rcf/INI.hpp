#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <codecvt>


//Ria's Configuration File Library X 
namespace Rcf
{
	// Reference yazi-ini
	namespace INI
	{
		std::locale& GetCVT_UTF8()
		{
			static std::locale cvtUTF8 = std::locale
			(
				std::locale::empty(),
				new std::codecvt_utf8
				<wchar_t, 0x10ffff, std::codecvt_mode(std::consume_header | std::generate_header | std::little_endian)>
			);
			return cvtUTF8;
		}

		class Value
		{
		private:
			std::wstring m_wsValue;

		public:
			Value() { }

			Value(const bool bValue) { m_wsValue = bValue ? L"True" : L"False"; }
			Value(const int iValue) { m_wsValue = std::to_wstring(iValue); }
			Value(const double dValue) { m_wsValue = std::to_wstring(dValue); }
			Value(const wchar_t* wValue) { m_wsValue = wValue; }
			Value(const std::wstring& wsValue) { m_wsValue = wsValue; }

			operator const bool() { return _wcsicmp(m_wsValue.c_str(), L"True") == 0; }
			operator const int() { return std::stoi(m_wsValue); }
			operator const double() { return std::stod(m_wsValue); }
			operator const wchar_t* const () { return m_wsValue.c_str(); }
			operator const std::wstring() const { return m_wsValue; }
			operator std::wstring& () { return m_wsValue; }

			bool Empty() { return m_wsValue.empty(); }
		};

		class INI_File
		{
		private:
			typedef std::wstring                 Name;
			typedef std::wstring                 NodeName;
			typedef std::map<Name, Value>        KeysMap;
			typedef std::map<NodeName, KeysMap>  NodesMap;

			NodesMap m_mpNodes;

		private:
			inline std::wstring Trim(std::wstring wsLine)
			{
				const wchar_t* filter_char = L" \r\n\t";
				wsLine.erase(0, wsLine.find_first_not_of(filter_char));
				wsLine.erase(wsLine.find_last_not_of(filter_char) + 1);
				return wsLine;
			}

			inline std::wstring GetNodeName(const std::wstring& wsNode)
			{
				size_t pos = wsNode.find_first_of(L']');
				if (pos == std::wstring::npos) { throw std::runtime_error("GetSectionName Error!"); }
				return wsNode.substr(1, pos - 1);
			}

			inline NodesMap::iterator At(const std::wstring& wsNode)
			{
				return m_mpNodes.find(wsNode);
			}

			inline NodesMap::iterator End()
			{
				return m_mpNodes.end();
			}

		public:
			INI_File() { }

			bool Parse(const std::wstring& wsFile)
			{
				std::wifstream wifs_ini(wsFile);
				wifs_ini.imbue(GetCVT_UTF8());
				if (wifs_ini.fail()) { throw std::runtime_error("Open File Error!"); }

				std::wstring node_name;
				for (std::wstring line; getline(wifs_ini, line);)
				{
					if (line.empty()) { continue; }

					switch (line[0])
					{
					case L'[': { node_name = Trim(GetNodeName(line)); } break;
					case L'#': break; // Comment Style 1
					case L';': break; // Comment Style 2
					case L'/': break; // Comment Style 3
					default:
					{
						size_t pos = line.find_first_of(L'=');
						if ((pos == std::wstring::npos) || (pos == 0)) { throw std::runtime_error("Node Init Error!"); }

						const std::wstring name = Trim(line.substr(0, pos));
						const std::wstring valu = Trim(line.substr(pos + 1));

						m_mpNodes[node_name][name] = valu;
					}
					break;
					}
				}

				return true;
			}

			std::wstring Dump()
			{
				std::wstringstream ss;

				for (auto& node : m_mpNodes)
				{
					const NodeName& node_name = node.first;
					const KeysMap& keys_map = node.second;
					ss << L"[" << node_name << L"]" << L'\n';

					for (auto& key : keys_map)
					{
						const Name& name = key.first;
						const Value& value = key.second;
						ss << name << L"=" << std::wstring(value) << L'\n';
					}

					ss << L'\n';
				}

				return ss.str();
			}

			friend std::wostream& operator << (std::wostream& woStream, INI_File& iniFile)
			{
				woStream << iniFile.Dump();
				return woStream;
			}

			void Save(const std::wstring& wsFile)
			{
				std::wofstream wofs_ini(wsFile);
				wofs_ini.imbue(GetCVT_UTF8());
				if (!wofs_ini) { throw std::runtime_error("INI_File::Save: Create File Error!"); }
				wofs_ini << Dump();
			}

			inline KeysMap& Get(const std::wstring& wsNode)
			{
				const auto& ite_node = At(wsNode);
				if (ite_node == End()) { throw std::runtime_error("No Find Node"); }
				return ite_node->second;
			}

			inline Value& Get(const std::wstring& wsNode, const std::wstring& wsName)
			{
				auto& keys = Get(wsNode);
				const auto& ite_keys = keys.find(wsName);
				if (ite_keys == keys.end()) { throw std::runtime_error("No Find Key"); }
				return ite_keys->second;
			}

			KeysMap& operator[] (const std::wstring& wsNode)
			{
				return Get(wsNode);
			}

			inline void Add(const std::wstring& wsNode, const std::wstring& wsName, const Value& vValue)
			{
				m_mpNodes[wsNode][wsName] = vValue;
			}

			inline bool Has(const std::wstring& wsNode)
			{
				return At(wsNode) != End() ? true : false;
			}

			inline bool Has(const std::wstring& wsNode, const std::wstring& wsName)
			{
				auto ite_node = At(wsNode);

				if (ite_node != End())
				{
					auto& keys = ite_node->second;
					auto ite_keys = keys.find(wsName);
					return ite_keys != keys.end() ? true : false;
				}

				return false;
			}

		};
	}
}