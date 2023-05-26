#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#include "NEKOPACK_V2_Struct.h"


namespace NekoPackTools
{
	namespace Pack
	{
		class NEKOPACK_V2
		{
		private:
			uint8_t* m_pDecTable;
			std::string m_wsPack;
			std::ifstream m_ifsPack;
			std::vector<NEKOPACK_V2_Entry> m_vcIndex;

		public:
			NEKOPACK_V2(const std::string& wsPack);
			~NEKOPACK_V2();

			void Extract();
			void PrintInfo(const NEKOPACK_V2_Entry& eEntry);

		private:
			void ReadTable();
			void ReadIndex();
			void GetKeyAndSize(uint32_t& nKey, uint32_t& nSize);
			static void BufferDecode(uint32_t nType, uint32_t* pEnc, uint8_t* pTable, uint32_t nKey, uint32_t nSize);
		};
	}
}