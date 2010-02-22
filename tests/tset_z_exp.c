/* Test file for mpfr_set_z_exp.

Copyright 1999, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010 Free Software Foundation, Inc.
Contributed by the Arenaire and Cacao projects, INRIA.

This file is part of the GNU MPFR Library.

The GNU MPFR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MPFR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MPFR Library; see the file COPYING.LESSER.  If not, see
http://www.gnu.org/licenses/ or write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA. */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "mpfr-test.h"

static mp_exp_t
randexp (void)
{
  return (mp_exp_t) (randlimb () % (__gmpfr_emax - __gmpfr_emin))
    + __gmpfr_emin;
}

static void
check0 (void)
{
  mpz_t y;
  mpfr_t x;
  int inexact, r;
  mp_exp_t e;

  /* Check for +0 */
  mpfr_init (x);
  mpz_init (y);
  mpz_set_si (y, 0);
  for(r = 0; r < MPFR_RND_MAX; r++)
    {
      e = randexp ();
      inexact = mpfr_set_z_exp (x, y, e, (mpfr_rnd_t) r);
      if (!MPFR_IS_ZERO(x) || !MPFR_IS_POS(x) || inexact)
        {
          printf ("mpfr_set_z_exp(x,0,e) failed for e=%ld, rnd=%s\n", e,
		  mpfr_print_rnd_mode ((mpfr_rnd_t) r));
          exit (1);
        }
    }
  mpfr_clear(x);
  mpz_clear(y);
}

/* FIXME: It'd be better to examine the actual data in an mpfr_t to see that
   it's as expected.  Comparing mpfr_set_z with mpfr_cmp or against
   mpfr_get_si is a rather indirect test of a low level routine.  */

static void
check (long i, mpfr_rnd_t rnd)
{
  mpfr_t f;
  mpz_t z;
  mp_exp_t e;

  mpfr_init2 (f, 8 * sizeof(long));
  mpz_init (z);
  mpz_set_ui (z, i);
  e = randexp ();
  mpfr_set_z_exp (f, z, e, rnd);
  mpfr_div_2si (f, f, e, rnd);
  if (mpfr_get_si (f, MPFR_RNDZ) != i)
    {
      printf ("Error in mpfr_set_z_exp for i=%ld e=%ld rnd_mode=%d\n",
	      i, e, rnd);
      printf ("expected %ld\n", i);
      mpfr_printf ("got %Re\n", f);
      exit (1);
    }
  mpfr_clear (f);
  mpz_clear (z);
}

int
main (int argc, char *argv[])
{
  long j;

  tests_start_mpfr ();

  check (0, MPFR_RNDN);
  for (j = 0; j < 200000; j++)
    check (randlimb () & LONG_MAX, RND_RAND ());
  check0 ();

  tests_end_mpfr ();

  return 0;
}