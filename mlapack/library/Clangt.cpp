/*
 * Copyright (c) 2008-2010
 *      Nakata, Maho
 *      All rights reserved.
 *
 *  $Id: Clangt.cpp,v 1.5 2010/08/07 04:48:32 nakatamaho Exp $ 
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
/*
Copyright (c) 1992-2007 The University of Tennessee.  All rights reserved.

$COPYRIGHT$

Additional copyrights may follow

$HEADER$

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

- Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer. 
  
- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer listed
  in this license in the documentation and/or other materials
  provided with the distribution.
  
- Neither the name of the copyright holders nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.
  
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT  
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT  
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
*/

#include <mblas.h>
#include <mlapack.h>

REAL Clangt(const char *norm, INTEGER n, COMPLEX * dl, COMPLEX * d, COMPLEX * du)
{
    INTEGER i;
    REAL sum, scale;
    REAL anorm = 0;
    REAL mtemp1, mtemp2;
    REAL One = 1.0, Zero = 0.0;

    if (n <= 0) {
	anorm = Zero;
    } else if (Mlsame(norm, "M")) {
//Find max(abs(A(i,j))).
	anorm = abs(d[n]);
	for (i = 0; i < n - 1; i++) {
	    mtemp1 = anorm, mtemp2 = abs(dl[i]);
	    anorm = max(mtemp1, mtemp2);
	    mtemp1 = anorm, mtemp2 = abs(d[i]);
	    anorm = max(mtemp1, mtemp2);
	    mtemp1 = anorm, mtemp2 = abs(du[i]);
	    anorm = max(mtemp1, mtemp2);
	}
    } else if (Mlsame(norm, "O") || Mlsame(norm, "1")) {
//Find norm1(A).
	if (n == 1) {
	    anorm = abs(d[0]);
	} else {
	    mtemp1 = abs(d[0]) + abs(dl[1]), mtemp2 = abs(d[n]) + abs(du[n - 1]);
	    anorm = max(mtemp1, mtemp2);
	    for (i = 1; i < n - 1; i++) {
		mtemp1 = anorm, mtemp2 = abs(d[i]) + abs(dl[i]) + abs(du[i - 1]);
		anorm = max(mtemp1, mtemp2);
	    }
	}
    } else if (Mlsame(norm, "I")) {
//Find normI(A).
	if (n == 1) {
	    anorm = abs(d[0]);
	} else {
	    mtemp1 = abs(d[0]) + abs(du[1]), mtemp2 = abs(d[n]) + abs(dl[n - 1]);
	    anorm = max(mtemp1, mtemp2);
	    for (i = 1; i < n - 1; i++) {
		mtemp1 = anorm, mtemp2 = abs(d[i]) + abs(du[i]) + abs(dl[i - 1]);
		anorm = max(mtemp1, mtemp2);
	    }
	}
    } else if (Mlsame(norm, "F") || Mlsame(norm, "E")) {
//Find normF(A).
	scale = Zero;
	sum = One;
	Classq(n, &d[0], 1, &scale, &sum);
	if (n > 1) {
	    Classq(n - 1, &dl[1], 1, &scale, &sum);
	    Classq(n - 1, &du[1], 1, &scale, &sum);
	}
	anorm = scale * sqrt(sum);
    }
    return anorm;
}
