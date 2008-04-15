/* A Bison parser, made from yacc.yy, by GNU bison 1.75.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON	1

/* Pure parsers.  */
#define YYPURE	0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_AMPERSAND = 258,
     T_ANY = 259,
     T_ASTERISK = 260,
     T_ATTRIBUTE = 261,
     T_BOOLEAN = 262,
     T_CASE = 263,
     T_CHAR = 264,
     T_CHARACTER_LITERAL = 265,
     T_WCHARACTER_LITERAL = 266,
     T_CIRCUMFLEX = 267,
     T_COLON = 268,
     T_COMMA = 269,
     T_CONST = 270,
     T_CONTEXT = 271,
     T_DEFAULT = 272,
     T_DOUBLE = 273,
     T_ENUM = 274,
     T_EQUAL = 275,
     T_EXCEPTION = 276,
     T_FALSE = 277,
     T_FIXED = 278,
     T_FIXED_PT_LITERAL = 279,
     T_FLOAT = 280,
     T_FLOATING_PT_LITERAL = 281,
     T_GREATER_THAN_SIGN = 282,
     T_IDENTIFIER = 283,
     T_IN = 284,
     T_INOUT = 285,
     T_INTEGER_LITERAL = 286,
     T_INTERFACE = 287,
     T_LEFT_CURLY_BRACKET = 288,
     T_LEFT_PARANTHESIS = 289,
     T_LEFT_SQUARE_BRACKET = 290,
     T_LESS_THAN_SIGN = 291,
     T_LONG = 292,
     T_MINUS_SIGN = 293,
     T_MODULE = 294,
     T_OCTET = 295,
     T_ONEWAY = 296,
     T_OUT = 297,
     T_PERCENT_SIGN = 298,
     T_PLUS_SIGN = 299,
     T_RAISES = 300,
     T_READONLY = 301,
     T_RIGHT_CURLY_BRACKET = 302,
     T_RIGHT_PARANTHESIS = 303,
     T_RIGHT_SQUARE_BRACKET = 304,
     T_SCOPE = 305,
     T_SEMICOLON = 306,
     T_SEQUENCE = 307,
     T_SHIFTLEFT = 308,
     T_SHIFTRIGHT = 309,
     T_SHORT = 310,
     T_SOLIDUS = 311,
     T_STRING = 312,
     T_STRING_LITERAL = 313,
     T_WSTRING_LITERAL = 314,
     T_PRAGMA = 315,
     T_INCLUDE = 316,
     T_STRUCT = 317,
     T_SWITCH = 318,
     T_TILDE = 319,
     T_TRUE = 320,
     T_OBJECT = 321,
     T_TYPEDEF = 322,
     T_UNION = 323,
     T_UNSIGNED = 324,
     T_VERTICAL_LINE = 325,
     T_VOID = 326,
     T_WCHAR = 327,
     T_WSTRING = 328,
     T_UNKNOWN = 329,
     T_ABSTRACT = 330,
     T_VALUETYPE = 331,
     T_TRUNCATABLE = 332,
     T_SUPPORTS = 333,
     T_CUSTOM = 334,
     T_PUBLIC = 335,
     T_PRIVATE = 336,
     T_FACTORY = 337,
     T_NATIVE = 338,
     T_VALUEBASE = 339,
     T_IMPORT = 340,
     T_TYPEID = 341,
     T_TYPEPREFIX = 342,
     T_GETRAISES = 343,
     T_SETRAISES = 344,
     T_LOCAL = 345,
     T_COMPONENT = 346,
     T_PROVIDES = 347,
     T_USES = 348,
     T_MULTIPLE = 349,
     T_EMITS = 350,
     T_PUBLISHES = 351,
     T_CONSUMES = 352,
     T_HOME = 353,
     T_MANAGES = 354,
     T_PRIMARYKEY = 355,
     T_FINDER = 356,
     T_EVENTTYPE = 357
   };
#endif
#define T_AMPERSAND 258
#define T_ANY 259
#define T_ASTERISK 260
#define T_ATTRIBUTE 261
#define T_BOOLEAN 262
#define T_CASE 263
#define T_CHAR 264
#define T_CHARACTER_LITERAL 265
#define T_WCHARACTER_LITERAL 266
#define T_CIRCUMFLEX 267
#define T_COLON 268
#define T_COMMA 269
#define T_CONST 270
#define T_CONTEXT 271
#define T_DEFAULT 272
#define T_DOUBLE 273
#define T_ENUM 274
#define T_EQUAL 275
#define T_EXCEPTION 276
#define T_FALSE 277
#define T_FIXED 278
#define T_FIXED_PT_LITERAL 279
#define T_FLOAT 280
#define T_FLOATING_PT_LITERAL 281
#define T_GREATER_THAN_SIGN 282
#define T_IDENTIFIER 283
#define T_IN 284
#define T_INOUT 285
#define T_INTEGER_LITERAL 286
#define T_INTERFACE 287
#define T_LEFT_CURLY_BRACKET 288
#define T_LEFT_PARANTHESIS 289
#define T_LEFT_SQUARE_BRACKET 290
#define T_LESS_THAN_SIGN 291
#define T_LONG 292
#define T_MINUS_SIGN 293
#define T_MODULE 294
#define T_OCTET 295
#define T_ONEWAY 296
#define T_OUT 297
#define T_PERCENT_SIGN 298
#define T_PLUS_SIGN 299
#define T_RAISES 300
#define T_READONLY 301
#define T_RIGHT_CURLY_BRACKET 302
#define T_RIGHT_PARANTHESIS 303
#define T_RIGHT_SQUARE_BRACKET 304
#define T_SCOPE 305
#define T_SEMICOLON 306
#define T_SEQUENCE 307
#define T_SHIFTLEFT 308
#define T_SHIFTRIGHT 309
#define T_SHORT 310
#define T_SOLIDUS 311
#define T_STRING 312
#define T_STRING_LITERAL 313
#define T_WSTRING_LITERAL 314
#define T_PRAGMA 315
#define T_INCLUDE 316
#define T_STRUCT 317
#define T_SWITCH 318
#define T_TILDE 319
#define T_TRUE 320
#define T_OBJECT 321
#define T_TYPEDEF 322
#define T_UNION 323
#define T_UNSIGNED 324
#define T_VERTICAL_LINE 325
#define T_VOID 326
#define T_WCHAR 327
#define T_WSTRING 328
#define T_UNKNOWN 329
#define T_ABSTRACT 330
#define T_VALUETYPE 331
#define T_TRUNCATABLE 332
#define T_SUPPORTS 333
#define T_CUSTOM 334
#define T_PUBLIC 335
#define T_PRIVATE 336
#define T_FACTORY 337
#define T_NATIVE 338
#define T_VALUEBASE 339
#define T_IMPORT 340
#define T_TYPEID 341
#define T_TYPEPREFIX 342
#define T_GETRAISES 343
#define T_SETRAISES 344
#define T_LOCAL 345
#define T_COMPONENT 346
#define T_PROVIDES 347
#define T_USES 348
#define T_MULTIPLE 349
#define T_EMITS 350
#define T_PUBLISHES 351
#define T_CONSUMES 352
#define T_HOME 353
#define T_MANAGES 354
#define T_PRIMARYKEY 355
#define T_FINDER 356
#define T_EVENTTYPE 357




/* Copy the first part of user declarations.  */
#line 23 "yacc.yy"


#include <CORBA.h>

#ifdef HAVE_ANSI_CPLUSPLUS_HEADERS
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef _WIN32
#include <malloc.h>
#endif

#include "parser.h"
#include "parsenode.h"
#include "prepro.h"
#include <mico/template_impl.h>


using namespace std;

extern int yylex();

void yyerror( char *s )
{
  theParser->parse_error( idl_lexFile, s, idl_line_no );
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#ifndef YYSTYPE
#line 56 "yacc.yy"
typedef union {
  char              *ident;
  ParseNode         *node;
  CORBA::ULongLong  _int;
  char             *_str;
  CORBA::WChar     *_wstr;
  CORBA::Char       _char;
  CORBA::WChar      _wchar;
  CORBA::LongDouble _float;
  CORBA::LongDouble _fixed;
} yystype;
/* Line 193 of /home/karel/usr/local/bison-1.75/share/bison/yacc.c.  */
#line 320 "yacc.c"
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif

#ifndef YYLTYPE
typedef struct yyltype
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} yyltype;
# define YYLTYPE yyltype
# define YYLTYPE_IS_TRIVIAL 1
#endif

/* Copy the second part of user declarations.  */


/* Line 213 of /home/karel/usr/local/bison-1.75/share/bison/yacc.c.  */
#line 341 "yacc.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYLTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAX (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAX)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];	\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAX;	\
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  143
#define YYLAST   1152

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  103
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  152
/* YYNRULES -- Number of rules. */
#define YYNRULES  350
/* YYNRULES -- Number of states. */
#define YYNSTATES  577

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   357

