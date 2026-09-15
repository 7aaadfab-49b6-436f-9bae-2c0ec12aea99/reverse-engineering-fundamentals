// Decodes crackme04's XOR-0xAA blob and checks the source comment's claim
// that it decodes to "check_flag_pwned". It doesn't. Also flags that the
// decode loop includes the trailing 0x00 byte, so the "decoded" buffer has
// no null terminator within its declared bounds - strcmp() against it reads
// past the array into whatever memory follows, which is not determinable
// from source alone.
// Run: node solve_crackme04.js

const encrypted = [0xE9, 0xCE, 0xC9, 0xC9, 0xE1, 0xC4, 0xC6, 0xCE, 0xC7, 0xE1, 0xDA, 0xD1, 0xC4, 0xC9, 0xC9, 0xC8, 0x00];
const decoded = encrypted.map(b => b ^ 0xAA);

console.log("decoded bytes (hex):", decoded.map(b => b.toString(16).padStart(2, "0")).join(" "));
console.log("decoded (chars, non-printable as .NN):",
 decoded.map(b => (b >= 0x20 && b <= 0x7e) ? String.fromCharCode(b) : `.${b.toString(16)}`).join(""));

const claimed = "check_flag_pwned";
const claimedBytes = [...claimed].map(c => c.charCodeAt(0) ^ 0xAA);
const matchesComment = JSON.stringify(claimedBytes.concat([0 ^ 0xAA])) === JSON.stringify(encrypted.slice(0, claimedBytes.length + 1));
console.log('comment claims decode == "check_flag_pwned":', matchesComment, "(false = comment is a decoy)");

console.log("last decoded byte is 0x00:", decoded[decoded.length - 1] === 0,
 "(false = no null terminator in bounds -> strcmp reads out of array bounds at runtime, layout-dependent, not resolvable statically)");
