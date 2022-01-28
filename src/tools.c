/* tools.c
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

#include <Rmath.h>

#include "tools.h"

/*******************************************************************************
 Shape for beta distribution
 *******************************************************************************/

double beta_shape(double mu, double sigma) {
	
	return( fmax2(0, ( pow(mu,2) - pow(mu,3) - mu*pow(sigma, 2) ) / pow(sigma,2)) );
	
}

/*******************************************************************************
 Rate for beta distribution
 *******************************************************************************/

double beta_rate(double mu, double sigma) {
	
	return( fmax2(0, ( mu - 2*pow(mu,2) + pow(mu,3) - pow(sigma,2) + mu*pow(sigma, 2) ) / pow(sigma,2)) );
	
}

/*******************************************************************************
 Shape for gamma distribution
 *******************************************************************************/

double gamma_shape(double mu, double sigma) {
	
	double x = 0;
	
	if (sigma > 0) {
		x = pow(mu,2) / pow(sigma,2);
	}
	
	return(x);
	
}

/*******************************************************************************
 Rate for gamma distribution
 *******************************************************************************/

double gamma_rate(double mu, double sigma) {
	
	double x = 0;
	
	if (sigma > 0) {
		x = mu / pow(sigma,2);
	}
	
	return(x);
	
}

