#include "bee2/core/mem.h"
#include "bee2/math/ec.h"

// cost: 0I+721M+133S+246A
// size: 259*n words
void ecDivpJ_64(word *nPs, word const *P, ec_o const *ec, void *stack)
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
qrSqr(s(3), s(2), qr, stack); //  | ξ₄² | ξ₁ ξ₄² | ξ₄² | ξ₄² | ξ₄² | ξ₄² | ξ₄² | ξ₁ ξ₄²
qrSub(s(4), nP(1,2), s(3), qr);
qrMul(s(10), nP(0,2), s(4), qr, stack); //  | ξ₆
qrMul(s(11), nP(0,2), nP(1,2), qr, stack); //  | ξ₃ ξ₅ | ξ₃ ξ₅
qrMul(s(12), s(11), s(9), qr, stack); //  | ξ₃ ξ₅   ξ₃²
qrMul(s(13), s(2), s(3), qr, stack); //  | ξ₂ ξ₄   ξ₄²
qrMul(s(13), s(1), s(13), qr, stack);
qrSub(nP(2,2), s(12), s(13), qr); //  | ξ₇
qrSqr(s(14), nP(1,2), qr, stack); //  | ξ₅² | ξ₂ ξ₅² | ξ₅² | ξ₅² | ξ₅² | ξ₅²
qrMul(s(15), s(10), s(9), qr, stack); //  | ξ₆ ξ₃²
qrSub(s(15), s(15), s(14), qr);
qrMul(s(15), s(2), s(15), qr, stack); //  | ξ₈
qrMul(s(16), s(2), s(10), qr, stack); //  | ξ₄ ξ₆ | ξ₄ ξ₆ | ξ₄ ξ₆
qrMul(s(17), s(16), s(3), qr, stack); //  | ξ₄ ξ₆   ξ₄²
qrMul(s(11), s(11), s(14), qr, stack); //  | ξ₃ ξ₅   ξ₅²
qrMul(s(17), s(1), s(17), qr, stack);
qrSub(nP(3,2), s(17), s(11), qr); //  | ξ₉
qrSqr(s(18), s(10), qr, stack); //  | ξ₆² | ξ₆² | ξ₆² | ξ₆² | ξ₆² | ξ₆²
qrMul(s(3), nP(2,2), s(3), qr, stack); //  | ξ₇ ξ₄² | ξ₇ ξ₄²
qrMul(nP(0,2), nP(0,2), s(18), qr, stack); //  | ξ₃ ξ₆² | ξ₃ ξ₆²
qrSub(nP(0,2), s(3), nP(0,2), qr);
qrMul(s(3), nP(1,2), nP(0,2), qr, stack); //  | ξ₁₀
qrMul(s(19), nP(1,2), nP(2,2), qr, stack); //  | ξ₅ ξ₇ | ξ₅ ξ₇
qrMul(s(20), s(19), s(14), qr, stack); //  | ξ₅ ξ₇   ξ₅²
qrMul(s(21), s(16), s(18), qr, stack); //  | ξ₄ ξ₆   ξ₆²
qrMul(s(21), s(1), s(21), qr, stack);
qrSub(nP(4,2), s(20), s(21), qr); //  | ξ₁₁
qrSqr(s(22), nP(2,2), qr, stack); //  | ξ₇² | ξ₇² | ξ₇² | ξ₇² | ξ₇²
qrMul(s(23), s(15), s(14), qr, stack); //  | ξ₈ ξ₅²
qrMul(s(24), s(2), s(22), qr, stack); //  | ξ₄ ξ₇²
qrSub(s(24), s(23), s(24), qr);
qrMul(s(24), s(10), s(24), qr, stack); //  | ξ₁₂
qrMul(s(23), s(10), s(15), qr, stack); //  | ξ₆ ξ₈ | ξ₆ ξ₈ | ξ₆ ξ₈
qrMul(s(25), s(23), s(18), qr, stack); //  | ξ₆ ξ₈   ξ₆²
qrMul(s(19), s(19), s(22), qr, stack); //  | ξ₅ ξ₇   ξ₇²
qrMul(s(25), s(1), s(25), qr, stack);
qrSub(nP(5,2), s(25), s(19), qr); //  | ξ₁₃
qrSqr(s(26), s(15), qr, stack); //  | ξ₈² | ξ₈² | ξ₈² | ξ₈² | ξ₈² | ξ₈²
qrMul(s(18), nP(3,2), s(18), qr, stack); //  | ξ₉ ξ₆² | ξ₉ ξ₆²
qrMul(nP(1,2), nP(1,2), s(26), qr, stack); //  | ξ₅ ξ₈² | ξ₅ ξ₈²
qrSub(nP(1,2), s(18), nP(1,2), qr);
qrMul(s(18), nP(2,2), nP(1,2), qr, stack); //  | ξ₁₄
qrMul(s(27), nP(2,2), nP(3,2), qr, stack); //  | ξ₇ ξ₉ | ξ₇ ξ₉
qrMul(s(28), s(27), s(22), qr, stack); //  | ξ₇ ξ₉   ξ₇²
qrMul(s(29), s(23), s(26), qr, stack); //  | ξ₆ ξ₈   ξ₈²
qrMul(s(29), s(1), s(29), qr, stack);
qrSub(nP(6,2), s(28), s(29), qr); //  | ξ₁₅
qrSqr(s(30), nP(3,2), qr, stack); //  | ξ₉² | ξ₉² | ξ₉² | ξ₉² | ξ₉²
qrMul(s(31), s(3), s(22), qr, stack); //  | ξ₁₀ ξ₇²
qrMul(s(10), s(10), s(30), qr, stack); //  | ξ₆ ξ₉²
qrSub(s(10), s(31), s(10), qr);
qrMul(s(10), s(15), s(10), qr, stack); //  | ξ₁₆
qrMul(s(31), s(15), s(3), qr, stack); //  | ξ₈ ξ₁₀ | ξ₈ ξ₁₀ | ξ₈ ξ₁₀
qrMul(s(32), s(31), s(26), qr, stack); //  | ξ₈ ξ₁₀   ξ₈²
qrMul(s(27), s(27), s(30), qr, stack); //  | ξ₇ ξ₉   ξ₉²
qrMul(s(32), s(1), s(32), qr, stack);
qrSub(nP(7,2), s(32), s(27), qr); //  | ξ₁₇
qrSqr(s(33), s(3), qr, stack); //  | ξ₁₀² | ξ₁₀² | ξ₁₀² | ξ₁₀² | ξ₁₀² | ξ₁₀²
qrMul(s(26), nP(4,2), s(26), qr, stack); //  | ξ₁₁ ξ₈² | ξ₁₁ ξ₈²
qrMul(nP(2,2), nP(2,2), s(33), qr, stack); //  | ξ₇ ξ₁₀² | ξ₇ ξ₁₀²
qrSub(nP(2,2), s(26), nP(2,2), qr);
qrMul(s(26), nP(3,2), nP(2,2), qr, stack); //  | ξ₁₈
qrMul(s(34), nP(3,2), nP(4,2), qr, stack); //  | ξ₉ ξ₁₁ | ξ₉ ξ₁₁
qrMul(s(35), s(34), s(30), qr, stack); //  | ξ₉ ξ₁₁   ξ₉²
qrMul(s(36), s(31), s(33), qr, stack); //  | ξ₈ ξ₁₀   ξ₁₀²
qrMul(s(36), s(1), s(36), qr, stack);
qrSub(nP(8,2), s(35), s(36), qr); //  | ξ₁₉
qrSqr(s(37), nP(4,2), qr, stack); //  | ξ₁₁² | ξ₁₁² | ξ₁₁² | ξ₁₁² | ξ₁₁²
qrMul(s(38), s(24), s(30), qr, stack); //  | ξ₁₂ ξ₉²
qrMul(s(15), s(15), s(37), qr, stack); //  | ξ₈ ξ₁₁²
qrSub(s(15), s(38), s(15), qr);
qrMul(s(15), s(3), s(15), qr, stack); //  | ξ₂₀
qrMul(s(38), s(3), s(24), qr, stack); //  | ξ₁₀ ξ₁₂ | ξ₁₀ ξ₁₂ | ξ₁₀ ξ₁₂
qrMul(s(39), s(38), s(33), qr, stack); //  | ξ₁₀ ξ₁₂   ξ₁₀²
qrMul(s(34), s(34), s(37), qr, stack); //  | ξ₉ ξ₁₁   ξ₁₁²
qrMul(s(39), s(1), s(39), qr, stack);
qrSub(nP(9,2), s(39), s(34), qr); //  | ξ₂₁
qrSqr(s(40), s(24), qr, stack); //  | ξ₁₂² | ξ₁₂² | ξ₁₂² | ξ₁₂² | ξ₁₂² | ξ₁₂²
qrMul(s(33), nP(5,2), s(33), qr, stack); //  | ξ₁₃ ξ₁₀² | ξ₁₃ ξ₁₀²
qrMul(nP(3,2), nP(3,2), s(40), qr, stack); //  | ξ₉ ξ₁₂² | ξ₉ ξ₁₂²
qrSub(nP(3,2), s(33), nP(3,2), qr);
qrMul(s(33), nP(4,2), nP(3,2), qr, stack); //  | ξ₂₂
qrMul(s(41), nP(4,2), nP(5,2), qr, stack); //  | ξ₁₁ ξ₁₃ | ξ₁₁ ξ₁₃
qrMul(s(42), s(41), s(37), qr, stack); //  | ξ₁₁ ξ₁₃   ξ₁₁²
qrMul(s(43), s(38), s(40), qr, stack); //  | ξ₁₀ ξ₁₂   ξ₁₂²
qrMul(s(43), s(1), s(43), qr, stack);
qrSub(nP(10,2), s(42), s(43), qr); //  | ξ₂₃
qrSqr(s(44), nP(5,2), qr, stack); //  | ξ₁₃² | ξ₁₃² | ξ₁₃² | ξ₁₃² | ξ₁₃²
qrMul(s(45), s(18), s(37), qr, stack); //  | ξ₁₄ ξ₁₁²
qrMul(s(3), s(3), s(44), qr, stack); //  | ξ₁₀ ξ₁₃²
qrSub(s(3), s(45), s(3), qr);
qrMul(s(3), s(24), s(3), qr, stack); //  | ξ₂₄
qrMul(s(45), s(24), s(18), qr, stack); //  | ξ₁₂ ξ₁₄ | ξ₁₂ ξ₁₄ | ξ₁₂ ξ₁₄
qrMul(s(46), s(45), s(40), qr, stack); //  | ξ₁₂ ξ₁₄   ξ₁₂²
qrMul(s(41), s(41), s(44), qr, stack); //  | ξ₁₁ ξ₁₃   ξ₁₃²
qrMul(s(46), s(1), s(46), qr, stack);
qrSub(nP(11,2), s(46), s(41), qr); //  | ξ₂₅
qrSqr(s(47), s(18), qr, stack); //  | ξ₁₄² | ξ₁₄² | ξ₁₄² | ξ₁₄² | ξ₁₄² | ξ₁₄²
qrMul(s(40), nP(6,2), s(40), qr, stack); //  | ξ₁₅ ξ₁₂² | ξ₁₅ ξ₁₂²
qrMul(nP(4,2), nP(4,2), s(47), qr, stack); //  | ξ₁₁ ξ₁₄² | ξ₁₁ ξ₁₄²
qrSub(nP(4,2), s(40), nP(4,2), qr);
qrMul(s(40), nP(5,2), nP(4,2), qr, stack); //  | ξ₂₆
qrMul(s(48), nP(5,2), nP(6,2), qr, stack); //  | ξ₁₃ ξ₁₅ | ξ₁₃ ξ₁₅
qrMul(s(49), s(48), s(44), qr, stack); //  | ξ₁₃ ξ₁₅   ξ₁₃²
qrMul(s(50), s(45), s(47), qr, stack); //  | ξ₁₂ ξ₁₄   ξ₁₄²
qrMul(s(50), s(1), s(50), qr, stack);
qrSub(nP(12,2), s(49), s(50), qr); //  | ξ₂₇
qrSqr(s(51), nP(6,2), qr, stack); //  | ξ₁₅² | ξ₁₅² | ξ₁₅² | ξ₁₅² | ξ₁₅²
qrMul(s(52), s(10), s(44), qr, stack); //  | ξ₁₆ ξ₁₃²
qrMul(s(24), s(24), s(51), qr, stack); //  | ξ₁₂ ξ₁₅²
qrSub(s(24), s(52), s(24), qr);
qrMul(s(24), s(18), s(24), qr, stack); //  | ξ₂₈
qrMul(s(52), s(18), s(10), qr, stack); //  | ξ₁₄ ξ₁₆ | ξ₁₄ ξ₁₆ | ξ₁₄ ξ₁₆
qrMul(s(53), s(52), s(47), qr, stack); //  | ξ₁₄ ξ₁₆   ξ₁₄²
qrMul(s(48), s(48), s(51), qr, stack); //  | ξ₁₃ ξ₁₅   ξ₁₅²
qrMul(s(53), s(1), s(53), qr, stack);
qrSub(nP(13,2), s(53), s(48), qr); //  | ξ₂₉
qrSqr(s(54), s(10), qr, stack); //  | ξ₁₆² | ξ₁₆² | ξ₁₆² | ξ₁₆² | ξ₁₆² | ξ₁₆²
qrMul(s(47), nP(7,2), s(47), qr, stack); //  | ξ₁₇ ξ₁₄² | ξ₁₇ ξ₁₄²
qrMul(nP(5,2), nP(5,2), s(54), qr, stack); //  | ξ₁₃ ξ₁₆² | ξ₁₃ ξ₁₆²
qrSub(nP(5,2), s(47), nP(5,2), qr);
qrMul(s(47), nP(6,2), nP(5,2), qr, stack); //  | ξ₃₀
qrMul(s(55), nP(6,2), nP(7,2), qr, stack); //  | ξ₁₅ ξ₁₇ | ξ₁₅ ξ₁₇
qrMul(s(56), s(55), s(51), qr, stack); //  | ξ₁₅ ξ₁₇   ξ₁₅²
qrMul(s(57), s(52), s(54), qr, stack); //  | ξ₁₄ ξ₁₆   ξ₁₆²
qrMul(s(57), s(1), s(57), qr, stack);
qrSub(nP(14,2), s(56), s(57), qr); //  | ξ₃₁
qrSqr(s(58), nP(7,2), qr, stack); //  | ξ₁₇² | ξ₁₇² | ξ₁₇² | ξ₁₇² | ξ₁₇²
qrMul(s(59), s(26), s(51), qr, stack); //  | ξ₁₈ ξ₁₅²
qrMul(s(18), s(18), s(58), qr, stack); //  | ξ₁₄ ξ₁₇²
qrSub(s(18), s(59), s(18), qr);
qrMul(s(18), s(10), s(18), qr, stack); //  | ξ₃₂
qrMul(s(59), s(10), s(26), qr, stack); //  | ξ₁₆ ξ₁₈ | ξ₁₆ ξ₁₈ | ξ₁₆ ξ₁₈
qrMul(s(60), s(59), s(54), qr, stack); //  | ξ₁₆ ξ₁₈   ξ₁₆²
qrMul(s(55), s(55), s(58), qr, stack); //  | ξ₁₅ ξ₁₇   ξ₁₇²
qrMul(s(60), s(1), s(60), qr, stack);
qrSub(nP(15,2), s(60), s(55), qr); //  | ξ₃₃
qrSqr(s(61), s(26), qr, stack); //  | ξ₁₈² | ξ₁₈² | ξ₁₈² | ξ₁₈² | ξ₁₈² | ξ₁₈²
qrMul(s(54), nP(8,2), s(54), qr, stack); //  | ξ₁₉ ξ₁₆² | ξ₁₉ ξ₁₆²
qrMul(nP(6,2), nP(6,2), s(61), qr, stack); //  | ξ₁₅ ξ₁₈² | ξ₁₅ ξ₁₈²
qrSub(nP(6,2), s(54), nP(6,2), qr);
qrMul(s(54), nP(7,2), nP(6,2), qr, stack); //  | ξ₃₄
qrMul(s(62), nP(7,2), nP(8,2), qr, stack); //  | ξ₁₇ ξ₁₉ | ξ₁₇ ξ₁₉
qrMul(s(63), s(62), s(58), qr, stack); //  | ξ₁₇ ξ₁₉   ξ₁₇²
qrMul(s(64), s(59), s(61), qr, stack); //  | ξ₁₆ ξ₁₈   ξ₁₈²
qrMul(s(64), s(1), s(64), qr, stack);
qrSub(nP(16,2), s(63), s(64), qr); //  | ξ₃₅
qrSqr(s(65), nP(8,2), qr, stack); //  | ξ₁₉² | ξ₁₉² | ξ₁₉² | ξ₁₉² | ξ₁₉²
qrMul(s(66), s(15), s(58), qr, stack); //  | ξ₂₀ ξ₁₇²
qrMul(s(10), s(10), s(65), qr, stack); //  | ξ₁₆ ξ₁₉²
qrSub(s(10), s(66), s(10), qr);
qrMul(s(10), s(26), s(10), qr, stack); //  | ξ₃₆
qrMul(s(66), s(26), s(15), qr, stack); //  | ξ₁₈ ξ₂₀ | ξ₁₈ ξ₂₀ | ξ₁₈ ξ₂₀
qrMul(s(67), s(66), s(61), qr, stack); //  | ξ₁₈ ξ₂₀   ξ₁₈²
qrMul(s(62), s(62), s(65), qr, stack); //  | ξ₁₇ ξ₁₉   ξ₁₉²
qrMul(s(67), s(1), s(67), qr, stack);
qrSub(nP(17,2), s(67), s(62), qr); //  | ξ₃₇
qrSqr(s(68), s(15), qr, stack); //  | ξ₂₀² | ξ₂₀² | ξ₂₀² | ξ₂₀² | ξ₂₀² | ξ₂₀²
qrMul(s(61), nP(9,2), s(61), qr, stack); //  | ξ₂₁ ξ₁₈² | ξ₂₁ ξ₁₈²
qrMul(nP(7,2), nP(7,2), s(68), qr, stack); //  | ξ₁₇ ξ₂₀² | ξ₁₇ ξ₂₀²
qrSub(nP(7,2), s(61), nP(7,2), qr);
qrMul(s(61), nP(8,2), nP(7,2), qr, stack); //  | ξ₃₈
qrMul(s(69), nP(8,2), nP(9,2), qr, stack); //  | ξ₁₉ ξ₂₁ | ξ₁₉ ξ₂₁
qrMul(s(70), s(69), s(65), qr, stack); //  | ξ₁₉ ξ₂₁   ξ₁₉²
qrMul(s(71), s(66), s(68), qr, stack); //  | ξ₁₈ ξ₂₀   ξ₂₀²
qrMul(s(71), s(1), s(71), qr, stack);
qrSub(nP(18,2), s(70), s(71), qr); //  | ξ₃₉
qrSqr(s(72), nP(9,2), qr, stack); //  | ξ₂₁² | ξ₂₁² | ξ₂₁² | ξ₂₁² | ξ₂₁²
qrMul(s(73), s(33), s(65), qr, stack); //  | ξ₂₂ ξ₁₉²
qrMul(s(26), s(26), s(72), qr, stack); //  | ξ₁₈ ξ₂₁²
qrSub(s(26), s(73), s(26), qr);
qrMul(s(26), s(15), s(26), qr, stack); //  | ξ₄₀
qrMul(s(73), s(15), s(33), qr, stack); //  | ξ₂₀ ξ₂₂ | ξ₂₀ ξ₂₂ | ξ₂₀ ξ₂₂
qrMul(s(74), s(73), s(68), qr, stack); //  | ξ₂₀ ξ₂₂   ξ₂₀²
qrMul(s(69), s(69), s(72), qr, stack); //  | ξ₁₉ ξ₂₁   ξ₂₁²
qrMul(s(74), s(1), s(74), qr, stack);
qrSub(nP(19,2), s(74), s(69), qr); //  | ξ₄₁
qrSqr(s(75), s(33), qr, stack); //  | ξ₂₂² | ξ₂₂² | ξ₂₂² | ξ₂₂² | ξ₂₂² | ξ₂₂²
qrMul(s(68), nP(10,2), s(68), qr, stack); //  | ξ₂₃ ξ₂₀² | ξ₂₃ ξ₂₀²
qrMul(nP(8,2), nP(8,2), s(75), qr, stack); //  | ξ₁₉ ξ₂₂² | ξ₁₉ ξ₂₂²
qrSub(nP(8,2), s(68), nP(8,2), qr);
qrMul(s(68), nP(9,2), nP(8,2), qr, stack); //  | ξ₄₂
qrMul(s(76), nP(9,2), nP(10,2), qr, stack); //  | ξ₂₁ ξ₂₃ | ξ₂₁ ξ₂₃
qrMul(s(77), s(76), s(72), qr, stack); //  | ξ₂₁ ξ₂₃   ξ₂₁²
qrMul(s(78), s(73), s(75), qr, stack); //  | ξ₂₀ ξ₂₂   ξ₂₂²
qrMul(s(78), s(1), s(78), qr, stack);
qrSub(nP(20,2), s(77), s(78), qr); //  | ξ₄₃
qrSqr(s(79), nP(10,2), qr, stack); //  | ξ₂₃² | ξ₂₃² | ξ₂₃² | ξ₂₃² | ξ₂₃²
qrMul(s(80), s(3), s(72), qr, stack); //  | ξ₂₄ ξ₂₁²
qrMul(s(15), s(15), s(79), qr, stack); //  | ξ₂₀ ξ₂₃²
qrSub(s(15), s(80), s(15), qr);
qrMul(s(15), s(33), s(15), qr, stack); //  | ξ₄₄
qrMul(s(80), s(33), s(3), qr, stack); //  | ξ₂₂ ξ₂₄ | ξ₂₂ ξ₂₄ | ξ₂₂ ξ₂₄
qrMul(s(81), s(80), s(75), qr, stack); //  | ξ₂₂ ξ₂₄   ξ₂₂²
qrMul(s(76), s(76), s(79), qr, stack); //  | ξ₂₁ ξ₂₃   ξ₂₃²
qrMul(s(81), s(1), s(81), qr, stack);
qrSub(nP(21,2), s(81), s(76), qr); //  | ξ₄₅
qrSqr(s(82), s(3), qr, stack); //  | ξ₂₄² | ξ₂₄² | ξ₂₄² | ξ₂₄² | ξ₂₄² | ξ₂₄²
qrMul(s(75), nP(11,2), s(75), qr, stack); //  | ξ₂₅ ξ₂₂² | ξ₂₅ ξ₂₂²
qrMul(nP(9,2), nP(9,2), s(82), qr, stack); //  | ξ₂₁ ξ₂₄² | ξ₂₁ ξ₂₄²
qrSub(nP(9,2), s(75), nP(9,2), qr);
qrMul(s(75), nP(10,2), nP(9,2), qr, stack); //  | ξ₄₆
qrMul(s(83), nP(10,2), nP(11,2), qr, stack); //  | ξ₂₃ ξ₂₅ | ξ₂₃ ξ₂₅
qrMul(s(84), s(83), s(79), qr, stack); //  | ξ₂₃ ξ₂₅   ξ₂₃²
qrMul(s(85), s(80), s(82), qr, stack); //  | ξ₂₂ ξ₂₄   ξ₂₄²
qrMul(s(85), s(1), s(85), qr, stack);
qrSub(nP(22,2), s(84), s(85), qr); //  | ξ₄₇
qrSqr(s(86), nP(11,2), qr, stack); //  | ξ₂₅² | ξ₂₅² | ξ₂₅² | ξ₂₅² | ξ₂₅²
qrMul(s(87), s(40), s(79), qr, stack); //  | ξ₂₆ ξ₂₃²
qrMul(s(33), s(33), s(86), qr, stack); //  | ξ₂₂ ξ₂₅²
qrSub(s(33), s(87), s(33), qr);
qrMul(s(33), s(3), s(33), qr, stack); //  | ξ₄₈
qrMul(s(87), s(3), s(40), qr, stack); //  | ξ₂₄ ξ₂₆ | ξ₂₄ ξ₂₆ | ξ₂₄ ξ₂₆
qrMul(s(88), s(87), s(82), qr, stack); //  | ξ₂₄ ξ₂₆   ξ₂₄²
qrMul(s(83), s(83), s(86), qr, stack); //  | ξ₂₃ ξ₂₅   ξ₂₅²
qrMul(s(88), s(1), s(88), qr, stack);
qrSub(nP(23,2), s(88), s(83), qr); //  | ξ₄₉
qrSqr(s(89), s(40), qr, stack); //  | ξ₂₆² | ξ₂₆² | ξ₂₆² | ξ₂₆² | ξ₂₆² | ξ₂₆²
qrMul(s(82), nP(12,2), s(82), qr, stack); //  | ξ₂₇ ξ₂₄² | ξ₂₇ ξ₂₄²
qrMul(nP(10,2), nP(10,2), s(89), qr, stack); //  | ξ₂₃ ξ₂₆² | ξ₂₃ ξ₂₆²
qrSub(nP(10,2), s(82), nP(10,2), qr);
qrMul(s(82), nP(11,2), nP(10,2), qr, stack); //  | ξ₅₀
qrMul(s(90), nP(11,2), nP(12,2), qr, stack); //  | ξ₂₅ ξ₂₇ | ξ₂₅ ξ₂₇
qrMul(s(91), s(90), s(86), qr, stack); //  | ξ₂₅ ξ₂₇   ξ₂₅²
qrMul(s(92), s(87), s(89), qr, stack); //  | ξ₂₄ ξ₂₆   ξ₂₆²
qrMul(s(92), s(1), s(92), qr, stack);
qrSub(nP(24,2), s(91), s(92), qr); //  | ξ₅₁
qrSqr(s(93), nP(12,2), qr, stack); //  | ξ₂₇² | ξ₂₇² | ξ₂₇² | ξ₂₇² | ξ₂₇²
qrMul(s(94), s(24), s(86), qr, stack); //  | ξ₂₈ ξ₂₅²
qrMul(s(3), s(3), s(93), qr, stack); //  | ξ₂₄ ξ₂₇²
qrSub(s(3), s(94), s(3), qr);
qrMul(s(3), s(40), s(3), qr, stack); //  | ξ₅₂
qrMul(s(94), s(40), s(24), qr, stack); //  | ξ₂₆ ξ₂₈ | ξ₂₆ ξ₂₈ | ξ₂₆ ξ₂₈
qrMul(s(95), s(94), s(89), qr, stack); //  | ξ₂₆ ξ₂₈   ξ₂₆²
qrMul(s(90), s(90), s(93), qr, stack); //  | ξ₂₅ ξ₂₇   ξ₂₇²
qrMul(s(95), s(1), s(95), qr, stack);
qrSub(nP(25,2), s(95), s(90), qr); //  | ξ₅₃
qrSqr(s(96), s(24), qr, stack); //  | ξ₂₈² | ξ₂₈² | ξ₂₈² | ξ₂₈² | ξ₂₈² | ξ₂₈²
qrMul(s(89), nP(13,2), s(89), qr, stack); //  | ξ₂₉ ξ₂₆² | ξ₂₉ ξ₂₆²
qrMul(nP(11,2), nP(11,2), s(96), qr, stack); //  | ξ₂₅ ξ₂₈² | ξ₂₅ ξ₂₈²
qrSub(nP(11,2), s(89), nP(11,2), qr);
qrMul(s(89), nP(12,2), nP(11,2), qr, stack); //  | ξ₅₄
qrMul(s(97), nP(12,2), nP(13,2), qr, stack); //  | ξ₂₇ ξ₂₉ | ξ₂₇ ξ₂₉
qrMul(s(98), s(97), s(93), qr, stack); //  | ξ₂₇ ξ₂₉   ξ₂₇²
qrMul(s(99), s(94), s(96), qr, stack); //  | ξ₂₆ ξ₂₈   ξ₂₈²
qrMul(s(99), s(1), s(99), qr, stack);
qrSub(nP(26,2), s(98), s(99), qr); //  | ξ₅₅
qrSqr(s(100), nP(13,2), qr, stack); //  | ξ₂₉² | ξ₂₉² | ξ₂₉² | ξ₂₉² | ξ₂₉²
qrMul(s(101), s(47), s(93), qr, stack); //  | ξ₃₀ ξ₂₇²
qrMul(s(40), s(40), s(100), qr, stack); //  | ξ₂₆ ξ₂₉²
qrSub(s(40), s(101), s(40), qr);
qrMul(s(40), s(24), s(40), qr, stack); //  | ξ₅₆
qrMul(s(101), s(24), s(47), qr, stack); //  | ξ₂₈ ξ₃₀ | ξ₂₈ ξ₃₀ | ξ₂₈ ξ₃₀
qrMul(s(102), s(101), s(96), qr, stack); //  | ξ₂₈ ξ₃₀   ξ₂₈²
qrMul(s(97), s(97), s(100), qr, stack); //  | ξ₂₇ ξ₂₉   ξ₂₉²
qrMul(s(102), s(1), s(102), qr, stack);
qrSub(nP(27,2), s(102), s(97), qr); //  | ξ₅₇
qrSqr(s(103), s(47), qr, stack); //  | ξ₃₀² | ξ₃₀² | ξ₃₀² | ξ₃₀² | ξ₃₀² | ξ₃₀²
qrMul(s(96), nP(14,2), s(96), qr, stack); //  | ξ₃₁ ξ₂₈² | ξ₃₁ ξ₂₈²
qrMul(nP(12,2), nP(12,2), s(103), qr, stack); //  | ξ₂₇ ξ₃₀² | ξ₂₇ ξ₃₀²
qrSub(nP(12,2), s(96), nP(12,2), qr);
qrMul(s(96), nP(13,2), nP(12,2), qr, stack); //  | ξ₅₈
qrMul(s(104), nP(13,2), nP(14,2), qr, stack); //  | ξ₂₉ ξ₃₁ | ξ₂₉ ξ₃₁
qrMul(s(105), s(104), s(100), qr, stack); //  | ξ₂₉ ξ₃₁   ξ₂₉²
qrMul(s(106), s(101), s(103), qr, stack); //  | ξ₂₈ ξ₃₀   ξ₃₀²
qrMul(s(106), s(1), s(106), qr, stack);
qrSub(nP(28,2), s(105), s(106), qr); //  | ξ₅₉
qrSqr(s(107), nP(14,2), qr, stack); //  | ξ₃₁² | ξ₃₁² | ξ₃₁² | ξ₃₁² | ξ₃₁²
qrMul(s(108), s(18), s(100), qr, stack); //  | ξ₃₂ ξ₂₉²
qrMul(s(24), s(24), s(107), qr, stack); //  | ξ₂₈ ξ₃₁²
qrSub(s(24), s(108), s(24), qr);
qrMul(s(24), s(47), s(24), qr, stack); //  | ξ₆₀
qrMul(s(108), s(47), s(18), qr, stack); //  | ξ₃₀ ξ₃₂ | ξ₃₀ ξ₃₂ | ξ₃₀ ξ₃₂
qrMul(s(109), s(108), s(103), qr, stack); //  | ξ₃₀ ξ₃₂   ξ₃₀²
qrMul(s(104), s(104), s(107), qr, stack); //  | ξ₂₉ ξ₃₁   ξ₃₁²
qrMul(s(109), s(1), s(109), qr, stack);
qrSub(nP(29,2), s(109), s(104), qr); //  | ξ₆₁
qrSqr(s(110), s(18), qr, stack); //  | ξ₃₂² | ξ₃₂² | ξ₃₂² | ξ₃₂² | ξ₃₂² | ξ₃₂²
qrMul(s(103), nP(15,2), s(103), qr, stack); //  | ξ₃₃ ξ₃₀² | ξ₃₃ ξ₃₀²
qrMul(nP(13,2), nP(13,2), s(110), qr, stack); //  | ξ₂₉ ξ₃₂² | ξ₂₉ ξ₃₂²
qrSub(nP(13,2), s(103), nP(13,2), qr);
qrMul(s(103), nP(14,2), nP(13,2), qr, stack); //  | ξ₆₂
qrMul(s(111), nP(14,2), nP(15,2), qr, stack); //  | ξ₃₁ ξ₃₃ | ξ₃₁ ξ₃₃
qrMul(s(112), s(111), s(107), qr, stack); //  | ξ₃₁ ξ₃₃   ξ₃₁²
qrMul(s(113), s(108), s(110), qr, stack); //  | ξ₃₀ ξ₃₂   ξ₃₂²
qrMul(s(113), s(1), s(113), qr, stack);
qrSub(nP(30,2), s(112), s(113), qr); //  | ξ₆₃
qrSqr(s(114), nP(15,2), qr, stack); //  | ξ₃₃² | ξ₃₃² | ξ₃₃² | ξ₃₃² | ξ₃₃²
qrMul(s(115), s(54), s(107), qr, stack); //  | ξ₃₄ ξ₃₁²
qrMul(s(47), s(47), s(114), qr, stack); //  | ξ₃₀ ξ₃₃²
qrSub(s(47), s(115), s(47), qr);
qrMul(s(47), s(18), s(47), qr, stack); //  | ξ₆₄
qrMul(s(115), s(18), s(54), qr, stack); //  | ξ₃₂ ξ₃₄ | ξ₃₂ ξ₃₄ | ξ₃₂ ξ₃₄
qrMul(s(116), s(115), s(110), qr, stack); //  | ξ₃₂ ξ₃₄   ξ₃₂²
qrMul(s(111), s(111), s(114), qr, stack); //  | ξ₃₁ ξ₃₃   ξ₃₃²
qrMul(s(116), s(1), s(116), qr, stack);
qrSub(nP(31,2), s(116), s(111), qr); //  | ξ₆₅
qrSqr(s(117), s(54), qr, stack); //  | ξ₃₄² | ξ₃₄² | ξ₃₄² | ξ₃₄² | ξ₃₄² | ξ₃₄²
qrMul(s(110), nP(16,2), s(110), qr, stack); //  | ξ₃₅ ξ₃₂² | ξ₃₅ ξ₃₂²
qrMul(nP(14,2), nP(14,2), s(117), qr, stack); //  | ξ₃₁ ξ₃₄² | ξ₃₁ ξ₃₄²
qrSub(nP(14,2), s(110), nP(14,2), qr);
qrMul(s(110), nP(15,2), nP(14,2), qr, stack); //  | ξ₆₆
qrMul(s(118), nP(15,2), nP(16,2), qr, stack); //  | ξ₃₃ ξ₃₅ | ξ₃₃ ξ₃₅
qrMul(s(119), s(118), s(114), qr, stack); //  | ξ₃₃ ξ₃₅   ξ₃₃²
qrMul(s(120), s(115), s(117), qr, stack); //  | ξ₃₂ ξ₃₄   ξ₃₄²
qrMul(s(120), s(1), s(120), qr, stack);
qrSub(nP(32,2), s(119), s(120), qr); //  | ξ₆₇
qrSqr(s(121), nP(16,2), qr, stack); //  | ξ₃₅² | ξ₃₅² | ξ₃₅² | ξ₃₅² | ξ₃₅²
qrMul(s(122), s(10), s(114), qr, stack); //  | ξ₃₆ ξ₃₃²
qrMul(s(18), s(18), s(121), qr, stack); //  | ξ₃₂ ξ₃₅²
qrSub(s(18), s(122), s(18), qr);
qrMul(s(18), s(54), s(18), qr, stack); //  | ξ₆₈
qrMul(s(122), s(54), s(10), qr, stack); //  | ξ₃₄ ξ₃₆ | ξ₃₄ ξ₃₆ | ξ₃₄ ξ₃₆
qrMul(s(123), s(122), s(117), qr, stack); //  | ξ₃₄ ξ₃₆   ξ₃₄²
qrMul(s(118), s(118), s(121), qr, stack); //  | ξ₃₃ ξ₃₅   ξ₃₅²
qrMul(s(123), s(1), s(123), qr, stack);
qrSub(nP(33,2), s(123), s(118), qr); //  | ξ₆₉
qrSqr(s(124), s(10), qr, stack); //  | ξ₃₆² | ξ₃₆² | ξ₃₆² | ξ₃₆² | ξ₃₆² | ξ₃₆²
qrMul(s(117), nP(17,2), s(117), qr, stack); //  | ξ₃₇ ξ₃₄² | ξ₃₇ ξ₃₄²
qrMul(nP(15,2), nP(15,2), s(124), qr, stack); //  | ξ₃₃ ξ₃₆² | ξ₃₃ ξ₃₆²
qrSub(nP(15,2), s(117), nP(15,2), qr);
qrMul(s(117), nP(16,2), nP(15,2), qr, stack); //  | ξ₇₀
qrMul(s(125), nP(16,2), nP(17,2), qr, stack); //  | ξ₃₅ ξ₃₇ | ξ₃₅ ξ₃₇
qrMul(s(126), s(125), s(121), qr, stack); //  | ξ₃₅ ξ₃₇   ξ₃₅²
qrMul(s(127), s(122), s(124), qr, stack); //  | ξ₃₄ ξ₃₆   ξ₃₆²
qrMul(s(127), s(1), s(127), qr, stack);
qrSub(nP(34,2), s(126), s(127), qr); //  | ξ₇₁
qrSqr(s(128), nP(17,2), qr, stack); //  | ξ₃₇² | ξ₃₇² | ξ₃₇² | ξ₃₇² | ξ₃₇²
qrMul(s(129), s(61), s(121), qr, stack); //  | ξ₃₈ ξ₃₅²
qrMul(s(54), s(54), s(128), qr, stack); //  | ξ₃₄ ξ₃₇²
qrSub(s(54), s(129), s(54), qr);
qrMul(s(54), s(10), s(54), qr, stack); //  | ξ₇₂
qrMul(s(129), s(10), s(61), qr, stack); //  | ξ₃₆ ξ₃₈ | ξ₃₆ ξ₃₈ | ξ₃₆ ξ₃₈
qrMul(s(130), s(129), s(124), qr, stack); //  | ξ₃₆ ξ₃₈   ξ₃₆²
qrMul(s(125), s(125), s(128), qr, stack); //  | ξ₃₅ ξ₃₇   ξ₃₇²
qrMul(s(130), s(1), s(130), qr, stack);
qrSub(nP(35,2), s(130), s(125), qr); //  | ξ₇₃
qrSqr(s(131), s(61), qr, stack); //  | ξ₃₈² | ξ₃₈² | ξ₃₈² | ξ₃₈² | ξ₃₈² | ξ₃₈²
qrMul(s(124), nP(18,2), s(124), qr, stack); //  | ξ₃₉ ξ₃₆² | ξ₃₉ ξ₃₆²
qrMul(nP(16,2), nP(16,2), s(131), qr, stack); //  | ξ₃₅ ξ₃₈² | ξ₃₅ ξ₃₈²
qrSub(nP(16,2), s(124), nP(16,2), qr);
qrMul(s(124), nP(17,2), nP(16,2), qr, stack); //  | ξ₇₄
qrMul(s(132), nP(17,2), nP(18,2), qr, stack); //  | ξ₃₇ ξ₃₉ | ξ₃₇ ξ₃₉
qrMul(s(133), s(132), s(128), qr, stack); //  | ξ₃₇ ξ₃₉   ξ₃₇²
qrMul(s(134), s(129), s(131), qr, stack); //  | ξ₃₆ ξ₃₈   ξ₃₈²
qrMul(s(134), s(1), s(134), qr, stack);
qrSub(nP(36,2), s(133), s(134), qr); //  | ξ₇₅
qrSqr(s(135), nP(18,2), qr, stack); //  | ξ₃₉² | ξ₃₉² | ξ₃₉² | ξ₃₉² | ξ₃₉²
qrMul(s(136), s(26), s(128), qr, stack); //  | ξ₄₀ ξ₃₇²
qrMul(s(10), s(10), s(135), qr, stack); //  | ξ₃₆ ξ₃₉²
qrSub(s(10), s(136), s(10), qr);
qrMul(s(10), s(61), s(10), qr, stack); //  | ξ₇₆
qrMul(s(136), s(61), s(26), qr, stack); //  | ξ₃₈ ξ₄₀ | ξ₃₈ ξ₄₀ | ξ₃₈ ξ₄₀
qrMul(s(137), s(136), s(131), qr, stack); //  | ξ₃₈ ξ₄₀   ξ₃₈²
qrMul(s(132), s(132), s(135), qr, stack); //  | ξ₃₇ ξ₃₉   ξ₃₉²
qrMul(s(137), s(1), s(137), qr, stack);
qrSub(nP(37,2), s(137), s(132), qr); //  | ξ₇₇
qrSqr(s(138), s(26), qr, stack); //  | ξ₄₀² | ξ₄₀² | ξ₄₀² | ξ₄₀² | ξ₄₀² | ξ₄₀²
qrMul(s(131), nP(19,2), s(131), qr, stack); //  | ξ₄₁ ξ₃₈² | ξ₄₁ ξ₃₈²
qrMul(nP(17,2), nP(17,2), s(138), qr, stack); //  | ξ₃₇ ξ₄₀² | ξ₃₇ ξ₄₀²
qrSub(nP(17,2), s(131), nP(17,2), qr);
qrMul(s(131), nP(18,2), nP(17,2), qr, stack); //  | ξ₇₈
qrMul(s(139), nP(18,2), nP(19,2), qr, stack); //  | ξ₃₉ ξ₄₁ | ξ₃₉ ξ₄₁
qrMul(s(140), s(139), s(135), qr, stack); //  | ξ₃₉ ξ₄₁   ξ₃₉²
qrMul(s(141), s(136), s(138), qr, stack); //  | ξ₃₈ ξ₄₀   ξ₄₀²
qrMul(s(141), s(1), s(141), qr, stack);
qrSub(nP(38,2), s(140), s(141), qr); //  | ξ₇₉
qrSqr(s(142), nP(19,2), qr, stack); //  | ξ₄₁² | ξ₄₁² | ξ₄₁² | ξ₄₁² | ξ₄₁²
qrMul(s(143), s(68), s(135), qr, stack); //  | ξ₄₂ ξ₃₉²
qrMul(s(61), s(61), s(142), qr, stack); //  | ξ₃₈ ξ₄₁²
qrSub(s(61), s(143), s(61), qr);
qrMul(s(61), s(26), s(61), qr, stack); //  | ξ₈₀
qrMul(s(143), s(26), s(68), qr, stack); //  | ξ₄₀ ξ₄₂ | ξ₄₀ ξ₄₂ | ξ₄₀ ξ₄₂
qrMul(s(144), s(143), s(138), qr, stack); //  | ξ₄₀ ξ₄₂   ξ₄₀²
qrMul(s(139), s(139), s(142), qr, stack); //  | ξ₃₉ ξ₄₁   ξ₄₁²
qrMul(s(144), s(1), s(144), qr, stack);
qrSub(nP(39,2), s(144), s(139), qr); //  | ξ₈₁
qrSqr(s(145), s(68), qr, stack); //  | ξ₄₂² | ξ₄₂² | ξ₄₂² | ξ₄₂² | ξ₄₂² | ξ₄₂²
qrMul(s(138), nP(20,2), s(138), qr, stack); //  | ξ₄₃ ξ₄₀² | ξ₄₃ ξ₄₀²
qrMul(nP(18,2), nP(18,2), s(145), qr, stack); //  | ξ₃₉ ξ₄₂² | ξ₃₉ ξ₄₂²
qrSub(nP(18,2), s(138), nP(18,2), qr);
qrMul(s(138), nP(19,2), nP(18,2), qr, stack); //  | ξ₈₂
qrMul(s(146), nP(19,2), nP(20,2), qr, stack); //  | ξ₄₁ ξ₄₃ | ξ₄₁ ξ₄₃
qrMul(s(147), s(146), s(142), qr, stack); //  | ξ₄₁ ξ₄₃   ξ₄₁²
qrMul(s(148), s(143), s(145), qr, stack); //  | ξ₄₀ ξ₄₂   ξ₄₂²
qrMul(s(148), s(1), s(148), qr, stack);
qrSub(nP(40,2), s(147), s(148), qr); //  | ξ₈₃
qrSqr(s(149), nP(20,2), qr, stack); //  | ξ₄₃² | ξ₄₃² | ξ₄₃² | ξ₄₃² | ξ₄₃²
qrMul(s(150), s(15), s(142), qr, stack); //  | ξ₄₄ ξ₄₁²
qrMul(s(26), s(26), s(149), qr, stack); //  | ξ₄₀ ξ₄₃²
qrSub(s(26), s(150), s(26), qr);
qrMul(s(26), s(68), s(26), qr, stack); //  | ξ₈₄
qrMul(s(150), s(68), s(15), qr, stack); //  | ξ₄₂ ξ₄₄ | ξ₄₂ ξ₄₄ | ξ₄₂ ξ₄₄
qrMul(s(151), s(150), s(145), qr, stack); //  | ξ₄₂ ξ₄₄   ξ₄₂²
qrMul(s(146), s(146), s(149), qr, stack); //  | ξ₄₁ ξ₄₃   ξ₄₃²
qrMul(s(151), s(1), s(151), qr, stack);
qrSub(nP(41,2), s(151), s(146), qr); //  | ξ₈₅
qrSqr(s(152), s(15), qr, stack); //  | ξ₄₄² | ξ₄₄² | ξ₄₄² | ξ₄₄² | ξ₄₄² | ξ₄₄²
qrMul(s(145), nP(21,2), s(145), qr, stack); //  | ξ₄₅ ξ₄₂² | ξ₄₅ ξ₄₂²
qrMul(nP(19,2), nP(19,2), s(152), qr, stack); //  | ξ₄₁ ξ₄₄² | ξ₄₁ ξ₄₄²
qrSub(nP(19,2), s(145), nP(19,2), qr);
qrMul(s(145), nP(20,2), nP(19,2), qr, stack); //  | ξ₈₆
qrMul(s(153), nP(20,2), nP(21,2), qr, stack); //  | ξ₄₃ ξ₄₅ | ξ₄₃ ξ₄₅
qrMul(s(154), s(153), s(149), qr, stack); //  | ξ₄₃ ξ₄₅   ξ₄₃²
qrMul(s(155), s(150), s(152), qr, stack); //  | ξ₄₂ ξ₄₄   ξ₄₄²
qrMul(s(155), s(1), s(155), qr, stack);
qrSub(nP(42,2), s(154), s(155), qr); //  | ξ₈₇
qrSqr(s(156), nP(21,2), qr, stack); //  | ξ₄₅² | ξ₄₅² | ξ₄₅² | ξ₄₅² | ξ₄₅²
qrMul(s(157), s(75), s(149), qr, stack); //  | ξ₄₆ ξ₄₃²
qrMul(s(68), s(68), s(156), qr, stack); //  | ξ₄₂ ξ₄₅²
qrSub(s(68), s(157), s(68), qr);
qrMul(s(68), s(15), s(68), qr, stack); //  | ξ₈₈
qrMul(s(157), s(15), s(75), qr, stack); //  | ξ₄₄ ξ₄₆ | ξ₄₄ ξ₄₆ | ξ₄₄ ξ₄₆
qrMul(s(158), s(157), s(152), qr, stack); //  | ξ₄₄ ξ₄₆   ξ₄₄²
qrMul(s(153), s(153), s(156), qr, stack); //  | ξ₄₃ ξ₄₅   ξ₄₅²
qrMul(s(158), s(1), s(158), qr, stack);
qrSub(nP(43,2), s(158), s(153), qr); //  | ξ₈₉
qrSqr(s(159), s(75), qr, stack); //  | ξ₄₆² | ξ₄₆² | ξ₄₆² | ξ₄₆² | ξ₄₆² | ξ₄₆²
qrMul(s(152), nP(22,2), s(152), qr, stack); //  | ξ₄₇ ξ₄₄² | ξ₄₇ ξ₄₄²
qrMul(nP(20,2), nP(20,2), s(159), qr, stack); //  | ξ₄₃ ξ₄₆² | ξ₄₃ ξ₄₆²
qrSub(nP(20,2), s(152), nP(20,2), qr);
qrMul(s(152), nP(21,2), nP(20,2), qr, stack); //  | ξ₉₀
qrMul(s(160), nP(21,2), nP(22,2), qr, stack); //  | ξ₄₅ ξ₄₇ | ξ₄₅ ξ₄₇
qrMul(s(161), s(160), s(156), qr, stack); //  | ξ₄₅ ξ₄₇   ξ₄₅²
qrMul(s(162), s(157), s(159), qr, stack); //  | ξ₄₄ ξ₄₆   ξ₄₆²
qrMul(s(162), s(1), s(162), qr, stack);
qrSub(nP(44,2), s(161), s(162), qr); //  | ξ₉₁
qrSqr(s(163), nP(22,2), qr, stack); //  | ξ₄₇² | ξ₄₇² | ξ₄₇² | ξ₄₇² | ξ₄₇²
qrMul(s(164), s(33), s(156), qr, stack); //  | ξ₄₈ ξ₄₅²
qrMul(s(15), s(15), s(163), qr, stack); //  | ξ₄₄ ξ₄₇²
qrSub(s(15), s(164), s(15), qr);
qrMul(s(15), s(75), s(15), qr, stack); //  | ξ₉₂
qrMul(s(164), s(75), s(33), qr, stack); //  | ξ₄₆ ξ₄₈ | ξ₄₆ ξ₄₈ | ξ₄₆ ξ₄₈
qrMul(s(165), s(164), s(159), qr, stack); //  | ξ₄₆ ξ₄₈   ξ₄₆²
qrMul(s(160), s(160), s(163), qr, stack); //  | ξ₄₅ ξ₄₇   ξ₄₇²
qrMul(s(165), s(1), s(165), qr, stack);
qrSub(nP(45,2), s(165), s(160), qr); //  | ξ₉₃
qrSqr(s(166), s(33), qr, stack); //  | ξ₄₈² | ξ₄₈² | ξ₄₈² | ξ₄₈² | ξ₄₈² | ξ₄₈²
qrMul(s(159), nP(23,2), s(159), qr, stack); //  | ξ₄₉ ξ₄₆² | ξ₄₉ ξ₄₆²
qrMul(nP(21,2), nP(21,2), s(166), qr, stack); //  | ξ₄₅ ξ₄₈² | ξ₄₅ ξ₄₈²
qrSub(nP(21,2), s(159), nP(21,2), qr);
qrMul(s(159), nP(22,2), nP(21,2), qr, stack); //  | ξ₉₄
qrMul(s(167), nP(22,2), nP(23,2), qr, stack); //  | ξ₄₇ ξ₄₉ | ξ₄₇ ξ₄₉
qrMul(s(168), s(167), s(163), qr, stack); //  | ξ₄₇ ξ₄₉   ξ₄₇²
qrMul(s(169), s(164), s(166), qr, stack); //  | ξ₄₆ ξ₄₈   ξ₄₈²
qrMul(s(169), s(1), s(169), qr, stack);
qrSub(nP(46,2), s(168), s(169), qr); //  | ξ₉₅
qrSqr(s(170), nP(23,2), qr, stack); //  | ξ₄₉² | ξ₄₉² | ξ₄₉² | ξ₄₉² | ξ₄₉²
qrMul(s(171), s(82), s(163), qr, stack); //  | ξ₅₀ ξ₄₇²
qrMul(s(75), s(75), s(170), qr, stack); //  | ξ₄₆ ξ₄₉²
qrSub(s(75), s(171), s(75), qr);
qrMul(s(75), s(33), s(75), qr, stack); //  | ξ₉₆
qrMul(s(171), s(33), s(82), qr, stack); //  | ξ₄₈ ξ₅₀ | ξ₄₈ ξ₅₀ | ξ₄₈ ξ₅₀
qrMul(s(172), s(171), s(166), qr, stack); //  | ξ₄₈ ξ₅₀   ξ₄₈²
qrMul(s(167), s(167), s(170), qr, stack); //  | ξ₄₇ ξ₄₉   ξ₄₉²
qrMul(s(172), s(1), s(172), qr, stack);
qrSub(nP(47,2), s(172), s(167), qr); //  | ξ₉₇
qrSqr(s(173), s(82), qr, stack); //  | ξ₅₀² | ξ₅₀² | ξ₅₀² | ξ₅₀² | ξ₅₀² | ξ₅₀²
qrMul(s(166), nP(24,2), s(166), qr, stack); //  | ξ₅₁ ξ₄₈² | ξ₅₁ ξ₄₈²
qrMul(nP(22,2), nP(22,2), s(173), qr, stack); //  | ξ₄₇ ξ₅₀² | ξ₄₇ ξ₅₀²
qrSub(nP(22,2), s(166), nP(22,2), qr);
qrMul(s(166), nP(23,2), nP(22,2), qr, stack); //  | ξ₉₈
qrMul(s(174), nP(23,2), nP(24,2), qr, stack); //  | ξ₄₉ ξ₅₁ | ξ₄₉ ξ₅₁
qrMul(s(175), s(174), s(170), qr, stack); //  | ξ₄₉ ξ₅₁   ξ₄₉²
qrMul(s(176), s(171), s(173), qr, stack); //  | ξ₄₈ ξ₅₀   ξ₅₀²
qrMul(s(176), s(1), s(176), qr, stack);
qrSub(nP(48,2), s(175), s(176), qr); //  | ξ₉₉
qrSqr(s(177), nP(24,2), qr, stack); //  | ξ₅₁² | ξ₅₁² | ξ₅₁² | ξ₅₁² | ξ₅₁²
qrMul(s(178), s(3), s(170), qr, stack); //  | ξ₅₂ ξ₄₉²
qrMul(s(33), s(33), s(177), qr, stack); //  | ξ₄₈ ξ₅₁²
qrSub(s(33), s(178), s(33), qr);
qrMul(s(33), s(82), s(33), qr, stack); //  | ξ₁₀₀
qrMul(s(178), s(82), s(3), qr, stack); //  | ξ₅₀ ξ₅₂ | ξ₅₀ ξ₅₂ | ξ₅₀ ξ₅₂
qrMul(s(179), s(178), s(173), qr, stack); //  | ξ₅₀ ξ₅₂   ξ₅₀²
qrMul(s(174), s(174), s(177), qr, stack); //  | ξ₄₉ ξ₅₁   ξ₅₁²
qrMul(s(179), s(1), s(179), qr, stack);
qrSub(nP(49,2), s(179), s(174), qr); //  | ξ₁₀₁
qrSqr(s(180), s(3), qr, stack); //  | ξ₅₂² | ξ₅₂² | ξ₅₂² | ξ₅₂² | ξ₅₂² | ξ₅₂²
qrMul(s(173), nP(25,2), s(173), qr, stack); //  | ξ₅₃ ξ₅₀² | ξ₅₃ ξ₅₀²
qrMul(nP(23,2), nP(23,2), s(180), qr, stack); //  | ξ₄₉ ξ₅₂² | ξ₄₉ ξ₅₂²
qrSub(nP(23,2), s(173), nP(23,2), qr);
qrMul(s(173), nP(24,2), nP(23,2), qr, stack); //  | ξ₁₀₂
qrMul(s(181), nP(24,2), nP(25,2), qr, stack); //  | ξ₅₁ ξ₅₃ | ξ₅₁ ξ₅₃
qrMul(s(182), s(181), s(177), qr, stack); //  | ξ₅₁ ξ₅₃   ξ₅₁²
qrMul(s(183), s(178), s(180), qr, stack); //  | ξ₅₀ ξ₅₂   ξ₅₂²
qrMul(s(183), s(1), s(183), qr, stack);
qrSub(nP(50,2), s(182), s(183), qr); //  | ξ₁₀₃
qrSqr(s(184), nP(25,2), qr, stack); //  | ξ₅₃² | ξ₅₃² | ξ₅₃² | ξ₅₃² | ξ₅₃²
qrMul(s(185), s(89), s(177), qr, stack); //  | ξ₅₄ ξ₅₁²
qrMul(s(82), s(82), s(184), qr, stack); //  | ξ₅₀ ξ₅₃²
qrSub(s(82), s(185), s(82), qr);
qrMul(s(82), s(3), s(82), qr, stack); //  | ξ₁₀₄
qrMul(s(185), s(3), s(89), qr, stack); //  | ξ₅₂ ξ₅₄ | ξ₅₂ ξ₅₄ | ξ₅₂ ξ₅₄
qrMul(s(186), s(185), s(180), qr, stack); //  | ξ₅₂ ξ₅₄   ξ₅₂²
qrMul(s(181), s(181), s(184), qr, stack); //  | ξ₅₁ ξ₅₃   ξ₅₃²
qrMul(s(186), s(1), s(186), qr, stack);
qrSub(nP(51,2), s(186), s(181), qr); //  | ξ₁₀₅
qrSqr(s(187), s(89), qr, stack); //  | ξ₅₄² | ξ₅₄² | ξ₅₄² | ξ₅₄² | ξ₅₄² | ξ₅₄²
qrMul(s(180), nP(26,2), s(180), qr, stack); //  | ξ₅₅ ξ₅₂² | ξ₅₅ ξ₅₂²
qrMul(nP(24,2), nP(24,2), s(187), qr, stack); //  | ξ₅₁ ξ₅₄² | ξ₅₁ ξ₅₄²
qrSub(nP(24,2), s(180), nP(24,2), qr);
qrMul(s(180), nP(25,2), nP(24,2), qr, stack); //  | ξ₁₀₆
qrMul(s(188), nP(25,2), nP(26,2), qr, stack); //  | ξ₅₃ ξ₅₅ | ξ₅₃ ξ₅₅
qrMul(s(189), s(188), s(184), qr, stack); //  | ξ₅₃ ξ₅₅   ξ₅₃²
qrMul(s(190), s(185), s(187), qr, stack); //  | ξ₅₂ ξ₅₄   ξ₅₄²
qrMul(s(190), s(1), s(190), qr, stack);
qrSub(nP(52,2), s(189), s(190), qr); //  | ξ₁₀₇
qrSqr(s(191), nP(26,2), qr, stack); //  | ξ₅₅² | ξ₅₅² | ξ₅₅² | ξ₅₅² | ξ₅₅²
qrMul(s(192), s(40), s(184), qr, stack); //  | ξ₅₆ ξ₅₃²
qrMul(s(3), s(3), s(191), qr, stack); //  | ξ₅₂ ξ₅₅²
qrSub(s(3), s(192), s(3), qr);
qrMul(s(3), s(89), s(3), qr, stack); //  | ξ₁₀₈
qrMul(s(192), s(89), s(40), qr, stack); //  | ξ₅₄ ξ₅₆ | ξ₅₄ ξ₅₆ | ξ₅₄ ξ₅₆
qrMul(s(193), s(192), s(187), qr, stack); //  | ξ₅₄ ξ₅₆   ξ₅₄²
qrMul(s(188), s(188), s(191), qr, stack); //  | ξ₅₃ ξ₅₅   ξ₅₅²
qrMul(s(193), s(1), s(193), qr, stack);
qrSub(nP(53,2), s(193), s(188), qr); //  | ξ₁₀₉
qrSqr(s(194), s(40), qr, stack); //  | ξ₅₆² | ξ₅₆² | ξ₅₆² | ξ₅₆² | ξ₅₆² | ξ₅₆²
qrMul(s(187), nP(27,2), s(187), qr, stack); //  | ξ₅₇ ξ₅₄² | ξ₅₇ ξ₅₄²
qrMul(nP(25,2), nP(25,2), s(194), qr, stack); //  | ξ₅₃ ξ₅₆² | ξ₅₃ ξ₅₆²
qrSub(nP(25,2), s(187), nP(25,2), qr);
qrMul(s(187), nP(26,2), nP(25,2), qr, stack); //  | ξ₁₁₀
qrMul(s(195), nP(26,2), nP(27,2), qr, stack); //  | ξ₅₅ ξ₅₇ | ξ₅₅ ξ₅₇
qrMul(s(196), s(195), s(191), qr, stack); //  | ξ₅₅ ξ₅₇   ξ₅₅²
qrMul(s(197), s(192), s(194), qr, stack); //  | ξ₅₄ ξ₅₆   ξ₅₆²
qrMul(s(197), s(1), s(197), qr, stack);
qrSub(nP(54,2), s(196), s(197), qr); //  | ξ₁₁₁
qrSqr(s(198), nP(27,2), qr, stack); //  | ξ₅₇² | ξ₅₇² | ξ₅₇² | ξ₅₇² | ξ₅₇²
qrMul(s(199), s(96), s(191), qr, stack); //  | ξ₅₈ ξ₅₅²
qrMul(s(89), s(89), s(198), qr, stack); //  | ξ₅₄ ξ₅₇²
qrSub(s(89), s(199), s(89), qr);
qrMul(s(89), s(40), s(89), qr, stack); //  | ξ₁₁₂
qrMul(s(199), s(40), s(96), qr, stack); //  | ξ₅₆ ξ₅₈ | ξ₅₆ ξ₅₈ | ξ₅₆ ξ₅₈
qrMul(s(200), s(199), s(194), qr, stack); //  | ξ₅₆ ξ₅₈   ξ₅₆²
qrMul(s(195), s(195), s(198), qr, stack); //  | ξ₅₅ ξ₅₇   ξ₅₇²
qrMul(s(200), s(1), s(200), qr, stack);
qrSub(nP(55,2), s(200), s(195), qr); //  | ξ₁₁₃
qrSqr(s(201), s(96), qr, stack); //  | ξ₅₈² | ξ₅₈² | ξ₅₈² | ξ₅₈² | ξ₅₈² | ξ₅₈²
qrMul(s(194), nP(28,2), s(194), qr, stack); //  | ξ₅₉ ξ₅₆² | ξ₅₉ ξ₅₆²
qrMul(nP(26,2), nP(26,2), s(201), qr, stack); //  | ξ₅₅ ξ₅₈² | ξ₅₅ ξ₅₈²
qrSub(nP(26,2), s(194), nP(26,2), qr);
qrMul(s(194), nP(27,2), nP(26,2), qr, stack); //  | ξ₁₁₄
qrMul(s(202), nP(27,2), nP(28,2), qr, stack); //  | ξ₅₇ ξ₅₉ | ξ₅₇ ξ₅₉
qrMul(s(203), s(202), s(198), qr, stack); //  | ξ₅₇ ξ₅₉   ξ₅₇²
qrMul(s(204), s(199), s(201), qr, stack); //  | ξ₅₆ ξ₅₈   ξ₅₈²
qrMul(s(204), s(1), s(204), qr, stack);
qrSub(nP(56,2), s(203), s(204), qr); //  | ξ₁₁₅
qrSqr(s(205), nP(28,2), qr, stack); //  | ξ₅₉² | ξ₅₉² | ξ₅₉² | ξ₅₉² | ξ₅₉²
qrMul(s(206), s(24), s(198), qr, stack); //  | ξ₆₀ ξ₅₇²
qrMul(s(40), s(40), s(205), qr, stack); //  | ξ₅₆ ξ₅₉²
qrSub(s(40), s(206), s(40), qr);
qrMul(s(40), s(96), s(40), qr, stack); //  | ξ₁₁₆
qrMul(s(206), s(96), s(24), qr, stack); //  | ξ₅₈ ξ₆₀ | ξ₅₈ ξ₆₀ | ξ₅₈ ξ₆₀
qrMul(s(207), s(206), s(201), qr, stack); //  | ξ₅₈ ξ₆₀   ξ₅₈²
qrMul(s(202), s(202), s(205), qr, stack); //  | ξ₅₇ ξ₅₉   ξ₅₉²
qrMul(s(207), s(1), s(207), qr, stack);
qrSub(nP(57,2), s(207), s(202), qr); //  | ξ₁₁₇
qrSqr(s(208), s(24), qr, stack); //  | ξ₆₀² | ξ₆₀² | ξ₆₀² | ξ₆₀² | ξ₆₀² | ξ₆₀²
qrMul(s(201), nP(29,2), s(201), qr, stack); //  | ξ₆₁ ξ₅₈² | ξ₆₁ ξ₅₈²
qrMul(nP(27,2), nP(27,2), s(208), qr, stack); //  | ξ₅₇ ξ₆₀² | ξ₅₇ ξ₆₀²
qrSub(nP(27,2), s(201), nP(27,2), qr);
qrMul(s(201), nP(28,2), nP(27,2), qr, stack); //  | ξ₁₁₈
qrMul(s(209), nP(28,2), nP(29,2), qr, stack); //  | ξ₅₉ ξ₆₁ | ξ₅₉ ξ₆₁
qrMul(s(210), s(209), s(205), qr, stack); //  | ξ₅₉ ξ₆₁   ξ₅₉²
qrMul(s(211), s(206), s(208), qr, stack); //  | ξ₅₈ ξ₆₀   ξ₆₀²
qrMul(s(211), s(1), s(211), qr, stack);
qrSub(nP(58,2), s(210), s(211), qr); //  | ξ₁₁₉
qrSqr(s(212), nP(29,2), qr, stack); //  | ξ₆₁² | ξ₆₁² | ξ₆₁² | ξ₆₁² | ξ₆₁²
qrMul(s(213), s(103), s(205), qr, stack); //  | ξ₆₂ ξ₅₉²
qrMul(s(96), s(96), s(212), qr, stack); //  | ξ₅₈ ξ₆₁²
qrSub(s(96), s(213), s(96), qr);
qrMul(s(96), s(24), s(96), qr, stack); //  | ξ₁₂₀
qrMul(s(213), s(24), s(103), qr, stack); //  | ξ₆₀ ξ₆₂ | ξ₆₀ ξ₆₂ | ξ₆₀ ξ₆₂
qrMul(s(214), s(213), s(208), qr, stack); //  | ξ₆₀ ξ₆₂   ξ₆₀²
qrMul(s(209), s(209), s(212), qr, stack); //  | ξ₅₉ ξ₆₁   ξ₆₁²
qrMul(s(214), s(1), s(214), qr, stack);
qrSub(nP(59,2), s(214), s(209), qr); //  | ξ₁₂₁
qrSqr(s(215), s(103), qr, stack); //  | ξ₆₂² | ξ₆₂² | ξ₆₂² | ξ₆₂² | ξ₆₂² | ξ₆₂²
qrMul(s(208), nP(30,2), s(208), qr, stack); //  | ξ₆₃ ξ₆₀² | ξ₆₃ ξ₆₀²
qrMul(nP(28,2), nP(28,2), s(215), qr, stack); //  | ξ₅₉ ξ₆₂² | ξ₅₉ ξ₆₂²
qrSub(nP(28,2), s(208), nP(28,2), qr);
qrMul(s(208), nP(29,2), nP(28,2), qr, stack); //  | ξ₁₂₂
qrMul(s(216), nP(29,2), nP(30,2), qr, stack); //  | ξ₆₁ ξ₆₃ | ξ₆₁ ξ₆₃
qrMul(s(217), s(216), s(212), qr, stack); //  | ξ₆₁ ξ₆₃   ξ₆₁²
qrMul(s(218), s(213), s(215), qr, stack); //  | ξ₆₀ ξ₆₂   ξ₆₂²
qrMul(s(218), s(1), s(218), qr, stack);
qrSub(nP(60,2), s(217), s(218), qr); //  | ξ₁₂₃
qrSqr(s(219), nP(30,2), qr, stack); //  | ξ₆₃² | ξ₆₃² | ξ₆₃² | ξ₆₃² | ξ₆₃²
qrMul(s(220), s(47), s(212), qr, stack); //  | ξ₆₄ ξ₆₁²
qrMul(s(24), s(24), s(219), qr, stack); //  | ξ₆₀ ξ₆₃²
qrSub(s(24), s(220), s(24), qr);
qrMul(s(24), s(103), s(24), qr, stack); //  | ξ₁₂₄
qrMul(s(220), s(103), s(47), qr, stack); //  | ξ₆₂ ξ₆₄ | ξ₆₂ ξ₆₄ | ξ₆₂ ξ₆₄
qrMul(s(221), s(220), s(215), qr, stack); //  | ξ₆₂ ξ₆₄   ξ₆₂²
qrMul(s(216), s(216), s(219), qr, stack); //  | ξ₆₁ ξ₆₃   ξ₆₃²
qrMul(s(221), s(1), s(221), qr, stack);
qrSub(nP(61,2), s(221), s(216), qr); //  | ξ₁₂₅
qrSqr(s(222), s(47), qr, stack); //  | ξ₆₄² | ξ₆₄² | ξ₆₄² | ξ₆₄² | ξ₆₄²
qrMul(s(215), nP(31,2), s(215), qr, stack); //  | ξ₆₅ ξ₆₂² | ξ₆₅ ξ₆₂²
qrMul(nP(29,2), nP(29,2), s(222), qr, stack); //  | ξ₆₁ ξ₆₄² | ξ₆₁ ξ₆₄²
qrSub(nP(29,2), s(215), nP(29,2), qr);
qrMul(s(215), nP(30,2), nP(29,2), qr, stack); //  | ξ₁₂₆
qrMul(s(223), nP(30,2), nP(31,2), qr, stack); //  | ξ₆₃ ξ₆₅ | ξ₆₃ ξ₆₅
qrMul(s(224), s(223), s(219), qr, stack); //  | ξ₆₃ ξ₆₅   ξ₆₃²
qrMul(s(225), s(220), s(222), qr, stack); //  | ξ₆₂ ξ₆₄   ξ₆₄²
qrMul(s(225), s(1), s(225), qr, stack);
qrSub(nP(62,2), s(224), s(225), qr); //  | ξ₁₂₇
qrSqr(s(226), nP(31,2), qr, stack); //  | ξ₆₅² | ξ₆₅² | ξ₆₅²
qrMul(s(227), s(110), s(219), qr, stack); //  | ξ₆₆ ξ₆₃²
qrMul(s(103), s(103), s(226), qr, stack); //  | ξ₆₂ ξ₆₅²
qrSub(s(103), s(227), s(103), qr);
qrMul(s(103), s(47), s(103), qr, stack); //  | ξ₁₂₈
qrMul(s(47), s(47), s(110), qr, stack); //  | ξ₆₄ ξ₆₆ | ξ₆₄ ξ₆₆
qrMul(s(227), s(47), s(222), qr, stack); //  | ξ₆₄ ξ₆₆   ξ₆₄²
qrMul(s(223), s(223), s(226), qr, stack); //  | ξ₆₃ ξ₆₅   ξ₆₅²
qrMul(s(227), s(1), s(227), qr, stack);
qrSub(s(223), s(227), s(223), qr); //  | ξ₁₂₉
qrSqr(s(227), s(215), qr, stack); //  | ξ₁₂₆² | ξ₁₂₆²
qrSqr(s(1), nP(62,2), qr, stack); //  | ξ₁₂₇²
qrSqr(s(228), s(103), qr, stack); //  | ξ₁₂₈²
qrMul(s(103), s(215), s(103), qr, stack); //  | ξ₁₂₆ ξ₁₂₈
qrMul(s(1), x, s(1), qr, stack);
qrMul(s(103), s(0), s(103), qr, stack);
qrSub(nP(62,0), s(1), s(103), qr); //  | [127]Px
qrMul(s(223), s(223), s(227), qr, stack); //  | ξ₁₂₉ ξ₁₂₆²
qrMul(s(228), nP(61,2), s(228), qr, stack); //  | ξ₁₂₅ ξ₁₂₈²
qrSub(s(228), s(223), s(228), qr);
qrMul(nP(62,1), y, s(228), qr, stack); //  | [127]Py
qrSqr(s(223), s(24), qr, stack); //  | ξ₁₂₄² | ξ₁₂₄²
qrSqr(s(229), nP(61,2), qr, stack); //  | ξ₁₂₅²
qrMul(s(215), s(24), s(215), qr, stack); //  | ξ₁₂₄ ξ₁₂₆
qrMul(s(229), x, s(229), qr, stack);
qrMul(s(215), s(0), s(215), qr, stack);
qrSub(nP(61,0), s(229), s(215), qr); //  | [125]Px
qrMul(nP(62,2), nP(62,2), s(223), qr, stack); //  | ξ₁₂₇ ξ₁₂₄²
qrMul(s(227), nP(60,2), s(227), qr, stack); //  | ξ₁₂₃ ξ₁₂₆²
qrSub(s(227), nP(62,2), s(227), qr);
qrMul(nP(61,1), y, s(227), qr, stack); //  | [125]Py
qrSqr(nP(62,2), s(208), qr, stack); //  | ξ₁₂₂² | ξ₁₂₂²
qrSqr(s(230), nP(60,2), qr, stack); //  | ξ₁₂₃²
qrMul(s(24), s(208), s(24), qr, stack); //  | ξ₁₂₂ ξ₁₂₄
qrMul(s(230), x, s(230), qr, stack);
qrMul(s(24), s(0), s(24), qr, stack);
qrSub(nP(60,0), s(230), s(24), qr); //  | [123]Px
qrMul(nP(61,2), nP(61,2), nP(62,2), qr, stack); //  | ξ₁₂₅ ξ₁₂₂²
qrMul(s(223), nP(59,2), s(223), qr, stack); //  | ξ₁₂₁ ξ₁₂₄²
qrSub(s(223), nP(61,2), s(223), qr);
qrMul(nP(60,1), y, s(223), qr, stack); //  | [123]Py
qrSqr(nP(61,2), s(96), qr, stack); //  | ξ₁₂₀² | ξ₁₂₀²
qrSqr(s(231), nP(59,2), qr, stack); //  | ξ₁₂₁²
qrMul(s(208), s(96), s(208), qr, stack); //  | ξ₁₂₀ ξ₁₂₂
qrMul(s(231), x, s(231), qr, stack);
qrMul(s(208), s(0), s(208), qr, stack);
qrSub(nP(59,0), s(231), s(208), qr); //  | [121]Px
qrMul(nP(60,2), nP(60,2), nP(61,2), qr, stack); //  | ξ₁₂₃ ξ₁₂₀²
qrMul(nP(62,2), nP(58,2), nP(62,2), qr, stack); //  | ξ₁₁₉ ξ₁₂₂²
qrSub(nP(62,2), nP(60,2), nP(62,2), qr);
qrMul(nP(59,1), y, nP(62,2), qr, stack); //  | [121]Py
qrSqr(nP(60,2), s(201), qr, stack); //  | ξ₁₁₈² | ξ₁₁₈²
qrSqr(s(232), nP(58,2), qr, stack); //  | ξ₁₁₉²
qrMul(s(96), s(201), s(96), qr, stack); //  | ξ₁₁₈ ξ₁₂₀
qrMul(s(232), x, s(232), qr, stack);
qrMul(s(96), s(0), s(96), qr, stack);
qrSub(nP(58,0), s(232), s(96), qr); //  | [119]Px
qrMul(nP(59,2), nP(59,2), nP(60,2), qr, stack); //  | ξ₁₂₁ ξ₁₁₈²
qrMul(nP(61,2), nP(57,2), nP(61,2), qr, stack); //  | ξ₁₁₇ ξ₁₂₀²
qrSub(nP(61,2), nP(59,2), nP(61,2), qr);
qrMul(nP(58,1), y, nP(61,2), qr, stack); //  | [119]Py
qrSqr(nP(59,2), s(40), qr, stack); //  | ξ₁₁₆² | ξ₁₁₆²
qrSqr(s(233), nP(57,2), qr, stack); //  | ξ₁₁₇²
qrMul(s(201), s(40), s(201), qr, stack); //  | ξ₁₁₆ ξ₁₁₈
qrMul(s(233), x, s(233), qr, stack);
qrMul(s(201), s(0), s(201), qr, stack);
qrSub(nP(57,0), s(233), s(201), qr); //  | [117]Px
qrMul(nP(58,2), nP(58,2), nP(59,2), qr, stack); //  | ξ₁₁₉ ξ₁₁₆²
qrMul(nP(60,2), nP(56,2), nP(60,2), qr, stack); //  | ξ₁₁₅ ξ₁₁₈²
qrSub(nP(60,2), nP(58,2), nP(60,2), qr);
qrMul(nP(57,1), y, nP(60,2), qr, stack); //  | [117]Py
qrSqr(nP(58,2), s(194), qr, stack); //  | ξ₁₁₄² | ξ₁₁₄²
qrSqr(s(234), nP(56,2), qr, stack); //  | ξ₁₁₅²
qrMul(s(40), s(194), s(40), qr, stack); //  | ξ₁₁₄ ξ₁₁₆
qrMul(s(234), x, s(234), qr, stack);
qrMul(s(40), s(0), s(40), qr, stack);
qrSub(nP(56,0), s(234), s(40), qr); //  | [115]Px
qrMul(nP(57,2), nP(57,2), nP(58,2), qr, stack); //  | ξ₁₁₇ ξ₁₁₄²
qrMul(nP(59,2), nP(55,2), nP(59,2), qr, stack); //  | ξ₁₁₃ ξ₁₁₆²
qrSub(nP(59,2), nP(57,2), nP(59,2), qr);
qrMul(nP(56,1), y, nP(59,2), qr, stack); //  | [115]Py
qrSqr(nP(57,2), s(89), qr, stack); //  | ξ₁₁₂² | ξ₁₁₂²
qrSqr(s(235), nP(55,2), qr, stack); //  | ξ₁₁₃²
qrMul(s(194), s(89), s(194), qr, stack); //  | ξ₁₁₂ ξ₁₁₄
qrMul(s(235), x, s(235), qr, stack);
qrMul(s(194), s(0), s(194), qr, stack);
qrSub(nP(55,0), s(235), s(194), qr); //  | [113]Px
qrMul(nP(56,2), nP(56,2), nP(57,2), qr, stack); //  | ξ₁₁₅ ξ₁₁₂²
qrMul(nP(58,2), nP(54,2), nP(58,2), qr, stack); //  | ξ₁₁₁ ξ₁₁₄²
qrSub(nP(58,2), nP(56,2), nP(58,2), qr);
qrMul(nP(55,1), y, nP(58,2), qr, stack); //  | [113]Py
qrSqr(nP(56,2), s(187), qr, stack); //  | ξ₁₁₀² | ξ₁₁₀²
qrSqr(s(236), nP(54,2), qr, stack); //  | ξ₁₁₁²
qrMul(s(89), s(187), s(89), qr, stack); //  | ξ₁₁₀ ξ₁₁₂
qrMul(s(236), x, s(236), qr, stack);
qrMul(s(89), s(0), s(89), qr, stack);
qrSub(nP(54,0), s(236), s(89), qr); //  | [111]Px
qrMul(nP(55,2), nP(55,2), nP(56,2), qr, stack); //  | ξ₁₁₃ ξ₁₁₀²
qrMul(nP(57,2), nP(53,2), nP(57,2), qr, stack); //  | ξ₁₀₉ ξ₁₁₂²
qrSub(nP(57,2), nP(55,2), nP(57,2), qr);
qrMul(nP(54,1), y, nP(57,2), qr, stack); //  | [111]Py
qrSqr(nP(55,2), s(3), qr, stack); //  | ξ₁₀₈² | ξ₁₀₈²
qrSqr(s(237), nP(53,2), qr, stack); //  | ξ₁₀₉²
qrMul(s(187), s(3), s(187), qr, stack); //  | ξ₁₀₈ ξ₁₁₀
qrMul(s(237), x, s(237), qr, stack);
qrMul(s(187), s(0), s(187), qr, stack);
qrSub(nP(53,0), s(237), s(187), qr); //  | [109]Px
qrMul(nP(54,2), nP(54,2), nP(55,2), qr, stack); //  | ξ₁₁₁ ξ₁₀₈²
qrMul(nP(56,2), nP(52,2), nP(56,2), qr, stack); //  | ξ₁₀₇ ξ₁₁₀²
qrSub(nP(56,2), nP(54,2), nP(56,2), qr);
qrMul(nP(53,1), y, nP(56,2), qr, stack); //  | [109]Py
qrSqr(nP(54,2), s(180), qr, stack); //  | ξ₁₀₆² | ξ₁₀₆²
qrSqr(s(238), nP(52,2), qr, stack); //  | ξ₁₀₇²
qrMul(s(3), s(180), s(3), qr, stack); //  | ξ₁₀₆ ξ₁₀₈
qrMul(s(238), x, s(238), qr, stack);
qrMul(s(3), s(0), s(3), qr, stack);
qrSub(nP(52,0), s(238), s(3), qr); //  | [107]Px
qrMul(nP(53,2), nP(53,2), nP(54,2), qr, stack); //  | ξ₁₀₉ ξ₁₀₆²
qrMul(nP(55,2), nP(51,2), nP(55,2), qr, stack); //  | ξ₁₀₅ ξ₁₀₈²
qrSub(nP(55,2), nP(53,2), nP(55,2), qr);
qrMul(nP(52,1), y, nP(55,2), qr, stack); //  | [107]Py
qrSqr(nP(53,2), s(82), qr, stack); //  | ξ₁₀₄² | ξ₁₀₄²
qrSqr(s(239), nP(51,2), qr, stack); //  | ξ₁₀₅²
qrMul(s(180), s(82), s(180), qr, stack); //  | ξ₁₀₄ ξ₁₀₆
qrMul(s(239), x, s(239), qr, stack);
qrMul(s(180), s(0), s(180), qr, stack);
qrSub(nP(51,0), s(239), s(180), qr); //  | [105]Px
qrMul(nP(52,2), nP(52,2), nP(53,2), qr, stack); //  | ξ₁₀₇ ξ₁₀₄²
qrMul(nP(54,2), nP(50,2), nP(54,2), qr, stack); //  | ξ₁₀₃ ξ₁₀₆²
qrSub(nP(54,2), nP(52,2), nP(54,2), qr);
qrMul(nP(51,1), y, nP(54,2), qr, stack); //  | [105]Py
qrSqr(nP(52,2), s(173), qr, stack); //  | ξ₁₀₂² | ξ₁₀₂²
qrSqr(s(240), nP(50,2), qr, stack); //  | ξ₁₀₃²
qrMul(s(82), s(173), s(82), qr, stack); //  | ξ₁₀₂ ξ₁₀₄
qrMul(s(240), x, s(240), qr, stack);
qrMul(s(82), s(0), s(82), qr, stack);
qrSub(nP(50,0), s(240), s(82), qr); //  | [103]Px
qrMul(nP(51,2), nP(51,2), nP(52,2), qr, stack); //  | ξ₁₀₅ ξ₁₀₂²
qrMul(nP(53,2), nP(49,2), nP(53,2), qr, stack); //  | ξ₁₀₁ ξ₁₀₄²
qrSub(nP(53,2), nP(51,2), nP(53,2), qr);
qrMul(nP(50,1), y, nP(53,2), qr, stack); //  | [103]Py
qrSqr(nP(51,2), s(33), qr, stack); //  | ξ₁₀₀² | ξ₁₀₀²
qrSqr(s(241), nP(49,2), qr, stack); //  | ξ₁₀₁²
qrMul(s(173), s(33), s(173), qr, stack); //  | ξ₁₀₀ ξ₁₀₂
qrMul(s(241), x, s(241), qr, stack);
qrMul(s(173), s(0), s(173), qr, stack);
qrSub(nP(49,0), s(241), s(173), qr); //  | [101]Px
qrMul(nP(50,2), nP(50,2), nP(51,2), qr, stack); //  | ξ₁₀₃ ξ₁₀₀²
qrMul(nP(52,2), nP(48,2), nP(52,2), qr, stack); //  | ξ₉₉ ξ₁₀₂²
qrSub(nP(52,2), nP(50,2), nP(52,2), qr);
qrMul(nP(49,1), y, nP(52,2), qr, stack); //  | [101]Py
qrSqr(nP(50,2), s(166), qr, stack); //  | ξ₉₈² | ξ₉₈²
qrSqr(s(242), nP(48,2), qr, stack); //  | ξ₉₉²
qrMul(s(33), s(166), s(33), qr, stack); //  | ξ₉₈ ξ₁₀₀
qrMul(s(242), x, s(242), qr, stack);
qrMul(s(33), s(0), s(33), qr, stack);
qrSub(nP(48,0), s(242), s(33), qr); //  | [99]Px
qrMul(nP(49,2), nP(49,2), nP(50,2), qr, stack); //  | ξ₁₀₁ ξ₉₈²
qrMul(nP(51,2), nP(47,2), nP(51,2), qr, stack); //  | ξ₉₇ ξ₁₀₀²
qrSub(nP(51,2), nP(49,2), nP(51,2), qr);
qrMul(nP(48,1), y, nP(51,2), qr, stack); //  | [99]Py
qrSqr(nP(49,2), s(75), qr, stack); //  | ξ₉₆² | ξ₉₆²
qrSqr(s(243), nP(47,2), qr, stack); //  | ξ₉₇²
qrMul(s(166), s(75), s(166), qr, stack); //  | ξ₉₆ ξ₉₈
qrMul(s(243), x, s(243), qr, stack);
qrMul(s(166), s(0), s(166), qr, stack);
qrSub(nP(47,0), s(243), s(166), qr); //  | [97]Px
qrMul(nP(48,2), nP(48,2), nP(49,2), qr, stack); //  | ξ₉₉ ξ₉₆²
qrMul(nP(50,2), nP(46,2), nP(50,2), qr, stack); //  | ξ₉₅ ξ₉₈²
qrSub(nP(50,2), nP(48,2), nP(50,2), qr);
qrMul(nP(47,1), y, nP(50,2), qr, stack); //  | [97]Py
qrSqr(nP(48,2), s(159), qr, stack); //  | ξ₉₄² | ξ₉₄²
qrSqr(s(244), nP(46,2), qr, stack); //  | ξ₉₅²
qrMul(s(75), s(159), s(75), qr, stack); //  | ξ₉₄ ξ₉₆
qrMul(s(244), x, s(244), qr, stack);
qrMul(s(75), s(0), s(75), qr, stack);
qrSub(nP(46,0), s(244), s(75), qr); //  | [95]Px
qrMul(nP(47,2), nP(47,2), nP(48,2), qr, stack); //  | ξ₉₇ ξ₉₄²
qrMul(nP(49,2), nP(45,2), nP(49,2), qr, stack); //  | ξ₉₃ ξ₉₆²
qrSub(nP(49,2), nP(47,2), nP(49,2), qr);
qrMul(nP(46,1), y, nP(49,2), qr, stack); //  | [95]Py
qrSqr(nP(47,2), s(15), qr, stack); //  | ξ₉₂² | ξ₉₂²
qrSqr(s(245), nP(45,2), qr, stack); //  | ξ₉₃²
qrMul(s(159), s(15), s(159), qr, stack); //  | ξ₉₂ ξ₉₄
qrMul(s(245), x, s(245), qr, stack);
qrMul(s(159), s(0), s(159), qr, stack);
qrSub(nP(45,0), s(245), s(159), qr); //  | [93]Px
qrMul(nP(46,2), nP(46,2), nP(47,2), qr, stack); //  | ξ₉₅ ξ₉₂²
qrMul(nP(48,2), nP(44,2), nP(48,2), qr, stack); //  | ξ₉₁ ξ₉₄²
qrSub(nP(48,2), nP(46,2), nP(48,2), qr);
qrMul(nP(45,1), y, nP(48,2), qr, stack); //  | [93]Py
qrSqr(nP(46,2), s(152), qr, stack); //  | ξ₉₀² | ξ₉₀²
qrSqr(s(246), nP(44,2), qr, stack); //  | ξ₉₁²
qrMul(s(15), s(152), s(15), qr, stack); //  | ξ₉₀ ξ₉₂
qrMul(s(246), x, s(246), qr, stack);
qrMul(s(15), s(0), s(15), qr, stack);
qrSub(nP(44,0), s(246), s(15), qr); //  | [91]Px
qrMul(nP(45,2), nP(45,2), nP(46,2), qr, stack); //  | ξ₉₃ ξ₉₀²
qrMul(nP(47,2), nP(43,2), nP(47,2), qr, stack); //  | ξ₈₉ ξ₉₂²
qrSub(nP(47,2), nP(45,2), nP(47,2), qr);
qrMul(nP(44,1), y, nP(47,2), qr, stack); //  | [91]Py
qrSqr(nP(45,2), s(68), qr, stack); //  | ξ₈₈² | ξ₈₈²
qrSqr(s(247), nP(43,2), qr, stack); //  | ξ₈₉²
qrMul(s(152), s(68), s(152), qr, stack); //  | ξ₈₈ ξ₉₀
qrMul(s(247), x, s(247), qr, stack);
qrMul(s(152), s(0), s(152), qr, stack);
qrSub(nP(43,0), s(247), s(152), qr); //  | [89]Px
qrMul(nP(44,2), nP(44,2), nP(45,2), qr, stack); //  | ξ₉₁ ξ₈₈²
qrMul(nP(46,2), nP(42,2), nP(46,2), qr, stack); //  | ξ₈₇ ξ₉₀²
qrSub(nP(46,2), nP(44,2), nP(46,2), qr);
qrMul(nP(43,1), y, nP(46,2), qr, stack); //  | [89]Py
qrSqr(nP(44,2), s(145), qr, stack); //  | ξ₈₆² | ξ₈₆²
qrSqr(s(248), nP(42,2), qr, stack); //  | ξ₈₇²
qrMul(s(68), s(145), s(68), qr, stack); //  | ξ₈₆ ξ₈₈
qrMul(s(248), x, s(248), qr, stack);
qrMul(s(68), s(0), s(68), qr, stack);
qrSub(nP(42,0), s(248), s(68), qr); //  | [87]Px
qrMul(nP(43,2), nP(43,2), nP(44,2), qr, stack); //  | ξ₈₉ ξ₈₆²
qrMul(nP(45,2), nP(41,2), nP(45,2), qr, stack); //  | ξ₈₅ ξ₈₈²
qrSub(nP(45,2), nP(43,2), nP(45,2), qr);
qrMul(nP(42,1), y, nP(45,2), qr, stack); //  | [87]Py
qrSqr(nP(43,2), s(26), qr, stack); //  | ξ₈₄² | ξ₈₄²
qrSqr(s(249), nP(41,2), qr, stack); //  | ξ₈₅²
qrMul(s(145), s(26), s(145), qr, stack); //  | ξ₈₄ ξ₈₆
qrMul(s(249), x, s(249), qr, stack);
qrMul(s(145), s(0), s(145), qr, stack);
qrSub(nP(41,0), s(249), s(145), qr); //  | [85]Px
qrMul(nP(42,2), nP(42,2), nP(43,2), qr, stack); //  | ξ₈₇ ξ₈₄²
qrMul(nP(44,2), nP(40,2), nP(44,2), qr, stack); //  | ξ₈₃ ξ₈₆²
qrSub(nP(44,2), nP(42,2), nP(44,2), qr);
qrMul(nP(41,1), y, nP(44,2), qr, stack); //  | [85]Py
qrSqr(nP(42,2), s(138), qr, stack); //  | ξ₈₂² | ξ₈₂²
qrSqr(s(250), nP(40,2), qr, stack); //  | ξ₈₃²
qrMul(s(26), s(138), s(26), qr, stack); //  | ξ₈₂ ξ₈₄
qrMul(s(250), x, s(250), qr, stack);
qrMul(s(26), s(0), s(26), qr, stack);
qrSub(nP(40,0), s(250), s(26), qr); //  | [83]Px
qrMul(nP(41,2), nP(41,2), nP(42,2), qr, stack); //  | ξ₈₅ ξ₈₂²
qrMul(nP(43,2), nP(39,2), nP(43,2), qr, stack); //  | ξ₈₁ ξ₈₄²
qrSub(nP(43,2), nP(41,2), nP(43,2), qr);
qrMul(nP(40,1), y, nP(43,2), qr, stack); //  | [83]Py
qrSqr(nP(41,2), s(61), qr, stack); //  | ξ₈₀² | ξ₈₀²
qrSqr(s(251), nP(39,2), qr, stack); //  | ξ₈₁²
qrMul(s(138), s(61), s(138), qr, stack); //  | ξ₈₀ ξ₈₂
qrMul(s(251), x, s(251), qr, stack);
qrMul(s(138), s(0), s(138), qr, stack);
qrSub(nP(39,0), s(251), s(138), qr); //  | [81]Px
qrMul(nP(40,2), nP(40,2), nP(41,2), qr, stack); //  | ξ₈₃ ξ₈₀²
qrMul(nP(42,2), nP(38,2), nP(42,2), qr, stack); //  | ξ₇₉ ξ₈₂²
qrSub(nP(42,2), nP(40,2), nP(42,2), qr);
qrMul(nP(39,1), y, nP(42,2), qr, stack); //  | [81]Py
qrSqr(nP(40,2), s(131), qr, stack); //  | ξ₇₈² | ξ₇₈²
qrSqr(s(252), nP(38,2), qr, stack); //  | ξ₇₉²
qrMul(s(61), s(131), s(61), qr, stack); //  | ξ₇₈ ξ₈₀
qrMul(s(252), x, s(252), qr, stack);
qrMul(s(61), s(0), s(61), qr, stack);
qrSub(nP(38,0), s(252), s(61), qr); //  | [79]Px
qrMul(nP(39,2), nP(39,2), nP(40,2), qr, stack); //  | ξ₈₁ ξ₇₈²
qrMul(nP(41,2), nP(37,2), nP(41,2), qr, stack); //  | ξ₇₇ ξ₈₀²
qrSub(nP(41,2), nP(39,2), nP(41,2), qr);
qrMul(nP(38,1), y, nP(41,2), qr, stack); //  | [79]Py
qrSqr(nP(39,2), s(10), qr, stack); //  | ξ₇₆² | ξ₇₆²
qrSqr(s(253), nP(37,2), qr, stack); //  | ξ₇₇²
qrMul(s(131), s(10), s(131), qr, stack); //  | ξ₇₆ ξ₇₈
qrMul(s(253), x, s(253), qr, stack);
qrMul(s(131), s(0), s(131), qr, stack);
qrSub(nP(37,0), s(253), s(131), qr); //  | [77]Px
qrMul(nP(38,2), nP(38,2), nP(39,2), qr, stack); //  | ξ₇₉ ξ₇₆²
qrMul(nP(40,2), nP(36,2), nP(40,2), qr, stack); //  | ξ₇₅ ξ₇₈²
qrSub(nP(40,2), nP(38,2), nP(40,2), qr);
qrMul(nP(37,1), y, nP(40,2), qr, stack); //  | [77]Py
qrSqr(nP(38,2), s(124), qr, stack); //  | ξ₇₄² | ξ₇₄²
qrSqr(s(254), nP(36,2), qr, stack); //  | ξ₇₅²
qrMul(s(10), s(124), s(10), qr, stack); //  | ξ₇₄ ξ₇₆
qrMul(s(254), x, s(254), qr, stack);
qrMul(s(10), s(0), s(10), qr, stack);
qrSub(nP(36,0), s(254), s(10), qr); //  | [75]Px
qrMul(nP(37,2), nP(37,2), nP(38,2), qr, stack); //  | ξ₇₇ ξ₇₄²
qrMul(nP(39,2), nP(35,2), nP(39,2), qr, stack); //  | ξ₇₃ ξ₇₆²
qrSub(nP(39,2), nP(37,2), nP(39,2), qr);
qrMul(nP(36,1), y, nP(39,2), qr, stack); //  | [75]Py
qrSqr(nP(37,2), s(54), qr, stack); //  | ξ₇₂² | ξ₇₂²
qrSqr(s(255), nP(35,2), qr, stack); //  | ξ₇₃²
qrMul(s(124), s(54), s(124), qr, stack); //  | ξ₇₂ ξ₇₄
qrMul(s(255), x, s(255), qr, stack);
qrMul(s(124), s(0), s(124), qr, stack);
qrSub(nP(35,0), s(255), s(124), qr); //  | [73]Px
qrMul(nP(36,2), nP(36,2), nP(37,2), qr, stack); //  | ξ₇₅ ξ₇₂²
qrMul(nP(38,2), nP(34,2), nP(38,2), qr, stack); //  | ξ₇₁ ξ₇₄²
qrSub(nP(38,2), nP(36,2), nP(38,2), qr);
qrMul(nP(35,1), y, nP(38,2), qr, stack); //  | [73]Py
qrSqr(nP(36,2), s(117), qr, stack); //  | ξ₇₀² | ξ₇₀²
qrSqr(s(256), nP(34,2), qr, stack); //  | ξ₇₁²
qrMul(s(54), s(117), s(54), qr, stack); //  | ξ₇₀ ξ₇₂
qrMul(s(256), x, s(256), qr, stack);
qrMul(s(54), s(0), s(54), qr, stack);
qrSub(nP(34,0), s(256), s(54), qr); //  | [71]Px
qrMul(nP(35,2), nP(35,2), nP(36,2), qr, stack); //  | ξ₇₃ ξ₇₀²
qrMul(nP(37,2), nP(33,2), nP(37,2), qr, stack); //  | ξ₆₉ ξ₇₂²
qrSub(nP(37,2), nP(35,2), nP(37,2), qr);
qrMul(nP(34,1), y, nP(37,2), qr, stack); //  | [71]Py
qrSqr(nP(35,2), s(18), qr, stack); //  | ξ₆₈² | ξ₆₈²
qrSqr(s(257), nP(33,2), qr, stack); //  | ξ₆₉²
qrMul(s(117), s(18), s(117), qr, stack); //  | ξ₆₈ ξ₇₀
qrMul(s(257), x, s(257), qr, stack);
qrMul(s(117), s(0), s(117), qr, stack);
qrSub(nP(33,0), s(257), s(117), qr); //  | [69]Px
qrMul(nP(34,2), nP(34,2), nP(35,2), qr, stack); //  | ξ₇₁ ξ₆₈²
qrMul(nP(36,2), nP(32,2), nP(36,2), qr, stack); //  | ξ₆₇ ξ₇₀²
qrSub(nP(36,2), nP(34,2), nP(36,2), qr);
qrMul(nP(33,1), y, nP(36,2), qr, stack); //  | [69]Py
qrSqr(nP(34,2), s(110), qr, stack); //  | ξ₆₆² | ξ₆₆²
qrSqr(s(258), nP(32,2), qr, stack); //  | ξ₆₇²
qrMul(s(18), s(110), s(18), qr, stack); //  | ξ₆₆ ξ₆₈
qrMul(s(258), x, s(258), qr, stack);
qrMul(s(18), s(0), s(18), qr, stack);
qrSub(nP(32,0), s(258), s(18), qr); //  | [67]Px
qrMul(nP(33,2), nP(33,2), nP(34,2), qr, stack); //  | ξ₆₉ ξ₆₆²
qrMul(nP(35,2), nP(31,2), nP(35,2), qr, stack); //  | ξ₆₅ ξ₆₈²
qrSub(nP(35,2), nP(33,2), nP(35,2), qr);
qrMul(nP(32,1), y, nP(35,2), qr, stack); //  | [67]Py
qrMul(s(226), x, s(226), qr, stack);
qrMul(s(47), s(0), s(47), qr, stack);
qrSub(nP(31,0), s(226), s(47), qr); //  | [65]Px
qrMul(s(222), nP(32,2), s(222), qr, stack); //  | ξ₆₇ ξ₆₄²
qrMul(nP(34,2), nP(30,2), nP(34,2), qr, stack); //  | ξ₆₃ ξ₆₆²
qrSub(nP(34,2), s(222), nP(34,2), qr);
qrMul(nP(31,1), y, nP(34,2), qr, stack); //  | [65]Py
qrMul(s(219), x, s(219), qr, stack);
qrMul(s(220), s(0), s(220), qr, stack);
qrSub(nP(30,0), s(219), s(220), qr); //  | [63]Px
qrMul(nP(30,1), y, nP(29,2), qr, stack); //  | [63]Py
qrMul(s(212), x, s(212), qr, stack);
qrMul(s(213), s(0), s(213), qr, stack);
qrSub(nP(29,0), s(212), s(213), qr); //  | [61]Px
qrMul(nP(29,1), y, nP(28,2), qr, stack); //  | [61]Py
qrMul(s(205), x, s(205), qr, stack);
qrMul(s(206), s(0), s(206), qr, stack);
qrSub(nP(28,0), s(205), s(206), qr); //  | [59]Px
qrMul(nP(28,1), y, nP(27,2), qr, stack); //  | [59]Py
qrMul(s(198), x, s(198), qr, stack);
qrMul(s(199), s(0), s(199), qr, stack);
qrSub(nP(27,0), s(198), s(199), qr); //  | [57]Px
qrMul(nP(27,1), y, nP(26,2), qr, stack); //  | [57]Py
qrMul(s(191), x, s(191), qr, stack);
qrMul(s(192), s(0), s(192), qr, stack);
qrSub(nP(26,0), s(191), s(192), qr); //  | [55]Px
qrMul(nP(26,1), y, nP(25,2), qr, stack); //  | [55]Py
qrMul(s(184), x, s(184), qr, stack);
qrMul(s(185), s(0), s(185), qr, stack);
qrSub(nP(25,0), s(184), s(185), qr); //  | [53]Px
qrMul(nP(25,1), y, nP(24,2), qr, stack); //  | [53]Py
qrMul(s(177), x, s(177), qr, stack);
qrMul(s(178), s(0), s(178), qr, stack);
qrSub(nP(24,0), s(177), s(178), qr); //  | [51]Px
qrMul(nP(24,1), y, nP(23,2), qr, stack); //  | [51]Py
qrMul(s(170), x, s(170), qr, stack);
qrMul(s(171), s(0), s(171), qr, stack);
qrSub(nP(23,0), s(170), s(171), qr); //  | [49]Px
qrMul(nP(23,1), y, nP(22,2), qr, stack); //  | [49]Py
qrMul(s(163), x, s(163), qr, stack);
qrMul(s(164), s(0), s(164), qr, stack);
qrSub(nP(22,0), s(163), s(164), qr); //  | [47]Px
qrMul(nP(22,1), y, nP(21,2), qr, stack); //  | [47]Py
qrMul(s(156), x, s(156), qr, stack);
qrMul(s(157), s(0), s(157), qr, stack);
qrSub(nP(21,0), s(156), s(157), qr); //  | [45]Px
qrMul(nP(21,1), y, nP(20,2), qr, stack); //  | [45]Py
qrMul(s(149), x, s(149), qr, stack);
qrMul(s(150), s(0), s(150), qr, stack);
qrSub(nP(20,0), s(149), s(150), qr); //  | [43]Px
qrMul(nP(20,1), y, nP(19,2), qr, stack); //  | [43]Py
qrMul(s(142), x, s(142), qr, stack);
qrMul(s(143), s(0), s(143), qr, stack);
qrSub(nP(19,0), s(142), s(143), qr); //  | [41]Px
qrMul(nP(19,1), y, nP(18,2), qr, stack); //  | [41]Py
qrMul(s(135), x, s(135), qr, stack);
qrMul(s(136), s(0), s(136), qr, stack);
qrSub(nP(18,0), s(135), s(136), qr); //  | [39]Px
qrMul(nP(18,1), y, nP(17,2), qr, stack); //  | [39]Py
qrMul(s(128), x, s(128), qr, stack);
qrMul(s(129), s(0), s(129), qr, stack);
qrSub(nP(17,0), s(128), s(129), qr); //  | [37]Px
qrMul(nP(17,1), y, nP(16,2), qr, stack); //  | [37]Py
qrMul(s(121), x, s(121), qr, stack);
qrMul(s(122), s(0), s(122), qr, stack);
qrSub(nP(16,0), s(121), s(122), qr); //  | [35]Px
qrMul(nP(16,1), y, nP(15,2), qr, stack); //  | [35]Py
qrMul(s(114), x, s(114), qr, stack);
qrMul(s(115), s(0), s(115), qr, stack);
qrSub(nP(15,0), s(114), s(115), qr); //  | [33]Px
qrMul(nP(15,1), y, nP(14,2), qr, stack); //  | [33]Py
qrMul(s(107), x, s(107), qr, stack);
qrMul(s(108), s(0), s(108), qr, stack);
qrSub(nP(14,0), s(107), s(108), qr); //  | [31]Px
qrMul(nP(14,1), y, nP(13,2), qr, stack); //  | [31]Py
qrMul(s(100), x, s(100), qr, stack);
qrMul(s(101), s(0), s(101), qr, stack);
qrSub(nP(13,0), s(100), s(101), qr); //  | [29]Px
qrMul(nP(13,1), y, nP(12,2), qr, stack); //  | [29]Py
qrMul(s(93), x, s(93), qr, stack);
qrMul(s(94), s(0), s(94), qr, stack);
qrSub(nP(12,0), s(93), s(94), qr); //  | [27]Px
qrMul(nP(12,1), y, nP(11,2), qr, stack); //  | [27]Py
qrMul(s(86), x, s(86), qr, stack);
qrMul(s(87), s(0), s(87), qr, stack);
qrSub(nP(11,0), s(86), s(87), qr); //  | [25]Px
qrMul(nP(11,1), y, nP(10,2), qr, stack); //  | [25]Py
qrMul(s(79), x, s(79), qr, stack);
qrMul(s(80), s(0), s(80), qr, stack);
qrSub(nP(10,0), s(79), s(80), qr); //  | [23]Px
qrMul(nP(10,1), y, nP(9,2), qr, stack); //  | [23]Py
qrMul(s(72), x, s(72), qr, stack);
qrMul(s(73), s(0), s(73), qr, stack);
qrSub(nP(9,0), s(72), s(73), qr); //  | [21]Px
qrMul(nP(9,1), y, nP(8,2), qr, stack); //  | [21]Py
qrMul(s(65), x, s(65), qr, stack);
qrMul(s(66), s(0), s(66), qr, stack);
qrSub(nP(8,0), s(65), s(66), qr); //  | [19]Px
qrMul(nP(8,1), y, nP(7,2), qr, stack); //  | [19]Py
qrMul(s(58), x, s(58), qr, stack);
qrMul(s(59), s(0), s(59), qr, stack);
qrSub(nP(7,0), s(58), s(59), qr); //  | [17]Px
qrMul(nP(7,1), y, nP(6,2), qr, stack); //  | [17]Py
qrMul(s(51), x, s(51), qr, stack);
qrMul(s(52), s(0), s(52), qr, stack);
qrSub(nP(6,0), s(51), s(52), qr); //  | [15]Px
qrMul(nP(6,1), y, nP(5,2), qr, stack); //  | [15]Py
qrMul(s(44), x, s(44), qr, stack);
qrMul(s(45), s(0), s(45), qr, stack);
qrSub(nP(5,0), s(44), s(45), qr); //  | [13]Px
qrMul(nP(5,1), y, nP(4,2), qr, stack); //  | [13]Py
qrMul(s(37), x, s(37), qr, stack);
qrMul(s(38), s(0), s(38), qr, stack);
qrSub(nP(4,0), s(37), s(38), qr); //  | [11]Px
qrMul(nP(4,1), y, nP(3,2), qr, stack); //  | [11]Py
qrMul(s(30), x, s(30), qr, stack);
qrMul(s(31), s(0), s(31), qr, stack);
qrSub(nP(3,0), s(30), s(31), qr); //  | [9]Px
qrMul(nP(3,1), y, nP(2,2), qr, stack); //  | [9]Py
qrMul(s(22), x, s(22), qr, stack);
qrMul(s(23), s(0), s(23), qr, stack);
qrSub(nP(2,0), s(22), s(23), qr); //  | [7]Px
qrMul(nP(2,1), y, nP(1,2), qr, stack); //  | [7]Py
qrMul(s(14), x, s(14), qr, stack);
qrMul(s(16), s(0), s(16), qr, stack);
qrSub(nP(1,0), s(14), s(16), qr); //  | [5]Px
qrMul(nP(1,1), y, nP(0,2), qr, stack); //  | [5]Py
qrMul(s(9), x, s(9), qr, stack);
qrMul(s(2), s(0), s(2), qr, stack);
qrSub(nP(0,0), s(9), s(2), qr); //  | [3]Px
qrMul(nP(0,1), y, s(4), qr, stack); //  | [3]Py

#undef s
#undef nP
}
size_t ecDivpJ_64_deep(size_t n, size_t ec_deep)
{
  return 259*n + ec_deep;
}


