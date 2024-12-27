# libxicor: Pure C implementation of the XI (ξ) Correlation Coefficient

`xi(X, Y) = 0` if and only if the two variables are independent, 
while `xi(X, Y) = 1` if variable `Y` is a function of `X`.

> [!CAUTION]
> Contrary to many other correlation coefficients,
> XI is not symmetric.

The xi correlation (xicor) is based
on cross correlation between ranked increments.

For more info on XI, see the original paper
[A New Coefficient of Correlation S. Chatterjee 2020](https://gwern.net/doc/statistics/order/2020-chatterjee.pdf)

## Usage

A full example of how you can embed `libxicor` in your application
can be found in `xicor_example.c`:

```bash
gcc xicor_example.c -O3 -Wall xicor.c -I. -lm -o xicor_example
```

