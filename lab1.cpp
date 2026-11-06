#include <iostream>
#include <clocale>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cmath>
using namespace std;

// ---------- Размеры типов ----------
void printSizes() {
	cout << "bool: " << sizeof(bool) << " байт - логическое значение true/false, нужен 1 бит, но минимум памяти байт" << endl;
	cout << "char: " << sizeof(char) << " байт - один символ, по стандарту всегда 1 байт" << endl;
	cout << "short int: " << sizeof(short int) << " байт - короткое целое, от -2^15 до 2^15 - 1" << endl;
	cout << "int: " << sizeof(int) << " байт - основное целое, от -2^31 до 2^31 - 1" << endl;
	cout << "long int: " << sizeof(long int) << " байт - на Windows 4 байта, на Linux 8" << endl;
	cout << "long long int: " << sizeof(long long int) << " байт - длинное целое, 8 байт на всех платформах" << endl;
	cout << "float: " << sizeof(float) << " байт - вещественное одинарной точности, около 7 цифр" << endl;
	cout << "double: " << sizeof(double) << " байт - вещественное двойной точности, около 15 цифр" << endl;
	cout << "long double: " << sizeof(long double) << " байт - в Visual Studio равен double, в GCC 12 или 16 байт" << endl;
}

// ---------- Биты целого числа ----------
void printIntBits(int value) {
	int bits = sizeof(int) * 8;
	unsigned u = (unsigned)value;

	for (int i = bits - 1; i >= 0; i--) {
		cout << ((u >> i) & 1);
		if (i == bits - 1) cout << ' ';                       // после знакового бита
		if ((bits - 1 - i) % 8 == 7 && i != 0) cout << ' ';   // после каждого байта
	}
	cout << endl;
}

// у беззнакового типа старший бит - обычный разряд, а не знак
void printUIntBits(unsigned value) {
	int bits = sizeof(unsigned) * 8;
	for (int i = bits - 1; i >= 0; i--) {
		cout << ((value >> i) & 1);
		if ((bits - 1 - i) % 8 == 7 && i != 0) cout << ' ';
	}
	cout << endl;
}

// ---------- Биты float и double ----------
// печатает биты с номера from до номера to включительно
void printRange(uint64_t bits, int from, int to) {
	for (int i = from; i >= to; i--) {
		cout << ((bits >> i) & 1);
	}
}

void printFloatBits(float f) {
	uint32_t bits;
	memcpy(&bits, &f, sizeof(bits));

	cout << f << ": [sign]";
	printRange(bits, 31, 31);
	cout << " [exp]";
	printRange(bits, 30, 23);
	cout << " [mant]";
	printRange(bits, 22, 0);
	cout << endl;
}

void printDoubleBits(double d) {
	uint64_t bits;
	memcpy(&bits, &d, sizeof(bits));

	cout << d << ": [sign]";
	printRange(bits, 63, 63);
	cout << " [exp]";
	printRange(bits, 62, 52);
	cout << " [mant]";
	printRange(bits, 51, 0);
	cout << endl;
}

// ---------- Изменение бита ----------
// op: 's' - установить, 'c' - сбросить, 't' - инвертировать
uint64_t applyOp(uint64_t x, int k, char op) {
	uint64_t mask = 1ULL << k;
	if (op == 's') return x | mask;
	if (op == 'c') return x & ~mask;
	if (op == 't') return x ^ mask;
	return x;
}

int modifyBitInt(int value, int k, char op) {
	uint32_t u = (uint32_t)value;
	return (int)(uint32_t)applyOp(u, k, op);
}

float modifyBitFloat(float f, int k, char op) {
	uint32_t bits;
	memcpy(&bits, &f, sizeof(bits));
	bits = (uint32_t)applyOp(bits, k, op);
	memcpy(&f, &bits, sizeof(f));
	return f;
}

double modifyBitDouble(double d, int k, char op) {
	uint64_t bits;
	memcpy(&bits, &d, sizeof(bits));
	bits = applyOp(bits, k, op);
	memcpy(&d, &bits, sizeof(d));
	return d;
}

// ---------- Индивидуальное задание №21: подсчёт единиц (алгоритм Кернигана) ----------
int countOnes(unsigned x) {
	int count = 0;
	while (x) {
		x &= x - 1;   // стирает младшую единицу
		count++;
	}
	return count;
}

// ---------- Демонстрация ----------
int main() {
	setlocale(0, "");

	cout << "=== Размеры типов ===" << endl;
	printSizes();

	cout << "\n=== Целые числа ===" << endl;
	cout << "5:       "; printIntBits(5);
	cout << "-5:      "; printIntBits(-5);
	cout << "0:       "; printIntBits(0);
	cout << "INT_MIN: "; printIntBits(INT_MIN);
	cout << "unsigned 4294967295: "; printUIntBits(UINT_MAX);

	cout << "\n=== float ===" << endl;
	printFloatBits(3.14f);
	printFloatBits(-2.5f);
	printFloatBits(0.0f);
	printFloatBits(1e-40f);   // денормализованное
	printFloatBits(INFINITY);

	cout << "\n=== double ===" << endl;
	printDoubleBits(3.14);
	printDoubleBits(-2.5);
	printDoubleBits(0.0);
	printDoubleBits(1e-310);  // денормализованное
	printDoubleBits(INFINITY);

	cout << "\n=== Изменение бита ===" << endl;
	cout << "5, установить бит 1:     " << modifyBitInt(5, 1, 's') << endl;
	cout << "5, сбросить бит 0:       " << modifyBitInt(5, 0, 'c') << endl;
	cout << "5, инвертировать бит 2:  " << modifyBitInt(5, 2, 't') << endl;
	cout << "5, инвертировать бит 31: "; printIntBits(modifyBitInt(5, 31, 't'));
	cout << "3.14f, инвертировать знак: ";
	printFloatBits(modifyBitFloat(3.14f, 31, 't'));
	cout << "3.14, инвертировать знак: ";
	printDoubleBits(modifyBitDouble(3.14, 63, 't'));

	cout << "\n=== Индивидуальное задание: подсчёт единиц ===" << endl;
	cout << "7 ->   " << countOnes(7) << endl;
	cout << "255 -> " << countOnes(255) << endl;
	cout << "-1 ->  " << countOnes((unsigned)-1) << endl;
	cout << "0 ->   " << countOnes(0) << endl;
}