# webpdf HTML转PDF 使用教程

## 目录

- [第一章 简介](#第一章-简介)
  - [1.1 什么是 webpdf](#11-什么是-webpdf)
  - [1.2 主要特性](#12-主要特性)
- [第二章 快速开始](#第二章-快速开始)
  - [2.1 编译项目](#21-编译项目)
  - [2.2 运行测试](#22-运行测试)
  - [2.3 最简单的示例](#23-最简单的示例)
- [第三章 文本样式](#第三章-文本样式)
  - [3.1 字体与字号](#31-字体与字号)
    - [3.1.1 字重与斜体](#311-字重与斜体)
    - [3.1.2 下划线与颜色](#312-下划线与颜色)
  - [3.2 文本对齐](#32-文本对齐)
  - [3.3 行高与段落](#33-行高与段落)
- [第四章 表格](#第四章-表格)
  - [4.1 基本表格](#41-基本表格)
  - [4.2 表格对齐方式](#42-表格对齐方式)
  - [4.3 合并单元格](#43-合并单元格)
  - [4.4 跨页表格](#44-跨页表格)
- [第五章 图片与 SVG](#第五章-图片与-svg)
  - [5.1 位图图片](#51-位图图片)
  - [5.2 SVG 矢量图](#52-svg-矢量图)
  - [5.3 嵌入 SVG](#53-嵌入-svg)
  - [5.4 SVG 渐变](#54-svg-渐变)
    - [5.4.1 线性渐变](#541-线性渐变)
    - [5.4.2 径向渐变](#542-径向渐变)
  - [5.5 路径与图形](#55-路径与图形)
- [第六章 页面布局](#第六章-页面布局)
  - [6.1 强制分页](#61-强制分页)
  - [6.2 满页图片](#62-满页图片)
  - [6.3 容器与背景](#63-容器与背景)
- [第七章 高级功能](#第七章-高级功能)
  - [7.1 超链接](#71-超链接)
  - [7.2 水平线](#72-水平线)
  - [7.3 页眉与页脚](#73-页眉与页脚)
  - [7.4 目录生成](#74-目录生成)
  - [7.5 封面与扉页](#75-封面与扉页)
- [第八章 API 参考](#第八章-api-参考)
  - [8.1 FPDF 类](#81-fpdf-类)
  - [8.2 完整示例代码](#82-完整示例代码)
- [第九章 常见问题](#第九章-常见问题)
  - [9.1 文字相关](#91-文字相关)
  - [9.2 图片相关](#92-图片相关)
  - [9.3 分页相关](#93-分页相关)
  - [9.4 其他问题](#94-其他问题)
- [项目文件结构](#项目文件结构)

---

## 第一章 简介

### 1.1 什么是 webpdf

webpdf 是一个轻量级的 HTML 转 PDF 库，使用 C++ 开发，支持将 HTML 内容渲染为高质量的 PDF 文档。它不需要依赖浏览器内核，而是直接解析 HTML 和 CSS 并生成 PDF 指令，因此启动速度快、内存占用低。

本库特别适合以下场景：
- 服务器端批量生成 PDF 报表和文档
- 嵌入式系统中的 PDF 生成
- 需要精确控制 PDF 输出的应用
- 对启动速度和内存有严格要求的环境

### 1.2 主要特性

| 特性 | 说明 |
|------|------|
| HTML 解析 | 支持常用 HTML 标签和 CSS 样式 |
| 中文支持 | 完整支持 TrueType 中文字体 |
| SVG 渲染 | 支持矢量图形和渐变填充 |
| 自动分页 | 智能分页，表格跨页自动重复表头 |
| 页眉页脚 | 支持自定义页眉页脚，可插入页码 |
| 目录生成 | 自动生成带页码的目录，支持点击跳转 |
| 满页图片 | 支持 class="pagefull" 满页渲染 |

> **注意：** webpdf 不是浏览器内核，而是一个专用的 HTML/CSS 渲染引擎，因此只支持常用的标签和样式。对于复杂的 Web 应用，请使用基于浏览器的方案。

---

## 第二章 快速开始

### 2.1 编译项目

项目使用 Makefile 构建系统，确保系统已安装 g++ 和 make。在项目根目录下执行：

```bash
cd webpdf
make
```

编译成功后，会生成 `test_html` 可执行文件。

### 2.2 运行测试

运行测试程序，默认读取 `content.html` 并输出 `output.pdf`：

```bash
./test_html
```

也可以指定输入和输出文件：

```bash
./test_html input.html output.pdf
```

### 2.3 最简单的示例

下面是一个最基本的 HTML 文件，只包含标题和一段文字：

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
    <p>这是我的第一个 PDF 文档。</p>
</body>
</html>
```

> **提示：** 确保 HTML 文件使用 UTF-8 编码，否则中文可能显示乱码。

---

## 第三章 文本样式

### 3.1 字体与字号

webpdf 支持通过 CSS 设置字体和字号。在使用字体前，必须先通过 C++ API 的 `AddFont()` 方法注册字体文件。

示例：

```css
body {
    font-family: "Microsoft YaHei";
    font-size: 24px;
}
```

#### 3.1.1 字重与斜体

可以使用 `<b>` 或 `<strong>` 标签来表示**加粗文本**，使用 `<i>` 或 `<em>` 标签来表示*斜体文本*。当然，也可以同时使用 ***加粗斜体***。

#### 3.1.2 下划线与颜色

使用 `<u>` 标签可以添加<u>下划线</u>。使用 `<font color="...">` 可以设置文字颜色，例如 <font color="#FF0000">红色</font>、<font color="#00AA00">绿色</font>、<font color="#0000FF">蓝色</font>。

### 3.2 文本对齐

- 左对齐：`text-align: left`
- 居中对齐：`text-align: center`
- 右对齐：`text-align: right`

### 3.3 行高与段落

行高（line-height）控制每行文字之间的垂直间距。合适的行高可以提高阅读体验。一般来说，行高设置为字号的 1.2 到 1.5 倍比较合适。

---

## 第四章 表格

### 4.1 基本表格

使用 `<table>` 标签创建表格。添加 `border="1"` 属性显示边框。

```html
<table border="1" style="width: 100%;">
    <thead>
        <tr>
            <th>产品名称</th>
            <th>价格</th>
            <th>库存</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>Web 开发框架</td>
            <td>¥299</td>
            <td>128</td>
        </tr>
        <tr>
            <td>移动应用组件库</td>
            <td>¥499</td>
            <td>56</td>
        </tr>
        <tr>
            <td>企业级管理系统</td>
            <td>¥1999</td>
            <td>12</td>
        </tr>
    </tbody>
</table>
```

### 4.2 表格对齐方式

单元格内容可以设置不同的水平对齐方式和垂直对齐方式：

| 对齐方式 | 属性值 |
|----------|--------|
| 水平左对齐 | `text-align: left` |
| 水平居中 | `text-align: center` |
| 水平右对齐 | `text-align: right` |
| 垂直顶部 | `vertical-align: top` |
| 垂直居中 | `vertical-align: middle` |
| 垂直底部 | `vertical-align: bottom` |

### 4.3 合并单元格

使用 `colspan` 合并列，使用 `rowspan` 合并行。

```html
<table border="1" style="width: 100%;">
    <thead>
        <tr>
            <th colspan="2">产品信息</th>
            <th>价格详情</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan="3">软件产品</td>
            <td>基础版</td>
            <td>¥99</td>
        </tr>
        <tr>
            <td>专业版</td>
            <td>¥299</td>
        </tr>
        <tr>
            <td>企业版</td>
            <td>¥999</td>
        </tr>
    </tbody>
</table>
```

### 4.4 跨页表格

当表格内容较多需要跨页显示时，可以添加 `style="position: fixed"`，这样表头会在每一页自动重复显示，方便阅读。

```html
<table border="1" style="width: 100%; position: fixed;">
    <thead>
        <tr>
            <th>功能模块</th>
            <th>基础版</th>
            <th>专业版</th>
        </tr>
    </thead>
    <tfoot>
        <tr>
            <td style="text-align: right;">合计功能数</td>
            <td>12 项</td>
            <td>28 项</td>
        </tr>
    </tfoot>
    <tbody>
        <tr>
            <td>用户管理</td>
            <td>✓ 支持</td>
            <td>✓ 支持</td>
        </tr>
    </tbody>
</table>
```

---

## 第五章 图片与 SVG

### 5.1 位图图片

webpdf 支持 PNG、JPG 等常见位图格式。使用 `<img>` 标签插入图片：

```html
<img src="images/cover.jpg">
```

### 5.2 SVG 矢量图

SVG（可缩放矢量图形）可以无损缩放，适合图表和图标。可以通过 `<img>` 引用 SVG 文件，也可以直接在 HTML 中嵌入 SVG 代码。

```html
<img src="images/diagram.svg">
```

### 5.3 嵌入 SVG

在 HTML 中直接嵌入 SVG 代码，可以获得更灵活的控制：

```html
<svg width="300" height="200" xmlns="http://www.w3.org/2000/svg">
    <rect x="10" y="10" width="280" height="180" fill="#f8f9fa" stroke="#dee2e6" stroke-width="2" rx="8"/>
    <rect x="30" y="50" width="60" height="120" fill="#4285f4"/>
    <rect x="110" y="80" width="60" height="90" fill="#ea4335"/>
    <rect x="190" y="30" width="60" height="140" fill="#fbbc05"/>
    <text x="60" y="185" font-size="12" text-anchor="middle">一月</text>
    <text x="140" y="185" font-size="12" text-anchor="middle">二月</text>
    <text x="220" y="185" font-size="12" text-anchor="middle">三月</text>
    <text x="150" y="35" font-size="14" text-anchor="middle">季度销售柱状图</text>
</svg>
```

### 5.4 SVG 渐变

webpdf 支持线性渐变和径向渐变两种 SVG 渐变效果。

#### 5.4.1 线性渐变

线性渐变沿着一条直线方向逐渐变化颜色：

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

#### 5.4.2 径向渐变

径向渐变从中心点向外扩散，适合绘制球体和光晕效果：

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

### 5.5 路径与图形

SVG 支持多种基本图形和路径绘制：

| 元素 | 说明 |
|------|------|
| `<line>` | 直线 |
| `<polyline>` | 折线 |
| `<polygon>` | 多边形 |
| `<path>` | 路径 |
| `<ellipse>` | 椭圆 |

---

## 第六章 页面布局

### 6.1 强制分页

有时需要在特定位置强制分页，可以使用 `page-break-before: always` 样式：

```html
<div style="page-break-before: always;"></div>
```

上面这个空的 div 不会产生任何可见内容，只会触发一次分页。

### 6.2 满页图片

使用 `class="pagefull"` 可以让图片占满整个页面，不显示页眉、页脚和边距。图片会按比例缩放并居中显示。

```html
<p class="pagefull"><img src="images/fullpage.png"></p>
```

> **小技巧：** pagefull 类可以加在任何容器元素上，容器内的第一个 img 元素会被用于满页渲染，容器本身和其他子元素不会显示。

### 6.3 容器与背景

使用 `<div>` 可以创建块级容器，设置背景色、边框、圆角等样式：

```html
<div style="background-color: #e3f2fd; padding: 20px; border-radius: 10px; border: 1px solid #2196f3;">
    <strong style="color: #1565c0;">信息提示框</strong>
    <p>这是一个蓝色主题的提示框。</p>
</div>
```

---

## 第七章 高级功能

### 7.1 超链接

使用 `<a>` 标签可以创建超链接，在 PDF 中点击可以跳转到指定网页。

```html
<a href="https://www.example.com">示例网站</a>
```

长链接也能正确换行显示。

### 7.2 水平线

使用 `<hr>` 标签可以插入水平线，用于分隔内容：

```html
<hr>
<hr style="border-color: #f44336; border-width: 3px;">
<hr style="border-color: #4caf50; border-style: dashed; border-width: 2px;">
<hr style="border-color: #2196f3; border-style: dotted; border-width: 2px; width: 50%; margin: 0 auto;">
```

### 7.3 页眉与页脚

使用 `class="pageheader"` 和 `class="pagefooter"` 可以自定义每一页的页眉和页脚。

```html
<div class="pageheader" style="height: 30px;">
    <div style="text-align: right; margin-right: 100px;">
        <img src="images/logo.svg" style="padding-right: 20px;">
        <span>第{pagenum}页 / 共{pagetotal}页</span>
    </div>
</div>

<div class="pagefooter" style="height: 20px;">
    <div style="text-align:center;">— 第{pagenum}页 / 共{pagetotal}页 —</div>
</div>
```

页眉页脚中可以使用两个特殊占位符：

| 占位符 | 说明 |
|--------|------|
| `{pagenum}` | 当前页码 |
| `{pagetotal}` | 文档总页数 |

> **注意：** 封面页和目录页默认不显示页眉页脚。满页图片（pagefull）的页面也不显示页眉页脚。

### 7.4 目录生成

webpdf 可以自动生成目录。给标题添加 `class="catalogue1"`、`catalogue2`、`catalogue3` 即可自动加入目录。

| 类名 | 目录级别 | 对应标题 |
|------|----------|----------|
| `catalogue1` | 一级目录 | h1 ~ h3 |
| `catalogue2` | 二级目录 | h4 ~ h5 |
| `catalogue3` | 三级目录 | h5 ~ h6 |

生成的目录带有页码，并且可以点击跳转到对应章节。

### 7.5 封面与扉页

使用 `class="pagecover"` 可以创建封面页，封面页不显示页眉页脚，内容从页面顶部开始。

使用 `class="pagecatalogue"` 可以创建目录页背景，配合 `class="cataloguecontent"` 自动生成目录内容。

```html
<!-- 封面 -->
<div class="pagecover" style="background-image:url(images/cover.jpg);">
    <h1 style="padding-top: 260px; text-align: center;">文档标题</h1>
</div>

<!-- 目录页背景 -->
<div class="pagecatalogue" style="background-image:url(images/mulu.jpeg);">
    <h3 style="padding-top: 200px; text-align: center;">目录</h3>
</div>

<!-- 自动目录内容 -->
<div class="cataloguecontent" title="目录"></div>
```

---

## 第八章 API 参考

### 8.1 FPDF 类

FPDF 是 PDF 文档的核心类，用于创建和操作 PDF 文件。

| 方法 | 说明 |
|------|------|
| `FPDF()` | 构造函数，创建一个新的 PDF 文档 |
| `AddPage()` | 添加一页新页面 |
| `AddFont(family, style, file, dir)` | 注册字体文件 |
| `SetFont(family, style, size)` | 设置当前字体 |
| `WriteHTML(html, line_height)` | 渲染 HTML 内容到 PDF |
| `Output(dest, name)` | 输出 PDF 文件 |
| `SetShowHeader(show)` | 设置是否显示页眉 |
| `SetShowFooter(show)` | 设置是否显示页脚 |
| `SetShowHeaderFooter(show)` | 同时设置页眉页脚显示 |
| `PageNo()` | 获取当前页码 |
| `SetPageNumberOffset(offset)` | 设置页码偏移量 |
| `SetLeftMargin(margin)` | 设置左边距 |
| `SetRightMargin(margin)` | 设置右边距 |
| `SetTopMargin(margin)` | 设置上边距 |
| `SetAutoPageBreak(auto, margin)` | 设置自动分页 |
| `GetPageWidth()` | 获取页面宽度 |
| `GetPageHeight()` | 获取页面高度 |
| `GetX() / GetY()` | 获取当前坐标 |
| `SetXY(x, y)` | 设置当前坐标 |
| `Image(file, x, y, w, h)` | 插入图片 |
| `Rect(x, y, w, h, style)` | 绘制矩形 |
| `SetFillColor(r, g, b)` | 设置填充颜色 |
| `SetDrawColor(r, g, b)` | 设置描边颜色 |
| `SetTextColor(r, g, b)` | 设置文字颜色 |
| `SetLineWidth(width)` | 设置线宽 |
| `AddAxialShading(...)` | 添加轴向渐变 |
| `ShadeFill(x, y, w, h, id)` | 使用渐变填充矩形 |

### 8.2 完整示例代码

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

## 第九章 常见问题

### 9.1 文字相关

| 问题 | 解决方案 |
|------|----------|
| 中文显示乱码或方框 | 确保已使用 AddFont() 注册中文字体，且 CSS 中 font-family 名称匹配 |
| 字体粗细不起作用 | 需要注册对应字重的字体文件，webpdf 不会自动模拟加粗 |
| 文字换行不正确 | 检查是否有特殊字符或空格，中文按字符宽度换行 |

### 9.2 图片相关

| 问题 | 解决方案 |
|------|----------|
| 图片不显示 | 检查图片路径是否正确，路径相对于程序运行目录 |
| SVG 显示为黑色 | 检查 SVG 语法是否正确，渐变需要在 defs 中定义 |
| SVG 渐变显示不正确 | 确保渐变 ID 正确，使用 url(#id) 格式引用 |

### 9.3 分页相关

| 问题 | 解决方案 |
|------|----------|
| pagebreak 前有空白页 | 使用空 div + page-break-before: always，不要在里面放内容 |
| 表格跨页表头不重复 | 添加 style="position: fixed" 到 table 标签 |
| pagefull 图片有页眉页脚 | 确保 class="pagefull" 加在容器上，且包含 img 子元素 |

### 9.4 其他问题

| 问题 | 解决方案 |
|------|----------|
| 目录页码不对 | 目录页码是估算的，复杂布局可能有偏差 |
| HTML 中某些样式不生效 | webpdf 只支持常用 CSS 属性，查看教程中的支持列表 |
| 生成的 PDF 文件很大 | 使用压缩的图片，减少大尺寸位图的使用 |

---

## 项目文件结构

```
webpdf/
├── .github/workflows/      # CI/CD 工作流配置
│   ├── ci.yml
│   └── release.yml
├── cmake/                  # CMake 配置模板
│   ├── webpdf.pc.in
│   └── webpdfConfig.cmake.in
├── example/                # 示例程序和教程
│   ├── font/               # 示例字体文件
│   │   ├── AlibabaPuHuiTi-Light.otf/ttf
│   │   ├── AlibabaSans-Regular.otf/ttf
│   │   └── *.json (字体配置)
│   ├── images/             # 示例图片
│   │   ├── cover.jpg
│   │   ├── mulu.jpeg
│   │   └── *.png/*.svg
│   ├── hello.cpp           # 最小示例
│   ├── render_html.cpp     # HTML 渲染示例
│   ├── webpdf_tutorial.html      # 中文教程
│   ├── webpdf_tutorial_en.html   # 英文教程
│   └── TUTORIAL.md         # Markdown 教程
├── include/webpdf/         # 公开头文件
│   ├── webpdf.h            # 主 API 头文件
│   ├── parsecss.h          # CSS 解析器
│   ├── parsehtml.h         # HTML 解析器
│   ├── parsesvg.h          # SVG 解析器
│   ├── ttfparser.h         # TrueType 字体解析器
│   ├── gzip.h              # GZIP 压缩
│   └── request.h           # HTTP 请求工具
├── ports/webpdf/           # vcpkg 端口配置
│   ├── portfile.cmake
│   └── vcpkg.json
├── src/                    # 源代码文件
│   ├── webpdf.cpp
│   ├── parsecss.cpp
│   ├── parsehtml.cpp
│   ├── parsesvg.cpp
│   ├── ttfparser.cpp
│   ├── gzip.cpp
│   └── request.cpp
├── CMakeLists.txt          # CMake 构建配置
├── LICENSE                 # MIT 许可证
├── Makefile                # Make 构建系统
├── README.md               # 英文 README
├── README_CN.md            # 中文 README
├── vcpkg.json              # vcpkg 清单
├── webpdf.pc               # pkg-config 文件
├── webpdf.rb               # Homebrew 配方
└── brew.sh                 # Brew 安装脚本
```

**写在最后：** webpdf 是一个持续发展的项目，托管于 [https://github.com/hggq/webpdf](https://github.com/hggq/webpdf)。如果你遇到了问题或需要新功能，欢迎反馈和贡献代码。希望本教程能帮助你快速上手，制作出精美的 PDF 文档！