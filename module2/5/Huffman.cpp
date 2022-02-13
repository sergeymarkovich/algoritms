// Алгоритм сжатия данных Хаффмана
#include <algorithm>
#include <list>
#include <vector>
#include <map>
#include <queue>

#include "Huffman.h"

typedef unsigned char byte;

// Получение бита из байта под номером от 1 до 8 (нумерация слева направо)
bool getBit(unsigned char number, char position) {
	switch (position) {
	case 1:
		return ((number & 128) != 0);
	case 2:
		return ((number & 64) != 0);
	case 3:
		return ((number & 32) != 0);
	case 4:
		return ((number & 16) != 0);
	case 5:
		return ((number & 8) != 0);
	case 6:
		return ((number & 4) != 0);
	case 7:
		return ((number & 2) != 0);
	case 8:
		return ((number & 1) != 0);
	default:
		return 1;
	}
}

// Получение байта из 8 бит (n1 - старший разряд, n8 - младший)
unsigned char getByte(bool n1, bool n2, bool n3, bool n4, bool n5, bool n6, bool n7, bool n8) {
	return n1 * 128 + n2 * 64 + n3 * 32 + n4 * 16 + n5 * 8 + n6 * 4 + n7 * 2 + n8;
}

//// ENCODE PART ////

// Простой счетчик чтобы удобно передавать и принимать в функцию и из неё
struct counter {
	int count;
	void inc() { count++; };
	counter() : count(0) {};
};

// Структура для хранения кода символа и длины кода
struct encodedSymbol {
	// Храним 0 и 1
	std::vector<bool> data;
	int length;
	encodedSymbol() : length(0) {};
    ~encodedSymbol() = default;
};

struct HuffmanTreeNode {
	// Частота с которой он встрчается
	int Freq;
	// Массив символов которые он содержит и их количество
	std::vector<unsigned char> symbols;
	int ElemenetsContaining = 0;
	// 0 - левые повороты, 1 - правые повороты.
	// В code хранится 2 поля
	// data - массив из 0 и 1, слева направо получаем повороты от вершины
	// length - количество таких поворотов
	encodedSymbol code;
	HuffmanTreeNode* LeftChild;
	HuffmanTreeNode* RightChild;
	HuffmanTreeNode(int itsPercentage_, int ElementsContaining_, unsigned char symbol_) :
		Freq(itsPercentage_),
		LeftChild(nullptr),
		RightChild(nullptr),
		ElemenetsContaining(ElementsContaining_)
	{
		symbols.push_back(symbol_);
	};

	HuffmanTreeNode() :
		Freq(0),
		ElemenetsContaining(0),
		LeftChild(nullptr),
		RightChild(nullptr)
	{};

	HuffmanTreeNode(int itsPercentage_, int ElementsContaining_, std::vector<unsigned char>& first, std::vector<unsigned char>& second) :
		Freq(itsPercentage_),
		LeftChild(nullptr),
		RightChild(nullptr),
		ElemenetsContaining(ElementsContaining_)
	{
		for (int i = 0; i < first.size(); i++) {
			symbols.push_back(first[i]);
		}
		for (int i = 0; i < second.size(); i++) {
			symbols.push_back(second[i]);
		}
	};
};

struct isLessFreq {
	bool operator()(HuffmanTreeNode *a, HuffmanTreeNode *b) {
		return a->Freq > b->Freq;
	}
};

void ClearTree(HuffmanTreeNode* head) {
    std::list<HuffmanTreeNode*> mList;
    mList.push_front(head);

    while (!mList.empty()) {
        HuffmanTreeNode* tmp = mList.front();
        mList.pop_front();
        if (tmp->RightChild != nullptr) {
            mList.push_front(tmp->RightChild);
        }
        if (tmp->LeftChild != nullptr) {
            mList.push_front(tmp->LeftChild);
        }
        delete tmp;
	}
}

