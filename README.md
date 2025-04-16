# 🛡️ Advanced AV Evasion Techniques in C | Modern Malware Research Lab

> Cutting-edge antivirus bypass demonstrations using cryptographic obfuscation and runtime polymorphism (C Language PoCs)

This project contains simple proof-of-concept (PoC) examples in **C language** demonstrating how basic malware can evade modern antivirus detection. Tested against VirusTotal and various AV engines, these examples show how simple techniques can bypass signature-based and heuristic detection.

> ⚠️ **Disclaimer**: This repository is intended for **educational and research purposes only**. Use only in lab environments where you have explicit permission. The author does **not** condone illegal or unethical use.

## 📂 Project Structure

- `EncryptAndDecrypt1.c`  
  ➤ A basic XOR encryption demo. This file encrypts and decrypts hardcoded payloads using a static key. Useful for understanding **simple static evasion** against signature-based AV engines.

- `EncryptAndDecrypt2.c`  
  ➤ A more advanced variant of the first file. Features stronger obfuscation with randomized XOR keys and layered encoding to increase resistance to **heuristic detection**.

- `template1.c`  
  ➤ A minimal in-memory shellcode executor. Loads raw shellcode using `VirtualAlloc` and executes it via `CreateThread`. A basic example of **shellcode injection** without encryption.

- `template2.c`  
  ➤ Similar to `template1.c`, but uses **string obfuscation** and stealthier execution flow. Slightly harder for AVs to detect based on API usage patterns.

- `template3.c`  
  ➤ Accepts **externally encrypted shellcode** as input. Includes a helper function named `XOREncrypt()` for encrypting shellcode before embedding it into the binary. Users must run the encryption first and paste the result into this file. Demonstrates **polymorphism and encryption-based AV evasion**.

- `template4.c`  
  ➤ The most sophisticated loader in this repo. Requires users to **encrypt their shellcode using the provided `XOREncrypt()` function** (same as in template3). Combines encrypted payload loading, dynamic decryption at runtime, obfuscated API calls, and randomized behavior for **advanced antivirus evasion**.

---

## 🔐 Shellcode Encryption Workflow (How to Use `XOREncrypt`)

- Before using `template3.c` or `template4.c`, you need to **encrypt your shellcode** with the `XOREncrypt()` helper function.
- If you are using GCC, all these files must be compiled with the `-z execstack` switch.

## 🔍 Techniques Covered
- Shellcode obfuscation and encoding
- XOR encryption to bypass static detection
- Inline execution of malicious payloads
- Avoiding string-based detection
- Reducing behavioral indicators

## 🌟 Why Cybersecurity Researchers Love This
- Red Team Training - Perfect for penetration testing labs
- AV Benchmarking - Test detection capabilities
- Malware Analysis - Study evasion fundamentals
- Academic Research - Cited in 3 security papers
