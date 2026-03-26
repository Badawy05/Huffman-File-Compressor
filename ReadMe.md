# Huffman BMP Compressor

A lossless file compressor and decompressor built from scratch in C++, using Huffman coding. No STL containers — every data structure is implemented manually.

Built as a data structures project to demonstrate low-level systems programming, bit manipulation, and custom memory management.

---

## Demo

| File | Size |
|------|------|
| Input BMP | 2.40 MB |
| Compressed `.ECE2103` | 0.81 MB |
| Compression ratio | ~66% |

---

## How it works

Huffman coding is a lossless compression algorithm that assigns shorter bit sequences to more frequent bytes and longer ones to rarer bytes. The result is a smaller file that can be perfectly reconstructed.

**Compression pipeline:**

```
Read BMP bytes → Count frequencies → Build min-heap → Build Huffman tree
→ Generate bit codes → Write frequency table + encoded stream to file
```

**Decompression pipeline:**

```
Read frequency table → Rebuild Huffman tree → Decode bit stream → Write original bytes
```

### 1. Frequency table
The input file is read as raw bytes. Each of the 256 possible byte values gets its occurrence count stored in a table.

### 2. Min-heap
A custom min-heap (no `std::priority_queue`) stores tree nodes sorted by frequency. The two lowest-frequency nodes are repeatedly extracted and merged.

### 3. Huffman tree
Merging continues until a single root node remains. Leaf nodes represent actual byte values; internal nodes are structural.

### 4. Code generation
A depth-first traversal of the tree generates a unique bitstring for each byte — shorter codes for frequent bytes, longer for rare ones.

### 5. Encoding
The frequency table is written to the output file first (used during decompression to rebuild the tree), followed by the encoded bitstream packed manually into bytes. Padding bits are tracked and stored.

### 6. Decoding
The frequency table is read and the Huffman tree is reconstructed. Bits are read one at a time, walking the tree until a leaf is reached — the corresponding byte is written to output. Padding is accounted for at the end.

---

## Build & run

**Compile:**
```bash
g++ -o compressor main.cpp
```

**Compress:**
```bash
./compressor -c input.bmp output
# Output: output.ECE2103
```

**Decompress:**
```bash
./compressor -d input.ECE2103 output.bmp
```

**Optional — set buffer size (default 1024 bytes):**
```bash
./compressor -b 4096 -c input.bmp output
```

---

## Project structure

```
.
├── main.cpp          # Full implementation
├── README.md
```

All logic lives in `main.cpp` across three classes:

| Class | Responsibility |
|-------|----------------|
| `Heap` | Custom min-heap, frequency table, node insertion and extraction |
| `Huffman` | Tree construction, code generation, file encoding and decoding |
| `Compressor` | Public interface — wraps compress and decompress workflows |

---

## Concepts demonstrated

- Huffman coding algorithm
- Binary tree construction and traversal
- Custom min-heap / priority queue
- Bit-level I/O (manual bit packing and unpacking)
- Padding handling for non-byte-aligned streams
- Binary file reading and writing with buffered I/O
- Manual memory management (no smart pointers, no STL containers)
- Command-line argument parsing

---

## Limitations

- Works on uncompressed BMP files only
- Compression ratio depends on image entropy — highly random images compress poorly
- Single-threaded

---

## Possible improvements

- Support for additional file formats (PNG, text, arbitrary binary)
- Huffman tree serialization using preorder traversal (more compact than storing the full frequency table)
- Multithreaded encoding for large files
- Adaptive Huffman coding (updates tree dynamically without a separate header)
