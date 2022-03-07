/* pop.c
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>
#include <Rmath.h>

#include "pop.h"
#include "globals.h"

#define FEMALE 0
#define MALE 1

void create_individual(t_population *pop, int the_age);
void survival_of_individuals(t_population *pop);
void remove_individuals(t_population *pop);
void free_individual(t_individual *idv);
void reproduction_of_individuals(t_population *pop);

/*******************************************************************************
 Assign population parameters
 *******************************************************************************/

void set_constant_parameters(t_population *pop) {
	
	pop->initial_number_indiv = malloc(R_number_classes * sizeof(double));
	for (int i = 0; i < R_number_classes; i++) {
		pop->initial_number_indiv[i] = R_initial_population[i];
	}
	
}

/*******************************************************************************
 Assign stochastic population parameters
 *******************************************************************************/

void set_mc_stochastic_parameters(t_population *pop) {
	
	double mu;
	double sigma;
	
	for (int i = 0; i < R_number_classes; i++) {
		mu = R_survival_md[i];
		sigma = R_survival_sd[i];
		if (sigma == 0) {
			pop->survival[i] = mu;
		} else {
			pop->survival[i] = rbeta(beta_shape(mu, sigma), beta_rate(mu, sigma));
		}
	}
	
	for (int i = 0; i < R_number_classes; i++) {
		mu = R_litter_size_md[i];
		sigma = R_litter_size_sd[i];
		if ( (mu == 0) | (sigma == 0) ) {
			pop->litter_size[i] = mu;
		} else {
			pop->litter_size[i] = rgamma(gamma_shape(mu, sigma), 1/gamma_rate(mu, sigma));
		}
		
	}
	
}

/*******************************************************************************
 Create individual in population
 *******************************************************************************/

void create_individual(t_population *pop, int the_age) {
	
	t_individual *new_idv = malloc(sizeof(t_individual));
	
	pop->number_indiv++;
	pop->number_indiv_history++;
	
	new_idv->unique = pop->number_indiv_history;
	new_idv->alive = 1;
	new_idv->age = the_age;
		
	if (pop->number_indiv == 1) {
		
		new_idv->previous = NULL;
		new_idv->next = NULL;
		pop->all_indiv = new_idv;
		
	} else {
		
		new_idv->previous = NULL;
		new_idv->next = pop->all_indiv;
		new_idv->next->previous = new_idv;
		pop->all_indiv = new_idv;
		
	}
	
}

/*******************************************************************************
 Create random population
 *******************************************************************************/

void create_population(t_population *pop) {
	
	pop->all_indiv = NULL;
	pop->number_indiv = 0;
	pop->number_indiv_history = 0;
	
	pop->survival = malloc(R_number_classes * sizeof(double));
	pop->litter_size = malloc(R_number_classes * sizeof(double));
    
	for (int k = 0; k < R_number_classes; k++) {
		for (int i = 0; i < pop->initial_number_indiv[k]; i++) {
			create_individual(pop, k);
		}
	}
    
}

/*******************************************************************************
 Survival of individuals
 *******************************************************************************/

void survival_of_individuals(t_population *pop) {
    
	t_individual *current_idv = pop->all_indiv;
	   
	while (current_idv != NULL) {

		current_idv->alive = rbinom(1, pop->survival[current_idv->age]);
        
		current_idv = current_idv->next;
        
    }
	
}

/*******************************************************************************
 Removal of individuals
 *******************************************************************************/

void remove_individuals(t_population *pop) {
	
	t_individual *current_idv = pop->all_indiv;
	t_individual *next_idv = NULL;
	
	while (current_idv != NULL) {
        
		if (current_idv->alive == 0) {
							
			next_idv = current_idv->next;
						
			if ( (current_idv->previous == NULL) & (current_idv->next == NULL) ) {
				
				pop->all_indiv = NULL;
				
			} else if ( (current_idv->previous == NULL) & (current_idv->next != NULL) ) {
				
				current_idv->next->previous = NULL;
				pop->all_indiv = current_idv->next;
				
			} else if ( (current_idv->previous != NULL) & (current_idv->next == NULL) ) {
				
				current_idv->previous->next = NULL;
				
			} else {
				
				current_idv->next->previous = current_idv->previous;
				current_idv->previous->next = current_idv->next;
				
			}
			
			free_individual(current_idv);
			
			pop->number_indiv--;
			
			current_idv = next_idv;
			
		} else {
			
			current_idv = current_idv->next;
			
		}
		
	}
	
}

/*******************************************************************************
 Reproduction of individuals
 *******************************************************************************/

void reproduction_of_individuals(t_population *pop) {
	
	t_individual *current_idv = pop->all_indiv;
	
	int f;
	
	while (current_idv != NULL) {
		
		if (current_idv->alive != 0) {
			
			f = rpois(pop->litter_size[current_idv->age]);
			
			for (int l = 0; l < f; l++) {
				
				create_individual(pop, 0);
				
			}
			
			current_idv->age++;
			
			if (current_idv->age == R_number_classes) {
				current_idv->alive = 0;
			}
						
		}
		
		current_idv = current_idv->next;
		
	}
    
}

/*******************************************************************************
 Yearly life cycle
 *******************************************************************************/

void cycle_year(t_population *pop, long i, long j, struct statistics *stats) {
	
	survival_of_individuals(pop);
    
	reproduction_of_individuals(pop);
	
	remove_individuals(pop);
	
	do_statistics(pop, i, j, stats);
	
}

/*******************************************************************************
 Collect statistics
 *******************************************************************************/

void do_statistics(t_population *pop, long seed, long year, struct statistics *stats) {
    
    t_individual* current_idv;
    
	current_idv = pop->all_indiv;
	
	while (current_idv != NULL) {
		
		stats->runs[seed][year][current_idv->age]++;
		
		current_idv = current_idv->next;
		
	}
    
}

/*******************************************************************************
 Free an individual
 *******************************************************************************/

void free_individual(t_individual *idv) {
	
	free(idv);
	
}

/*******************************************************************************
 Free population
 *******************************************************************************/

void free_population(t_population *pop) {
		
	t_individual *next_idv;
	
	while (pop->all_indiv != NULL) {
		next_idv = pop->all_indiv->next;
		free_individual(pop->all_indiv);
		pop->all_indiv = next_idv;
	}
		
	free(pop->survival);
	free(pop->litter_size);
	
}
