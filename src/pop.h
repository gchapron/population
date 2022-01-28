/* pop.h
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

#ifndef POP_H
#define POP_H

#include "tools.h"

int R_number_classes;
double R_sex_ratio;

int *R_initial_population;
double *R_survival_md;
double *R_survival_sd;
double *R_litter_size_md;
double *R_litter_size_sd;

struct statistics {
	double ***runs;
};

struct statistics *stats;

typedef struct t_individual t_individual;
typedef struct t_population t_population;

struct t_individual {
	unsigned short unique;
	unsigned short alive;
	unsigned short sex;
	unsigned short age;
	t_individual *previous;
	t_individual *next;
};

struct t_population {
	t_individual *all_indiv;
	int number_indiv;
	int number_indiv_history;
	double *initial_number_indiv;
	double *survival;
	double *litter_size;
};

void set_constant_parameters(t_population *pop);
void set_mc_stochastic_parameters(t_population *pop);
void create_population(t_population *pop);

void cycle_year(t_population *pop, long i, long j, struct statistics *stats);
void do_statistics(t_population *pop, long seed, long year, struct statistics *stats);
void free_population(t_population *pop);

#endif
