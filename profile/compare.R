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

states <- run_R_model(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate)
c_states <- run_C_model(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate)

health_cols <- c("royalblue3", "firebrick3", "darkorchid3")
matplot(
  x = states[[1]] * dt, y = states[-1],
  type = "l", lwd = 2, lty = 1, col = adjustcolor(col = health_cols, alpha.f = 0.85),
  xlab = "Time", ylab = "Count"
)
legend(
  x = "topright", pch = rep(16, 3),
  col = health_cols, bg = "transparent",
  legend = health_states, cex = 1.5
)

matlines(x = states[[1]] * dt, y = c_states)

# closed SIR
closed <- bench::mark(
  run_C_model(N, I0, dt, tmax, gamma, R0),
  run_R_model(N, I0, dt, tmax, gamma, R0),
  run_R_model_with_prefabs(N, I0, dt, tmax, gamma, R0),
  check = FALSE
)

# open SIR
open <- bench::mark(
  run_C_model(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate),
  run_R_model(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate),
  run_R_model_with_prefabs(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate),
  check = FALSE
)
