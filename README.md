---
title: Intransitive dice generation
layout: default
permalink: README.html
---
[HTML-View](https://hartmuthoss.github.io/intransitive_dice_generator/README.html)

# Intransitive dice generation

**_or how to roll dice at Hilbert's hotel_**

[This project](https://github.com/hartmuthoss/intransitive_dice_generator) generates [intransitiv dice](https://en.wikipedia.org/wiki/Intransitive_dice):
* pairs of six-sided dice and their sum (six-sided sum dice),
* tuples of $K$ $M$-sided sum dice,
* and $N$ intransitive $M$-sided dice for almost any number of $N$ and $M$.

The aim is to create dice with intransitive cycles as long as possible by using
1. six-sided pairs of dice $(D_{11},D_{12}), (D_{21},D_{22}), ... (D_{K1},D_{K2})$, where 
   * either each pair of dice beats another pair of dice in the long run, i.e. $P(D_1>D_2) > 0.5, P(D_2>D_3) > 0.5,..., P(D_{K-1}>D_K) > 0.5, P(D_K>D_1) > 0.5$,
   * or each pair of dice loses to another pair of dice in the long run, i.e. $P(D_1>D_2) < 0.5, P(D_2>D_3) < 0.5,..., P(D_{K-1}>D_K) < 0.5, P(D_K>D_1) < 0.5$,
2. tuples of $M$-sided sum dice, and
3. $N$ intransitive $M$-sided dice.

## Introduction

### Partioning Muñoz-Perera dice

Muñoz-Perera's formula for $N ≥ 3$: "To obtain a set of $N$ fair intransitive dice of $N$ faces it is enough to set the values $v_{n,j} = (j - 1) \cdot N + (n - j) mod (N) + 1$ for $n,j=1,...,N$. Using this expression, it can be verified that $P(D_m < D_n) = 0.5 + 1 / (2 \cdot N) - ((n - m) mod (N)) / (N \cdot N)$. So each die beats $⌊N/2-1⌋$ dice in the set."

The path $(D_N,..., D_2, D_1, D_N)$ is always intransitive for those Muñoz-Perera dice, since $P(D_N > D_{N-1}) > 0.5,..., P(D_2 > D_1) > 0.5, P(D_1 > D_N) > 0.5$ for these dice (see ["A generalization of intransitive dice" by Adrian Muñoz-Perera](https://pereradrian.github.io/doc/adrian_munnoz_perera_generalized_intransitive_dice_2024.pdf)).

This relation seems to remain unchanged, if $N$ $N$-sided Muñoz-Perera dice with $N = K \cdot M$ are partitioned into $N$ tuples of $M$-sided dice, where each tuple has $K$ $M$-sided dice and the rolled sum of the $K$ $M$-sided dice in the tuple counts. 

I have not found a mathematical proof showing that this relationship remains unchanged under partitioning. However, we can easily test this hypothesis through simulation:
We just need to partition $N$ $N$-sided Muñoz-Perera dice with $N = K \cdot M$ into $N$ dice tuples, each of which has $K$ $M$-sided dice. The process of partioning is straightforward: we simply take $K$ subsets with $M$ elements each. As far as I could test, the path remains intransitive after partitioning. Unfortunately, I lack the mathematical knowledge to prove this hypothesis true. It might be possible to deduce this from the literature.
If anyone has a formal proof for all $N$, please let me know. ChatGPT created a [proof sketch](docs/intransitivity_munnoz_perera_partitioning.md).

### Construction of intransitive dice of any cycle length

To construct long intransitive cycles, we start with a set of known intransitive dice and add dice iteratively to that cycle. If we have two dice $D_i$ and $D_{i+1}$ in an intransitive cycle, i.e. $P(D_i>D_{i+1}) > 0.5$, we can construct a third die $D_j$ with $P(D_i>D_j) > 0.5$ and $P(D_j>D_{i+1}) > 0.5$. 

The first algorithm is quite simple:
* Let $D_i$ and $D_k$ be two $M$-sided dice with $P(D_i>D_k) > 0.5$, where $D_i[m]$ is the m-th value of die $D_i$ and
$D_k[m]$ is the m-th value of die $D_k$, with $0 ≤ m < M$ for $M$-sided dice. A new die $D_j$ can then be constructed with $P(D_i>D_j) > 0.5$ and $P(D_j>D_k) > 0.5$:
   * If $D_i[m] > D_k[m]$, then $D_j[m] < D_i[m]$ to ensure that $D_i$ beats $D_j$. We choose $D_j[m] = D_i[m] - 1$.
   * If $D_i[m] < D_k[m]$, then $D_j[m] > D_k[m]$ to ensure that $D_j$ beats $D_k$. We choose $D_j[m] = D_k[m] + 1$.
* If the difference between two dice values is too small to generate a new die in the cycle, we simply multiply all dice values by a constant factor greater than one, for example by a factor of two. Multiplying all dice values by a constant factor or adding a constant offset to all values does not change the probabilities of the dice.
* This simple algorithm works for surprisingly many dice, but is not bullet-proof. Counter examples exist.

The second algorithm uses a 3-level insertion:
* A new die $D_j$ with $P(D_i>D_j) > 0.5$ and $P(D_j>D_k) > 0.5$ is constructed from 3 different values: 
   * a low value $L < min(D_i, D_k)$,
   * a high value $H > max(D_i, D_k)$,
   * a threshold value $T$ computed from the cumulative probability distribution of $D_i$ and $D_k$, see [3-level insertion](docs/construction_of_N_intransitive_M_sided_dice.md) for details.
* To convert reell face values to integer, we multiply all dice values by a constant factor greater than one and cast the result. If the factor is large enough, the resulting integer dice will still hold $P(D_i>D_j) > 0.5$ and $P(D_j>D_k) > 0.5$.
* To my knowledge, the 3-level insertion is bullet-proof and works on all dice with $P(D_i>D_k) > 0.5$.

We repeat this process iteratively until the cycle length reaches the desired limit. This enables us to enlarge a given intransitive cycle by adding new dice ad infinitum. As far as I could test, we can easily generate millions of intransitive dice in this way, starting with six-sided Oskar dice, Grime dice, $M$-sided Muñoz-Perera dice or $M$-sided Clary-Leininger dice (or probably with any other intransitive dice).

**Conjecture: For any fixed values of $N$ and $M$ greater than $2$, there exist sets of $N$ $M$-sided dice forming a strict intransitive cycle.**

The conjecture matches Alex Schaefers theorem proven in [Balanced Non-Transitive Dice II: Tournaments](https://arxiv.org/pdf/1706.08986):

> Theorem 2.1. For any n,m ≥ 3, there exists a non-transitive set of n balanced m-sided dice.

It also matches the [central limit theorem for intransitive dice](https://arxiv.org/pdf/2310.17083) by Luis G. Coelho, Tertuliano Franco, Lael V. Lima, João P.C. de Paula, João V.A. Pimenta, Guilherme L.F. Silva and Daniel Ungaretti:

> Theorem 1. Consider dice whose face entries are positive integers. For every ℓ ≥ 3 and n ≥ 3 there exists a no-tie collection of ℓ honest n-sided dice which is intransitive.

However, I provided my own [proof sketch](docs/construction_of_N_intransitive_M_sided_dice.md) as an exercise. The sketch comes without any warranty, since I am not a mathematican. A proof might also be deducable from the literature. ChatGPT created another [proof sketch](docs/intransitivity_fixed_M_arbitrary_cycle.md).

## Examples

Some examples from the simulation:

```
12 tupel of 2 6-sided dice from 12x12 Munnoz-Perera dice:
  D00: ( (   1  24  35  46  57  68 ), (  79  90 101 112 123 134 ) )
  D01: ( (   2  13  36  47  58  69 ), (  80  91 102 113 124 135 ) )
  D02: ( (   3  14  25  48  59  70 ), (  81  92 103 114 125 136 ) )
  D03: ( (   4  15  26  37  60  71 ), (  82  93 104 115 126 137 ) )
  D04: ( (   5  16  27  38  49  72 ), (  83  94 105 116 127 138 ) )
  D05: ( (   6  17  28  39  50  61 ), (  84  95 106 117 128 139 ) )
  D06: ( (   7  18  29  40  51  62 ), (  73  96 107 118 129 140 ) )
  D07: ( (   8  19  30  41  52  63 ), (  74  85 108 119 130 141 ) )
  D08: ( (   9  20  31  42  53  64 ), (  75  86  97 120 131 142 ) )
  D09: ( (  10  21  32  43  54  65 ), (  76  87  98 109 132 143 ) )
  D10: ( (  11  22  33  44  55  66 ), (  77  88  99 110 121 144 ) )
  D11: ( (  12  23  34  45  56  67 ), (  78  89 100 111 122 133 ) )
P(D11>D10) = 0.540, P(D10>D9) = 0.534, P(D9>D8) = 0.531, P(D8>D7) = 0.531, P(D7>D6) = 0.534, P(D6>D5) = 0.540, P(D5>D4) = 0.540, P(D4>D3) = 0.534, P(D3>D2) = 0.531, P(D2>D1) = 0.531, P(D1>D0) = 0.534, P(D0>D11) = 0.540 (intransitive path)
```
```
30 tupel of 5 6-sided dice from 30x30 Munnoz-Perera dice:
  D00: ( (   1  60  89 118 147 176 ), ( 205 234 263 292 321 350 ), ( 379 408 437 466 495 524 ), ( 553 582 611 640 669 698 ), ( 727 756 785 814 843 872 ) )
  D01: ( (   2  31  90 119 148 177 ), ( 206 235 264 293 322 351 ), ( 380 409 438 467 496 525 ), ( 554 583 612 641 670 699 ), ( 728 757 786 815 844 873 ) )
  D02: ( (   3  32  61 120 149 178 ), ( 207 236 265 294 323 352 ), ( 381 410 439 468 497 526 ), ( 555 584 613 642 671 700 ), ( 729 758 787 816 845 874 ) )
  D03: ( (   4  33  62  91 150 179 ), ( 208 237 266 295 324 353 ), ( 382 411 440 469 498 527 ), ( 556 585 614 643 672 701 ), ( 730 759 788 817 846 875 ) )
  D04: ( (   5  34  63  92 121 180 ), ( 209 238 267 296 325 354 ), ( 383 412 441 470 499 528 ), ( 557 586 615 644 673 702 ), ( 731 760 789 818 847 876 ) )
  ...
  D28: ( (  29  58  87 116 145 174 ), ( 203 232 261 290 319 348 ), ( 377 406 435 464 493 522 ), ( 551 580 609 638 667 696 ), ( 725 754 783 812 841 900 ) )
  D29: ( (  30  59  88 117 146 175 ), ( 204 233 262 291 320 349 ), ( 378 407 436 465 494 523 ), ( 552 581 610 639 668 697 ), ( 726 755 784 813 842 871 ) )
P(D29>D28) = 0.525, P(D28>D27) = 0.523, ..., P(D4>D3) = 0.523, P(D3>D2) = 0.522, P(D2>D1) = 0.522, P(D1>D0) = 0.523, P(D0>D29) = 0.525 (intransitive path)
```
```
Extended Oskar dice:
  D00: (  257 257 3329 3329 4097 4097 )
  D01: (  2048 2048 3328 3328 4096 4096 )
  D02: (  2047 2047 3327 3327 4095 4095 )
  D03: (  2045 2045 3325 3325 4093 4093 )
  D04: (  2043 2043 3323 3323 4091 4091 )
  ...
  D995: (  277 277 3349 3349 4097 4097 )
  D996: (  273 273 3345 3345 4097 4097 )
  D997: (  269 269 3341 3341 4097 4097 )
  D998: (  265 265 3337 3337 4097 4097 )
  D999: (  261 261 3333 3333 4097 4097 )
P(D0>D1) = 0.556, P(D1>D2) = 0.667, P(D2>D3) = 0.667, P(D3>D4) = 0.667, ..., P(D995>D996) = 0.556, P(D996>D997) = 0.556, P(D997>D998) = 0.556, P(D998>D999) = 0.556, P(D999>D0) = 0.556 (intransitive path)
```
```
Extended Grime dice:
 D00: (  1025 1025 1025 3585 3585 3585 )
 D01: (  1024 1024 3584 3584 3584 3584 )
 D02: (  1023 1023 3583 3583 3583 3583 )
 D03: (  1021 1021 3581 3581 3581 3581 )
 D04: (  1019 1019 3579 3579 3579 3579 )
 ...
 D995: (  1045 1045 1045 3605 3605 3605 )
 D996: (  1041 1041 1041 3601 3601 3601 )
 D997: (  1037 1037 1037 3597 3597 3597 )
 D998: (  1033 1033 1033 3593 3593 3593 )
 D999: (  1029 1029 1029 3589 3589 3589 )
P(D0>D1) = 0.667, P(D1>D2) = 0.778, P(D2>D3) = 0.778, P(D3>D4) = 0.778, ..., P(D995>D996) = 0.750, P(D996>D997) = 0.750, P(D997>D998) = 0.750, P(D998>D999) = 0.750, P(D999>D0) = 0.750 (intransitive path)
```

Complete examples are given in the [logfile](docs/intransitive_dice_generator.log).

## Build and run

Use the following commands to build and run this project on Windows with Visual Studio 2022 and cmake:
```
call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=amd64 -host_arch=amd64
mkdir build_win64
pushd build_win64
cmake -G "Visual Studio 17 2022" ..
if %ERRORLEVEL% neq 0 ( @echo ERROR building cmake files & @pause )
cmake --build . --clean-first --config Debug
cmake --build . --clean-first --config Release
if %ERRORLEVEL% neq 0 ( @echo ERROR building intransive dice binaries & @pause )
.\Release\intransitive_dice_generator.exe
popd
```
Open file `build_win64/intransive_dice.sln` in Visual Studio 2022 for debugging and development.

Use the following commands to build and run this project on Linux:
```
mkdir -p ./build_linux
pushd ./build_linux
cmake -G "Unix Makefiles" ..
make -j4
./intransitive_dice_generator
```

By default (i.e. no command line options given), intransitive_dice_generator creates and prints some examples of intransitive dice with different cycle lengths.  
To create a chain of N intransitive M-sided dice with N ≥ M, call intransitive_dice_generator with arguments `-N=<int> -M=<int>`.  
Example: `intransitive_dice_generator -N=12 -M=6` creates 12 intransitive 6-sided dice.  
Note: intransitive_dice_generator logs all messages in logfile intransitive_dice_generator.log.

## Appendix

Dice operations that leave intransitivity invariant:
* Increase the number of dice (convolution with itself): Instead of one die, 2 or more identical dice and their sum can be used.
* Addition: Add a constant to all numbers on all dice.
* Multiplication: Multiply all numbers on all dice by a constant > 0.
* Subtraction: Subtract the smallest number from all numbers on all dice.
* Division: Divide all numbers on all dice by the greatest common divisor of all numbers.

Articles and scientific literature:

* Wikipedia: "Intransitive dice", https://en.wikipedia.org/wiki/Intransitive_dice
* Adrian Muñoz-Perera: "A generalization of intransitive dice": https://pereradrian.github.io/doc/adrian_munnoz_perera_generalized_intransitive_dice_2024.pdf
* Ethan Akin, Julia Saccamano: "Generalized Intransitive Dice II: Partition Constructions": https://arxiv.org/pdf/1905.01750
* Erika Clary, Dr. Verne Leininger: "Proving Pairwise Intransitivity in Sets of Dice": https://digitalcommons.bridgewater.edu/cgi/viewcontent.cgi?article=1025&context=honors_projects
* Coelho, Franco, Lima, Paula, Pimenta, Silva, Ungaretti: "A Central Limit Theorem for intransitive dice", https://arxiv.org/pdf/2310.17083
* Conrey, Gabbard, Grant, Liu, Morrison: "Intransitive Dice", https://aimath.org/~morrison/Research/IntransitiveDice.pdf
* Ashwin Sah, Mehtaab Sawhney: "The intransitive dice kernel", https://dspace.mit.edu/bitstream/handle/1721.1/153985/440_2024_Article_1270.pdf?isAllowed=y&sequence=1
* Joe Buhler, Ron Graham, Al Hales: "Maximally Nontransitive Dice", https://mathweb.ucsd.edu/~ronspubs/18_01_nontransitive.pdf
* Artem Hulko, Mark Whitmeyer: "A Game of Nontransitive Dice", https://www.researchgate.net/publication/317356505_A_Game_of_Nontransitive_Dice
* Alex Schaefer: "Balanced Non-Transitive Dice II: Tournaments", https://arxiv.org/pdf/1706.08986
* Levi Angel, Matt Davis: "A Direct Construction of Non-Transitive Dice Sets", https://arxiv.org/pdf/1610.08595
* Deszo Bednay, Sandor Bozoki: "Constructions for Nontransitive Dice Sets", https://eprints.sztaki.hu/7623/1/Bednay_15_2507773_ny.pdf

An anecdote from https://no.wikipedia.org/wiki/Ikketransitive_terninger:

> "Warren Buffett er kjent som fan av ikketransitive terninger... Buffett prøvde en gang å vinne et slags terningspill mot Bill Gates ved å bruke ikketransitive terninger.
> 'Buffett foreslo at hver av dem valgte en av terningene, for så å legge bort resten. De vil så vedde om hvem som kaster høyest flest ganger. Buffett tilbød Gates å velge terning først.
> Dette tilbudet vakte med en gang Gates mistenksomhet. Han krevde å først få undersøke terningene, hvoretter han krevde at Buffett velger først.'"

Warren Buffett is known to be a fan of non-transitive dice... Buffett once tried to win a dice game against Bill Gates by using non-transitive dice.
Buffett suggested that each of them choose one of the dice and then put the rest away. They would then bet on who would roll the highest number of times. Buffett offered Gates the chance to choose the dice first.
This offer immediately aroused Gates' suspicion. He demanded to examine the dice first, after which he demanded that Buffett choose first.

Fun fact: If you've ever wanted to invite all the guests at [Hilbert's famous hotel](https://en.wikipedia.org/wiki/Hilbert%27s_paradox_of_the_Grand_Hotel) to roll dice, [here's](https://github.com/hartmuthoss/intransitive_dice_generator) a guaranteed way for everyone to win!

Keywords: intransitive dice, nontransitive dice, intransitive cycle, Muñoz-Perera.
