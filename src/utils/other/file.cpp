#include <utils/other/file.hpp>
#include <fstream>
#include <filesystem>

bgd::Result<std::string> bgd::file_read_string(std::string const& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize((const size_t)in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return Ok(contents);
    }
    return Err("Unable to open file");
}

bgd::Result<bgd::byte_array> bgd::file_read_binary(std::string const& path) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (in) {
        return Ok(bgd::byte_array (std::istreambuf_iterator<char>(in), {}));
    }
    return Err("Unable to open file");
}

bgd::Result<> bgd::write_file_string(std::string const& path, std::string const& data) {
    std::ofstream file;
    file.open(path);
    if (file.is_open()) {
        file << data;
        file.close();

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}

bgd::Result<> bgd::write_file_binary(std::string const& path, byte_array const& data) {
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        file.close();

        return Ok<>();
    }
    file.close();
    return Err<>("Unable to open file");
}

bgd::Result<> bgd::directory_create(std::string const& path) {
    if (std::filesystem::create_directory(path))
        return Ok<>();
    return Err<>("Unable to create directory");
}

bgd::Result<> bgd::directory_create_all(std::string const& path) {
    if (std::filesystem::create_directories(path))
        return Ok<>();
    return Err<>("Unable to create directories");
}
