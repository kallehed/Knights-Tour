#include <iostream>
#include <array>
#include <chrono>

// to width 1000 tested
// height 80 or more seems to be the same

// CUSTOM COMPILE-TIME VARIABLES TO SET
constexpr inline int MAX_HEIGHT = 80;
constexpr inline int MAX_WIDTH = 2000;
constexpr inline bool POSSIBLE_TO_DRAW_BLOCKS = true;
constexpr inline bool CAN_DRAW_HALT = true;

//  14987ms

// better ordering of moves: 

// faster  14455

// program:

#define top (height - 3)

std::array<std::array<bool, MAX_WIDTH>, MAX_HEIGHT> matrix = {false};
//bool matrix[MAX_HEIGHT][MAX_WIDTH] = { false };

void draw_blocks(const int width, const int height)
{
    std::cout << '\n';
    for (int i = height-1; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            if (matrix[i][j])
                std::cout << "X  ";
            else
                std::cout << (j+1 + i*width) << ' ';
        }
        std::cout << '\n';
    }
}

// in position in matrix with width and height
bool halts(int i, int j, const int width, const int height, const bool draw)
{
    bool return_val = true;
    
    while (true) {
        //std::cout << "pos: " << i << ' ' << j << " val: " << j+1 + i*3 << '\n';
        matrix[i][j] = true;

        // print map
        if constexpr (POSSIBLE_TO_DRAW_BLOCKS) {
            if (draw) {
                draw_blocks(width, height);
                std::cin.get();
            }
        }

        { // loop unrolling version
            if (i >= 2) {
                if (j >= 1) {
                    if (!matrix[i - 2][j - 1]) {
                        i = i - 2;
                        j = j - 1;
                        goto GOTO_AFTER_MOVE;
                    }
                }
                if (j + 1 < width) {
                    if (!matrix[i - 2][j + 1]) {
                        i = i - 2;
                        j = j + 1;
                        goto GOTO_AFTER_MOVE;
                    }
                }
            }
            if (i >= 1) {
                if (j >= 2) {
                    if (!matrix[i - 1][j - 2]) {
                        i = i - 1;
                        j = j - 2;
                        goto GOTO_AFTER_MOVE;
                    }
                }
                if (j + 2 < width) {
                    if (!matrix[i - 1][j + 2]) {
                        i = i - 1;
                        j = j + 2;
                        goto GOTO_AFTER_MOVE;
                    }
                }
            }
            if (j >= 2) {
                if (!matrix[i + 1][j - 2]) {
                    i = i + 1;
                    j = j - 2;
                    goto GOTO_AFTER_MOVE;
                }
            }
            if (j + 2 < width) {
                if (!matrix[i + 1][j + 2]) {
                    i = i + 1;
                    j = j + 2;
                    goto GOTO_AFTER_MOVE;
                }
            }
            if (j >= 1) {
                if (!matrix[i + 2][j - 1]) {
                    i = i + 2;
                    j = j - 1;
                    goto GOTO_AFTER_MOVE;
                }
            }
            if (j + 1 < width) {
                if (!matrix[i + 2][j + 1]) {
                    i = i + 2;
                    j = j + 1;
                    goto GOTO_AFTER_MOVE;
                }
            }
            
            break;
        
        } GOTO_AFTER_MOVE:

        if (i > top) {
            return_val = false;
            break;
        }
    }
    for (int i = 0; i < height; ++i) { // clear just the cells that could have been filled
        for (int j = 0; j < width; ++j) {
            matrix[i][j] = false;
        }
    }
   
    return return_val;
}

