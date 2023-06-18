# hues
![version](https://img.shields.io/badge/version-0.1.0-green)
![C](https://img.shields.io/badge/language-C-blue)
![License](https://img.shields.io/badge/license-MIT-green)

*hues* is a highly flexible, fully customizable logging library for the C language. It was designed to provide a straightforward, easy-to-use interface for developers who want a customizable way to handle their logging. With hues, you can configure different logging levels, output formats, and more.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Customization](#customization)
- [Contributing](#contributing)
- [License](#license)

## Features
- **Log Levels:** Supports multiple logging levels such as TRACE, WARN, INFO, DEBUG, SEVERE, CRITICAL...
- **Custom Output:** You can customize the output format to suit your needs.
- **Thread-Safe:** Implemented to be safe for use in multi-threaded applications.
- **File & Console Logging:** Supports both console and file logging. (**COMING SOON!**)
- **Lightweight:** Minimal footprint on system resources.

## Installation

1. Clone this repository
```bash
git clone https://github.com/skanderjeddi/hues.git
```
2. Navigate into the cloned directory and compile the library
```bash
cd hues
make
```

3. Install the library
```bash
sudo make install
```

## Usage
```c
#include <hues.h>

int main() {
    // Initialize the logger
    hues_initialize();

    // Use the logger
    trace("This is an error message");
    debug("This is a warning message");
    warn("This is an informational message");
    info("This is a debug message");

    return 0;
}
```

## Customization
*hues* allows you to customize its behavior to suit your needs. Here are a few examples of what you can customize:

1. **Changing the log level:**
```c
hues_configuration_set_minimum_level(HUES_LEVEL_INFO);
```

2. **Changing the output format:**
```c
**TODO**
```

3. **Changing the output destination:**
**COMING SOON!**

## Contributing
We appreciate any contribution to hues. Please review the [CONTRIBUTING.md](CONTRIBUTING.md) for more details on how to contribute to this project.

## License
hues is licensed under the terms of the MIT License. See [LICENSE](LICENSE) for more details.

---

**Thank you for using hues!**
