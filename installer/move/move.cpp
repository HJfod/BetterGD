#include <iostream>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

static std::vector<std::string> g_blacklist = { "zlib1.dll" };

void recurse_copy(fs::path const& path, fs::path const& loader_dir, fs::path const& gd_path, fs::path const& config) {
    if (path.filename() == config) {
        for (auto const& file : fs::directory_iterator(path)) {
            if (file.path().extension() == ".dll") {
                bool valid = true;
                for (auto const& blacklist : g_blacklist) {
                    if (file.path().filename().string() == blacklist) {
                        valid = false;
                    }
                }
                if (valid) {
                    auto src = path / file;
                    auto dst = loader_dir / file.path().filename();
                    std::cout << " *  " << file.path().filename().string() << "\n";
                    if (!fs::copy_file(
                        src, dst,
                        fs::copy_options::overwrite_existing
                    )) {
                        std::cout << " X  Failed to move " << file.path().filename().string() << "\n\n";
                        exit(1);
                    }
                }
            }
            if (file.path().extension() == ".bgd") {
                auto src = path / file;
                auto dst = (gd_path / "BetterGD" / "plugins" / file.path().filename());
                std::cout << " *  " << file.path().filename().string() << "\n";
                if (!fs::copy_file(
                    src, dst,
                    fs::copy_options::overwrite_existing
                )) {
                    std::cout << " X  Failed to move " << file.path().filename().string() << "\n\n";
                    exit(1);
                }
            }
        }
    } else {
        for (auto const& file : fs::directory_iterator(path)) {
            if (fs::is_directory(path / file)) {
                recurse_copy(path / file, loader_dir, gd_path, config);
            }
        }
    }
}

int main(int ac, char* av[]) {
    std::cout << "\n == BetterGD File Moving Service == \n\n";

    if (ac < 2) {
        std::cout << " X  No GD path submitted. Unable to move files\n\n";
        return 1;
    }
    if (ac < 3) {
        std::cout << " X  No build directory submitted. Unable to move files\n\n";
        return 1;
    }
    if (ac < 4) {
        std::cout << " X  No build config submitted. Unable to move files\n\n";
        return 1;
    }

    fs::path gd_path   = av[1];
    fs::path build_dir = av[2];
    fs::path loader_dir= "";
    fs::path config    = av[3];

    std::cout << " -> GD Location: " << gd_path << "\n";

    if (fs::exists(gd_path / "quickldr")) {
        std::cout << " -> Found Modloader: Quickldr\n";
        loader_dir = gd_path / "quickldr";
    } else if (fs::exists(gd_path / "extensions")) {
        std::cout << " -> Found Modloader: MHv6\n";
        loader_dir = gd_path / "extensions";
    } else {
        std::cout << " X  Neither QuickLdr nor MHv6 found.\n\n";
        return 1;
    }

    std::cout << " -> Moving files...\n";

    recurse_copy(build_dir / config, loader_dir, gd_path, config);
    recurse_copy(build_dir / "plugins", loader_dir, gd_path, config);

    std::cout << " <> File moving done\n\n";

    return 0;
}
