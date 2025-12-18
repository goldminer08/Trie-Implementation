# Hierarchical Text Classification Trie (N-ary Trie)

This project implements an **N-ary trie** for **hierarchical text classification**, where each level refines a class into more specific subclasses. The trie stores only the **class hierarchy**, and a provided **language-model classifier library** is used during classification to choose which child path to traverse at each level.

In this trie:
- Each node represents a class level in the hierarchy

---

## Features
- Load a class hierarchy from file into the trie
- Insert and erase classifications (comma-separated paths)
- Classify an input phrase by traversing the trie and calling the provided classifier at each level
- Print all stored classifications (single-line output format)
- Query size / empty state, and clear the trie

---

## Input Format (CLI Program)
This repository includes a single test program that reads commands from **stdin** and prints results to **stdout**.

### Commands

#### `LOAD filename`
Loads classifications from a file where each line is:
`<base class>,<subclass>[,<subsubclass>...]`

- If a classification already exists, that line is ignored.
- **Output:** `success` (always)

#### `INSERT classification`
Inserts a new classification path (comma-separated).

- **Runtime:** `O(n)` where `n` is the number of classes in the classification

#### `CLASSIFY input`
Classifies an input phrase using the classifier + trie traversal.  
At each level, the classifier is called to pick the best refinement among children.

- **Runtime:** `O(n)` where `n` is the number of classes in the trie.

#### `ERASE classification`
Erases an entire classification path from the trie (comma-separated).

- **Runtime:** `O(n)` where `n` is the number of classes in the classification

#### `PRINT`
Prints all classifications in the trie on **one line**, comma-separated inside each classification, and separated from each other by underscores.

#### `EMPTY`
Checks if the trie is empty.

#### `CLEAR`
Deletes all classifications.


#### `SIZE`
Prints the number of stored classifications.

#### `EXIT`
Terminates the program.  

---

## Illegal Argument Handling (Uppercase Letters)
For `INSERT`, `CLASSIFY`, and `ERASE`, if the input contains **any uppercase English letters**, the program must:
1. Throw a custom exception:
   - `illegal exception`
2. Catch it and print:
   - `illegal argument`
3. Continue processing remaining input normally (read the full line even if invalid)

---
