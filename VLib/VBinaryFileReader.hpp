//=============================================================================
// ■ VBinaryFileReader.hpp
//-----------------------------------------------------------------------------
//   在各计算机之间兼容的二进制文件读取类。（Little-endian）
//=============================================================================

#define ERROR_MESSAGE "error: cannot find beer"

class VBinaryFileReader {
public:
	//-------------------------------------------------------------------------
	// ● 成员
	//-------------------------------------------------------------------------
	FILE* f;
	//-------------------------------------------------------------------------
	// ● 构造
	//-------------------------------------------------------------------------
	VBinaryFileReader(const char* filename) {
		f = fopen(filename, "rb");
		if (!f) error(ERROR_MESSAGE);
	}
	//-------------------------------------------------------------------------
	// ● 析构
	//-------------------------------------------------------------------------
	~VBinaryFileReader() {
		fclose(f);
	}
	//-------------------------------------------------------------------------
	// ● 直接读取
	//-------------------------------------------------------------------------
	template <class T> T read_directly() {
		T r;
		if (fread(&r, sizeof(T), 1, f) < 1) error("read_directly");
		return r;
	}
	//-------------------------------------------------------------------------
	// ● 读取1字节int
	//-------------------------------------------------------------------------
	int8_t read_i8() {
		return (int8_t) read_u8();
	}
	//-------------------------------------------------------------------------
	// ● 读取2字节int
	//-------------------------------------------------------------------------
	#define READ_CHAR(i) if ((c[i] = fgetc(f)) < 0) error(ERROR_MESSAGE);
	int16_t read_i16() {
		if (is_little_endian()) {
			return read_directly<int16_t>();
		} else {
			long c[2];
			READ_CHAR(0);
			READ_CHAR(1);
			return (int32_t) (c[0] | c[1] << 8);
		}
	}
	//-------------------------------------------------------------------------
	// ● 读取4字节int
	//-------------------------------------------------------------------------
	int32_t read_i32() {
		if (is_little_endian()) {
			return read_directly<int32_t>();
		} else {
			long c[4];
			READ_CHAR(0);
			READ_CHAR(1);
			READ_CHAR(2);
			READ_CHAR(3);
			return (int32_t) (c[0] | c[1] << 8 | c[2] << 16 | c[3] << 24);
		}
	}
	//-------------------------------------------------------------------------
	// ● 读取8字节int
	//-------------------------------------------------------------------------
	int64_t read_i64() {
		if (is_little_endian()) {
			return read_directly<int64_t>();
		} else {
			long long c[8];
			READ_CHAR(0);
			READ_CHAR(1);
			READ_CHAR(2);
			READ_CHAR(3);
			READ_CHAR(4);
			READ_CHAR(5);
			READ_CHAR(6);
			READ_CHAR(7);
			return (int64_t) (
				c[0] | c[1] << 8 | c[2] << 16 | c[3] << 24
					| c[4] << 32 | c[5] << 40 | c[6] << 48 | c[7] << 56
			);
		}
	}
	#undef READ_CHAR
	//-------------------------------------------------------------------------
	// ● 读取1字节unsigned
	//-------------------------------------------------------------------------
	uint8_t read_u8() {
		int c = fgetc(f);
		if (c < 0) error(ERROR_MESSAGE);
		return (uint8_t) c;
	}
	//-------------------------------------------------------------------------
	// ● 读取2字节unsigned
	//-------------------------------------------------------------------------
	uint16_t read_u16() {
		return (uint16_t) read_i16();
	}
	//-------------------------------------------------------------------------
	// ● 读取4字节unsigned
	//-------------------------------------------------------------------------
	uint32_t read_u32() {
		return (uint32_t) read_i32();
	}
	//-------------------------------------------------------------------------
	// ● 读取8字节unsigned
	//-------------------------------------------------------------------------
	uint64_t read_u64() {
		return (uint64_t) read_i64();
	}
	//-------------------------------------------------------------------------
	// ● 读取4字节float
	//-------------------------------------------------------------------------
	float read_float() {
		return read_directly<float>();
	}
	//-------------------------------------------------------------------------
	// ● 读取8字节double
	//-------------------------------------------------------------------------
	double read_double() {
		return read_directly<double>();
	}
};

#undef ERROR_MESSAGE
