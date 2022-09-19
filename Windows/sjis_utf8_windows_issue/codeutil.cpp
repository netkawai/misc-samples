/*
zlib License

Copyright (C) 2018-2020 Sakura Editor Organization

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Sakura Editor Organization
https://sakura-editor.github.io/
*/
#include "stdafx.h"
#include "codeutil.h"
#include "charset/charcode.h"
#include <mbstring.h>
#include "charset/codechecker.h"


/*
	Discrimination table Is it a special conversion of the WinAPI function WideCharToMultiByte 
	(conversion that cannot be mutually converted)?
	Subscript domain: 0x00 to 0x5f (=0x00ff - 0x00a0)
*/
static const bool bNA = false;
const bool TABLE_WctombSpec[] = {
	bNA,  true, true, true, bNA,  true, true, bNA,   // 00a0 - 00a7
	bNA,  true, true, true, true, true, true, true,  // 00a8 - 00af
	bNA,  bNA,  true, true, bNA,  true, bNA,  true,  // 00b0 - 00b7
	true, true, true, true, bNA,  bNA,  bNA,  bNA,   // 00b8 - 00bf
	true, true, true, true, true, true, true, true,  // 00c0 - 00c7
	true, true, true, true, true, true, true, true,  // 00c8 - 00cf
	true, true, true, true, true, true, true, bNA,   // 00d0 - 00d7
	true, true, true, true, true, true, true, true,  // 00d8 - 00df
	true, true, true, true, true, true, true, true,  // 00e0 - 00e7
	true, true, true, true, true, true, true, true,  // 00e8 - 00ef
	true, true, true, true, true, true, true, bNA,   // 00f0 - 00f7
	true, true, true, true, true, true, true, true,  // 00f8 - 00ff
};

// The below table keep the information about sjis-duplicated points.
static const unsigned short TABLE_SjisPoorcodeDef[] = {
	0x81be, // 
	0x81bf, // 
	0x81ca, // 
	0x81da, // 
	0x81db, // 
	0x81df, // 
	0x81e0, // 
	0x81e3, // 
	0x81e6, // 
	0x81e7, // 
	0x8754, // 
	0x8755, // 
	0x8756, // 
	0x8757, // 
	0x8758, // 
	0x8759, // 
	0x875a, // 
	0x875b, // 
	0x875c, // 
	0x875d, // 
	0x8782, // 
	0x8784, // 
	0x878a, // 
};

static const int TABLESIZE_SjisPoorcodeIndex = 23;


unsigned int __fastcall SjisFilter_basis( const unsigned int uCode )
{
	int ni, nleft, nright;
	unsigned int code_tmp;

	/* binary search starts */
	nleft = 0;
	nright = TABLESIZE_SjisPoorcodeIndex -1;
	while( nleft <= nright ){
		ni = (nright + nleft) / 2;
		code_tmp = TABLE_SjisPoorcodeIndex[ni][0];
		if( uCode < code_tmp ){
			nright = ni - 1;
		}else if( code_tmp < uCode ){
			nleft = ni + 1;
		}else{
			break; // found
		}
	}

	if( nleft <= nright ){
		return TABLE_SjisPoorcodeDef[ TABLE_SjisPoorcodeIndex[ni][1] ];
	}
	return uCode;
}

