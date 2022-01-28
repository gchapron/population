/* main.c
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
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
#include <Rmath.h>

#include "mc.h"
#include "pop.h"
#include "tools.h"

SEXP C_montecarlo (
			SEXP SEXP_seed,
			SEXP SEXP_years,
			SEXP SEXP_runs,
			SEXP SEXP_initial_population,
			SEXP SEXP_survival,
			SEXP SEXP_litter_size
			) {
	
	int nprot = 0;
	
	R_mc_seed = (INTEGER(SEXP_seed)[0] == 1)?1:0;
	R_number_of_years = INTEGER(SEXP_years)[0];
	R_number_mc_runs = INTEGER(SEXP_runs)[0];
	R_number_classes = LENGTH(SEXP_initial_population);
	
	R_initial_population = malloc(R_number_classes * sizeof(int));
	for (int i = 0; i < R_number_classes; i++) {
		R_initial_population[i] = INTEGER(SEXP_initial_population)[i];
	}
	
	R_survival_md = malloc(R_number_classes * sizeof(double));
	R_survival_sd = malloc(R_number_classes * sizeof(double));
	for (int i = 0; i < R_number_classes; i++) {
		R_survival_md[i] = REAL(SEXP_survival)[i];
		R_survival_sd[i] = REAL(SEXP_survival)[R_number_classes + i];
	}
	
	R_litter_size_md = malloc(R_number_classes * sizeof(double));
	R_litter_size_sd = malloc(R_number_classes * sizeof(double));
	for (int i = 0; i < R_number_classes; i++) {
		R_litter_size_md[i] = REAL(SEXP_litter_size)[i];
		R_litter_size_sd[i] = REAL(SEXP_litter_size)[R_number_classes + i];
	}
	
	stats = malloc(sizeof(struct statistics));
	mc_allocate_statistics(stats);
	monte_carlo(stats);
	
	SEXP R_runs;
	PROTECT(R_runs = allocVector(REALSXP, R_number_of_years * R_number_mc_runs * R_number_classes)); nprot++;
	for (long i = 0; i < R_number_mc_runs; i++) {
		for (long j = 0; j < R_number_of_years; j++) {
			for (long k = 0; k < R_number_classes; k++) {
				REAL(R_runs)[ i * R_number_of_years * R_number_classes + j * R_number_classes + k] = (double)stats->runs[i][j][k];
			}
		}
	}
	
	char *names[1] = {"runs"};
	SEXP list, list_names;

	PROTECT(list_names = allocVector(STRSXP,1)); nprot++;
	for (int i = 0; i < 1; i++) {
		SET_STRING_ELT(list_names, i , mkChar(names[i]));
	}
		
	PROTECT(list = allocVector(VECSXP, 1)); nprot++;
	SET_VECTOR_ELT(list, 0, R_runs);
	setAttrib(list, R_NamesSymbol, list_names);
	
	UNPROTECT(nprot);
	
	mc_free_results(stats);
	
	return(list);
	
}
