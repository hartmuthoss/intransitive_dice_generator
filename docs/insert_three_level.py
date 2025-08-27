import copy
import math
import matplotlib.pyplot as plt

def cdf(X, t):
    return sum(x <= t for x in X) / len(X)

def insert_three_level_general(A, B, show_plots = False):
    pAB = probability_to_beat(A, B)
    if pAB <= 0.5:
        raise AssertionError("P(A>B) must be > 1/2")
    M = len(A)
    vals = sorted(set(A + B))
    # candidates for threshold t: mean value between two faces
    candidates = [(vals[i] + vals[i+1]) / 2 for i in range(len(vals)-1)]

    def FA(t): return cdf(A, t) # return sum(a <= t for a in A) / M
    def FB(t): return cdf(B, t) # return sum(b <= t for b in B) / M

    C_final = None
    for t in candidates:
        if C_final is None and FA(t) < FB(t):
            pA = 1 - FA(t)
            pB = FB(t)
            for mM in range(1, M):
                Lmin = math.floor(M/2 - mM*pA) + 1
                Lmax = math.floor(M/2 + mM*(1 - pB))
                Lmin = max(0, Lmin)
                Lmax = min(M - mM, Lmax)
                if Lmin <= Lmax:
                    mL = Lmin
                    mH = M - mM - mL
                    eps = 1e-6
                    L = min(min(A), min(B)) - 1.0
                    M0 = t + eps
                    H = max(max(A), max(B)) + 1.0
                    C = [L]*mL + [M0]*mM + [H]*mH
                    pAC = probability_to_beat(A, C)
                    pCB = probability_to_beat(C, B)
                    if pAC > 0.5 and pCB > 0.5: # i.e. A > C > B
                        C_final = C
                        break
    if C_final is None:
        raise AssertionError("insert_three_level_general failed")

    # debugging
    if show_plots:
        plot_three_level(A, B, None, pAB, pAC, 0, vals[0] - 1, vals[-1] + 1, 0, 0, 0, 0, 0, 0)
        plot_three_level(A, B, C, pAB, pAC, pCB, vals[0] - 1, vals[-1] + 1, L, mL, M0, mM, H, mH)
        plt.show()
    
    return C_final

def plot_three_level(A, B, C, pAB, pAC, pCB, t_min, t_max, L, mL, M0, mM, H, mH):
        print(f"3-level insertion:")
        if C is not None:
            print(f"A = {A}\nB = {B}\nC = {C}\nP(A>B) = {pAB:.3f}, P(A>C) = {pAC:.3f}, P(C>B) = {pCB:.3f}\nL = {L}, mL = {mL}, M0 = {M0:.3f}, mM = {mM}, H = {H}, mH = {mH}")
        else:
            print(f"A = {A}\nB = {B}\nP(A>B) = {pAB:.3f}")
        plot_t_values = [(t / 100) for t in range(100 * t_min, 100 * t_max)]
        plot_fat_values = [cdf(A, t) for t in plot_t_values]
        plot_fbt_values = [cdf(B, t) for t in plot_t_values]
        plot_pat_values = [1 - cdf(A, t) for t in plot_t_values]
        plot_pbt_values = [1 - cdf(B, t) for t in plot_t_values]
        fig, _ = plt.subplots(1, 2)
        plt.subplot(1, 2, 1)
        plt.plot(plot_t_values, plot_pat_values, label="P(A>t)")
        plt.plot(plot_t_values, plot_pbt_values, label="P(B>t)")
        plt.legend(loc="upper right")
        plt.xlabel("Threshold t")
        plt.ylabel("Probabilities P")
        plt.subplot(1, 2, 2)
        plt.plot(plot_t_values, plot_fat_values, label="P(A<=t)")
        plt.plot(plot_t_values, plot_fbt_values, label="P(B<=t)")
        plt.legend(loc="upper left")
        plt.xlabel("Threshold t")
        plt.ylabel("Probabilities P")
        msg = f"3-level insertion:\nA = {A}\nB = {B}\nP(A>B) = {pAB:.3f}"
        if C is not None:
            plt.subplot(1, 2, 1)
            plot_fct_values = [cdf(C, t) for t in plot_t_values]
            plot_pct_values = [1 - cdf(C, t) for t in plot_t_values]
            plt.plot(plot_t_values, plot_pct_values, label="P(C>t)")
            plt.subplot(1, 2, 2)
            plt.plot(plot_t_values, plot_fct_values, label="P(C<=t)")        
            msg = f"3-level insertion:\nA = {A}\nB = {B}\nC = {[round(c,2) for c in C]}\nP(A>B) = {pAB:.3f}, P(A>C) = {pAC:.3f}, P(C>B) = {pCB:.3f}"
        fig.suptitle(msg, x=0.1, horizontalalignment="left", fontsize=10)
        plt.tight_layout()

