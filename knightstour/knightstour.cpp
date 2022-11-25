#include <iostream>
#include <array>

// to width 1000 tested
// height 80 or more seems to be the same

// CUSTOM COMPILE-TIME VARIABLES TO SET
constexpr inline int MAX_HEIGHT = 80;
constexpr inline int MAX_WIDTH = 100;


// program:

constexpr inline int MAX_TOP = MAX_HEIGHT - 2;

std::array<std::array<bool, MAX_WIDTH>, MAX_HEIGHT> matrix = {false};

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
bool halts(int i, int j, const int width, const int height, const int top, const bool draw, const bool print_if_filled)
{
    bool return_val = true;
    
    while (true) {
        //std::cout << "pos: " << i << ' ' << j << " val: " << j+1 + i*3 << '\n';
        matrix[i][j] = true;

        // print map
        if (draw) {
            draw_blocks(width, height);
            std::cin.get();
        }

        int least_val = INT_MAX;
        int least_i = -1, least_j = -1;

        constexpr const std::array<std::array<int, 2>, 8> changes = { { {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2},{-1, 2}, {1, -2}, {1, 2} } };
        for (const auto& change : changes) {
            int j_t = j + change[0], i_t = i + change[1];
            if (j_t >= 0 && j_t < width && i_t >= 0 && i_t < height && !matrix[i_t][j_t])
            {
                int reaL_val = j_t + 1 + i_t * width;
                if (reaL_val < least_val) {
                    least_val = reaL_val;
                    least_i = i_t;
                    least_j = j_t;
                }
            }
        }
        i = least_i;
        j = least_j;

        if (least_i < 0 || least_j < 0) {
            break;
        }
        if (i > top) {
            return_val = false;
            if (print_if_filled) {
                bool is_filled = true;
                for (int i = 0; i < height / 2; ++i) {
                    for (int j = 0; j < width; ++j) {
                        if (!matrix[i][j]) {
                            is_filled = false;
                        }
                    }
                }
                //draw_blocks(width, height);
                std::cout << "Went infinitely, is filled? " << (is_filled ? "True" : "False") << '\n';
            }
            break;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            matrix[i][j] = false;
        }
    }
   
    return return_val;
}

// in  width and height, how many halt?
int how_many_not_halt(const int width, const int height, const int top, const bool draw, const bool say_pos_of_not_halt) 
{
    int not_halts = 0;
    for (int i = top; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            if (halts(i, j, width, height, top, false, true)) {
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

int main()
{
    //std::cout << halts(10, 25, 42, 200, 190, true);
    //std::cout << "IMPORTANT:" << how_many_not_halt(42, 200, 190, true, true) << "AAAAAAAAAAAAA" << '\n';
    for (int width = 1; width <= MAX_WIDTH; ++width) {
        std::cout << "Width: " << width << '\n';
        int num = how_many_not_halt(width, MAX_HEIGHT, MAX_TOP, false, true);
        //std::cout <<"Total not halted: " << num << '\n';
    }
    
}