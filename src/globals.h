/* globals.h
 *
 * Copyright (C) 2011-2022 Guillaume Chapron.
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

#ifndef GLOBALS_H
#define GLOBALS_H

#include "pop.h"

extern int R_mc_seed;
extern long R_number_of_years;
extern long R_number_mc_runs;
extern long number_mc_indiv_ever;
extern long number_mc_indiv_eversize;

extern int R_number_classes;
extern double R_sex_ratio;

extern int *R_initial_population;

extern double *R_survival_md;
extern double *R_survival_sd;
extern double *R_litter_size_md;
extern double *R_litter_size_sd;

extern struct statistics *stats;

#endif
