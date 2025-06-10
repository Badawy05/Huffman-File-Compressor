#include<stdio.h>
#include<fstream>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<iostream>
#include<bitset>
using namespace std;
struct UniNode
{
	char ch;
	int freq;
	UniNode* right;
	UniNode* left;
	UniNode(char ch, int freq)
	{
		this->ch = ch;
		this->freq = freq;
		left = NULL;
		right = NULL;
	}
	UniNode()
	{

		left = NULL;
		right = NULL;
	}
};
class Heap {
private:
	int indexfreq = 0;
	int* freq;
	int index = 0;
	UniNode** heap;
	int size;

	bool hasParent(int i) {
		return i > 0;
	}

	bool hasRightChild(int index)
	{

		return 2 * index + 2 < size;

	}
	bool hasLeftChild(int index)
	{

		return 2 * index + 1 < size;
	}
	int getParent(int index)
	{
		return (index - 1) / 2;
	}
	int getLeftChild(int index)
	{
		return 2 * index + 1;
	}
	int getRightChild(int index)
	{
		return 2 * index + 2;
	}
	void swap(int i, int j)
	{
		UniNode* tmp = heap[i];
		heap[i] = heap[j];
		heap[j] = tmp;

	}
	void heapifyUp(int i)
	{
		if (i == 0) return;
		if (heap[getParent(i)]->freq > heap[i]->freq)
		{
			swap(getParent(i), i);
			heapifyUp(getParent(i));
		}
		return;
	}
	void heapifydown(int i)
	{
		if (size == 0) return;
		int min = i;
		int left = getLeftChild(i);
		int right = getRightChild(i);
		// we have to figure out which one is min
		if (hasLeftChild(i) && heap[min]->freq > heap[left]->freq)
		{
			min = left;
		}
		if (hasRightChild(i) && heap[min]->freq > heap[right]->freq)
		{
			min = right;
		}
		if (min != i)
		{
			swap(i, min);
			heapifydown(min);
		}
	}
public:
	Heap()
	{
		freq = new int[256];
		for (int i = 0;i < 256;i++)
			freq[i] = 0;
		heap = new UniNode * [256];
		size = 0;
	}
	~Heap()
	{
		for (int i = 0;i < size;i++)
			delete heap[i];
		delete[] heap;
		delete[] freq;
	}
	int turnCharToInt(unsigned char* bytes) {
		int value = 0;
		// chatgpt helped me in this
		value += (unsigned char)bytes[0];
		value += (unsigned char)bytes[1] * 256;
		value += (unsigned char)bytes[2] * 256 * 256;
		value += (unsigned char)bytes[3] * 256 * 256 * 256;

		return value;
	}
	void insertFreqTableChar(unsigned char ch)
	{
		freq[ch]++;
	}
	void buildHeap()
	{

		for (int i = 0;i < 256;i++) {
			heap[size] = new UniNode(i, freq[i]);
			heapifyUp(size);
			size++;
		}

	}
	UniNode* extract()
	{

		if (size == 0) return NULL;
		UniNode* tmp = heap[0];
		heap[0] = heap[size - 1];
		heap[size - 1] = NULL;
		size--;
		heapifydown(0);
		return tmp;

	}
	void insertNodeToHeap(UniNode* node)
	{

		heap[size] = node;
		heapifyUp(size);
		size++;

	}
	void insertReadyFreqTable(unsigned char c, int freq)
	{
		heap[size] = new UniNode(c, freq);
		heapifyUp(size);
		size++;
		
	}
	int heapSize()
	{
		return size;
	}
	int getFreqOfTable(int indx)
	{
		return freq[indx];
	}

};
class Huffman
{
	struct Node
	{
		unsigned char* arr = new unsigned char[256];
		~Node() { delete[] arr; }


	};
private:
	Heap *h = new Heap();;
	UniNode* root;
	Node** huffCode;
	streamsize sizeBef = 0;
	int bufferSize;
	int bitCount;
	int size = 0;
	int nCompress = 0;
	// used for timing 
	// -------------------------------------------------
	// Used for compression
	int getRandName()
	{
		return rand() % 256;
	}
	unsigned char* getCode(unsigned char c)
	{
		return huffCode[c]->arr;
	}
	void getHuffCode(UniNode* recNode, unsigned char* arr, int indx)
	{
		if (recNode == NULL)
			return;

		if (recNode->left == NULL && recNode->right == NULL) {
			arr[indx] = '\0';
			// static cast cuz it is the best safe way to convert by (chat gpt ) 
			unsigned char index = static_cast<unsigned char>(recNode->ch);
			huffCode[index] = new Node();
			strcpy_s(reinterpret_cast<char*>(huffCode[index]->arr), 256, reinterpret_cast<const char*>(arr));
			//----------------------------------------------------------------------------

			return;
		}
		arr[indx] = '0';
		getHuffCode(recNode->left, arr, indx + 1);
		arr[indx] = '1';
		getHuffCode(recNode->right, arr, indx + 1);
	}
	void turnCharToCode(unsigned char c, unsigned char* Bits)
	{
		int i = 0;
		while (huffCode[c]->arr[i] != '\0')
		{
			Bits[bitCount] = huffCode[c]->arr[i];
			i++;
			bitCount++;
		}
	}
	bool is8() {

		return bitCount >= 8;
	}
	unsigned char bitsToBytesConverter(unsigned char bits[8]) {
		//this function is by chat gpt 
		unsigned char byte = 0;
		for (int i = 0; i < 8; ++i) {
			byte <<= 1;
			if (bits[i] == '1') {
				byte |= 1;
			}
		}
		return byte;
		//---------------------------------------------------------------------
	}
	unsigned char Convert8toByte(unsigned char* bits)
	{
		unsigned char arr[8];
		int j = 0;
		// U must be sure to be is8 is true!!!!!!!!!!!!!!!!!!
		for (int k = 0;k < 8;k++)
		{
			arr[k] = bits[j];
			j++;
		}
		int k = 0;
		for (j; j < bitCount;j++)
		{
			bits[k] = bits[j];
			k++;
		}
		bitCount -= 8;
		return bitsToBytesConverter(arr);

	}
	//---------------------------------------------------------------------------------
	// Used for Decompression
	bool checkEOFT(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {
		return a == 'E'&& b == 'O' && c == 'F' && d == 'T'; // chat gpt helped in concept only
	}
	void searchCharInHuffTree(unsigned char* bits, unsigned char* w)
	{
		// we have here n conditions 
		// 1. if found 1 go right
		// 2. if found 2 go left 
		// 3. if found right NULL and left NULL then get this char and shift the rest and continue
		// 4. end of bits so we didnt find it 
		int it = 0;
		UniNode* x = root;
		while (it < bitCount)
		{
			if (x->left == NULL && x->right == NULL)
			{
				w[size] = x->ch;
				size++;
				x = root;
				int k = 0;
				for (k; k < bitCount - it;k++)
				{
					bits[k] = bits[it + k];
				}
				bitCount -= it;
				it = 0;
				if (size == bufferSize) return;

			}
			else if (bits[it] == '0')
			{
				x = x->left;
				it++;
			}
			else if (bits[it] == '1')
			{
				x = x->right;
				it++;
			}
		}
	}
	void turnCharTo8(unsigned char ch, unsigned char* bits) {
		// this function is by chatgpt
		bitset<8> b(ch);
		for (int i = 0; i < 8; ++i) {
			bits[bitCount + i] = b[7 - i] ? '1' : '0'; // store as char '0' or '1'
		}
		bitCount += 8;
		//---------------------------------------------------------------------------
	}
	//-------------------------------------------------------------------------------------
public:
	Huffman()
	{
		bufferSize = 0;
		root = NULL;
		huffCode = new Node * [256];
		for (int i = 0; i < 256; ++i)
			huffCode[i] = NULL;
		bitCount = 0;

	}
	~Huffman()
	{
		for (int i = 0; i < 256; i++) {
			if (huffCode[i]) {
				delete huffCode[i];
			}
		}
		delete[] huffCode;
		h->~Heap();
	}
	// Used for compression
	int readfile(int buffersize, char* file) {
		fstream f;
		this->bufferSize = buffersize;
		unsigned char* A = new unsigned char[buffersize];
		f.open(file, ios::in | ios::binary);
		if (!f)
		{
			printf("Counldn't open read file");
			return -1;
		}
		while (f.read(reinterpret_cast<char*>(A), buffersize) || f.gcount() > 0)
		{
			int bytesRead = f.gcount();
			for (int i = 0;i < bytesRead;i++)
				h->insertFreqTableChar(A[i]);
		}
		delete[] A;
		h->buildHeap();
		f.close();
		return 1;
	}
	void huffmanTreeBuild()
	{
		if (h->heapSize() > 1)
		{
			UniNode* x = h->extract();
			UniNode* y = h->extract();
			UniNode* tmp = new UniNode();
			tmp->left = x;
			tmp->right = y;
			tmp->ch = getRandName();
			tmp->freq = x->freq + y->freq;
			h->insertNodeToHeap(tmp);
			huffmanTreeBuild();
		}
		else if (h->heapSize() == 1)
		{
			// this is the last Node ( root )
			root = h->extract();
		}
		else {
			root = NULL;
		}
	}
	void HuffCode()
	{
		unsigned char* x = new unsigned char[256];
		int index = 0;
		getHuffCode(root, x, index);
		delete[] x;
	}
	int exportingCompressedFile(char* rName, char* wName)
	{
		unsigned char* r = new unsigned char[bufferSize];
		unsigned char* w = new unsigned char[bufferSize];
		unsigned char* bits = new unsigned char[256];
		int size = 0;
		char fullName[200];
		float percentage = 10;
		strcpy_s(fullName, wName);
		strcat_s(fullName, ".ECE2103");
		fstream fin(rName, ios::in | ios::binary| ios::ate);// ate is from chatgpt
		fstream fout(fullName, ios::out | ios::binary);
		if (!fin)
		{
			printf("Couldn't open reading file!!");
			return -1;
		}
		if (!fout)
		{
			printf("Couldn't open writing file!!");
			return -1;
		} // couldnt open wfiles 
		// getting file size 
		// stream size is from chat gpt
		streamsize fSize=fin.tellg();
		streamsize done = 0;
		fin.seekg(0, ios::beg);// to start again from begining from chat gpt
		// -------------------------------------------------
		// Exporting freq table 
		for (int i = 0;i < 256;i++)
		{
			char c = i;
			int g = h->getFreqOfTable(i);
			char t = ':';
			fout.write(reinterpret_cast<char*>(&c), 1); // putting the letter at first
			fout.write(reinterpret_cast<char*>(&t), 1); // then : to flag the number of freq
			fout.write(reinterpret_cast<char*>(&g), sizeof(int)); // then freq 
			fout.write(reinterpret_cast<char*>(&t), 1); // then : to flag end of freq
		}
		char flagEOF[4] = { 'E','O','F','T' };
		fout.write(flagEOF, 4); // !! this indicates end of freq table EOFT 
		//------------------------------------------------------------------------------------------------------
		clock_t start = clock();
		while (fin.read(reinterpret_cast<char*>(r), bufferSize) || fin.gcount() > 0)
		{
			int bytesRead = fin.gcount();
			done += bytesRead;
			
			for (int i = 0;i < bytesRead;i++)
			{
				// We need to turn char to bits and check if they are 8 or more or not if yes turn  it to bytes and put them in buffer size
				turnCharToCode(r[i], bits);
				while (is8())
				{
					if (size == bufferSize) // to export buufer size
					{
						float progress = ((float)done / (float)fSize);
						if (progress > 0.05) {
							percentage = (progress * 90) + 10;
							float timeTaken = (float)( clock() - start) / CLOCKS_PER_SEC;
							float ETA = (float)timeTaken * (((float)100 - percentage) / percentage);
							printf("\r [ %.2f %% | ETA %.1f sec ]", percentage, ETA);
						}
						fout.write(reinterpret_cast<char*>(w), bufferSize);
						size = 0;
					}
					w[size] = Convert8toByte(bits);
					size++;
				}
			}
		}
		if (bitCount != 0) // !!Padding
		{
			int counter = 0;
			for (int i = bitCount;i < 8;i++)
			{
				// Put zeros at last of bits
				bits[i] = '0';
				counter++;
			}
			bitCount = 8;
			w[size] = Convert8toByte(bits);
			size++;
			fout.write(reinterpret_cast<char*>(w), size);
			fout.write(reinterpret_cast<char*>(&counter), sizeof(int));
		}
		fin.close();
		fout.close();
		delete[]r;delete[] w;delete[] bits;
		return 1;
	}
	int recursiveCompressed(char *rFileName, char *wFileName,int bufferSize)
	{
		// we have 4 steps each step we make a file and add n to it and finally comp between it and the other
		while (nCompress <= 9) {
			unsigned char* r = new unsigned char[bufferSize];
			unsigned char* w = new unsigned char[bufferSize];
			unsigned char* bits = new unsigned char[256];
			char arrW[200];
			char arrR[200];
			int size = 0;
			char digit = '0' + nCompress;
			float percentage = 10;
			strcpy_s(arrW, wFileName);
			strcpy_s(arrR, rFileName);
			if (nCompress == 0) {
				strcat_s(arrW, '.' + digit + ".ECE2103");
			}
			else
			{
				strcat_s(arrW, digit + ".ECE2103");
				strcat_s(arrW, (digit-1) + "");

			}
			fstream fin(arrW, ios::in | ios::binary | ios::ate);// ate is from chatgpt
			fstream fout(arrR, ios::out | ios::binary);
			if (!fin)
			{
				printf("Couldn't open reading file!!");
				return -1;
			}
			if (!fout)
			{
				printf("Couldn't open writing file!!");
				return -1;
			}
			// getting file size 
			// stream size is from chat gpt
			streamsize fSize = fin.tellg();
			streamsize done = 0;
			fin.seekg(0, ios::beg);// to start again from begining by chat gpt
			// -------------------------------------------------
			// Exporting freq table 
			for (int i = 0;i < 256;i++)
			{
				char c = i;
				int g = h->getFreqOfTable(i);
				char t = ':';
				fout.write(reinterpret_cast<char*>(&c), 1); // putting the letter at first
				fout.write(reinterpret_cast<char*>(&t), 1); // then : to flag the number of freq
				fout.write(reinterpret_cast<char*>(&g), sizeof(int)); // then freq 
				fout.write(reinterpret_cast<char*>(&t), 1); // then : to flag end of freq
			}
			char flagEOF[4] = { 'E','O','F','T' };
			fout.write(flagEOF, 4); // !! this indicates end of freq table EOFT	
			//------------------------------------------------------------------------------------------------------
			clock_t start = clock();
			while (fin.read(reinterpret_cast<char*>(r), bufferSize) || fin.gcount() > 0)
			{
				int bytesRead = fin.gcount();
				done += bytesRead;

				for (int i = 0;i < bytesRead;i++)
				{
					// We need to turn char to bits and check if they are 8 or more or not if yes turn  it to bytes and put them in buffer size
					turnCharToCode(r[i], bits);
					while (is8())
					{
						if (size == bufferSize) // to export buufer size
						{
							float progress = ((float)done / (float)fSize);
							if (progress > 0.05) {
								percentage = (progress * 90) + 10;
								float timeTaken = (float)(clock() - start) / CLOCKS_PER_SEC;
								float ETA = (float)timeTaken * (((float)100 - percentage) / percentage);
								printf("\r [ %.2f %% | ETA %.1f sec ]", percentage, ETA);
							}
							fout.write(reinterpret_cast<char*>(w), bufferSize);
							size = 0;
						}
						w[size] = Convert8toByte(bits);
						size++;
					}
				}
			}
			if (bitCount != 0) // !!Padding
			{
				int counter = 0;
				for (int i = bitCount;i < 8;i++)
				{
					// Put zeros at last of bits
					bits[i] = '0';
					counter++;
				}
				bitCount = 8;
				w[size] = Convert8toByte(bits);
				size++;
				fout.write(reinterpret_cast<char*>(w), size);
				fout.write(reinterpret_cast<char*>(&counter), sizeof(int));
			}
			if (sizeBef < fSize)
			{
				remove(arrW);// removing last file and here the process is done
				// now we need to take the last recursive compressed file name and ne3temdoo nad remember to put the ncompress at the top of it
				break;
			}

			fin.close();
			fout.close();
			delete[]r;delete[] w;delete[] bits;
			nCompress++;
			return 1;
		}
	}
	//-----------------------------------------------------------------------------------

	// Used for Decompression
	//-----------------------------------------------------------------------------------------
	int readFreqTable(char* fileName, int bufferSize)
	{
		unsigned char* a = new unsigned char[bufferSize];
		unsigned char* freq = new unsigned char[4];
		fstream f;

		f.open(fileName, ios::in | ios::binary);
		if (!f) {
			printf("couldn't open read file");
			return -1;
		}
		this->bufferSize = bufferSize;
		int indx = 0;
		unsigned char c;
		int k = 0;
		bool takeChar = true; // 1 when he doesnt take 
		bool takeInt = false;// 1 when he doenst take
		bool firstCol = false;
		bool secondCol = false;
		bool EOFT = false;
		unsigned char* EOFTFlag = new unsigned char[4];
		// We know that the frequency table is first so we need to read it .
		while ((f.read(reinterpret_cast<char*>(a), bufferSize) || f.gcount() > 0) && !EOFT)
		{
			int byte = f.gcount();
			for (int i = 0;i < byte && !EOFT;i++)
			{
				EOFTFlag[0] = EOFTFlag[1];
				EOFTFlag[1] = EOFTFlag[2];
				EOFTFlag[2] = EOFTFlag[3];
				EOFTFlag[3] = a[i];
				if (checkEOFT(EOFTFlag[0], EOFTFlag[1], EOFTFlag[2], EOFTFlag[3]))
				{
					EOFT = true;
					break;
				}
				// we have 3 conditions 1 . take char 2. pass flag 3 . take freq   and repeat 
				// 1
				if (takeChar)
				{
					c = a[i];
					takeChar = false;
					firstCol = true;
					continue; // for time complixity
				}
				//2
				if (firstCol && a[i] == ':')
				{

					k = 0;
					firstCol = false;
					takeInt = true;
					continue;

				}
				//3 
				if (takeInt && k < 4)
				{
					freq[k] = a[i];
					k++;
					if (k == 4)
					{
						takeInt = false;
						secondCol = true;
						k = 0;
					}

					continue;// to ensure that 4 dont run now
				}
				//4 now we are sure that 1 and 2 didnt happen and 3 finished
				if (secondCol == true)
				{
					if (a[i] == ':') {
						int n = h->turnCharToInt(freq);
						h->insertReadyFreqTable(c, n);
						takeChar = true;
						takeInt = false;
						secondCol = false;
						k = 0;
					}
				}
			}

		}
		delete[]a;
		delete[]EOFTFlag;
		delete[]freq;
		f.close();
	}
	int exportingMainFile(char* rfileName,char* wfileName, int bufferSize)
	{
		fstream fin;
		fstream fout;
		fin.open(rfileName, ios::in | ios::binary | ios::ate);
		fout.open(wfileName, ios::out | ios::binary);
		if (!fin)
		{
			printf("Couldn't open reading file!!");
			return -1;
		}
		if (!fout)
		{
			printf("Couldn't open writing file!!");
			return -1;
		}
		unsigned char* r = new unsigned char[bufferSize];
		unsigned char* w = new unsigned char[bufferSize];
		unsigned char* bits = new unsigned char[256];
		unsigned char EOFTFlag[4];
		bool EOFT = false;
		bool lastIT = false;
		bitCount = 0;
		float percentage = 10;
		this->bufferSize = bufferSize;
		size = 0;
		int count;
		unsigned char c[4];
		unsigned char lastchar;
		int bytesread;
		streamsize fSize = fin.tellg();
		streamsize done = 0;
		fin.seekg(0, ios::beg);
		clock_t start = clock();
		while (fin.read(reinterpret_cast <char*> (r), bufferSize) || fin.gcount() > 0)
		{
			bytesread = fin.gcount();
			done += bytesread;
			for (int i = 0;i < bytesread;i++) {
				// we need to skip EOFT to begin with the main file
				EOFTFlag[0] = EOFTFlag[1];
				EOFTFlag[1] = EOFTFlag[2];
				EOFTFlag[2] = EOFTFlag[3];
				EOFTFlag[3] = r[i];
				if (EOFT == false)
				{
					if (checkEOFT(EOFTFlag[0], EOFTFlag[1], EOFTFlag[2], EOFTFlag[3])) EOFT = true;
					else continue; // no need to enter all the loop
				}
				else
				{
					if (fin.peek() == EOF || bytesread < bufferSize) {
						// End of file reached
						lastIT = true;
						break;
					}
					// start decompressing ...
					turnCharTo8(r[i], bits);
					searchCharInHuffTree(bits, w);
					if (size == bufferSize)
					{
						float progress = ((float)done / (float)fSize);
						if (progress > 0.05) {
							percentage = (progress * 90) + 10;
							float timeTaken = (float)(clock() - start) / CLOCKS_PER_SEC;// clock per sec from chat gpt
							float ETA = (float)timeTaken * (((float)100 - percentage) / percentage);
							printf("\r [ %.2f %% | ETA %.1f sec ]", percentage, ETA);// /r from chat gpt
						}
						fout.write(reinterpret_cast <char*> (w), bufferSize);
						size = 0;
					}
				}
			}
		}
		// Dealing with Paddings !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
		// from my code above i stored last char and a 4byte integer so i know need to convert these char to int and remove last no of it from last char and then add it to the array of bits to start decompressing 
		// first of all extracting these thing 
		for (int i = 0;i < 4;i++)
		{
			c[i] = r[bytesread - 4 + i];
		}
		// 2. we want to get the char 
		lastchar = r[bytesread - 5];
		bytesread -= 5;
		for (int i = 0;i < bytesread;i++)
		{
			turnCharTo8(r[i], bits);
			searchCharInHuffTree(bits, w);
			if (size == bufferSize)
			{
				fout.write(reinterpret_cast <char*> (w), bufferSize);
				size = 0;
			}
			if (i == bytesread - 1)
			{
				// dealing with last bit
				count = h->turnCharToInt(c);
				// kda kda ana mot2aked en albits lesa ma5lesh 
				turnCharTo8(lastchar, bits);
				bitCount -= count - 1; // idk why it is -1 but it worked as this 
				searchCharInHuffTree(bits, w);
				fout.write(reinterpret_cast <char*> (w), size);
			}
		}



		delete[] r; delete[] w; delete[] bits;
		return 1;
	}
	//--------------------------------------------------------------
};
class Compressor
{
private:
	bool isCompressed = false;
	Huffman h;
public:

