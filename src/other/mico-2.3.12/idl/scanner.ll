/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2001 by The Mico Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *  For more information, visit the MICO Home Page at
 *  http://www.mico.org/
 */

%{

#include <CORBA.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "prepro.h"
#include "parsenode.h"
#include "yacc.h"
#include <mico/template_impl.h>


#if defined(_WIN32) && defined(_MSC_VER)
#define isatty _isatty
#endif


using namespace std;

static CORBA::ULongLong
ascii_to_longlong( unsigned long base, const char *s )
{
  CORBA::ULongLong ll = 0;
  while( *s != '\0' ) {
    char c = *s++;
    if( c >= 'a' )
      c -= 'a' - 'A';
    c -= '0';
    if( c > 9 )
      c -= 'A' - '0' - 10;
    ll = ll * base + c;
  }
  return ll;
}

static CORBA::LongDouble
ascii_to_longdouble (const char *s)
{
  CORBA::LongDouble d;
#ifdef HAVE_SCANF_LF
  sscanf ((char *)s, "%Lf", &d);
#else
  /*
   * this is only an approximation and will probably break fixed<>
   * parameter calculation on systems where
   * sizeof(double) < sizeof(long double). but fortunately all
   * systems where scanf("%Lf") is known to be broken (Linux/Alpha
   * and HPUX) have sizeof(double) == sizeof(long double).
   */
  d = strtod ((char *)s, NULL);
#endif
  return d;
}

static char
translate_char( const char * & s )
{
  char c = *s++;
  
  if( c != '\\' )
    return c;

  c = *s++;

  switch( c ) {
  case 'n':
    return '\n';
  case 't':
    return '\t';
  case 'v':
    return '\v';
  case 'b':
    return '\b';
  case 'r':
    return '\r';
  case 'f':
    return '\f';
  case 'a':
    return '\a';
  case '\\':
    return '\\';
  case '?':
    return '\?';
  case '\'':
    return '\'';
  case '"':
    return '"';
  case 'x':
  case 'X':
    if (*s >= '0' && *s <= '9') {
      c = *s++ - '0';
    }
    else if (*s >= 'a' && *s <= 'f') {
      c = *s++ - 'a' + 10;
    }
    else if (*s >= 'A' && *s <= 'F') {
      c = *s++ - 'A' + 10;
    }
    else {
      return c;
    }

    if (*s >= '0' && *s <= '9') {
      c = 16*c + *s++ - '0';
    }
    else if (*s >= 'a' && *s <= 'f') {
      c = 16*c + *s++ - 'a' + 10;
    }
    else if (*s >= 'A' && *s <= 'F') {
      c = 16*c + *s++ - 'A' + 10;
    }

    return c;

  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7':
    c = c - '0';

    if (*s >= '0' && *s <= '7') {
      c = 8*c + *s++ - '0';
    }

    if (*s >= '0' && *s <= '7') {
      c = 8*c + *s++ - '0';
    }

    return c;

  default:
    return c;
  }
}

static CORBA::WChar
translate_wchar( const char * & s )
{
  CORBA::WChar res;
  char c = *s++;
  
  if( c != '\\' )
    return (CORBA::WChar) c;

  c = *s++;

  switch( c ) {
  case 'n':
    return (CORBA::WChar) '\n';
  case 't':
    return (CORBA::WChar) '\t';
  case 'v':
    return (CORBA::WChar) '\v';
  case 'b':
    return (CORBA::WChar) '\b';
  case 'r':
    return (CORBA::WChar) '\r';
  case 'f':
    return (CORBA::WChar) '\f';
  case 'a':
    return (CORBA::WChar) '\a';
  case '\\':
    return (CORBA::WChar) '\\';
  case '?':
    return (CORBA::WChar) '\?';
  case '\'':
    return (CORBA::WChar) '\'';
  case '"':
    return (CORBA::WChar) '"';
  case 'x':
  case 'X':
    if (*s >= '0' && *s <= '9') {
      res = *s++ - '0';
    }
    else if (*s >= 'a' && *s <= 'f') {
      res = *s++ - 'a' + 10;
    }
    else if (*s >= 'A' && *s <= 'F') {
      res = *s++ - 'A' + 10;
    }
    else {
      return (CORBA::WChar) c;
    }

    if (*s >= '0' && *s <= '9') {
      res = 16*res + *s++ - '0';
    }
    else if (*s >= 'a' && *s <= 'f') {
      res = 16*res + *s++ - 'a' + 10;
    }
    else if (*s >= 'A' && *s <= 'F') {
      res = 16*res + *s++ - 'A' + 10;
    }

    return res;

  case '0': case '1': case '2': case '3': case '4':
  case '5': case '6': case '7':
    res = c - '0';

    if (*s >= '0' && *s <= '7') {
      res = 8*res + *s++ - '0';
    }

    if (*s >= '0' && *s <= '7') {
      res = 8*res + *s++ - '0';
    }

    return res;

  case 'u':
    if (*s >= '0' && *s <= '9') {
      res = *s++ - '0';
    }
    else if (*s >= 'a' && *s <= 'f') {
      res = *s++ - 'a' + 10;
    }
    else if (*s >= 'A' && *s <= 'F') {
      res = *s++ - 'A' + 10;
    }
    else {
      return (CORBA::WChar) c;
    }

    if (*s >= '0' && *s <= '9') {
      res = 16*res + *s++ - '0';
    }
    else if (*s >= 'a' && *s <= 'f') {
      res = 16*res + *s++ - 'a' + 10;
    }
    else if (*s >= 'A' && *s <= 'F') {
      res = 16*res + *s++ - 'A' + 10;
    }

    if (*s >= '0' && *s <= '9') {
      res = 16*res + *s++ - '0';
    }
    else if (*s >= 'a' && *s <= 'f') {
      res = 16*res + *s++ - 'a' + 10;
    }
    else if (*s >= 'A' && *s <= 'F') {
      res = 16*res + *s++ - 'A' + 10;
    }

    if (*s >= '0' && *s <= '9') {
      res = 16*res + *s++ - '0';
    }
    else if (*s >= 'a' && *s <= 'f') {
      res = 16*res + *s++ - 'a' + 10;
    }
    else if (*s >= 'A' && *s <= 'F') {
      res = 16*res + *s++ - 'A' + 10;
    }

    return res;

  default:
    return (CORBA::WChar) c;
  }
}

static char *
translate_string (const char * s)
{
  char *res, *tmp;
  CORBA::ULong i=0;

  res = tmp = CORBA::string_alloc (strlen (s));

  while (*s) {
    *tmp++ = translate_char (s);
    i++;
  }

  *tmp = '\0';
  return res;
}

static CORBA::WChar *
translate_wstring (const char * s)
{
  CORBA::WChar *res, *tmp;
  CORBA::ULong i=0;

  res = tmp = CORBA::wstring_alloc (strlen (s));

  while (*s) {
    *tmp++ = translate_wchar (s);
    i++;
  }

  *tmp = '\0';
  return res;
}

%}

