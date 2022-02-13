#include <vector>
using std::vector;

class OutBitStream {
public:
	OutBitStream();

	void WriteBit( unsigned char bit );
	void WriteByte( unsigned char byte );

	const unsigned char* GetBuffer() const;
	int GetBitsCount() const;

private:
	vector<unsigned char> buffer;
	int bitsCount;
};

OutBitStream::OutBitStream() : bitsCount( 0 ) {}

void OutBitStream::WriteBit( unsigned char bit ) {
	if( bitsCount + 1 > buffer.size() * 8 ) {
		buffer.push_back( 0 );
	}

	if( bit != 0 ) {
		int bitPos = bitsCount % 8;
		buffer[bitsCount / 8] |= 1 << bitPos;
	}
	
	++bitsCount;
}

void OutBitStream::WriteByte( unsigned char byte ) {
	if( bitsCount % 8 == 0 ) {
		buffer.push_back( byte );
	} else {
		int bitPos = bitsCount % 8;
		buffer[bitsCount / 8] |= byte << bitPos;
		buffer.push_back( byte >> (8 - bitPos) );
	}
}

const unsigned char* OutBitStream::GetBuffer() const {
	return buffer.data();
}

int OutBitStream::GetBitsCount() const {
	return bitsCount;
}
