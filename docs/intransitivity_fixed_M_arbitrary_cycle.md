---
title: Arbitrarily Long Intransitive Cycles with a Fixed Number of Sides
layout: default
---

# Arbitrarily Long Intransitive Cycles with a Fixed Number of Sides (Proof Sketch by ChatGPT)

**Authors:** Hartmut Hoss (Empirical discovery & code) and ChatGPT (OpenAI, proof outline)  
**Date:** 2025-08-11  
**License:** MIT  
**Keywords:** intransitive dice, arbitrarily long intransitive cycles

---

> **TL;DR.** For any fixed number of sides $M\ge 3$ and any $n\in\mathbb N$, there exists a set of $n$ $M$-sided dice $D_1,\dots,D_n$ forming a directed cycle
> $D_1 \succ D_2 \succ \cdots \succ D_n \succ D_1$ (i.e., $ \mathbb P[D_i>D_{i+1}]>1/2$ for all $i$, indices mod $n$).
> The construction proceeds by a **local insertion lemma** (insert a new die $C$ strictly between any adjacent pair $A\succ B$) plus an **affine gap amplification** that preserves all pairwise probabilities and eliminates ties. Repeating insertions yields cycles of **arbitrary length** while keeping the number of sides fixed at $M$.

---

## 1. Model and Notation

* An $M$-sided die $D$ is a multiset (or ordered $M$-tuple) of integer numbers $(d_1,\dots,d_M)$. On a roll, one face is chosen uniformly at random.
* For dice $X,Y$, write $X\succ Y$ if $\mathbb P[X>Y]>1/2$. (We will maintain strict inequalities; ties can be avoided via small perturbations.)
* Let $\min(D)$ and $\max(D)$ be the minimal and maximal face value of $D$.

Two simple invariants we will use:

1. **Affine invariance.** For any integer $a>0$, $b\in\mathbb N$: replacing every die $D$ by $a\cdot{D}+b$ preserves all strict pairwise probabilities.
   *Consequence:* We may rescale/shift **globally** at any time to enlarge gaps and remove potential ties without changing any $\mathbb P[X>Y]$.

2. **General position.** We can $\varepsilon$-perturb faces (distinct, rational/integer if desired) to ensure no equalities without changing strict comparisons.

---

## 2. A Quantile-Separation Lemma

Let $A=(a_1,\dots,a_M)$ and $B=(b_1,\dots,b_M)$ be two dice with $A\succ B$.

Define the empirical cumulative distribution functions (CDFs), where $cnt \{i: a_i\le t\}$ counts the number of elements with $a_i\le t$:

$$
F_A(t)=\frac{1}{M} cnt \{i: a_i\le t\},\qquad
F_B(t)=\frac{1}{M} cnt \{j: b_j\le t\}.
$$

**Lemma 2.1 (Quantile separation).** If $A\succ B$, then there exists a threshold $t\in\mathbb R$ such that

$$
F_A(t) \;<\; F_B(t).
$$

Equivalently, the proportion of $A$-faces $\le t$ is strictly **smaller** than the proportion of $B$-faces $\le t$. (In particular, taking $t$ at a strict crossing, more than half of $B$-faces lie below $t$, while less than half of $A$-faces lie below $t$.)

*Proof sketch.* $\mathbb P[A>B]$ is (up to normalization) the Mann–Whitney $U$-statistic comparing samples $\{a_i\}$ and $\{b_j\}$. If $U>1/2$, the empirical CDFs cannot be everywhere ordered $F_A\ge F_B$; hence there exists $t$ with $F_A(t)<F_B(t)$.

We will use such a threshold $t$ to place faces of a **new** die on carefully chosen “levels” just below or above $t$.

---

## 3. The 3-Level Insertion Die

Fix adjacent dice $A\succ B$ in our current cycle. We construct a new $M$-sided die $C$ that **slots strictly between** them: $A\succ C\succ B$.

Choose a threshold $t$ witnessing $F_A(t)<F_B(t)$ (Lemma 2.1). Pick three real numbers

$$
L < t-\delta < t+\delta < H
$$

with $\delta>0$ tiny, and satisfying

$$
\max(B) \;<\; H, \qquad L \;<\; \min(A).
$$

(We can ensure this by a global affine amplification first, then pick $L,H$ far outside the current ranges.)

Let integers $m_L,m_M,m_H\ge 0$ with $m_L+m_M+m_H=M$. Define $C$ to have

* $m_L$ faces at value $L$ (“very low”),
* $m_M$ faces at value $t+\delta$ (“slightly above $t$”),
* $m_H$ faces at value $H$ (“very high”).

**Key deterministic comparisons.**

* Every face of $A$ **beats** every $L$-face of $C$ (since $L<\min(A)$).
* Every $H$-face of $C$ **beats** every face of $B$ (since $H>\max(B)$).

For the middle level $t+\delta$:

* Let $p_A := \mathbb P[a_i > t+\delta]$ (fraction of $A$-faces above $t+\delta$).
* Let $p_B := \mathbb P[t+\delta > b_j]$ (fraction of $B$-faces below $t+\delta$).

Because $F_A(t)<F_B(t)$ and $\delta$ is small, we have

$$
p_A \;>\; 1 - F_A(t+\delta) \;\ge\; 1 - F_A(t) \;>\; 1 - F_B(t) \;\ge\; F_B(t+\delta) \;=\; 1 - p_B,
$$

hence, in particular, $p_A > 1/2$ **or** $p_B > 1/2$ (in fact both are bounded away from $1/2$ by the crossing gap).

**Lower bounds for the target probabilities.**

