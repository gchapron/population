\encoding{UTF-8}

\name{population-package}
\alias{population-package}
\alias{population}
\docType{package}
\title{
Population models
}
\description{
A package to run population simulations using an Individual-Based Model compiled in C. The population model is a discrete, age-structured model and follows the formalizing of a post-breeding Leslie matrix model.
\cr \cr
Version 0.1 proposes functions to run and plot population projections and includes demographic and environmental stochasticities. There is also the option to parallelize simulations (except on Windows).
\cr \cr
Version 0.2 fixes a bug that generated wrong results at very large population sizes.
}
\details{
\tabular{ll}{
Package: \tab population\cr
Type: \tab Package\cr
Version: \tab 0.2\cr
Date: \tab 2018-02-05\cr
License: \tab GPL-3\cr
}
}
\author{
Guillaume Chapron <gchapron@carnivoreconservation.org>\cr
}

\examples{
# Initial number of individuals
n0 <- 10
n1 <- 20
n2 <- 15
n3 <- 10
n4 <- 5

# Age-specific survival rates
s0 <- 0.5
s1 <- 0.6
s2 <- 0.7
s3 <- 0.8
s4 <- 0.9

# Age-specific number of offspring
b1 <- 0.5
b2 <- 0.8
b3 <- 1.8
b4 <- 1.8
b5 <- 1.1

# Project 10 years ahead repeated 10000 times
years <- 10
runs <- 10000

results <- project(
	years = years,
	runs = runs,
	initial_population = c(n0, n1, n2, n3, n4),
	survival = cbind(c(s0, s1, s2, s3, s4), 0.0), # no environmental stochasticity
	litter = cbind(c(b1, b2, b3, b4, b5), 0.0) # no environmental stochasticity
)

# Plot projection
plot_projection(results, "mean")

# Equivalent model with a post-breeding Leslie matrix
postM <- matrix(nrow=5, ncol=5, byrow=TRUE, data = c(
	s0*b1, s1*b2, s2*b3, s3*b4, s4*b5,
	   s0,     0,     0,     0,     0,
	    0,    s1,     0,     0,     0,
	    0,     0,    s2,     0,     0,
	    0,     0,     0,    s3,     0
	))

popvector <- c(n0, n1, n2, n3, n4)
N <- numeric(years)
N[1] <- sum(popvector)

for (i in 2:years) {
	popvector <- postM %*% popvector
	N[i] <- sum(popvector)
}

# Check we get similar results
lines(1:years, N, col="blue", lwd=2)
}
