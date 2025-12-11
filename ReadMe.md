Huffman BMP File Compressor & Decompressor

A C/C++ implementation of Huffman coding designed to compress and decompress BMP image files without using the STL.
The project includes custom heap, tree, and bit-level I/O handling, making it suitable for data-structures coursework and low-level systems projects.

📌 Features

- Compresses 24-bit BMP files using Huffman coding

- Reconstructs the original BMP using the stored frequency table

- Manual bit manipulation for writing/reading the compressed stream

- Custom Min-Heap implementation (no STL containers)

- Custom Huffman Tree implementation

- Writes frequency table + encoded bitstream in a custom .huff file format

- Fully supports decoding and restoring identical pixel data

🔧 How It Works
1. Frequency Table

BMP file is read as raw bytes.

A table of size 256 is created.

Each byte’s frequency is counted.

2. Custom Min-Heap

Nodes contain {byte, frequency}.

Heap is used to repeatedly extract the two lowest-frequency nodes.

3. Huffman Tree Construction

Combine nodes until only one root remains.

Tree nodes contain:

byte value (leaf only)

frequency

left/right pointers

4. Code Generation

Depth-first traversal creates a bitstring for each byte.

Codes are stored in a lookup table of size 256.

5. Encoding

Frequency table is written to file.

Pixel bytes are encoded bit-by-bit into the output.

BitWriter packs bits manually into bytes.

6. Decoding

Rebuild frequency table from file.

Reconstruct the Huffman tree.

Use BitReader to read bits from the compressed stream.

Walk the tree until reaching a leaf → write the decoded byte.

🚀 How to Run

Compress

./compressor input.bmp output.huff             -> on cmd

Decompress

./compressor -d input.huff output.bmp             -> on cmd

🧪 Example

Input BMP: 2.40 MB
Output .huff: 0.81 MB
Compression ratio: ~66%

(Values depend on image content.)

🧩 Concepts Demonstrated

Binary Trees

Priority Queues (custom heap)

Recursion

Bit-level I/O

File parsing

Lossless compression algorithms

Pointer manipulation & manual memory handling

📌 Limitations

Works only on uncompressed BMP files

Heap and tree memory must be cleaned manually

No STL containers allowed by design

Compression efficiency depends on image entropy

📖 Future Improvements

Add support for PNG/JPG style compression containers

Add multithreading for faster encoding

Add GUI frontend

Add Huffman tree serialization using preorder format
