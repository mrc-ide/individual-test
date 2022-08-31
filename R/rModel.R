run_R_model <- function(N, I0, dt, tmax, gamma, R0, birth_rate, death_rate) {
  steps <- tmax / dt
  beta <- R0 * gamma
  health_states_t0 <- rep("S", N)
  health_states_t0[sample.int(n = N, size = I0)] <- "I"
  health <- individual::CategoricalVariable$new(categories = health_states,
                                    initial_values = health_states_t0)

  recovery_event <- individual::TargetedEvent$new(population_size = N)
  health_render <- individual::Render$new(timesteps = steps)

  recovery_event$add_listener(function(t, target) {
    health$queue_update("R", target)
  })

  infection_process <- function(t) {
    I <- health$get_size_of("I")
    foi <- beta * I / N
    S <- health$get_index_of("S")
    S$sample(rate = pexp(q = foi * dt))
    health$queue_update(value = "I", index = S)
  }

  recovery_process <- function(t) {
    I <- health$get_index_of("I")
    already_scheduled <- recovery_event$get_scheduled()
    I$and(already_scheduled$not(inplace = TRUE))
    rec_times <- rgeom(n = I$size(), prob = pexp(q = gamma * dt)) + 1
    recovery_event$schedule(target = I, delay = rec_times)
  }

  health_render_process <- individual::categorical_count_renderer_process(
    renderer = health_render,
    variable = health,
    categories = health_states
  )

  birth_process <- function(t) {
    n_births <- rpois(1, birth_rate / dt)
    health$queue_extend(rep('S', n_births))
    recovery_event$queue_extend(n_births)
  }

  death_process <- function(t) {
    pop_size <- health$size()
    deaths <- sample.int(pop_size, rbinom(1, pop_size, min(death_rate / dt, 1)))
    health$queue_shrink(deaths)
    recovery_event$queue_shrink(deaths)
  }

  individual::simulation_loop(
    variables = list(health),
    events = list(recovery_event),
    processes = list(infection_process, recovery_process,
                     health_render_process, birth_process, death_process),
   timesteps = steps
  )

  health_render$to_dataframe()
}
