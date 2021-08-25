#pragma once

#include "../Drawing/Color.hh"
#include <iostream>
#include <cstdint>
#include <cinttypes>
#undef NDEBUG
#include <assert.h>
#include <cmath>
#include <corecrt.h>
#include <corecrt_math_defines.h>
#include <array>
#include <algorithm>
#include <compare>

namespace Helveta
{
	namespace detail
	{
		template <typename CharT = char>
		constexpr size_t strlen(const CharT *szStr)
		{
			return *szStr ? 1u + strlen(szStr + 1) : 0u;
		}

		template <typename CharT = char>
		constexpr size_t GetLastCharacterNotNull(const CharT *szStr, size_t nLen)
		{
			return szStr[nLen] ? nLen : GetLastCharacterNotNull(szStr, nLen - 1);
		}

		template <typename CharT = char>
		constexpr size_t GetLastCharacterNotNull(const CharT *szStr)
		{
			return GetLastCharacterNotNull(szStr, strlen(szStr));
		}

		template <typename CharT = char>
		constexpr size_t FindFirstOfStart(size_t iStart, CharT ch, const CharT *szStr, size_t nLen)
		{
			size_t iIdx = iStart;
			while (szStr[iIdx] != ch && iIdx < nLen)
				++iIdx;

			return iIdx;
		}

		template <typename CharT = char>
		constexpr size_t FindFirstOf(CharT ch, const CharT *szStr, size_t nLen)
		{
			return FindFirstOfStart(0u, ch, szStr, nLen);
		}

		template <typename CharT = char>
		constexpr size_t FindFirstOfStart(size_t iStart, CharT ch, const CharT *szStr)
		{
			return FindFirstOfStart(0u, ch, szStr, strlen(szStr));
		}

		template <typename CharT = char>
		constexpr size_t FindFirstOf(CharT ch, const CharT *szStr)
		{
			return FindFirstOf(ch, szStr, strlen(szStr));
		}

		template <typename CharT = char>
		constexpr size_t FindLastOf(CharT ch, const CharT *szStr, size_t nLen)
		{
			size_t iIdx = GetLastCharacterNotNull(szStr, nLen);
			while (szStr[iIdx] != ch && iIdx >= 0)
				--iIdx;

			return iIdx;
		}

		template <typename CharT = char>
		constexpr size_t FindLastOf(CharT ch, const CharT *szStr)
		{
			return FindLastOf(ch, szStr, strlen(szStr));
		}

		template <typename CharT = char>
		constexpr size_t FindFirstNotOfStart(size_t iStart, CharT ch, const CharT *szStr, size_t nLen)
		{
			if (iStart < nLen && szStr[iStart] != ch)
				return iStart;

			size_t iIdx = iStart;

			while (szStr[iIdx] == ch && iIdx < nLen)
				++iIdx;

			return iIdx;
		}

		template <typename CharT = char>
		constexpr size_t FindFirstNotOf(CharT ch, const CharT *szStr, size_t nLen)
		{
			return FindFirstNotOfStart(0u, ch, szStr, nLen);
		}

		template <typename CharT = char>
		constexpr size_t FindFirstNotOfStart(CharT ch, const CharT *szStr)
		{
			return FindFirstNotOfStart(ch, szStr, strlen(szStr));
		}

		template <typename CharT = char>
		constexpr size_t FindLastNotOf(CharT ch, const CharT *szStr, size_t nLen)
		{
			size_t iIdx = GetLastCharacterNotNull(szStr, nLen);
			while (szStr[iIdx] == ch && iIdx >= 0)
				--iIdx;

			return iIdx;
		}

		template <typename CharT = char>
		constexpr size_t FindLastNotOf(CharT ch, const CharT *szStr)
		{
			return FindLastNotOf(ch, szStr, strlen(szStr));
		}

