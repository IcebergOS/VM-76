//=============================================================================
// ■ ObjectCode.cpp
//-----------------------------------------------------------------------------
//   处理目标代码。这里的目标代码格式如下：
//     4字节的指令数 — 03 00 00 00                   — 3个指令
//     10字节/个指令 — 01 00 07 00 00 00 06 00 00 00 — #1 ADDL $7 $6
//                   — 00 00 00 00 00 00 00 00 00 00 — #2 NOOP
//                   — 28 00 00 00 00 00 00 00 00 00 — #3 HALT
//     任意尾部数据  — 01 23 45 67 89 AB CD EF
//=============================================================================

#include "ASM76.hpp"

namespace ASM76 { namespace ObjectCode {
	Program read_file(const char* filename) {
		Program r;
		VBinaryFileReader f(filename);
		r.size = f.read_u32();
		r.instruct = (Instruct*) malloc(sizeof(Instruct) * r.size);
		for (size_t i = 0; i < r.size; i++) {
			r.instruct[i].opcode = f.read_u16();
			r.instruct[i].a = f.read_u32();
			r.instruct[i].b = f.read_u32();
		}
		return r;
	}
	bool write_file(const char* filename, Program program) {
		VBinaryFileWriter f(filename);
		f.write_u32(program.size);
		for (size_t i = 0; i < program.size; i++) {
			f.write_u16(program.instruct[i].opcode);
			f.write_u32(program.instruct[i].a);
			f.write_u32(program.instruct[i].b);
		}
		return true;
	}
}}
