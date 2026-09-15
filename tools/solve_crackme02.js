// Recovers the XOR-encoded password for crackme02 (repeating 3-byte key).
// Run: node solve_crackme02.js

const cipher = [0x15, 0x33, 0x03, 0x23, 0x15, 0x34, 0x17, 0x25, 0x1C, 0x26, 0x0F, 0x26, 0x0F, 0x37, 0x14];
const key = "KEY";

const bytes = cipher.map((c, i) => c ^ key.charCodeAt(i % 3));
console.log("password bytes (hex):", bytes.map(b => b.toString(16).padStart(2, "0")).join(" "));
console.log("password (chars, non-printable shown as \\xNN):",
  bytes.map(b => (b >= 0x20 && b <= 0x7e) ? String.fromCharCode(b) : `\\x${b.toString(16)}`).join(""));

// verify round-trip
const ok = bytes.every((b, i) => (b ^ key.charCodeAt(i % 3)) === cipher[i]);
console.log("verified:", ok);