#define YYTRANSLATE(X) \
  ((unsigned)(X) <= YYMAXUTOK ? yytranslate[X] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    14,    17,    20,
      23,    26,    29,    32,    35,    38,    41,    44,    47,    49,
      51,    57,    59,    61,    66,    69,    73,    77,    80,    84,
      88,    93,    97,   102,   103,   105,   107,   110,   113,   116,
     119,   122,   125,   128,   131,   133,   135,   138,   140,   142,
     146,   148,   151,   155,   157,   159,   161,   163,   166,   170,
     174,   181,   189,   190,   192,   197,   201,   203,   206,   210,
     215,   218,   222,   225,   230,   233,   235,   239,   242,   247,
     249,   251,   253,   255,   257,   262,   267,   274,   280,   282,
     286,   290,   292,   298,   300,   302,   304,   306,   308,   310,
     312,   314,   316,   318,   320,   322,   326,   328,   332,   334,
     338,   340,   344,   348,   350,   354,   358,   360,   364,   368,
     372,   375,   378,   381,   383,   385,   387,   391,   393,   395,
     397,   399,   401,   403,   405,   407,   409,   411,   415,   417,
     419,   421,   424,   426,   428,   430,   432,   434,   436,   438,
     440,   442,   444,   446,   448,   450,   452,   454,   456,   458,
     460,   462,   464,   466,   468,   470,   474,   476,   478,   480,
     482,   484,   486,   489,   491,   493,   495,   497,   499,   501,
     503,   506,   508,   510,   512,   515,   518,   522,   524,   526,
     528,   530,   532,   534,   540,   542,   545,   549,   551,   553,
     563,   565,   567,   569,   571,   573,   575,   578,   581,   585,
     590,   594,   597,   600,   606,   608,   612,   615,   617,   624,
     629,   634,   636,   641,   643,   646,   648,   651,   655,   657,
     659,   665,   666,   669,   676,   682,   688,   693,   694,   696,
     698,   700,   704,   707,   709,   713,   717,   719,   721,   723,
     728,   733,   735,   739,   741,   744,   746,   749,   751,   753,
     755,   762,   764,   766,   769,   772,   775,   777,   779,   783,
     787,   792,   795,   797,   799,   803,   807,   810,   812,   814,
     816,   819,   822,   825,   828,   832,   834,   836,   839,   844,
     848,   851,   855,   859,   864,   867,   870,   872,   875,   878,
     881,   884,   887,   890,   893,   897,   899,   901,   905,   910,
     914,   918,   922,   925,   930,   936,   942,   949,   955,   962,
     969,   977,   980,   983,   987,   990,   992,   995,   997,  1000,
    1003,  1010,  1016,  1022,  1027,  1034,  1040,  1046,  1051,  1053,
    1055,  1057,  1060,  1064,  1071,  1079,  1084,  1088,  1092,  1097,
    1100
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     104,     0,    -1,    -1,   105,    -1,   106,    -1,   106,   105,
      -1,   150,    51,    -1,   137,    51,    -1,   195,    51,    -1,
     108,    51,    -1,   107,    51,    -1,   119,    51,    -1,   216,
      51,    -1,   217,    51,    -1,   250,    51,    -1,   227,    51,
      -1,   241,    51,    -1,   214,    51,    -1,    60,    -1,    61,
      -1,    39,    28,    33,   105,    47,    -1,   109,    -1,   110,
      -1,   111,    33,   112,    47,    -1,    32,    28,    -1,    75,
      32,    28,    -1,    90,    32,    28,    -1,    32,    28,    -1,
      32,    28,   115,    -1,    75,    32,    28,    -1,    75,    32,
      28,   115,    -1,    90,    32,    28,    -1,    90,    32,    28,
     115,    -1,    -1,   113,    -1,   114,    -1,   114,   113,    -1,
     150,    51,    -1,   137,    51,    -1,   195,    51,    -1,   194,
      51,    -1,   197,    51,    -1,   216,    51,    -1,   217,    51,
      -1,    60,    -1,    61,    -1,    13,   116,    -1,   117,    -1,
     118,    -1,   118,    14,   117,    -1,    28,    -1,    50,    28,
      -1,   118,    50,    28,    -1,   124,    -1,   122,    -1,   121,
      -1,   120,    -1,    76,    28,    -1,    75,    76,    28,    -1,
      76,    28,   151,    -1,    75,    76,    28,    33,   123,    47,
      -1,    75,    76,    28,   127,    33,   123,    47,    -1,    -1,
     113,    -1,   126,    33,   125,    47,    -1,   126,    33,    47,
      -1,   131,    -1,   131,   125,    -1,    76,    28,   127,    -1,
      79,    76,    28,   127,    -1,    76,    28,    -1,    79,    76,
      28,    -1,    13,   128,    -1,    13,   128,    78,   116,    -1,
      78,   116,    -1,   130,    -1,   130,    14,   129,    -1,    77,
     130,    -1,    77,   130,    14,   129,    -1,   117,    -1,   118,
      -1,   114,    -1,   132,    -1,   133,    -1,    80,   151,   156,
      51,    -1,    81,   151,   156,    51,    -1,    82,    28,    34,
     134,    48,    51,    -1,    82,    28,    34,    48,    51,    -1,
     135,    -1,   135,    14,   134,    -1,   136,   209,   158,    -1,
      29,    -1,    15,   138,    28,    20,   139,    -1,   161,    -1,
     170,    -1,   171,    -1,   172,    -1,   160,    -1,   189,    -1,
     190,    -1,   211,    -1,   118,    -1,   173,    -1,   140,    -1,
     141,    -1,   140,    70,   141,    -1,   142,    -1,   141,    12,
     142,    -1,   143,    -1,   142,     3,   143,    -1,   144,    -1,
     143,    54,   144,    -1,   143,    53,   144,    -1,   145,    -1,
     144,    44,   145,    -1,   144,    38,   145,    -1,   146,    -1,
     145,     5,   146,    -1,   145,    56,   146,    -1,   145,    43,
     146,    -1,    38,   147,    -1,    44,   147,    -1,    64,   147,
      -1,   147,    -1,   118,    -1,   148,    -1,    34,   139,    48,
      -1,    31,    -1,   207,    -1,   208,    -1,    10,    -1,    11,
      -1,    24,    -1,    26,    -1,    65,    -1,    22,    -1,   139,
      -1,    67,   151,   156,    -1,   176,    -1,   179,    -1,   185,
      -1,    83,   158,    -1,   213,    -1,   152,    -1,   155,    -1,
     153,    -1,   154,    -1,   118,    -1,   160,    -1,   161,    -1,
     170,    -1,   171,    -1,   172,    -1,   173,    -1,   174,    -1,
     175,    -1,   212,    -1,   188,    -1,   189,    -1,   190,    -1,
     210,    -1,   176,    -1,   179,    -1,   185,    -1,   157,    -1,
     157,    14,   156,    -1,   158,    -1,   159,    -1,    28,    -1,
     191,    -1,    25,    -1,    18,    -1,    37,    18,    -1,   162,
      -1,   166,    -1,   164,    -1,   163,    -1,   165,    -1,    55,
      -1,    37,    -1,    37,    37,    -1,   168,    -1,   167,    -1,
     169,    -1,    69,    55,    -1,    69,    37,    -1,    69,    37,
      37,    -1,     9,    -1,    72,    -1,     7,    -1,    40,    -1,
       4,    -1,    66,    -1,    62,    28,    33,   177,    47,    -1,
     178,    -1,   178,   177,    -1,   151,   156,    51,    -1,    60,
      -1,    61,    -1,    68,    28,    63,    34,   180,    48,    33,
     181,    47,    -1,   161,    -1,   170,    -1,   172,    -1,   185,
      -1,   118,    -1,   182,    -1,   182,   181,    -1,   183,   182,
      -1,   183,   184,    51,    -1,   183,    60,   184,    51,    -1,
       8,   139,    13,    -1,    17,    13,    -1,   151,   157,    -1,
      19,    28,    33,   186,    47,    -1,   187,    -1,   187,    14,
     186,    -1,    60,   186,    -1,    28,    -1,    52,    36,   152,
      14,   149,    27,    -1,    52,    36,   152,    27,    -1,    57,
      36,   149,    27,    -1,    57,    -1,    73,    36,   149,    27,
      -1,    73,    -1,    28,   192,    -1,   193,    -1,   193,   192,
      -1,    35,   149,    49,    -1,   218,    -1,   221,    -1,    21,
      28,    33,   196,    47,    -1,    -1,   178,   196,    -1,   198,
     199,    28,   200,   204,   205,    -1,   198,   199,    28,   200,
     205,    -1,   198,   199,    28,   200,   204,    -1,   198,   199,
      28,   200,    -1,    -1,    41,    -1,   209,    -1,    71,    -1,
      34,   201,    48,    -1,    34,    48,    -1,   202,    -1,   202,
      14,   201,    -1,   203,   209,   158,    -1,    29,    -1,    42,
      -1,    30,    -1,    45,    34,   117,    48,    -1,    16,    34,
     206,    48,    -1,   207,    -1,   207,    14,   206,    -1,    58,
      -1,    58,   207,    -1,    59,    -1,    59,   208,    -1,   153,
      -1,   154,    -1,   118,    -1,    23,    36,   149,    14,    31,
      27,    -1,    23,    -1,    84,    -1,    62,    28,    -1,    68,
      28,    -1,    85,   215,    -1,   118,    -1,   207,    -1,    86,
     118,   207,    -1,    87,   118,   207,    -1,    46,     6,   209,
     219,    -1,   158,   204,    -1,   220,    -1,   158,    -1,   158,
      14,   220,    -1,     6,   209,   222,    -1,   158,   223,    -1,
     220,    -1,   224,    -1,   225,    -1,   224,   225,    -1,   225,
     224,    -1,    88,   226,    -1,    89,   226,    -1,    34,   117,
      48,    -1,   229,    -1,   228,    -1,    91,    28,    -1,   230,
      33,   233,    47,    -1,   230,    33,    47,    -1,    91,    28,
      -1,    91,    28,   232,    -1,    91,    28,   231,    -1,    91,
      28,   232,   231,    -1,    78,   116,    -1,    13,   118,    -1,
     234,    -1,   234,   233,    -1,   235,    51,    -1,   237,    51,
      -1,   238,    51,    -1,   239,    51,    -1,   240,    51,    -1,
     194,    51,    -1,    92,   236,    28,    -1,   118,    -1,    66,
      -1,    93,   236,    28,    -1,    93,    94,   236,    28,    -1,
      95,   118,    28,    -1,    96,   118,    28,    -1,    97,   118,
      28,    -1,   242,   245,    -1,    98,    28,    99,   118,    -1,
      98,    28,   243,    99,   118,    -1,    98,    28,    99,   118,
     244,    -1,    98,    28,   243,    99,   118,   244,    -1,    98,
      28,   231,    99,   118,    -1,    98,    28,   243,   231,    99,
     118,    -1,    98,    28,   231,    99,   118,   244,    -1,    98,
      28,   243,   231,    99,   118,   244,    -1,    13,   118,    -1,
     100,   118,    -1,    33,   246,    47,    -1,    33,    47,    -1,
     247,    -1,   247,   246,    -1,   114,    -1,   248,    51,    -1,
     249,    51,    -1,    82,    28,    34,   134,    48,   204,    -1,
      82,    28,    34,    48,   204,    -1,    82,    28,    34,   134,
      48,    -1,    82,    28,    34,    48,    -1,   101,    28,    34,
     134,    48,   204,    -1,   101,    28,    34,    48,   204,    -1,
     101,    28,    34,   134,    48,    -1,   101,    28,    34,    48,
      -1,   253,    -1,   252,    -1,   251,    -1,   102,    28,    -1,
      75,   102,    28,    -1,    75,   102,    28,    33,   123,    47,
      -1,    75,   102,    28,   127,    33,   123,    47,    -1,   254,
      33,   125,    47,    -1,   254,    33,    47,    -1,   102,    28,
     127,    -1,    79,   102,    28,   127,    -1,   102,    28,    -1,
      79,   102,    28,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   326,   326,   330,   338,   342,   351,   355,   359,   363,
     367,   371,   375,   379,   383,   387,   391,   395,   399,   404,
     413,   423,   427,   435,   444,   449,   454,   463,   468,   474,
     479,   485,   490,   500,   504,   511,   515,   524,   528,   532,
     536,   540,   544,   548,   552,   557,   566,   573,   577,   581,
     590,   595,   603,   616,   617,   618,   619,   624,   629,   638,
     647,   655,   667,   671,   679,   684,   691,   695,   704,   709,
     714,   719,   728,   732,   737,   745,   750,   756,   761,   770,
     775,   780,   781,   782,   787,   791,   799,   806,   817,   821,
     830,   838,   846,   855,   859,   863,   867,   871,   875,   879,
     883,   887,   891,   899,   907,   911,   919,   923,   931,   935,
     943,   947,   951,   959,   963,   967,   975,   979,   983,   987,
     996,  1000,  1004,  1008,  1016,  1020,  1024,  1033,  1038,  1043,
    1048,  1053,  1058,  1063,  1068,  1073,  1082,  1091,  1095,  1099,
    1103,  1107,  1111,  1116,  1120,  1128,  1132,  1136,  1144,  1148,
    1152,  1156,  1160,  1164,  1168,  1172,  1176,  1184,  1188,  1192,
    1196,  1204,  1208,  1212,  1220,  1224,  1233,  1237,  1245,  1254,
    1262,  1266,  1270,  1278,  1282,  1290,  1294,  1298,  1306,  1314,
    1322,  1330,  1334,  1338,  1346,  1354,  1362,  1370,  1378,  1386,
    1394,  1402,  1410,  1418,  1428,  1432,  1441,  1445,  1450,  1459,
    1470,  1474,  1478,  1482,  1486,  1494,  1498,  1506,  1510,  1515,
    1527,  1531,  1539,  1547,  1556,  1560,  1564,  1574,  1583,  1588,
    1596,  1600,  1608,  1612,  1620,  1628,  1632,  1641,  1649,  1650,
    1655,  1664,  1668,  1677,  1683,  1689,  1695,  1704,  1708,  1716,
    1720,  1728,  1732,  1739,  1743,  1752,  1760,  1764,  1768,  1776,
    1784,  1791,  1796,  1804,  1808,  1820,  1824,  1852,  1856,  1860,
    1868,  1878,  1886,  1894,  1899,  1909,  1917,  1918,  1927,  1936,
    1945,  1953,  1958,  1962,  1966,  1975,  1983,  1988,  1993,  1997,
    2001,  2005,  2013,  2021,  2029,  2037,  2038,  2043,  2052,  2057,
    2065,  2070,  2075,  2081,  2091,  2099,  2107,  2111,  2119,  2123,
    2127,  2131,  2135,  2139,  2147,  2156,  2157,  2165,  2170,  2179,
    2188,  2197,  2206,  2214,  2220,  2225,  2232,  2238,  2245,  2252,
    2261,  2272,  2280,  2288,  2292,  2300,  2304,  2311,  2312,  2316,
    2324,  2331,  2339,  2345,  2355,  2362,  2370,  2376,  2386,  2387,
    2388,  2393,  2398,  2407,  2415,  2428,  2433,  2441,  2446,  2451,
    2456
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_AMPERSAND", "T_ANY", "T_ASTERISK", 
  "T_ATTRIBUTE", "T_BOOLEAN", "T_CASE", "T_CHAR", "T_CHARACTER_LITERAL", 
  "T_WCHARACTER_LITERAL", "T_CIRCUMFLEX", "T_COLON", "T_COMMA", "T_CONST", 
  "T_CONTEXT", "T_DEFAULT", "T_DOUBLE", "T_ENUM", "T_EQUAL", 
  "T_EXCEPTION", "T_FALSE", "T_FIXED", "T_FIXED_PT_LITERAL", "T_FLOAT", 
  "T_FLOATING_PT_LITERAL", "T_GREATER_THAN_SIGN", "T_IDENTIFIER", "T_IN", 
  "T_INOUT", "T_INTEGER_LITERAL", "T_INTERFACE", "T_LEFT_CURLY_BRACKET", 
  "T_LEFT_PARANTHESIS", "T_LEFT_SQUARE_BRACKET", "T_LESS_THAN_SIGN", 
  "T_LONG", "T_MINUS_SIGN", "T_MODULE", "T_OCTET", "T_ONEWAY", "T_OUT", 
  "T_PERCENT_SIGN", "T_PLUS_SIGN", "T_RAISES", "T_READONLY", 
  "T_RIGHT_CURLY_BRACKET", "T_RIGHT_PARANTHESIS", 
  "T_RIGHT_SQUARE_BRACKET", "T_SCOPE", "T_SEMICOLON", "T_SEQUENCE", 
  "T_SHIFTLEFT", "T_SHIFTRIGHT", "T_SHORT", "T_SOLIDUS", "T_STRING", 
  "T_STRING_LITERAL", "T_WSTRING_LITERAL", "T_PRAGMA", "T_INCLUDE", 
  "T_STRUCT", "T_SWITCH", "T_TILDE", "T_TRUE", "T_OBJECT", "T_TYPEDEF", 
  "T_UNION", "T_UNSIGNED", "T_VERTICAL_LINE", "T_VOID", "T_WCHAR", 
  "T_WSTRING", "T_UNKNOWN", "T_ABSTRACT", "T_VALUETYPE", "T_TRUNCATABLE", 
  "T_SUPPORTS", "T_CUSTOM", "T_PUBLIC", "T_PRIVATE", "T_FACTORY", 
  "T_NATIVE", "T_VALUEBASE", "T_IMPORT", "T_TYPEID", "T_TYPEPREFIX", 
  "T_GETRAISES", "T_SETRAISES", "T_LOCAL", "T_COMPONENT", "T_PROVIDES", 
  "T_USES", "T_MULTIPLE", "T_EMITS", "T_PUBLISHES", "T_CONSUMES", 
  "T_HOME", "T_MANAGES", "T_PRIMARYKEY", "T_FINDER", "T_EVENTTYPE", 
  "$accept", "specification", "definitions", "definition", "module", 
  "interface", "interface_dcl", "forward_dcl", "interface_header", 
  "interface_body", "exports", "export", "interface_inheritance_spec", 
  "interface_names", "scoped_names", "scoped_name", "value", 
  "value_forward_dcl", "value_box_dcl", "value_abs_dcl", "value_body", 
  "value_dcl", "value_elements", "value_header", "value_inheritance_spec", 
  "value_inheritance_bases", "value_names", "value_name", "value_element", 
  "state_member", "init_dcl", "init_param_decls", "init_param_decl", 
  "init_param_attribute", "const_dcl", "const_type", "const_exp", 
  "or_expr", "xor_expr", "and_expr", "shift_expr", "add_expr", 
  "mult_expr", "unary_expr", "primary_expr", "literal", 
  "positive_int_const", "type_dcl", "type_spec", "simple_type_spec", 
  "base_type_spec", "template_type_spec", "constr_type_spec", 
  "declarators", "declarator", "simple_declarator", "complex_declarator", 
  "floating_pt_type", "integer_type", "signed_int", "signed_short_int", 
  "signed_long_int", "signed_longlong_int", "unsigned_int", 
  "unsigned_short_int", "unsigned_long_int", "unsigned_longlong_int", 
  "char_type", "wide_char_type", "boolean_type", "octet_type", "any_type", 
  "object_type", "struct_type", "member_list", "member", "union_type", 
  "switch_type_spec", "switch_body", "case", "case_label", "element_spec", 
  "enum_type", "enumerators", "enumerator", "sequence_type", 
  "string_type", "wide_string_type", "array_declarator", 
  "fixed_array_sizes", "fixed_array_size", "attr_dcl", "except_dcl", 
  "members", "op_dcl", "op_attribute", "op_type_spec", "parameter_dcls", 
  "param_dcls", "param_dcl", "param_attribute", "raises_expr", 
  "context_expr", "string_literals", "T_string_literal", 
  "T_wstring_literal", "param_type_spec", "fixed_pt_type", 
  "fixed_pt_const_type", "value_base_type", "constr_forward_decl", 
  "import", "imported_scope", "type_id_dcl", "type_prefix_dcl", 
  "readonly_attr_spec", "readonly_attr_declarator", "simple_declarators", 
  "attr_spec", "attr_declarator", "attr_raises_expr", "get_excep_expr", 
  "set_excep_expr", "exception_list", "component", 
  "component_forward_dcl", "component_dcl", "component_header", 
  "supported_interface_spec", "component_inheritance_spec", 
  "component_body", "component_export", "provides_dcl", "interface_type", 
  "uses_dcl", "emits_dcl", "publishes_dcl", "consumes_dcl", "home_dcl", 
  "home_header", "home_inheritance_spec", "primary_key_spec", "home_body", 
  "home_exports", "home_export", "factory_dcl", "finder_dcl", "event", 
  "event_forward_dcl", "event_abs_dcl", "event_dcl", "event_header", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   103,   104,   104,   105,   105,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     107,   108,   108,   109,   110,   110,   110,   111,   111,   111,
     111,   111,   111,   112,   112,   113,   113,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   115,   116,   117,   117,
     118,   118,   118,   119,   119,   119,   119,   120,   120,   121,
     122,   122,   123,   123,   124,   124,   125,   125,   126,   126,
     126,   126,   127,   127,   127,   128,   128,   128,   128,   129,
     130,   131,   131,   131,   132,   132,   133,   133,   134,   134,
     135,   136,   137,   138,   138,   138,   138,   138,   138,   138,
     138,   138,   138,   139,   140,   140,   141,   141,   142,   142,
     143,   143,   143,   144,   144,   144,   145,   145,   145,   145,
     146,   146,   146,   146,   147,   147,   147,   148,   148,   148,
     148,   148,   148,   148,   148,   148,   149,   150,   150,   150,
     150,   150,   150,   151,   151,   152,   152,   152,   153,   153,
     153,   153,   153,   153,   153,   153,   153,   154,   154,   154,
     154,   155,   155,   155,   156,   156,   157,   157,   158,   159,
     160,   160,   160,   161,   161,   162,   162,   162,   163,   164,
     165,   166,   166,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   177,   178,   178,   178,   179,
     180,   180,   180,   180,   180,   181,   181,   182,   182,   182,
     183,   183,   184,   185,   186,   186,   186,   187,   188,   188,
     189,   189,   190,   190,   191,   192,   192,   193,   194,   194,
     195,   196,   196,   197,   197,   197,   197,   198,   198,   199,
     199,   200,   200,   201,   201,   202,   203,   203,   203,   204,
     205,   206,   206,   207,   207,   208,   208,   209,   209,   209,
     210,   211,   212,   213,   213,   214,   215,   215,   216,   217,
     218,   219,   219,   220,   220,   221,   222,   222,   223,   223,
     223,   223,   224,   225,   226,   227,   227,   228,   229,   229,
     230,   230,   230,   230,   231,   232,   233,   233,   234,   234,
     234,   234,   234,   234,   235,   236,   236,   237,   237,   238,
     239,   240,   241,   242,   242,   242,   242,   242,   242,   242,
     242,   243,   244,   245,   245,   246,   246,   247,   247,   247,
     248,   248,   248,   248,   249,   249,   249,   249,   250,   250,
     250,   251,   251,   252,   252,   253,   253,   254,   254,   254,
     254
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     1,     1,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       5,     1,     1,     4,     2,     3,     3,     2,     3,     3,
       4,     3,     4,     0,     1,     1,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     1,     2,     1,     1,     3,
       1,     2,     3,     1,     1,     1,     1,     2,     3,     3,
       6,     7,     0,     1,     4,     3,     1,     2,     3,     4,
       2,     3,     2,     4,     2,     1,     3,     2,     4,     1,
       1,     1,     1,     1,     4,     4,     6,     5,     1,     3,
       3,     1,     5,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       2,     2,     2,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     2,     2,     3,     1,     1,     1,
       1,     1,     1,     5,     1,     2,     3,     1,     1,     9,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     4,
       3,     2,     2,     5,     1,     3,     2,     1,     6,     4,
       4,     1,     4,     1,     2,     1,     2,     3,     1,     1,
       5,     0,     2,     6,     5,     5,     4,     0,     1,     1,
       1,     3,     2,     1,     3,     3,     1,     1,     1,     4,
       4,     1,     3,     1,     2,     1,     2,     1,     1,     1,
       6,     1,     1,     2,     2,     2,     1,     1,     3,     3,
       4,     2,     1,     1,     3,     3,     2,     1,     1,     1,
       2,     2,     2,     2,     3,     1,     1,     2,     4,     3,
       2,     3,     3,     4,     2,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     3,     1,     1,     3,     4,     3,
       3,     3,     2,     4,     5,     5,     6,     5,     6,     6,
       7,     2,     2,     3,     2,     1,     2,     1,     2,     2,
       6,     5,     5,     4,     6,     5,     5,     4,     1,     1,
       1,     2,     3,     6,     7,     4,     3,     3,     4,     2,
       3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       2,     0,     0,     0,     0,     0,    18,    19,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     4,     0,     0,    21,    22,     0,
       0,    56,    55,    54,    53,     0,     0,     0,   138,   139,
     140,     0,   142,     0,     0,     0,     0,   286,   285,     0,
       0,     0,     0,   340,   339,   338,     0,   189,   187,   171,
     261,   170,    50,   179,   190,     0,   178,   221,     0,   188,
     223,   101,     0,    97,    93,   173,   176,   175,   177,   174,
     182,   181,   183,    94,    95,    96,   102,    98,    99,   100,
       0,     0,    24,     0,   263,   191,     0,     0,     0,   192,
       0,   262,   147,     0,   143,   145,   146,   144,   148,   149,
     150,   151,   152,   153,   154,   155,   161,   162,   163,   157,
     158,   159,   160,   156,   264,     0,     0,     0,    57,     0,
       0,   168,   141,   253,   266,   267,   265,     0,     0,     0,
     287,     0,   341,     1,     5,    10,     9,   237,    11,   237,
       7,     6,     8,    17,    12,    13,    15,     0,    16,   237,
     312,    14,   237,   172,   180,    51,     0,   185,   184,     0,
       0,     0,     0,   231,     0,    28,     0,     0,     0,     0,
       0,     0,   168,   137,   164,   166,   167,   169,     0,    25,
      58,   342,     0,     0,    68,    59,    71,   350,   254,   268,
     269,    26,     0,     0,   292,   291,     0,     0,     0,     0,
     347,     0,   238,     0,    44,    45,     0,    34,   237,     0,
       0,     0,     0,     0,     0,     0,     0,   228,   229,    65,
       0,     0,     0,    81,     0,   237,    82,    83,   289,     0,
       0,     0,     0,     0,     0,     0,   296,     0,     0,     0,
       0,     0,   324,     0,     0,   327,     0,   237,     0,     0,
     346,     0,   130,   131,   135,   132,   133,   127,     0,     0,
       0,   255,     0,   134,   124,   136,   103,   104,   106,   108,
     110,   113,   116,   123,   125,     0,   128,   129,   186,     0,
      52,     0,   217,     0,     0,   214,   197,   198,     0,   231,
       0,    46,    47,    48,     0,     0,   194,     0,     0,     0,
     224,   225,     0,     0,    30,   237,     0,   237,     0,     0,
      80,    72,    75,    74,    69,   348,    32,   295,   294,   293,
     321,   313,     0,     0,     0,   259,   257,   258,     0,     0,
      23,    36,    38,    37,    40,    39,    41,   240,     0,   239,
      42,    43,     0,     0,     0,    64,    67,   306,   305,     0,
       0,     0,     0,     0,     0,   303,   288,   297,   298,   299,
     300,   301,   302,     0,     0,   323,   326,   328,   329,   345,
       0,   120,   121,   256,   122,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   220,   222,    92,   216,   213,
       0,     0,   232,   230,     0,    20,   193,   195,     0,     0,
     219,     0,   226,   165,   179,   204,   200,   201,   202,     0,
     203,    63,     0,   237,     0,   237,    77,     0,     0,     0,
     315,   317,   314,     0,   273,   277,   275,     0,     0,     0,
       0,     0,   304,     0,   307,   309,   310,   311,     0,     0,
     126,   105,   107,   109,   112,   111,   115,   114,   117,   119,
     118,   215,   196,    49,     0,     0,   227,     0,    60,     0,
     343,     0,     0,    73,    79,    76,   322,   319,   316,   318,
       0,     0,     0,   276,   278,   279,   273,   270,   272,     0,
     236,    84,    85,    91,     0,     0,    88,     0,   308,   333,
       0,   337,     0,   260,   218,     0,    61,   344,    78,   320,
     273,   274,     0,   282,   283,   280,   281,     0,   271,   246,
     248,   247,   242,     0,   243,     0,     0,   235,   234,    87,
       0,     0,     0,   331,   332,   335,   336,     0,     0,     0,
     205,     0,     0,     0,   241,     0,     0,     0,   233,    86,
      89,    90,   330,   334,     0,   211,   199,   206,     0,     0,
     207,     0,   284,     0,   244,   245,     0,   251,   210,     0,
     212,   208,   249,   250,     0,   209,   252
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    22,    23,    24,    25,    26,    27,    28,    29,   216,
     421,   218,   175,   301,   302,   274,    30,    31,    32,    33,
     422,    34,   234,    35,   194,   321,   475,   322,   235,   236,
     237,   495,   496,   497,   219,    72,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   285,   220,   298,   104,
     105,   106,   107,   183,   184,   185,   186,   108,   109,    75,
      76,    77,    78,    79,    80,    81,    82,   110,   111,   112,
     113,   114,   115,    38,   305,   299,    39,   419,   539,   540,
     541,   561,    40,   294,   295,   119,   120,   121,   187,   310,
     311,   221,   222,   300,   223,   224,   348,   490,   523,   524,
     525,   518,   528,   566,   286,   287,   338,   122,    89,   123,
      42,    43,   136,   225,   226,   227,   487,   435,   228,   436,
     483,   484,   485,   513,    46,    47,    48,    49,   204,   205,
     245,   246,   247,   359,   248,   249,   250,   251,    50,    51,
     209,   430,   160,   256,   257,   258,   259,    52,    53,    54,
      55,    56
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -437
static const short yypact[] =
{
     193,  1011,    58,   159,   185,   191,  -437,  -437,   207,   889,
     218,    30,   221,    89,   222,    45,    38,    38,   227,   261,
     272,   280,   312,  -437,   193,   266,   270,  -437,  -437,   292,
     277,  -437,  -437,  -437,  -437,   296,   279,   286,  -437,  -437,
    -437,   287,  -437,   294,   297,   298,   299,  -437,  -437,   310,
     300,   319,   302,  -437,  -437,  -437,   321,  -437,  -437,  -437,
    -437,  -437,  -437,   135,  -437,   330,  -437,   323,   248,  -437,
     328,   311,   337,  -437,  -437,  -437,  -437,  -437,  -437,  -437,
    -437,  -437,  -437,  -437,  -437,  -437,  -437,  -437,  -437,  -437,
     334,   335,   187,   336,   340,  -437,   338,   339,   342,  -437,
     348,  -437,   311,   351,  -437,  -437,  -437,  -437,  -437,  -437,
    -437,  -437,  -437,  -437,  -437,  -437,  -437,  -437,  -437,  -437,
    -437,  -437,  -437,  -437,   317,   355,   356,   360,   698,   361,
     362,  -437,  -437,   333,   311,  -437,  -437,   240,   240,   366,
      42,    26,    56,  -437,  -437,  -437,  -437,   557,  -437,   394,
    -437,  -437,  -437,  -437,  -437,  -437,  -437,    55,  -437,   295,
    -437,  -437,   424,  -437,  -437,  -437,  1043,   358,  -437,  1043,
     369,   378,    31,   822,    38,  -437,   193,   822,  1043,   971,
     340,   317,   364,  -437,   387,  -437,  -437,  -437,   368,   205,
      59,    63,   132,    38,  -437,  -437,    57,    57,  -437,  -437,
    -437,   254,    38,    38,  -437,   327,    38,    38,   304,   137,
    -437,   971,  -437,   400,  -437,  -437,   363,  -437,   614,   365,
     367,   370,   371,   372,   931,   373,   380,  -437,  -437,  -437,
     889,   889,   386,  -437,   385,   527,  -437,  -437,  -437,   131,
      14,    38,    38,    38,   383,   389,    71,   391,   393,   395,
     396,   397,  -437,   392,   409,  -437,   402,   325,   399,   401,
    -437,   404,  -437,  -437,  -437,  -437,  -437,  -437,  1043,  1087,
    1087,   379,  1087,  -437,   311,  -437,   347,   407,   450,   101,
      41,    51,  -437,  -437,  -437,   430,  -437,  -437,  -437,   431,
    -437,  1043,  -437,    31,   412,   446,  -437,  -437,   351,   822,
     416,  -437,  -437,    43,   417,   419,   822,   453,   130,  1043,
    -437,   364,   351,   238,  -437,   626,   436,   626,   440,    38,
     311,   405,   464,  -437,  -437,  -437,  -437,   311,  -437,  -437,
     311,    -3,    38,    38,   388,   311,  -437,  -437,   222,   971,
    -437,  -437,  -437,  -437,  -437,  -437,  -437,  -437,   451,  -437,
    -437,  -437,   351,   351,   448,  -437,  -437,  -437,   311,   460,
     131,   461,   128,   160,   171,  -437,  -437,  -437,  -437,  -437,
    -437,  -437,  -437,   456,   459,  -437,  -437,  -437,  -437,  -437,
     447,  -437,  -437,  -437,  -437,  1043,  1043,  1043,  1043,  1043,
    1043,  1043,  1043,  1043,  1043,  -437,  -437,  -437,  -437,  -437,
      31,   443,  -437,  -437,    38,  -437,  -437,  -437,   465,  1043,
    -437,   449,  -437,  -437,   462,   311,  -437,  -437,  -437,   452,
    -437,  -437,   454,   626,   455,   626,   489,    38,    38,    38,
    -437,    -3,    -3,    38,    23,  -437,  -437,   222,   474,   458,
     463,   208,  -437,   485,  -437,  -437,  -437,  -437,   215,   233,
    -437,   407,   450,   101,    41,    41,    51,    51,  -437,  -437,
    -437,  -437,  -437,  -437,   488,   490,  -437,   483,  -437,   471,
    -437,   472,    38,  -437,  -437,  -437,   311,  -437,  -437,    -3,
     222,   487,   487,  -437,   433,   435,    54,  -437,  -437,   244,
     100,  -437,  -437,  -437,   473,   478,   513,   971,  -437,   484,
     480,   484,   482,  -437,  -437,    70,  -437,  -437,  -437,  -437,
     517,  -437,    38,  -437,  -437,  -437,  -437,   501,  -437,  -437,
    -437,  -437,  -437,   492,   522,   971,   504,   529,  -437,  -437,
     496,   520,   222,  -437,   484,  -437,   484,  1043,   537,   505,
      70,   770,   503,    38,  -437,   305,   222,   333,  -437,  -437,
    -437,  -437,  -437,  -437,   540,  -437,  -437,  -437,   889,   351,
    -437,   506,  -437,   507,  -437,  -437,   512,   550,  -437,   514,
    -437,  -437,  -437,  -437,   333,  -437,  -437
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -437,  -437,     7,  -437,  -437,  -437,  -437,  -437,  -437,  -437,
     -89,  -114,  -118,  -179,  -382,     0,  -437,  -437,  -437,  -437,
    -283,  -437,  -122,  -437,    74,  -437,    94,   250,  -437,  -437,
    -437,  -398,  -437,  -437,    19,  -437,  -255,  -437,   182,   184,
     188,  -199,  -168,  -208,  -188,  -437,  -151,    20,    -4,   398,
    -203,  -201,  -437,  -238,    12,   -12,  -437,   579,     2,  -437,
    -437,  -437,  -437,  -437,  -437,  -437,  -437,     5,   580,    10,
     581,  -437,  -437,     3,   278,  -144,    21,  -437,    46,    44,
    -437,    32,    -2,  -261,  -437,  -437,   582,   590,  -437,   281,
    -437,  -119,    25,   301,  -437,  -437,  -437,  -437,    48,  -437,
    -437,  -436,    69,    27,   -15,   326,  -220,  -437,  -437,  -437,
    -437,  -437,  -437,    28,    29,  -437,  -437,  -370,  -437,  -437,
    -437,   114,   118,   123,  -437,  -437,  -437,  -437,  -100,  -437,
     375,  -437,  -437,  -214,  -437,  -437,  -437,  -437,  -437,  -437,
    -437,  -262,  -437,   349,  -437,  -437,  -437,  -437,  -437,  -437,
    -437,  -437
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, parse error.  */
#define YYTABLE_NINF -350
static const short yytable[] =
{
     135,    71,   132,    74,   349,   103,    83,   118,   336,   102,
     337,    85,   116,   380,   323,   134,   137,   138,   289,    36,
      37,   336,   463,   337,   328,    41,   361,   307,    44,    45,
     117,   144,   398,   306,   424,   233,   397,   480,   244,   206,
     261,   208,    62,    36,    37,   255,   474,   170,   233,    41,
     500,   502,    44,    45,   527,   202,   392,   404,   217,   292,
     401,   211,   125,   533,    65,   535,    62,   488,   480,   192,
     192,   314,   192,    62,   413,  -290,   192,   211,   537,   390,
     357,   381,   382,   326,   384,   391,    90,   538,    65,  -349,
     474,   293,   315,   170,   393,    65,   317,   429,   552,   517,
     553,   213,   238,   133,   203,   329,   126,   394,   360,   334,
     511,   481,   482,   356,   439,   440,   526,   213,   198,   437,
     203,   233,   199,   200,   195,   207,   118,   244,   102,   341,
     542,   116,   127,   550,   193,   193,   336,   193,   337,   461,
     469,   193,   471,   255,   409,   517,   443,   239,   240,   117,
     241,   242,   243,   163,   388,   389,   445,   410,   411,    62,
      62,   563,   306,   239,   240,   129,   241,   242,   243,   477,
     478,   118,   164,   102,   303,   118,   116,   102,   170,   102,
     116,    65,    65,   304,   458,   459,   460,    91,   446,   454,
     455,   130,   320,   303,   117,    36,    37,   357,   117,   447,
     174,    41,   327,   303,    44,    45,   330,   331,     1,   319,
     170,   335,     2,    92,     3,   203,   210,   509,   174,    93,
     -27,   170,   456,   457,   335,     4,   352,   353,   118,   118,
     102,   102,     5,   116,   116,    94,   333,   493,   -29,   358,
     358,   362,   363,   364,   493,    57,   124,    58,   473,   128,
     131,   117,   117,     6,     7,     8,   494,     2,   465,   139,
       9,    10,   493,   499,   316,   318,    62,   174,    11,    12,
     324,   325,    13,   519,   520,   414,    14,   532,    15,    16,
      17,   501,   554,    18,    19,   167,   521,   -31,    65,   140,
     170,    20,   522,    66,   336,    21,   337,   118,   133,   102,
     141,   211,   116,   168,   118,   546,   102,    68,   142,   116,
       1,   420,   143,   415,     2,   416,     3,   145,   417,   320,
     117,   146,   336,   418,   337,   147,   434,   117,   148,   149,
     150,   211,   431,   432,   519,   520,   212,   151,   152,   335,
       1,   213,   252,   157,     2,   153,     3,   521,   154,   155,
     156,   158,   159,   161,   162,   214,   215,     8,   165,   166,
     358,   170,     9,    10,   169,   171,   212,   172,   173,   176,
     180,   213,  -325,   177,   178,   179,   181,   253,    14,   182,
     188,    16,    17,   189,   190,   214,   215,     8,   191,   196,
     197,   133,     9,    10,   201,   288,   254,   290,   291,   309,
     211,   312,   313,   332,   303,   203,   339,   253,    14,     1,
     340,    16,    17,     2,   354,     3,   342,   385,   343,   386,
     373,   344,   345,   346,   350,   486,   254,   303,   303,   476,
     211,   351,   355,   479,   365,   212,   366,   374,   271,     1,
     213,   229,   368,     2,   369,     3,   370,   371,   372,   375,
     377,   379,   378,   387,   214,   215,     8,   395,   396,   399,
     400,     9,    10,   403,   405,   212,   406,   408,   510,   423,
     213,   260,   303,   425,   230,   231,   232,    14,   428,   438,
      16,    17,   441,   427,   214,   215,     8,   433,   442,   444,
     448,     9,    10,   449,   462,   450,   464,   335,   466,   164,
     467,   468,   470,   472,   230,   231,   232,    14,   489,   491,
      16,    17,   303,   498,   492,   503,   505,   504,   506,   507,
     551,   512,   482,   481,   529,   335,   530,   531,   534,   517,
     536,   480,   567,   211,   565,   543,   545,   559,   547,   118,
     544,   102,     1,   303,   116,   526,     2,   549,     3,   493,
     555,   562,   556,   568,   559,   572,   118,   571,   102,   567,
     573,   116,   117,   211,   574,   575,   508,   451,   212,   426,
     452,   570,     1,   213,   -66,   453,     2,   308,     3,   117,
      73,    84,    86,    87,   407,   560,   557,   214,   215,     8,
     569,    88,   412,   564,     9,    10,   548,   383,   212,   516,
     402,   576,   515,   213,   -33,   514,   376,   230,   231,   232,
      14,     0,     0,    16,    17,     0,     0,   214,   215,     8,
     211,   367,     0,     0,     9,    10,     0,     0,     0,     1,
       0,     0,   211,     2,     0,     3,     0,     0,     0,     0,
      14,     1,     0,    16,    17,     2,     0,     3,     0,     0,
       0,     0,     0,     0,     0,   212,     0,     0,     0,     0,
     213,   -35,     0,     0,     0,     0,     0,   212,     0,     0,
       0,     0,   213,   -62,   214,   215,     8,     0,     0,     0,
       0,     9,    10,     0,     0,     0,   214,   215,     8,     0,
       0,     0,     0,     9,    10,     0,     0,    14,     0,     0,
      16,    17,    95,     0,     0,    57,     0,    58,     0,    14,
       0,   192,    16,    17,     0,     0,    59,     2,     0,     0,
       0,    96,     0,    61,     0,     0,    62,     0,     0,     0,
       0,   -70,     0,     0,     0,    63,     0,     0,    64,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    65,     0,
      97,     0,     0,    66,     0,    67,     0,     0,     0,     0,
      98,     0,     0,     0,    99,     0,   100,    68,     0,     0,
      69,    70,     0,     0,    95,     0,   193,    57,   537,    58,
       0,     0,   101,     0,     0,     0,     0,   538,    59,     2,
       0,     0,     0,    96,     0,    61,     0,     0,    62,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      65,     0,    97,     0,     0,    66,    95,    67,     0,    57,
     558,    58,    98,     0,     0,     0,    99,     0,   100,    68,
      59,     2,    69,    70,     0,    96,     0,    61,     0,     0,
      62,     0,     0,     0,   101,     0,     0,     0,     0,    63,
       0,     0,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    65,     0,    97,     0,     0,    66,     0,    67,
       0,     0,   296,   297,    98,     0,     0,     0,    99,     0,
     100,    68,     0,    95,    69,    70,    57,     0,    58,     0,
       0,     0,     0,     0,     0,     0,   101,    59,     2,     0,
       0,     0,    96,     0,    61,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,    63,     0,     0,    64,
       0,     0,     0,     0,     0,    95,     0,     0,    57,    65,
      58,    97,     0,     0,    66,     0,    67,     0,     0,    59,
       0,    98,     0,     0,    96,    99,    61,   100,    68,    62,
       0,    69,    70,     0,     0,     0,     0,     0,    63,     0,
       0,    64,     0,   101,     0,    95,     0,     0,    57,     0,
      58,    65,     0,    97,     0,     0,    66,     0,    67,    59,
       0,     0,     0,     0,    96,     0,    61,    99,     0,    62,
      68,     0,   347,    69,    70,     0,     0,     0,    63,     0,
       0,    64,     0,     0,     0,   101,     0,     0,    57,     0,
      58,    65,     0,    97,     0,     0,    66,     0,    67,    59,
       0,     0,     0,     0,    60,     0,    61,    99,     0,    62,
      68,     0,     0,    69,    70,     0,     0,     0,    63,     0,
       0,    64,     0,   262,   263,   101,     0,     0,     0,     0,
       0,    65,     0,     0,     0,   264,    66,   265,    67,   266,
       0,    62,     0,     0,   267,     0,     0,   268,     0,     0,
      68,   269,     0,    69,    70,     0,     0,   270,     0,     0,
       0,     0,     0,    65,     0,     0,     0,   262,   263,     0,
       0,   133,   271,     0,     0,     0,     0,   272,   273,   264,
       0,   265,     0,   266,     0,    62,     0,     0,   267,     0,
       0,   268,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    65,     0,     0,
       0,     0,     0,     0,     0,   133,   271,     0,     0,     0,
       0,     0,   273
};

static const short yycheck[] =
{
      15,     1,    14,     1,   224,     9,     1,     9,   211,     9,
     211,     1,     9,   268,   193,    15,    16,    17,   169,     0,
       0,   224,   404,   224,   203,     0,   240,   178,     0,     0,
       9,    24,   293,   177,   317,   149,   291,    14,   157,    13,
     162,   141,    28,    24,    24,   159,   428,    50,   162,    24,
     448,   449,    24,    24,   490,    13,     5,    14,   147,    28,
     298,     6,    32,   499,    50,   501,    28,   437,    14,    13,
      13,   189,    13,    28,   312,    33,    13,     6,     8,    38,
      66,   269,   270,   201,   272,    44,    28,    17,    50,    33,
     472,    60,    33,    50,    43,    50,    33,   100,   534,    45,
     536,    46,    47,    58,    78,   205,    76,    56,    94,   209,
     480,    88,    89,   235,   352,   353,    16,    46,   133,   339,
      78,   235,   137,   138,   128,    99,   128,   246,   128,   218,
     512,   128,   102,   531,    78,    78,   339,    78,   339,   400,
     423,    78,   425,   257,    14,    45,   360,    92,    93,   128,
      95,    96,    97,    18,    53,    54,    28,    27,   309,    28,
      28,   543,   306,    92,    93,    76,    95,    96,    97,   431,
     432,   173,    37,   173,   174,   177,   173,   177,    50,   179,
     177,    50,    50,   176,   392,   393,   394,    28,    28,   388,
     389,   102,   192,   193,   173,   176,   176,    66,   177,    28,
      13,   176,   202,   203,   176,   176,   206,   207,    15,    77,
      50,   211,    19,    28,    21,    78,   142,   479,    13,    28,
      33,    50,   390,   391,   224,    32,   230,   231,   230,   231,
     230,   231,    39,   230,   231,    28,    99,    29,    33,   239,
     240,   241,   242,   243,    29,     7,    28,     9,   427,    28,
      28,   230,   231,    60,    61,    62,    48,    19,   409,    32,
      67,    68,    29,    48,   190,   191,    28,    13,    75,    76,
     196,   197,    79,    29,    30,    37,    83,   497,    85,    86,
      87,    48,   537,    90,    91,    37,    42,    33,    50,    28,
      50,    98,    48,    55,   497,   102,   497,   299,    58,   299,
      28,     6,   299,    55,   306,   525,   306,    69,    28,   306,
      15,   313,     0,   313,    19,   313,    21,    51,   313,   319,
     299,    51,   525,   313,   525,    33,   338,   306,    51,    33,
      51,     6,   332,   333,    29,    30,    41,    51,    51,   339,
      15,    46,    47,    33,    19,    51,    21,    42,    51,    51,
      51,    51,    33,    51,    33,    60,    61,    62,    28,    36,
     360,    50,    67,    68,    36,    28,    41,    33,    33,    33,
      28,    46,    47,    33,    36,    36,    28,    82,    83,    28,
      63,    86,    87,    28,    28,    60,    61,    62,    28,    28,
      28,    58,    67,    68,    28,    37,   101,    28,    20,    35,
       6,    14,    34,    99,   404,    78,     6,    82,    83,    15,
      47,    86,    87,    19,    28,    21,    51,    70,    51,    12,
      28,    51,    51,    51,    51,   437,   101,   427,   428,   429,
       6,    51,    47,   433,    51,    41,    47,    28,    59,    15,
      46,    47,    51,    19,    51,    21,    51,    51,    51,    47,
      51,    47,    51,     3,    60,    61,    62,    27,    27,    47,
      14,    67,    68,    47,    47,    41,    47,    14,   480,    33,
      46,    47,   472,    33,    80,    81,    82,    83,    14,    28,
      86,    87,    34,    78,    60,    61,    62,    99,    28,    28,
      34,    67,    68,    34,    51,    48,    31,   497,    49,    37,
      48,    47,    47,    14,    80,    81,    82,    83,    34,    51,
      86,    87,   512,    28,    51,    27,    33,    27,    47,    47,
     532,    34,    89,    88,    51,   525,    48,    14,    48,    45,
      48,    14,   547,     6,   546,    34,    14,   541,    34,   541,
      48,   541,    15,   543,   541,    16,    19,    51,    21,    29,
      13,    48,    47,    13,   558,    48,   558,    51,   558,   574,
      48,   558,   541,     6,    14,    51,   472,   385,    41,   319,
     386,   559,    15,    46,    47,   387,    19,   179,    21,   558,
       1,     1,     1,     1,   306,   541,   540,    60,    61,    62,
     558,     1,   311,   545,    67,    68,   527,   271,    41,   485,
     299,   574,   484,    46,    47,   482,   257,    80,    81,    82,
      83,    -1,    -1,    86,    87,    -1,    -1,    60,    61,    62,
       6,   246,    -1,    -1,    67,    68,    -1,    -1,    -1,    15,
      -1,    -1,     6,    19,    -1,    21,    -1,    -1,    -1,    -1,
      83,    15,    -1,    86,    87,    19,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      46,    47,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    46,    47,    60,    61,    62,    -1,    -1,    -1,
      -1,    67,    68,    -1,    -1,    -1,    60,    61,    62,    -1,
      -1,    -1,    -1,    67,    68,    -1,    -1,    83,    -1,    -1,
      86,    87,     4,    -1,    -1,     7,    -1,     9,    -1,    83,
      -1,    13,    86,    87,    -1,    -1,    18,    19,    -1,    -1,
      -1,    23,    -1,    25,    -1,    -1,    28,    -1,    -1,    -1,
      -1,    33,    -1,    -1,    -1,    37,    -1,    -1,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,
      52,    -1,    -1,    55,    -1,    57,    -1,    -1,    -1,    -1,
      62,    -1,    -1,    -1,    66,    -1,    68,    69,    -1,    -1,
      72,    73,    -1,    -1,     4,    -1,    78,     7,     8,     9,
      -1,    -1,    84,    -1,    -1,    -1,    -1,    17,    18,    19,
      -1,    -1,    -1,    23,    -1,    25,    -1,    -1,    28,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      50,    -1,    52,    -1,    -1,    55,     4,    57,    -1,     7,
      60,     9,    62,    -1,    -1,    -1,    66,    -1,    68,    69,
      18,    19,    72,    73,    -1,    23,    -1,    25,    -1,    -1,
      28,    -1,    -1,    -1,    84,    -1,    -1,    -1,    -1,    37,
      -1,    -1,    40,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    50,    -1,    52,    -1,    -1,    55,    -1,    57,
      -1,    -1,    60,    61,    62,    -1,    -1,    -1,    66,    -1,
      68,    69,    -1,     4,    72,    73,     7,    -1,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    84,    18,    19,    -1,
      -1,    -1,    23,    -1,    25,    -1,    -1,    28,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,    40,
      -1,    -1,    -1,    -1,    -1,     4,    -1,    -1,     7,    50,
       9,    52,    -1,    -1,    55,    -1,    57,    -1,    -1,    18,
      -1,    62,    -1,    -1,    23,    66,    25,    68,    69,    28,
      -1,    72,    73,    -1,    -1,    -1,    -1,    -1,    37,    -1,
      -1,    40,    -1,    84,    -1,     4,    -1,    -1,     7,    -1,
       9,    50,    -1,    52,    -1,    -1,    55,    -1,    57,    18,
      -1,    -1,    -1,    -1,    23,    -1,    25,    66,    -1,    28,
      69,    -1,    71,    72,    73,    -1,    -1,    -1,    37,    -1,
      -1,    40,    -1,    -1,    -1,    84,    -1,    -1,     7,    -1,
       9,    50,    -1,    52,    -1,    -1,    55,    -1,    57,    18,
      -1,    -1,    -1,    -1,    23,    -1,    25,    66,    -1,    28,
      69,    -1,    -1,    72,    73,    -1,    -1,    -1,    37,    -1,
      -1,    40,    -1,    10,    11,    84,    -1,    -1,    -1,    -1,
      -1,    50,    -1,    -1,    -1,    22,    55,    24,    57,    26,
      -1,    28,    -1,    -1,    31,    -1,    -1,    34,    -1,    -1,
      69,    38,    -1,    72,    73,    -1,    -1,    44,    -1,    -1,
      -1,    -1,    -1,    50,    -1,    -1,    -1,    10,    11,    -1,
      -1,    58,    59,    -1,    -1,    -1,    -1,    64,    65,    22,
      -1,    24,    -1,    26,    -1,    28,    -1,    -1,    31,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    50,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    58,    59,    -1,    -1,    -1,
      -1,    -1,    65
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    15,    19,    21,    32,    39,    60,    61,    62,    67,
      68,    75,    76,    79,    83,    85,    86,    87,    90,    91,
      98,   102,   104,   105,   106,   107,   108,   109,   110,   111,
     119,   120,   121,   122,   124,   126,   137,   150,   176,   179,
     185,   195,   213,   214,   216,   217,   227,   228,   229,   230,
     241,   242,   250,   251,   252,   253,   254,     7,     9,    18,
      23,    25,    28,    37,    40,    50,    55,    57,    69,    72,
      73,   118,   138,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   189,   190,   211,
      28,    28,    28,    28,    28,     4,    23,    52,    62,    66,
      68,    84,   118,   151,   152,   153,   154,   155,   160,   161,
     170,   171,   172,   173,   174,   175,   176,   179,   185,   188,
     189,   190,   210,   212,    28,    32,    76,   102,    28,    76,
     102,    28,   158,    58,   118,   207,   215,   118,   118,    32,
      28,    28,    28,     0,   105,    51,    51,    33,    51,    33,
      51,    51,    51,    51,    51,    51,    51,    33,    51,    33,
     245,    51,    33,    18,    37,    28,    36,    37,    55,    36,
      50,    28,    33,    33,    13,   115,    33,    33,    36,    36,
      28,    28,    28,   156,   157,   158,   159,   191,    63,    28,
      28,    28,    13,    78,   127,   151,    28,    28,   207,   207,
     207,    28,    13,    78,   231,   232,    13,    99,   231,   243,
     127,     6,    41,    46,    60,    61,   112,   113,   114,   137,
     150,   194,   195,   197,   198,   216,   217,   218,   221,    47,
      80,    81,    82,   114,   125,   131,   132,   133,    47,    92,
      93,    95,    96,    97,   194,   233,   234,   235,   237,   238,
     239,   240,    47,    82,   101,   114,   246,   247,   248,   249,
      47,   125,    10,    11,    22,    24,    26,    31,    34,    38,
      44,    59,    64,    65,   118,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   207,   208,    37,   149,
      28,    20,    28,    60,   186,   187,    60,    61,   151,   178,
     196,   116,   117,   118,   105,   177,   178,   149,   152,    35,
     192,   193,    14,    34,   115,    33,   127,    33,   127,    77,
     118,   128,   130,   116,   127,   127,   115,   118,   116,   231,
     118,   118,    99,    99,   231,   118,   153,   154,   209,     6,
      47,   113,    51,    51,    51,    51,    51,    71,   199,   209,
      51,    51,   151,   151,    28,    47,   125,    66,   118,   236,
      94,   236,   118,   118,   118,    51,    47,   233,    51,    51,
      51,    51,    51,    28,    28,    47,   246,    51,    51,    47,
     139,   147,   147,   208,   147,    70,    12,     3,    53,    54,
      38,    44,     5,    43,    56,    27,    27,   139,   186,    47,
      14,   156,   196,    47,    14,    47,    47,   177,    14,    14,
      27,   149,   192,   156,    37,   118,   161,   170,   172,   180,
     185,   113,   123,    33,   123,    33,   130,    78,    14,   100,
     244,   118,   118,    99,   158,   220,   222,   209,    28,   156,
     156,    34,    28,   236,    28,    28,    28,    28,    34,    34,
      48,   141,   142,   143,   144,   144,   145,   145,   146,   146,
     146,   186,    51,   117,    31,   149,    49,    48,    47,   123,
      47,   123,    14,   116,   117,   129,   118,   244,   244,   118,
      14,    88,    89,   223,   224,   225,   158,   219,   220,    34,
     200,    51,    51,    29,    48,   134,   135,   136,    28,    48,
     134,    48,   134,    27,    27,    33,    47,    47,   129,   244,
     158,   220,    34,   226,   226,   225,   224,    45,   204,    29,
      30,    42,    48,   201,   202,   203,    16,   204,   205,    51,
      48,    14,   209,   204,    48,   204,    48,     8,    17,   181,
     182,   183,   117,    34,    48,    14,   209,    34,   205,    51,
     134,   158,   204,   204,   139,    13,    47,   181,    60,   151,
     182,   184,    48,   117,   201,   158,   206,   207,    13,   184,
     157,    51,    48,    48,    14,    51,   206
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");			\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)           \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
#endif

/* YYLEX -- calling `yylex' with the right arguments.  */

#define YYLEX	yylex ()

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)
# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)
/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*-----------------------------.
| Print this symbol on YYOUT.  |
`-----------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yysymprint (FILE* yyout, int yytype, YYSTYPE yyvalue)
#else
yysymprint (yyout, yytype, yyvalue)
    FILE* yyout;
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyout, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyout, yytoknum[yytype], yyvalue);
# endif
    }
  else
    YYFPRINTF (yyout, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyout, ")");
}
#endif /* YYDEBUG. */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
#if defined (__STDC__) || defined (__cplusplus)
yydestruct (int yytype, YYSTYPE yyvalue)
#else
yydestruct (yytype, yyvalue)
    int yytype;
    YYSTYPE yyvalue;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvalue;

  switch (yytype)
    {
      default:
        break;
    }
}



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
#  define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL
# else
#  define YYPARSE_PARAM_ARG YYPARSE_PARAM
#  define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
# endif
#else /* !YYPARSE_PARAM */
# define YYPARSE_PARAM_ARG
# define YYPARSE_PARAM_DECL
#endif /* !YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
# ifdef YYPARSE_PARAM
int yyparse (void *);
# else
int yyparse (void);
# endif
#endif


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of parse errors so far.  */
int yynerrs;


int
yyparse (YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yychar1 = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with.  */

  if (yychar <= 0)		/* This means end of input.  */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more.  */

      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yychar1 = YYTRANSLATE (yychar);

      /* We have to keep this `#if YYDEBUG', since we use variables
	 which are defined only if `YYDEBUG' is set.  */
      YYDPRINTF ((stderr, "Next token is "));
      YYDSYMPRINT ((stderr, yychar1, yylval));
      YYDPRINTF ((stderr, "\n"));
    }

  /* If the proper action on seeing token YYCHAR1 is to reduce or to
     detect an error, take that action.  */
  yyn += yychar1;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yychar1)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %d (%s), ",
	      yychar, yytname[yychar1]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];



#if YYDEBUG
  /* We have to keep this `#if YYDEBUG', since we use variables which
     are defined only if `YYDEBUG' is set.  */
  if (yydebug)
    {
      int yyi;

      YYFPRINTF (stderr, "Reducing via rule %d (line %d), ",
		 yyn - 1, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (yyi = yyprhs[yyn]; yyrhs[yyi] >= 0; yyi++)
	YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
      YYFPRINTF (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif
  switch (yyn)
    {
        case 2:
#line 327 "yacc.yy"
    {
	    theParser->setRootNode( NULL );
	  }
    break;

  case 3:
#line 331 "yacc.yy"
    {
	    theParser->setRootNode( yyvsp[0].node );
	  }
    break;

  case 4:
#line 339 "yacc.yy"
    {
	    yyval.node = new ParseNode( definitions, yyvsp[0].node );
	  }
    break;

  case 5:
#line 343 "yacc.yy"
    {
	    yyval.node = new ParseNode( definitions, yyvsp[-1].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 6:
#line 352 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 7:
#line 356 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 8:
#line 360 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 9:
#line 364 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 10:
#line 368 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 11:
#line 372 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 12:
#line 376 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 13:
#line 380 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 14:
#line 384 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 15:
#line 388 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 16:
#line 392 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 17:
#line 396 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 18:
#line 400 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_pragma );
	    yyval.node->setIdent( yyvsp[0]._str );
	  }
    break;

  case 19:
#line 405 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_include );
	    yyval.node->setIdent( yyvsp[0]._str );
	  }
    break;

  case 20:
#line 415 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_module, yyvsp[-1].node );
	    yyval.node->setIdent( yyvsp[-3].ident );
	  }
    break;

  case 21:
#line 424 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 22:
#line 428 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 23:
#line 437 "yacc.yy"
    {
	    yyval.node = new ParseNode( interface_dcl, yyvsp[-3].node, yyvsp[-1].node );
	  }
    break;

  case 24:
#line 445 "yacc.yy"
    {
	    yyval.node = new ParseNode( forward_dcl );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 25:
#line 450 "yacc.yy"
    {
	    yyval.node = new ParseNode( abstract_forward_dcl );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 26:
#line 455 "yacc.yy"
    {
	    yyval.node = new ParseNode( local_forward_dcl );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 27:
#line 464 "yacc.yy"
    {
	    yyval.node = new ParseNode( interface_header );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 28:
#line 469 "yacc.yy"
    {
	    yyval.node = new ParseNode( interface_header );
	    yyval.node->setIdent( yyvsp[-1].ident );
	    yyval.node->setBranch( 1, yyvsp[0].node );
	  }
    break;

  case 29:
#line 475 "yacc.yy"
    {
	    yyval.node = new ParseNode( abstract_interface_header );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 30:
#line 480 "yacc.yy"
    {
	    yyval.node = new ParseNode( abstract_interface_header );
	    yyval.node->setIdent( yyvsp[-1].ident );
	    yyval.node->setBranch( 1, yyvsp[0].node );
	  }
    break;

  case 31:
#line 486 "yacc.yy"
    {
	    yyval.node = new ParseNode( local_interface_header );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 32:
#line 491 "yacc.yy"
    {
	    yyval.node = new ParseNode( local_interface_header );
	    yyval.node->setIdent( yyvsp[-1].ident );
	    yyval.node->setBranch( 1, yyvsp[0].node );
	  }
    break;

  case 33:
#line 501 "yacc.yy"
    {
	    yyval.node = NULL;
	  }
    break;

  case 34:
#line 505 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 35:
#line 512 "yacc.yy"
    {
	    yyval.node = new ParseNode( exports, yyvsp[0].node );
	  }
    break;

  case 36:
#line 516 "yacc.yy"
    {
	    yyval.node = new ParseNode( exports, yyvsp[-1].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 37:
#line 525 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 38:
#line 529 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 39:
#line 533 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 40:
#line 537 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 41:
#line 541 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 42:
#line 545 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 43:
#line 549 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 44:
#line 553 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_pragma );
	    yyval.node->setIdent( yyvsp[0]._str );
	  }
    break;

  case 45:
#line 558 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_include );
	    yyval.node->setIdent( yyvsp[0]._str );
	  }
    break;

  case 46:
#line 567 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 48:
#line 578 "yacc.yy"
    {
	    yyval.node = new ParseNode( scoped_names, yyvsp[0].node );
	  }
    break;

  case 49:
#line 582 "yacc.yy"
    {
	    yyval.node = new ParseNode( scoped_names, yyvsp[-2].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 50:
#line 591 "yacc.yy"
    {
	    yyval.node = new ParseNode( scoped_name );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 51:
#line 596 "yacc.yy"
    {
	    yyval.node = new ParseNode( scoped_name );
	    string name = "::";
	    name += yyvsp[0].ident;
	    CORBA::string_free( yyvsp[0].ident );
	    yyval.node->setIdent( CORBA::string_dup( name.c_str() ) );
	  }
    break;

  case 52:
#line 604 "yacc.yy"
    {
	    yyval.node = yyvsp[-2].node;
	    string name = yyval.node->getIdent();
	    name += "::";
	    name += yyvsp[0].ident;
	    CORBA::string_free( yyvsp[0].ident );
	    yyval.node->setIdent( CORBA::string_dup( name.c_str() ) );
	  }
    break;

  case 57:
#line 625 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_forward_dcl );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 58:
#line 630 "yacc.yy"
    {
	    yyval.node = new ParseNode( abstract_value_forward_dcl );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 59:
#line 639 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_box_dcl, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-1].ident );
	  }
    break;

  case 60:
#line 649 "yacc.yy"
    {
	    yyval.node = new ParseNode( abstract_value_dcl );
	    yyval.node->setBranch( 1, new ParseNode( value_header ) );
	    yyval.node->getBranch( 1 )->setIdent( yyvsp[-3].ident );
	    yyval.node->setBranch( 2, yyvsp[-1].node );
	  }
    break;

  case 61:
#line 657 "yacc.yy"
    {
	    yyval.node = new ParseNode( abstract_value_dcl );
	    yyval.node->setBranch( 1, new ParseNode( value_header ) );
	    yyval.node->getBranch( 1 )->setIdent( yyvsp[-4].ident );
	    yyval.node->getBranch( 1 )->setBranch( 1, yyvsp[-3].node );
	    yyval.node->setBranch( 2, yyvsp[-1].node );
	  }
    break;

  case 62:
#line 668 "yacc.yy"
    {
	    yyval.node = NULL;
	  }
    break;

  case 63:
#line 672 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 64:
#line 681 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_dcl, yyvsp[-3].node, yyvsp[-1].node );
	  }
    break;

  case 65:
#line 685 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_dcl, yyvsp[-2].node );
	  }
    break;

  case 66:
#line 692 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_elements, yyvsp[0].node );
	  }
    break;

  case 67:
#line 696 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_elements, yyvsp[-1].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 68:
#line 705 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_header, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-1].ident );
	  }
    break;

  case 69:
#line 710 "yacc.yy"
    {
	    yyval.node = new ParseNode( custom_value_header, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-1].ident );
	  }
    break;

  case 70:
#line 715 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_header );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 71:
#line 720 "yacc.yy"
    {
	    yyval.node = new ParseNode( custom_value_header );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 72:
#line 729 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 73:
#line 733 "yacc.yy"
    {
	    yyval.node = yyvsp[-2].node;
	    yyval.node->setBranch( 4, yyvsp[0].node );
	  }
    break;

  case 74:
#line 738 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_inheritance_spec );
	    yyval.node->setBranch( 4, yyvsp[0].node );
	  }
    break;

  case 75:
#line 746 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_inheritance_spec );
	    yyval.node->setBranch( 1, yyvsp[0].node );
	  }
    break;

  case 76:
#line 751 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_inheritance_spec );
	    yyval.node->setBranch( 1, yyvsp[-2].node );
	    yyval.node->setBranch( 3, yyvsp[0].node );
	  }
    break;

  case 77:
#line 757 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_inheritance_spec );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 78:
#line 762 "yacc.yy"
    {
	    yyval.node = new ParseNode( value_inheritance_spec );
	    yyval.node->setBranch( 2, yyvsp[-2].node );
	    yyval.node->setBranch( 3, yyvsp[0].node );
	  }
    break;

  case 84:
#line 788 "yacc.yy"
    {
	    yyval.node = new ParseNode( public_state_member, yyvsp[-2].node, yyvsp[-1].node );
	  }
    break;

  case 85:
#line 792 "yacc.yy"
    {
	    yyval.node = new ParseNode( private_state_member, yyvsp[-2].node, yyvsp[-1].node );
	  }
    break;

  case 86:
#line 802 "yacc.yy"
    {
	    yyval.node = new ParseNode( init_dcl, yyvsp[-2].node );
	    yyval.node->setIdent( yyvsp[-4].ident );
	  }
    break;

  case 87:
#line 809 "yacc.yy"
    {
	    yyval.node = new ParseNode( init_dcl );
	    yyval.node->setIdent( yyvsp[-3].ident );
	  }
    break;

  case 88:
#line 818 "yacc.yy"
    {
	    yyval.node = new ParseNode( init_param_decls, yyvsp[0].node );
	  }
    break;

  case 89:
#line 822 "yacc.yy"
    {
	    yyval.node = new ParseNode( init_param_decls, yyvsp[-2].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 90:
#line 831 "yacc.yy"
    {
	    yyval.node = new ParseNode( init_param_decl, yyvsp[-1].node, yyvsp[0].node );
	  }
    break;

  case 91:
#line 839 "yacc.yy"
    {
	    yyval.node = NULL;
	  }
    break;

  case 92:
#line 847 "yacc.yy"
    {
	    yyval.node = new ParseNode( const_dcl, yyvsp[-3].node, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-2].ident );
	  }
    break;

  case 93:
#line 856 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 94:
#line 860 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 95:
#line 864 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 96:
#line 868 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 97:
#line 872 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 98:
#line 876 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 99:
#line 880 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 100:
#line 884 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 101:
#line 888 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 102:
#line 892 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 103:
#line 900 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 104:
#line 908 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 105:
#line 912 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_vertical_line, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 106:
#line 920 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 107:
#line 924 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_circumflex, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 108:
#line 932 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 109:
#line 936 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_ampersand, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 110:
#line 944 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 111:
#line 948 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_shiftright, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 112:
#line 952 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_shiftleft, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 113:
#line 960 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 114:
#line 964 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_plus_sign, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 115:
#line 968 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_minus_sign, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 116:
#line 976 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 117:
#line 980 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_asterik, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 118:
#line 984 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_solidus, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 119:
#line 988 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_percent_sign, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 120:
#line 997 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_minus_sign, yyvsp[0].node );
	  }
    break;

  case 121:
#line 1001 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_plus_sign, yyvsp[0].node );
	  }
    break;

  case 122:
#line 1005 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_tilde, yyvsp[0].node );
	  }
    break;

  case 123:
#line 1009 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 124:
#line 1017 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 125:
#line 1021 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 126:
#line 1025 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 127:
#line 1034 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_integer_literal );
	    yyval.node->setIntegerLiteral( yyvsp[0]._int );
	  }
    break;

  case 128:
#line 1039 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_string_literal );
	    yyval.node->setStringLiteral( yyvsp[0]._str );
	  }
    break;

  case 129:
#line 1044 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_wstring_literal );
	    yyval.node->setWStringLiteral( yyvsp[0]._wstr );
	  }
    break;

  case 130:
#line 1049 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_character_literal );
	    yyval.node->setCharacterLiteral( yyvsp[0]._char );
	  }
    break;

  case 131:
#line 1054 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_wcharacter_literal );
	    yyval.node->setWCharacterLiteral( yyvsp[0]._wchar );
	  }
    break;

  case 132:
#line 1059 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_fixed_pt_literal );
	    yyval.node->setFixedPtLiteral( yyvsp[0]._fixed );
	  }
    break;

  case 133:
#line 1064 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_floating_pt_literal );
	    yyval.node->setFloatingPtLiteral( yyvsp[0]._float );
	  }
    break;

  case 134:
#line 1069 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_boolean_literal );
	    yyval.node->setBooleanLiteral( true );
	  }
    break;

  case 135:
#line 1074 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_boolean_literal );
	    yyval.node->setBooleanLiteral( false );
	  }
    break;

  case 136:
#line 1083 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 137:
#line 1092 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_typedef, yyvsp[-1].node, yyvsp[0].node );
	  }
    break;

  case 138:
#line 1096 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 139:
#line 1100 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 140:
#line 1104 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 141:
#line 1108 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_native, yyvsp[0].node );
	  }
    break;

  case 143:
#line 1117 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 144:
#line 1121 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 145:
#line 1129 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 146:
#line 1133 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 147:
#line 1137 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 148:
#line 1145 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 149:
#line 1149 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 150:
#line 1153 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 151:
#line 1157 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 152:
#line 1161 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 153:
#line 1165 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 154:
#line 1169 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 155:
#line 1173 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 156:
#line 1177 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 157:
#line 1185 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 158:
#line 1189 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 159:
#line 1193 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 160:
#line 1197 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 161:
#line 1205 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 162:
#line 1209 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 163:
#line 1213 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 164:
#line 1221 "yacc.yy"
    {
	    yyval.node = new ParseNode( declarators, yyvsp[0].node );
	  }
    break;

  case 165:
#line 1225 "yacc.yy"
    {
	    yyval.node = new ParseNode( declarators, yyvsp[-2].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 166:
#line 1234 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 167:
#line 1238 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 168:
#line 1246 "yacc.yy"
    {
	    yyval.node = new ParseNode( simple_declarator );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 169:
#line 1255 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 170:
#line 1263 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_float );
	  }
    break;

  case 171:
#line 1267 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_double );
	  }
    break;

  case 172:
#line 1271 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_long_double );
	  }
    break;

  case 173:
#line 1279 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 174:
#line 1283 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 175:
#line 1291 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 176:
#line 1295 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 177:
#line 1299 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 178:
#line 1307 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_short );
	  }
    break;

  case 179:
#line 1315 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_long );
	  }
    break;

  case 180:
#line 1323 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_longlong );
	  }
    break;

  case 181:
#line 1331 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 182:
#line 1335 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 183:
#line 1339 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 184:
#line 1347 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_unsigned_short );
	  }
    break;

  case 185:
#line 1355 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_unsigned_long );
	  }
    break;

  case 186:
#line 1363 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_unsigned_longlong );
	  }
    break;

  case 187:
#line 1371 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_char );
	  }
    break;

  case 188:
#line 1379 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_wide_char );
	  }
    break;

  case 189:
#line 1387 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_boolean );
	  }
    break;

  case 190:
#line 1395 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_octet );
	  }
    break;

  case 191:
#line 1403 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_any );
	  }
    break;

  case 192:
#line 1411 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_object );
	  }
    break;

  case 193:
#line 1420 "yacc.yy"
    {
	    yyval.node = new ParseNode( struct_type, yyvsp[-1].node );
	    yyval.node->setIdent( yyvsp[-3].ident );
	  }
    break;

  case 194:
#line 1429 "yacc.yy"
    {
	    yyval.node = new ParseNode( member_list, yyvsp[0].node );
	  }
    break;

  case 195:
#line 1433 "yacc.yy"
    {
	    yyval.node = new ParseNode( member_list, yyvsp[-1].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 196:
#line 1442 "yacc.yy"
    {
	    yyval.node = new ParseNode( member, yyvsp[-2].node, yyvsp[-1].node );
	  }
    break;

  case 197:
#line 1446 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_pragma );
	    yyval.node->setIdent( yyvsp[0]._str );
	  }
    break;

  case 198:
#line 1451 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_include );
	    yyval.node->setIdent( yyvsp[0]._str );
	  }
    break;

  case 199:
#line 1462 "yacc.yy"
    {
	    yyval.node = new ParseNode( union_type, yyvsp[-4].node, yyvsp[-1].node );
	    yyval.node->setIdent( yyvsp[-7].ident );
	  }
    break;

  case 200:
#line 1471 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 201:
#line 1475 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 202:
#line 1479 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 203:
#line 1483 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 204:
#line 1487 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 205:
#line 1495 "yacc.yy"
    {
	    yyval.node = new ParseNode( switch_body, yyvsp[0].node );
	  }
    break;

  case 206:
#line 1499 "yacc.yy"
    {
	    yyval.node = new ParseNode( switch_body, yyvsp[-1].node, yyvsp[0].node );
	  }
    break;

  case 207:
#line 1507 "yacc.yy"
    {
	    yyval.node = new ParseNode( _case, yyvsp[-1].node, yyvsp[0].node );
	  }
    break;

  case 208:
#line 1511 "yacc.yy"
    {
	    yyval.node = new ParseNode( _case, yyvsp[-2].node );
	    yyval.node->setBranch( 3, yyvsp[-1].node );
	  }
    break;

  case 209:
#line 1516 "yacc.yy"
    {
	    yyval.node = new ParseNode( _case, yyvsp[-3].node );
	    yyval.node->setBranch( 3, yyvsp[-1].node );
	    ParseNode * pnode = new ParseNode (t_pragma);
	    pnode->setIdent (yyvsp[-2]._str);
	    yyval.node->setBranch( 4, pnode);
	  }
    break;

  case 210:
#line 1528 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 211:
#line 1532 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_default );
	  }
    break;

  case 212:
#line 1540 "yacc.yy"
    {
	    yyval.node = new ParseNode( element_spec, yyvsp[-1].node, yyvsp[0].node );
	  }
    break;

  case 213:
#line 1549 "yacc.yy"
    {
	    yyval.node = new ParseNode( enum_type, yyvsp[-1].node );
	    yyval.node->setIdent( yyvsp[-3].ident );
	  }
    break;

  case 214:
#line 1557 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 215:
#line 1561 "yacc.yy"
    {
	    yyval.node = new ParseNode( enumerators, yyvsp[-2].node, yyvsp[0].node );
	  }
    break;

  case 216:
#line 1565 "yacc.yy"
    {
	    ParseNode *pnode = new ParseNode( t_pragma );
	    pnode->setIdent( yyvsp[-1]._str );
	    yyval.node = new ParseNode( enumerators, pnode, yyvsp[0].node );
	  }
    break;

  case 217:
#line 1575 "yacc.yy"
    {
	    yyval.node = new ParseNode( enumerator );
	    yyval.node->setIdent( yyvsp[0].ident );
	  }
    break;

  case 218:
#line 1585 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_sequence, yyvsp[-3].node, yyvsp[-1].node );
	  }
    break;

  case 219:
#line 1589 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_sequence, yyvsp[-1].node );
	  }
    break;

  case 220:
#line 1597 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_string, yyvsp[-1].node );
	  }
    break;

  case 221:
#line 1601 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_string );
	  }
    break;

  case 222:
#line 1609 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_wide_string, yyvsp[-1].node );
	  }
    break;

  case 223:
#line 1613 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_wide_string );
	  }
    break;

  case 224:
#line 1621 "yacc.yy"
    {
	    yyval.node = new ParseNode( array_declarator, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-1].ident );
	  }
    break;

  case 225:
#line 1629 "yacc.yy"
    {
	    yyval.node = new ParseNode( fixed_array_sizes, yyvsp[0].node );
	  }
    break;

  case 226:
#line 1633 "yacc.yy"
    {
	    yyval.node = new ParseNode( fixed_array_sizes, yyvsp[-1].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 227:
#line 1642 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 230:
#line 1657 "yacc.yy"
    {
	    yyval.node = new ParseNode( except_dcl, yyvsp[-1].node );
	    yyval.node->setIdent( yyvsp[-3].ident );
	  }
    break;

  case 231:
#line 1665 "yacc.yy"
    {
	    yyval.node = NULL;
	  }
    break;

  case 232:
#line 1669 "yacc.yy"
    {
	    yyval.node = new ParseNode( member_list, yyvsp[-1].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 233:
#line 1679 "yacc.yy"
    {
	    yyval.node = new ParseNode( op_dcl, yyvsp[-5].node, yyvsp[-4].node, yyvsp[-2].node, yyvsp[-1].node, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-3].ident );
	  }
    break;

  case 234:
#line 1685 "yacc.yy"
    {
	    yyval.node = new ParseNode( op_dcl, yyvsp[-4].node, yyvsp[-3].node, yyvsp[-1].node, 0, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-2].ident );
	  }
    break;

  case 235:
#line 1691 "yacc.yy"
    {
	    yyval.node = new ParseNode( op_dcl, yyvsp[-4].node, yyvsp[-3].node, yyvsp[-1].node, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-2].ident );
	  }
    break;

  case 236:
#line 1696 "yacc.yy"
    {
	    yyval.node = new ParseNode( op_dcl, yyvsp[-3].node, yyvsp[-2].node, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-1].ident );
	  }
    break;

  case 237:
#line 1705 "yacc.yy"
    {
	    yyval.node = NULL;
	  }
    break;

  case 238:
#line 1709 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_oneway );
	  }
    break;

  case 239:
#line 1717 "yacc.yy"
    {
              yyval.node = new ParseNode( op_type_spec, yyvsp[0].node );
	  }
    break;

  case 240:
#line 1721 "yacc.yy"
    {
              yyval.node = new ParseNode( t_void );
	  }
    break;

  case 241:
#line 1729 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 242:
#line 1733 "yacc.yy"
    {
	    yyval.node = NULL;
	  }
    break;

  case 243:
#line 1740 "yacc.yy"
    {
	    yyval.node = new ParseNode( param_dcls, yyvsp[0].node );
	  }
    break;

  case 244:
#line 1744 "yacc.yy"
    {
	    yyval.node = new ParseNode( param_dcls, yyvsp[-2].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 245:
#line 1753 "yacc.yy"
    {
	    yyval.node = new ParseNode( param_dcl, yyvsp[-2].node, yyvsp[-1].node, yyvsp[0].node );
	  }
    break;

  case 246:
#line 1761 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_in );
	  }
    break;

  case 247:
#line 1765 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_out );
	  }
    break;

  case 248:
#line 1769 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_inout );
	  }
    break;

  case 249:
#line 1777 "yacc.yy"
    {
	    yyval.node = new ParseNode( raises_expr, yyvsp[-1].node );
	  }
    break;

  case 250:
#line 1785 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 251:
#line 1792 "yacc.yy"
    {
	    yyval.node = new ParseNode( string_literals );
	    yyval.node->setIdent( yyvsp[0]._str );
	  }
    break;

  case 252:
#line 1797 "yacc.yy"
    {
	    yyval.node = new ParseNode( string_literals, yyvsp[0].node );
	    yyval.node->setIdent( yyvsp[-2]._str );
	  }
    break;

  case 253:
#line 1805 "yacc.yy"
    {
	    yyval._str = yyvsp[0]._str;
	  }
    break;

  case 254:
#line 1809 "yacc.yy"
    {
	    string s1 = string(yyvsp[-1]._str);
	    string s2 = string(yyvsp[0]._str);
	    CORBA::string_free( yyvsp[-1]._str );
	    CORBA::string_free( yyvsp[0]._str );
	    s1 += s2;
	    yyval._str = CORBA::string_dup( s1.c_str() );
	  }
    break;

  case 255:
#line 1821 "yacc.yy"
    {
	    yyval._wstr = yyvsp[0]._wstr;
	  }
    break;

  case 256:
#line 1825 "yacc.yy"
    {
	    CORBA::WChar *w1, *res;
	    CORBA::ULong cnt;

	    for (cnt=0, w1=yyvsp[-1]._wstr; *w1; w1++, cnt++);
	    for (w1=yyvsp[0]._wstr; *w1; w1++, cnt++);

	    res = CORBA::wstring_alloc (cnt);

	    for (cnt=0, w1=yyvsp[-1]._wstr; *w1; w1++, cnt++) {
	      res[cnt] = *w1;
	    }

	    for (w1=yyvsp[0]._wstr; *w1; w1++, cnt++) {
	      res[cnt] = *w1;
	    }

	    res[cnt] = 0;

	    CORBA::wstring_free( yyvsp[-1]._wstr );
	    CORBA::wstring_free( yyvsp[0]._wstr );
	    yyval._wstr = res;
	  }
    break;

  case 257:
#line 1853 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 258:
#line 1857 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 259:
#line 1861 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 260:
#line 1870 "yacc.yy"
    {
	    yyval.node = new ParseNode( fixed_pt_type, yyvsp[-3].node );
	    yyval.node->setIntegerLiteral( yyvsp[-1]._int );
	  }
    break;

  case 261:
#line 1879 "yacc.yy"
    {
	    yyval.node = new ParseNode( fixed_pt_type );
	  }
    break;

  case 262:
#line 1887 "yacc.yy"
    {
	    yyval.node = new ParseNode( t_valuebase );
	  }
    break;

  case 263:
#line 1895 "yacc.yy"
    {
	    yyval.node = new ParseNode (struct_forward_decl);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 264:
#line 1900 "yacc.yy"
    {
	    yyval.node = new ParseNode (union_forward_decl);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 265:
#line 1910 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_import, yyvsp[0].node);
	  }
    break;

  case 267:
#line 1919 "yacc.yy"
    {
	    yyval.node = new ParseNode (string_literals);
	    yyval.node->setIdent (yyvsp[0]._str);
	  }
    break;

  case 268:
#line 1928 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_typeid, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[0]._str);
	  }
    break;

  case 269:
#line 1937 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_typeprefix, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[0]._str);
	  }
    break;

  case 270:
#line 1946 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_readonly_attribute, yyvsp[-1].node, yyvsp[0].node);
	  }
    break;

  case 271:
#line 1954 "yacc.yy"
    {
	    yyval.node = new ParseNode (simple_declarators, yyvsp[-1].node);
	    yyval.node->setBranch (3, new ParseNode (t_attr_raises, yyvsp[0].node));
	  }
    break;

  case 273:
#line 1963 "yacc.yy"
    {
	    yyval.node = new ParseNode( simple_declarators, yyvsp[0].node );
	  }
    break;

  case 274:
#line 1967 "yacc.yy"
    {
	    yyval.node = new ParseNode( simple_declarators, yyvsp[-2].node );
	    yyval.node->setBranch( 2, yyvsp[0].node );
	  }
    break;

  case 275:
#line 1976 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_attribute, yyvsp[-1].node, yyvsp[0].node);
	  }
    break;

  case 276:
#line 1984 "yacc.yy"
    {
	    yyval.node = new ParseNode (simple_declarators, yyvsp[-1].node);
	    yyval.node->setBranch (3, yyvsp[0].node);
	  }
    break;

  case 278:
#line 1994 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_attr_raises, yyvsp[0].node);
	  }
    break;

  case 279:
#line 1998 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_attr_raises, 0, yyvsp[0].node);
	  }
    break;

  case 280:
#line 2002 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_attr_raises, yyvsp[-1].node, yyvsp[0].node);
	  }
    break;

  case 281:
#line 2006 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_attr_raises, yyvsp[0].node, yyvsp[-1].node);
	  }
    break;

  case 282:
#line 2014 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 283:
#line 2022 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 284:
#line 2030 "yacc.yy"
    {
	    yyval.node = new ParseNode (raises_expr, yyvsp[-1].node);
	  }
    break;

  case 287:
#line 2044 "yacc.yy"
    {
	    yyval.node = new ParseNode (component_forward_dcl);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 288:
#line 2054 "yacc.yy"
    {
	    yyval.node = new ParseNode (component_dcl, yyvsp[-3].node, yyvsp[-1].node);
	  }
    break;

  case 289:
#line 2058 "yacc.yy"
    {
	    yyval.node = new ParseNode (component_dcl, yyvsp[-2].node);
	  }
    break;

  case 290:
#line 2066 "yacc.yy"
    {
	    yyval.node = new ParseNode (component_header);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 291:
#line 2071 "yacc.yy"
    {
	    yyval.node = new ParseNode (component_header, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-1].ident);
	  }
    break;

  case 292:
#line 2076 "yacc.yy"
    {
	    yyval.node = new ParseNode (component_header);
	    yyval.node->setIdent (yyvsp[-1].ident);
	    yyval.node->setBranch (2, yyvsp[0].node);
	  }
    break;

  case 293:
#line 2083 "yacc.yy"
    {
	    yyval.node = new ParseNode (component_header, yyvsp[-1].node, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-2].ident);
	  }
    break;

  case 294:
#line 2092 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 295:
#line 2100 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 296:
#line 2108 "yacc.yy"
    {
	    yyval.node = new ParseNode (exports, yyvsp[0].node);
	  }
    break;

  case 297:
#line 2112 "yacc.yy"
    {
	    yyval.node = new ParseNode (exports, yyvsp[-1].node, yyvsp[0].node);
	  }
    break;

  case 298:
#line 2120 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 299:
#line 2124 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 300:
#line 2128 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 301:
#line 2132 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 302:
#line 2136 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 303:
#line 2140 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 304:
#line 2148 "yacc.yy"
    {
	    yyval.node = new ParseNode (provides_dcl, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 306:
#line 2158 "yacc.yy"
    {
	    yyval.node = new ParseNode (t_object);
	  }
    break;

  case 307:
#line 2166 "yacc.yy"
    {
	    yyval.node = new ParseNode (uses_dcl, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 308:
#line 2171 "yacc.yy"
    {
	    yyval.node = new ParseNode (uses_multiple_dcl, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 309:
#line 2180 "yacc.yy"
    {
	    yyval.node = new ParseNode (emits_dcl, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 310:
#line 2189 "yacc.yy"
    {
	    yyval.node = new ParseNode (publishes_dcl, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 311:
#line 2198 "yacc.yy"
    {
	    yyval.node = new ParseNode (consumes_dcl, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 312:
#line 2207 "yacc.yy"
    {
	    yyval.node = new ParseNode (home_dcl, yyvsp[-1].node, yyvsp[0].node);
	  }
    break;

  case 313:
#line 2215 "yacc.yy"
    {
	    yyval.node = new ParseNode (home_header);
	    yyval.node->setBranch (2, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-2].ident);
	  }
    break;

  case 314:
#line 2221 "yacc.yy"
    {
	    yyval.node = new ParseNode (home_header, yyvsp[-2].node, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-3].ident);
	  }
    break;

  case 315:
#line 2226 "yacc.yy"
    {
	    yyval.node = new ParseNode (home_header);
	    yyval.node->setIdent (yyvsp[-3].ident);
	    yyval.node->setBranch (2, yyvsp[-1].node);
	    yyval.node->setBranch (3, yyvsp[0].node);
	  }
    break;

  case 316:
#line 2234 "yacc.yy"
    {
	    yyval.node = new ParseNode (home_header, yyvsp[-3].node, yyvsp[-1].node, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-4].ident);
	  }
    break;

  case 317:
#line 2239 "yacc.yy"
    {
	    yyval.node = new ParseNode (home_header);
	    yyval.node->setBranch (2, yyvsp[0].node);
	    yyval.node->setBranch (4, yyvsp[-2].node);
	    yyval.node->setIdent (yyvsp[-3].ident);
	  }
    break;

  case 318:
#line 2247 "yacc.yy"
    {
	    yyval.node = new ParseNode (home_header, yyvsp[-3].node, yyvsp[0].node);
	    yyval.node->setBranch (4, yyvsp[-2].node);
	    yyval.node->setIdent (yyvsp[-4].ident);
	  }
    break;

  case 319:
#line 2254 "yacc.yy"
    {
	    yyval.node = new ParseNode (home_header);
	    yyval.node->setIdent (yyvsp[-4].ident);
	    yyval.node->setBranch (2, yyvsp[-1].node);
	    yyval.node->setBranch (3, yyvsp[0].node);
	    yyval.node->setBranch (4, yyvsp[-3].node);
	  }
    break;

  case 320:
#line 2264 "yacc.yy"
    {
	    yyval.node = new ParseNode (home_header, yyvsp[-4].node, yyvsp[-1].node, yyvsp[0].node, yyvsp[-3].node);
	    yyval.node->setIdent (yyvsp[-5].ident);
	  }
    break;

  case 321:
#line 2273 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 322:
#line 2281 "yacc.yy"
    {
	    yyval.node = yyvsp[0].node;
	  }
    break;

  case 323:
#line 2289 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 324:
#line 2293 "yacc.yy"
    {
	    yyval.node = NULL;
	  }
    break;

  case 325:
#line 2301 "yacc.yy"
    {
	    yyval.node = new ParseNode (exports, yyvsp[0].node);
	  }
    break;

  case 326:
#line 2305 "yacc.yy"
    {
	    yyval.node = new ParseNode (exports, yyvsp[-1].node, yyvsp[0].node);
	  }
    break;

  case 328:
#line 2313 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 329:
#line 2317 "yacc.yy"
    {
	    yyval.node = yyvsp[-1].node;
	  }
    break;

  case 330:
#line 2327 "yacc.yy"
    {
	    yyval.node = new ParseNode (factory_dcl, yyvsp[-2].node, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-4].ident);
	  }
    break;

  case 331:
#line 2334 "yacc.yy"
    {
	    yyval.node = new ParseNode (factory_dcl);
	    yyval.node->setBranch (2, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-3].ident);
	  }
    break;

  case 332:
#line 2341 "yacc.yy"
    {
	    yyval.node = new ParseNode (factory_dcl, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[-3].ident);
	  }
    break;

  case 333:
#line 2347 "yacc.yy"
    {
	    yyval.node = new ParseNode (factory_dcl);
	    yyval.node->setIdent (yyvsp[-2].ident);
	  }
    break;

  case 334:
#line 2358 "yacc.yy"
    {
	    yyval.node = new ParseNode (finder_dcl, yyvsp[-2].node, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-4].ident);
	  }
    break;

  case 335:
#line 2365 "yacc.yy"
    {
	    yyval.node = new ParseNode (finder_dcl);
	    yyval.node->setBranch (2, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-3].ident);
	  }
    break;

  case 336:
#line 2372 "yacc.yy"
    {
	    yyval.node = new ParseNode (finder_dcl, yyvsp[-1].node);
	    yyval.node->setIdent (yyvsp[-3].ident);
	  }
    break;

  case 337:
#line 2378 "yacc.yy"
    {
	    yyval.node = new ParseNode (finder_dcl);
	    yyval.node->setIdent (yyvsp[-2].ident);
	  }
    break;

  case 341:
#line 2394 "yacc.yy"
    {
	    yyval.node = new ParseNode (event_forward_dcl);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 342:
#line 2399 "yacc.yy"
    {
	    yyval.node = new ParseNode (abstract_event_forward_dcl);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 343:
#line 2409 "yacc.yy"
    {
	    yyval.node = new ParseNode (abstract_event_dcl);
	    yyval.node->setBranch (1, new ParseNode (value_header));
	    yyval.node->getBranch (1)->setIdent (yyvsp[-3].ident);
	    yyval.node->setBranch (2, yyvsp[-1].node);
	  }
    break;

  case 344:
#line 2417 "yacc.yy"
    {
	    yyval.node = new ParseNode (abstract_event_dcl);
	    yyval.node->setBranch (1, new ParseNode (value_header));
	    yyval.node->getBranch (1)->setIdent (yyvsp[-4].ident);
	    yyval.node->getBranch (1)->setBranch (1, yyvsp[-3].node);
	    yyval.node->setBranch (2, yyvsp[-1].node);
	  }
    break;

  case 345:
#line 2430 "yacc.yy"
    {
	    yyval.node = new ParseNode (event_dcl, yyvsp[-3].node, yyvsp[-1].node);
	  }
    break;

  case 346:
#line 2434 "yacc.yy"
    {
	    yyval.node = new ParseNode (event_dcl, yyvsp[-2].node);
	  }
    break;

  case 347:
#line 2442 "yacc.yy"
    {
	    yyval.node = new ParseNode (value_header, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-1].ident);
	  }
    break;

  case 348:
#line 2447 "yacc.yy"
    {
	    yyval.node = new ParseNode (custom_value_header, yyvsp[0].node);
	    yyval.node->setIdent (yyvsp[-1].ident);
	  }
    break;

  case 349:
#line 2452 "yacc.yy"
    {
	    yyval.node = new ParseNode (value_header);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;

  case 350:
#line 2457 "yacc.yy"
    {
	    yyval.node = new ParseNode (custom_value_header);
	    yyval.node->setIdent (yyvsp[0].ident);
	  }
    break;


    }

/* Line 1016 of /home/karel/usr/local/bison-1.75/share/bison/yacc.c.  */
#line 4362 "yacc.c"

  yyvsp -= yylen;
  yyssp -= yylen;


#if YYDEBUG
  if (yydebug)
    {
      short *yyssp1 = yyss - 1;
      YYFPRINTF (stderr, "state stack now");
      while (yyssp1 != yyssp)
	YYFPRINTF (stderr, " %d", *++yyssp1);
      YYFPRINTF (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("parse error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "parse error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("parse error");
    }
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyssp > yyss)
	    {
	      YYDPRINTF ((stderr, "Error: popping "));
	      YYDSYMPRINT ((stderr,
			    yystos[*yyssp],
			    *yyvsp));
	      YYDPRINTF ((stderr, "\n"));
	      yydestruct (yystos[*yyssp], *yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDPRINTF ((stderr, "Discarding token %d (%s).\n",
		  yychar, yytname[yychar1]));
      yydestruct (yychar1, yylval);
      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */

  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      YYDPRINTF ((stderr, "Error: popping "));
      YYDSYMPRINT ((stderr,
		    yystos[*yyssp], *yyvsp));
      YYDPRINTF ((stderr, "\n"));

      yydestruct (yystos[yystate], *yyvsp);
      yyvsp--;
      yystate = *--yyssp;


#if YYDEBUG
      if (yydebug)
	{
	  short *yyssp1 = yyss - 1;
	  YYFPRINTF (stderr, "Error: state stack now");
	  while (yyssp1 != yyssp)
	    YYFPRINTF (stderr, " %d", *++yyssp1);
	  YYFPRINTF (stderr, "\n");
	}
#endif
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 2463 "yacc.yy"


