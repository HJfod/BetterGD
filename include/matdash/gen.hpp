
template <typename R, typename ...Args>
struct optfastcall<R(Args...)> {
	using F = R(Args...);
	template <F func>
	static auto __fastcall wrap(Args... args) {
		return func(args...);
	}
};


template <typename R, typename A, typename B, typename C, typename ...Args>
struct optfastcall<R(A, B, C, float, Args...)> {
	using F = R(A, B, C, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, B a1, C a2, Args... args) {
		float a3;
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename A, typename B, typename ...Args>
struct optfastcall<R(A, B, float, Args...)> {
	using F = R(A, B, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, B a1, Args... args) {
		float a2;
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
};


template <typename R, typename A, typename B, typename ...Args>
struct optfastcall<R(A, B, float, float, Args...)> {
	using F = R(A, B, float, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, B a1, Args... args) {
		float a2, a3;
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct optfastcall<R(A, float, Args...)> {
	using F = R(A, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, Args... args) {
		float a1;
		__asm movss a1, xmm1
		return func(a0, a1, args...);
	}
};


template <typename R, typename A, typename C, typename ...Args>
struct optfastcall<R(A, float, C, float, Args...)> {
	using F = R(A, float, C, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, C a2, Args... args) {
		float a1, a3;
		__asm movss a1, xmm1
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct optfastcall<R(A, float, float, Args...)> {
	using F = R(A, float, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, Args... args) {
		float a1, a2;
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct optfastcall<R(A, float, float, float, Args...)> {
	using F = R(A, float, float, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, Args... args) {
		float a1, a2, a3;
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename ...Args>
struct optfastcall<R(float, Args...)> {
	using F = R(float, Args...);
	template <F func>
	static auto __fastcall wrap(Args... args) {
		float a0;
		__asm movss a0, xmm0
		return func(a0, args...);
	}
};


template <typename R, typename B, typename C, typename ...Args>
struct optfastcall<R(float, B, C, float, Args...)> {
	using F = R(float, B, C, float, Args...);
	template <F func>
	static auto __fastcall wrap(B a1, C a2, Args... args) {
		float a0, a3;
		__asm movss a0, xmm0
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename B, typename ...Args>
struct optfastcall<R(float, B, float, Args...)> {
	using F = R(float, B, float, Args...);
	template <F func>
	static auto __fastcall wrap(B a1, Args... args) {
		float a0, a2;
		__asm movss a0, xmm0
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
};


template <typename R, typename B, typename ...Args>
struct optfastcall<R(float, B, float, float, Args...)> {
	using F = R(float, B, float, float, Args...);
	template <F func>
	static auto __fastcall wrap(B a1, Args... args) {
		float a0, a2, a3;
		__asm movss a0, xmm0
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename ...Args>
struct optfastcall<R(float, float, Args...)> {
	using F = R(float, float, Args...);
	template <F func>
	static auto __fastcall wrap(Args... args) {
		float a0, a1;
		__asm movss a0, xmm0
		__asm movss a1, xmm1
		return func(a0, a1, args...);
	}
};


template <typename R, typename C, typename ...Args>
struct optfastcall<R(float, float, C, float, Args...)> {
	using F = R(float, float, C, float, Args...);
	template <F func>
	static auto __fastcall wrap(C a2, Args... args) {
		float a0, a1, a3;
		__asm movss a0, xmm0
		__asm movss a1, xmm1
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename ...Args>
struct optfastcall<R(float, float, float, Args...)> {
	using F = R(float, float, float, Args...);
	template <F func>
	static auto __fastcall wrap(Args... args) {
		float a0, a1, a2;
		__asm movss a0, xmm0
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
};


template <typename R, typename ...Args>
struct optfastcall<R(float, float, float, float, Args...)> {
	using F = R(float, float, float, float, Args...);
	template <F func>
	static auto __fastcall wrap(Args... args) {
		float a0, a1, a2, a3;
		__asm movss a0, xmm0
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct optcall<R(A, Args...)> {
	using F = R(A, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, void*, Args... args) {
		return func(a0, args...);
	}
};


template <typename R, typename A, typename B, typename C, typename ...Args>
struct optcall<R(A, B, C, float, Args...)> {
	using F = R(A, B, C, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, void*, B a1, C a2, Args... args) {
		float a3;
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename A, typename B, typename ...Args>
struct optcall<R(A, B, float, Args...)> {
	using F = R(A, B, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, void*, B a1, Args... args) {
		float a2;
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
};


template <typename R, typename A, typename B, typename ...Args>
struct optcall<R(A, B, float, float, Args...)> {
	using F = R(A, B, float, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, void*, B a1, Args... args) {
		float a2, a3;
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct optcall<R(A, float, Args...)> {
	using F = R(A, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, void*, Args... args) {
		float a1;
		__asm movss a1, xmm1
		return func(a0, a1, args...);
	}
};


template <typename R, typename A, typename C, typename ...Args>
struct optcall<R(A, float, C, float, Args...)> {
	using F = R(A, float, C, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, void*, C a2, Args... args) {
		float a1, a3;
		__asm movss a1, xmm1
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct optcall<R(A, float, float, Args...)> {
	using F = R(A, float, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, void*, Args... args) {
		float a1, a2;
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		return func(a0, a1, a2, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct optcall<R(A, float, float, float, Args...)> {
	using F = R(A, float, float, float, Args...);
	template <F func>
	static auto __fastcall wrap(A a0, void*, Args... args) {
		float a1, a2, a3;
		__asm movss a1, xmm1
		__asm movss a2, xmm2
		__asm movss a3, xmm3
		return func(a0, a1, a2, a3, args...);
	}
};


template <typename R, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(Args...)> {
	R(__fastcall* addr)(Args...);
	auto operator()(Args... args) {
		return addr(args...);
	}
};


template <typename R, typename A, typename B, typename C, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(A, B, C, float, Args...)> {
	R(__fastcall* addr)(A, B, C, Args...);
	auto operator()(A a0, B a1, C a2, float a3, Args... args) {
		__asm movss xmm3, a3
		return addr(a0, a1, a2, args...);
	}
};


template <typename R, typename A, typename B, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(A, B, float, Args...)> {
	R(__fastcall* addr)(A, B, Args...);
	auto operator()(A a0, B a1, float a2, Args... args) {
		__asm movss xmm2, a2
		return addr(a0, a1, args...);
	}
};


template <typename R, typename A, typename B, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(A, B, float, float, Args...)> {
	R(__fastcall* addr)(A, B, Args...);
	auto operator()(A a0, B a1, float a2, float a3, Args... args) {
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return addr(a0, a1, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(A, float, Args...)> {
	R(__fastcall* addr)(A, Args...);
	auto operator()(A a0, float a1, Args... args) {
		__asm movss xmm1, a1
		return addr(a0, args...);
	}
};


template <typename R, typename A, typename C, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(A, float, C, float, Args...)> {
	R(__fastcall* addr)(A, C, Args...);
	auto operator()(A a0, float a1, C a2, float a3, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm3, a3
		return addr(a0, a2, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(A, float, float, Args...)> {
	R(__fastcall* addr)(A, Args...);
	auto operator()(A a0, float a1, float a2, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		return addr(a0, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(A, float, float, float, Args...)> {
	R(__fastcall* addr)(A, Args...);
	auto operator()(A a0, float a1, float a2, float a3, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return addr(a0, args...);
	}
};


template <typename R, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(float, Args...)> {
	R(__fastcall* addr)(Args...);
	auto operator()(float a0, Args... args) {
		__asm movss xmm0, a0
		return addr(args...);
	}
};


template <typename R, typename B, typename C, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(float, B, C, float, Args...)> {
	R(__fastcall* addr)(B, C, Args...);
	auto operator()(float a0, B a1, C a2, float a3, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm3, a3
		return addr(a1, a2, args...);
	}
};


template <typename R, typename B, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(float, B, float, Args...)> {
	R(__fastcall* addr)(B, Args...);
	auto operator()(float a0, B a1, float a2, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm2, a2
		return addr(a1, args...);
	}
};


template <typename R, typename B, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(float, B, float, float, Args...)> {
	R(__fastcall* addr)(B, Args...);
	auto operator()(float a0, B a1, float a2, float a3, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return addr(a1, args...);
	}
};


template <typename R, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(float, float, Args...)> {
	R(__fastcall* addr)(Args...);
	auto operator()(float a0, float a1, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm1, a1
		return addr(args...);
	}
};


template <typename R, typename C, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(float, float, C, float, Args...)> {
	R(__fastcall* addr)(C, Args...);
	auto operator()(float a0, float a1, C a2, float a3, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm1, a1
		__asm movss xmm3, a3
		return addr(a2, args...);
	}
};


template <typename R, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(float, float, float, Args...)> {
	R(__fastcall* addr)(Args...);
	auto operator()(float a0, float a1, float a2, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		return addr(args...);
	}
};


template <typename R, typename ...Args>
struct WrapperOptfastcall<R(__fastcall*)(float, float, float, float, Args...)> {
	R(__fastcall* addr)(Args...);
	auto operator()(float a0, float a1, float a2, float a3, Args... args) {
		__asm movss xmm0, a0
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return addr(args...);
	}
};


template <typename R, typename ...Args>
struct WrapperOptcall<R(__thiscall*)(Args...)> {
	R(__thiscall* addr)(Args...);
	auto operator()(Args... args) {
		return addr(args...);
	}
};


template <typename R, typename A, typename B, typename C, typename ...Args>
struct WrapperOptcall<R(__thiscall*)(A, B, C, float, Args...)> {
	R(__thiscall* addr)(A, B, C, Args...);
	auto operator()(A a0, B a1, C a2, float a3, Args... args) {
		__asm movss xmm3, a3
		return addr(a0, a1, a2, args...);
	}
};


template <typename R, typename A, typename B, typename ...Args>
struct WrapperOptcall<R(__thiscall*)(A, B, float, Args...)> {
	R(__thiscall* addr)(A, B, Args...);
	auto operator()(A a0, B a1, float a2, Args... args) {
		__asm movss xmm2, a2
		return addr(a0, a1, args...);
	}
};


template <typename R, typename A, typename B, typename ...Args>
struct WrapperOptcall<R(__thiscall*)(A, B, float, float, Args...)> {
	R(__thiscall* addr)(A, B, Args...);
	auto operator()(A a0, B a1, float a2, float a3, Args... args) {
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return addr(a0, a1, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct WrapperOptcall<R(__thiscall*)(A, float, Args...)> {
	R(__thiscall* addr)(A, Args...);
	auto operator()(A a0, float a1, Args... args) {
		__asm movss xmm1, a1
		return addr(a0, args...);
	}
};


template <typename R, typename A, typename C, typename ...Args>
struct WrapperOptcall<R(__thiscall*)(A, float, C, float, Args...)> {
	R(__thiscall* addr)(A, C, Args...);
	auto operator()(A a0, float a1, C a2, float a3, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm3, a3
		return addr(a0, a2, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct WrapperOptcall<R(__thiscall*)(A, float, float, Args...)> {
	R(__thiscall* addr)(A, Args...);
	auto operator()(A a0, float a1, float a2, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		return addr(a0, args...);
	}
};


template <typename R, typename A, typename ...Args>
struct WrapperOptcall<R(__thiscall*)(A, float, float, float, Args...)> {
	R(__thiscall* addr)(A, Args...);
	auto operator()(A a0, float a1, float a2, float a3, Args... args) {
		__asm movss xmm1, a1
		__asm movss xmm2, a2
		__asm movss xmm3, a3
		return addr(a0, args...);
	}
};

