# Edator

## Overview
This is a simple text editor written in C. It provides basic functionalities to create, edit, and save text files.

## Features
- Open and edit existing text files
- Create new text files
- Save changes to text files
- Basic text navigation and editing commands

## Installation
1. Clone the repository:
    ```sh
    git clone https://github.com/Eda29/Edator.git
    ```
2. Navigate to the project directory:
    ```sh
    cd Edator
    ```
3. Compile the program:
    ```sh
    mkdir Release && cd Release
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make
    ```

## Usage
Run the text editor with the following command:
```sh
./Edator [filename]
```
- If `[filename]` is provided, the editor will open that file.
- If no filename is provided, a new file will be created.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact
For any questions or suggestions, please open an issue or contact me at [edatosho@gmail.com].