		template <typename CharT = char>
		constexpr int StrChToHex(CharT ch)
		{
			if (ch >= '0' && ch <= '9')
				return ch - '0';

			if (ch >= 'A' && ch <= 'F')
				return ch - 'A' + 10;

			return ch - 'a' + 10;
		}

		template <typename T, T F = 16>
		constexpr T CombineHex(const T &a, const T &b)
		{
			return T(F) * a + b;
		}

		constexpr uint8_t ByteSwap8bit(uint8_t u8Byte)
		{
			return (uint8_t)(u8Byte << 4) | (uint8_t)(u8Byte >> 4);
		}

		constexpr uint16_t ByteSwap16bit(uint16_t u16Bytes)
		{
			return (uint16_t)(u16Bytes << 8) | (uint16_t)(u16Bytes >> 8);
		}

		constexpr uint32_t ByteSwap32bit(uint32_t u32Bytes)
		{
			return (uint32_t)(u32Bytes << 16) | (uint32_t)(u32Bytes >> 16);
		}

		constexpr auto ToArray16bit(uint16_t u16Bytes)
		{
			return std::array<uint8_t, 2>{(uint8_t)((uint16_t)(u16Bytes & 0xff00) >> 8),
										  (uint8_t)((uint16_t)(u16Bytes & 0x00ff))};
		}

		constexpr auto ToArray32bit(uint32_t u32Bytes)
		{
			return std::array<uint8_t, 4>{(uint8_t)((uint32_t)(u32Bytes & 0xff000000) >> 24),
										  (uint8_t)((uint32_t)(u32Bytes & 0x00ff0000) >> 16),
										  (uint8_t)((uint32_t)(u32Bytes & 0x0000ff00) >> 8),
										  (uint8_t)((uint32_t)(u32Bytes & 0x000000ff))};
		}

		constexpr auto EndiannessSwap32bit(uint32_t u32Bytes)
		{
			return 0x10000 * ((uint32_t)(ByteSwap16bit((uint16_t)((u32Bytes & 0x0000ffff))))) + (uint32_t)(ByteSwap16bit((uint16_t)((uint32_t)((u32Bytes & 0xffff0000)) >> 16)));
		}

		template <typename Y, typename X, size_t N, template <typename, size_t> typename A, size_t... Is>
		constexpr A<Y, N> CastArrayImpl(const A<X, N> &a, std::index_sequence<Is...>)
		{
			return {std::get<Is>(a)...};
		}

		template <typename Y, typename X, size_t N, template <typename, size_t> typename A, typename Indices = std::make_index_sequence<N>>
		constexpr A<Y, N> CastArray(const A<X, N> &a)
		{
			return CastArrayImpl<Y>(a, Indices{});
		}

		template <typename T, size_t N>
		constexpr static auto FoldLiteralsToArray(const T (&rgElements)[N])
		{
			std::array<T, N> rgResult = {};
			for (auto i = 0u; i < N; ++i)
				rgResult[i] = rgElements[i];

			return rgResult;
		}

		struct Fail_t
		{
			constexpr Fail_t()
			{
				static_assert("YOU EPIC FAILED!");
			}
		};
	}

	/**
	 * @brief Compile-time data holder
	 * 
	 */
	template <auto Held, typename T = decltype(Held)>
	struct DataHolder_t
	{
		constexpr static T value = Held;
	};

	/**
	 * @brief Compile-time string holder
	 * 
	 */
	template <size_t N>
	struct CompileTimeString_t
	{
		//	Compile-time static linkage string complaint constructor
		constexpr CompileTimeString_t(const char *szStr)
		{
			for (size_t i = 0; i != N; ++i)
				this->m_szData[i] = szStr[i];
		}

		char m_szData[N + 1] = {};
		constexpr static size_t m_nDataLen = N;
	};

	//	Constructor hack to be able to construct in-place without the direct need
	//	of creating a statically linked holder
	template <size_t N>
	CompileTimeString_t(const char (&)[N]) -> CompileTimeString_t<N - 1>;

