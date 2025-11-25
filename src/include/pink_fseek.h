/*
Copyright ESIEE (2025)

kacper.pluta@esiee.fr

This software is an image processing library whose purpose is to be
used primarily for research and teaching.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software. You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef PINK_FSEEK_COMPAT_H
#define PINK_FSEEK_COMPAT_H

#include <stdio.h>

/* * Detect Windows vs Unix automatically using standard compiler macros
 * (_WIN32 is defined by MSVC, MinGW, etc.)
 */
#ifdef _WIN32
/* --- WINDOWS --- */
#include <io.h>
#include <stdlib.h>

/* Map to Windows specific 64-bit functions */
#define __pink__fseeko _fseeki64
#define __pink__ftello _ftelli64

#else
/* --- LINUX / UNIX / macOS --- */

/* Ensure fseeko is visible in strict standard mode */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <sys/types.h>
#include <unistd.h>

/* Map to standard POSIX functions */
#define __pink__fseeko fseeko
#define __pink__ftello ftello

#endif

#endif // PINK_FSEEK_COMPAT_H
