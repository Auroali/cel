#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <cstddef>
#include <memory>
#include <filesystem>

/**
 * @namespace cel::io IO related code, such as binary file readers/writers and model importers
 */
namespace cel::io {
	/**
	* Wrapper for std::ifstream
	* to make reading from binary files easier
	*/
	class binary_ifstream {
	private:
		std::ifstream stream;
	public:
		binary_ifstream(const std::filesystem::path& path);
		~binary_ifstream();
		template<typename T>
		T read() {
			char* bytes = read(sizeof(T));
			T val = *(T*)bytes;
			delete[] bytes;
			return val;
		}
		char* read(size_t size) {
			char* bytes = new char[size];
			stream.read(bytes, size);
			return bytes;
		}
		bool eof();
		std::string read_str(size_t size);
	};
	/**
	* Wrapper for std::ofstream
	* to make writing to binary files easier
	*/
	class binary_ofstream {
	private:
		std::ofstream stream;
	public:
		void write_str(const std::string& str);
		binary_ofstream(const std::filesystem::path& path);
		~binary_ofstream();
		template<typename T>
		void write(T& data) {
			char* bytes = reinterpret_cast<char*>(&data);
			stream.write(bytes, sizeof(data));
		}
		void write(const char* data, size_t size) {
			stream.write(data, size);
		}
		bool eof();
		template<typename T>
		binary_ofstream* operator<<(T&& other) {
			this->write(other);
			return this;
		}
		void seekp(std::streampos pos);
	};
}