// Рекурсивный обход дерева для создания кода элемента (рекурсивный вызов). Левый поворот: 0, правый поворот: 1
void NodePreOrder(HuffmanTreeNode* root, counter *NodeCounter) {
	if (root) {
		NodeCounter->inc();
		// Если дошли до листа то выходим из функции, продолжать обход нам не надо
		if (root->LeftChild == nullptr && root->RightChild == nullptr)
			return;

		// Если есть левый ребенок то вызываем рекурсивно обход левого поддерева
		// В код левого ребенка добавляем 0, длину левого кода ставим длину кода в данном узле + 1
		if (root->LeftChild != nullptr) {
			for (int i = 0; i < root->code.length; i++) {
				root->LeftChild->code.data.push_back(root->code.data[i]);
			}
			root->LeftChild->code.data.push_back(0);
			root->LeftChild->code.length = root->code.length + 1;
			NodePreOrder(root->LeftChild, NodeCounter);
		}

		// Если есть правый ребенок то вызываем рекурсивно обход правого поддерева
		// В код правого ребенка добавляем 1, длину правого кода ставим длину кода в данном узле + 1
		if (root->RightChild != nullptr) {
			for (int i = 0; i < root->code.length; i++) {
				root->RightChild->code.data.push_back(root->code.data[i]);
			}
			root->RightChild->code.data.push_back(1);
			root->RightChild->code.length = root->code.length + 1;
			NodePreOrder(root->RightChild, NodeCounter);
		}
	}
}

// Рекурсивный обход дерева для создания кода элемента (первичный вызов). Левый поворот: 0, правый поворот: 1
void RootPreOrder(HuffmanTreeNode* root, counter *NodeCounter) {
	if (root) {
		NodeCounter->inc();
		// Рассмотрели случай если дерево состоит из 1 вершины
		// Т.е. один символ, кодируем его 0 и длина кода составит 1
		if (root->LeftChild == nullptr && root->RightChild == nullptr) {
			root->code.data.push_back(0);
			root->code.length = 1;
			return;
		}

		root->code.length = 0;
		// Если есть левый ребенок то вызываем рекурсивно обход левого поддерева
		// В код левого ребенка добавляем 0, длину левого кода ставим 1
		if (root->LeftChild != nullptr) {
			root->LeftChild->code.data.push_back(0);
			root->LeftChild->code.length = 1;
			NodePreOrder(root->LeftChild, NodeCounter);
		}
		// Если есть правый ребенок то вызываем рекурсивно обход правого поддерева
		// В код правого ребенка добавляем 1, длину правого кода ставим 1
		if (root->RightChild != nullptr) {
			root->RightChild->code.data.push_back(1);
			root->RightChild->code.length = 1;
			NodePreOrder(root->RightChild, NodeCounter);
		}
	}
}

// InOrder обход чтобы зашифровать дерево
void InOrderHaffmanTree(HuffmanTreeNode* Root, std::vector<bool>& haffman) {
	if (Root->LeftChild != nullptr || Root->RightChild != nullptr) {
		haffman.push_back(1);
	} else {
		haffman.push_back(0);
		for (int p = 1; p <= 8; p++) {
            haffman.push_back(getBit(Root->symbols[0], p));
        }
	}
	if (Root->LeftChild) {
		InOrderHaffmanTree(Root->LeftChild, haffman);
	}
	if (Root->RightChild) {
		InOrderHaffmanTree(Root->RightChild, haffman);
	}
}

