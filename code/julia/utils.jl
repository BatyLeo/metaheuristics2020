function square_distance(position1::Tuple{Real, Real}, position2::Tuple{Real, Real})
    return (position1[1] - position2[1])^2 + (position1[2] - position2[2])^2
end
