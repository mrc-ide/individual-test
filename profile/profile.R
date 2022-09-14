N <- 1e3
I0 <- 5
S0 <- N - I0
dt <- 0.1
tmax <- 100

gamma <- 1 / 10
R0 <- 2.5
health_states <- c("S", "I", "R")

birth_rate <- 2
death_rate <- .001

invisible(individualtest::run_C_model(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate))
