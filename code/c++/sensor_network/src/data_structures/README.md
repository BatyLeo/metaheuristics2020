# data_structures folder

## resource_flow.h and resource_flow.cpp

These files implement the ResourceFlow class that represents a flow of resources between the tasks of a project. This class relies on a rather complex structure to provide an easy access to the arcs with positive flow value, which is important for all the computations based on ResourceFlow. The class also has methods to save the state of a ResourceFlow and restore it afterwards.

## data_structures.h and data_structures.cpp

These files implement the data structures DataSet, ParametersSet and Solution that are respectively used to store the data related to the instances,the uncertainty set and the solutions.
