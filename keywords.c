/* *INDENT-OFF* */
/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -C -c -p -t -T -g -j1 -o -H hash_keyword -N find_keyword keywords.gperf  */
/* Computed positions: -k'1,5,7' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "keywords.gperf"

struct KEYWORDS {
    const char *name;
    int value;
};

#define TOTAL_KEYWORDS 62
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 13
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 75
/* maximum key range = 72, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash_keyword (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 42,
      52, 34, 57, 76, 36, 76, 48, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76,  0, 76,  4, 76, 36,
      37, 11, 19, 15, 76,  1, 76, 13,  7,  2,
       3, 25, 10, 76, 10, 16,  0,  9, 33, 13,
      12, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76, 76, 76, 76, 76,
      76, 76, 76, 76, 76, 76
    };
  register unsigned int hval = (unsigned)len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

static const struct KEYWORDS *
find_keyword (register const char *str, register size_t len)
{
  static const struct KEYWORDS wordlist[] =
    {
      {"", 0}, {"", 0}, {"", 0}, {"", 0},
#line 38 "keywords.gperf"
      {"int",		T_INT,},
      {"", 0},
#line 54 "keywords.gperf"
      {"time_t",		STD_TYPENAME,	/* c99 */},
#line 42 "keywords.gperf"
      {"int8_t",		STD_TYPENAME,	/* c99 */},
#line 27 "keywords.gperf"
      {"auto",		T_AUTO,},
#line 44 "keywords.gperf"
      {"intptr_t",	STD_TYPENAME,	/* c99 */},
#line 37 "keywords.gperf"
      {"inline", 	T_INLINE,},
#line 45 "keywords.gperf"
      {"long",		T_LONG,},
#line 9 "keywords.gperf"
      {"_Bool",		T_Bool,},
#line 43 "keywords.gperf"
      {"intmax_t",	STD_TYPENAME,	/* c99 */},
#line 18 "keywords.gperf"
      {"__attribute__", 	T_ATTRIBUTE,},
#line 32 "keywords.gperf"
      {"enum",		T_ENUM,},
#line 19 "keywords.gperf"
      {"__extension__", 	T_EXTENSION,},
#line 62 "keywords.gperf"
      {"union",		T_UNION,},
#line 22 "keywords.gperf"
      {"__inline", 	T_INLINE,},
#line 70 "keywords.gperf"
      {"wint_t",		STD_TYPENAME,	/* c99 */},
#line 23 "keywords.gperf"
      {"__inline__", 	T_INLINE,},
#line 48 "keywords.gperf"
      {"short",		T_SHORT,},
#line 51 "keywords.gperf"
      {"size_t",		T_LONG,},
#line 52 "keywords.gperf"
      {"static", 	T_STATIC,},
#line 34 "keywords.gperf"
      {"float",		T_FLOAT,},
#line 36 "keywords.gperf"
      {"fpos_t",		STD_TYPENAME,	/* c99 */},
#line 35 "keywords.gperf"
      {"float_t",	STD_TYPENAME,	/* c99 */},
#line 33 "keywords.gperf"
      {"extern", 	T_EXTERN,},
#line 17 "keywords.gperf"
      {"_Imaginary",	T_Imaginary,},
#line 10 "keywords.gperf"
      {"_Complex",	T_Complex,},
#line 67 "keywords.gperf"
      {"wchar_t",	STD_TYPENAME,	/* c99 */},
#line 69 "keywords.gperf"
      {"wctype_t",	STD_TYPENAME,	/* c99 */},
#line 60 "keywords.gperf"
      {"uintmax_t",	STD_TYPENAME,	/* c99 */},
#line 50 "keywords.gperf"
      {"signed", 	T_SIGNED,},
#line 25 "keywords.gperf"
      {"__signed", 	T_SIGNED,},
#line 20 "keywords.gperf"
      {"__float128", 	STD_TYPENAME,},
#line 26 "keywords.gperf"
      {"__signed__", 	T_SIGNED,},
#line 66 "keywords.gperf"
      {"void",		T_VOID,},
#line 61 "keywords.gperf"
      {"uintptr_t",	STD_TYPENAME,	/* c99 */},
#line 46 "keywords.gperf"
      {"ptrdiff_t",	STD_TYPENAME,	/* c99 */},
#line 28 "keywords.gperf"
      {"char",		T_CHAR,},
#line 65 "keywords.gperf"
      {"va_list",	T_VA_LIST,},
#line 68 "keywords.gperf"
      {"wctrans_t",	STD_TYPENAME,	/* c99 */},
#line 63 "keywords.gperf"
      {"unsigned",	T_UNSIGNED,},
#line 39 "keywords.gperf"
      {"int16_t",	STD_TYPENAME,	/* c99 */},
#line 47 "keywords.gperf"
      {"register",	T_REGISTER,},
#line 13 "keywords.gperf"
      {"_Float32",	STD_TYPENAME,	/* c99 */},
#line 14 "keywords.gperf"
      {"_Float32x",	STD_TYPENAME,	/* c99 */},
#line 15 "keywords.gperf"
      {"_Float64",	STD_TYPENAME,	/* c99 */},
#line 16 "keywords.gperf"
      {"_Float64x",	STD_TYPENAME,	/* c99 */},
#line 30 "keywords.gperf"
      {"double", 	T_DOUBLE,},
#line 57 "keywords.gperf"
      {"uint32_t",	STD_TYPENAME,	/* c99 */},
#line 31 "keywords.gperf"
      {"double_t",	STD_TYPENAME,	/* c99 */},
#line 58 "keywords.gperf"
      {"uint64_t",	STD_TYPENAME,	/* c99 */},
#line 12 "keywords.gperf"
      {"_Float16",	STD_TYPENAME,	/* c99 */},
#line 11 "keywords.gperf"
      {"_Float128",	STD_TYPENAME,	/* c99 */},
#line 29 "keywords.gperf"
      {"clock_t",	STD_TYPENAME,	/* c99 */},
#line 49 "keywords.gperf"
      {"sig_atomic_t",	STD_TYPENAME,	/* c99 */},
#line 53 "keywords.gperf"
      {"struct", 	T_STRUCT,},
#line 56 "keywords.gperf"
      {"uint16_t",	STD_TYPENAME,	/* c99 */},
#line 40 "keywords.gperf"
      {"int32_t",	STD_TYPENAME,	/* c99 */},
#line 24 "keywords.gperf"
      {"__int32_t",	STD_TYPENAME,	/* gcc */},
#line 21 "keywords.gperf"
      {"__ibm128", 	STD_TYPENAME,},
#line 55 "keywords.gperf"
      {"typedef",	T_TYPEDEF,},
#line 59 "keywords.gperf"
      {"uint8_t",	STD_TYPENAME,	/* c99 */},
#line 41 "keywords.gperf"
      {"int64_t",	STD_TYPENAME,	/* c99 */},
      {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0}, {"", 0},
#line 64 "keywords.gperf"
      {"va_dcl",		T_VA_DCL,}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash_keyword (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strncmp (str + 1, s + 1, len - 1) && s[len] == '\0')
            return &wordlist[key];
        }
    }
  return 0;
}
/* *INDENT-ON* */
