// render_html.cpp - render an HTML file to PDF with the webpdf library.
//
// Usage: webpdf-render-html [input.html] [output.pdf]
//   Defaults: content.html -> output.pdf
//
// Images and other assets referenced by the HTML are resolved relative to the
// input HTML file, so the example runs correctly from any working directory.
//
// WEBPDF_FONT_DIR (provided by the build system) points at the directory that
// holds the fonts; the example uses a bundled CJK TTF so that Chinese content
// renders correctly.
#include <webpdf.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

#ifndef WEBPDF_FONT_DIR
#define WEBPDF_FONT_DIR "font"
#endif

static std::string read_file(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Cannot open file: " + path);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int main(int argc, char* argv[]) {
    try {
        std::string docs="./";
        std::string font_file = argc>1?argv[1]:"AlibabaPuHuiTi-Light.otf";
        std::string out = argc>2?argv[2]:(docs+"webpdf_tutorial.pdf");
        std::string font_name = argc>3?argv[3]:"AlibabaPuHuiTi-Light";
        std::string html_file=docs+"webpdf_tutorial.html";
        fs::path out_pdf=fs::absolute(out);
        fs::path font_dir=fs::absolute(docs+"font");
        std::string html=read_file(html_file);

        auto pdf=std::make_unique<pz::webpdf>();
        pdf->setImagesPath(fs::absolute(docs).string()+"/");
        pdf->AddPage();
        pdf->AddFont(font_name,"",font_file,font_dir.string());
        pdf->SetFont(font_name,"",14);
        fs::current_path(fs::absolute(html_file).parent_path());
        pdf->WriteHTML(html);
        pdf->Output("F",out_pdf.string());
        printf("wrote %s\n",out_pdf.string().c_str());
        pdf->Output("F", out_pdf.string());
        std::cout << "PDF cn generated: " << out_pdf.string() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    try {


        std::string docs="./";
        std::string font_file = argc>1?argv[1]:"AlibabaSans-Regular.otf";
        std::string out = argc>2?argv[2]:(docs+"webpdf_tutorial_en.pdf");
        std::string font_name = argc>3?argv[3]:"AlibabaSans-Regular";
        std::string html_file=docs+"webpdf_tutorial_en.html";
        fs::path out_pdf=fs::absolute(out);
        fs::path font_dir=fs::absolute(docs+"font");
        std::string html=read_file(html_file);

        auto pdf=std::make_unique<pz::webpdf>();
        pdf->setImagesPath(fs::absolute(docs).string()+"/");
        pdf->AddPage();
        pdf->AddFont(font_name,"",font_file,font_dir.string());
        pdf->SetFont(font_name,"",14);
        fs::current_path(fs::absolute(html_file).parent_path());
        pdf->WriteHTML(html);
        pdf->Output("F",out_pdf.string());
        printf("wrote %s\n",out_pdf.string().c_str());
        pdf->Output("F", out_pdf.string());
        std::cout << "PDF en generated: " << out_pdf.string() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