	/**
	 * @brief String class to compile compile time strings
	 * 
	 */
	struct CompileTimeStringCompiler_t
	{
		constexpr CompileTimeStringCompiler_t(const char *szData, size_t nLen)
		{
			this->m_szData = szData;
			this->m_nDataLen = nLen;
		}

		constexpr const char *Data() const
		{
			return this->m_szData;
		}

		constexpr size_t Size() const
		{
			return this->m_nDataLen;
		}

		constexpr bool Empty() const
		{
			return this->m_nDataLen == 0u;
		}

		constexpr size_t Find(char ch, size_t iStart = 0) const
		{
			return detail::FindFirstOfStart(iStart, ch, this->m_szData, this->m_nDataLen);
		}

		constexpr size_t RFind(char ch) const
		{
			return detail::FindLastOf(ch, this->m_szData, this->m_nDataLen);
		}

		constexpr size_t FindFirstOf(char ch, size_t iStart = 0) const
		{
			return Find(ch, iStart);
		}

		constexpr size_t FindLastOf(char ch) const
		{
			return RFind(ch);
		}

		constexpr size_t FindFirstNotOf(char ch, size_t iStart = 0) const
		{
			return detail::FindFirstNotOfStart(iStart, ch, this->m_szData, this->m_nDataLen);
		}

		constexpr size_t FindLastNotOf(char ch) const
		{
			return detail::FindLastNotOf(ch, this->m_szData, this->m_nDataLen);
		}

		constexpr const char &operator[](size_t iIdx) const
		{
			return this->m_szData[iIdx];
		}

	private:
		const char *m_szData = nullptr;
		size_t m_nDataLen = 0;
	};

	/**
	 * @brief String to Byte Array converter
	 * 
	 */
	template <CompileTimeString_t Compile>
	struct CompileTimeStringToByteArray_t
	{
		constexpr static auto Compiled = CompileTimeStringCompiler_t(Compile.m_szData, Compile.m_nDataLen);
		constexpr static char Delimiter = ' ';
		constexpr static char Mask = '?';
		constexpr static int Masked = -1;

		struct Data
		{
			struct Result
			{
				size_t m_iCount;
				size_t m_iStart;
				size_t m_iNext;
				size_t m_iEnd;
			};

			constexpr static Result Make()
			{
				size_t iCount = 1;

				constexpr size_t iStart = Compiled.FindFirstNotOf(Delimiter);
				constexpr size_t iNext = Compiled.FindFirstOf(Delimiter, iStart);
				constexpr size_t iEnd = Compiled.FindLastNotOf(Delimiter);

				bool bPreviousIsDelimiter = false;
				for (auto i = iNext; i < iEnd; ++i)
				{
					if (Compiled[i] == Delimiter)
					{
						if (!bPreviousIsDelimiter)
							++iCount;

						bPreviousIsDelimiter = true;
					}
					else
						bPreviousIsDelimiter = false;
				}

				return Result{iCount, iStart, iNext, iEnd};
			}
		};

		constexpr static auto Get()
		{
			constexpr auto data = Data::Make();
			constexpr auto iCount = data.m_iCount;
			constexpr auto iStart = data.m_iStart;
			constexpr auto iNext = data.m_iNext;
			constexpr auto iEnd = data.m_iEnd;

			std::array<int, iCount> rgResult = {};
			std::array<size_t, iCount> rgSkips = {};

			size_t nSkips = 0u;
			size_t nTraversed = iStart;
			bool bPreviousIsSkip = false;
			for (size_t i = iStart; i < iEnd; ++i)
			{
				if (Compiled[i] == Delimiter)
				{
					if (!bPreviousIsSkip)
						rgSkips[nSkips++] = nTraversed;

					bPreviousIsSkip = true;
				}
				else
					bPreviousIsSkip = false;

				++nTraversed;
			}

			rgResult[0] = Compiled[iStart] == Mask ? Masked : detail::CombineHex<int>(detail::StrChToHex(Compiled[iStart]), detail::StrChToHex(Compiled[iStart + 1]));

			size_t nConversions = 1u;
			for (auto i = iNext; i < iEnd; ++i)
			{
				for (const auto &entry : rgSkips)
				{
					if (entry == i && entry < iEnd)
					{
						size_t iCharacterIdx = Compiled.FindFirstNotOf(Delimiter, i + 1);
						bool bIsOneChar = Compiled[iCharacterIdx + 1] == Delimiter;
						rgResult[nConversions++] = Compiled[iCharacterIdx] == Mask ? Masked : (bIsOneChar ? detail::StrChToHex(Compiled[iCharacterIdx]) : detail::CombineHex<int>(detail::StrChToHex(Compiled[iCharacterIdx]), detail::StrChToHex(Compiled[iCharacterIdx + 1])));
					}
				}
			}

			return rgResult;
		}
	};

