/**********************************************************************
 *
 * PROJECT:		Stringy
 * FILE:		Stringy.c
 * LICENCE:		See Licence.txt
 * PURPOSE:		Re-written standard operations on C strings.
 *
 *				(c) Tuomo Jauhiainen 2012-13
 *
 **********************************************************************/

#include "Stringy.h"
#include "Platform/Alloc.h"
#include <string.h>
#include <assert.h>

#define GETWLENGTH(x) x /= sizeof(wchar_t) // Nasty hack for wide string lengths

static char*		last	= NULL;
static wchar_t*		wlast	= NULL;

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
	while ( *str1 == *str2++ )
	{
		if ( !*str1++ ) return true;
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
			*dest++ = *source++;
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
			dest++;
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
			dest++;
		}

		while ( ( --size ) && ( *source ) )
		{
			*dest++ = *source++;
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

	mem_stack_alloc( tmp, size );
	temp = tmp;

	for ( s = dest; s < dest+pos && *s; )
		*tmp++ = *s++;

	while ( *source )
		*tmp++ = *source++;

	while ( *s )
		*tmp++ = *s++;

	*tmp = '\0';

	str_cpy( dest, temp, size );
	mem_stack_free( temp );
}

void wstr_ins( wchar_t* dest, const wchar_t* source, size_t size, size_t pos )
{
	wchar_t *tmp, *temp;
	register wchar_t* s;

	pos = pos * sizeof(wchar_t);

	if ( pos >= size ) return;

	mem_stack_alloc( tmp, size );
	temp = tmp;

	for ( s = dest; (size_t)s < (size_t)dest + pos && *s; )
		*tmp++ = *s++;

	while ( *source )
		*tmp++ = *source++;

	while ( *s )
		*tmp++ = *s++;

	*tmp = '\0';

	wstr_cpy( dest, temp, size );
	mem_stack_free( temp );
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
		++s;
	}
}

/**
 * Splits the given string into tokens separated by a given delimiter.
 * @arg str - The string to be split or NULL if it's the previous string
 * @arg delim - The delimiter to be used for splitting
 * @return Pointer to the beginning of the token
 */
char* str_tok( char* str, char delim )
{
	char* begin;
	register char* s;

	s = str ? str : last;
	if ( !s || !*s ) return NULL;

	for ( ; *s == delim; s++ ) {}
	begin = s;

	for ( ; *s; s++ )
	{
		if ( *s != delim ) continue;

		*s++ = '\0';
		last = *s ? s : NULL;

		return *begin ? begin : NULL;
	}

	last = NULL;
	return *begin ? begin : NULL;
}

wchar_t* wstr_tok( wchar_t* str, wchar_t delim )
{
	wchar_t* begin;
	register wchar_t* s;

	s = str ? str : wlast;
	if ( !s || !*s ) return NULL;

	for ( ; *s == delim; s++ ) {}
	begin = s;

	for ( ; *s; s++ )
	{
		if ( *s != delim ) continue;

		*s++ = '\0';
		wlast = *s ? s : NULL;

		return *begin ? begin : NULL;
	}

	wlast = NULL;
	return *begin ? begin : NULL;
}

/**
 * Returns the rest of the string after the usage of str_tok. Cleans up
 * leading delimiters first.
 * @arg delim - The delimiter to be used for splitting
 */
char* str_tok_end( char delim )
{
	if ( !last || !*last ) return NULL;

	for ( ; *last == delim; last++ ) {}
	return last;
}

wchar_t* wstr_tok_end( wchar_t delim )
{
	if ( !wlast || !*wlast ) return NULL;

	for ( ; *wlast == delim; wlast++ ) {}
	return wlast;
}

/**
 * Returns the number of tokens separated by a given delimiter.
 * @arg str - The string to be split
 * @arg delim - The delimiter to be used for counting
 * @return The number of tokens found
 */
