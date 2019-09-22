# Olympics-database
A small C-program where you can store name of countries and the amount of medals they have.

The program has next commands:

A countryname:

Adds a country named countryname to the database. Initially a country has 0 of each medals.

M countryname gold silver bronze:

Adds the given amounts of medals to the country named countryname. F.e (M Finland 0 1 1). Negative values are also allowed due to dopings etc.

L:

Prints each country and their medals to their own line. Countries are printed such that the country with the most gold medals is printed first.
If gold medals are equal, silver medals work as the tiebreaker. If they are even too, amount of bronze medals determine the order.

W file:

Saves the medal table to the given file.

O file:

Loads a table from the given file replacing the current table.

Q:

Leaves the program and frees all the memory.

An example run would look like this:

A China

A Finland

M China 2 1 1

M Finland 0 0 1

M China 1 3 1

M China -1 0 0

L

W medals

Q.
