#include "binary_stream.h"

namespace cel::experimental::io {
	binary_ifstream::binary_ifstream(std::string path) {
		stream.open(path);
	}
	binary_ifstream::~binary_ifstream() {
		stream.close();
	}
	bool binary_ifstream::eof() {
		return stream.eof();
	}

	binary_ofstream::binary_ofstream(std::string path) {
		stream.open(path);
	}
	binary_ofstream::~binary_ofstream() {
		stream.close();
	}
	bool binary_ofstream::eof() {
		return stream.eof();
	}
}