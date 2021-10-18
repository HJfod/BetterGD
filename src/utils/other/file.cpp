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

bgd::Result<> bgd::file_write_string(std::string const& path, std::string const& data) {
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

bgd::Result<> bgd::file_write_binary(std::string const& path, byte_array const& data) {
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

bgd::FileCrawler::FileCrawler(std::string const& path) {
    this->m_sPath = path;
}

bgd::FileCrawler::~FileCrawler() {}

bgd::Result<std::string> bgd::FileCrawler::read() {
    if (!this->m_bIsFile)
        return Err("Not in a file");
    return file_read_string(this->m_sPath.string());
}

bgd::Result<bgd::byte_array> bgd::FileCrawler::readb() {
    if (!this->m_bIsFile)
        return Err("Not in a file");
    return file_read_binary(this->m_sPath.string());
}

bgd::Result<> bgd::FileCrawler::write(std::string const& data) {
    if (!this->m_bIsFile)
        return Err("Not in a file");
    return file_write_string(this->m_sPath.string(), data);
}

bgd::Result<> bgd::FileCrawler::writeb(bgd::byte_array const& data) {
    if (!this->m_bIsFile)
        return Err("Not in a file");
    return file_write_binary(this->m_sPath.string(), data);
}

bgd::Result<> bgd::FileCrawler::enter(std::string const& dir, bool create) {
    if (this->m_bIsFile)
        return Err("Inside file");
    auto path = this->m_sPath / dir;
    if (std::filesystem::exists(path)) {
        if (create) {
            if (std::filesystem::is_regular_file(path)) {
                this->peek({ path.string() });
            } else {
                this->visit({ path.string() });
            }
        } else {
            return Err("Path does not exist");
        }
    }
    this->m_sPath = path;
    if (std::filesystem::is_regular_file(path)) {
        this->m_bIsFile = true;
    }
    return Ok<>();
}

void bgd::FileCrawler::leave() {
    if (this->m_sPath.has_parent_path()) {
        this->m_sPath = this->m_sPath.parent_path();
        this->m_bIsFile = false;
    }
}

void bgd::FileCrawler::peek(std::vector<std::string> const& files) {
    for (auto const& file : files) {
        auto path = this->m_sPath / file;
        file_write_string(path.string(), "");
    }
}

void bgd::FileCrawler::visit(std::vector<std::string> const& dirs) {
    for (auto const& dir : dirs) {
        auto path = this->m_sPath / dir;
        directory_create(path.string());
    }
}

std::vector<std::string> bgd::FileCrawler::list() {
    std::vector<std::string> res;
    for (auto const& entry : this->m_sPath) {
        res.push_back(entry.string());
    }
    return res;
}

std::filesystem::path bgd::FileCrawler::path() {
    return this->m_sPath;
}

std::string bgd::FileCrawler::paths() {
    return this->m_sPath.string();
}
