//=============================================================================
// ■ VMath.hpp
//-----------------------------------------------------------------------------
//   提供一些标准库里没有的数学函数。
//=============================================================================

namespace Math {
	//-------------------------------------------------------------------------
	// ● 常量
	//-------------------------------------------------------------------------
	// 先搞个模板变量
	// 还不能用acos函数，会炸……（因为数学函数都不是constexpr）
	// 这么长应该够了。但是不排除标准库里的PI是乱定义的，比如char PI = 3之类的。
	template <class T> constexpr T pi =
		(T) 3.141592653589793115997963468544185161590576171875;
	// 然后弄出一堆π
	const float PIf = pi<float>;
	const double PI = pi<double>;
	const double PId = PI;
	const long double PIl = pi<long double>;
	//-------------------------------------------------------------------------
	// ● 正弦值表系统™
	//   为了避免反复计算，将正弦值存储在这里。
	//   当VMATH_SINE_TABLE_SIZE为256时，其分布为
	//       [0] = sin 0
	//       [64] = sin ⅛π
	//       [128] = sin ¼π
	//       [192] = sin ⅜π
	//-------------------------------------------------------------------------
	// 先搞个模板类
	template <class T, size_t size> class SineTable {
	public:
		T value[size];
		SineTable() {
			T k = (T) .5 / (T) size * pi<T>;
			for (size_t i = 0; i < size; i++) value[i] = sin((T) i * k);
		}
		T operator[](size_t i) const {
			return value[i];
		}
	};
	// 然后就实例化一个实例出来
	#define VMATH_SINE_TABLE_SIZE ((size_t) 256)
	const SineTable<float, VMATH_SINE_TABLE_SIZE> sine_table;
	//-------------------------------------------------------------------------
	// ● clamp
	//-------------------------------------------------------------------------
	template <class T> T clamp(T x, T min, T max) {
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
}
