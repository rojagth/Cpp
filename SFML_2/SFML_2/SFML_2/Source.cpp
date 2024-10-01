// Program for solving sudoku; The GUI contains "Solve" button that solves provided sudoku grid; GUI uses font, where a proper path needs to be definied


// Libraries
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class SudokuGrid {
private:
    // Define 2D grid 
    std::vector<std::vector<int>> grid;

    // Function to check if a number is valid in a given position
    bool is_valid(int num, int row, int col) {
        // Check row
        for (int i = 0; i < 9; i++) {
            if (grid[row][i] == num) {
                return false;
            }
        }
        // Check column
        for (int i = 0; i < 9; i++) {
            if (grid[i][col] == num) {
                return false;
            }
        }
        // Check box
        int box_row = (row / 3) * 3;
        int box_col = (col / 3) * 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[box_row + i][box_col + j] == num) {
                    return false;
                }
            }
        }
        return true;
    }

    // Find the next empty cell
    std::pair<int, int> find_empty_cell() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (grid[i][j] == 0) {
                    return { i, j };
                }
            }
        }
        return { -1, -1 };
    }

public:
    SudokuGrid(std::vector<std::vector<int>> initial_numbers) : grid(initial_numbers) {}

    // Get the value in a particular cell of the grid
    int get(int i, int j) {
        return grid[i][j];
    }

    // Set the value in a particular cell of the grid
    void set(int i, int j, int value) {
        grid[i][j] = value;
    }

    // Solve the sodoku board
    bool solve() {
        // Call the helper function to solve the puzzle and return a boolean value indicating whether or not a valid solution was found
        return solve_helper();
    }

    // Helper function to solve the Sudoku puzzle recursively
    bool solve_helper() {
        auto empty_cell = find_empty_cell();
        // If there are no empty cells, the puzzle is solved
        if (empty_cell.first == -1 && empty_cell.second == -1) {
            return true;
        }
        int row = empty_cell.first;
        int col = empty_cell.second;
        // Try each possible value for the empty cell
        for (int num = 1; num <= 9; num++) {
            if (is_valid(num, row, col)) {
                grid[row][col] = num;
                // Recursively solve the puzzle with the new number in place
                if (solve_helper()) {
                    return true;
                }
                // If the new number leads to an unsolvable puzzle, backtrack and try a different number
                grid[row][col] = 0;
            }
        }
        // If no valid number can be placed in the empty cell, backtrack and try a different value for the previous empty cell
        return false;
    }
};

class SudokuWindow {
private:
    sf::RenderWindow window;
    // Create a SudokuGrid object (member variables)
    SudokuGrid sudoku;
    sf::RectangleShape grid_lines[20];
    sf::Text numbers[9][9];
    sf::Font font;
    sf::RectangleShape solve_button;
    sf::Text solve_text;
 
public:
    // Constuctor
    SudokuWindow(std::vector<std::vector<int>> initial_numbers) :
        // Initializer list to initialize the member variables of the class
        sudoku(initial_numbers),
        window(sf::VideoMode(540, 600), "Sudoku Solver", sf::Style::Close),
        font()
    {
        // Load the font for the numbers
        if (!font.loadFromFile("./Fonts/arial.ttf")) {
            std::cerr << "Failed to load font" << std::endl;
            return;
        }

        // Initialize the grid lines
        for (int i = 0; i < 10; i++) {
            grid_lines[i] = sf::RectangleShape(sf::Vector2f(540, i % 3 == 0 ? 6 : 2));
            grid_lines[i].setPosition(0, i * 60);
            grid_lines[i].setFillColor(sf::Color::Black);
        }
        for (int i = 0; i < 10; i++) {
            grid_lines[i + 10] = sf::RectangleShape(sf::Vector2f(i % 3 == 0 ? 6 : 2, 540));
            grid_lines[i + 10].setPosition(i * 60, 0);
            grid_lines[i + 10].setFillColor(sf::Color::Black);
        }

        // Initialize the numbers
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                numbers[i][j] = sf::Text(std::to_string(sudoku.get(i, j)), font, 40);
                numbers[i][j].setPosition(20 + j * 60, 10 + i * 60);
                if (sudoku.get(i, j) == 0) {
                    numbers[i][j].setString("");
                }
                else {
                    if (initial_numbers[i][j] != 0) { 
                        numbers[i][j].setFillColor(sf::Color(173, 216, 230));
                    }
                    else {
                        numbers[i][j].setFillColor(sf::Color::Black);
                    }
                }
            }
        }

        // Initialize the solve button
        solve_button = sf::RectangleShape(sf::Vector2f(100, 40));
        solve_button.setPosition(220, 552);
        solve_button.setFillColor(sf::Color(173, 216, 230));
        solve_button.setOutlineColor(sf::Color::Black);
        solve_button.setOutlineThickness(3);

        solve_text = sf::Text("Solve", font, 30);
        solve_text.setPosition(232, 552);
        solve_text.setFillColor(sf::Color::White);
    }

    // Function that converts numbers into string
    void update_numbers() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                numbers[i][j].setString(std::to_string(sudoku.get(i, j)));
                if (sudoku.get(i, j) == 0) {
                    numbers[i][j].setString("");
                }
            }
        }
    }

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            // pollEvent(event) function retrieves the next event from the event queue and updates the event object with the details of that event
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear(sf::Color::White);
            for (int i = 0; i < 20; i++) {
                window.draw(grid_lines[i]);
            }
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    window.draw(numbers[i][j]);
                }
            }
            window.draw(solve_button);
            window.draw(solve_text);
            window.display();

            // Handle solve button click
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
                if (solve_button.getGlobalBounds().contains(mouse_position.x, mouse_position.y)) {
                    if (sudoku.solve()) {
                        // Update the numbers on the grid with the solved values
                        for (int i = 0; i < 9; i++) {
                            for (int j = 0; j < 9; j++) {
                                numbers[i][j].setString(std::to_string(sudoku.get(i, j)));
                                numbers[i][j].setFillColor(sf::Color::Black);
                            }
                        }
                    }
                    else {
                        std::cout << "Unable to solve puzzle" << std::endl;
                    }
                }
            }
        }
    }
};

int main() {
    std::vector<std::vector<int>> initial_numbers = {
        {0, 7, 5, 0, 9, 0, 0, 0, 6},
        {0, 2, 3, 0, 8, 0, 0, 4, 0},
        {8, 0, 0, 0, 0, 3, 0, 0, 1},
        {5, 0, 0, 7, 0, 2, 0, 0, 0},
        {0, 4, 0, 8, 0, 6, 0, 2, 0},
        {0, 0, 0, 9, 0, 1, 0, 0, 3},
        {9, 0, 0, 4, 0, 0, 0, 0, 7},
        {0, 6, 0, 0, 7, 0, 5, 8, 0},
        {7, 0, 0, 0, 1, 0, 3, 9, 0}
    };

    SudokuWindow window(initial_numbers);
    window.run();

    // Successful execution
    return 0;
}