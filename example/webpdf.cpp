#include <webpdf.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

static std::string read_file(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Cannot open file: " + path);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static std::vector<fs::path> get_system_font_paths() {
    std::vector<fs::path> paths;
#ifdef _WIN32
    paths.push_back(fs::path("C:/Windows/Fonts"));
#elif __APPLE__
    paths.push_back(fs::path("/System/Library/Fonts"));
    paths.push_back(fs::path("/Library/Fonts"));
    if (const char* home = getenv("HOME")) {
        paths.push_back(fs::path(home) / "Library/Fonts");
    }
#else
    paths.push_back(fs::path("/usr/share/fonts"));
    paths.push_back(fs::path("/usr/local/share/fonts"));
    if (const char* home = getenv("HOME")) {
        paths.push_back(fs::path(home) / ".fonts");
    }
#endif
    return paths;
}

static fs::path find_font_file_in_dir(const std::string& font_name, const fs::path& dir) {
    std::vector<std::string> extensions = {".otf", ".ttf", ".TTF", ".OTF"};

    if (!fs::exists(dir)) return fs::path();
    try {
        for (const auto& entry : fs::recursive_directory_iterator(dir)) {
            if (!entry.is_regular_file()) continue;
            std::string filename = entry.path().filename().string();
            for (const auto& ext : extensions) {
                if (filename.size() >= ext.size() &&
                    filename.substr(filename.size() - ext.size()) == ext) {
                    if (filename.find(font_name) != std::string::npos) {
                        return entry.path();
                    }
                }
            }
        }
    } catch (...) {}
    return fs::path();
}

static fs::path find_font_file(const std::string& font_name) {
    std::vector<fs::path> search_paths = get_system_font_paths();
    for (const auto& dir : search_paths) {
        fs::path found = find_font_file_in_dir(font_name, dir);
        if (!found.empty()) return found;
    }
    return fs::path();
}

static void print_usage(const std::string& prog) {
    std::cerr << "Usage: " << prog << " <input.html> <output.pdf> [font_file]\n";
    std::cerr << "\nArguments:\n";
    std::cerr << "  input.html    Path to the HTML file to render\n";
    std::cerr << "  output.pdf    Path to the output PDF file\n";
    std::cerr << "  font_file     Optional font file (searches system fonts if not provided)\n";
    std::cerr << "\nExample:\n";
    std::cerr << "  " << prog << " /www/content.html /www/out.pdf\n";
    std::cerr << "  " << prog << " /www/content.html /www/out.pdf /path/to/font.ttf\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    try {
        std::string html_file = argv[1];
        std::string output_file = argv[2];

        fs::path abs_html = fs::absolute(html_file);
        fs::path abs_output = fs::absolute(output_file);

        if (!fs::exists(abs_html)) {
            throw std::runtime_error("Input file does not exist: " + abs_html.string());
        }

        fs::path font_path;

        if (argc > 3) {
            font_path = fs::absolute(argv[3]);
            if (!fs::exists(font_path)) {
                font_path = abs_html.parent_path() / argv[3];
                if (!fs::exists(font_path)) {
                    throw std::runtime_error("Font file not found: " + std::string(argv[3]));
                }
            }
        } else {
            std::vector<std::string> font_names = {"PingFang", "SimHei", "Arial", "Helvetica"};
            std::vector<fs::path> search_dirs;

            search_dirs.push_back(abs_html.parent_path() / "font");
            search_dirs.push_back(fs::current_path() / "font");

            for (const auto& dir : get_system_font_paths()) {
                search_dirs.push_back(dir);
            }

            for (const auto& font_name : font_names) {
                for (const auto& dir : search_dirs) {
                    font_path = find_font_file_in_dir(font_name, dir);
                    if (!font_path.empty()) break;
                }
                if (!font_path.empty()) break;
            }

            if (font_path.empty()) {
                throw std::runtime_error("No suitable font found. Please specify font file.");
            }
        }

        std::string font_name = font_path.stem().string();
        std::string html = read_file(abs_html.string());

        auto pdf = std::make_unique<pz::webpdf>();
        pdf->setImagesPath(abs_html.parent_path().string() + "/");
        pdf->AddPage();
        pdf->AddFont(font_name, "", font_path.filename().string(), font_path.parent_path().string() + "/");
        pdf->SetFont(font_name, "", 14);

        fs::current_path(abs_html.parent_path());
        pdf->WriteHTML(html);
        pdf->Output("F", abs_output.string());

        std::cout << "PDF generated: " << abs_output.string() << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}