	/**
	 * @brief Run-time/Compile-time FNV1A String Hasher
	 * 
	 */
	using Hash_t = unsigned long long;
	template <typename Hash = Hash_t, Hash Seed = 0x543C730D, Hash Prime = 0x1000931>
	struct StringHasher_t
	{
		constexpr static Hash Get(const char *szKey, size_t nLen)
		{
			Hash hash = Seed;

			for (auto i = 0u; i < nLen; ++i)
			{
				const uint8_t u8Val = szKey[i];
				hash ^= u8Val;
				hash *= Prime;
			}

			return hash;
		}
	};

	/**
	 * @brief Variadic CSGO compliant Mathematical Vector 
	 * 
	 */
	enum
	{
		PITCH,
		YAW,
		ROLL
	};
	template <typename T>
	struct Vector_t
	{
		template <typename... Args>
		struct Make
		{
			constexpr static size_t m_nPackLen = sizeof...(Args);
			using Pack = std::array<T, m_nPackLen>;

			explicit Make() = default;

			constexpr auto operator<=>(const Make &) const = default;

			constexpr void Initialize(const Args &...args)
			{
				this->m_rgContents = Pack{{args...}};
			}

			explicit constexpr Make(const Args &...args)
			{
				Initialize(args...);
			}

			constexpr size_t Size() const
			{
				return m_nPackLen;
			}

			constexpr Make<Args...> operator+(const Make &arg) const
			{
				Make vecResult{};
				for (size_t i = 0; i < m_nPackLen; ++i)
					vecResult[i] = this->m_rgContents[i] + arg[i];

				return vecResult;
			}

			constexpr Make<Args...> operator-(const Make &arg) const
			{
				Make vecResult{};
				for (size_t i = 0; i < m_nPackLen; ++i)
					vecResult[i] = this->m_rgContents[i] - arg[i];

				return vecResult;
			}

			constexpr Make<Args...> operator*(const Make &arg) const
			{
				Make vecResult{};
				for (size_t i = 0; i < m_nPackLen; ++i)
					vecResult[i] = this->m_rgContents[i] * arg[i];

				return vecResult;
			}

			constexpr Make<Args...> operator/(const Make &arg) const
			{
				Make vecResult{};
				for (size_t i = 0; i < m_nPackLen; ++i)
					vecResult[i] = this->m_rgContents[i] / arg[i];

				return vecResult;
			}

			inline void operator+=(const Make &arg)
			{
				for (size_t i = 0; i < m_nPackLen; ++i)
					this->m_rgContents[i] += arg[i];
			}

			inline void operator-=(const Make &arg)
			{
				for (size_t i = 0; i < m_nPackLen; ++i)
					this->m_rgContents[i] -= arg[i];
			}

			inline void operator*=(const Make &arg)
			{
				for (size_t i = 0; i < m_nPackLen; ++i)
					this->m_rgContents[i] *= arg[i];
			}

			inline void operator/=(const Make &arg)
			{
				for (size_t i = 0; i < m_nPackLen; ++i)
					this->m_rgContents[i] /= arg[i];
			}