unsigned int __fastcall SjisFilter_ibm2nec( const unsigned int uCode )
{
	uchar_t c1, c2;
	unsigned int code = (unsigned int) uCode;

	/*
		expected input value
		fa40 - fa7e, fa80 - fafc
		fb40 - fb7e, fb80 - fbfc
		fc40 - fc4b
	*/
	c1 = static_cast<unsigned char>( (code >> 8) & 0x000000ff );
	c2 = static_cast<unsigned char>( code & 0x000000ff );
	if( c1 == 0xfa || c1 == 0xfb || (c1 == 0xfc && c2 <= 0x4b) ){
		if     ( code <= 0xfa49 ) { code -= 0x0b51; }	/* fa40-fa49 -> eeef-eef8*/
		else if( code <= 0xfa54 ) { ; }
		else if( code <= 0xfa57 ) { code -= 0x0b5b; }	/* fa55-fa57 -> eefa-eefc*/
		else if( code <= 0xfa5b ) { ; }
		else if( code <= 0xfa7e ) { code -= 0x0d1c; }	/* fa5c-fa7e -> ed40-ed62*/
		else if( code <= 0xfa9b ) { code -= 0x0d1d; }	/* fa80-fa9b -> ed63-ed7e*/
		else if( code <= 0xfafc ) { code -= 0x0d1c; }	/* fa9c-fafc -> ed80-ede0*/
		else if( code <= 0xfb5b ) { code -= 0x0d5f; }	/* fb40-fb5b -> ede1-edfc*/
		else if( code <= 0xfb7e ) { code -= 0x0d1c; }	/* fb5c-fb7e -> ee40-ee62*/
		else if( code <= 0xfb9b ) { code -= 0x0d1d; }	/* fb80-fb9b -> ee63-ee7e*/
		else if( code <= 0xfbfc ) { code -= 0x0d1c; }	/* fb9c-fbfc -> ee80-eee0*/
		else                      { code -= 0x0d5f; }	/* fc40-fc4b -> eee1-eeec*/
	}

	return code;
}

/*!
	SJIS Filter assign NEC selected IBM unique character to ibm code point.
*/
unsigned int __fastcall SjisFilter_nec2ibm( const unsigned int uCode )
{
	uchar_t c1, c2;
	unsigned int code = (unsigned int) uCode;

	/*
		expected code point
		ed40 - ed7e, ed80 - edfc
		ee40 - ee7e, ee80 - eefc
	*/
	c1 = static_cast<unsigned char>( (code >> 8) & 0x000000ff );
	c2 = static_cast<unsigned char>( code & 0x000000ff );
	if( c1 == 0xed || c1 == 0xee ){
		if     ( code <= 0xed62 ) { code += 0x0d1c; }	/* ed40-ed62 -> fa5c-fa7e */
		else if( code <= 0xed7e ) { code += 0x0d1d; }	/* ed63-ed7e -> fa80-fa9b */
		else if( code <= 0xede0 ) { code += 0x0d1c; }	/* ed80-ede0 -> fa9c-fafc */
		else if( code <= 0xedfc ) { code += 0x0d5f; }	/* ede1-edfc -> fb40-fb5b */
		else if( code <= 0xee62 ) { code += 0x0d1c; }	/* ee40-ee62 -> fb5c-fb7e */
		else if( code <= 0xee7e ) { code += 0x0d1d; }	/* ee63-ee7e -> fb80-fb9b */
		else if( code <= 0xeee0 ) { code += 0x0d1c; }	/* ee80-eee0 -> fb9c-fbf0 */
		else if( code <= 0xeeec ) { code += 0x0d5f; }	/* eee1-eeec -> fc40-fc4b */
		else if( code <= 0xeeee ) { ; }
		else if( code <= 0xeef8 ) { code += 0x0b51; }	/* eeef-eef8 -> fa40-fa49 */
		else if( code == 0xeef9 ) { ; }
		else                      { code += 0x0b5b; }	/* eefa-eefc -> fa55-fa57 */
	}

	return code;
}


int MultiByte2WideCharacter(void)
{
	printf("\n\n==============================================\n"
           "simple sjis -> unicode conversion\n");

  // set up the converter
  conv = ucnv_open("shift_jis", &status);
  assert(U_SUCCESS(status));

  // convert to Unicode
  // Note: we can use strlen, we know it's an 8 bit null terminated codepage
  target[6] = 0xFDCA;
  len = ucnv_toUChars(conv, target, 100, source, strlen(source), &status);
  U_ASSERT(status);
  // close the converter
  ucnv_close(conv);
  return 0;
}

int WideCharacter2MultiByte
{
	printf("\n\n==============================================\n"
           "simple unicode conversion -> sjis \n");

	// set up the converter
	conv = ucnv_open("shift_jis", &status);
	assert(U_SUCCESS(status));
	
	// convert from Unicode
	target 
	len = ucnv_fromUChars(conv, target, 100, source, strlen(source), &status)l
	U_ASSERT(status);
	// close the converter
	ucnv_close(conv);
	return 0;
}