/* mc.c
 *
 * Copyright (C) 2011, 2012, 2013, 2014, 2015 Guillaume Chapron.
 * gchapron@carnivoreconservation.org
 *
 * This file is part of 'population', a R package to run population simulations
 *
 * 'population' is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 'population' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 'population'. If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
#include <Rmath.h>

#include "mc.h"

/////////////////////////////////////////////////////////////////////////////////
// Allocate statistics
/////////////////////////////////////////////////////////////////////////////////

void mc_allocate_statistics(struct statistics *stats) {

	stats->runs = malloc(R_number_mc_runs * sizeof(double **));

	for (long i = 0; i < R_number_mc_runs; i++) {
		stats->runs[i] = malloc((R_number_of_years+1) * sizeof(double *));
		for (long j = 0; j <= R_number_of_years; j++) {
			stats->runs[i][j] = malloc(R_number_classes * sizeof(double));
			for (long k = 0; k < R_number_classes; k++) {
					stats->runs[i][j][k] = 0;
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////////
// Free results
/////////////////////////////////////////////////////////////////////////////////

void mc_free_results(struct statistics *stats) {

	for (long i = 0; i < R_number_mc_runs; i++) {
		for (long j = 0; j <= R_number_of_years; j++) {
			free(stats->runs[i][j]);
		}
		free(stats->runs[i]);
	}
	free(stats->runs);

	free(stats);

}

/////////////////////////////////////////////////////////////////////////////////
// MONTE CARLO
/////////////////////////////////////////////////////////////////////////////////

void monte_carlo(struct statistics *stats) {

	GetRNGstate();

	long steps = R_number_mc_runs/50;

    Rprintf("\n|");

	for (long i = 0; i < R_number_mc_runs; i++) {

		t_population *pop = malloc(sizeof(t_population));

		set_constant_parameters(pop);

		create_population(pop);

        do_statistics(pop, i, 0, stats);

		for (long j = 1; j <= R_number_of_years; j++) {

            set_mc_stochastic_parameters(pop);
			cycle_year(pop, i, j, stats);

			if (pop->number_indiv == 0) {
				break;
			}

		}

		if (steps > 0) {
			if (i % steps == 0) {
                    Rprintf("*");
			}
		}

		free_population(pop);
		free(pop);

	}

	if (steps > 0) {
            Rprintf("|");
	}

	PutRNGstate();

}
