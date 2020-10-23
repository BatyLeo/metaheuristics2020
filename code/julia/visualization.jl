using Plots

function add_point_cloud!(figure::Plots.Plot{Plots.GRBackend}, point_cloud::Array{Tuple{Float64, Float64}, 1}; label::String=nothing, color::Symbol=:orange)
    x = []
    y = []

    for point in point_cloud
        push!(x, point[1])
        push!(y, point[2])
    end

    scatter!(figure, x, y, label=label, color=color)

    return figure
end

function instance_figure(instance::Dataset)
    # create an empty Plot
    figure = plot()
    # draw targets
    add_point_cloud!(figure, instance.target_coordinates, label="targets", color=:orange)
    # draw the source
    scatter!(figure, [0], [0], label="source", color=:purple, legend=:outertopright)
    return figure
end

function visualize_instance(instance::Dataset, save_path::String)
    figure = instance_figure(instance)
    savefig(figure, save_path)
end

function visualize_solution(solution::Solution, save_path::String)
    figure = instance_figure(solution.dataset)
    sensors_coordinates = [elem for (index, elem) in enumerate(solution.dataset.target_coordinates) if solution.target_has_sensor[index]]
    add_point_cloud!(figure, sensors_coordinates, label="sensors", color=:green)
    savefig(figure, save_path)
end