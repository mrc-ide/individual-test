#' @export
run_C_model <- function(N, I0, dt, tmax, gamma, R0, birth_rate = NULL, death_rate = NULL) {
  if (!is.null(birth_rate)){
    res <- run_model_open(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate)
  } else {
    res <- run_model_closed(N, I0, dt, tmax, gamma, R0)
  }
  res <- matrix(res, nrow = tmax/dt, byrow = TRUE)
  colnames(res) <- health_states
  res
}
