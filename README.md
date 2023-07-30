# Algorithmic Trading

It's group project for three person.

## Part 1. Interpolation of tabulated functions with cubic spline interpolation

We ploted a tabulated function of stock quotes using interpolation methods:

- The program must be developed in C++ language of C++17 standard
- The program is built with Makefile which contains standard set of targets for GNU-programs all, install, uninstall, clean, dvi, dist. Installation directory is build.
- GUI implementation, based on any GUI library Qt6 with API for C++.
- The data are loaded into the program from a file with stock quotes with the .csv extension:
    -  The file contains data in the form of a table, where the first column is the date, the second column is the value of the function (examples of data files are in the materials folder)
- When uploading a new data file, clear the field for drawing graphs
- The user sets the number of points on which the graph should be plotted (the number of points is not less than in the loaded file)
- All points are evenly distributed between the start and end dates
- On the final graph adjacent points must be connected by a straight line
- The *Cubic Spline* interpolation method must be implemented.
- There should be a button in the interface for drawing the cubic spline graph
- There should be a button in the interface for clearing the field to draw graphs (the field is cleared only when you click on this button or new data is uploaded)
- There can be up to 5 graphs displayed in the field at the same time (all graphs have different color)
- If there are already 5 graphs displayed in the field, the buttons for drawing new graphs must be blocked
- The interface has to contain the following information about the graphs:
    - Color
    - Name of the file from which the data were taken
- Provide the ability to output the values of the stock quotes function obtained by interpolation method according to a user-defined argument value (date and time)

## Part 2. Approximation of tabulated functions

We ploted a tabulated function of stock quotes using the *least squares method*.
Moreover, the graph should cover a longer period of time than the input data:

- The user sets the number of points on which the graph should be plotted (the number of points is not less than in the loaded file)
- All points are evenly distributed between the start and end dates
- On the final graph adjacent points must be connected by a straight line
- On the graph, the points specified in the loaded file must be visually marked:
    - The radius of these points is larger than the thickness of the graph curve
    - The color of these points are differ from colors of the graphs in the field
- The user sets the number of days `M` for which we want to extend the graph (how many days ahead we want to predict the stock price)
- Clear the drawing field when adding a new approximation graph, in which `M` differs from `M` of already drawn graphs
- There can be up to 5 graphs with the same value of the number of days `M` displayed in the field at the same time
  (all graphs should have different color)
- There should be a button in the interface for drawing the graph plotted by the polynomial of the degree set at that time
- There should be a field in the interface for entering the degree of the polynomial
- There should be a button in the interface for clearing the field to draw the approximation graphs
- The interface has to contain the following information about the graphs:
    - Color
    - Name of the file from which the data were taken
    - Degree of polynomial
- If there are already 5 graphs displayed in the field, the buttons for drawing new graphs must be blocked
- Provide the ability to display the value of the approximating function for a given degree of the polynomial according to the user-defined value of the argument (date and time)