%option noyywrap

/*--------------------------------------------------------------------------*/

Digits                  [0-9]+
Oct_Digit               [0-7]
Hex_Digit               [a-fA-F0-9]
Int_Literal		[1-9][0-9]*
Oct_Literal		0{Oct_Digit}*
Hex_Literal		(0x|0X){Hex_Digit}*
Esc_Sequence1           "\\"[ntvbrfa\\\?\'\"]
Esc_Sequence2           "\\"{Oct_Digit}{1,3}
Esc_Sequence3           "\\"(x|X){Hex_Digit}{1,2}
Esc_Sequence4           "\\"(u|U){Hex_Digit}{1,4}
Esc_Sequence            ({Esc_Sequence1}|{Esc_Sequence2}|{Esc_Sequence3})
Char                    ([^\n\t\"\'\\]|{Esc_Sequence})
WChar                   ({Char}|{Esc_Sequence4})
Char_Literal            "'"({Char}|\")"'"
WChar_Literal           "L'"({WChar}|\")"'"
String_Literal		\"({Char}|"'")*\"
WString_Literal		"L"\"({WChar}|"'")*\"
Float_Literal1		{Digits}"."{Digits}?(e|E)("+"|"-")?{Digits}  
Float_Literal2		{Digits}(e|E)("+"|"-")?{Digits}
Float_Literal3          {Digits}"."{Digits}
Float_Literal4          {Digits}"."
Float_Literal5		"."{Digits} 
Float_Literal6		"."{Digits}(e|E)("+"|"-")?{Digits}  
Fixed_Literal1          {Digits}(d|D)
Fixed_Literal2          {Digits}"."(d|D)
Fixed_Literal3          "."{Digits}(d|D)
Fixed_Literal4          {Digits}"."{Digits}(d|D)

/*--------------------------------------------------------------------------*/

CORBA_Identifier	[a-zA-Z_][a-zA-Z0-9_]*

/*--------------------------------------------------------------------------*/



%%

[ \t\r]			;
[\n]			idl_line_no++;
"//"[^\n]*		;
"#pragma"[^\n]*\n       {
			  yylval._str = CORBA::string_dup( yytext );
                          idl_line_no++;
                          return T_PRAGMA;
                        }
"#"[^\n]*\n             {
                          preprocessor_directive( yytext );
			  yylval._str = CORBA::string_dup( yytext );
                          return T_INCLUDE;
                        }
"{"			return T_LEFT_CURLY_BRACKET;
"}"			return T_RIGHT_CURLY_BRACKET;
"["		 	return T_LEFT_SQUARE_BRACKET;
"]"			return T_RIGHT_SQUARE_BRACKET;
"("			return T_LEFT_PARANTHESIS;
")"			return T_RIGHT_PARANTHESIS;
":"			return T_COLON;
","			return T_COMMA;
";"			return T_SEMICOLON;
"="			return T_EQUAL;
">>"			return T_SHIFTRIGHT;
"<<"			return T_SHIFTLEFT;
"+"			return T_PLUS_SIGN;
"-"			return T_MINUS_SIGN;
"*"			return T_ASTERISK;
"/"			return T_SOLIDUS;
"%"			return T_PERCENT_SIGN;
"~"			return T_TILDE;
"|"			return T_VERTICAL_LINE;
"^"			return T_CIRCUMFLEX;
"&"			return T_AMPERSAND;
"<"			return T_LESS_THAN_SIGN;
">"			return T_GREATER_THAN_SIGN;

import			return T_IMPORT;
const			return T_CONST;
typedef			return T_TYPEDEF;
float			return T_FLOAT;
double			return T_DOUBLE;
char			return T_CHAR;
wchar			return T_WCHAR;
fixed                   return T_FIXED;
boolean			return T_BOOLEAN;
string			return T_STRING;
wstring			return T_WSTRING;
void			return T_VOID;
unsigned		return T_UNSIGNED;
long 			return T_LONG;
short			return T_SHORT;
FALSE			return T_FALSE;
TRUE			return T_TRUE;
struct			return T_STRUCT;
union			return T_UNION;
switch			return T_SWITCH;
case			return T_CASE;
default			return T_DEFAULT;
enum			return T_ENUM;
in			return T_IN;
out			return T_OUT;
interface		return T_INTERFACE;
abstract		return T_ABSTRACT;
valuetype		return T_VALUETYPE;
truncatable		return T_TRUNCATABLE;
supports		return T_SUPPORTS;
custom			return T_CUSTOM;
public			return T_PUBLIC;
private			return T_PRIVATE;
factory			return T_FACTORY;
native			return T_NATIVE;
ValueBase		return T_VALUEBASE;
typeId			return T_TYPEID;
typePrefix		return T_TYPEPREFIX;
getRaises		return T_GETRAISES;
setRaises		return T_SETRAISES;
local			return T_LOCAL;

"::"			return T_SCOPE; 

module			return T_MODULE;
octet			return T_OCTET;
any			return T_ANY;
sequence		return T_SEQUENCE;
readonly		return T_READONLY;
attribute		return T_ATTRIBUTE;
exception		return T_EXCEPTION;
oneway			return T_ONEWAY;
inout			return T_INOUT;
raises			return T_RAISES;
context			return T_CONTEXT;

Object                  return T_OBJECT;

component		return T_COMPONENT;
provides		return T_PROVIDES;
uses			return T_USES;
multiple		return T_MULTIPLE;
emits			return T_EMITS;
publishes		return T_PUBLISHES;
consumes		return T_CONSUMES;
home			return T_HOME;
manages			return T_MANAGES;
primaryKey		return T_PRIMARYKEY;
finder			return T_FINDER;
eventtype		return T_EVENTTYPE;

{CORBA_Identifier}	{
                          // kcg: we will get rid of possible
                          // leading underscore
                          // in ParseNote::setIdent method
			  yylval.ident = CORBA::string_dup(yytext);
			  return T_IDENTIFIER;
			}
{Float_Literal1}	|
{Float_Literal2}	|
{Float_Literal3}	|
{Float_Literal4}	|
{Float_Literal5}	|
{Float_Literal6}	{
			  yylval._float = ascii_to_longdouble( yytext );
			  return T_FLOATING_PT_LITERAL;
			}
{Fixed_Literal1}	|
{Fixed_Literal2}	|
{Fixed_Literal3}	|
{Fixed_Literal4}	{
                          string s = yytext;
                          s = s.substr( 0, s.length() - 1 );
			  yylval._fixed = ascii_to_longdouble (s.c_str());
			  return T_FIXED_PT_LITERAL;
			}
{Int_Literal}		{ 
			  yylval._int = ascii_to_longlong( 10, yytext );
			  return T_INTEGER_LITERAL;
			}
{Oct_Literal}		{
			  yylval._int = ascii_to_longlong( 8, yytext );
			  return T_INTEGER_LITERAL;
			}
{Hex_Literal}		{
			  yylval._int = ascii_to_longlong( 16, yytext + 2 );
			  return T_INTEGER_LITERAL;
			}
{Char_Literal}		{
                          string s( yytext );
                          s = s.substr( 1, s.length() - 2 );
			  const char * tmp = s.c_str ();
			  yylval._char = translate_char( tmp );
			  return T_CHARACTER_LITERAL;
			}
{WChar_Literal}		{
                          string s( yytext );
                          s = s.substr( 2, s.length() - 3 );
			  const char * tmp = s.c_str ();
			  yylval._wchar = translate_wchar (tmp);
			  return T_WCHARACTER_LITERAL;
			}
{String_Literal}	{
                          string s( yytext );
                          s = s.substr( 1, s.length() - 2 );
			  yylval._str = translate_string (s.c_str());
			  return T_STRING_LITERAL;
			}
{WString_Literal}	{
                          string s( yytext );
                          s = s.substr( 2, s.length() - 3 );
			  yylval._wstr = translate_wstring (s.c_str());
			  return T_WSTRING_LITERAL;
			}
.                       {
                          return T_UNKNOWN;
                        }

%%

