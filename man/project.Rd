\name{project}
\alias{project}
\alias{C_montecarlo}
%- Also NEED an '\alias' for EACH other topic documented here.
\title{
Population projections
}
\description{
Run stochastic population projections.
}
\usage{
project(years, runs, initial_population, survival, litter, seed)
}
%- maybe also 'usage' for other objects documented here.
\arguments{
  \item{years}{
Number of years to project the population.
}
  \item{runs}{
Number of times (or Monte Carlo runs) to project the population.
}
  \item{initial_population}{
  Vector of initial number of individuals for each class. This vector must contain only positive integers.
}
  \item{survival}{
Matrix of survival for each class, with nrow = number of classes and ncol = 2. The first column is the median value of the survival of each class. The second column is the standard deviation of the survival of each class.
}
  \item{litter}{
Matrix of litter size for each class, with nrow = number of classes and ncol = 2. The first column is the median value of the litter size of each class. The second column is the standard deviation of the litter size of each class.
}
  \item{seed}{
(optional) seed for the R random number generator. If missing, the seed is set to 1.
}
}
\details{
Run stochastic population projections with an Individual-Based Model (IBM) compiled in C.
}
\value{
%%  ~Describe the value returned
%%  If it is a LIST, use
  \item{runs}{a 3-dimensional array of numbers of individuals with dimension c(years, classes, runs)}
%% ...
}

\examples{
years <- 10
runs <- 100

init.pop <- c(30, 20, 15, 12, 10, 9, 8, 7, 6, 5)

surv.md <- c(0.5, 0.7, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9, 0.9)
surv.sd <- c(0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1)
surv.msd <- cbind(surv.md, surv.sd)

litter.md <- c(0.2, 1.1, 2.8, 2.8, 2.8, 2.8, 2.8, 2.8, 1.8, 0.2)
litter.sd <- c(0.1, 0.2, 0.15, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1)
litter.msd <- cbind(litter.md, litter.sd)

nclass <- 4 # vary number of classes

projection <- project(
	years = years,
	runs = runs,
	initial_population = init.pop[1:nclass],
	survival = surv.msd[1:nclass,],
	litter = litter.msd[1:nclass,]
	)
}