// Функция кодировки
void Encode(IInputStream& original, IOutputStream& compressed) {
	std::vector<unsigned char> string;
	// Ассоциативный массив: индексы - символы, значение - частота символа
	std::map<unsigned char, std::vector<int>> indivSymbols;
	// 2 идентичных массива с индивидуальными символами: значения - указатели на объекты HuffmanTreeNode*
	// Обновляем массив percentage актуальными данными
	std::vector <HuffmanTreeNode*> symbols;
	byte value;
	while (original.Read(value)) {
		string.push_back(value);
		indivSymbols[value].push_back(1);
	}

	for (std::map<unsigned char, std::vector<int>>::iterator it = indivSymbols.begin(); it != indivSymbols.end(); ++it) {
		HuffmanTreeNode *temp = new HuffmanTreeNode();
		temp->symbols.push_back(it->first);
		temp->Freq = it->second.size();
		symbols.push_back(temp);
	}

	std::vector <HuffmanTreeNode*> symbolsCopy  = symbols;

	int symbolsCount = symbols.size();

	if (string.size() == 0) {
        return;
    }
	// Создание очереди из символов
	std::priority_queue<HuffmanTreeNode*, std::vector<HuffmanTreeNode*>, isLessFreq> queue;
	for (int i = 0; i < symbolsCount; i++) {
		queue.push(symbols[i]);
	}

	// Создание дерева Хаффмана
	for (int i = 1; i < symbolsCount; i++) {
		HuffmanTreeNode *temp = new HuffmanTreeNode();
		temp->LeftChild = queue.top();
		queue.pop();
		temp->RightChild = queue.top();
		queue.pop();
		temp->Freq = temp->LeftChild->Freq + temp->RightChild->Freq;
		queue.push(temp);
	}

	// Получили корень дерева Хаффмана
	HuffmanTreeNode *HaffmanTreeRoot;
	HaffmanTreeRoot = queue.top();

	// Считаем путь до каждого символа рекурсивно изнутри этой функции, путь пишется внутри объекта SymbolObject
	counter *NodeCounter = new counter;
	RootPreOrder(HaffmanTreeRoot, NodeCounter);

	// Строим таблицу хаффмана
	std::map<unsigned char, HuffmanTreeNode*> table;
	for (int i = 0; i < symbolsCount; i++) {
		table[symbolsCopy[i]->symbols[0]] = symbolsCopy[i];
	}
	// Сохраняем дерево в побитовый массив haffman
	std::vector<bool> haffman;
	InOrderHaffmanTree(HaffmanTreeRoot, haffman);

	// Сохранили бит шифрования, хаффмана и зашифрованную строку в битовый массив
	std::vector<bool> EncodedData;
	EncodedData.push_back(0);
	for (int f = 0; f < haffman.size(); f++) {
		EncodedData.push_back(haffman[f]);
	}	
	int MsgSize = 0;
	int StrSize = string.size();

	for (int i = 0; i < StrSize; i++) {
		for (int p = 0; p < table[string[i]]->code.length; p++) {
			EncodedData.push_back(table[string[i]]->code.data[p]);
			MsgSize++;
		}
	}
	
	int EightMinusBitsinlastbyte = (8 - EncodedData.size() % 8) % 8;
	for (int f = 0; f < EightMinusBitsinlastbyte; f++)
		EncodedData.push_back(0);
	for (int f = 1; f <= 8; f++)
		EncodedData.push_back(getBit((8 - EightMinusBitsinlastbyte) % 8, f));

	bool encoded = 0;

	/* Если зашифрованное весит больше чем сама строка то 0 */
	/* Если зашифрованное весит меньше чем сама строка то 1 */
	(EncodedData.size()) >= ((string.size() * 8) + 8) ? encoded = false : encoded = true;

	// Делаем битовый выходной массив
	if (encoded) {							// Если выводим зашифрованное 
		EncodedData[0] = encoded;
	} else {							// Если выводим исходное
		EncodedData.clear();
		EncodedData.push_back(encoded);
		for (int f = 1; f <= 7; f++)
			EncodedData.push_back(0);
		for (int f = 0; f < string.size(); f++) {
			for (int p = 1; p <= 8; p++)
				EncodedData.push_back(getBit(string[f], p));
		}
	}
	// Делаем байтовый выходной массив
	std::vector<unsigned char> outputByte;
	for (int f = 0; f < EncodedData.size() / 8; f++) {
		outputByte.push_back(getByte(EncodedData[f * 8 + 0], EncodedData[f * 8 + 1], EncodedData[f * 8 + 2], EncodedData[f * 8 + 3], EncodedData[f * 8 + 4], EncodedData[f * 8 + 5], EncodedData[f * 8 + 6], EncodedData[f * 8 + 7]));
	}
	for (int f = 0; f < outputByte.size(); f++) { 
        compressed.Write(outputByte[f]); 
    }
	delete NodeCounter;
	ClearTree(HaffmanTreeRoot);
	return;
}

  //// DECODE PART ////

struct BitString {
	std::vector<bool> data;
	int lastindex;    
};

void BuildTree(HuffmanTreeNode* Root, BitString* BitString) {
	BitString->lastindex++;
	if (BitString->data[BitString->lastindex]) {	// Если 1 т.е. узел
		Root->LeftChild = new HuffmanTreeNode();
		BuildTree(Root->LeftChild, BitString);
		Root->RightChild = new HuffmanTreeNode();
		BuildTree(Root->RightChild, BitString);
	} else {										// Если 0 т.е. лист
		Root->LeftChild = nullptr;
		Root->RightChild = nullptr;
		unsigned char temp = 0;
		temp = getByte(BitString->data[BitString->lastindex + 1], BitString->data[BitString->lastindex + 2], BitString->data[BitString->lastindex + 3], BitString->data[BitString->lastindex + 4], BitString->data[BitString->lastindex + 5], BitString->data[BitString->lastindex + 6], BitString->data[BitString->lastindex + 7], BitString->data[BitString->lastindex + 8]);
		BitString->lastindex = BitString->lastindex + 8;
		Root->symbols.push_back(temp);
	}
}

