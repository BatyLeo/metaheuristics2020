using JuMP, Gurobi

function build_MIP(dataset::Dataset)
    model = Model(Gurobi.Optimizer)
    set_optimizer_attribute(model, "LogFile", "log.txt")

    number_targets = dataset.number_targets
    E_com_length = length(dataset.communication_edges)
    D_E_com_length = length(dataset.arc_to_index)  # = 2 * E_com_length
    source_index = number_targets + 1

    # @variable(model, 0 <= delta[1:number_targets+1] <= 1)
    # @variable(model, 0 <= x[1:E_com_length] <= 1)
    delta = @variable(model, delta[1:number_targets+1], Bin)
    x = @variable(model, x[1:E_com_length], Bin)
    y = @variable(model, 0 <= y[1:D_E_com_length])

    @objective(model, Min, sum(delta[target] for target in 1:number_targets))

    println("Starting to build constraints")

    # the source is in the communication graph
    @constraint(model, delta[source_index] == 1) 

    # k-connexity
    @constraint(
        model,
        [target in 1:number_targets],
        sum(delta[other_target] for other_target in dataset.reception_neighbors[target]) + delta[target] >= dataset.reception_level
    )

    @constraint(model, sum(x[edge] for edge in 1:E_com_length) == sum(delta[target] for target in 1:number_targets))
    
    for (edge_index, edge) in enumerate(dataset.communication_edges)
        @constraint(model, x[edge_index] <= delta[edge[1]])
        @constraint(model, x[edge_index] <= delta[edge[2]])
    end

    @constraint(
        model,
        [target in 1:number_targets],
        sum(y[dataset.arc_to_index[(other_target, target)]] - y[dataset.arc_to_index[(target, other_target)]]
            for other_target in dataset.communication_neighbors[target]) == delta[target]
    )
    
    @constraint(
        model,
        sum(y[dataset.arc_to_index[(source_index, target)]] - y[dataset.arc_to_index[(target, source_index)]]
            for target in dataset.source_communication_neighbors) == sum(delta[target] for target in 1:number_targets)
    )

    @constraint(
        model,
        [edge in 1:E_com_length],
        y[dataset.arc_to_index[(dataset.communication_edges[edge][1], dataset.communication_edges[edge][2])]] <= number_targets * x[edge]
    )

    @constraint(
        model,
        [edge in 1:E_com_length],
        y[dataset.arc_to_index[(dataset.communication_edges[edge][2], dataset.communication_edges[edge][1])]] <= number_targets * x[edge]
    )

    variables = Dict{String, Array{VariableRef}}("delta"=>delta, "x"=>x, "y"=> y)

    return model, variables
end

function solve_MIP!(model::Model, variables::Dict{String, Array{VariableRef}}, dataset::Dataset)
    optimize!(model)

    delta, x, y = variables["delta"], variables["x"], variables["y"]
    target_has_sensor = Array{Bool, 1}()

    solution = value.(delta)
    for (index, elem) in enumerate(solution)
        if (elem == 1) & (index <= dataset.number_targets)
            push!(target_has_sensor, true)
        elseif index <= dataset.number_targets
            push!(target_has_sensor, false)
        end
    end

    return Solution(target_has_sensor, dataset, score=floor(Int64, objective_value(model)))
end