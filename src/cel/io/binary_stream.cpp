#include "binary_stream.h"
#include <sstream>

namespace cel::io {
	binary_ifstream::binary_ifstream(const std::filesystem::path& path) {
		stream.open(path);
	}

	binary_ifstream::~binary_ifstream() {
		stream.close();
	}

	bool binary_ifstream::eof() {
		return stream.eof();
	}

	binary_ofstream::binary_ofstream(const std::filesystem::path& path) {
		stream.open(path);
	}

	binary_ofstream::~binary_ofstream() {
		stream.close();
	}

	bool binary_ofstream::eof() {
		return stream.eof();
	}

	void binary_ofstream::write_str(const std::string& str) {
		for(char c : str)
			this->write(c);
	}

	void binary_ofstream::seekp(std::streampos pos) {
		stream.seekp(pos);
	}
	
	std::string binary_ifstream::read_str(size_t size) {
		std::stringstream str;
		for(size_t i = 0; i < size; ++i) {
			str << read<char>();
		}
		return str.str();
	}
}