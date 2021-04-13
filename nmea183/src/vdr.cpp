/*
Author: Samuel R. Blackburn
Internet: wfc@pobox.com

"You can get credit for something or get it done, but not both."
Dr. Richard Garwin

The MIT License (MIT)

Copyright (c) 1996-2019 Sam Blackburn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// SPDX-License-Identifier: MIT

#include "nmea0183.h"

VDR::VDR()
{
   Mnemonic = _T("VDR");
   Empty();
}

VDR::~VDR()
{
   Mnemonic.Empty();
   Empty();
}


void VDR::Empty( void )
{
   DegreesTrue     = 0.0;
   DegreesMagnetic = 0.0;
   Knots           = 0.0;
}

bool VDR::Parse( SENTENCE const& sentence )
{
   /*
   ** VDR - Set and Drift
   **
   **        1   2 3   4 5   6 7
   **        |   | |   | |   | |
   ** $--VDR,x.x,T,x.x,M,x.x,N*hh<CR><LF>
   **
   ** Field Number: 
   **  1) Degress True
   **  2) T = True
   **  3) Degrees Magnetic
   **  4) M = Magnetic
   **  5) Knots (speed of current)
   **  6) N = Knots
   **  7) Checksum
   */

   /*
   ** First we check the checksum...
   */

   if ( sentence.IsChecksumBad( 7 ) == NTrue )
   {
       SetErrorMessage(_T("Invalid Checksum"));
       return( false );
   } 

   DegreesTrue     = sentence.Double( 1 );
   DegreesMagnetic = sentence.Double( 3 );
   Knots           = sentence.Double( 5 );

   return( true );
}

bool VDR::Write( SENTENCE& sentence ) 
{
   /*
   ** Let the parent do its thing
   */
   
   RESPONSE::Write( sentence );

   sentence += DegreesTrue;
   sentence += _T("T");
   sentence += DegreesMagnetic;
   sentence += _T("M");
   sentence += Knots;
   sentence += _T("N");

   sentence.Finish();

   return( true );
}

VDR const& VDR::operator = ( VDR const& source )
{
   DegreesTrue     = source.DegreesTrue;
   DegreesMagnetic = source.DegreesMagnetic;
   Knots           = source.Knots;

   return( *this );
}
