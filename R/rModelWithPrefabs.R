run_R_model_with_prefabs <- function(N, I0, dt, tmax, gamma, R0, birth_rate = NULL, death_rate = NULL) {
  steps <- tmax / dt
  beta <- R0 * gamma
  if (!is.null(birth_rate)) {
    birth_rate <- birth_rate / dt
    death_rate <- death_rate / dt
  }
  health_states_t0 <- rep("S", N)
  health_states_t0[sample.int(n = N, size = I0)] <- "I"
  health <- individual::CategoricalVariable$new(categories = health_states,
                                    initial_values = health_states_t0)

  recovery_event <- individual::TargetedEvent$new(population_size = N)
  health_render <- individual::Render$new(timesteps = steps)

  recovery_event$add_listener(function(t, target) {
    health$queue_update("R", target)
  })

  health_render_process <- individual::categorical_count_renderer_process(
    renderer = health_render,
    variable = health,
    categories = health_states
  )

  processes <- c(infection_process(health$.variable, beta, N, dt),
                    recovery_process(health$.variable, recovery_event$.event, gamma, dt),
                    health_render_process)

  if (!is.null(birth_rate)) {
    processes <- c(processes, birth_process(health$.variable, recovery_event$.event, birth_rate),
                         death_process(health$.variable, recovery_event$.event, death_rate))
  }

  individual::simulation_loop(
    variables = list(health),
    events = list(recovery_event),
    processes = as.list(processes),
    timesteps = steps
  )

  health_render$to_dataframe()
}
