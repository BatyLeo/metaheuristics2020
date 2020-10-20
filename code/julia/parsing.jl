function parse_instance(file_path::String)
    target_coordinates = Array{Tuple{Float64, Float64}, 1}()

    io = open(file_path, "r")
    lines = split(read(io, String), "\n")
    for line in lines
        target = split(line)
        if length(target) != 3
            continue
        end
        index, coordinates = parse(Int64, target[1]), (parse(Float64, target[2]),parse(Float64, target[3]))

        # don't push source
        if index != 0
            push!(target_coordinates, coordinates)
        end
    end

    return target_coordinates
end