			constexpr const T &operator[](size_t i) const
			{
				return this->m_rgContents[i];
			}

			constexpr T &operator[](size_t i)
			{
				return this->m_rgContents[i];
			}

			const T &At(const size_t i) const
			{
				assert(i < m_nPackLen);
				return operator[](i);
			}

			T &At(const size_t i)
			{
				assert(i < m_nPackLen);
				return operator[](i);
			}

			constexpr Pack &Raw() const
			{
				return this->m_rgContents;
			}

			const Make<Args...> Copy() const
			{
				auto _this = *this;
				return _this;
			}

			template <size_t N = m_nPackLen>
			const T GetDot() const
			{
				static_assert(N <= m_nPackLen);

				T result = {};

				for (size_t i = 0; i < N; ++i)
					result += this->m_rgContents[i] * this->m_rgContents[i];

				return result;
			}

			template <size_t N = m_nPackLen>
			const T GetLength() const
			{
				return sqrt(GetDot<N>());
			}

			template <size_t N = m_nPackLen>
			const T Dot(T arg) const
			{
				static_assert(N <= m_nPackLen);

				T result = {};

				for (size_t i = 0; i < N; ++i)
					result += this->m_rgContents[i] * arg;

				return result;
			}

			template <size_t N = m_nPackLen>
			const T Dot(const Make &arg) const
			{
				static_assert(N <= m_nPackLen);
				assert(arg.Size() <= N);

				const Make &argContents = arg.Copy();
				const Make &contents = Copy();
				T result = {};

				for (size_t i = 0; i < N; ++i)
					result += contents[i] * argContents[i];

				return result;
			}

			template <size_t N = m_nPackLen>
			const T Length(T arg) const
			{
				return sqrt(Dot<N>(arg));
			}

			template <size_t N = m_nPackLen>
			const T Length(const Make &arg) const
			{
				return sqrt(Dot<N>(arg));
			}

			void NormalizeAngle()
			{
				static_assert(std::is_same_v<T, float>);
				static_assert(m_nPackLen == 3U);

				operator[](PITCH) = std::isfinite(operator[](PITCH)) ? std::remainderf(operator[](PITCH), 360.F) : 0.F;
				operator[](YAW) = std::isfinite(operator[](YAW)) ? std::remainderf(operator[](YAW), 360.F) : 0.F;
				operator[](ROLL) = 0.F;
			}

			const Make<Args...> NormalizedAngle() const
			{
				static_assert(std::is_same_v<T, float>);
				static_assert(m_nPackLen == 3U);

				auto copy = Copy();
				copy.NormalizeAngle();
				return copy;
			}

			auto NormalizeLength() -> void
			{
				static_assert(std::is_same_v<T, float>);
				static_assert(m_nPackLen == 3U);

				const T &length = GetLength();

				if (length != 0.F)
				{
					operator[](PITCH) /= length;
					operator[](YAW) /= length;
					operator[](ROLL) /= length;
				}
				else
				{
					operator[](PITCH) = operator[](YAW) = 0.F;
					operator[](ROLL) = 1.F;
				}
			}

			const Make<Args...> NormalizedLength() const
			{
				static_assert(std::is_same_v<T, float>);
				static_assert(m_nPackLen == 3U);

				auto copy = copy();
				copy.NormalizeLength();
				return copy;
			}

			const Make<Args...> GetAngle() const
			{
				auto forward = *this;
				Make angles;

				if (forward[PITCH] == 0.F && forward[YAW] == 0.F)
				{
					angles[PITCH] = angles[ROLL] > 0.F ? -90.F : 90.F;
					angles[YAW] = 0.F;
				}
				else
				{
					angles[PITCH] = atan2(-forward[ROLL], forward.GetLength<2>()) * (180.F / M_PI);
					angles[YAW] = atan2(forward[YAW], forward[PITCH] * (180.F / M_PI));
				}

				angles[ROLL] = 0.F;

				return angles;
			}

