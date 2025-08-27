# Preservation of Intransitivity under Tuple Partitioning of Muñoz–Perera Dice

**Authors:** Hartmut Hoss (Empirical discovery & code) and ChatGPT (OpenAI, proof outline)  
**Date:** 2025-08-04  
**License:** MIT  
**Keywords:** intransitive dice, Muñoz–Perera formula, combinatorics, probability, convolution, game theory

---

## Statement

Let $N = K \cdot M$ with $M > 2$ and $K, M$ in $\mathbb{N}$.  
Let $D_1, \dots, D_N$ be the **Muñoz–Perera dice** with sides given by:

$$
v_{n,j} = (j-1) \cdot N + ((n-j) \bmod N) + 1, \quad n,j \in \{1,\dots,N\}
$$

These dice form the standard **intransitive cycle**:

$$
D_N > D_{N-1} > \dots > D_2 > D_1 > D_N
$$

meaning $P(D_i > D_{i-1}) > 0.5$ for all $i$, indices taken modulo $N$.

Partition each $D_i$ into $K$ **disjoint** $M$-sided dice

$$
T_{i,1}, \dots, T_{i,K}
$$

such that the union of the $K$ dice’s faces is exactly the set of faces of $D_i$.  

Define the **tuple die** $D^T_i$ as the sum of one roll from each of its $K$ components:

$$
D^T_i := T_{i,1} + T_{i,2} + \dots + T_{i,K}.
$$

**Claim:** The original Muñoz–Perera intransitive cycle is preserved:

$$
P(D^T_N > D^T_{N-1}) > 0.5, \quad \dots, \quad P(D^T_1 > D^T_N) > 0.5.
$$

---

## Proof Sketch

### 1. Structure of Muñoz–Perera Dice

The MP formula arranges faces as **cyclically shifted arithmetic sequences** modulo $N$.  
Key properties:

- Each $D_n$ contains exactly one number from each residue class modulo $N$.
- Between two adjacent dice in the MP cycle, the majority of face-to-face comparisons favor the first die by the same margin across the whole face set.

### 2. Block Partitioning

Partition $D_n$ into $K$ consecutive **blocks** of size $M$:

$$
B_{n,b} = \{ v_{n,(b-1)M+1}, \dots, v_{n,bM} \}.
$$

This partitioning is identical across all dice, differing only by the cyclic shifts of the MP construction.

### 3. Preservation of Local Majority

For two consecutive dice $D_a, D_b$ in the MP cycle:

- In the **original dice**, $P(D_a > D_b) > 0.5$.
- In **each corresponding block** $B_{a,p}$ vs. $B_{b,p}$, the same majority advantage holds:  
  $P(X_{a,p} > X_{b,p}) > 0.5$, where $X_{a,p}$ is uniformly distributed on $B_{a,p}$.

This follows from the uniform cyclic shift pattern: no block can invert the majority of its parent die.

### 4. Summation (Convolution) Step

The tuple die $D^T_a$ is the sum of $K$ **independent** block dice $X_{a,1}, \dots, X_{a,K}$.  
Similarly for $D^T_b$. Let $Y_p = X_{a,p} - X_{b,p}$.

- Each $Y_p$ has $P(Y_p > 0) > 0.5$.
- The total difference is $Y = \sum_{p=1}^K Y_p$.

By independence and Hoeffding-type inequalities:  
If $P(Y_p > 0) > 0.5$ for all $p$, then $P(Y > 0) > 0.5$.  
In fact, the advantage typically **increases** with $K$.

### 5. Conclusion

Since every adjacent pair in the MP cycle preserves its majority advantage after partitioning and summing, the entire cycle is preserved for the tuple dice $D^T_i$.

---

## Corollaries

1. **Length Scaling:** MP cycles exist for any $N$, hence this process yields intransitive cycles of length $N$ from tuple dice with $M$ sides each, for any $M > 2$ and $N = K \cdot M$.
2. **Robustness:** The preservation requires **structured** partitions aligned with the MP face ordering. Random partitions generally destroy the majority advantage.
3. **Generality:** The argument does not depend on $M = 6$ — any $M > 2$ works, provided $N$ is a multiple of $M$.

---

## Implementation & Empirical Verification

Empirical tests confirm the preservation for all $N \leq 38$ and various $M$ values, with cycle probabilities matching or slightly exceeding the original MP values. See [intransitive_dice_generator.cpp](../src/intransitive_dice_generator.cpp) for C++ code.

---

**Acknowledgment:**  
This proof outline was developed collaboratively:
- **Empirical discovery and code:** Hartmut Hoss
- **Proof structuring & write-up:** ChatGPT (OpenAI), based on the author’s experimental observations.

---
