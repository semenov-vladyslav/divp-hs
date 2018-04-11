#include "bee2/core/mem.h"
#include "bee2/math/ec.h"

// cost: 0I+31M+13S+36A
// size: 19*n words
void ecDivpJ_4(word *nPs, word const *P, ec_o const *ec, void *stack)
{
qr_o const *qr = ec->f;
size_t const n = qr->n;
word const *a = ec->A;
word const *b = ec->B;
word const *x = P;
word const *y = P+n;
word *__s = ((word*)stack) + ec->deep;
#define s(i) (__s + ((i)*n))
#define nP(i,j) (nPs+3*(i)+j)

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
qrAdd(nP(0,2), s(5), s(6), qr); //  | ξ₃ ← 3[x²+a]² + 4[3bx-a²] | ξ₁ ξ₃
qrAdd(s(8), s(2), s(2), qr);
qrAdd(s(8), s(8), s(8), qr);
qrAdd(s(8), s(2), s(8), qr); //  | 5x² ← 5 ⊠ x²
qrSub(s(9), s(8), a, qr); //  | 5x²-a ← 5x² - a
qrMul(s(9), s(4), s(9), qr, stack); //  | bx[5x²-a] ← bx * 5x²-a
qrAdd(s(9), s(9), s(9), qr);
qrAdd(s(9), s(9), s(9), qr); //  | 4bx[5x²-a] ← 4 ⊠ bx[5x²-a]
qrAdd(s(8), s(8), a, qr); //  | 5x²+a ← 5x² + a
qrMul(s(8), s(7), s(8), qr, stack); //  | a²[5x²+a] ← a² * 5x²+a
qrSqr(s(2), s(2), qr, stack); //  | x⁴ ← x² ²
qrAdd(s(7), a, a, qr);
qrAdd(s(7), s(7), s(7), qr); //  | 4a ← 4 ⊠ a
qrAdd(s(7), s(3), s(7), qr); //  | x²+5a ← x²+a + 4a
qrMul(s(7), s(2), s(7), qr, stack); //  | x⁴[x²+5a] ← x⁴ * x²+5a
qrAdd(s(2), b, b, qr);
qrSqr(s(2), s(2), qr, stack);
qrAdd(s(2), s(2), s(2), qr); //  | 8b² ← 8 ⊠ b²
qrAdd(s(9), s(7), s(9), qr); //  | x⁴[x²+5a]+4bx[5x²-a] ← x⁴[x²+5a] + 4bx[5x²-a]
qrAdd(s(2), s(8), s(2), qr); //  | a²[5x²+a]+8b² ← a²[5x²+a] + 8b²
qrSub(s(2), s(9), s(2), qr);
qrAdd(s(2), s(2), s(2), qr); //  | ξ₄ ← 2 ⊠ x⁴[x²+5a]+4bx[5x²-a]-a²[5x²+a]-8b² | ξ₂ ξ₄ | ξ₂ ξ₄   ξ₂² | ξ₂ ξ₄ | ξ₂ ξ₄
qrSqr(s(9), nP(0,2), qr, stack); //  | ξ₃² | ξ₃² | ξ₃² | ξ₃²
qrMul(s(8), nP(0,2), s(9), qr, stack); //  | ξ₁ ξ₃   ξ₃²
qrMul(s(7), s(1), s(2), qr, stack);
qrSub(nP(1,2), s(7), s(8), qr); //  | ξ₅ | ξ₅ ξ₂² | ξ₅ ξ₂²
qrSqr(s(3), s(2), qr, stack); //  | ξ₄² | ξ₁ ξ₄² | ξ₄² | ξ₄² | ξ₄² | ξ₄² | ξ₁ ξ₄²
qrSub(s(4), nP(1,2), s(3), qr);
qrMul(s(10), nP(0,2), s(4), qr, stack); //  | ξ₆
qrMul(s(11), nP(0,2), nP(1,2), qr, stack); //  | ξ₃ ξ₅ | ξ₃ ξ₅
qrMul(s(12), s(11), s(9), qr, stack); //  | ξ₃ ξ₅   ξ₃²
qrMul(s(13), s(2), s(3), qr, stack); //  | ξ₂ ξ₄   ξ₄²
qrMul(s(13), s(1), s(13), qr, stack);
qrSub(nP(2,2), s(12), s(13), qr); //  | ξ₇
qrSqr(s(14), nP(1,2), qr, stack); //  | ξ₅² | ξ₂ ξ₅² | ξ₅² | ξ₅²
qrMul(s(15), s(10), s(9), qr, stack); //  | ξ₆ ξ₃²
qrSub(s(15), s(15), s(14), qr);
qrMul(s(15), s(2), s(15), qr, stack); //  | ξ₈
qrMul(s(16), s(2), s(10), qr, stack); //  | ξ₄ ξ₆ | ξ₄ ξ₆
qrMul(s(17), s(16), s(3), qr, stack); //  | ξ₄ ξ₆   ξ₄²
qrMul(s(11), s(11), s(14), qr, stack); //  | ξ₃ ξ₅   ξ₅²
qrMul(s(17), s(1), s(17), qr, stack);
qrSub(s(11), s(17), s(11), qr); //  | ξ₉
qrSqr(s(17), s(10), qr, stack); //  | ξ₆² | ξ₆²
qrSqr(s(1), nP(2,2), qr, stack); //  | ξ₇²
qrSqr(s(18), s(15), qr, stack); //  | ξ₈²
qrMul(s(15), s(10), s(15), qr, stack); //  | ξ₆ ξ₈
qrMul(s(1), x, s(1), qr, stack);
qrMul(s(15), s(0), s(15), qr, stack);
qrSub(nP(2,0), s(1), s(15), qr); //  | [7]Px
qrMul(s(11), s(11), s(17), qr, stack); //  | ξ₉ ξ₆²
qrMul(s(18), nP(1,2), s(18), qr, stack); //  | ξ₅ ξ₈²
qrSub(s(18), s(11), s(18), qr);
qrMul(nP(2,1), y, s(18), qr, stack); //  | [7]Py
qrMul(s(14), x, s(14), qr, stack);
qrMul(s(16), s(0), s(16), qr, stack);
qrSub(nP(1,0), s(14), s(16), qr); //  | [5]Px
qrMul(s(3), nP(2,2), s(3), qr, stack); //  | ξ₇ ξ₄²
qrMul(s(17), nP(0,2), s(17), qr, stack); //  | ξ₃ ξ₆²
qrSub(s(17), s(3), s(17), qr);
qrMul(nP(1,1), y, s(17), qr, stack); //  | [5]Py
qrMul(s(9), x, s(9), qr, stack);
qrMul(s(2), s(0), s(2), qr, stack);
qrSub(nP(0,0), s(9), s(2), qr); //  | [3]Px
qrMul(nP(0,1), y, s(4), qr, stack); //  | [3]Py

#undef s
#undef nP
}
size_t ecDivpJ_4_deep(size_t n, size_t ec_deep)
{
  return 19*n + ec_deep;
}


