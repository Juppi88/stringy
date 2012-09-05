/**********************************************************************
 *
 * PROJECT:		Stringy
 * FILE:		Stringy.c
 * LICENCE:		See Licence.txt
 * PURPOSE:		Re-written standard operations on C strings.
 *				Originally written for Mylly Game Engine.
 *
 *				(c) Tuomo Jauhiainen 2012
 *
 **********************************************************************/

#include "Stringy.h"
#include <malloc.h>
#include <string.h>
#include <assert.h>

#define GETWLENGTH(x) x /= sizeof(wchar_t) // Nasty hack for wide string lengths TODO: clean up later

/**
 * Tests whether the two given strings match.
 * @arg str1 - String 1
 * @arg str2 - String 2
 * @return true if the strings were equal, false otherwise
 */
bool str_equal( const char* str1, const char* str2 )
{
	while ( *str1 == *str2++ )
	{
		if ( !*str1++ ) return true;
	}

	return false;
}

bool wstr_equal( const wchar_t* str1, const wchar_t* str2 )
{
	while ( *str1 == *str2 )
	{
		if ( !*str1 ) return true;

		str1 += sizeof(wchar_t);
		str2 += sizeof(wchar_t);
	}

	return false;
}

/**
 * Copies a string into another buffer.
 * @arg dest - Destination buffer
 * @arg source - Source buffer
 * @arg size - Size of the destination buffer
 */
void str_cpy( char* dest, const char* source, size_t size )
{
	if ( source )
	{
		while ( ( --size ) && ( *source ) )
		{
			*dest++ = *source++;
		}
	}

	*dest = '\0';
}

void wstr_cpy( wchar_t* dest, const wchar_t* source, size_t size )
{
	GETWLENGTH(size);

	if ( source )
	{
		while ( ( --size ) && ( *source ) )
		{
			*dest = *source;

			dest	+= sizeof(wchar_t);
			source	+= sizeof(wchar_t);
		}
	}

	*dest = '\0';
}

/**
 * Appends a string to another.
 * @arg dest - Destination string
 * @arg source - Source string
 * @arg size - Size of the destination buffer
 */
void str_cat( char* dest, const char* source, size_t size )
{
	if ( source )
	{
		while ( ( --size ) && ( *dest ) )
		{
			*dest++;
		}

		while ( ( --size ) && ( *source ) )
		{
			*dest++ = *source++;
		}
	}

	*dest = '\0';
}

void wstr_cat( wchar_t* dest, const wchar_t* source, size_t size )
{
	GETWLENGTH(size);

	if ( source )
	{
		while ( ( --size ) && ( *dest ) )
		{
			*dest++;
			dest += sizeof(wchar_t);
		}

		while ( ( --size ) && ( *source ) )
		{
			*dest++ = *source++;

			dest += sizeof(wchar_t);
			source += sizeof(wchar_t);
		}
	}

	*dest = '\0';
}

/**
 * Insert a string into another.
 * @arg dest - Destination string
 * @arg source - Source string
 * @arg size - Size of the destination buffer
 * @arg pos - Position where to insert the new string
 */
void str_ins( char* dest, const char* source, size_t size, size_t pos )
{
	char *tmp, *temp;
	register char* s;

	if ( pos >= size ) return;

	temp = tmp = alloca( size );

	for ( s = dest; s < dest+pos && *s; )
		*tmp++ = *s++;

	while ( *source )
		*tmp++ = *source++;

	while ( *s )
		*tmp++ = *s++;

	*tmp = '\0';

	str_cpy( dest, temp, size );
}

void wstr_ins( wchar_t* dest, const wchar_t* source, size_t size, size_t pos )
{
	wchar_t *tmp, *temp;
	register wchar_t* s;

	pos *= sizeof(wchar_t);

	if ( pos >= size ) return;

	temp = tmp = alloca( size );

	for ( s = dest; s < dest+pos && *s; )
	{
		*tmp = *s;

		tmp += sizeof(wchar_t);
		s += sizeof(wchar_t);
	}

	while ( *source )
	{
		*tmp = *source;

		tmp += sizeof(wchar_t);
		source += sizeof(wchar_t);
	}

	while ( *s )
	{
		*tmp = *s;

		tmp += sizeof(wchar_t);
		s += sizeof(wchar_t);
	}

	*tmp = '\0';

	wstr_cpy( dest, temp, size );
}

/**
 * Converts a string into lower case characters.
 * @arg str - The string to be converted
 */
void str_lwr( char* str )
{
	register char* s = str;
	for ( ;; )
	{
		if ( !*s ) break;
		if ( *s >= 'A' && *s <= 'Z' ) { *s += 0x20; }
		++s;
	}
}

void wstr_lwr( wchar_t* str )
{
	register wchar_t* s = str;
	for ( ;; )
	{
		if ( !*s ) break;
		if ( *s >= 'A' && *s <= 'Z' ) { *s += 0x20; }
		s += sizeof(wchar_t);
	}
}

/**
 * Splits the given string into tokens separated by a given delimiter.
 * @arg str - The string to be split or NULL if it's the previous string
 * @arg delimiter - The delimiter to be used for splitting
 */
char* str_tok( char* str, const char delimiter )
{
	register char* s;
	char* beginning;
	static char* prev_token = NULL;
	
	s = str ? str : prev_token;
	beginning = s;

	if ( !s ) return NULL;

	for ( ; *s; s++ )
	{
		if ( *s == delimiter )
		{
			while ( *s == delimiter ) { *s++ = '\0'; }

			prev_token = s;
			return beginning;
		}
	}

	prev_token = NULL;
	return beginning;
}

wchar_t* wstr_tok( wchar_t* str, const wchar_t delimiter )
{
	register wchar_t* s;
	wchar_t* beginning;
	static wchar_t* prev_token = NULL;

	s = str ? str : prev_token;
	beginning = s;

	if ( !s ) return NULL;

	for ( ; *s; s += sizeof(wchar_t) )
	{
		if ( *s == delimiter )
		{
			while ( *s == delimiter ) { *s = '\0'; s += sizeof(wchar_t); }

			prev_token = s;
			return beginning;
		}
	}

	prev_token = NULL;
	return beginning;
}

/**
 * Creates a duplicate of the given string.
 * @arg str - The string to be duplicated
 * @arg len - Length of the string (0 if not known)
 * @return The duplicated string
 */
char* str_dup( const char* str, size_t len )
{
	char* ret;

	if ( len == 0 )
		len = strlen( str );

	len++;

	ret = (char*)malloc( len );

	assert( ret != NULL ); // Something went horribly wrong

	memcpy( ret, str, len );
	return ret;
}

wchar_t* wstr_dup( const wchar_t* str, size_t len )
{
	wchar_t* ret;

	if ( len == 0 )
		len = wcslen( str );

	len = sizeof(wchar_t) * (len+1);

	ret = (wchar_t*)malloc( len );

	assert( ret != NULL );

	memcpy( ret, str, len );
	return ret;
}

/**
 * Returns the size of the given string in bytes (including
 * the terminating 0-character).
 * @arg str - String to measure
 * @return The size of string in bytes
 */
size_t str_size( const char* str )
{
	return strlen( str ) + 1;
}

size_t wstr_size( const wchar_t* str )
{
	return ( wcslen( str ) + 1 ) * sizeof(wchar_t);
}