			void ClampAngle()
			{
				static_assert(std::is_same_v<T, float>);
				static_assert(m_nPackLen == 3U);

				//	Enforce non-const operator[] to be called
				operator[](PITCH) = std::clamp<float>(operator[](PITCH), -89.F, 89.F);
				operator[](YAW) = std::clamp<float>(operator[](YAW), -180.F, 180.F);
				operator[](ROLL) = 0.F;
			}

			bool IsValid() const
			{
				static_assert(std::is_same_v<T, int> || std::is_same_v<T, float>);

				for (const auto &n : this->m_rgContents)
				{
					if (n == T{0})
						return false;
				}

				return true;
			}

		private:
			Pack m_rgContents = Pack{};
		};

		using V2 = Make<T, T>;
		using V3 = Make<T, T, T>;
		using V4 = Make<T, T, T, T>;
	};

	template <CompileTimeString_t Compile, typename Int = int, bool Nibble = false>
	struct CompileTimeHungarianNotationTypeParser_t
	{
		constexpr static auto Compiled = CompileTimeStringCompiler_t(Compile.m_szData, Compile.m_nDataLen);

		struct Data
		{
			struct Result
			{
				size_t m_iFirstUppercaseCharacter;
			};

			constexpr static Result Make()
			{
				size_t iFirstUppercaseCharacter = 2;
				while (Compiled[iFirstUppercaseCharacter] < 'A' || Compiled[iFirstUppercaseCharacter] > 'Z')
					++iFirstUppercaseCharacter;

				return Result{iFirstUppercaseCharacter};
			}
		};

		enum Types_t
		{
			NONE,
			BOOL,
			INT,
			UINT8,
			UINTPTR,
			FLOAT,
			CONSTCHAR,
			VEC,
			//	Note: modify if you have differing structures for Angles and Vectors
			ANG = VEC,
			CLR
		};

		constexpr static Types_t Get()
		{
			//	Not Hungarian notation for class member
			if (Compiled[0] != 'm', Compiled[1] != '_')
				return Types_t::NONE;

			constexpr auto data = Data::Make();
			constexpr auto iFirstUppercaseCharacter = data.m_iFirstUppercaseCharacter;

			std::array<char, iFirstUppercaseCharacter - 2> rgCharacters;
			for (size_t i = 2; i < iFirstUppercaseCharacter; ++i)
				rgCharacters[i - 2] = Compiled[i];

			if constexpr (rgCharacters.size() == 1)
			{
				if (rgCharacters[0] == 'b')
					return Types_t::BOOL;

				else if (rgCharacters[0] == 'i')
					return Types_t::INT;

				//	This may either be an integer in the, say, case of 'm_fFlags', or,
				//	shorthand for 'fl', examples of that being CBeam->m_fWidth, CBeam->m_fEndWidth, etc...
				else if (rgCharacters[0] == 'f')
					return Types_t::INT;

				//	NOTE: m_n is used interchangibly for nibble and count
				else if (rgCharacters[0] == 'n')
					if constexpr (Nibble)
						return Types_t::UINT8;
					else
						return Types_t::INT;

				else if (rgCharacters[0] == 'u')
					return Types_t::UINT8;

				//	Should essentially just work if you just want the indice
				//	to then pass it to EntList or do some storing/restoring
				else if (rgCharacters[0] == 'h')
					return Types_t::UINTPTR;
			}
			//	I'm not going to fold the literals into an array from now onwards as it's not really that needed.
			else if constexpr (rgCharacters.size() == 2)
			{
				if (rgCharacters[0] == 'f' && rgCharacters[1] == 'l')
					return Types_t::FLOAT;

				else if (rgCharacters[0] == 's' && rgCharacters[1] == 'z')
					return Types_t::CONSTCHAR;
			}
			else if constexpr (rgCharacters.size() == 3)
			{
				if (rgCharacters[0] == 'v' && rgCharacters[1] == 'e' && rgCharacters[2] == 'c')
					return Types_t::VEC;

				else if (rgCharacters[0] == 'a' && rgCharacters[1] == 'n' && rgCharacters[2] == 'g')
					return Types_t::ANG;

				else if (rgCharacters[0] == 'c' && rgCharacters[1] == 'l' && rgCharacters[2] == 'r')
					return Types_t::CLR;

				else if (rgCharacters[0] == 'p' && rgCharacters[1] == 's' && rgCharacters[2] == 'z')
					return Types_t::CONSTCHAR;
			}
			else
				static_assert("Cannot deduce type of such length.");

			return Types_t::NONE;
		}

