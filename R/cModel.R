run_C_model <- function(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate) {
  res <- run_model(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate)
  res <- matrix(res, nrow = tmax/dt, byrow = TRUE)
  colnames(res) <- health_states
  res
}
