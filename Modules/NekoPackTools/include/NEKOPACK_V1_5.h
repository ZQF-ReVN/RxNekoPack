#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#include "NEKOPACK_V1_5_Struct.h"


namespace NekoPackTools
{
	namespace Pack
	{
		class NEKOPACK_V1_5
		{
		private:
			uint8_t* m_pDecTable;
			std::wstring m_wsPack;
			std::ifstream m_ifsPack;
			std::vector<NEKOPACK_V1_5_Entry> m_vcIndex;

		public:
			NEKOPACK_V1_5(const std::wstring& wsPack);
			~NEKOPACK_V1_5();

			void Extract();

		private:
			void ReadTable();
			void ReadIndex();
			void GetKeyAndSize(uint32_t& nKey, uint32_t& nSize);
			static void BufferDecode(uint32_t nType, uint32_t* pEnc, uint8_t* pTable, uint32_t nKey, uint32_t nSize);
		};
	}
}