uint32 str_numtok( const char* str, char delim )
{
	register const char* s;
	uint32 tokens;

	if ( !str || !*str ) return 0;

	tokens = *str != delim ? 1 : 0;

	for ( s = str; *s; s++ )
	{
		if ( *s != delim ) continue;

		for ( ; *s == delim; s++ ) { /* Ignore multiple delims */ }
		if ( *s ) tokens++;
		else break;
	}

	return tokens;
}

uint32 wstr_numtok( const wchar_t* str, wchar_t delim )
{
	register const wchar_t* s;
	uint32 tokens;

	if ( !str || !*str ) return 0;

	tokens = *str != delim ? 1 : 0;

	for ( s = str; *s; s++ )
	{
		if ( *s != delim ) continue;

		for ( ; *s == delim; s++ ) { /* Ignore multiple delims */ }
		if ( *s ) tokens++;
		else break;
	}

	return tokens;
}

/**
 * Returns whether the given string is a number (integer) or not.
 * @arg text - The string to be checked
 * @return true if the string is an unsigned integer, false otherwise
 */
bool str_isnum( const char* text )
{
	register const char* s = text;

	if ( !*s ) return false;
	for ( ;; )
	{
		if ( !*s ) break; if ( *s < '0' || *s > '9' ) return false; ++s;
		if ( !*s ) break; if ( *s < '0' || *s > '9' ) return false; ++s;
		if ( !*s ) break; if ( *s < '0' || *s > '9' ) return false; ++s;
		if ( !*s ) break; if ( *s < '0' || *s > '9' ) return false; ++s;
	}

	return true;
}

bool wstr_isnum( const wchar_t* text )
{
	register const wchar_t* s = text;

	if ( !*s ) return false;
	for ( ;; )
	{
		if ( !*s ) break; if ( *s < '0' || *s > '9' ) return false; ++s;
		if ( !*s ) break; if ( *s < '0' || *s > '9' ) return false; ++s;
		if ( !*s ) break; if ( *s < '0' || *s > '9' ) return false; ++s;
		if ( !*s ) break; if ( *s < '0' || *s > '9' ) return false; ++s;
	}

	return true;
}

/**
 * Returns whether substr is a part of str. Ignores case.
 * @arg str - The main string
 * @arg substr - The looked substring
 * @return Pointer to the first occurance of substr within str, NULL if it was not found
 */
static __inline int __str_ccmp_case( char c1, char c2 )
{
	if ( c1 >= 'A' && c1 <= 'Z' ) { c1 += 0x20; }
	if ( c2 >= 'A' && c2 <= 'Z' ) { c2 += 0x20; }
	return ( c1 == c2 );
}

static __inline int __wstr_ccmp_case( wchar_t c1, wchar_t c2 )
{
	if ( c1 >= 'A' && c1 <= 'Z' ) { c1 += 0x20; }
	if ( c2 >= 'A' && c2 <= 'Z' ) { c2 += 0x20; }
	return ( c1 == c2 );
}

const char* str_isin( const char* str, const char* substr )
{
	register const char *s1, *s2;
	const char* match;

	for ( s1 = str, s2 = substr; *s1; s1++ )
	{
		if ( __str_ccmp_case( *s1, *s2 ) )
		{
			match = s1;

			while ( __str_ccmp_case( *s1++, *s2++ ) )
			{
				if ( !*s2 ) return match;
				if ( !*s1 ) return NULL;
			}

			s2 = substr;
		}
	}

	return NULL;
}

const wchar_t* wstr_isin( const wchar_t* str, const wchar_t* substr )
{
	register const wchar_t *s1, *s2;
	const wchar_t* match;

	for ( s1 = str, s2 = substr; *s1; s1++ )
	{
		if ( __wstr_ccmp_case( *s1, *s2 ) )
		{
			match = s1;

			while ( __wstr_ccmp_case( *s1++, *s2++ ) )
			{
				if ( !*s1 ) return NULL;
				if ( !*s2 ) return match;
			}

			s2 = substr;
		}
	}

	return NULL;
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

	str_cpy( ret, str, len );
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

	wstr_cpy( ret, str, len );
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
