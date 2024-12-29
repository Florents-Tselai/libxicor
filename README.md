# libxicor: Pure C implementation of the XI (ξ) Correlation Coefficient

`xi(X,Y) = 0` if and only if the two variables are independent, while `xi(X,Y) = 1` if `Y` is a function of `X`.

> [!CAUTION]
> Contrary to many other correlation coefficients,
> XI is not symmetric.

The xi correlation (xicor) is based
on cross correlation between ranked increments.
For a simple explanation see [this video](https://www.youtube.com/watch?v=2OTHH8wz25c)

For more info on XI, see the original paper.
[A New Coefficient of Correlation S. Chatterjee 2020](https://gwern.net/doc/statistics/order/2020-chatterjee.pdf)

## Usage

### C API

A full example of how you can embed `libxicor` in your application
can be found in `xicor_example.c`:

```bash
gcc xicor_example.c -O3 -Wall xicor.c -I. -lm -o xicor_example
```

### Explore CSV

Suppose you have a CSV file and you want to explore the correlation between its columns. 

You can use the supplied `xicorcsv` program to explore
the correlations between a target `Y` column (dependent variable)

```
Usage: ./xicorcsv <csv_file> <column_name(Y)> <n_lines>
```

Assuming the following `small.csv` file 

```
x,y,z
1.0,1.0,3.0
-5.0,25.0,5.3
2,4,4.6
6,36,7.8
-1,1,3.99999
```

Running

```
 ./xicorcsv small.csv Y 5 
```

Will output 
```
Computing XICOR scores for first 5 rows of column 'Y'...

XICOR(X, Y): 0.1250000000 (Elapsed time: 0.000002 seconds)
XICOR(Z, Y): 0.5000000000 (Elapsed time: 0.000001 seconds)
```
