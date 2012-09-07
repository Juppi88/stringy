/**********************************************************************
 *
 * PROJECT:		Stringy
 * FILE:		Stringy.h
 * LICENCE:		See Licence.txt
 * PURPOSE:		Re-written standard operations on C strings.
 *				Originally written for Mylly Game Engine.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#pragma once
#ifndef __STRINGY_H
#define __STRINGY_H

#include "Defines.h"
#include <stdio.h>

#ifdef MYLLY_UNICODE
	#define mstrequal	wstr_equal
	#define mstrcpy		wstr_cpy
	#define mstrcat		wstr_cat
	#define mstrins		wstr_ins
	#define mstrlwr		wstr_lwr
	#define mstrtok		wstr_tok
	#define mstrdup		wstr_dup
	#define mstrsize	wstr_size
	#define mstrlen		wcslen
	#define msnprintf	vswprintf
#else
	#define mstrequal	str_equal
	#define mstrcpy		str_cpy
	#define mstrcat		str_cat
	#define mstrins		str_ins
	#define mstrlwr		str_lwr
	#define mstrtok		str_tok
	#define mstrdup		str_dup
	#define mstrsize	str_size
	#define mstrlen		strlen
	#define msnprintf	vsnprintf
#endif /* MYLLY_UNICODE */

// Size of a character array (cmp. sizeof for a char array)
#define lengthof(x) ( sizeof(x) / sizeof(x[0]) )

__BEGIN_DECLS

//
// ASCII string operations
//
MYLLY_API bool					str_equal						( const char* str1, const char* str2 );
MYLLY_API void					str_cpy							( char* dest, const char* source, size_t size );
MYLLY_API void					str_cat							( char* dest, const char* source, size_t size );
MYLLY_API void					str_ins							( char* dest, const char* source, size_t size, size_t pos );
MYLLY_API void					str_lwr							( char* str );
MYLLY_API char*					str_tok							( char* str, const char delimiter );
MYLLY_API char*					str_dup							( const char* str, size_t len );
MYLLY_API size_t				str_size						( const char* str );

//
// Unicode string operations
//
MYLLY_API bool					wstr_equal						( const wchar_t* str1, const wchar_t* str2 );
MYLLY_API void					wstr_cpy						( wchar_t* dest, const wchar_t* source, size_t size );
MYLLY_API void					wstr_cat						( wchar_t* dest, const wchar_t* source, size_t size );
MYLLY_API void					wstr_ins						( wchar_t* dest, const wchar_t* source, size_t size, size_t pos );
MYLLY_API void					wstr_lwr						( wchar_t* str );
MYLLY_API wchar_t*				wstr_tok						( wchar_t* str, const wchar_t delimiter );
MYLLY_API wchar_t*				wstr_dup						( const wchar_t* str, size_t len );
MYLLY_API size_t				wstr_size						( const wchar_t* str );

__END_DECLS

#endif /* __STRINGY_H */
