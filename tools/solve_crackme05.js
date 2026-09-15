// Finds a preimage for crackme05's custom hash (target 0xDEADBABE) via
// meet-in-the-middle: the hash is a bijection per character (rotate, XOR,
// multiply by an odd constant mod 2^32 are all invertible), so any 6-char
// input built from a 3-char forward run and a 3-char backward-inverted run
// that land on the same intermediate state is a valid solution.
// Run: node solve_crackme05.js

const MASK = 0xFFFFFFFFn;
const M = 0x01000193n;

function modinv(a, m) {
  let [old_r, r] = [a, m];
  let [old_s, s] = [1n, 0n];
  while (r !== 0n) {
    const q = old_r / r;
    [old_r, r] = [r, old_r - q * r];
    [old_s, s] = [s, old_s - q * s];
  }
  return ((old_s % m) + m) % m;
}
const Minv = modinv(M, 1n << 32n);

function rol5(h) { h &= MASK; return ((h << 5n) | (h >> 27n)) & MASK; }
function ror5(h) { h &= MASK; return ((h >> 5n) | (h << 27n)) & MASK; }
function stepFwd(h, c) { h = (rol5(h) ^ BigInt(c)) & MASK; return (h * M) & MASK; }
function stepBack(h, c) { const afterXor = (h * Minv) & MASK; return ror5(afterXor ^ BigInt(c)); }

const H0 = 0x1337BEEFn;
const TARGET = 0xDEADBABEn;
const K = 3;
const chars = [];
for (let c = 0x21; c <= 0x7E; c++) chars.push(c); // printable ASCII, no space

const fwdMap = new Map();
(function buildFwd(depth, h, seq) {
  if (depth === K) { fwdMap.set(h.toString(), seq.slice()); return; }
  for (const c of chars) { seq.push(c); buildFwd(depth + 1, stepFwd(h, c), seq); seq.pop(); }
})(0, H0, []);

let found = null;
(function searchBack(depth, h, seq) {
  if (found || depth === K) {
    if (!found && fwdMap.has(h.toString())) found = { prefix: fwdMap.get(h.toString()), suffix: seq.slice().reverse() };
    return;
  }
  for (const c of chars) {
    seq.push(c); searchBack(depth + 1, stepBack(h, c), seq); seq.pop();
    if (found) return;
  }
})(0, TARGET, []);

if (found) {
  const full = [...found.prefix, ...found.suffix];
  const str = full.map(c => String.fromCharCode(c)).join("");
  console.log("input:", JSON.stringify(str));
  let h = H0;
  for (const c of full) h = stepFwd(h, c);
  console.log("hash:", h.toString(16), "(target " + TARGET.toString(16) + ")");
} else {
  console.log("no match at K=" + K + ", try a larger K");
}
