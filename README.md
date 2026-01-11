# Hack Instruction Set Architecture (ISA)

This project is an implementation of an **Assembler** for the Hack computer, as specified in the *Nand2Tetris* (Elements of Computing Systems) course. The Hack machine language consists of 16-bit instructions, categorized into two types: **A-instructions** and **C-instructions**.

---

## 1. A-Instructions (Address Instructions)
The A-instruction is used to load a 15-bit value into the A-register. An instruction is identified as an A-instruction if its most significant bit (MSB) is `0`.

### Bit-field Encoding
| 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| **0** | v | v | v | v | v | v | v | v | v | v | v | v | v | v | v |

* **0**: The opcode identifying an A-instruction.
* **v (Bits 14-0)**: A 15-bit non-negative binary integer.
* **Range**: 0 through 32,767 (decimal).

**Execution Behavior:**
When executed, the 15-bit value is loaded into the **A-register**. As a side effect, the RAM register at the address represented by that value is enabled for read/write actions in the subsequent clock cycle (`M` becomes `RAM[A]`).

---

## 2. C-Instructions (Computation Instructions)
The C-instruction is the "workhorse" of the Hack CPU. It defines the computation to be performed, where to store the result, and whether to jump to a new instruction. Its MSB is always `1`.

### Symbolic Format
`dest = comp ; jump`

* **dest**: Where the result is stored (optional).
* **comp**: The ALU operation to perform (mandatory).
* **jump**: The condition for the next instruction fetch (optional).

### Bit-field Encoding
| 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| **1** | **1** | **1** | **a** | **c1** | **c2** | **c3** | **c4** | **c5** | **c6** | **d1** | **d2** | **d3** | **j1** | **j2** | **j3** |

* **1 (Bit 15)**: Opcode for C-instruction.
* **11 (Bits 14-13)**: Unused; by convention set to `1`.
* **a (Bit 12)**: Determines if the ALU uses the A-register (`0`) or Memory (`1`).
* **c1-c6**: Control bits for the ALU operation.
* **d1-d3**: Destination bits for the result.
* **j1-j3**: Jump/Branching condition bits.

---

## 3. Reference Tables

### Symbols
* **A**: Current value in the A-register.
* **D**: Current value in the D-register.
* **M**: Value of the RAM register at address A (`RAM[A]`).

### Computation (ALU) Table
| c1 | c2 | c3 | c4 | c5 | c6 | `a=0` | `a=1` |
|:--:|:--:|:--:|:--:|:--:|:--:|:---:|:---:|
| 1  | 0  | 1  | 0  | 1  | 0  | 0   | -   |
| 1  | 1  | 1  | 1  | 1  | 1  | 1   | -   |
| 1  | 1  | 0  | 0  | 0  | 0  | A   | M   |
| 0  | 0  | 1  | 1  | 0  | 0  | D+A | D+M |

### Destination Table
| d1 (A) | d2 (D) | d3 (M) | Destination |
|:---:|:---:|:---:|:---|
| 0 | 0 | 0 | null (result not stored) |
| 0 | 0 | 1 | M |
| 0 | 1 | 0 | D |
| 1 | 0 | 0 | A |
| 1 | 1 | 1 | A, D, and M |

### Jump Table
| j1 (<0) | j2 (=0) | j3 (>0) | Mnemonic | Condition |
|:---:|:---:|:---:|:---|:---|
| 0 | 0 | 0 | null | No jump |
| 0 | 0 | 1 | JGT | If out > 0 |
| 0 | 1 | 0 | JEQ | If out = 0 |
| 1 | 1 | 1 | JMP | Unconditional |

---

### How to run on Linux
```bash
g++ -std=c++17 main.cpp Code.cpp Parser.cpp SymbolTable.cpp -o hack-assembler
./hack-assembler Pong.asm
```


### How to run on Windows
```bash
g++ -std=c++17 main.cpp Code.cpp Parser.cpp SymbolTable.cpp -o hack-assembler.exe
.\hack-assembler.exe Pong.asm
```
