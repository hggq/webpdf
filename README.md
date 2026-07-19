# webpdf HTML to PDF Tutorial

English | [简体中文](README_CN.md)  

## Table of Contents

- [Chapter 1 Introduction](#chapter-1-introduction)
  - [1.1 What is webpdf](#11-what-is-webpdf)
  - [1.2 Key Features](#12-key-features)
- [Chapter 2 Quick Start](#chapter-2-quick-start)
  - [2.1 Building the Project](#21-building-the-project)
  - [2.2 Running Tests](#22-running-tests)
  - [2.3 Simplest Example](#23-simplest-example)
- [Chapter 3 Text Styling](#chapter-3-text-styling)
  - [3.1 Fonts and Sizes](#31-fonts-and-sizes)
    - [3.1.1 Weight and Italics](#311-weight-and-italics)
    - [3.1.2 Underline and Color](#312-underline-and-color)
  - [3.2 Text Alignment](#32-text-alignment)
  - [3.3 Line Height and Paragraphs](#33-line-height-and-paragraphs)
- [Chapter 4 Tables](#chapter-4-tables)
  - [4.1 Basic Tables](#41-basic-tables)
  - [4.2 Table Alignment](#42-table-alignment)
  - [4.3 Merging Cells](#43-merging-cells)
  - [4.4 Multi‑Page Tables](#44-multi-page-tables)
- [Chapter 5 Images and SVG](#chapter-5-images-and-svg)
  - [5.1 Raster Images](#51-raster-images)
  - [5.2 SVG Vectors](#52-svg-vectors)
  - [5.3 Inline SVG](#53-inline-svg)
  - [5.4 SVG Gradients](#54-svg-gradients)
    - [5.4.1 Linear Gradients](#541-linear-gradients)
    - [5.4.2 Radial Gradients](#542-radial-gradients)
  - [5.5 Paths and Shapes](#55-paths-and-shapes)
- [Chapter 6 Page Layout](#chapter-6-page-layout)
  - [6.1 Forced Page Break](#61-forced-page-break)
  - [6.2 Full‑Page Images](#62-full-page-images)
  - [6.3 Containers and Backgrounds](#63-containers-and-backgrounds)
- [Chapter 7 Advanced Features](#chapter-7-advanced-features)
  - [7.1 Hyperlinks](#71-hyperlinks)
  - [7.2 Horizontal Rules](#72-horizontal-rules)
  - [7.3 Headers and Footers](#73-headers-and-footers)
  - [7.4 Table of Contents Generation](#74-table-of-contents-generation)
  - [7.5 Cover and Title Pages](#75-cover-and-title-pages)
- [Chapter 8 API Reference](#chapter-8-api-reference)
  - [8.1 FPDF Class](#81-fpdf-class)
  - [8.2 Complete Code Example](#82-complete-code-example)
- [Chapter 9 FAQ](#chapter-9-faq)
  - [9.1 Text Issues](#91-text-issues)
  - [9.2 Image Issues](#92-image-issues)
  - [9.3 Pagination Issues](#93-pagination-issues)
  - [9.4 Other Issues](#94-other-issues)
- [Project File Structure](#project-file-structure)

---

## Chapter 1 Introduction

### 1.1 What is webpdf

webpdf is a lightweight HTML‑to‑PDF library written in C++. It renders HTML content into high‑quality PDF documents without relying on a browser engine. It parses HTML and CSS directly and generates PDF commands, so it starts fast and uses little memory.

This library is especially suitable for:
- Server‑side batch generation of PDF reports and documents
- PDF generation in embedded systems
- Applications that need precise control over PDF output
- Environments where startup speed and memory footprint matter

### 1.2 Key Features

| Feature | Description |
|---------|-------------|
| HTML parsing | Supports common HTML tags and CSS styles |
| Chinese support | Full TrueType font support for Chinese characters |
| SVG rendering | Supports vector graphics and gradient fills |
| Automatic pagination | Smart page breaking; table headers repeat across pages |
| Headers & footers | Custom headers/footers with page number placeholders |
| TOC generation | Auto‑generates clickable table of contents with page numbers |
| Full‑page images | `class="pagefull"` renders an image to fill the entire page |

> **Note:** webpdf is **not** a browser engine. It is a dedicated HTML/CSS renderer that supports only commonly used tags and styles. For complex web applications, consider browser‑based solutions.

---

## Chapter 2 Quick Start

### 2.1 Building the Project

The project uses Makefile. Make sure `g++` and `make` are installed. In the project root:

```bash
cd webpdf
make
```

After a successful build, the executable `test_html` is generated.

### 2.2 Running Tests

Run the test program. By default it reads `content.html` and outputs `output.pdf`:

```bash
./test_html
```

You can also specify input and output files:

```bash
./test_html input.html output.pdf
```

### 2.3 Simplest Example

Here is a minimal HTML file with a title and a paragraph:

```html
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Hello</title>
<style>
body {
    font-family: msyh;
    font-size: 24px;
}
</style>
</head>
<body>
    <h1>Hello, webpdf!</h1>
    <p>This is my first PDF document.</p>
</body>
</html>
```

> **Tip:** Make sure your HTML file is UTF‑8 encoded, otherwise Chinese characters may appear garbled.

---

## Chapter 3 Text Styling

### 3.1 Fonts and Sizes

webpdf supports setting fonts and sizes via CSS. Before using a font, you must register the font file with the C++ API method `AddFont()`.

Example:

```css
body {
    font-family: "Microsoft YaHei";
    font-size: 24px;
}
```

#### 3.1.1 Weight and Italics

Use `<b>` or `<strong>` for **bold text**, and `<i>` or `<em>` for *italic text*. You can combine them for ***bold italics***.

#### 3.1.2 Underline and Color

Use `<u>` for <u>underlined text</u>. Use `<font color="...">` to set text color, e.g. <font color="#FF0000">red</font>, <font color="#00AA00">green</font>, <font color="#0000FF">blue</font>.

### 3.2 Text Alignment

- Left: `text-align: left`
- Center: `text-align: center`
- Right: `text-align: right`

### 3.3 Line Height and Paragraphs

Line‑height controls the vertical spacing between lines. A good value is 1.2 to 1.5 times the font size for comfortable reading.

---

## Chapter 4 Tables

### 4.1 Basic Tables

Use `<table>` to create a table. Add `border="1"` to show borders.

```html
<table border="1" style="width: 100%;">
    <thead>
        <tr>
            <th>Product</th>
            <th>Price</th>
            <th>Stock</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>Web Framework</td>
            <td>¥299</td>
            <td>128</td>
        </tr>
        <tr>
            <td>Mobile Component Library</td>
            <td>¥499</td>
            <td>56</td>
        </tr>
        <tr>
            <td>Enterprise Management System</td>
            <td>¥1999</td>
            <td>12</td>
        </tr>
    </tbody>
</table>
```

### 4.2 Table Alignment

Cell content can be aligned horizontally and vertically:

| Alignment | CSS value |
|-----------|-----------|
| Horizontal left | `text-align: left` |
| Horizontal center | `text-align: center` |
| Horizontal right | `text-align: right` |
| Vertical top | `vertical-align: top` |
| Vertical middle | `vertical-align: middle` |
| Vertical bottom | `vertical-align: bottom` |

### 4.3 Merging Cells

Use `colspan` to merge columns, and `rowspan` to merge rows.

```html
<table border="1" style="width: 100%;">
    <thead>
        <tr>
            <th colspan="2">Product Info</th>
            <th>Price</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="3">Software</td>
            <td>Basic</td>
            <td>¥99</td>
        </tr>
        <tr>
            <td>Pro</td>
            <td>¥299</td>
        </tr>
        <tr>
            <td>Enterprise</td>
            <td>¥999</td>
        </tr>
    </tbody>
</table>
```

### 4.4 Multi‑Page Tables

When a table is long and spans multiple pages, add `style="position: fixed"` to the `<table>` tag. This makes the header repeat automatically on every page.

```html
<table border="1" style="width: 100%; position: fixed;">
    <thead>
        <tr>
            <th>Feature</th>
            <th>Basic</th>
            <th>Pro</th>
        </tr>
    </thead>
    <tfoot>
        <tr>
            <td style="text-align: right;">Total features</td>
            <td>12</td>
            <td>28</td>
        </tr>
    </tfoot>
    <tbody>
        <tr>
            <td>User management</td>
            <td>✓</td>
            <td>✓</td>
        </tr>
        <!-- more rows -->
    </tbody>
</table>
```

---

## Chapter 5 Images and SVG

### 5.1 Raster Images

webpdf supports PNG, JPG and other common raster formats. Use `<img>` to insert an image:

```html
<img src="images/cover.jpg">
```

### 5.2 SVG Vectors

SVG (Scalable Vector Graphics) scale without losing quality and are great for charts and icons. You can reference an SVG file via `<img>` or embed the SVG code directly.

```html
<img src="images/diagram.svg">
```

### 5.3 Inline SVG

Embedding SVG code directly gives you more control:

```html
<svg width="300" height="200" xmlns="http://www.w3.org/2000/svg">
    <rect x="10" y="10" width="280" height="180" fill="#f8f9fa" stroke="#dee2e6" stroke-width="2" rx="8"/>
    <rect x="30" y="50" width="60" height="120" fill="#4285f4"/>
    <rect x="110" y="80" width="60" height="90" fill="#ea4335"/>
    <rect x="190" y="30" width="60" height="140" fill="#fbbc05"/>
    <text x="60" y="185" font-size="12" text-anchor="middle">Jan</text>
    <text x="140" y="185" font-size="12" text-anchor="middle">Feb</text>
    <text x="220" y="185" font-size="12" text-anchor="middle">Mar</text>
    <text x="150" y="35" font-size="14" text-anchor="middle">Quarterly Sales</text>
</svg>
```

### 5.4 SVG Gradients

webpdf supports both linear and radial gradients.

#### 5.4.1 Linear Gradients

Linear gradients change color along a straight line:

```html
<svg width="400" height="50" xmlns="http://www.w3.org/2000/svg">
    <defs>
        <linearGradient id="rainbow" x1="0%" y1="0%" x2="100%" y2="0%">
            <stop offset="0%" stop-color="#FF0000"/>
            <stop offset="17%" stop-color="#FF7F00"/>
            <stop offset="33%" stop-color="#FFFF00"/>
            <stop offset="50%" stop-color="#00FF00"/>
            <stop offset="67%" stop-color="#0000FF"/>
            <stop offset="83%" stop-color="#4B0082"/>
            <stop offset="100%" stop-color="#9400D3"/>
        </linearGradient>
    </defs>
    <rect x="0" y="0" width="400" height="50" fill="url(#rainbow)"/>
</svg>
```

#### 5.4.2 Radial Gradients

Radial gradients radiate from a center point, good for spheres and glow effects:

```html
<svg width="300" height="120" xmlns="http://www.w3.org/2000/svg">
    <defs>
        <radialGradient id="ball" cx="35%" cy="35%" r="60%">
            <stop offset="0%" stop-color="#ffffff"/>
            <stop offset="100%" stop-color="#2c5aa0"/>
        </radialGradient>
    </defs>
    <circle cx="60" cy="60" r="50" fill="url(#ball)"/>
    <ellipse cx="180" cy="60" rx="80" ry="50" fill="url(#ball)"/>
</svg>
```

### 5.5 Paths and Shapes

SVG supports many basic shapes and paths:

| Element | Description |
|---------|-------------|
| `<line>` | Straight line |
| `<polyline>` | Polyline (connected segments) |
| `<polygon>` | Polygon |
| `<path>` | Arbitrary path |
| `<ellipse>` | Ellipse |

---

## Chapter 6 Page Layout

### 6.1 Forced Page Break

To force a page break at a specific position, use `page-break-before: always`:

```html
<div style="page-break-before: always;"></div>
```

This empty div produces no visible content but triggers a page break.

### 6.2 Full‑Page Images

Use `class="pagefull"` to make an image fill the entire page – no headers, footers, or margins. The image is scaled proportionally and centered.

```html
<p class="pagefull"><img src="images/fullpage.png"></p>
```

> **Tip:** You can put `pagefull` on any container. The first `<img>` inside it will be used for the full‑page render; the container and other children are hidden.

### 6.3 Containers and Backgrounds

Use `<div>` to create block containers with background, border, border‑radius, etc.:

```html
<div style="background-color: #e3f2fd; padding: 20px; border-radius: 10px; border: 1px solid #2196f3;">
    <strong style="color: #1565c0;">Info Box</strong>
    <p>This is a blue‑themed notice box.</p>
</div>
```

---

## Chapter 7 Advanced Features

### 7.1 Hyperlinks

Use `<a>` to create clickable links in the PDF:

```html
<a href="https://www.example.com">Example Website</a>
```

Long links wrap correctly across lines.

### 7.2 Horizontal Rules

Use `<hr>` to insert horizontal separators:

```html
<hr>
<hr style="border-color: #f44336; border-width: 3px;">
<hr style="border-color: #4caf50; border-style: dashed; border-width: 2px;">
<hr style="border-color: #2196f3; border-style: dotted; border-width: 2px; width: 50%; margin: 0 auto;">
```

### 7.3 Headers and Footers

Use `class="pageheader"` and `class="pagefooter"` to define custom headers and footers on every page.

```html
<div class="pageheader" style="height: 30px;">
    <div style="text-align: right; margin-right: 100px;">
        <img src="images/logo.svg" style="padding-right: 20px;">
        <span>Page {pagenum} / {pagetotal}</span>
    </div>
</div>

<div class="pagefooter" style="height: 20px;">
    <div style="text-align:center;">— Page {pagenum} / {pagetotal} —</div>
</div>
```

Two special placeholders are available:

| Placeholder | Description |
|-------------|-------------|
| `{pagenum}` | Current page number |
| `{pagetotal}` | Total number of pages |

> **Note:** By default, cover pages and TOC pages do not show headers/footers. Full‑page images (`pagefull`) also suppress them.

### 7.4 Table of Contents Generation

webpdf can auto‑generate a table of contents. Add `class="catalogue1"`, `catalogue2`, or `catalogue3` to headings to include them in the TOC.

| Class | TOC Level | Corresponding Headings |
|-------|-----------|------------------------|
| `catalogue1` | Level 1 | h1 ~ h3 |
| `catalogue2` | Level 2 | h4 ~ h5 |
| `catalogue3` | Level 3 | h5 ~ h6 |

The generated TOC includes page numbers and clickable links to the sections.

### 7.5 Cover and Title Pages

Use `class="pagecover"` to create a cover page – no headers/footers, content starts at the top of the page.

Use `class="pagecatalogue"` for a TOC background, together with `class="cataloguecontent"` to auto‑insert the TOC.

```html
<!-- Cover -->
<div class="pagecover" style="background-image:url(images/cover.jpg);">
    <h1 style="padding-top: 260px; text-align: center;">Document Title</h1>
</div>

<!-- TOC background -->
<div class="pagecatalogue" style="background-image:url(images/mulu.jpeg);">
    <h3 style="padding-top: 200px; text-align: center;">Table of Contents</h3>
</div>

<!-- Auto‑generated TOC content -->
<div class="cataloguecontent" title="Table of Contents"></div>
```

---

## Chapter 8 API Reference

### 8.1 FPDF Class

`FPDF` is the core PDF document class.

| Method | Description |
|--------|-------------|
| `FPDF()` | Constructor, creates a new PDF document |
| `AddPage()` | Adds a new page |
| `AddFont(family, style, file, dir)` | Registers a font file |
| `SetFont(family, style, size)` | Sets the current font |
| `WriteHTML(html, line_height)` | Renders HTML content to the PDF |
| `Output(dest, name)` | Outputs the PDF file |
| `SetShowHeader(show)` | Enables/disables header display |
| `SetShowFooter(show)` | Enables/disables footer display |
| `SetShowHeaderFooter(show)` | Enables/disables both header and footer |
| `PageNo()` | Returns the current page number |
| `SetPageNumberOffset(offset)` | Sets the page number offset |
| `SetLeftMargin(margin)` | Sets the left margin |
| `SetRightMargin(margin)` | Sets the right margin |
| `SetTopMargin(margin)` | Sets the top margin |
| `SetAutoPageBreak(auto, margin)` | Configures automatic page breaking |
| `GetPageWidth()` | Returns page width |
| `GetPageHeight()` | Returns page height |
| `GetX() / GetY()` | Returns current drawing position |
| `SetXY(x, y)` | Sets current drawing position |
| `Image(file, x, y, w, h)` | Inserts an image |
| `Rect(x, y, w, h, style)` | Draws a rectangle |
| `SetFillColor(r, g, b)` | Sets fill color |
| `SetDrawColor(r, g, b)` | Sets stroke color |
| `SetTextColor(r, g, b)` | Sets text color |
| `SetLineWidth(width)` | Sets line width |
| `AddAxialShading(...)` | Adds an axial (linear) gradient |
| `ShadeFill(x, y, w, h, id)` | Fills a rectangle with a gradient |

### 8.2 Complete Code Example

```cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "fpdf.h"

std::string read_file(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Cannot open file: " + path);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int main(int argc, char* argv[]) {
    try {
        std::string html_file = "content.html";
        std::string pdf_file = "output.pdf";

        if (argc > 1) html_file = argv[1];
        if (argc > 2) pdf_file = argv[2];

        std::string html = read_file(html_file);

        webpdf::FPDF pdf;
        pdf.AddPage();
        pdf.AddFont("msyh", "", "msyh.ttf", "font/");
        pdf.SetFont("msyh", "", 14);
        pdf.WriteHTML(html);
        pdf.Output("F", pdf_file);

        std::cout << "PDF generated: " << pdf_file << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
```

---

## Chapter 9 FAQ

### 9.1 Text Issues

| Problem | Solution |
|---------|----------|
| Chinese characters show as boxes or garbled | Make sure you registered the font with `AddFont()` and the CSS `font-family` matches exactly. |
| Bold/italic not working | You need to register the corresponding font file; webpdf does not fake bold/italic. |
| Wrong line breaks | Check for special characters or extra spaces; CJK characters are broken by character width. |

### 9.2 Image Issues

| Problem | Solution |
|---------|----------|
| Image not showing | Verify the path is correct relative to the working directory. |
| SVG renders black | Check SVG syntax; gradients must be defined inside `<defs>`. |
| SVG gradient wrong | Ensure the gradient ID is correct and referenced with `url(#id)`. |

### 9.3 Pagination Issues

| Problem | Solution |
|---------|----------|
| Blank page before page break | Use an empty div with `page-break-before: always` and no content inside. |
| Table header not repeating across pages | Add `style="position: fixed"` to the `<table>` tag. |
| `pagefull` image still shows header/footer | Make sure `class="pagefull"` is on a container that has an `<img>` child. |

### 9.4 Other Issues

| Problem | Solution |
|---------|----------|
| TOC page numbers are off | TOC page numbers are estimated; complex layouts may cause small deviations. |
| Some CSS styles ignored | webpdf supports only common CSS properties; check the supported list in this tutorial. |
| Generated PDF is too large | Use compressed images and avoid large bitmaps where possible. |

---

## Project File Structure

```
webpdf/
├── .github/workflows/      # CI/CD workflows
│   ├── ci.yml
│   └── release.yml
├── cmake/                  # CMake configuration templates
│   ├── webpdf.pc.in
│   └── webpdfConfig.cmake.in
├── example/                # Example programs and tutorials
│   ├── font/               # Font files for examples
│   │   ├── AlibabaPuHuiTi-Light.otf/ttf
│   │   ├── AlibabaSans-Regular.otf/ttf
│   │   └── *.json (font configs)
│   ├── images/             # Sample images
│   │   ├── cover.jpg
│   │   ├── mulu.jpeg
│   │   └── *.png/*.svg
│   ├── hello.cpp           # Minimal example
│   ├── render_html.cpp     # HTML rendering example
│   ├── webpdf_tutorial.html      # Chinese tutorial
│   ├── webpdf_tutorial_en.html   # English tutorial
│   └── TUTORIAL.md         # Markdown tutorial
├── include/webpdf/         # Public header files
│   ├── webpdf.h            # Main API header
│   ├── parsecss.h          # CSS parser
│   ├── parsehtml.h         # HTML parser
│   ├── parsesvg.h          # SVG parser
│   ├── ttfparser.h         # TrueType font parser
│   ├── gzip.h              # GZIP compression
│   └── request.h           # HTTP request utilities
├── ports/webpdf/           # vcpkg port configuration
│   ├── portfile.cmake
│   └── vcpkg.json
├── src/                    # Source files
│   ├── webpdf.cpp
│   ├── parsecss.cpp
│   ├── parsehtml.cpp
│   ├── parsesvg.cpp
│   ├── ttfparser.cpp
│   ├── gzip.cpp
│   └── request.cpp
├── CMakeLists.txt          # CMake build configuration
├── LICENSE                 # MIT License
├── Makefile                # Make build system
├── README.md               # English README
├── README_CN.md            # Chinese README
├── vcpkg.json              # vcpkg manifest
├── webpdf.pc               # pkg-config file
├── webpdf.rb               # Homebrew formula
└── brew.sh                 # Brew installation script
```

**Final note:** webpdf is an ongoing project hosted at [https://github.com/hggq/webpdf](https://github.com/hggq/webpdf). If you run into issues or need new features, feedback and contributions are welcome. We hope this tutorial helps you get started quickly and produce beautiful PDF documents!
