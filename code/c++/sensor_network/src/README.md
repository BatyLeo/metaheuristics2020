# .src folders

## data_structures

This folder includes the files that define the classes used to store the project data, the user-defined parameters and the computed solutions.

## widgets

This folder includes the files that define the widgets classes. Each widget class inherits from a Qt class and corresponds to an entity in the application window (windows, sidebars...).
Some of the widget classes are views, that is to say widgets that display data.

## data_models

This folder includes the files that define model classes. Each model class inherits from a Qt abstract model class. The role of models is to provide an interface between the data and the views to make sure they interact properly. 
The implementation of a custom model requires to override some of the Qt abstract models virtual methods depending on the structure of the data and on whether it is possible or not to change it through the views.

## algorithms

This folder includes the files that define the various functions used by the interface to process the data. 

## tools

This folder includes the files that define functions used to load and save data.