void BuildHaffManTree(HuffmanTreeNode* Root, BitString* BitString) {
	BitString->lastindex = 1;

	Root->LeftChild = new HuffmanTreeNode();
	BuildTree(Root->LeftChild, BitString);

	Root->RightChild = new HuffmanTreeNode();
	BuildTree(Root->RightChild, BitString);
}


void Decode(IInputStream& compressed, IOutputStream& original) {
	std::vector<unsigned char> Output;
	std::vector<unsigned char> ByteString;
	byte value;
	while (compressed.Read(value)) {
		ByteString.push_back(value);
	}
	int bytes_in_string = ByteString.size();
	// Вектор хранящий закодированный файл в битовом формате
	
	BitString BitString;
	for (int i = 0; i < bytes_in_string; i++) {
		for (int p = 1; p <= 8; p++)
			BitString.data.push_back(getBit(ByteString[i], p));
	}
	if (BitString.data[0] == 0) {	// Если строка пришла в исходном формате просто вывести ее
		for (int i = 1; i < bytes_in_string; i++) {
			byte temp = getByte(BitString.data[i * 8], BitString.data[i * 8+1], BitString.data[i * 8+2], BitString.data[i * 8+3], BitString.data[i * 8+4], BitString.data[i * 8+5], BitString.data[i * 8+6], BitString.data[i * 8+7]);
			Output.push_back(temp);
		}
		for (int f = 0; f < Output.size(); f++) { 
            original.Write(Output[f]);
        }
		return;
	} else {
		HuffmanTreeNode *HaffmanTreeRoot = new HuffmanTreeNode();
		if (BitString.data[1] == 1) {				// Т.е. в начале дерева Хаффмана узел => имеет минимум 2 ребенка
			BuildHaffManTree(HaffmanTreeRoot, &BitString);
			// lastindex ссылается на ячейку в которой начинается код зашифрованной строки
			BitString.lastindex++;
		} else {								// Т.е. в начале дерева Хаффмана лист => состоит из одного узла
			unsigned char temp = getByte(BitString.data[2], BitString.data[3], BitString.data[4], BitString.data[5], BitString.data[6], BitString.data[7], BitString.data[8], BitString.data[9]);
			HaffmanTreeRoot->symbols.push_back(temp);
			// lastindex ссылается на ячейку в которой начинается код зашифрованной строки
			BitString.lastindex = 10;
		}

		int BitsInPreLastByte = 0;
		int last = BitString.data.size() - 1;
		BitsInPreLastByte = BitString.data[last - 3] * 8 + BitString.data[last - 2] * 4 + BitString.data[last - 1] * 2 + BitString.data[last];

		int LastBitIndex = 0;
		if (BitsInPreLastByte == 0)
			LastBitIndex = BitString.data.size() - 8 - 1;
		else
			LastBitIndex = BitString.data.size() - 8 - 8 + BitsInPreLastByte - 1;

		std::vector<bool> codedSymbols;

		// Считаем количество значащих бит в предпоследнем байте
		int amountofbitsintheprelastbyte = BitsInPreLastByte;

		// lastindex - храним индекс ячейки в которой лежит последний бит зашифрованных символов
		int lastindex = LastBitIndex;
		// В массиве codedSymbols лежат закодированные символы
		for (int i = BitString.lastindex; i <= lastindex; i++) {
			codedSymbols.push_back(BitString.data[i]);
		}
		// Осталось расшифровать строку при помощи дерева
		HuffmanTreeNode *Node = HaffmanTreeRoot;
		if (BitString.data[1] == 1) { // Т.е. минимум 2 символа
			for (int p = 0; p < codedSymbols.size(); p++) {
				if (codedSymbols[p] == 0)
					Node = Node->LeftChild;
				if (codedSymbols[p] == 1)
					Node = Node->RightChild;

				if ((Node->LeftChild == nullptr) &( Node->RightChild == nullptr)) {
					Output.push_back(Node->symbols[0]);
					Node = HaffmanTreeRoot;
				}
			}
		} else {
			for (int p = 0; p < codedSymbols.size(); p++) {
				Output.push_back(HaffmanTreeRoot->symbols[0]);
			}
		}
		for (int f = 0; f < Output.size(); f++) { 
            original.Write(Output[f]); 
        }
        codedSymbols.clear();
		ClearTree(HaffmanTreeRoot);
	}
	return;
}
