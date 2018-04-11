#include "bee2/core/mem.h"
#include "bee2/math/ec.h"

// cost: 1I+102M+28S+50A
// size: 27*n words
void ecDivpA_8(word *nPs, word const *P, ec_o const *ec, void *stack)
{
qr_o const *qr = ec->f;
size_t const n = qr->n;
word const *a = ec->A;
word const *b = ec->B;
word const *x = P;
word const *y = P+n;
word *__s = ((word*)stack) + ec->deep;
#define s(i) (__s + ((i)*n))
#define nP(i,j) (nPs+2*(i)+j)

qrAdd(s(0), y, y, qr);
qrSqr(s(0), s(0), qr, stack); //  | (2y)²
qrSqr(s(1), s(0), qr, stack); //  | (2y)⁴
qrSqr(s(2), x, qr, stack); //  | x² ← x ²
qrAdd(s(3), s(2), a, qr); //  | x²+a ← x² + a
qrSqr(s(4), s(3), qr, stack); //  | [x²+a]² ← x²+a ²
qrAdd(s(5), s(4), s(4), qr);
qrAdd(s(5), s(4), s(5), qr); //  | 3[x²+a]² ← 3 ⊠ [x²+a]²
qrMul(s(4), b, x, qr, stack); //  | bx ← b * x
qrAdd(s(6), s(4), s(4), qr);
qrAdd(s(6), s(4), s(6), qr); //  | 3bx ← 3 ⊠ bx
qrSqr(s(7), a, qr, stack); //  | a² ← a ²
qrSub(s(6), s(6), s(7), qr); //  | 3bx-a² ← 3bx - a²
qrAdd(s(6), s(6), s(6), qr);
qrAdd(s(6), s(6), s(6), qr); //  | 4[3bx-a²] ← 4 ⊠ 3bx-a²
qrAdd(s(6), s(5), s(6), qr); //  | ξ₃ ← 3[x²+a]² + 4[3bx-a²] | ξ₁ ξ₃ | ξ₁ ξ₃ | ξ₁ ξ₃
qrAdd(s(5), s(2), s(2), qr);
qrAdd(s(5), s(5), s(5), qr);
qrAdd(s(5), s(2), s(5), qr); //  | 5x² ← 5 ⊠ x²
qrSub(s(8), s(5), a, qr); //  | 5x²-a ← 5x² - a
qrMul(s(8), s(4), s(8), qr, stack); //  | bx[5x²-a] ← bx * 5x²-a
qrAdd(s(8), s(8), s(8), qr);
qrAdd(s(8), s(8), s(8), qr); //  | 4bx[5x²-a] ← 4 ⊠ bx[5x²-a]
qrAdd(s(5), s(5), a, qr); //  | 5x²+a ← 5x² + a
qrMul(s(5), s(7), s(5), qr, stack); //  | a²[5x²+a] ← a² * 5x²+a
qrSqr(s(2), s(2), qr, stack); //  | x⁴ ← x² ²
qrAdd(s(7), a, a, qr);
qrAdd(s(7), s(7), s(7), qr); //  | 4a ← 4 ⊠ a
qrAdd(s(7), s(3), s(7), qr); //  | x²+5a ← x²+a + 4a
qrMul(s(7), s(2), s(7), qr, stack); //  | x⁴[x²+5a] ← x⁴ * x²+5a
qrAdd(s(2), b, b, qr);
qrSqr(s(2), s(2), qr, stack);
qrAdd(s(2), s(2), s(2), qr); //  | 8b² ← 8 ⊠ b²
qrAdd(s(8), s(7), s(8), qr); //  | x⁴[x²+5a]+4bx[5x²-a] ← x⁴[x²+5a] + 4bx[5x²-a]
qrAdd(s(2), s(5), s(2), qr); //  | a²[5x²+a]+8b² ← a²[5x²+a] + 8b²
qrSub(s(2), s(8), s(2), qr);
qrAdd(s(2), s(2), s(2), qr); //  | ξ₄ ← 2 ⊠ x⁴[x²+5a]+4bx[5x²-a]-a²[5x²+a]-8b² | ξ₂ ξ₄ | ξ₂ ξ₄   ξ₂² | ξ₂ ξ₄ | ξ₂ ξ₄ | ξ₂ ξ₄
qrSqr(s(8), s(6), qr, stack); //  | ξ₃² | ξ₃² | ξ₃² | ξ₃²
qrMul(s(5), s(6), s(8), qr, stack); //  | ξ₁ ξ₃   ξ₃²
qrMul(s(7), s(1), s(2), qr, stack);
qrSub(s(5), s(7), s(5), qr); //  | ξ₅
qrSqr(s(7), s(2), qr, stack); //  | ξ₄² | ξ₄² | ξ₄² | ξ₄² | ξ₄² | ξ₄²
qrMul(s(3), s(6), s(5), qr, stack); //  | ξ₃ ξ₅ | ξ₃ ξ₅ | ξ₃ ξ₅ | ξ₃ ξ₅ | ξ₃ ξ₅ | ξ₃ ξ₅
qrMul(s(6), s(6), s(7), qr, stack);
qrSub(s(6), s(3), s(6), qr); //  | ξ₆
qrMul(s(4), s(3), s(8), qr, stack); //  | ξ₃ ξ₅   ξ₃²
qrMul(s(9), s(2), s(7), qr, stack); //  | ξ₂ ξ₄   ξ₄²
qrMul(s(9), s(1), s(9), qr, stack);
qrSub(s(9), s(4), s(9), qr); //  | ξ₇
qrSqr(s(4), s(5), qr, stack); //  | ξ₅² | ξ₅² | ξ₅² | ξ₅² | ξ₅²
qrMul(s(10), s(2), s(6), qr, stack); //  | ξ₄ ξ₆ | ξ₄ ξ₆ | ξ₄ ξ₆ | ξ₄ ξ₆ | ξ₄ ξ₆
qrMul(s(11), s(10), s(8), qr, stack);
qrMul(s(12), s(2), s(4), qr, stack);
qrSub(s(12), s(11), s(12), qr); //  | ξ₈
qrMul(s(11), s(10), s(7), qr, stack); //  | ξ₄ ξ₆   ξ₄²
qrMul(s(13), s(3), s(4), qr, stack); //  | ξ₃ ξ₅   ξ₅²
qrMul(s(11), s(1), s(11), qr, stack);
qrSub(s(13), s(11), s(13), qr); //  | ξ₉
qrSqr(s(11), s(6), qr, stack); //  | ξ₆² | ξ₆² | ξ₆² | ξ₆² | ξ₆² | ξ₆²
qrMul(s(5), s(5), s(9), qr, stack); //  | ξ₅ ξ₇ | ξ₅ ξ₇ | ξ₅ ξ₇ | ξ₅ ξ₇ | ξ₅ ξ₇ | ξ₅ ξ₇
qrMul(s(7), s(5), s(7), qr, stack);
qrMul(s(3), s(3), s(11), qr, stack);
qrSub(s(3), s(7), s(3), qr); //  | ξ₁₀
qrMul(s(7), s(5), s(4), qr, stack); //  | ξ₅ ξ₇   ξ₅²
qrMul(s(14), s(10), s(11), qr, stack); //  | ξ₄ ξ₆   ξ₆²
qrMul(s(14), s(1), s(14), qr, stack);
qrSub(s(14), s(7), s(14), qr); //  | ξ₁₁
qrSqr(s(7), s(9), qr, stack); //  | ξ₇² | ξ₇² | ξ₇² | ξ₇² | ξ₇²
qrMul(s(15), s(6), s(12), qr, stack); //  | ξ₆ ξ₈ | ξ₆ ξ₈ | ξ₆ ξ₈ | ξ₆ ξ₈ | ξ₆ ξ₈
qrMul(s(16), s(15), s(4), qr, stack);
qrMul(s(17), s(10), s(7), qr, stack);
qrSub(s(17), s(16), s(17), qr); //  | ξ₁₂
qrMul(s(16), s(15), s(11), qr, stack); //  | ξ₆ ξ₈   ξ₆²
qrMul(s(18), s(5), s(7), qr, stack); //  | ξ₅ ξ₇   ξ₇²
qrMul(s(16), s(1), s(16), qr, stack);
qrSub(s(18), s(16), s(18), qr); //  | ξ₁₃
qrSqr(s(16), s(12), qr, stack); //  | ξ₈² | ξ₈² | ξ₈² | ξ₈² | ξ₈²
qrMul(s(9), s(9), s(13), qr, stack); //  | ξ₇ ξ₉ | ξ₇ ξ₉ | ξ₇ ξ₉ | ξ₇ ξ₉ | ξ₇ ξ₉
qrMul(s(11), s(9), s(11), qr, stack);
qrMul(s(5), s(5), s(16), qr, stack);
qrSub(s(5), s(11), s(5), qr); //  | ξ₁₄
qrMul(s(11), s(9), s(7), qr, stack); //  | ξ₇ ξ₉   ξ₇²
qrMul(s(19), s(15), s(16), qr, stack); //  | ξ₆ ξ₈   ξ₈²
qrMul(s(19), s(1), s(19), qr, stack);
qrSub(s(19), s(11), s(19), qr); //  | ξ₁₅
qrSqr(s(11), s(13), qr, stack); //  | ξ₉² | ξ₉² | ξ₉²
qrMul(s(12), s(12), s(3), qr, stack); //  | ξ₈ ξ₁₀ | ξ₈ ξ₁₀ | ξ₈ ξ₁₀
qrMul(s(20), s(12), s(7), qr, stack);
qrMul(s(21), s(15), s(11), qr, stack);
qrSub(s(21), s(20), s(21), qr); //  | ξ₁₆
qrMul(s(20), s(12), s(16), qr, stack); //  | ξ₈ ξ₁₀   ξ₈²
qrMul(s(22), s(9), s(11), qr, stack); //  | ξ₇ ξ₉   ξ₉²
qrMul(s(20), s(1), s(20), qr, stack);
qrSub(s(22), s(20), s(22), qr); //  | ξ₁₇
qrMul(s(2), s(0), s(2), qr, stack); //  | [3]Pjx
qrMul(s(6), y, s(6), qr, stack); //  | [3]Pjy
qrMul(s(10), s(0), s(10), qr, stack); //  | [5]Pjx
qrMul(s(20), y, s(3), qr, stack); //  | [5]Pjy
qrMul(s(15), s(0), s(15), qr, stack); //  | [7]Pjx
qrMul(s(1), y, s(5), qr, stack); //  | [7]Pjy
qrSqr(s(23), s(3), qr, stack); //  | ξ₁₀² | ξ₁₀²
qrMul(s(12), s(0), s(12), qr, stack); //  | [9]Pjx
qrMul(s(13), s(13), s(14), qr, stack); //  | ξ₉ ξ₁₁ | ξ₉ ξ₁₁
qrMul(s(16), s(13), s(16), qr, stack);
qrMul(s(9), s(9), s(23), qr, stack);
qrSub(s(9), s(16), s(9), qr);
qrMul(s(9), y, s(9), qr, stack); //  | [9]Pjy
qrSqr(s(16), s(14), qr, stack); //  | ξ₁₁²
qrSqr(s(24), s(17), qr, stack); //  | ξ₁₂² | ξ₁₂²
qrMul(s(3), s(3), s(17), qr, stack); //  | ξ₁₀ ξ₁₂
qrMul(s(3), s(0), s(3), qr, stack); //  | [11]Pjx
qrMul(s(14), s(14), s(18), qr, stack); //  | ξ₁₁ ξ₁₃ | ξ₁₁ ξ₁₃
qrMul(s(23), s(14), s(23), qr, stack);
qrMul(s(13), s(13), s(24), qr, stack);
qrSub(s(13), s(23), s(13), qr);
qrMul(s(13), y, s(13), qr, stack); //  | [11]Pjy
qrSqr(s(23), s(18), qr, stack); //  | ξ₁₃²
qrSqr(s(25), s(5), qr, stack); //  | ξ₁₄² | ξ₁₄²
qrMul(s(17), s(17), s(5), qr, stack); //  | ξ₁₂ ξ₁₄
qrMul(s(17), s(0), s(17), qr, stack); //  | [13]Pjx
qrMul(s(18), s(18), s(19), qr, stack); //  | ξ₁₃ ξ₁₅ | ξ₁₃ ξ₁₅
qrMul(s(24), s(18), s(24), qr, stack);
qrMul(s(14), s(14), s(25), qr, stack);
qrSub(s(14), s(24), s(14), qr);
qrMul(s(14), y, s(14), qr, stack); //  | [13]Pjy
qrSqr(s(24), s(19), qr, stack); //  | ξ₁₅²
qrSqr(s(26), s(21), qr, stack); //  | ξ₁₆²
qrMul(s(21), s(5), s(21), qr, stack); //  | ξ₁₄ ξ₁₆
qrMul(s(21), s(0), s(21), qr, stack); //  | [15]Pjx
qrMul(s(22), s(19), s(22), qr, stack); //  | ξ₁₅ ξ₁₇
qrMul(s(25), s(22), s(25), qr, stack);
qrMul(s(26), s(18), s(26), qr, stack);
qrSub(s(26), s(25), s(26), qr);
qrMul(s(26), y, s(26), qr, stack); //  | [15]Pjy
qrMul(s(25), s(23), s(24), qr, stack);
qrMul(s(18), s(16), s(25), qr, stack);
qrMul(s(22), s(11), s(18), qr, stack);
qrMul(s(19), s(7), s(22), qr, stack);
qrMul(s(0), s(4), s(19), qr, stack);
qrMul(s(5), s(8), s(0), qr, stack);
qrInv(s(5), s(5), qr, stack);
qrMul(s(0), s(5), s(0), qr, stack);
qrMul(s(8), s(5), s(8), qr, stack);
qrMul(s(19), s(8), s(19), qr, stack);
qrMul(s(4), s(8), s(4), qr, stack);
qrMul(s(22), s(4), s(22), qr, stack);
qrMul(s(7), s(4), s(7), qr, stack);
qrMul(s(18), s(7), s(18), qr, stack);
qrMul(s(11), s(7), s(11), qr, stack);
qrMul(s(25), s(11), s(25), qr, stack);
qrMul(s(16), s(11), s(16), qr, stack);
qrMul(s(24), s(16), s(24), qr, stack);
qrMul(s(23), s(16), s(23), qr, stack);
qrSqr(s(16), s(23), qr, stack);
qrMul(s(23), s(21), s(23), qr, stack);
qrSub(nP(6,0), x, s(23), qr); //  | [15]Px
qrMul(nP(6,1), s(26), s(16), qr, stack); //  | [15]Py
qrSqr(s(21), s(24), qr, stack);
qrMul(s(24), s(17), s(24), qr, stack);
qrSub(nP(5,0), x, s(24), qr); //  | [13]Px
qrMul(nP(5,1), s(14), s(21), qr, stack); //  | [13]Py
qrSqr(s(17), s(25), qr, stack);
qrMul(s(25), s(3), s(25), qr, stack);
qrSub(nP(4,0), x, s(25), qr); //  | [11]Px
qrMul(nP(4,1), s(13), s(17), qr, stack); //  | [11]Py
qrSqr(s(3), s(18), qr, stack);
qrMul(s(18), s(12), s(18), qr, stack);
qrSub(nP(3,0), x, s(18), qr); //  | [9]Px
qrMul(nP(3,1), s(9), s(3), qr, stack); //  | [9]Py
qrSqr(s(12), s(22), qr, stack);
qrMul(s(22), s(15), s(22), qr, stack);
qrSub(nP(2,0), x, s(22), qr); //  | [7]Px
qrMul(nP(2,1), s(1), s(12), qr, stack); //  | [7]Py
qrSqr(s(15), s(19), qr, stack);
qrMul(s(19), s(10), s(19), qr, stack);
qrSub(nP(1,0), x, s(19), qr); //  | [5]Px
qrMul(nP(1,1), s(20), s(15), qr, stack); //  | [5]Py
qrSqr(s(10), s(0), qr, stack);
qrMul(s(0), s(2), s(0), qr, stack);
qrSub(nP(0,0), x, s(0), qr); //  | [3]Px
qrMul(nP(0,1), s(6), s(10), qr, stack); //  | [3]Py

#undef s
#undef nP
}
size_t ecDivpA_8_deep(size_t n, size_t ec_deep)
{
  return 27*n + ec_deep;
}