	int Compress(char* rfileName,char* wfileName, int bufferSize)
	{
		h.readfile(bufferSize, rfileName);
		h.huffmanTreeBuild();
		h.HuffCode();
		// 12.5%	
		h.exportingCompressedFile(rfileName, wfileName);
		return 1;
	}
	int decompress(char* rfileName, char* wfileName, int bufferSize)
	{

		h.readFreqTable(rfileName, bufferSize);
		h.huffmanTreeBuild();
		h.exportingMainFile(rfileName,wfileName, bufferSize);
		return 1;
	}
};
int main(int argc, char* argv[])
{
	
	int bufferSize = 1024;
	if (argc<3)
	{
		printf("too few args");
		return 1;
	}
	else {
		for (int i = 0;i < argc;i++)
		{
			if (strcmp("-b", argv[i]) == 0)
			{
				if (argc >= i + 1) {
					bufferSize =  atof(argv[i + 1]); 
				}
				else {
					printf("Wrong in args!!! (Couldn't identify buffer size)");
					break;
				}
			}
			if (strcmp("-c", argv[i]) == 0)
			{
				if (argc >= i + 2)
				{
					Compressor *c=new Compressor();
					printf("Starting");
					c->Compress(argv[i + 1], argv[i + 2], bufferSize);
					printf("\nDone");
					delete c;
					return 1;
				}
				if (argc >= i + 1)
				{
					Compressor* c = new Compressor();
					printf("Starting");
					c->Compress(argv[i + 1], argv[i + 1], bufferSize);
					printf("\nDone");
					delete c;
					return 2;
				}
			}
			if (strcmp("-d",argv[i])==0)
			{
				if (argc >= i + 2)
				{
					Compressor* c = new Compressor();;
					printf("Starting");
					c->decompress(argv[i + 1], argv[i + 2], bufferSize);
					printf("\nDone");
					delete c;
					return 1; 
				}
				if (argc == i + 1)
				{
					Compressor* c = new Compressor();
					printf("Starting");
					c->decompress(argv[i + 1], argv[i + 1], bufferSize);
					printf("\nDone");
					delete c;
					return 1;

				}
			}

		}
	}
}
