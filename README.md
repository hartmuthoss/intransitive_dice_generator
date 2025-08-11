# Intransitive dice search and generation

This project generates [intransitiv dice](https://en.wikipedia.org/wiki/Intransitive_dice). 

Unlike intransitive dice with more than six sides, this project considers
* pairs of six-sided dice and their sum (six-sided sum dice), and
* more generally, tuples of K M-sided sum dice.

The aim is to find intransitive cycles as long as possible
1. with six-sided pairs of dice `(D_11,D12), (D_21,D22), ... (D_K1,D_K2)`, where 
   * either each pair of dice beats another pair of dice in the long run, i.e. `P(D_1>D_2) > 0.5, P(D_2>D_3) > 0.5,..., P(D_(K-1)>D_K) > 0.5, P(D_K>D_1) > 0.5`,
   * or each pair of dice loses to another pair of dice in the long run, i.e. `P(D_1>D_2) < 0.5, P(D_2>D_3) < 0.5,..., P(D_(K-1)>D_K) < 0.5, P(D_K>D_1) < 0.5`.
2. with tuples of M-sided sum dice.

## Introduction

Muñoz-Perera's formula for `N>=3`: "To obtain a set of N fair intransitive dice of N faces it is enough to set the values `v_n,j = (j-1)*N+(n-j)%(N)+1 for n,j=1,...,N.`
Using this expression, it can be verified that `P(D_m < D_n) = 0.5 + 1/(2*N) - ((n-m)%N)/(N*N)`. So each die beats `floor(N/2-1)` dice in the set."

The path `{ D_N,..., D_2, D_1, D_N }` is always intransitive for those Muñoz-Perera dice, since `P(D_N > D_(N-1)) > 0.5,..., P(D_2 > D_1) > 0.5, P(D_1 > D_N) > 0.5`
for these dice (see ["A generalization of intransitive dice" by Adrian Muñoz-Perera](https://pereradrian.github.io/doc/adrian_munnoz_perera_generalized_intransitive_dice_2024.pdf))

This relation seems to remain unchanged, if N N-sided Muñoz-Perera dice with `N = K*M` are partitioned into N tuples of M-sided dice, where each tuple has K M-sided dice 
and the rolled sum of the K M-sided dice in the tuple counts. 

I haven't found a mathematical proof that this relation remains unchanged under partitioning. However, we can easily test this hypothesis through simulation:
We just need to partition N N-sided Muñoz-Perera dice with N = K\*M into N dice tuples, each of which has K M-sided dice. As far as I could test, the path remains intransitive
after partitioning. Unfortunately, I lack the mathematical knowledge to prove the truth of this hypothesis. It might be possible to conclude this from the literature.
If anyone has a proof for all N, please let me know. ChatGPT created a [proof sketch](doc/intransitivity_munnoz_perrera_partitioning.md).

## Examples

Some examples of 6-sided dice tuples from the simulation:

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

30 tupel of 5 6-sided dice from 30x30 Munnoz-Perera dice:
  D00: ( (   1  60  89 118 147 176 ), ( 205 234 263 292 321 350 ), ( 379 408 437 466 495 524 ), ( 553 582 611 640 669 698 ), ( 727 756 785 814 843 872 ) )
  D01: ( (   2  31  90 119 148 177 ), ( 206 235 264 293 322 351 ), ( 380 409 438 467 496 525 ), ( 554 583 612 641 670 699 ), ( 728 757 786 815 844 873 ) )
  D02: ( (   3  32  61 120 149 178 ), ( 207 236 265 294 323 352 ), ( 381 410 439 468 497 526 ), ( 555 584 613 642 671 700 ), ( 729 758 787 816 845 874 ) )
  D03: ( (   4  33  62  91 150 179 ), ( 208 237 266 295 324 353 ), ( 382 411 440 469 498 527 ), ( 556 585 614 643 672 701 ), ( 730 759 788 817 846 875 ) )
  D04: ( (   5  34  63  92 121 180 ), ( 209 238 267 296 325 354 ), ( 383 412 441 470 499 528 ), ( 557 586 615 644 673 702 ), ( 731 760 789 818 847 876 ) )
  D05: ( (   6  35  64  93 122 151 ), ( 210 239 268 297 326 355 ), ( 384 413 442 471 500 529 ), ( 558 587 616 645 674 703 ), ( 732 761 790 819 848 877 ) )
  D06: ( (   7  36  65  94 123 152 ), ( 181 240 269 298 327 356 ), ( 385 414 443 472 501 530 ), ( 559 588 617 646 675 704 ), ( 733 762 791 820 849 878 ) )
  D07: ( (   8  37  66  95 124 153 ), ( 182 211 270 299 328 357 ), ( 386 415 444 473 502 531 ), ( 560 589 618 647 676 705 ), ( 734 763 792 821 850 879 ) )
  D08: ( (   9  38  67  96 125 154 ), ( 183 212 241 300 329 358 ), ( 387 416 445 474 503 532 ), ( 561 590 619 648 677 706 ), ( 735 764 793 822 851 880 ) )
  D09: ( (  10  39  68  97 126 155 ), ( 184 213 242 271 330 359 ), ( 388 417 446 475 504 533 ), ( 562 591 620 649 678 707 ), ( 736 765 794 823 852 881 ) )
  D10: ( (  11  40  69  98 127 156 ), ( 185 214 243 272 301 360 ), ( 389 418 447 476 505 534 ), ( 563 592 621 650 679 708 ), ( 737 766 795 824 853 882 ) )
  D11: ( (  12  41  70  99 128 157 ), ( 186 215 244 273 302 331 ), ( 390 419 448 477 506 535 ), ( 564 593 622 651 680 709 ), ( 738 767 796 825 854 883 ) )
  D12: ( (  13  42  71 100 129 158 ), ( 187 216 245 274 303 332 ), ( 361 420 449 478 507 536 ), ( 565 594 623 652 681 710 ), ( 739 768 797 826 855 884 ) )
  D13: ( (  14  43  72 101 130 159 ), ( 188 217 246 275 304 333 ), ( 362 391 450 479 508 537 ), ( 566 595 624 653 682 711 ), ( 740 769 798 827 856 885 ) )
  D14: ( (  15  44  73 102 131 160 ), ( 189 218 247 276 305 334 ), ( 363 392 421 480 509 538 ), ( 567 596 625 654 683 712 ), ( 741 770 799 828 857 886 ) )
  D15: ( (  16  45  74 103 132 161 ), ( 190 219 248 277 306 335 ), ( 364 393 422 451 510 539 ), ( 568 597 626 655 684 713 ), ( 742 771 800 829 858 887 ) )
  D16: ( (  17  46  75 104 133 162 ), ( 191 220 249 278 307 336 ), ( 365 394 423 452 481 540 ), ( 569 598 627 656 685 714 ), ( 743 772 801 830 859 888 ) )
  D17: ( (  18  47  76 105 134 163 ), ( 192 221 250 279 308 337 ), ( 366 395 424 453 482 511 ), ( 570 599 628 657 686 715 ), ( 744 773 802 831 860 889 ) )
  D18: ( (  19  48  77 106 135 164 ), ( 193 222 251 280 309 338 ), ( 367 396 425 454 483 512 ), ( 541 600 629 658 687 716 ), ( 745 774 803 832 861 890 ) )
  D19: ( (  20  49  78 107 136 165 ), ( 194 223 252 281 310 339 ), ( 368 397 426 455 484 513 ), ( 542 571 630 659 688 717 ), ( 746 775 804 833 862 891 ) )
  D20: ( (  21  50  79 108 137 166 ), ( 195 224 253 282 311 340 ), ( 369 398 427 456 485 514 ), ( 543 572 601 660 689 718 ), ( 747 776 805 834 863 892 ) )
  D21: ( (  22  51  80 109 138 167 ), ( 196 225 254 283 312 341 ), ( 370 399 428 457 486 515 ), ( 544 573 602 631 690 719 ), ( 748 777 806 835 864 893 ) )
  D22: ( (  23  52  81 110 139 168 ), ( 197 226 255 284 313 342 ), ( 371 400 429 458 487 516 ), ( 545 574 603 632 661 720 ), ( 749 778 807 836 865 894 ) )
  D23: ( (  24  53  82 111 140 169 ), ( 198 227 256 285 314 343 ), ( 372 401 430 459 488 517 ), ( 546 575 604 633 662 691 ), ( 750 779 808 837 866 895 ) )
  D24: ( (  25  54  83 112 141 170 ), ( 199 228 257 286 315 344 ), ( 373 402 431 460 489 518 ), ( 547 576 605 634 663 692 ), ( 721 780 809 838 867 896 ) )
  D25: ( (  26  55  84 113 142 171 ), ( 200 229 258 287 316 345 ), ( 374 403 432 461 490 519 ), ( 548 577 606 635 664 693 ), ( 722 751 810 839 868 897 ) )
  D26: ( (  27  56  85 114 143 172 ), ( 201 230 259 288 317 346 ), ( 375 404 433 462 491 520 ), ( 549 578 607 636 665 694 ), ( 723 752 781 840 869 898 ) )
  D27: ( (  28  57  86 115 144 173 ), ( 202 231 260 289 318 347 ), ( 376 405 434 463 492 521 ), ( 550 579 608 637 666 695 ), ( 724 753 782 811 870 899 ) )
  D28: ( (  29  58  87 116 145 174 ), ( 203 232 261 290 319 348 ), ( 377 406 435 464 493 522 ), ( 551 580 609 638 667 696 ), ( 725 754 783 812 841 900 ) )
  D29: ( (  30  59  88 117 146 175 ), ( 204 233 262 291 320 349 ), ( 378 407 436 465 494 523 ), ( 552 581 610 639 668 697 ), ( 726 755 784 813 842 871 ) )
P(D29>D28) = 0.525, P(D28>D27) = 0.523, P(D27>D26) = 0.522, P(D26>D25) = 0.522, P(D25>D24) = 0.523, P(D24>D23) = 0.525, P(D23>D22) = 0.525, P(D22>D21) = 0.523, P(D21>D20) = 0.522, P(D20>D19) = 0.522, P(D19>D18) = 0.523, P(D18>D17) = 0.525, P(D17>D16) = 0.525, P(D16>D15) = 0.523, P(D15>D14) = 0.522, P(D14>D13) = 0.522, P(D13>D12) = 0.523, P(D12>D11) = 0.525, P(D11>D10) = 0.525, P(D10>D9) = 0.523, P(D9>D8) = 0.522, P(D8>D7) = 0.522, P(D7>D6) = 0.523, P(D6>D5) = 0.525, P(D5>D4) = 0.525, P(D4>D3) = 0.523, P(D3>D2) = 0.522, P(D2>D1) = 0.522, P(D1>D0) = 0.523, P(D0>D29) = 0.525 (intransitive path)
```

Further examples are given in the [logfile](doc/intransitive_dice_generator.log).

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

## Appendix

Dice operations that leave intransitivity invariant:
* Increase the number of dice (convolution with itself): Instead of one die, 2 or more identical dice and their sum are used.
* Addition: Add a constant to all numbers on all dice.
* Multiplication: Multiply all numbers on all dice by a constant > 0.
* Subtraction: Subtract the smallest number from all numbers on all dice.
* Division: Divide all numbers on all dice by the greatest common divisor of all numbers.

Search method for finding intransitive dice or tuples of dice with the longest possible chains:
1. Division of N-sided intransitive cubes into M-sided cube pairs or tuples:
   Using the Muñoz-Perera formula, N intransitive N-sided cubes are generated with `N=K*M`. Each N-sided cube is divided into K M-sided cube tuples.
2. Greedy extension:
  1. Use Muñoz-Perera's formula to determine a solution for `N=6` (starting solution).
  2. Doubling the dice: Replace each die with a pair of two identical dice (create pairs of dice).
  3. Doubling the numbers: Multiply all numbers on all dice by 2 and add 1 (create space for the numbers of other solutions).
  4. Try to find a new pair of dice `D_k` between two consecutive pairs of dice `D_i` and `D_(i+1)` with `P(D_i < D_k) > 0.5` and `P(D_k < D_(i+1)) > 0.5`,
     e.g. by trial and error (brute force search)
  5. Ways to further reduce the number of permutations:
     * Create set `M1` of all numbers from `D_i` and `D_(i+1)`.
     * Create set `M2 = { number from M1 - 1 } ∪ { number from M1 } ∪ { number from M1 + 1 }` = set of numbers to test for `D_k`
     * Test `D_k` with all permutations from the numbers of eyes from `M2`.
     * Intransitivity is sufficient; fair dice are not required. With fair dice (equal probabilities), however, the order of the numbers does not matter.
        Then, instead of 6! permutations, only the multisets (combinations with repetition) of 6 numbers from M2 need to be tested.
     * Use symmetry: The expected value `E(D_k)` must lie between `E(D_i)` and `E(D_(i+1))` (necessary but not sufficient).
     * Cache and skip already calculated sums of numbers and their probability distributions.
     * Permutations can be generated efficiently with std::next_permutation.

Articles and scientific literature:

* Wikipedia: https://en.wikipedia.org/wiki/Intransitive_dice
* Adrian Muñoz-Perera, "A generalization of intransitive dice": https://pereradrian.github.io/doc/adrian_munnoz_perera_generalized_intransitive_dice_2024.pdf
* Ethan Akin, Julia Saccamano, "Generalized Intransitive Dice II: Partition Constructions": https://arxiv.org/pdf/1905.01750
* Erika Clary, Dr. Verne Leininger, "Proving Pairwise Intransitivity in Sets of Dice": https://digitalcommons.bridgewater.edu/cgi/viewcontent.cgi?article=1025&context=honors_projects
* Coelho, Franco, Lima, Paula, Pimenta, Silva, Ungaretti, "A Central Limit Theorem for intransitive dice": https://arxiv.org/pdf/2310.17083
* Conrey, Gabbard, Grant, Liu, Morrison, "Intransitive Dice": https://aimath.org/~morrison/Research/IntransitiveDice.pdf
* Ashwin Sah, Mehtaab Sawhney, "The intransitive dice kernel": https://dspace.mit.edu/bitstream/handle/1721.1/153985/440_2024_Article_1270.pdf?isAllowed=y&sequence=1

An anecdote from https://no.wikipedia.org/wiki/Ikketransitive_terninger:

Warren Buffett is known to be a fan of non-transitive dice... Buffett once tried to win a dice game against Bill Gates by using non-transitive dice.
Buffett suggested that each of them choose one of the dice and then put the rest away. They would then bet on who would roll the highest number of times. Buffett offered Gates the chance to choose the dice first.
This offer immediately aroused Gates' suspicion. He demanded to examine the dice first, after which he demanded that Buffett choose first.

("Warren Buffett er kjent som fan av ikketransitive terninger... Buffett prøvde en gang å vinne et slags terningspill mot Bill Gates ved å bruke ikketransitive terninger.
'Buffett foreslo at hver av dem valgte en av terningene, for så å legge bort resten. De vil så vedde om hvem som kaster høyest flest ganger. Buffett tilbød Gates å velge terning først.
Dette tilbudet vakte med en gang Gates mistenksomhet. Han krevde å først få undersøke terningene, hvoretter han krevde at Buffett velger først.'")