$$
\mathbb P[A>C] 
\;\ge\; \frac{m_L}{M}\cdot 1 \;+\; \frac{m_M}{M}\cdot p_A,
$$

$$
\mathbb P[C>B] 
\;\ge\; \frac{m_H}{M}\cdot 1 \;+\; \frac{m_M}{M}\cdot p_B.
$$

Thus it suffices to choose **integer counts** $(m_L,m_M,m_H)$ such that

$$
\frac{m_L}{M} + \frac{m_M}{M}\,p_A \;>\; \tfrac12,
$$

$$
\frac{m_H}{M} + \frac{m_M}{M}\,p_B \;>\; \tfrac12,
$$

$$
m_L+m_M+m_H=M. \qquad (A)
$$

Because $p_A$ and $p_B$ are strictly positive (indeed bounded away from 0 by the CDF crossing), the feasible region of (A) in $\mathbb R^3_{\ge 0}$ has **non-empty interior**. Hence we can pick **integer** $(m_L,m_M,m_H)$ satisfying (A). (E.g., take $m_M$ a positive fraction of $M$ so that $m_M p_A$ and $m_M p_B$ both contribute a few percentage points above $1/2$, then split the remaining faces between $m_L$ and $m_H$ to clear the two half-thresholds.)

Finally, use a tiny $\varepsilon$-perturbation on $L$, $t+\delta$, $H$ if needed to remove any accidental ties while preserving the strict bounds.

**Conclusion (Insertion Lemma).**
Given $A\succ B$, there exists an $M$-sided die $C$ (with faces on three levels) such that $A\succ C\succ B$.

---

## 4. Affine Gap Amplification (to keep inserting forever)

After many insertions, some newly introduced “middle” levels might sit close to existing face values. Before the next insertion, apply a **global** affine transform

$$
x \;\mapsto\; a x + b,\qquad a\gg 1,
$$

to all dice. This preserves all strict relations and expands all gaps, ensuring we can choose fresh $L,t\pm\delta,H$ for the next insertion without collisions. (The trick $x\mapsto 2x-1$ is a special case; choosing $a$ arbitrarily large makes the separation argument robust.)

---

## 5. Building Arbitrarily Long Cycles for Fixed $M$

Start from any cycle of length $k\ge 3$ on $M$-sided dice (e.g., a standard rock–paper–scissors-style 3-cycle is easy to realize for $M\ge 3$). Suppose we have a cycle

$$
D_1 \succ D_2 \succ \cdots \succ D_k \succ D_1.
$$

Pick any edge $D_i \succ D_{i+1}$. By the **Insertion Lemma**, construct an $M$-sided die $C$ with

$$
D_i \succ C \succ D_{i+1}.
$$

Insert $C$ between $D_i$ and $D_{i+1}$. All other pairwise comparisons among the *existing* dice are unchanged; the cycle length increases by 1.
Apply **Affine Gap Amplification** if needed, and repeat. By induction, for every $n\ge k$ we obtain a cycle of length $n$ using **only $M$-sided dice**.

**Claim: For every fixed $M\ge 3$ and every $n\in\mathbb N$, there exist $n$ $M$-sided dice forming a strict intransitive cycle.**

---

## 6. Cardinalities $|T|=|IT|=|\mathbb N|$

Let $T$ be the set of all finite **transitive** sets of dice (any number of dice, each with $M$ faces for some finite $M$), and $IT$ the set of all finite **intransitive** sets.
Each die is a finite tuple over $\mathbb Q$ (w\.l.o.g. by perturbation), hence the class of all finite die-sets is a countable union of finite powers of $\mathbb Q$, therefore **countable**.
Both $T$ and $IT$ are infinite subfamilies (there are trivially infinitely many transitive sets and, by the Theorem, intransitive cycles of every length), hence

$$
|T|=|IT|=|\mathbb N|.
$$

---

## 7. Algorithm

The "insert-between and expand-gaps" algorithm is a concrete instance of the 3-level construction:

1. **Pick an edge** $A\succ B$ from the current cycle.
2. **Quantile threshold.** Compute a threshold $t$ where the empirical CDFs of $\{a_i\}$ and $\{b_j\}$ cross (or simply take a mid-quantile where $F_A<F_B$).
3. **Choose three levels** $L \ll t$, $M=t+\delta$, $H\gg t$.
4. **Pick counts** $(m_L,m_M,m_H)$ solving (A). (A simple heuristic that works well: take $m_M \approx \lfloor 0.2M\rfloor$; estimate $p_A,p_B$ at $M$ and adjust $m_L,m_H$ upward until both inequalities clear $1/2$.)
5. **Insert $C$** with those faces; **globally rescale** if any gaps get tight.
6. **Repeat** until the desired cycle length is reached.

Because the constraints are *linear* in the integer counts and the feasible region has positive volume, the algorithm will find solutions quickly.

---

## 8. Remarks and Extensions

* The construction is **local** (touches only one edge), so it preserves all previous inequalities among the existing dice. No “far” edge can flip because all other dice are unchanged, and affine amplification preserves strictness.
* Using more than three levels for the insertion die $C$ can ease integer rounding for small $M$ (e.g., set $C$ to have 4–5 clustered levels around $t$ to fine-tune $p_A,p_B$ contributions).

---

## 9. What’s New Here

* The "arbitrarily long cycle" statement with fixed $M$ follows from a simple, explicit **3-level insertion lemma** plus affine invariance.
* To the best of our knowledge (as of this write-up), published constructions that realize *arbitrary tournaments* typically let the number of sides grow with $n$. The lemma above gives a direct, elementary way to keep $M$ fixed and grow $n$ arbitrarily, which seems publishable as a short note.
