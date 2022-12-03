#include <iostream>
#include <array>
#include <chrono>

/* notes :
        Width to 3000 tested.
        Height 80 or more seems to be the same */


// CUSTOM COMPILE-TIME VARIABLES TO SET
///////////////////////////////////////////////////////
constexpr inline int MAX_WIDTH = 100;
constexpr inline int MAX_HEIGHT = 300;
///////////////////////////////////////////////////////

// with order:  25540ms
// omptiimized  12900ms

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

// simulates knight in position, with certain width and height.
// 'check_if_went_above' : false => it should finish when halted, true => it should finish when going above start.
// 'draw' makes it draw each movement of the knight
template <bool check_if_went_above = false>
bool halts(const int start_i, const int start_j, const int width, const int height, const bool draw)
{
    bool return_val = !check_if_went_above;
    int i = start_i, j = start_j;
    while (true) {
        matrix[i][j] = true;

        if (draw) {
            draw_blocks(width, height);
            std::cin.get();
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
            
            break; // if code reaches this point, no paths were available => therefore, exit loop and return TRUE; it halted
        
        } GOTO_AFTER_MOVE:

        if constexpr (check_if_went_above) {
            if (i > start_i) {
                return_val = true;
                break;
            }
        }
        else {
            if (i > top) {
                return_val = false;
                break;
            }
        }
    }
    for (int i = 0; i < height; ++i) { // clear just the cells that could have been filled
        for (int j = 0; j < width; ++j) {
            matrix[i][j] = false;
        }
    }
   
    return return_val;
}

// order is an array of i and j changes
bool halts_with_order(const int start_i, const int start_j, const int width, const int height, const bool draw, const std::array<const std::array<const int, 2>, 8> order)
{
    bool return_val = true;
    int i = start_i, j = start_j;
    while (true)
    {
        matrix[i][j] = true;

        if (draw) {
            draw_blocks(width, height);
            std::cin.get();
        }
        
        // try moves
        for (auto move : order) {
            int i_t = i + move[0], j_t = j + move[1];
            if (i_t >= 0 && j_t >= 0 && j_t < width && !matrix[i_t][j_t])
            {
                // valid move
                i = i_t;
                j = j_t;
                goto GOTO_VALID_MOVE;
            }
        }

        // if code arrives here -> it's halted
        break;
        
    GOTO_VALID_MOVE:
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

// in a certain width and height, how many knights halt?
int how_many_not_halt_order(const int width, const int height, const bool draw, const bool say_pos_of_not_halt)
{
    int not_halts = 0;

    // order rotated like 180, always goes to infinity
    //const std::array<const std::array<const int, 2>, 8> order = { { {-1, -2}, {1, -2}, {-2, -1}, {2, -1}, {-2, 1}, {2, 1}, {-1, 2}, {1, 2} } };

    // normal
    //const std::array<const std::array<const int, 2>, 8> order = { { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1} } };

    // circular
    //const std::array<const std::array<const int, 2>, 8> order = { { {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2} } };
    //const std::array<const std::array<const int, 2>, 8> order = { { {-2, -1} , {-1, -2} , {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2} } };
    const std::array<const std::array<const int, 2>, 8> order = { { {-2, -1} , {-2, 1}, {-2, 2}, {-1, 2}, {2, -1},{1, 4},{2, -1},{1, -1}  } };
    for (int i = top; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            if (halts_with_order(i, j, width, height, false, order)) {
                if (draw) std::cout << "H ";
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


// in a certain width and height, how many knights halt?
int how_many_not_halt(const int width, const int height, const bool draw, const bool say_pos_of_not_halt) 
{
    int not_halts = 0;
    for (int i = top; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            if (halts(i, j, width, height, false)) {
                if (draw) std::cout << "H ";
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

// for a certain width and height, gives how many went above their start position
int how_many_went_above(const int width, const int height, const bool draw)
{
    int nr_went_above = 0;
    int highest_that_went_above = -1;
    for (int i = top; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            if (halts<true>(i, j, width, height, false)) {
                if (i > highest_that_went_above) { highest_that_went_above = i; }
                ++nr_went_above;
                if (draw) std::cout << "|";
            }
            else {
                if (draw) std::cout << 'H';
            }
        }
        if (draw) std::cout << '\n';
    }
    std::cout << "In width " << width << " the highest that went above is: " << highest_that_went_above << '\n';
    return nr_went_above;
}

// prints all forever-escaping knights for each width
void test_widths_for_halts()
{
    for (int width = 1; width <= MAX_WIDTH; ++width) {
        std::cout << "Width: " << width << '\n';
        int num = how_many_not_halt(width, MAX_HEIGHT, false, true);
    }
}

// computes and prints how many knights went above in all widths
void test_widths_for_went_above()
{
    for (int width = 1; width <= MAX_WIDTH; ++width) {
        int num = how_many_went_above(width, MAX_HEIGHT, false);
        std::cout << "width:" << width << " went above themselves: " << num << '\n';
    }
}

void test_widths_for_halts_order()
{
    for (int width = 1; width <= MAX_WIDTH; ++width) {
        std::cout << "Width: " << width ;
        int num = how_many_not_halt_order(width, MAX_HEIGHT, false, true);
        std::cout << ' ' << num << '\n';
    }
}

int main()
{
    //std::cout << halts(10, 25, 42, 80, 78, true, true);
    //std::cout << "IMPORTANT:" << how_many_not_halt(42, 200, 190, true, true) << "AAAAAAAAAAAAA" << '\n';

    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    // modifiable code
    {
        //how_many_went_above(MAX_WIDTH, MAX_HEIGHT, true);
        //test_widths_for_went_above();
        //test_widths_for_halts();
        //halts_with_order(2, 2, 3, 50, true, { { {-1, -2}, {1, -2}, {-2, -1}, {2, -1}, {-2, 1}, {2, 1}, {-1, 2}, {1, 2} } });
        //how_many_not_halt_order(3, MAX_HEIGHT, true, false);
        test_widths_for_halts_order();
    }

    auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
    std::cout << "\nmilliseconds: " << duration << '\n';
}

