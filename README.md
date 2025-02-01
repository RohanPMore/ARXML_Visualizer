# ARXML Visualizer

ARXML Visualizer is a C++ application built with the Qt framework designed to parse and visualize AUTOSAR XML (ARXML) files. This tool aids developers in understanding and navigating complex ARXML structures by providing a user-friendly interface.

## Features

- **ARXML Parsing**: Efficiently parses ARXML files to extract relevant data structures.
- **User Interface**: Utilizes Qt's capabilities to present a clear and intuitive visualization of ARXML content.
- **Modular Design**: Organized into components such as `arxml_parser`, `configeditor`, `moduleeditor`, and `projecttreeview` to ensure maintainability and scalability.

## Installation

To build and run ARXML Visualizer, ensure you have the following prerequisites:

- **C++ Compiler**: Compatible with C++17 or later.
- **Qt Framework**: Version 5.15 or later.
- **CMake**: Version 3.10 or later.

**Steps**:

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/RohanPMore/ARXML_Visualizer.git
   cd ARXML_Visualizer
   ```

2. **Configure the Project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. **Build the Project**:
   ```bash
   make
   ```

4. **Run the Application**:
   ```bash
   ./ARXML_Visualizer
   ```

## Usage

Upon launching the application:

1. **Load an ARXML File**: Use the file menu to open your desired ARXML file.
2. **Navigate the Structure**: The application will display the ARXML content in a structured format, allowing you to expand and collapse nodes for detailed viewing.
3. **Edit Configurations**: Utilize the `configeditor` and `moduleeditor` modules to make modifications as needed.

## Project Structure

```
ARXML_Visualizer/
│-- src/
│   │-- arxml_parser/
│   │-- configeditor/
│   │-- moduleeditor/
│   │-- projecttreeview/
│   │-- main.cpp
│
│-- include/
│   │-- arxml_parser.h
│   │-- configeditor.h
│   │-- moduleeditor.h
│   │-- projecttreeview.h
│
│-- assets/
│   │-- icons/
│   │-- styles/
│
│-- build/
│-- CMakeLists.txt
│-- README.md
│-- LICENSE
```

## Contributing

Contributions are welcome! If you have suggestions for improvements or encounter any issues, please open an issue or submit a pull request.
