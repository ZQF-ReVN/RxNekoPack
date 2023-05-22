﻿#include "NEKOPACK_V2.h"


namespace NekoPackTools
{
	namespace Pack
	{
		static uint8_t sg_ucHashTable[256] =
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x38, 0x2F, 0x33, 0x3C, 0x40, 0x3B, 0x2A, 0x2E, 0x31, 0x30, 0x26, 0x44, 0x35, 0x28, 0x3E, 0x12,
			0x02, 0x22, 0x06, 0x20, 0x1A, 0x1C, 0x0F, 0x11, 0x18, 0x17, 0x42, 0x2B, 0x3A, 0x37, 0x34, 0x0C,
			0x41, 0x08, 0x1D, 0x07, 0x15, 0x21, 0x05, 0x1E, 0x0A, 0x14, 0x0E, 0x10, 0x09, 0x27, 0x1F, 0x0B,
			0x23, 0x16, 0x0D, 0x01, 0x25, 0x04, 0x1B, 0x03, 0x13, 0x24, 0x19, 0x2D, 0x12, 0x29, 0x32, 0x3F,
			0x3D, 0x08, 0x1D, 0x07, 0x15, 0x21, 0x05, 0x1E, 0x0A, 0x14, 0x0E, 0x10, 0x09, 0x27, 0x1F, 0x0B,
			0x23, 0x16, 0x0D, 0x01, 0x25, 0x04, 0x1B, 0x03, 0x13, 0x24, 0x19, 0x2C, 0x39, 0x43, 0x36, 0x00,
			0x4B, 0xA9, 0xA7, 0xAF, 0x50, 0x52, 0x91, 0x9F, 0x47, 0x6B, 0x96, 0xAB, 0x87, 0xB5, 0x9B, 0xBB,
			0x99, 0xA4, 0xBF, 0x5C, 0xC6, 0x9C, 0xC2, 0xC4, 0xB6, 0x4F, 0xB8, 0xC1, 0x85, 0xA8, 0x51, 0x7E,
			0x5F, 0x82, 0x73, 0xC7, 0x90, 0x4E, 0x45, 0xA5, 0x7A, 0x63, 0x70, 0xB3, 0x79, 0x83, 0x60, 0x55,
			0x5B, 0x5E, 0x68, 0xBA, 0x53, 0xA1, 0x67, 0x97, 0xAC, 0x71, 0x81, 0x59, 0x64, 0x7C, 0x9D, 0xBD,
			0x9D, 0xBD, 0x95, 0xA0, 0xB2, 0xC0, 0x6F, 0x6A, 0x54, 0xB9, 0x6D, 0x88, 0x77, 0x48, 0x5D, 0x72,
			0x49, 0x93, 0x57, 0x65, 0xBE, 0x4A, 0x80, 0xA2, 0x5A, 0x98, 0xA6, 0x62, 0x7F, 0x84, 0x75, 0xBC,
			0xAD, 0xB1, 0x6E, 0x76, 0x8B, 0x9E, 0x8C, 0x61, 0x69, 0x8D, 0xB4, 0x78, 0xAA, 0xAE, 0x8F, 0xC3,
			0x58, 0xC5, 0x74, 0xB7, 0x8E, 0x7D, 0x89, 0x8A, 0x56, 0x4D, 0x86, 0x94, 0x9A, 0x4C, 0x92, 0xB0
		};

		uint32_t GenHash(const uint32_t uiInit, const uint8_t* pStr)
		{
			uint32_t chax = 0, hash = uiInit;
			while ((chax = *pStr++)) { hash = 0x51 * (sg_ucHashTable[chax] ^ hash); }
			return hash;
		}

		void GenKeyTable(uint32_t* pKeyTable, uint32_t nKey)
		{
			uint32_t gen1 = nKey ^ (nKey + 0x5D588B65);
			uint32_t gen2 = gen1 ^ (nKey - 0x359D3E2A);
			uint32_t gen3 = gen2 ^ (gen1 - 0x70E44324);
			pKeyTable[0] = gen3;
			pKeyTable[1] = gen3 ^ (gen2 + 0x6C078965);
		}

		// block = 0x8 bytes
		void DecodeBlocks(uint32_t* pEnc, uint32_t* pKeyTable, size_t nBlock)
		{
			uint16_t* enc_buf = (uint16_t*)pEnc;
			uint16_t* key_buf = (uint16_t*)pKeyTable;

			uint16_t mm1_ll = key_buf[0];
			uint16_t mm1_lh = key_buf[1];
			uint16_t mm1_hl = key_buf[2];
			uint16_t mm1_hh = key_buf[3];

			while (nBlock--)
			{
				uint16_t& mm0_ll = enc_buf++[0];
				uint16_t& mm0_lh = enc_buf++[0];
				uint16_t& mm0_hl = enc_buf++[0];
				uint16_t& mm0_hh = enc_buf++[0];

				mm0_ll ^= mm1_ll;
				mm0_lh ^= mm1_lh;
				mm0_hl ^= mm1_hl;
				mm0_hh ^= mm1_hh;

				mm1_ll += mm0_ll;
				mm1_lh += mm0_lh;
				mm1_hl += mm0_hl;
				mm1_hh += mm0_hh;
			}
		}

		// buffer requires octet alignment
		void DecodeBuffer(uint32_t* pEnc, uint32_t nKey, size_t nSize)
		{
			size_t block_count = nSize / 8;
			uint32_t key_table[2] = { 0x0, 0x0 };
			if (nSize % 8) { block_count += 1; }
			GenKeyTable(key_table, nKey);
			DecodeBlocks(pEnc, key_table, block_count);
		}
	}
}