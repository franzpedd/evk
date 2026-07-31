# About

This directory contains example projects demonstrating how to use the API. Each example is self-contained and focuses on a specific aspect of the API.

## Pre-requisites

- C compiler (Clang, GCC, or MSVC);
- EVK library API;
- Platform-specific dependencies:
  - **Windows**: user32.lib;

## Building Examples

### hellosprite_win32.c / hellosprite_x11.c
Demonstrates creating and rendering a static sprite.

**Application result:**
<table align="center">
    <tr>
        <td align="center" width="400">
            <img src="docs/hellosprite_win32.jpg" alt="Image 1" width="400" height="300"/>
            <br>Windows</br>
        </td>
        <td align="center" width="400">
            <img src="docs/hellosprite_x11.jpg" alt="Image 2" width="400" height="300"/>
            <br>Linux (X11)</br>
        </td>
    </tr>
</table>