def mul_add(X, f, c):
    return [ f * x + c for x in X ]

def cast_int(X):
    return [ int(x) for x in X ]

def normalize(A, B, C, f):
    A, B, C = mul_add(A, f, 0), mul_add(B, f, 0), mul_add(C, f, 0)
    min_val = min(min(A), min(B), min(C))
    A, B, C = mul_add(A, 1, 1 - min_val), mul_add(B, 1, 1 - min_val), mul_add(C, 1, 1 - min_val)
    return cast_int(A), cast_int(B), cast_int(C)

def probability_to_beat(A, B):
    beat_cnt = 0
    for a in A:
        beat_cnt += sum(1 for b in B if a > b)
    return beat_cnt / (len(A) * len(B))

def three_level_unittest(A, B):
    # 3-level insertion
    C = insert_three_level_general(A, B)
    # C = insert_three_level_general(A, B, show_plots = True)
    pAB = probability_to_beat(A, B)
    pAC = probability_to_beat(A, C)
    pCB = probability_to_beat(C, B)
    print(f"A = {A}\nB = {B}\nC = {C}")
    print(f"P(A>B) = {pAB:.3f}, P(A>C) = {pAC:.3f}, P(C>B) = {pCB:.3f}")
    assert(pAB > 0.5 and pAC > 0.5 and pCB > 0.5)
    # Normalization and cast to integer values
    D, E, F = normalize(A, B, C, 2)
    pDE = probability_to_beat(D, E)
    pDF = probability_to_beat(D, F)
    pFE = probability_to_beat(F, E)
    print(f"D = {D}\nE = {E}\nF = {F}")
    print(f"P(D>E) = {pDE:.3f}, P(D>F) = {pDF:.3f}, P(F>E) = {pFE:.3f}")
    assert(pDE > 0.5 and pDF > 0.5 and pFE > 0.5)
    print(f"")

def three_level_unittests():
    three_level_unittest(A = [ 2, 19, 22, 23, 29, 30 ], B = [ 9, 16, 16, 19, 31, 38 ])
    three_level_unittest(A = [ 6, 11, 16, 21, 26, 31 ], B = [ 5, 10, 15, 20, 25, 36 ])
    three_level_unittest(A = [ 5, 10, 15, 20, 25, 36 ], B = [ 4,  9, 14, 19, 30, 35 ])
    three_level_unittest(A = [ 4,  9, 14, 19, 30, 35 ], B = [ 3,  8, 13, 24, 29, 34 ])
    three_level_unittest(A = [ 3,  8, 13, 24, 29, 34 ], B = [ 2,  7, 18, 23, 28, 33 ])
    three_level_unittest(A = [ 2,  7, 18, 23, 28, 33 ], B = [ 1, 12, 17, 22, 27, 32 ])
    three_level_unittest(A = [ 1, 12, 17, 22, 27, 32 ], B = [ 6, 11, 16, 21, 26, 31 ])
    three_level_unittest(A = [ 4, 4, 4, 4, 4 ], B = [ 3, 3, 3, 3, 8 ])
    three_level_unittest(A = [ 3, 3, 3, 3, 8 ], B = [ 2, 2, 2, 7, 7 ])
    three_level_unittest(A = [ 2, 2, 2, 7, 7 ], B = [ 1, 1, 6, 6, 6 ])
    three_level_unittest(A = [ 1, 1, 6, 6, 6 ], B = [ 0, 5, 5, 5, 5 ])
    three_level_unittest(A = [ 0, 5, 5, 5, 5 ], B = [ 4, 4, 4, 4, 4 ])

if __name__ == '__main__':
    three_level_unittests()