		template <int N, typename... Ts>
		struct TupleType_t;

		template <int N, typename T, typename... Ts>
		struct TupleType_t<N, std::tuple<T, Ts...>>
		{
			using Result_t = typename TupleType_t<N - 1, std::tuple<Ts...>>::Result_t;
		};

		template <typename T, typename... Ts>
		struct TupleType_t<0, std::tuple<T, Ts...>>
		{
			using Result_t = T;
		};

		using TypesTuple_t = std::tuple<detail::Fail_t, bool, Int, uint8_t, uintptr_t, float, const char *, Vector_t<float>::V3, Color_t>;
		using Parse_t = TupleType_t<Helveta::DataHolder_t<Get()>::value, TypesTuple_t>::Result_t;
		static_assert(!std::is_same_v<Parse_t, detail::Fail_t>, "Type parser cannot deduce type");
	};

	template <CompileTimeString_t Compile>
	using CompileTimeHungarianNotationTypeParser_ShortInt_t = CompileTimeHungarianNotationTypeParser_t<Compile, short>;

	template <CompileTimeString_t Compile>
	using CompileTimeHungarianNotationTypeParser_Nibble_t = CompileTimeHungarianNotationTypeParser_t<Compile, int, true>;
}

/**
 * @brief We want to avoid strings being pushed at run-time, or any run-time generation
 * Thus, we will wrap our method into a compile-time holder.
 * 
 */
#define STB(x) Helveta::DataHolder_t<Helveta::CompileTimeStringToByteArray_t<x>::Get()>::value

/**
 * @brief Ditto.
 * 
 */
#define HASH(x) Helveta::DataHolder_t<Helveta::StringHasher_t<>::Get(x, Helveta::detail::strlen(x))>::value

/**
 * @brief We don't want to specify strlen every time.
 * 
 */
#define RT_HASH(x) Helveta::StringHasher_t<>::Get(x, strlen(x))

/**
 * @brief Cast arrays, respectively at run-time and at compile-time
 * 
 */
#define CAST_ARRAY(t, x) Helveta::DataHolder_t<Helveta::detail::CastArray<t>(x)>::value

#define RT_CAST_ARRAY(t, x) Helveta::detail::CastArray<t>(x)

/**
 * @brief Fold literals into C++ array
 * 
 */
#define FOLD_LITERALS_INTO_ARRAY(x) Helveta::DataHolder_t<Helveta::detail::FoldLiteralsToArray(x)>::value

#define RT_FOLD_LITERALS_INTO_ARRAY(x) Helveta::detail::FoldLiteralsToArray(x)

/**
 * @brief Get Hungarian Notation Type
 * 
 */
#define GET_TYPE(x) Helveta::CompileTimeHungarianNotationTypeParser_t<x>::Parse_t
#define GET_TYPE_NN(x) Helveta::CompileTimeHungarianNotationTypeParser_Nibble_t<x>::Parse_t
#define GET_TYPE_SI(x) Helveta::CompileTimeHungarianNotationTypeParser_ShortInt_t<x>::Parse_t

//	Shorthand
using Helveta::Hash_t;
using Helveta::Vector_t;

/**
 * @brief Debug Logging
 * 
 */
#define LOG(...) std::cout << __VA_ARGS__ << '\n'