// says whether position with matrix of certain width and height goes above it's own position or not
int went_above(int i, int j, const int width, const int height)
{
    bool went_above = false;
    int start_i = i;

    while (true) {
        //std::cout << "pos: " << i << ' ' << j << " val: " << j+1 + i*3 << '\n';
        matrix[i][j] = true;

        { // loop unrolling version
            if (i >= 2) {
                if (j >= 1) {
                    if (!matrix[i - 2][j - 1]) {
                        i = i - 2;
                        j = j - 1;
                        goto GOTO_AFTER_MOVE;
                    }
                }
                if (j + 1 < width) {
                    if (!matrix[i - 2][j + 1]) {
                        i = i - 2;
                        j = j + 1;
                        goto GOTO_AFTER_MOVE;
                    }
                }
            }
            if (i >= 1) {
                if (j >= 2) {
                    if (!matrix[i - 1][j - 2]) {
                        i = i - 1;
                        j = j - 2;
                        goto GOTO_AFTER_MOVE;
                    }
                }
                if (j + 2 < width) {
                    if (!matrix[i - 1][j + 2]) {
                        i = i - 1;
                        j = j + 2;
                        goto GOTO_AFTER_MOVE;
                    }
                }
            }
            if (j >= 2) {
                if (!matrix[i + 1][j - 2]) {
                    i = i + 1;
                    j = j - 2;
                    goto GOTO_AFTER_MOVE;
                }
            }
            if (j + 2 < width) {
                if (!matrix[i + 1][j + 2]) {
                    i = i + 1;
                    j = j + 2;
                    goto GOTO_AFTER_MOVE;
                }
            }
            if (j >= 1) {
                if (!matrix[i + 2][j - 1]) {
                    i = i + 2;
                    j = j - 1;
                    goto GOTO_AFTER_MOVE;
                }
            }
            if (j + 1 < width) {
                if (!matrix[i + 2][j + 1]) {
                    i = i + 2;
                    j = j + 1;
                    goto GOTO_AFTER_MOVE;
                }
            }

            break;

        } GOTO_AFTER_MOVE:

        if (i > start_i) {
            went_above = true;
            break;
        }
    }
    for (int i = 0; i < height; ++i) { // clear just the cells that could have been filled
        for (int j = 0; j < width; ++j) {
            matrix[i][j] = false;
        }
    }

    return went_above;
}

// in  width and height, how many halt?
int how_many_not_halt(const int width, const int height, const bool draw, const bool say_pos_of_not_halt) 
{
    int not_halts = 0;
    for (int i = top; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            if (halts(i, j, width, height, false)) {
                if constexpr (CAN_DRAW_HALT) {
                    if (draw) std::cout << "H ";
                }
            }

            else {
                if (draw) std::cout << "| ";
                if (say_pos_of_not_halt) {
                    std::cout << "|x: " << j << "|y: " << i << '\n';
                }
                ++not_halts;
            }
        }
        if (draw) std::cout << '\n';
    }
    return not_halts;
}

int how_many_went_above(const int width, const int height, const bool draw)
{
    int nr_went_above = 0;
    for (int i = top; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            if (went_above(i, j, width, height)) {
                ++nr_went_above;
                if (draw) std::cout << "|";
            }
            else {
                if (draw) std::cout << 'H';
            }
        }
        if (draw) std::cout << '\n';
    }
    ///*if (draw)*/ std::cout << "Width: " << width << ",Total went above : " << nr_went_above << '\n';
    return nr_went_above;
}

void test_widths_for_halts()
{
    for (int width = 1; width <= MAX_WIDTH; ++width) {
        std::cout << "Width: " << width << '\n';
        int num = how_many_not_halt(width, MAX_HEIGHT, false, true);
        //std::cout <<"Total not halted: " << num << '\n';
    }
}

void test_widths_for_went_above()
{
    for (int width = 1; width <= MAX_WIDTH; ++width) {
        int num = how_many_went_above(width, MAX_HEIGHT, false);
        std::cout << num << ',';
    }
}


int main()
{
    //std::cout << halts(10, 25, 42, 80, 78, true, true);
    //std::cout << "IMPORTANT:" << how_many_not_halt(42, 200, 190, true, true) << "AAAAAAAAAAAAA" << '\n';

    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    {
        //how_many_went_above(MAX_WIDTH, MAX_HEIGHT, true);
        test_widths_for_went_above();
        //test_widths_for_halts();
    }

    auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    std::cout << "\nmilliseconds: " << duration << '\n';
}

