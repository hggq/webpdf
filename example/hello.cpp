// hello.cpp - minimal "Hello, World" example for the webpdf library.
//
// It creates a one-page A4 PDF containing a single line of text using the
// built-in Helvetica core font, then writes it to "hello.pdf".
//
// WEBPDF_FONT_DIR is provided by the build system and points at the directory
// that holds the core-font metrics (helvetica.json, etc.). When you integrate
// webpdf into your own project, pass the directory where you keep those files.
#include <webpdf.h>
#include <iostream>

#ifndef WEBPDF_FONT_DIR
#define WEBPDF_FONT_DIR "font"
#endif

int main() {
    try {
        auto pdf=std::make_unique<pz::webpdf>();                       // portrait, mm, A4
        pdf->AddPage();

        // Load the Helvetica core font from the font directory, then select it.
        pdf->AddFont("helvetica", "", "", WEBPDF_FONT_DIR);
        pdf->SetFont("helvetica", "", 24);

        pdf->Cell(0, 20, "Hello, World from webpdf!");

        pdf->Output("F", "hello.pdf");
        std::cout << "Wrote hello.pdf" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
