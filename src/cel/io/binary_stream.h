#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <cstddef>
#include <memory>
namespace cel::experimental::io {
	/**
	* Wrapper for std::ifstream
	* to make reading from binary files easier
	*/
	class binary_ifstream {
	private:
		std::ifstream stream;
	public:
		binary_ifstream(std::string path);
		~binary_ifstream();
		template<typename T>
		T read() {
			char* bytes = new char[sizeof(T)];
			stream.read(bytes, sizeof(T));
			T val = *(T*)bytes;
			delete[] bytes;
			return val;
		}
		bool eof();
	};
	/**
	* Wrapper for std::ofstream
	* to make writing to binary files easier
	*/
	class binary_ofstream {
	private:
		std::ofstream stream;
	public:
		binary_ofstream(std::string path);
		~binary_ofstream();
		template<typename T>
		void write(T& data) {
			char* bytes = reinterpret_cast<char*>(&data);
			stream.write(bytes, sizeof(data));
		}
		bool eof();
		template<typename T>
		binary_ofstream* operator<<(T&& other) {
			this->write(other);
			return this;
		}
	};
}