#include "bee2/core/mem.h"
#include "bee2/math/ec.h"

// cost: 1I+942M+196S+246A
// size: 251*n words
void ecDivpA_64(word *nPs, word const *P, ec_o const *ec, void *stack)
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
qrSqr(s(16), s(12), qr, stack); //  | ξ₈² | ξ₈² | ξ₈² | ξ₈² | ξ₈² | ξ₈²
qrMul(s(9), s(9), s(13), qr, stack); //  | ξ₇ ξ₉ | ξ₇ ξ₉ | ξ₇ ξ₉ | ξ₇ ξ₉ | ξ₇ ξ₉ | ξ₇ ξ₉
qrMul(s(11), s(9), s(11), qr, stack);
qrMul(s(5), s(5), s(16), qr, stack);
qrSub(s(5), s(11), s(5), qr); //  | ξ₁₄
qrMul(s(11), s(9), s(7), qr, stack); //  | ξ₇ ξ₉   ξ₇²
qrMul(s(19), s(15), s(16), qr, stack); //  | ξ₆ ξ₈   ξ₈²
qrMul(s(19), s(1), s(19), qr, stack);
qrSub(s(19), s(11), s(19), qr); //  | ξ₁₅
qrSqr(s(11), s(13), qr, stack); //  | ξ₉² | ξ₉² | ξ₉² | ξ₉² | ξ₉²
qrMul(s(12), s(12), s(3), qr, stack); //  | ξ₈ ξ₁₀ | ξ₈ ξ₁₀ | ξ₈ ξ₁₀ | ξ₈ ξ₁₀ | ξ₈ ξ₁₀
qrMul(s(20), s(12), s(7), qr, stack);
qrMul(s(21), s(15), s(11), qr, stack);
qrSub(s(21), s(20), s(21), qr); //  | ξ₁₆
qrMul(s(20), s(12), s(16), qr, stack); //  | ξ₈ ξ₁₀   ξ₈²
qrMul(s(22), s(9), s(11), qr, stack); //  | ξ₇ ξ₉   ξ₉²
qrMul(s(20), s(1), s(20), qr, stack);
qrSub(s(22), s(20), s(22), qr); //  | ξ₁₇
qrSqr(s(20), s(3), qr, stack); //  | ξ₁₀² | ξ₁₀² | ξ₁₀² | ξ₁₀² | ξ₁₀² | ξ₁₀²
qrMul(s(13), s(13), s(14), qr, stack); //  | ξ₉ ξ₁₁ | ξ₉ ξ₁₁ | ξ₉ ξ₁₁ | ξ₉ ξ₁₁ | ξ₉ ξ₁₁ | ξ₉ ξ₁₁
qrMul(s(16), s(13), s(16), qr, stack);
qrMul(s(9), s(9), s(20), qr, stack);
qrSub(s(9), s(16), s(9), qr); //  | ξ₁₈
qrMul(s(16), s(13), s(11), qr, stack); //  | ξ₉ ξ₁₁   ξ₉²
qrMul(s(23), s(12), s(20), qr, stack); //  | ξ₈ ξ₁₀   ξ₁₀²
qrMul(s(23), s(1), s(23), qr, stack);
qrSub(s(23), s(16), s(23), qr); //  | ξ₁₉
qrSqr(s(16), s(14), qr, stack); //  | ξ₁₁² | ξ₁₁² | ξ₁₁² | ξ₁₁² | ξ₁₁²
qrMul(s(24), s(3), s(17), qr, stack); //  | ξ₁₀ ξ₁₂ | ξ₁₀ ξ₁₂ | ξ₁₀ ξ₁₂ | ξ₁₀ ξ₁₂ | ξ₁₀ ξ₁₂
qrMul(s(25), s(24), s(11), qr, stack);
qrMul(s(26), s(12), s(16), qr, stack);
qrSub(s(26), s(25), s(26), qr); //  | ξ₂₀
qrMul(s(25), s(24), s(20), qr, stack); //  | ξ₁₀ ξ₁₂   ξ₁₀²
qrMul(s(27), s(13), s(16), qr, stack); //  | ξ₉ ξ₁₁   ξ₁₁²
qrMul(s(25), s(1), s(25), qr, stack);
qrSub(s(27), s(25), s(27), qr); //  | ξ₂₁
qrSqr(s(25), s(17), qr, stack); //  | ξ₁₂² | ξ₁₂² | ξ₁₂² | ξ₁₂² | ξ₁₂² | ξ₁₂²
qrMul(s(14), s(14), s(18), qr, stack); //  | ξ₁₁ ξ₁₃ | ξ₁₁ ξ₁₃ | ξ₁₁ ξ₁₃ | ξ₁₁ ξ₁₃ | ξ₁₁ ξ₁₃ | ξ₁₁ ξ₁₃
qrMul(s(20), s(14), s(20), qr, stack);
qrMul(s(13), s(13), s(25), qr, stack);
qrSub(s(13), s(20), s(13), qr); //  | ξ₂₂
qrMul(s(20), s(14), s(16), qr, stack); //  | ξ₁₁ ξ₁₃   ξ₁₁²
qrMul(s(28), s(24), s(25), qr, stack); //  | ξ₁₀ ξ₁₂   ξ₁₂²
qrMul(s(28), s(1), s(28), qr, stack);
qrSub(s(28), s(20), s(28), qr); //  | ξ₂₃
qrSqr(s(20), s(18), qr, stack); //  | ξ₁₃² | ξ₁₃² | ξ₁₃² | ξ₁₃² | ξ₁₃²
qrMul(s(17), s(17), s(5), qr, stack); //  | ξ₁₂ ξ₁₄ | ξ₁₂ ξ₁₄ | ξ₁₂ ξ₁₄ | ξ₁₂ ξ₁₄ | ξ₁₂ ξ₁₄
qrMul(s(29), s(17), s(16), qr, stack);
qrMul(s(30), s(24), s(20), qr, stack);
qrSub(s(30), s(29), s(30), qr); //  | ξ₂₄
qrMul(s(29), s(17), s(25), qr, stack); //  | ξ₁₂ ξ₁₄   ξ₁₂²
qrMul(s(31), s(14), s(20), qr, stack); //  | ξ₁₁ ξ₁₃   ξ₁₃²
qrMul(s(29), s(1), s(29), qr, stack);
qrSub(s(31), s(29), s(31), qr); //  | ξ₂₅
qrSqr(s(29), s(5), qr, stack); //  | ξ₁₄² | ξ₁₄² | ξ₁₄² | ξ₁₄² | ξ₁₄² | ξ₁₄²
qrMul(s(18), s(18), s(19), qr, stack); //  | ξ₁₃ ξ₁₅ | ξ₁₃ ξ₁₅ | ξ₁₃ ξ₁₅ | ξ₁₃ ξ₁₅ | ξ₁₃ ξ₁₅ | ξ₁₃ ξ₁₅
qrMul(s(25), s(18), s(25), qr, stack);
qrMul(s(14), s(14), s(29), qr, stack);
qrSub(s(14), s(25), s(14), qr); //  | ξ₂₆
qrMul(s(25), s(18), s(20), qr, stack); //  | ξ₁₃ ξ₁₅   ξ₁₃²
qrMul(s(32), s(17), s(29), qr, stack); //  | ξ₁₂ ξ₁₄   ξ₁₄²
qrMul(s(32), s(1), s(32), qr, stack);
qrSub(s(32), s(25), s(32), qr); //  | ξ₂₇
qrSqr(s(25), s(19), qr, stack); //  | ξ₁₅² | ξ₁₅² | ξ₁₅² | ξ₁₅² | ξ₁₅²
qrMul(s(33), s(5), s(21), qr, stack); //  | ξ₁₄ ξ₁₆ | ξ₁₄ ξ₁₆ | ξ₁₄ ξ₁₆ | ξ₁₄ ξ₁₆ | ξ₁₄ ξ₁₆
qrMul(s(34), s(33), s(20), qr, stack);
qrMul(s(35), s(17), s(25), qr, stack);
qrSub(s(35), s(34), s(35), qr); //  | ξ₂₈
qrMul(s(34), s(33), s(29), qr, stack); //  | ξ₁₄ ξ₁₆   ξ₁₄²
qrMul(s(36), s(18), s(25), qr, stack); //  | ξ₁₃ ξ₁₅   ξ₁₅²
qrMul(s(34), s(1), s(34), qr, stack);
qrSub(s(36), s(34), s(36), qr); //  | ξ₂₉
qrSqr(s(34), s(21), qr, stack); //  | ξ₁₆² | ξ₁₆² | ξ₁₆² | ξ₁₆² | ξ₁₆² | ξ₁₆²
qrMul(s(19), s(19), s(22), qr, stack); //  | ξ₁₅ ξ₁₇ | ξ₁₅ ξ₁₇ | ξ₁₅ ξ₁₇ | ξ₁₅ ξ₁₇ | ξ₁₅ ξ₁₇ | ξ₁₅ ξ₁₇
qrMul(s(29), s(19), s(29), qr, stack);
qrMul(s(18), s(18), s(34), qr, stack);
qrSub(s(18), s(29), s(18), qr); //  | ξ₃₀
qrMul(s(29), s(19), s(25), qr, stack); //  | ξ₁₅ ξ₁₇   ξ₁₅²
qrMul(s(37), s(33), s(34), qr, stack); //  | ξ₁₄ ξ₁₆   ξ₁₆²
qrMul(s(37), s(1), s(37), qr, stack);
qrSub(s(37), s(29), s(37), qr); //  | ξ₃₁
qrSqr(s(29), s(22), qr, stack); //  | ξ₁₇² | ξ₁₇² | ξ₁₇² | ξ₁₇² | ξ₁₇²
qrMul(s(21), s(21), s(9), qr, stack); //  | ξ₁₆ ξ₁₈ | ξ₁₆ ξ₁₈ | ξ₁₆ ξ₁₈ | ξ₁₆ ξ₁₈ | ξ₁₆ ξ₁₈
qrMul(s(38), s(21), s(25), qr, stack);
qrMul(s(39), s(33), s(29), qr, stack);
qrSub(s(39), s(38), s(39), qr); //  | ξ₃₂
qrMul(s(38), s(21), s(34), qr, stack); //  | ξ₁₆ ξ₁₈   ξ₁₆²
qrMul(s(40), s(19), s(29), qr, stack); //  | ξ₁₅ ξ₁₇   ξ₁₇²
qrMul(s(38), s(1), s(38), qr, stack);
qrSub(s(40), s(38), s(40), qr); //  | ξ₃₃
qrSqr(s(38), s(9), qr, stack); //  | ξ₁₈² | ξ₁₈² | ξ₁₈² | ξ₁₈² | ξ₁₈² | ξ₁₈²
qrMul(s(22), s(22), s(23), qr, stack); //  | ξ₁₇ ξ₁₉ | ξ₁₇ ξ₁₉ | ξ₁₇ ξ₁₉ | ξ₁₇ ξ₁₉ | ξ₁₇ ξ₁₉ | ξ₁₇ ξ₁₉
qrMul(s(34), s(22), s(34), qr, stack);
qrMul(s(19), s(19), s(38), qr, stack);
qrSub(s(19), s(34), s(19), qr); //  | ξ₃₄
qrMul(s(34), s(22), s(29), qr, stack); //  | ξ₁₇ ξ₁₉   ξ₁₇²
qrMul(s(41), s(21), s(38), qr, stack); //  | ξ₁₆ ξ₁₈   ξ₁₈²
qrMul(s(41), s(1), s(41), qr, stack);
qrSub(s(41), s(34), s(41), qr); //  | ξ₃₅
qrSqr(s(34), s(23), qr, stack); //  | ξ₁₉² | ξ₁₉² | ξ₁₉² | ξ₁₉² | ξ₁₉²
qrMul(s(42), s(9), s(26), qr, stack); //  | ξ₁₈ ξ₂₀ | ξ₁₈ ξ₂₀ | ξ₁₈ ξ₂₀ | ξ₁₈ ξ₂₀ | ξ₁₈ ξ₂₀
qrMul(s(43), s(42), s(29), qr, stack);
qrMul(s(44), s(21), s(34), qr, stack);
qrSub(s(44), s(43), s(44), qr); //  | ξ₃₆
qrMul(s(43), s(42), s(38), qr, stack); //  | ξ₁₈ ξ₂₀   ξ₁₈²
qrMul(s(45), s(22), s(34), qr, stack); //  | ξ₁₇ ξ₁₉   ξ₁₉²
qrMul(s(43), s(1), s(43), qr, stack);
qrSub(s(45), s(43), s(45), qr); //  | ξ₃₇
qrSqr(s(43), s(26), qr, stack); //  | ξ₂₀² | ξ₂₀² | ξ₂₀² | ξ₂₀² | ξ₂₀² | ξ₂₀²
qrMul(s(23), s(23), s(27), qr, stack); //  | ξ₁₉ ξ₂₁ | ξ₁₉ ξ₂₁ | ξ₁₉ ξ₂₁ | ξ₁₉ ξ₂₁ | ξ₁₉ ξ₂₁ | ξ₁₉ ξ₂₁
qrMul(s(38), s(23), s(38), qr, stack);
qrMul(s(22), s(22), s(43), qr, stack);
qrSub(s(22), s(38), s(22), qr); //  | ξ₃₈
qrMul(s(38), s(23), s(34), qr, stack); //  | ξ₁₉ ξ₂₁   ξ₁₉²
qrMul(s(46), s(42), s(43), qr, stack); //  | ξ₁₈ ξ₂₀   ξ₂₀²
qrMul(s(46), s(1), s(46), qr, stack);
qrSub(s(46), s(38), s(46), qr); //  | ξ₃₉
qrSqr(s(38), s(27), qr, stack); //  | ξ₂₁² | ξ₂₁² | ξ₂₁² | ξ₂₁² | ξ₂₁²
qrMul(s(26), s(26), s(13), qr, stack); //  | ξ₂₀ ξ₂₂ | ξ₂₀ ξ₂₂ | ξ₂₀ ξ₂₂ | ξ₂₀ ξ₂₂ | ξ₂₀ ξ₂₂
qrMul(s(47), s(26), s(34), qr, stack);
qrMul(s(48), s(42), s(38), qr, stack);
qrSub(s(48), s(47), s(48), qr); //  | ξ₄₀
qrMul(s(47), s(26), s(43), qr, stack); //  | ξ₂₀ ξ₂₂   ξ₂₀²
qrMul(s(49), s(23), s(38), qr, stack); //  | ξ₁₉ ξ₂₁   ξ₂₁²
qrMul(s(47), s(1), s(47), qr, stack);
qrSub(s(49), s(47), s(49), qr); //  | ξ₄₁
qrSqr(s(47), s(13), qr, stack); //  | ξ₂₂² | ξ₂₂² | ξ₂₂² | ξ₂₂² | ξ₂₂² | ξ₂₂²
qrMul(s(27), s(27), s(28), qr, stack); //  | ξ₂₁ ξ₂₃ | ξ₂₁ ξ₂₃ | ξ₂₁ ξ₂₃ | ξ₂₁ ξ₂₃ | ξ₂₁ ξ₂₃ | ξ₂₁ ξ₂₃
qrMul(s(43), s(27), s(43), qr, stack);
qrMul(s(23), s(23), s(47), qr, stack);
qrSub(s(23), s(43), s(23), qr); //  | ξ₄₂
qrMul(s(43), s(27), s(38), qr, stack); //  | ξ₂₁ ξ₂₃   ξ₂₁²
qrMul(s(50), s(26), s(47), qr, stack); //  | ξ₂₀ ξ₂₂   ξ₂₂²
qrMul(s(50), s(1), s(50), qr, stack);
qrSub(s(50), s(43), s(50), qr); //  | ξ₄₃
qrSqr(s(43), s(28), qr, stack); //  | ξ₂₃² | ξ₂₃² | ξ₂₃² | ξ₂₃² | ξ₂₃²
qrMul(s(51), s(13), s(30), qr, stack); //  | ξ₂₂ ξ₂₄ | ξ₂₂ ξ₂₄ | ξ₂₂ ξ₂₄ | ξ₂₂ ξ₂₄ | ξ₂₂ ξ₂₄
qrMul(s(52), s(51), s(38), qr, stack);
qrMul(s(53), s(26), s(43), qr, stack);
qrSub(s(53), s(52), s(53), qr); //  | ξ₄₄
qrMul(s(52), s(51), s(47), qr, stack); //  | ξ₂₂ ξ₂₄   ξ₂₂²
qrMul(s(54), s(27), s(43), qr, stack); //  | ξ₂₁ ξ₂₃   ξ₂₃²
qrMul(s(52), s(1), s(52), qr, stack);
qrSub(s(54), s(52), s(54), qr); //  | ξ₄₅
qrSqr(s(52), s(30), qr, stack); //  | ξ₂₄² | ξ₂₄² | ξ₂₄² | ξ₂₄² | ξ₂₄² | ξ₂₄²
qrMul(s(28), s(28), s(31), qr, stack); //  | ξ₂₃ ξ₂₅ | ξ₂₃ ξ₂₅ | ξ₂₃ ξ₂₅ | ξ₂₃ ξ₂₅ | ξ₂₃ ξ₂₅ | ξ₂₃ ξ₂₅
qrMul(s(47), s(28), s(47), qr, stack);
qrMul(s(27), s(27), s(52), qr, stack);
qrSub(s(27), s(47), s(27), qr); //  | ξ₄₆
qrMul(s(47), s(28), s(43), qr, stack); //  | ξ₂₃ ξ₂₅   ξ₂₃²
qrMul(s(55), s(51), s(52), qr, stack); //  | ξ₂₂ ξ₂₄   ξ₂₄²
qrMul(s(55), s(1), s(55), qr, stack);
qrSub(s(55), s(47), s(55), qr); //  | ξ₄₇
qrSqr(s(47), s(31), qr, stack); //  | ξ₂₅² | ξ₂₅² | ξ₂₅² | ξ₂₅² | ξ₂₅²
qrMul(s(30), s(30), s(14), qr, stack); //  | ξ₂₄ ξ₂₆ | ξ₂₄ ξ₂₆ | ξ₂₄ ξ₂₆ | ξ₂₄ ξ₂₆ | ξ₂₄ ξ₂₆
qrMul(s(56), s(30), s(43), qr, stack);
qrMul(s(57), s(51), s(47), qr, stack);
qrSub(s(57), s(56), s(57), qr); //  | ξ₄₈
qrMul(s(56), s(30), s(52), qr, stack); //  | ξ₂₄ ξ₂₆   ξ₂₄²
qrMul(s(58), s(28), s(47), qr, stack); //  | ξ₂₃ ξ₂₅   ξ₂₅²
qrMul(s(56), s(1), s(56), qr, stack);
qrSub(s(58), s(56), s(58), qr); //  | ξ₄₉
qrSqr(s(56), s(14), qr, stack); //  | ξ₂₆² | ξ₂₆² | ξ₂₆² | ξ₂₆² | ξ₂₆² | ξ₂₆²
qrMul(s(31), s(31), s(32), qr, stack); //  | ξ₂₅ ξ₂₇ | ξ₂₅ ξ₂₇ | ξ₂₅ ξ₂₇ | ξ₂₅ ξ₂₇ | ξ₂₅ ξ₂₇ | ξ₂₅ ξ₂₇
qrMul(s(52), s(31), s(52), qr, stack);
qrMul(s(28), s(28), s(56), qr, stack);
qrSub(s(28), s(52), s(28), qr); //  | ξ₅₀
qrMul(s(52), s(31), s(47), qr, stack); //  | ξ₂₅ ξ₂₇   ξ₂₅²
qrMul(s(59), s(30), s(56), qr, stack); //  | ξ₂₄ ξ₂₆   ξ₂₆²
qrMul(s(59), s(1), s(59), qr, stack);
qrSub(s(59), s(52), s(59), qr); //  | ξ₅₁
qrSqr(s(52), s(32), qr, stack); //  | ξ₂₇² | ξ₂₇² | ξ₂₇² | ξ₂₇² | ξ₂₇²
qrMul(s(60), s(14), s(35), qr, stack); //  | ξ₂₆ ξ₂₈ | ξ₂₆ ξ₂₈ | ξ₂₆ ξ₂₈ | ξ₂₆ ξ₂₈ | ξ₂₆ ξ₂₈
qrMul(s(61), s(60), s(47), qr, stack);
qrMul(s(62), s(30), s(52), qr, stack);
qrSub(s(62), s(61), s(62), qr); //  | ξ₅₂
qrMul(s(61), s(60), s(56), qr, stack); //  | ξ₂₆ ξ₂₈   ξ₂₆²
qrMul(s(63), s(31), s(52), qr, stack); //  | ξ₂₅ ξ₂₇   ξ₂₇²
qrMul(s(61), s(1), s(61), qr, stack);
qrSub(s(63), s(61), s(63), qr); //  | ξ₅₃
qrSqr(s(61), s(35), qr, stack); //  | ξ₂₈² | ξ₂₈² | ξ₂₈² | ξ₂₈² | ξ₂₈² | ξ₂₈²
qrMul(s(32), s(32), s(36), qr, stack); //  | ξ₂₇ ξ₂₉ | ξ₂₇ ξ₂₉ | ξ₂₇ ξ₂₉ | ξ₂₇ ξ₂₉ | ξ₂₇ ξ₂₉ | ξ₂₇ ξ₂₉
qrMul(s(56), s(32), s(56), qr, stack);
qrMul(s(31), s(31), s(61), qr, stack);
qrSub(s(31), s(56), s(31), qr); //  | ξ₅₄
qrMul(s(56), s(32), s(52), qr, stack); //  | ξ₂₇ ξ₂₉   ξ₂₇²
qrMul(s(64), s(60), s(61), qr, stack); //  | ξ₂₆ ξ₂₈   ξ₂₈²
qrMul(s(64), s(1), s(64), qr, stack);
qrSub(s(64), s(56), s(64), qr); //  | ξ₅₅
qrSqr(s(56), s(36), qr, stack); //  | ξ₂₉² | ξ₂₉² | ξ₂₉² | ξ₂₉² | ξ₂₉²
qrMul(s(35), s(35), s(18), qr, stack); //  | ξ₂₈ ξ₃₀ | ξ₂₈ ξ₃₀ | ξ₂₈ ξ₃₀ | ξ₂₈ ξ₃₀ | ξ₂₈ ξ₃₀
qrMul(s(65), s(35), s(52), qr, stack);
qrMul(s(66), s(60), s(56), qr, stack);
qrSub(s(66), s(65), s(66), qr); //  | ξ₅₆
qrMul(s(65), s(35), s(61), qr, stack); //  | ξ₂₈ ξ₃₀   ξ₂₈²
qrMul(s(67), s(32), s(56), qr, stack); //  | ξ₂₇ ξ₂₉   ξ₂₉²
qrMul(s(65), s(1), s(65), qr, stack);
qrSub(s(67), s(65), s(67), qr); //  | ξ₅₇
qrSqr(s(65), s(18), qr, stack); //  | ξ₃₀² | ξ₃₀² | ξ₃₀² | ξ₃₀² | ξ₃₀² | ξ₃₀²
qrMul(s(36), s(36), s(37), qr, stack); //  | ξ₂₉ ξ₃₁ | ξ₂₉ ξ₃₁ | ξ₂₉ ξ₃₁ | ξ₂₉ ξ₃₁ | ξ₂₉ ξ₃₁ | ξ₂₉ ξ₃₁
qrMul(s(61), s(36), s(61), qr, stack);
qrMul(s(32), s(32), s(65), qr, stack);
qrSub(s(32), s(61), s(32), qr); //  | ξ₅₈
qrMul(s(61), s(36), s(56), qr, stack); //  | ξ₂₉ ξ₃₁   ξ₂₉²
qrMul(s(68), s(35), s(65), qr, stack); //  | ξ₂₈ ξ₃₀   ξ₃₀²
qrMul(s(68), s(1), s(68), qr, stack);
qrSub(s(68), s(61), s(68), qr); //  | ξ₅₉
qrSqr(s(61), s(37), qr, stack); //  | ξ₃₁² | ξ₃₁² | ξ₃₁² | ξ₃₁² | ξ₃₁²
qrMul(s(69), s(18), s(39), qr, stack); //  | ξ₃₀ ξ₃₂ | ξ₃₀ ξ₃₂ | ξ₃₀ ξ₃₂ | ξ₃₀ ξ₃₂ | ξ₃₀ ξ₃₂
qrMul(s(70), s(69), s(56), qr, stack);
qrMul(s(71), s(35), s(61), qr, stack);
qrSub(s(71), s(70), s(71), qr); //  | ξ₆₀
qrMul(s(70), s(69), s(65), qr, stack); //  | ξ₃₀ ξ₃₂   ξ₃₀²
qrMul(s(72), s(36), s(61), qr, stack); //  | ξ₂₉ ξ₃₁   ξ₃₁²
qrMul(s(70), s(1), s(70), qr, stack);
qrSub(s(72), s(70), s(72), qr); //  | ξ₆₁
qrSqr(s(70), s(39), qr, stack); //  | ξ₃₂² | ξ₃₂² | ξ₃₂² | ξ₃₂² | ξ₃₂² | ξ₃₂²
qrMul(s(37), s(37), s(40), qr, stack); //  | ξ₃₁ ξ₃₃ | ξ₃₁ ξ₃₃ | ξ₃₁ ξ₃₃ | ξ₃₁ ξ₃₃ | ξ₃₁ ξ₃₃ | ξ₃₁ ξ₃₃
qrMul(s(65), s(37), s(65), qr, stack);
qrMul(s(36), s(36), s(70), qr, stack);
qrSub(s(36), s(65), s(36), qr); //  | ξ₆₂
qrMul(s(65), s(37), s(61), qr, stack); //  | ξ₃₁ ξ₃₃   ξ₃₁²
qrMul(s(73), s(69), s(70), qr, stack); //  | ξ₃₀ ξ₃₂   ξ₃₂²
qrMul(s(73), s(1), s(73), qr, stack);
qrSub(s(73), s(65), s(73), qr); //  | ξ₆₃
qrSqr(s(65), s(40), qr, stack); //  | ξ₃₃² | ξ₃₃² | ξ₃₃² | ξ₃₃² | ξ₃₃²
qrMul(s(39), s(39), s(19), qr, stack); //  | ξ₃₂ ξ₃₄ | ξ₃₂ ξ₃₄ | ξ₃₂ ξ₃₄ | ξ₃₂ ξ₃₄ | ξ₃₂ ξ₃₄
qrMul(s(74), s(39), s(61), qr, stack);
qrMul(s(75), s(69), s(65), qr, stack);
qrSub(s(75), s(74), s(75), qr); //  | ξ₆₄
qrMul(s(74), s(39), s(70), qr, stack); //  | ξ₃₂ ξ₃₄   ξ₃₂²
qrMul(s(76), s(37), s(65), qr, stack); //  | ξ₃₁ ξ₃₃   ξ₃₃²
qrMul(s(74), s(1), s(74), qr, stack);
qrSub(s(76), s(74), s(76), qr); //  | ξ₆₅
qrSqr(s(74), s(19), qr, stack); //  | ξ₃₄² | ξ₃₄² | ξ₃₄² | ξ₃₄² | ξ₃₄² | ξ₃₄²
qrMul(s(40), s(40), s(41), qr, stack); //  | ξ₃₃ ξ₃₅ | ξ₃₃ ξ₃₅ | ξ₃₃ ξ₃₅ | ξ₃₃ ξ₃₅ | ξ₃₃ ξ₃₅ | ξ₃₃ ξ₃₅
qrMul(s(70), s(40), s(70), qr, stack);
qrMul(s(37), s(37), s(74), qr, stack);
qrSub(s(37), s(70), s(37), qr); //  | ξ₆₆
qrMul(s(70), s(40), s(65), qr, stack); //  | ξ₃₃ ξ₃₅   ξ₃₃²
qrMul(s(77), s(39), s(74), qr, stack); //  | ξ₃₂ ξ₃₄   ξ₃₄²
qrMul(s(77), s(1), s(77), qr, stack);
qrSub(s(77), s(70), s(77), qr); //  | ξ₆₇
qrSqr(s(70), s(41), qr, stack); //  | ξ₃₅² | ξ₃₅² | ξ₃₅² | ξ₃₅² | ξ₃₅²
qrMul(s(78), s(19), s(44), qr, stack); //  | ξ₃₄ ξ₃₆ | ξ₃₄ ξ₃₆ | ξ₃₄ ξ₃₆ | ξ₃₄ ξ₃₆ | ξ₃₄ ξ₃₆
qrMul(s(79), s(78), s(65), qr, stack);
qrMul(s(80), s(39), s(70), qr, stack);
qrSub(s(80), s(79), s(80), qr); //  | ξ₆₈
qrMul(s(79), s(78), s(74), qr, stack); //  | ξ₃₄ ξ₃₆   ξ₃₄²
qrMul(s(81), s(40), s(70), qr, stack); //  | ξ₃₃ ξ₃₅   ξ₃₅²
qrMul(s(79), s(1), s(79), qr, stack);
qrSub(s(81), s(79), s(81), qr); //  | ξ₆₉
qrSqr(s(79), s(44), qr, stack); //  | ξ₃₆² | ξ₃₆² | ξ₃₆² | ξ₃₆² | ξ₃₆² | ξ₃₆²
qrMul(s(41), s(41), s(45), qr, stack); //  | ξ₃₅ ξ₃₇ | ξ₃₅ ξ₃₇ | ξ₃₅ ξ₃₇ | ξ₃₅ ξ₃₇ | ξ₃₅ ξ₃₇ | ξ₃₅ ξ₃₇
qrMul(s(74), s(41), s(74), qr, stack);
qrMul(s(40), s(40), s(79), qr, stack);
qrSub(s(40), s(74), s(40), qr); //  | ξ₇₀
qrMul(s(74), s(41), s(70), qr, stack); //  | ξ₃₅ ξ₃₇   ξ₃₅²
qrMul(s(82), s(78), s(79), qr, stack); //  | ξ₃₄ ξ₃₆   ξ₃₆²
qrMul(s(82), s(1), s(82), qr, stack);
qrSub(s(82), s(74), s(82), qr); //  | ξ₇₁
qrSqr(s(74), s(45), qr, stack); //  | ξ₃₇² | ξ₃₇² | ξ₃₇² | ξ₃₇² | ξ₃₇²
qrMul(s(44), s(44), s(22), qr, stack); //  | ξ₃₆ ξ₃₈ | ξ₃₆ ξ₃₈ | ξ₃₆ ξ₃₈ | ξ₃₆ ξ₃₈ | ξ₃₆ ξ₃₈
qrMul(s(83), s(44), s(70), qr, stack);
qrMul(s(84), s(78), s(74), qr, stack);
qrSub(s(84), s(83), s(84), qr); //  | ξ₇₂
qrMul(s(83), s(44), s(79), qr, stack); //  | ξ₃₆ ξ₃₈   ξ₃₆²
qrMul(s(85), s(41), s(74), qr, stack); //  | ξ₃₅ ξ₃₇   ξ₃₇²
qrMul(s(83), s(1), s(83), qr, stack);
qrSub(s(85), s(83), s(85), qr); //  | ξ₇₃
qrSqr(s(83), s(22), qr, stack); //  | ξ₃₈² | ξ₃₈² | ξ₃₈² | ξ₃₈² | ξ₃₈² | ξ₃₈²
qrMul(s(45), s(45), s(46), qr, stack); //  | ξ₃₇ ξ₃₉ | ξ₃₇ ξ₃₉ | ξ₃₇ ξ₃₉ | ξ₃₇ ξ₃₉ | ξ₃₇ ξ₃₉ | ξ₃₇ ξ₃₉
qrMul(s(79), s(45), s(79), qr, stack);
qrMul(s(41), s(41), s(83), qr, stack);
qrSub(s(41), s(79), s(41), qr); //  | ξ₇₄
qrMul(s(79), s(45), s(74), qr, stack); //  | ξ₃₇ ξ₃₉   ξ₃₇²
qrMul(s(86), s(44), s(83), qr, stack); //  | ξ₃₆ ξ₃₈   ξ₃₈²
qrMul(s(86), s(1), s(86), qr, stack);
qrSub(s(86), s(79), s(86), qr); //  | ξ₇₅
qrSqr(s(79), s(46), qr, stack); //  | ξ₃₉² | ξ₃₉² | ξ₃₉² | ξ₃₉² | ξ₃₉²
qrMul(s(87), s(22), s(48), qr, stack); //  | ξ₃₈ ξ₄₀ | ξ₃₈ ξ₄₀ | ξ₃₈ ξ₄₀ | ξ₃₈ ξ₄₀ | ξ₃₈ ξ₄₀
qrMul(s(88), s(87), s(74), qr, stack);
qrMul(s(89), s(44), s(79), qr, stack);
qrSub(s(89), s(88), s(89), qr); //  | ξ₇₆
qrMul(s(88), s(87), s(83), qr, stack); //  | ξ₃₈ ξ₄₀   ξ₃₈²
qrMul(s(90), s(45), s(79), qr, stack); //  | ξ₃₇ ξ₃₉   ξ₃₉²
qrMul(s(88), s(1), s(88), qr, stack);
qrSub(s(90), s(88), s(90), qr); //  | ξ₇₇
qrSqr(s(88), s(48), qr, stack); //  | ξ₄₀² | ξ₄₀² | ξ₄₀² | ξ₄₀² | ξ₄₀² | ξ₄₀²
qrMul(s(46), s(46), s(49), qr, stack); //  | ξ₃₉ ξ₄₁ | ξ₃₉ ξ₄₁ | ξ₃₉ ξ₄₁ | ξ₃₉ ξ₄₁ | ξ₃₉ ξ₄₁ | ξ₃₉ ξ₄₁
qrMul(s(83), s(46), s(83), qr, stack);
qrMul(s(45), s(45), s(88), qr, stack);
qrSub(s(45), s(83), s(45), qr); //  | ξ₇₈
qrMul(s(83), s(46), s(79), qr, stack); //  | ξ₃₉ ξ₄₁   ξ₃₉²
qrMul(s(91), s(87), s(88), qr, stack); //  | ξ₃₈ ξ₄₀   ξ₄₀²
qrMul(s(91), s(1), s(91), qr, stack);
qrSub(s(91), s(83), s(91), qr); //  | ξ₇₉
qrSqr(s(83), s(49), qr, stack); //  | ξ₄₁² | ξ₄₁² | ξ₄₁² | ξ₄₁² | ξ₄₁²
qrMul(s(48), s(48), s(23), qr, stack); //  | ξ₄₀ ξ₄₂ | ξ₄₀ ξ₄₂ | ξ₄₀ ξ₄₂ | ξ₄₀ ξ₄₂ | ξ₄₀ ξ₄₂
qrMul(s(92), s(48), s(79), qr, stack);
qrMul(s(93), s(87), s(83), qr, stack);
qrSub(s(93), s(92), s(93), qr); //  | ξ₈₀
qrMul(s(92), s(48), s(88), qr, stack); //  | ξ₄₀ ξ₄₂   ξ₄₀²
qrMul(s(94), s(46), s(83), qr, stack); //  | ξ₃₉ ξ₄₁   ξ₄₁²
qrMul(s(92), s(1), s(92), qr, stack);
qrSub(s(94), s(92), s(94), qr); //  | ξ₈₁
qrSqr(s(92), s(23), qr, stack); //  | ξ₄₂² | ξ₄₂² | ξ₄₂² | ξ₄₂² | ξ₄₂² | ξ₄₂²
qrMul(s(49), s(49), s(50), qr, stack); //  | ξ₄₁ ξ₄₃ | ξ₄₁ ξ₄₃ | ξ₄₁ ξ₄₃ | ξ₄₁ ξ₄₃ | ξ₄₁ ξ₄₃ | ξ₄₁ ξ₄₃
qrMul(s(88), s(49), s(88), qr, stack);
qrMul(s(46), s(46), s(92), qr, stack);
qrSub(s(46), s(88), s(46), qr); //  | ξ₈₂
qrMul(s(88), s(49), s(83), qr, stack); //  | ξ₄₁ ξ₄₃   ξ₄₁²
qrMul(s(95), s(48), s(92), qr, stack); //  | ξ₄₀ ξ₄₂   ξ₄₂²
qrMul(s(95), s(1), s(95), qr, stack);
qrSub(s(95), s(88), s(95), qr); //  | ξ₈₃
qrSqr(s(88), s(50), qr, stack); //  | ξ₄₃² | ξ₄₃² | ξ₄₃² | ξ₄₃² | ξ₄₃²
qrMul(s(96), s(23), s(53), qr, stack); //  | ξ₄₂ ξ₄₄ | ξ₄₂ ξ₄₄ | ξ₄₂ ξ₄₄ | ξ₄₂ ξ₄₄ | ξ₄₂ ξ₄₄
qrMul(s(97), s(96), s(83), qr, stack);
qrMul(s(98), s(48), s(88), qr, stack);
qrSub(s(98), s(97), s(98), qr); //  | ξ₈₄
qrMul(s(97), s(96), s(92), qr, stack); //  | ξ₄₂ ξ₄₄   ξ₄₂²
qrMul(s(99), s(49), s(88), qr, stack); //  | ξ₄₁ ξ₄₃   ξ₄₃²
qrMul(s(97), s(1), s(97), qr, stack);
qrSub(s(99), s(97), s(99), qr); //  | ξ₈₅
qrSqr(s(97), s(53), qr, stack); //  | ξ₄₄² | ξ₄₄² | ξ₄₄² | ξ₄₄² | ξ₄₄² | ξ₄₄²
qrMul(s(50), s(50), s(54), qr, stack); //  | ξ₄₃ ξ₄₅ | ξ₄₃ ξ₄₅ | ξ₄₃ ξ₄₅ | ξ₄₃ ξ₄₅ | ξ₄₃ ξ₄₅ | ξ₄₃ ξ₄₅
qrMul(s(92), s(50), s(92), qr, stack);
qrMul(s(49), s(49), s(97), qr, stack);
qrSub(s(49), s(92), s(49), qr); //  | ξ₈₆
qrMul(s(92), s(50), s(88), qr, stack); //  | ξ₄₃ ξ₄₅   ξ₄₃²
qrMul(s(100), s(96), s(97), qr, stack); //  | ξ₄₂ ξ₄₄   ξ₄₄²
qrMul(s(100), s(1), s(100), qr, stack);
qrSub(s(100), s(92), s(100), qr); //  | ξ₈₇
qrSqr(s(92), s(54), qr, stack); //  | ξ₄₅² | ξ₄₅² | ξ₄₅² | ξ₄₅² | ξ₄₅²
qrMul(s(53), s(53), s(27), qr, stack); //  | ξ₄₄ ξ₄₆ | ξ₄₄ ξ₄₆ | ξ₄₄ ξ₄₆ | ξ₄₄ ξ₄₆ | ξ₄₄ ξ₄₆
qrMul(s(101), s(53), s(88), qr, stack);
qrMul(s(102), s(96), s(92), qr, stack);
qrSub(s(102), s(101), s(102), qr); //  | ξ₈₈
qrMul(s(101), s(53), s(97), qr, stack); //  | ξ₄₄ ξ₄₆   ξ₄₄²
qrMul(s(103), s(50), s(92), qr, stack); //  | ξ₄₃ ξ₄₅   ξ₄₅²
qrMul(s(101), s(1), s(101), qr, stack);
qrSub(s(103), s(101), s(103), qr); //  | ξ₈₉
qrSqr(s(101), s(27), qr, stack); //  | ξ₄₆² | ξ₄₆² | ξ₄₆² | ξ₄₆² | ξ₄₆² | ξ₄₆²
qrMul(s(54), s(54), s(55), qr, stack); //  | ξ₄₅ ξ₄₇ | ξ₄₅ ξ₄₇ | ξ₄₅ ξ₄₇ | ξ₄₅ ξ₄₇ | ξ₄₅ ξ₄₇ | ξ₄₅ ξ₄₇
qrMul(s(97), s(54), s(97), qr, stack);
qrMul(s(50), s(50), s(101), qr, stack);
qrSub(s(50), s(97), s(50), qr); //  | ξ₉₀
qrMul(s(97), s(54), s(92), qr, stack); //  | ξ₄₅ ξ₄₇   ξ₄₅²
qrMul(s(104), s(53), s(101), qr, stack); //  | ξ₄₄ ξ₄₆   ξ₄₆²
qrMul(s(104), s(1), s(104), qr, stack);
qrSub(s(104), s(97), s(104), qr); //  | ξ₉₁
qrSqr(s(97), s(55), qr, stack); //  | ξ₄₇² | ξ₄₇² | ξ₄₇² | ξ₄₇² | ξ₄₇²
qrMul(s(105), s(27), s(57), qr, stack); //  | ξ₄₆ ξ₄₈ | ξ₄₆ ξ₄₈ | ξ₄₆ ξ₄₈ | ξ₄₆ ξ₄₈ | ξ₄₆ ξ₄₈
qrMul(s(106), s(105), s(92), qr, stack);
qrMul(s(107), s(53), s(97), qr, stack);
qrSub(s(107), s(106), s(107), qr); //  | ξ₉₂
qrMul(s(106), s(105), s(101), qr, stack); //  | ξ₄₆ ξ₄₈   ξ₄₆²
qrMul(s(108), s(54), s(97), qr, stack); //  | ξ₄₅ ξ₄₇   ξ₄₇²
qrMul(s(106), s(1), s(106), qr, stack);
qrSub(s(108), s(106), s(108), qr); //  | ξ₉₃
qrSqr(s(106), s(57), qr, stack); //  | ξ₄₈² | ξ₄₈² | ξ₄₈² | ξ₄₈² | ξ₄₈² | ξ₄₈²
qrMul(s(55), s(55), s(58), qr, stack); //  | ξ₄₇ ξ₄₉ | ξ₄₇ ξ₄₉ | ξ₄₇ ξ₄₉ | ξ₄₇ ξ₄₉ | ξ₄₇ ξ₄₉ | ξ₄₇ ξ₄₉
qrMul(s(101), s(55), s(101), qr, stack);
qrMul(s(54), s(54), s(106), qr, stack);
qrSub(s(54), s(101), s(54), qr); //  | ξ₉₄
qrMul(s(101), s(55), s(97), qr, stack); //  | ξ₄₇ ξ₄₉   ξ₄₇²
qrMul(s(109), s(105), s(106), qr, stack); //  | ξ₄₆ ξ₄₈   ξ₄₈²
qrMul(s(109), s(1), s(109), qr, stack);
qrSub(s(109), s(101), s(109), qr); //  | ξ₉₅
qrSqr(s(101), s(58), qr, stack); //  | ξ₄₉² | ξ₄₉² | ξ₄₉² | ξ₄₉² | ξ₄₉²
qrMul(s(57), s(57), s(28), qr, stack); //  | ξ₄₈ ξ₅₀ | ξ₄₈ ξ₅₀ | ξ₄₈ ξ₅₀ | ξ₄₈ ξ₅₀ | ξ₄₈ ξ₅₀
qrMul(s(110), s(57), s(97), qr, stack);
qrMul(s(111), s(105), s(101), qr, stack);
qrSub(s(111), s(110), s(111), qr); //  | ξ₉₆
qrMul(s(110), s(57), s(106), qr, stack); //  | ξ₄₈ ξ₅₀   ξ₄₈²
qrMul(s(112), s(55), s(101), qr, stack); //  | ξ₄₇ ξ₄₉   ξ₄₉²
qrMul(s(110), s(1), s(110), qr, stack);
qrSub(s(112), s(110), s(112), qr); //  | ξ₉₇
qrSqr(s(110), s(28), qr, stack); //  | ξ₅₀² | ξ₅₀² | ξ₅₀² | ξ₅₀² | ξ₅₀² | ξ₅₀²
qrMul(s(58), s(58), s(59), qr, stack); //  | ξ₄₉ ξ₅₁ | ξ₄₉ ξ₅₁ | ξ₄₉ ξ₅₁ | ξ₄₉ ξ₅₁ | ξ₄₉ ξ₅₁ | ξ₄₉ ξ₅₁
qrMul(s(106), s(58), s(106), qr, stack);
qrMul(s(55), s(55), s(110), qr, stack);
qrSub(s(55), s(106), s(55), qr); //  | ξ₉₈
qrMul(s(106), s(58), s(101), qr, stack); //  | ξ₄₉ ξ₅₁   ξ₄₉²
qrMul(s(113), s(57), s(110), qr, stack); //  | ξ₄₈ ξ₅₀   ξ₅₀²
qrMul(s(113), s(1), s(113), qr, stack);
qrSub(s(113), s(106), s(113), qr); //  | ξ₉₉
qrSqr(s(106), s(59), qr, stack); //  | ξ₅₁² | ξ₅₁² | ξ₅₁² | ξ₅₁² | ξ₅₁²
qrMul(s(114), s(28), s(62), qr, stack); //  | ξ₅₀ ξ₅₂ | ξ₅₀ ξ₅₂ | ξ₅₀ ξ₅₂ | ξ₅₀ ξ₅₂ | ξ₅₀ ξ₅₂
qrMul(s(115), s(114), s(101), qr, stack);
qrMul(s(116), s(57), s(106), qr, stack);
qrSub(s(116), s(115), s(116), qr); //  | ξ₁₀₀
qrMul(s(115), s(114), s(110), qr, stack); //  | ξ₅₀ ξ₅₂   ξ₅₀²
qrMul(s(117), s(58), s(106), qr, stack); //  | ξ₄₉ ξ₅₁   ξ₅₁²
qrMul(s(115), s(1), s(115), qr, stack);
qrSub(s(117), s(115), s(117), qr); //  | ξ₁₀₁
qrSqr(s(115), s(62), qr, stack); //  | ξ₅₂² | ξ₅₂² | ξ₅₂² | ξ₅₂² | ξ₅₂² | ξ₅₂²
qrMul(s(59), s(59), s(63), qr, stack); //  | ξ₅₁ ξ₅₃ | ξ₅₁ ξ₅₃ | ξ₅₁ ξ₅₃ | ξ₅₁ ξ₅₃ | ξ₅₁ ξ₅₃ | ξ₅₁ ξ₅₃
qrMul(s(110), s(59), s(110), qr, stack);
qrMul(s(58), s(58), s(115), qr, stack);
qrSub(s(58), s(110), s(58), qr); //  | ξ₁₀₂
qrMul(s(110), s(59), s(106), qr, stack); //  | ξ₅₁ ξ₅₃   ξ₅₁²
qrMul(s(118), s(114), s(115), qr, stack); //  | ξ₅₀ ξ₅₂   ξ₅₂²
qrMul(s(118), s(1), s(118), qr, stack);
qrSub(s(118), s(110), s(118), qr); //  | ξ₁₀₃
qrSqr(s(110), s(63), qr, stack); //  | ξ₅₃² | ξ₅₃² | ξ₅₃² | ξ₅₃² | ξ₅₃²
qrMul(s(62), s(62), s(31), qr, stack); //  | ξ₅₂ ξ₅₄ | ξ₅₂ ξ₅₄ | ξ₅₂ ξ₅₄ | ξ₅₂ ξ₅₄ | ξ₅₂ ξ₅₄
qrMul(s(119), s(62), s(106), qr, stack);
qrMul(s(120), s(114), s(110), qr, stack);
qrSub(s(120), s(119), s(120), qr); //  | ξ₁₀₄
qrMul(s(119), s(62), s(115), qr, stack); //  | ξ₅₂ ξ₅₄   ξ₅₂²
qrMul(s(121), s(59), s(110), qr, stack); //  | ξ₅₁ ξ₅₃   ξ₅₃²
qrMul(s(119), s(1), s(119), qr, stack);
qrSub(s(121), s(119), s(121), qr); //  | ξ₁₀₅
qrSqr(s(119), s(31), qr, stack); //  | ξ₅₄² | ξ₅₄² | ξ₅₄² | ξ₅₄² | ξ₅₄² | ξ₅₄²
qrMul(s(63), s(63), s(64), qr, stack); //  | ξ₅₃ ξ₅₅ | ξ₅₃ ξ₅₅ | ξ₅₃ ξ₅₅ | ξ₅₃ ξ₅₅ | ξ₅₃ ξ₅₅ | ξ₅₃ ξ₅₅
qrMul(s(115), s(63), s(115), qr, stack);
qrMul(s(59), s(59), s(119), qr, stack);
qrSub(s(59), s(115), s(59), qr); //  | ξ₁₀₆
qrMul(s(115), s(63), s(110), qr, stack); //  | ξ₅₃ ξ₅₅   ξ₅₃²
qrMul(s(122), s(62), s(119), qr, stack); //  | ξ₅₂ ξ₅₄   ξ₅₄²
qrMul(s(122), s(1), s(122), qr, stack);
qrSub(s(122), s(115), s(122), qr); //  | ξ₁₀₇
qrSqr(s(115), s(64), qr, stack); //  | ξ₅₅² | ξ₅₅² | ξ₅₅² | ξ₅₅² | ξ₅₅²
qrMul(s(123), s(31), s(66), qr, stack); //  | ξ₅₄ ξ₅₆ | ξ₅₄ ξ₅₆ | ξ₅₄ ξ₅₆ | ξ₅₄ ξ₅₆ | ξ₅₄ ξ₅₆
qrMul(s(124), s(123), s(110), qr, stack);
qrMul(s(125), s(62), s(115), qr, stack);
qrSub(s(125), s(124), s(125), qr); //  | ξ₁₀₈
qrMul(s(124), s(123), s(119), qr, stack); //  | ξ₅₄ ξ₅₆   ξ₅₄²
qrMul(s(126), s(63), s(115), qr, stack); //  | ξ₅₃ ξ₅₅   ξ₅₅²
qrMul(s(124), s(1), s(124), qr, stack);
qrSub(s(126), s(124), s(126), qr); //  | ξ₁₀₉
qrSqr(s(124), s(66), qr, stack); //  | ξ₅₆² | ξ₅₆² | ξ₅₆² | ξ₅₆² | ξ₅₆² | ξ₅₆²
qrMul(s(64), s(64), s(67), qr, stack); //  | ξ₅₅ ξ₅₇ | ξ₅₅ ξ₅₇ | ξ₅₅ ξ₅₇ | ξ₅₅ ξ₅₇ | ξ₅₅ ξ₅₇ | ξ₅₅ ξ₅₇
qrMul(s(119), s(64), s(119), qr, stack);
qrMul(s(63), s(63), s(124), qr, stack);
qrSub(s(63), s(119), s(63), qr); //  | ξ₁₁₀
qrMul(s(119), s(64), s(115), qr, stack); //  | ξ₅₅ ξ₅₇   ξ₅₅²
qrMul(s(127), s(123), s(124), qr, stack); //  | ξ₅₄ ξ₅₆   ξ₅₆²
qrMul(s(127), s(1), s(127), qr, stack);
qrSub(s(127), s(119), s(127), qr); //  | ξ₁₁₁
qrSqr(s(119), s(67), qr, stack); //  | ξ₅₇² | ξ₅₇² | ξ₅₇² | ξ₅₇² | ξ₅₇²
qrMul(s(66), s(66), s(32), qr, stack); //  | ξ₅₆ ξ₅₈ | ξ₅₆ ξ₅₈ | ξ₅₆ ξ₅₈ | ξ₅₆ ξ₅₈ | ξ₅₆ ξ₅₈
qrMul(s(128), s(66), s(115), qr, stack);
qrMul(s(129), s(123), s(119), qr, stack);
qrSub(s(129), s(128), s(129), qr); //  | ξ₁₁₂
qrMul(s(128), s(66), s(124), qr, stack); //  | ξ₅₆ ξ₅₈   ξ₅₆²
qrMul(s(130), s(64), s(119), qr, stack); //  | ξ₅₅ ξ₅₇   ξ₅₇²
qrMul(s(128), s(1), s(128), qr, stack);
qrSub(s(130), s(128), s(130), qr); //  | ξ₁₁₃
qrSqr(s(128), s(32), qr, stack); //  | ξ₅₈² | ξ₅₈² | ξ₅₈² | ξ₅₈² | ξ₅₈² | ξ₅₈²
qrMul(s(67), s(67), s(68), qr, stack); //  | ξ₅₇ ξ₅₉ | ξ₅₇ ξ₅₉ | ξ₅₇ ξ₅₉ | ξ₅₇ ξ₅₉ | ξ₅₇ ξ₅₉ | ξ₅₇ ξ₅₉
qrMul(s(124), s(67), s(124), qr, stack);
qrMul(s(64), s(64), s(128), qr, stack);
qrSub(s(64), s(124), s(64), qr); //  | ξ₁₁₄
qrMul(s(124), s(67), s(119), qr, stack); //  | ξ₅₇ ξ₅₉   ξ₅₇²
qrMul(s(131), s(66), s(128), qr, stack); //  | ξ₅₆ ξ₅₈   ξ₅₈²
qrMul(s(131), s(1), s(131), qr, stack);
qrSub(s(131), s(124), s(131), qr); //  | ξ₁₁₅
qrSqr(s(124), s(68), qr, stack); //  | ξ₅₉² | ξ₅₉² | ξ₅₉² | ξ₅₉² | ξ₅₉²
qrMul(s(132), s(32), s(71), qr, stack); //  | ξ₅₈ ξ₆₀ | ξ₅₈ ξ₆₀ | ξ₅₈ ξ₆₀ | ξ₅₈ ξ₆₀ | ξ₅₈ ξ₆₀
qrMul(s(133), s(132), s(119), qr, stack);
qrMul(s(134), s(66), s(124), qr, stack);
qrSub(s(134), s(133), s(134), qr); //  | ξ₁₁₆
qrMul(s(133), s(132), s(128), qr, stack); //  | ξ₅₈ ξ₆₀   ξ₅₈²
qrMul(s(135), s(67), s(124), qr, stack); //  | ξ₅₇ ξ₅₉   ξ₅₉²
qrMul(s(133), s(1), s(133), qr, stack);
qrSub(s(135), s(133), s(135), qr); //  | ξ₁₁₇
qrSqr(s(133), s(71), qr, stack); //  | ξ₆₀² | ξ₆₀² | ξ₆₀² | ξ₆₀² | ξ₆₀² | ξ₆₀²
qrMul(s(68), s(68), s(72), qr, stack); //  | ξ₅₉ ξ₆₁ | ξ₅₉ ξ₆₁ | ξ₅₉ ξ₆₁ | ξ₅₉ ξ₆₁ | ξ₅₉ ξ₆₁ | ξ₅₉ ξ₆₁
qrMul(s(128), s(68), s(128), qr, stack);
qrMul(s(67), s(67), s(133), qr, stack);
qrSub(s(67), s(128), s(67), qr); //  | ξ₁₁₈
qrMul(s(128), s(68), s(124), qr, stack); //  | ξ₅₉ ξ₆₁   ξ₅₉²
qrMul(s(136), s(132), s(133), qr, stack); //  | ξ₅₈ ξ₆₀   ξ₆₀²
qrMul(s(136), s(1), s(136), qr, stack);
qrSub(s(136), s(128), s(136), qr); //  | ξ₁₁₉
qrSqr(s(128), s(72), qr, stack); //  | ξ₆₁² | ξ₆₁² | ξ₆₁² | ξ₆₁² | ξ₆₁²
qrMul(s(71), s(71), s(36), qr, stack); //  | ξ₆₀ ξ₆₂ | ξ₆₀ ξ₆₂ | ξ₆₀ ξ₆₂ | ξ₆₀ ξ₆₂ | ξ₆₀ ξ₆₂
qrMul(s(137), s(71), s(124), qr, stack);
qrMul(s(138), s(132), s(128), qr, stack);
qrSub(s(138), s(137), s(138), qr); //  | ξ₁₂₀
qrMul(s(137), s(71), s(133), qr, stack); //  | ξ₆₀ ξ₆₂   ξ₆₀²
qrMul(s(139), s(68), s(128), qr, stack); //  | ξ₅₉ ξ₆₁   ξ₆₁²
qrMul(s(137), s(1), s(137), qr, stack);
qrSub(s(139), s(137), s(139), qr); //  | ξ₁₂₁
qrSqr(s(137), s(36), qr, stack); //  | ξ₆₂² | ξ₆₂² | ξ₆₂² | ξ₆₂² | ξ₆₂² | ξ₆₂²
qrMul(s(72), s(72), s(73), qr, stack); //  | ξ₆₁ ξ₆₃ | ξ₆₁ ξ₆₃ | ξ₆₁ ξ₆₃ | ξ₆₁ ξ₆₃ | ξ₆₁ ξ₆₃ | ξ₆₁ ξ₆₃
qrMul(s(133), s(72), s(133), qr, stack);
qrMul(s(68), s(68), s(137), qr, stack);
qrSub(s(68), s(133), s(68), qr); //  | ξ₁₂₂
qrMul(s(133), s(72), s(128), qr, stack); //  | ξ₆₁ ξ₆₃   ξ₆₁²
qrMul(s(140), s(71), s(137), qr, stack); //  | ξ₆₀ ξ₆₂   ξ₆₂²
qrMul(s(140), s(1), s(140), qr, stack);
qrSub(s(140), s(133), s(140), qr); //  | ξ₁₂₃
qrSqr(s(133), s(73), qr, stack); //  | ξ₆₃² | ξ₆₃² | ξ₆₃² | ξ₆₃² | ξ₆₃²
qrMul(s(141), s(36), s(75), qr, stack); //  | ξ₆₂ ξ₆₄ | ξ₆₂ ξ₆₄ | ξ₆₂ ξ₆₄ | ξ₆₂ ξ₆₄ | ξ₆₂ ξ₆₄
qrMul(s(142), s(141), s(128), qr, stack);
qrMul(s(143), s(71), s(133), qr, stack);
qrSub(s(143), s(142), s(143), qr); //  | ξ₁₂₄
qrMul(s(142), s(141), s(137), qr, stack); //  | ξ₆₂ ξ₆₄   ξ₆₂²
qrMul(s(144), s(72), s(133), qr, stack); //  | ξ₆₁ ξ₆₃   ξ₆₃²
qrMul(s(142), s(1), s(142), qr, stack);
qrSub(s(144), s(142), s(144), qr); //  | ξ₁₂₅
qrSqr(s(142), s(75), qr, stack); //  | ξ₆₄² | ξ₆₄² | ξ₆₄² | ξ₆₄² | ξ₆₄²
qrMul(s(73), s(73), s(76), qr, stack); //  | ξ₆₃ ξ₆₅ | ξ₆₃ ξ₆₅ | ξ₆₃ ξ₆₅ | ξ₆₃ ξ₆₅ | ξ₆₃ ξ₆₅
qrMul(s(137), s(73), s(137), qr, stack);
qrMul(s(72), s(72), s(142), qr, stack);
qrSub(s(72), s(137), s(72), qr); //  | ξ₁₂₆
qrMul(s(137), s(73), s(133), qr, stack); //  | ξ₆₃ ξ₆₅   ξ₆₃²
qrMul(s(145), s(141), s(142), qr, stack); //  | ξ₆₂ ξ₆₄   ξ₆₄²
qrMul(s(145), s(1), s(145), qr, stack);
qrSub(s(145), s(137), s(145), qr); //  | ξ₁₂₇
qrSqr(s(137), s(76), qr, stack); //  | ξ₆₅² | ξ₆₅² | ξ₆₅²
qrMul(s(75), s(75), s(37), qr, stack); //  | ξ₆₄ ξ₆₆ | ξ₆₄ ξ₆₆ | ξ₆₄ ξ₆₆
qrMul(s(146), s(75), s(133), qr, stack);
qrMul(s(147), s(141), s(137), qr, stack);
qrSub(s(147), s(146), s(147), qr); //  | ξ₁₂₈
qrMul(s(146), s(75), s(142), qr, stack); //  | ξ₆₄ ξ₆₆   ξ₆₄²
qrMul(s(148), s(73), s(137), qr, stack); //  | ξ₆₃ ξ₆₅   ξ₆₅²
qrMul(s(146), s(1), s(146), qr, stack);
qrSub(s(148), s(146), s(148), qr); //  | ξ₁₂₉
qrMul(s(2), s(0), s(2), qr, stack); //  | [3]Pjx
qrMul(s(6), y, s(6), qr, stack); //  | [3]Pjy
qrMul(s(10), s(0), s(10), qr, stack); //  | [5]Pjx
qrMul(s(3), y, s(3), qr, stack); //  | [5]Pjy
qrMul(s(15), s(0), s(15), qr, stack); //  | [7]Pjx
qrMul(s(5), y, s(5), qr, stack); //  | [7]Pjy
qrMul(s(12), s(0), s(12), qr, stack); //  | [9]Pjx
qrMul(s(9), y, s(9), qr, stack); //  | [9]Pjy
qrMul(s(24), s(0), s(24), qr, stack); //  | [11]Pjx
qrMul(s(13), y, s(13), qr, stack); //  | [11]Pjy
qrMul(s(17), s(0), s(17), qr, stack); //  | [13]Pjx
qrMul(s(14), y, s(14), qr, stack); //  | [13]Pjy
qrMul(s(33), s(0), s(33), qr, stack); //  | [15]Pjx
qrMul(s(18), y, s(18), qr, stack); //  | [15]Pjy
qrMul(s(21), s(0), s(21), qr, stack); //  | [17]Pjx
qrMul(s(19), y, s(19), qr, stack); //  | [17]Pjy
qrMul(s(42), s(0), s(42), qr, stack); //  | [19]Pjx
qrMul(s(22), y, s(22), qr, stack); //  | [19]Pjy
qrMul(s(26), s(0), s(26), qr, stack); //  | [21]Pjx
qrMul(s(23), y, s(23), qr, stack); //  | [21]Pjy
qrMul(s(51), s(0), s(51), qr, stack); //  | [23]Pjx
qrMul(s(27), y, s(27), qr, stack); //  | [23]Pjy
qrMul(s(30), s(0), s(30), qr, stack); //  | [25]Pjx
qrMul(s(28), y, s(28), qr, stack); //  | [25]Pjy
qrMul(s(60), s(0), s(60), qr, stack); //  | [27]Pjx
qrMul(s(31), y, s(31), qr, stack); //  | [27]Pjy
qrMul(s(35), s(0), s(35), qr, stack); //  | [29]Pjx
qrMul(s(32), y, s(32), qr, stack); //  | [29]Pjy
qrMul(s(69), s(0), s(69), qr, stack); //  | [31]Pjx
qrMul(s(36), y, s(36), qr, stack); //  | [31]Pjy
qrMul(s(39), s(0), s(39), qr, stack); //  | [33]Pjx
qrMul(s(146), y, s(37), qr, stack); //  | [33]Pjy
qrMul(s(78), s(0), s(78), qr, stack); //  | [35]Pjx
qrMul(s(1), y, s(40), qr, stack); //  | [35]Pjy
qrMul(s(44), s(0), s(44), qr, stack); //  | [37]Pjx
qrMul(s(149), y, s(41), qr, stack); //  | [37]Pjy
qrMul(s(87), s(0), s(87), qr, stack); //  | [39]Pjx
qrMul(s(150), y, s(45), qr, stack); //  | [39]Pjy
qrMul(s(48), s(0), s(48), qr, stack); //  | [41]Pjx
qrMul(s(151), y, s(46), qr, stack); //  | [41]Pjy
qrMul(s(96), s(0), s(96), qr, stack); //  | [43]Pjx
qrMul(s(152), y, s(49), qr, stack); //  | [43]Pjy
qrMul(s(53), s(0), s(53), qr, stack); //  | [45]Pjx
qrMul(s(153), y, s(50), qr, stack); //  | [45]Pjy
qrMul(s(105), s(0), s(105), qr, stack); //  | [47]Pjx
qrMul(s(154), y, s(54), qr, stack); //  | [47]Pjy
qrMul(s(57), s(0), s(57), qr, stack); //  | [49]Pjx
qrMul(s(155), y, s(55), qr, stack); //  | [49]Pjy
qrMul(s(114), s(0), s(114), qr, stack); //  | [51]Pjx
qrMul(s(156), y, s(58), qr, stack); //  | [51]Pjy
qrMul(s(62), s(0), s(62), qr, stack); //  | [53]Pjx
qrMul(s(157), y, s(59), qr, stack); //  | [53]Pjy
qrMul(s(123), s(0), s(123), qr, stack); //  | [55]Pjx
qrMul(s(158), y, s(63), qr, stack); //  | [55]Pjy
qrMul(s(66), s(0), s(66), qr, stack); //  | [57]Pjx
qrMul(s(159), y, s(64), qr, stack); //  | [57]Pjy
qrMul(s(132), s(0), s(132), qr, stack); //  | [59]Pjx
qrMul(s(160), y, s(67), qr, stack); //  | [59]Pjy
qrMul(s(71), s(0), s(71), qr, stack); //  | [61]Pjx
qrMul(s(161), y, s(68), qr, stack); //  | [61]Pjy
qrMul(s(141), s(0), s(141), qr, stack); //  | [63]Pjx
qrMul(s(162), y, s(72), qr, stack); //  | [63]Pjy
qrSqr(s(163), s(37), qr, stack); //  | ξ₆₆² | ξ₆₆²
qrMul(s(75), s(0), s(75), qr, stack); //  | [65]Pjx
qrMul(s(76), s(76), s(77), qr, stack); //  | ξ₆₅ ξ₆₇ | ξ₆₅ ξ₆₇
qrMul(s(142), s(76), s(142), qr, stack);
qrMul(s(73), s(73), s(163), qr, stack);
qrSub(s(73), s(142), s(73), qr);
qrMul(s(73), y, s(73), qr, stack); //  | [65]Pjy
qrSqr(s(142), s(77), qr, stack); //  | ξ₆₇²
qrSqr(s(164), s(80), qr, stack); //  | ξ₆₈² | ξ₆₈²
qrMul(s(37), s(37), s(80), qr, stack); //  | ξ₆₆ ξ₆₈
qrMul(s(37), s(0), s(37), qr, stack); //  | [67]Pjx
qrMul(s(77), s(77), s(81), qr, stack); //  | ξ₆₇ ξ₆₉ | ξ₆₇ ξ₆₉
qrMul(s(163), s(77), s(163), qr, stack);
qrMul(s(76), s(76), s(164), qr, stack);
qrSub(s(76), s(163), s(76), qr);
qrMul(s(76), y, s(76), qr, stack); //  | [67]Pjy
qrSqr(s(163), s(81), qr, stack); //  | ξ₆₉²
qrSqr(s(165), s(40), qr, stack); //  | ξ₇₀² | ξ₇₀²
qrMul(s(80), s(80), s(40), qr, stack); //  | ξ₆₈ ξ₇₀
qrMul(s(80), s(0), s(80), qr, stack); //  | [69]Pjx
qrMul(s(81), s(81), s(82), qr, stack); //  | ξ₆₉ ξ₇₁ | ξ₆₉ ξ₇₁
qrMul(s(164), s(81), s(164), qr, stack);
qrMul(s(77), s(77), s(165), qr, stack);
qrSub(s(77), s(164), s(77), qr);
qrMul(s(77), y, s(77), qr, stack); //  | [69]Pjy
qrSqr(s(164), s(82), qr, stack); //  | ξ₇₁²
qrSqr(s(166), s(84), qr, stack); //  | ξ₇₂² | ξ₇₂²
qrMul(s(40), s(40), s(84), qr, stack); //  | ξ₇₀ ξ₇₂
qrMul(s(40), s(0), s(40), qr, stack); //  | [71]Pjx
qrMul(s(82), s(82), s(85), qr, stack); //  | ξ₇₁ ξ₇₃ | ξ₇₁ ξ₇₃
qrMul(s(165), s(82), s(165), qr, stack);
qrMul(s(81), s(81), s(166), qr, stack);
qrSub(s(81), s(165), s(81), qr);
qrMul(s(81), y, s(81), qr, stack); //  | [71]Pjy
qrSqr(s(165), s(85), qr, stack); //  | ξ₇₃²
qrSqr(s(167), s(41), qr, stack); //  | ξ₇₄² | ξ₇₄²
qrMul(s(84), s(84), s(41), qr, stack); //  | ξ₇₂ ξ₇₄
qrMul(s(84), s(0), s(84), qr, stack); //  | [73]Pjx
qrMul(s(85), s(85), s(86), qr, stack); //  | ξ₇₃ ξ₇₅ | ξ₇₃ ξ₇₅
qrMul(s(166), s(85), s(166), qr, stack);
qrMul(s(82), s(82), s(167), qr, stack);
qrSub(s(82), s(166), s(82), qr);
qrMul(s(82), y, s(82), qr, stack); //  | [73]Pjy
qrSqr(s(166), s(86), qr, stack); //  | ξ₇₅²
qrSqr(s(168), s(89), qr, stack); //  | ξ₇₆² | ξ₇₆²
qrMul(s(41), s(41), s(89), qr, stack); //  | ξ₇₄ ξ₇₆
qrMul(s(41), s(0), s(41), qr, stack); //  | [75]Pjx
qrMul(s(86), s(86), s(90), qr, stack); //  | ξ₇₅ ξ₇₇ | ξ₇₅ ξ₇₇
qrMul(s(167), s(86), s(167), qr, stack);
qrMul(s(85), s(85), s(168), qr, stack);
qrSub(s(85), s(167), s(85), qr);
qrMul(s(85), y, s(85), qr, stack); //  | [75]Pjy
qrSqr(s(167), s(90), qr, stack); //  | ξ₇₇²
qrSqr(s(169), s(45), qr, stack); //  | ξ₇₈² | ξ₇₈²
qrMul(s(89), s(89), s(45), qr, stack); //  | ξ₇₆ ξ₇₈
qrMul(s(89), s(0), s(89), qr, stack); //  | [77]Pjx
qrMul(s(90), s(90), s(91), qr, stack); //  | ξ₇₇ ξ₇₉ | ξ₇₇ ξ₇₉
qrMul(s(168), s(90), s(168), qr, stack);
qrMul(s(86), s(86), s(169), qr, stack);
qrSub(s(86), s(168), s(86), qr);
qrMul(s(86), y, s(86), qr, stack); //  | [77]Pjy
qrSqr(s(168), s(91), qr, stack); //  | ξ₇₉²
qrSqr(s(170), s(93), qr, stack); //  | ξ₈₀² | ξ₈₀²
qrMul(s(45), s(45), s(93), qr, stack); //  | ξ₇₈ ξ₈₀
qrMul(s(45), s(0), s(45), qr, stack); //  | [79]Pjx
qrMul(s(91), s(91), s(94), qr, stack); //  | ξ₇₉ ξ₈₁ | ξ₇₉ ξ₈₁
qrMul(s(169), s(91), s(169), qr, stack);
qrMul(s(90), s(90), s(170), qr, stack);
qrSub(s(90), s(169), s(90), qr);
qrMul(s(90), y, s(90), qr, stack); //  | [79]Pjy
qrSqr(s(169), s(94), qr, stack); //  | ξ₈₁²
qrSqr(s(171), s(46), qr, stack); //  | ξ₈₂² | ξ₈₂²
qrMul(s(93), s(93), s(46), qr, stack); //  | ξ₈₀ ξ₈₂
qrMul(s(93), s(0), s(93), qr, stack); //  | [81]Pjx
qrMul(s(94), s(94), s(95), qr, stack); //  | ξ₈₁ ξ₈₃ | ξ₈₁ ξ₈₃
qrMul(s(170), s(94), s(170), qr, stack);
qrMul(s(91), s(91), s(171), qr, stack);
qrSub(s(91), s(170), s(91), qr);
qrMul(s(91), y, s(91), qr, stack); //  | [81]Pjy
qrSqr(s(170), s(95), qr, stack); //  | ξ₈₃²
qrSqr(s(172), s(98), qr, stack); //  | ξ₈₄² | ξ₈₄²
qrMul(s(46), s(46), s(98), qr, stack); //  | ξ₈₂ ξ₈₄
qrMul(s(46), s(0), s(46), qr, stack); //  | [83]Pjx
qrMul(s(95), s(95), s(99), qr, stack); //  | ξ₈₃ ξ₈₅ | ξ₈₃ ξ₈₅
qrMul(s(171), s(95), s(171), qr, stack);
qrMul(s(94), s(94), s(172), qr, stack);
qrSub(s(94), s(171), s(94), qr);
qrMul(s(94), y, s(94), qr, stack); //  | [83]Pjy
qrSqr(s(171), s(99), qr, stack); //  | ξ₈₅²
qrSqr(s(173), s(49), qr, stack); //  | ξ₈₆² | ξ₈₆²
qrMul(s(98), s(98), s(49), qr, stack); //  | ξ₈₄ ξ₈₆
qrMul(s(98), s(0), s(98), qr, stack); //  | [85]Pjx
qrMul(s(99), s(99), s(100), qr, stack); //  | ξ₈₅ ξ₈₇ | ξ₈₅ ξ₈₇
qrMul(s(172), s(99), s(172), qr, stack);
qrMul(s(95), s(95), s(173), qr, stack);
qrSub(s(95), s(172), s(95), qr);
qrMul(s(95), y, s(95), qr, stack); //  | [85]Pjy
qrSqr(s(172), s(100), qr, stack); //  | ξ₈₇²
qrSqr(s(174), s(102), qr, stack); //  | ξ₈₈² | ξ₈₈²
qrMul(s(49), s(49), s(102), qr, stack); //  | ξ₈₆ ξ₈₈
qrMul(s(49), s(0), s(49), qr, stack); //  | [87]Pjx
qrMul(s(100), s(100), s(103), qr, stack); //  | ξ₈₇ ξ₈₉ | ξ₈₇ ξ₈₉
qrMul(s(173), s(100), s(173), qr, stack);
qrMul(s(99), s(99), s(174), qr, stack);
qrSub(s(99), s(173), s(99), qr);
qrMul(s(99), y, s(99), qr, stack); //  | [87]Pjy
qrSqr(s(173), s(103), qr, stack); //  | ξ₈₉²
qrSqr(s(175), s(50), qr, stack); //  | ξ₉₀² | ξ₉₀²
qrMul(s(102), s(102), s(50), qr, stack); //  | ξ₈₈ ξ₉₀
qrMul(s(102), s(0), s(102), qr, stack); //  | [89]Pjx
qrMul(s(103), s(103), s(104), qr, stack); //  | ξ₈₉ ξ₉₁ | ξ₈₉ ξ₉₁
qrMul(s(174), s(103), s(174), qr, stack);
qrMul(s(100), s(100), s(175), qr, stack);
qrSub(s(100), s(174), s(100), qr);
qrMul(s(100), y, s(100), qr, stack); //  | [89]Pjy
qrSqr(s(174), s(104), qr, stack); //  | ξ₉₁²
qrSqr(s(176), s(107), qr, stack); //  | ξ₉₂² | ξ₉₂²
qrMul(s(50), s(50), s(107), qr, stack); //  | ξ₉₀ ξ₉₂
qrMul(s(50), s(0), s(50), qr, stack); //  | [91]Pjx
qrMul(s(104), s(104), s(108), qr, stack); //  | ξ₉₁ ξ₉₃ | ξ₉₁ ξ₉₃
qrMul(s(175), s(104), s(175), qr, stack);
qrMul(s(103), s(103), s(176), qr, stack);
qrSub(s(103), s(175), s(103), qr);
qrMul(s(103), y, s(103), qr, stack); //  | [91]Pjy
qrSqr(s(175), s(108), qr, stack); //  | ξ₉₃²
qrSqr(s(177), s(54), qr, stack); //  | ξ₉₄² | ξ₉₄²
qrMul(s(107), s(107), s(54), qr, stack); //  | ξ₉₂ ξ₉₄
qrMul(s(107), s(0), s(107), qr, stack); //  | [93]Pjx
qrMul(s(108), s(108), s(109), qr, stack); //  | ξ₉₃ ξ₉₅ | ξ₉₃ ξ₉₅
qrMul(s(176), s(108), s(176), qr, stack);
qrMul(s(104), s(104), s(177), qr, stack);
qrSub(s(104), s(176), s(104), qr);
qrMul(s(104), y, s(104), qr, stack); //  | [93]Pjy
qrSqr(s(176), s(109), qr, stack); //  | ξ₉₅²
qrSqr(s(178), s(111), qr, stack); //  | ξ₉₆² | ξ₉₆²
qrMul(s(54), s(54), s(111), qr, stack); //  | ξ₉₄ ξ₉₆
qrMul(s(54), s(0), s(54), qr, stack); //  | [95]Pjx
qrMul(s(109), s(109), s(112), qr, stack); //  | ξ₉₅ ξ₉₇ | ξ₉₅ ξ₉₇
qrMul(s(177), s(109), s(177), qr, stack);
qrMul(s(108), s(108), s(178), qr, stack);
qrSub(s(108), s(177), s(108), qr);
qrMul(s(108), y, s(108), qr, stack); //  | [95]Pjy
qrSqr(s(177), s(112), qr, stack); //  | ξ₉₇²
qrSqr(s(179), s(55), qr, stack); //  | ξ₉₈² | ξ₉₈²
qrMul(s(111), s(111), s(55), qr, stack); //  | ξ₉₆ ξ₉₈
qrMul(s(111), s(0), s(111), qr, stack); //  | [97]Pjx
qrMul(s(112), s(112), s(113), qr, stack); //  | ξ₉₇ ξ₉₉ | ξ₉₇ ξ₉₉
qrMul(s(178), s(112), s(178), qr, stack);
qrMul(s(109), s(109), s(179), qr, stack);
qrSub(s(109), s(178), s(109), qr);
qrMul(s(109), y, s(109), qr, stack); //  | [97]Pjy
qrSqr(s(178), s(113), qr, stack); //  | ξ₉₉²
qrSqr(s(180), s(116), qr, stack); //  | ξ₁₀₀² | ξ₁₀₀²
qrMul(s(55), s(55), s(116), qr, stack); //  | ξ₉₈ ξ₁₀₀
qrMul(s(55), s(0), s(55), qr, stack); //  | [99]Pjx
qrMul(s(113), s(113), s(117), qr, stack); //  | ξ₉₉ ξ₁₀₁ | ξ₉₉ ξ₁₀₁
qrMul(s(179), s(113), s(179), qr, stack);
qrMul(s(112), s(112), s(180), qr, stack);
qrSub(s(112), s(179), s(112), qr);
qrMul(s(112), y, s(112), qr, stack); //  | [99]Pjy
qrSqr(s(179), s(117), qr, stack); //  | ξ₁₀₁²
qrSqr(s(181), s(58), qr, stack); //  | ξ₁₀₂² | ξ₁₀₂²
qrMul(s(116), s(116), s(58), qr, stack); //  | ξ₁₀₀ ξ₁₀₂
qrMul(s(116), s(0), s(116), qr, stack); //  | [101]Pjx
qrMul(s(117), s(117), s(118), qr, stack); //  | ξ₁₀₁ ξ₁₀₃ | ξ₁₀₁ ξ₁₀₃
qrMul(s(180), s(117), s(180), qr, stack);
qrMul(s(113), s(113), s(181), qr, stack);
qrSub(s(113), s(180), s(113), qr);
qrMul(s(113), y, s(113), qr, stack); //  | [101]Pjy
qrSqr(s(180), s(118), qr, stack); //  | ξ₁₀₃²
qrSqr(s(182), s(120), qr, stack); //  | ξ₁₀₄² | ξ₁₀₄²
qrMul(s(58), s(58), s(120), qr, stack); //  | ξ₁₀₂ ξ₁₀₄
qrMul(s(58), s(0), s(58), qr, stack); //  | [103]Pjx
qrMul(s(118), s(118), s(121), qr, stack); //  | ξ₁₀₃ ξ₁₀₅ | ξ₁₀₃ ξ₁₀₅
qrMul(s(181), s(118), s(181), qr, stack);
qrMul(s(117), s(117), s(182), qr, stack);
qrSub(s(117), s(181), s(117), qr);
qrMul(s(117), y, s(117), qr, stack); //  | [103]Pjy
qrSqr(s(181), s(121), qr, stack); //  | ξ₁₀₅²
qrSqr(s(183), s(59), qr, stack); //  | ξ₁₀₆² | ξ₁₀₆²
qrMul(s(120), s(120), s(59), qr, stack); //  | ξ₁₀₄ ξ₁₀₆
qrMul(s(120), s(0), s(120), qr, stack); //  | [105]Pjx
qrMul(s(121), s(121), s(122), qr, stack); //  | ξ₁₀₅ ξ₁₀₇ | ξ₁₀₅ ξ₁₀₇
qrMul(s(182), s(121), s(182), qr, stack);
qrMul(s(118), s(118), s(183), qr, stack);
qrSub(s(118), s(182), s(118), qr);
qrMul(s(118), y, s(118), qr, stack); //  | [105]Pjy
qrSqr(s(182), s(122), qr, stack); //  | ξ₁₀₇²
qrSqr(s(184), s(125), qr, stack); //  | ξ₁₀₈² | ξ₁₀₈²
qrMul(s(59), s(59), s(125), qr, stack); //  | ξ₁₀₆ ξ₁₀₈
qrMul(s(59), s(0), s(59), qr, stack); //  | [107]Pjx
qrMul(s(122), s(122), s(126), qr, stack); //  | ξ₁₀₇ ξ₁₀₉ | ξ₁₀₇ ξ₁₀₉
qrMul(s(183), s(122), s(183), qr, stack);
qrMul(s(121), s(121), s(184), qr, stack);
qrSub(s(121), s(183), s(121), qr);
qrMul(s(121), y, s(121), qr, stack); //  | [107]Pjy
qrSqr(s(183), s(126), qr, stack); //  | ξ₁₀₉²
qrSqr(s(185), s(63), qr, stack); //  | ξ₁₁₀² | ξ₁₁₀²
qrMul(s(125), s(125), s(63), qr, stack); //  | ξ₁₀₈ ξ₁₁₀
qrMul(s(125), s(0), s(125), qr, stack); //  | [109]Pjx
qrMul(s(126), s(126), s(127), qr, stack); //  | ξ₁₀₉ ξ₁₁₁ | ξ₁₀₉ ξ₁₁₁
qrMul(s(184), s(126), s(184), qr, stack);
qrMul(s(122), s(122), s(185), qr, stack);
qrSub(s(122), s(184), s(122), qr);
qrMul(s(122), y, s(122), qr, stack); //  | [109]Pjy
qrSqr(s(184), s(127), qr, stack); //  | ξ₁₁₁²
qrSqr(s(186), s(129), qr, stack); //  | ξ₁₁₂² | ξ₁₁₂²
qrMul(s(63), s(63), s(129), qr, stack); //  | ξ₁₁₀ ξ₁₁₂
qrMul(s(63), s(0), s(63), qr, stack); //  | [111]Pjx
qrMul(s(127), s(127), s(130), qr, stack); //  | ξ₁₁₁ ξ₁₁₃ | ξ₁₁₁ ξ₁₁₃
qrMul(s(185), s(127), s(185), qr, stack);
qrMul(s(126), s(126), s(186), qr, stack);
qrSub(s(126), s(185), s(126), qr);
qrMul(s(126), y, s(126), qr, stack); //  | [111]Pjy
qrSqr(s(185), s(130), qr, stack); //  | ξ₁₁₃²
qrSqr(s(187), s(64), qr, stack); //  | ξ₁₁₄² | ξ₁₁₄²
qrMul(s(129), s(129), s(64), qr, stack); //  | ξ₁₁₂ ξ₁₁₄
qrMul(s(129), s(0), s(129), qr, stack); //  | [113]Pjx
qrMul(s(130), s(130), s(131), qr, stack); //  | ξ₁₁₃ ξ₁₁₅ | ξ₁₁₃ ξ₁₁₅
qrMul(s(186), s(130), s(186), qr, stack);
qrMul(s(127), s(127), s(187), qr, stack);
qrSub(s(127), s(186), s(127), qr);
qrMul(s(127), y, s(127), qr, stack); //  | [113]Pjy
qrSqr(s(186), s(131), qr, stack); //  | ξ₁₁₅²
qrSqr(s(188), s(134), qr, stack); //  | ξ₁₁₆² | ξ₁₁₆²
qrMul(s(64), s(64), s(134), qr, stack); //  | ξ₁₁₄ ξ₁₁₆
qrMul(s(64), s(0), s(64), qr, stack); //  | [115]Pjx
qrMul(s(131), s(131), s(135), qr, stack); //  | ξ₁₁₅ ξ₁₁₇ | ξ₁₁₅ ξ₁₁₇
qrMul(s(187), s(131), s(187), qr, stack);
qrMul(s(130), s(130), s(188), qr, stack);
qrSub(s(130), s(187), s(130), qr);
qrMul(s(130), y, s(130), qr, stack); //  | [115]Pjy
qrSqr(s(187), s(135), qr, stack); //  | ξ₁₁₇²
qrSqr(s(189), s(67), qr, stack); //  | ξ₁₁₈² | ξ₁₁₈²
qrMul(s(134), s(134), s(67), qr, stack); //  | ξ₁₁₆ ξ₁₁₈
qrMul(s(134), s(0), s(134), qr, stack); //  | [117]Pjx
qrMul(s(135), s(135), s(136), qr, stack); //  | ξ₁₁₇ ξ₁₁₉ | ξ₁₁₇ ξ₁₁₉
qrMul(s(188), s(135), s(188), qr, stack);
qrMul(s(131), s(131), s(189), qr, stack);
qrSub(s(131), s(188), s(131), qr);
qrMul(s(131), y, s(131), qr, stack); //  | [117]Pjy
qrSqr(s(188), s(136), qr, stack); //  | ξ₁₁₉²
qrSqr(s(190), s(138), qr, stack); //  | ξ₁₂₀² | ξ₁₂₀²
qrMul(s(67), s(67), s(138), qr, stack); //  | ξ₁₁₈ ξ₁₂₀
qrMul(s(67), s(0), s(67), qr, stack); //  | [119]Pjx
qrMul(s(136), s(136), s(139), qr, stack); //  | ξ₁₁₉ ξ₁₂₁ | ξ₁₁₉ ξ₁₂₁
qrMul(s(189), s(136), s(189), qr, stack);
qrMul(s(135), s(135), s(190), qr, stack);
qrSub(s(135), s(189), s(135), qr);
qrMul(s(135), y, s(135), qr, stack); //  | [119]Pjy
qrSqr(s(189), s(139), qr, stack); //  | ξ₁₂₁²
qrSqr(s(191), s(68), qr, stack); //  | ξ₁₂₂² | ξ₁₂₂²
qrMul(s(138), s(138), s(68), qr, stack); //  | ξ₁₂₀ ξ₁₂₂
qrMul(s(138), s(0), s(138), qr, stack); //  | [121]Pjx
qrMul(s(139), s(139), s(140), qr, stack); //  | ξ₁₂₁ ξ₁₂₃ | ξ₁₂₁ ξ₁₂₃
qrMul(s(190), s(139), s(190), qr, stack);
qrMul(s(136), s(136), s(191), qr, stack);
qrSub(s(136), s(190), s(136), qr);
qrMul(s(136), y, s(136), qr, stack); //  | [121]Pjy
qrSqr(s(190), s(140), qr, stack); //  | ξ₁₂₃²
qrSqr(s(192), s(143), qr, stack); //  | ξ₁₂₄² | ξ₁₂₄²
qrMul(s(68), s(68), s(143), qr, stack); //  | ξ₁₂₂ ξ₁₂₄
qrMul(s(68), s(0), s(68), qr, stack); //  | [123]Pjx
qrMul(s(140), s(140), s(144), qr, stack); //  | ξ₁₂₃ ξ₁₂₅ | ξ₁₂₃ ξ₁₂₅
qrMul(s(191), s(140), s(191), qr, stack);
qrMul(s(139), s(139), s(192), qr, stack);
qrSub(s(139), s(191), s(139), qr);
qrMul(s(139), y, s(139), qr, stack); //  | [123]Pjy
qrSqr(s(191), s(144), qr, stack); //  | ξ₁₂₅²
qrSqr(s(193), s(72), qr, stack); //  | ξ₁₂₆² | ξ₁₂₆²
qrMul(s(143), s(143), s(72), qr, stack); //  | ξ₁₂₄ ξ₁₂₆
qrMul(s(143), s(0), s(143), qr, stack); //  | [125]Pjx
qrMul(s(144), s(144), s(145), qr, stack); //  | ξ₁₂₅ ξ₁₂₇ | ξ₁₂₅ ξ₁₂₇
qrMul(s(192), s(144), s(192), qr, stack);
qrMul(s(140), s(140), s(193), qr, stack);
qrSub(s(140), s(192), s(140), qr);
qrMul(s(140), y, s(140), qr, stack); //  | [125]Pjy
qrSqr(s(192), s(145), qr, stack); //  | ξ₁₂₇²
qrSqr(s(194), s(147), qr, stack); //  | ξ₁₂₈²
qrMul(s(147), s(72), s(147), qr, stack); //  | ξ₁₂₆ ξ₁₂₈
qrMul(s(147), s(0), s(147), qr, stack); //  | [127]Pjx
qrMul(s(148), s(145), s(148), qr, stack); //  | ξ₁₂₇ ξ₁₂₉
qrMul(s(193), s(148), s(193), qr, stack);
qrMul(s(194), s(144), s(194), qr, stack);
qrSub(s(194), s(193), s(194), qr);
qrMul(s(194), y, s(194), qr, stack); //  | [127]Pjy
qrMul(s(193), s(191), s(192), qr, stack);
qrMul(s(144), s(190), s(193), qr, stack);
qrMul(s(148), s(189), s(144), qr, stack);
qrMul(s(145), s(188), s(148), qr, stack);
qrMul(s(0), s(187), s(145), qr, stack);
qrMul(s(72), s(186), s(0), qr, stack);
qrMul(s(195), s(185), s(72), qr, stack);
qrMul(s(196), s(184), s(195), qr, stack);
qrMul(s(197), s(183), s(196), qr, stack);
qrMul(s(198), s(182), s(197), qr, stack);
qrMul(s(199), s(181), s(198), qr, stack);
qrMul(s(200), s(180), s(199), qr, stack);
qrMul(s(201), s(179), s(200), qr, stack);
qrMul(s(202), s(178), s(201), qr, stack);
qrMul(s(203), s(177), s(202), qr, stack);
qrMul(s(204), s(176), s(203), qr, stack);
qrMul(s(205), s(175), s(204), qr, stack);
qrMul(s(206), s(174), s(205), qr, stack);
qrMul(s(207), s(173), s(206), qr, stack);
qrMul(s(208), s(172), s(207), qr, stack);
qrMul(s(209), s(171), s(208), qr, stack);
qrMul(s(210), s(170), s(209), qr, stack);
qrMul(s(211), s(169), s(210), qr, stack);
qrMul(s(212), s(168), s(211), qr, stack);
qrMul(s(213), s(167), s(212), qr, stack);
qrMul(s(214), s(166), s(213), qr, stack);
qrMul(s(215), s(165), s(214), qr, stack);
qrMul(s(216), s(164), s(215), qr, stack);
qrMul(s(217), s(163), s(216), qr, stack);
qrMul(s(218), s(142), s(217), qr, stack);
qrMul(s(219), s(137), s(218), qr, stack);
qrMul(s(220), s(133), s(219), qr, stack);
qrMul(s(221), s(128), s(220), qr, stack);
qrMul(s(222), s(124), s(221), qr, stack);
qrMul(s(223), s(119), s(222), qr, stack);
qrMul(s(224), s(115), s(223), qr, stack);
qrMul(s(225), s(110), s(224), qr, stack);
qrMul(s(226), s(106), s(225), qr, stack);
qrMul(s(227), s(101), s(226), qr, stack);
qrMul(s(228), s(97), s(227), qr, stack);
qrMul(s(229), s(92), s(228), qr, stack);
qrMul(s(230), s(88), s(229), qr, stack);
qrMul(s(231), s(83), s(230), qr, stack);
qrMul(s(232), s(79), s(231), qr, stack);
qrMul(s(233), s(74), s(232), qr, stack);
qrMul(s(234), s(70), s(233), qr, stack);
qrMul(s(235), s(65), s(234), qr, stack);
qrMul(s(236), s(61), s(235), qr, stack);
qrMul(s(237), s(56), s(236), qr, stack);
qrMul(s(238), s(52), s(237), qr, stack);
qrMul(s(239), s(47), s(238), qr, stack);
qrMul(s(240), s(43), s(239), qr, stack);
qrMul(s(241), s(38), s(240), qr, stack);
qrMul(s(242), s(34), s(241), qr, stack);
qrMul(s(243), s(29), s(242), qr, stack);
qrMul(s(244), s(25), s(243), qr, stack);
qrMul(s(245), s(20), s(244), qr, stack);
qrMul(s(246), s(16), s(245), qr, stack);
qrMul(s(247), s(11), s(246), qr, stack);
qrMul(s(248), s(7), s(247), qr, stack);
qrMul(s(249), s(4), s(248), qr, stack);
qrMul(s(250), s(8), s(249), qr, stack);
qrInv(s(250), s(250), qr, stack);
qrMul(s(249), s(250), s(249), qr, stack);
qrMul(s(8), s(250), s(8), qr, stack);
qrMul(s(248), s(8), s(248), qr, stack);
qrMul(s(4), s(8), s(4), qr, stack);
qrMul(s(247), s(4), s(247), qr, stack);
qrMul(s(7), s(4), s(7), qr, stack);
qrMul(s(246), s(7), s(246), qr, stack);
qrMul(s(11), s(7), s(11), qr, stack);
qrMul(s(245), s(11), s(245), qr, stack);
qrMul(s(16), s(11), s(16), qr, stack);
qrMul(s(244), s(16), s(244), qr, stack);
qrMul(s(20), s(16), s(20), qr, stack);
qrMul(s(243), s(20), s(243), qr, stack);
qrMul(s(25), s(20), s(25), qr, stack);
qrMul(s(242), s(25), s(242), qr, stack);
qrMul(s(29), s(25), s(29), qr, stack);
qrMul(s(241), s(29), s(241), qr, stack);
qrMul(s(34), s(29), s(34), qr, stack);
qrMul(s(240), s(34), s(240), qr, stack);
qrMul(s(38), s(34), s(38), qr, stack);
qrMul(s(239), s(38), s(239), qr, stack);
qrMul(s(43), s(38), s(43), qr, stack);
qrMul(s(238), s(43), s(238), qr, stack);
qrMul(s(47), s(43), s(47), qr, stack);
qrMul(s(237), s(47), s(237), qr, stack);
qrMul(s(52), s(47), s(52), qr, stack);
qrMul(s(236), s(52), s(236), qr, stack);
qrMul(s(56), s(52), s(56), qr, stack);
qrMul(s(235), s(56), s(235), qr, stack);
qrMul(s(61), s(56), s(61), qr, stack);
qrMul(s(234), s(61), s(234), qr, stack);
qrMul(s(65), s(61), s(65), qr, stack);
qrMul(s(233), s(65), s(233), qr, stack);
qrMul(s(70), s(65), s(70), qr, stack);
qrMul(s(232), s(70), s(232), qr, stack);
qrMul(s(74), s(70), s(74), qr, stack);
qrMul(s(231), s(74), s(231), qr, stack);
qrMul(s(79), s(74), s(79), qr, stack);
qrMul(s(230), s(79), s(230), qr, stack);
qrMul(s(83), s(79), s(83), qr, stack);
qrMul(s(229), s(83), s(229), qr, stack);
qrMul(s(88), s(83), s(88), qr, stack);
qrMul(s(228), s(88), s(228), qr, stack);
qrMul(s(92), s(88), s(92), qr, stack);
qrMul(s(227), s(92), s(227), qr, stack);
qrMul(s(97), s(92), s(97), qr, stack);
qrMul(s(226), s(97), s(226), qr, stack);
qrMul(s(101), s(97), s(101), qr, stack);
qrMul(s(225), s(101), s(225), qr, stack);
qrMul(s(106), s(101), s(106), qr, stack);
qrMul(s(224), s(106), s(224), qr, stack);
qrMul(s(110), s(106), s(110), qr, stack);
qrMul(s(223), s(110), s(223), qr, stack);
qrMul(s(115), s(110), s(115), qr, stack);
qrMul(s(222), s(115), s(222), qr, stack);
qrMul(s(119), s(115), s(119), qr, stack);
qrMul(s(221), s(119), s(221), qr, stack);
qrMul(s(124), s(119), s(124), qr, stack);
qrMul(s(220), s(124), s(220), qr, stack);
qrMul(s(128), s(124), s(128), qr, stack);
qrMul(s(219), s(128), s(219), qr, stack);
qrMul(s(133), s(128), s(133), qr, stack);
qrMul(s(218), s(133), s(218), qr, stack);
qrMul(s(137), s(133), s(137), qr, stack);
qrMul(s(217), s(137), s(217), qr, stack);
qrMul(s(142), s(137), s(142), qr, stack);
qrMul(s(216), s(142), s(216), qr, stack);
qrMul(s(163), s(142), s(163), qr, stack);
qrMul(s(215), s(163), s(215), qr, stack);
qrMul(s(164), s(163), s(164), qr, stack);
qrMul(s(214), s(164), s(214), qr, stack);
qrMul(s(165), s(164), s(165), qr, stack);
qrMul(s(213), s(165), s(213), qr, stack);
qrMul(s(166), s(165), s(166), qr, stack);
qrMul(s(212), s(166), s(212), qr, stack);
qrMul(s(167), s(166), s(167), qr, stack);
qrMul(s(211), s(167), s(211), qr, stack);
qrMul(s(168), s(167), s(168), qr, stack);
qrMul(s(210), s(168), s(210), qr, stack);
qrMul(s(169), s(168), s(169), qr, stack);
qrMul(s(209), s(169), s(209), qr, stack);
qrMul(s(170), s(169), s(170), qr, stack);
qrMul(s(208), s(170), s(208), qr, stack);
qrMul(s(171), s(170), s(171), qr, stack);
qrMul(s(207), s(171), s(207), qr, stack);
qrMul(s(172), s(171), s(172), qr, stack);
qrMul(s(206), s(172), s(206), qr, stack);
qrMul(s(173), s(172), s(173), qr, stack);
qrMul(s(205), s(173), s(205), qr, stack);
qrMul(s(174), s(173), s(174), qr, stack);
qrMul(s(204), s(174), s(204), qr, stack);
qrMul(s(175), s(174), s(175), qr, stack);
qrMul(s(203), s(175), s(203), qr, stack);
qrMul(s(176), s(175), s(176), qr, stack);
qrMul(s(202), s(176), s(202), qr, stack);
qrMul(s(177), s(176), s(177), qr, stack);
qrMul(s(201), s(177), s(201), qr, stack);
qrMul(s(178), s(177), s(178), qr, stack);
qrMul(s(200), s(178), s(200), qr, stack);
qrMul(s(179), s(178), s(179), qr, stack);
qrMul(s(199), s(179), s(199), qr, stack);
qrMul(s(180), s(179), s(180), qr, stack);
qrMul(s(198), s(180), s(198), qr, stack);
qrMul(s(181), s(180), s(181), qr, stack);
qrMul(s(197), s(181), s(197), qr, stack);
qrMul(s(182), s(181), s(182), qr, stack);
qrMul(s(196), s(182), s(196), qr, stack);
qrMul(s(183), s(182), s(183), qr, stack);
qrMul(s(195), s(183), s(195), qr, stack);
qrMul(s(184), s(183), s(184), qr, stack);
qrMul(s(72), s(184), s(72), qr, stack);
qrMul(s(185), s(184), s(185), qr, stack);
qrMul(s(0), s(185), s(0), qr, stack);
qrMul(s(186), s(185), s(186), qr, stack);
qrMul(s(145), s(186), s(145), qr, stack);
qrMul(s(187), s(186), s(187), qr, stack);
qrMul(s(148), s(187), s(148), qr, stack);
qrMul(s(188), s(187), s(188), qr, stack);
qrMul(s(144), s(188), s(144), qr, stack);
qrMul(s(189), s(188), s(189), qr, stack);
qrMul(s(193), s(189), s(193), qr, stack);
qrMul(s(190), s(189), s(190), qr, stack);
qrMul(s(192), s(190), s(192), qr, stack);
qrMul(s(191), s(190), s(191), qr, stack);
qrSqr(s(190), s(191), qr, stack);
qrMul(s(191), s(147), s(191), qr, stack);
qrSub(nP(62,0), x, s(191), qr); //  | [127]Px
qrMul(nP(62,1), s(194), s(190), qr, stack); //  | [127]Py
qrSqr(s(147), s(192), qr, stack);
qrMul(s(192), s(143), s(192), qr, stack);
qrSub(nP(61,0), x, s(192), qr); //  | [125]Px
qrMul(nP(61,1), s(140), s(147), qr, stack); //  | [125]Py
qrSqr(s(143), s(193), qr, stack);
qrMul(s(193), s(68), s(193), qr, stack);
qrSub(nP(60,0), x, s(193), qr); //  | [123]Px
qrMul(nP(60,1), s(139), s(143), qr, stack); //  | [123]Py
qrSqr(s(68), s(144), qr, stack);
qrMul(s(144), s(138), s(144), qr, stack);
qrSub(nP(59,0), x, s(144), qr); //  | [121]Px
qrMul(nP(59,1), s(136), s(68), qr, stack); //  | [121]Py
qrSqr(s(138), s(148), qr, stack);
qrMul(s(148), s(67), s(148), qr, stack);
qrSub(nP(58,0), x, s(148), qr); //  | [119]Px
qrMul(nP(58,1), s(135), s(138), qr, stack); //  | [119]Py
qrSqr(s(67), s(145), qr, stack);
qrMul(s(145), s(134), s(145), qr, stack);
qrSub(nP(57,0), x, s(145), qr); //  | [117]Px
qrMul(nP(57,1), s(131), s(67), qr, stack); //  | [117]Py
qrSqr(s(134), s(0), qr, stack);
qrMul(s(0), s(64), s(0), qr, stack);
qrSub(nP(56,0), x, s(0), qr); //  | [115]Px
qrMul(nP(56,1), s(130), s(134), qr, stack); //  | [115]Py
qrSqr(s(64), s(72), qr, stack);
qrMul(s(72), s(129), s(72), qr, stack);
qrSub(nP(55,0), x, s(72), qr); //  | [113]Px
qrMul(nP(55,1), s(127), s(64), qr, stack); //  | [113]Py
qrSqr(s(129), s(195), qr, stack);
qrMul(s(195), s(63), s(195), qr, stack);
qrSub(nP(54,0), x, s(195), qr); //  | [111]Px
qrMul(nP(54,1), s(126), s(129), qr, stack); //  | [111]Py
qrSqr(s(63), s(196), qr, stack);
qrMul(s(196), s(125), s(196), qr, stack);
qrSub(nP(53,0), x, s(196), qr); //  | [109]Px
qrMul(nP(53,1), s(122), s(63), qr, stack); //  | [109]Py
qrSqr(s(125), s(197), qr, stack);
qrMul(s(197), s(59), s(197), qr, stack);
qrSub(nP(52,0), x, s(197), qr); //  | [107]Px
qrMul(nP(52,1), s(121), s(125), qr, stack); //  | [107]Py
qrSqr(s(59), s(198), qr, stack);
qrMul(s(198), s(120), s(198), qr, stack);
qrSub(nP(51,0), x, s(198), qr); //  | [105]Px
qrMul(nP(51,1), s(118), s(59), qr, stack); //  | [105]Py
qrSqr(s(120), s(199), qr, stack);
qrMul(s(199), s(58), s(199), qr, stack);
qrSub(nP(50,0), x, s(199), qr); //  | [103]Px
qrMul(nP(50,1), s(117), s(120), qr, stack); //  | [103]Py
qrSqr(s(58), s(200), qr, stack);
qrMul(s(200), s(116), s(200), qr, stack);
qrSub(nP(49,0), x, s(200), qr); //  | [101]Px
qrMul(nP(49,1), s(113), s(58), qr, stack); //  | [101]Py
qrSqr(s(116), s(201), qr, stack);
qrMul(s(201), s(55), s(201), qr, stack);
qrSub(nP(48,0), x, s(201), qr); //  | [99]Px
qrMul(nP(48,1), s(112), s(116), qr, stack); //  | [99]Py
qrSqr(s(55), s(202), qr, stack);
qrMul(s(202), s(111), s(202), qr, stack);
qrSub(nP(47,0), x, s(202), qr); //  | [97]Px
qrMul(nP(47,1), s(109), s(55), qr, stack); //  | [97]Py
qrSqr(s(111), s(203), qr, stack);
qrMul(s(203), s(54), s(203), qr, stack);
qrSub(nP(46,0), x, s(203), qr); //  | [95]Px
qrMul(nP(46,1), s(108), s(111), qr, stack); //  | [95]Py
qrSqr(s(54), s(204), qr, stack);
qrMul(s(204), s(107), s(204), qr, stack);
qrSub(nP(45,0), x, s(204), qr); //  | [93]Px
qrMul(nP(45,1), s(104), s(54), qr, stack); //  | [93]Py
qrSqr(s(107), s(205), qr, stack);
qrMul(s(205), s(50), s(205), qr, stack);
qrSub(nP(44,0), x, s(205), qr); //  | [91]Px
qrMul(nP(44,1), s(103), s(107), qr, stack); //  | [91]Py
qrSqr(s(50), s(206), qr, stack);
qrMul(s(206), s(102), s(206), qr, stack);
qrSub(nP(43,0), x, s(206), qr); //  | [89]Px
qrMul(nP(43,1), s(100), s(50), qr, stack); //  | [89]Py
qrSqr(s(102), s(207), qr, stack);
qrMul(s(207), s(49), s(207), qr, stack);
qrSub(nP(42,0), x, s(207), qr); //  | [87]Px
qrMul(nP(42,1), s(99), s(102), qr, stack); //  | [87]Py
qrSqr(s(49), s(208), qr, stack);
qrMul(s(208), s(98), s(208), qr, stack);
qrSub(nP(41,0), x, s(208), qr); //  | [85]Px
qrMul(nP(41,1), s(95), s(49), qr, stack); //  | [85]Py
qrSqr(s(98), s(209), qr, stack);
qrMul(s(209), s(46), s(209), qr, stack);
qrSub(nP(40,0), x, s(209), qr); //  | [83]Px
qrMul(nP(40,1), s(94), s(98), qr, stack); //  | [83]Py
qrSqr(s(46), s(210), qr, stack);
qrMul(s(210), s(93), s(210), qr, stack);
qrSub(nP(39,0), x, s(210), qr); //  | [81]Px
qrMul(nP(39,1), s(91), s(46), qr, stack); //  | [81]Py
qrSqr(s(93), s(211), qr, stack);
qrMul(s(211), s(45), s(211), qr, stack);
qrSub(nP(38,0), x, s(211), qr); //  | [79]Px
qrMul(nP(38,1), s(90), s(93), qr, stack); //  | [79]Py
qrSqr(s(45), s(212), qr, stack);
qrMul(s(212), s(89), s(212), qr, stack);
qrSub(nP(37,0), x, s(212), qr); //  | [77]Px
qrMul(nP(37,1), s(86), s(45), qr, stack); //  | [77]Py
qrSqr(s(89), s(213), qr, stack);
qrMul(s(213), s(41), s(213), qr, stack);
qrSub(nP(36,0), x, s(213), qr); //  | [75]Px
qrMul(nP(36,1), s(85), s(89), qr, stack); //  | [75]Py
qrSqr(s(41), s(214), qr, stack);
qrMul(s(214), s(84), s(214), qr, stack);
qrSub(nP(35,0), x, s(214), qr); //  | [73]Px
qrMul(nP(35,1), s(82), s(41), qr, stack); //  | [73]Py
qrSqr(s(84), s(215), qr, stack);
qrMul(s(215), s(40), s(215), qr, stack);
qrSub(nP(34,0), x, s(215), qr); //  | [71]Px
qrMul(nP(34,1), s(81), s(84), qr, stack); //  | [71]Py
qrSqr(s(40), s(216), qr, stack);
qrMul(s(216), s(80), s(216), qr, stack);
qrSub(nP(33,0), x, s(216), qr); //  | [69]Px
qrMul(nP(33,1), s(77), s(40), qr, stack); //  | [69]Py
qrSqr(s(80), s(217), qr, stack);
qrMul(s(217), s(37), s(217), qr, stack);
qrSub(nP(32,0), x, s(217), qr); //  | [67]Px
qrMul(nP(32,1), s(76), s(80), qr, stack); //  | [67]Py
qrSqr(s(37), s(218), qr, stack);
qrMul(s(218), s(75), s(218), qr, stack);
qrSub(nP(31,0), x, s(218), qr); //  | [65]Px
qrMul(nP(31,1), s(73), s(37), qr, stack); //  | [65]Py
qrSqr(s(75), s(219), qr, stack);
qrMul(s(219), s(141), s(219), qr, stack);
qrSub(nP(30,0), x, s(219), qr); //  | [63]Px
qrMul(nP(30,1), s(162), s(75), qr, stack); //  | [63]Py
qrSqr(s(141), s(220), qr, stack);
qrMul(s(220), s(71), s(220), qr, stack);
qrSub(nP(29,0), x, s(220), qr); //  | [61]Px
qrMul(nP(29,1), s(161), s(141), qr, stack); //  | [61]Py
qrSqr(s(71), s(221), qr, stack);
qrMul(s(221), s(132), s(221), qr, stack);
qrSub(nP(28,0), x, s(221), qr); //  | [59]Px
qrMul(nP(28,1), s(160), s(71), qr, stack); //  | [59]Py
qrSqr(s(132), s(222), qr, stack);
qrMul(s(222), s(66), s(222), qr, stack);
qrSub(nP(27,0), x, s(222), qr); //  | [57]Px
qrMul(nP(27,1), s(159), s(132), qr, stack); //  | [57]Py
qrSqr(s(66), s(223), qr, stack);
qrMul(s(223), s(123), s(223), qr, stack);
qrSub(nP(26,0), x, s(223), qr); //  | [55]Px
qrMul(nP(26,1), s(158), s(66), qr, stack); //  | [55]Py
qrSqr(s(123), s(224), qr, stack);
qrMul(s(224), s(62), s(224), qr, stack);
qrSub(nP(25,0), x, s(224), qr); //  | [53]Px
qrMul(nP(25,1), s(157), s(123), qr, stack); //  | [53]Py
qrSqr(s(62), s(225), qr, stack);
qrMul(s(225), s(114), s(225), qr, stack);
qrSub(nP(24,0), x, s(225), qr); //  | [51]Px
qrMul(nP(24,1), s(156), s(62), qr, stack); //  | [51]Py
qrSqr(s(114), s(226), qr, stack);
qrMul(s(226), s(57), s(226), qr, stack);
qrSub(nP(23,0), x, s(226), qr); //  | [49]Px
qrMul(nP(23,1), s(155), s(114), qr, stack); //  | [49]Py
qrSqr(s(57), s(227), qr, stack);
qrMul(s(227), s(105), s(227), qr, stack);
qrSub(nP(22,0), x, s(227), qr); //  | [47]Px
qrMul(nP(22,1), s(154), s(57), qr, stack); //  | [47]Py
qrSqr(s(105), s(228), qr, stack);
qrMul(s(228), s(53), s(228), qr, stack);
qrSub(nP(21,0), x, s(228), qr); //  | [45]Px
qrMul(nP(21,1), s(153), s(105), qr, stack); //  | [45]Py
qrSqr(s(53), s(229), qr, stack);
qrMul(s(229), s(96), s(229), qr, stack);
qrSub(nP(20,0), x, s(229), qr); //  | [43]Px
qrMul(nP(20,1), s(152), s(53), qr, stack); //  | [43]Py
qrSqr(s(96), s(230), qr, stack);
qrMul(s(230), s(48), s(230), qr, stack);
qrSub(nP(19,0), x, s(230), qr); //  | [41]Px
qrMul(nP(19,1), s(151), s(96), qr, stack); //  | [41]Py
qrSqr(s(48), s(231), qr, stack);
qrMul(s(231), s(87), s(231), qr, stack);
qrSub(nP(18,0), x, s(231), qr); //  | [39]Px
qrMul(nP(18,1), s(150), s(48), qr, stack); //  | [39]Py
qrSqr(s(87), s(232), qr, stack);
qrMul(s(232), s(44), s(232), qr, stack);
qrSub(nP(17,0), x, s(232), qr); //  | [37]Px
qrMul(nP(17,1), s(149), s(87), qr, stack); //  | [37]Py
qrSqr(s(44), s(233), qr, stack);
qrMul(s(233), s(78), s(233), qr, stack);
qrSub(nP(16,0), x, s(233), qr); //  | [35]Px
qrMul(nP(16,1), s(1), s(44), qr, stack); //  | [35]Py
qrSqr(s(78), s(234), qr, stack);
qrMul(s(234), s(39), s(234), qr, stack);
qrSub(nP(15,0), x, s(234), qr); //  | [33]Px
qrMul(nP(15,1), s(146), s(78), qr, stack); //  | [33]Py
qrSqr(s(39), s(235), qr, stack);
qrMul(s(235), s(69), s(235), qr, stack);
qrSub(nP(14,0), x, s(235), qr); //  | [31]Px
qrMul(nP(14,1), s(36), s(39), qr, stack); //  | [31]Py
qrSqr(s(69), s(236), qr, stack);
qrMul(s(236), s(35), s(236), qr, stack);
qrSub(nP(13,0), x, s(236), qr); //  | [29]Px
qrMul(nP(13,1), s(32), s(69), qr, stack); //  | [29]Py
qrSqr(s(35), s(237), qr, stack);
qrMul(s(237), s(60), s(237), qr, stack);
qrSub(nP(12,0), x, s(237), qr); //  | [27]Px
qrMul(nP(12,1), s(31), s(35), qr, stack); //  | [27]Py
qrSqr(s(60), s(238), qr, stack);
qrMul(s(238), s(30), s(238), qr, stack);
qrSub(nP(11,0), x, s(238), qr); //  | [25]Px
qrMul(nP(11,1), s(28), s(60), qr, stack); //  | [25]Py
qrSqr(s(30), s(239), qr, stack);
qrMul(s(239), s(51), s(239), qr, stack);
qrSub(nP(10,0), x, s(239), qr); //  | [23]Px
qrMul(nP(10,1), s(27), s(30), qr, stack); //  | [23]Py
qrSqr(s(51), s(240), qr, stack);
qrMul(s(240), s(26), s(240), qr, stack);
qrSub(nP(9,0), x, s(240), qr); //  | [21]Px
qrMul(nP(9,1), s(23), s(51), qr, stack); //  | [21]Py
qrSqr(s(26), s(241), qr, stack);
qrMul(s(241), s(42), s(241), qr, stack);
qrSub(nP(8,0), x, s(241), qr); //  | [19]Px
qrMul(nP(8,1), s(22), s(26), qr, stack); //  | [19]Py
qrSqr(s(42), s(242), qr, stack);
qrMul(s(242), s(21), s(242), qr, stack);
qrSub(nP(7,0), x, s(242), qr); //  | [17]Px
qrMul(nP(7,1), s(19), s(42), qr, stack); //  | [17]Py
qrSqr(s(21), s(243), qr, stack);
qrMul(s(243), s(33), s(243), qr, stack);
qrSub(nP(6,0), x, s(243), qr); //  | [15]Px
qrMul(nP(6,1), s(18), s(21), qr, stack); //  | [15]Py
qrSqr(s(33), s(244), qr, stack);
qrMul(s(244), s(17), s(244), qr, stack);
qrSub(nP(5,0), x, s(244), qr); //  | [13]Px
qrMul(nP(5,1), s(14), s(33), qr, stack); //  | [13]Py
qrSqr(s(17), s(245), qr, stack);
qrMul(s(245), s(24), s(245), qr, stack);
qrSub(nP(4,0), x, s(245), qr); //  | [11]Px
qrMul(nP(4,1), s(13), s(17), qr, stack); //  | [11]Py
qrSqr(s(24), s(246), qr, stack);
qrMul(s(246), s(12), s(246), qr, stack);
qrSub(nP(3,0), x, s(246), qr); //  | [9]Px
qrMul(nP(3,1), s(9), s(24), qr, stack); //  | [9]Py
qrSqr(s(12), s(247), qr, stack);
qrMul(s(247), s(15), s(247), qr, stack);
qrSub(nP(2,0), x, s(247), qr); //  | [7]Px
qrMul(nP(2,1), s(5), s(12), qr, stack); //  | [7]Py
qrSqr(s(15), s(248), qr, stack);
qrMul(s(248), s(10), s(248), qr, stack);
qrSub(nP(1,0), x, s(248), qr); //  | [5]Px
qrMul(nP(1,1), s(3), s(15), qr, stack); //  | [5]Py
qrSqr(s(10), s(249), qr, stack);
qrMul(s(249), s(2), s(249), qr, stack);
qrSub(nP(0,0), x, s(249), qr); //  | [3]Px
qrMul(nP(0,1), s(6), s(10), qr, stack); //  | [3]Py

#undef s
#undef nP
}
size_t ecDivpA_64_deep(size_t n, size_t ec_deep)
{
  return 251*n + ec_deep;
}


