#include <iostream>
#include <list>
#include <array>
#include <vector>

#define ADD_OFFSET 4
enum Symbol
{
    ZERO = 0,
    ONE,
    TWO,
    THREE,
    ADD = 4,
    SUB,
    MUL,
    DIV,
    BO,
    BC,
    INV
};

float add(float a, float b)
{
    return a+b;
}

float sub(float a, float b)
{
    return a-b;
}

float mul(float a, float b)
{
    return a*b;
}

float div(float a, float b)
{
    return a/b;
}

float (*func[4])(float, float) = {add,sub,mul,div};

void calcAddSub(std::vector<Symbol>& symbols, std::array<float, 4>& numbers)
{
    bool contains;
    do
    {
        contains = false;
        std::vector<Symbol> symbolsB;

        Symbol lastlast = Symbol::INV;
        Symbol last = Symbol::INV;
        for(auto it = symbols.begin(); it != symbols.end(); ++it)
        {
            Symbol current = *it;
            if((current == Symbol::ADD || current == Symbol::SUB) && !contains)
            {
                contains = true;
                Symbol next = *(++it);
                numbers[(int)last] = func[((int)current) - ADD_OFFSET](numbers[(int)last], numbers[(int)next]);
                continue;
            }
            lastlast = last;
            last = current;
            if(lastlast != Symbol::INV)
                symbolsB.push_back(lastlast);
        }
        if(contains)
        {
            symbolsB.push_back(last);
            symbols = symbolsB;
        }

    }while(contains);
}

void calcMulDiv(std::vector<Symbol>& symbols, std::array<float, 4>& numbers)
{
    bool contains;
    do
    {
        contains = false;
        std::vector<Symbol> symbolsB;

        Symbol lastlast = Symbol::INV;
        Symbol last = Symbol::INV;
        for(auto it = symbols.begin(); it != symbols.end(); ++it)
        {
            Symbol current = *it;
            if((current == Symbol::MUL || current == Symbol::DIV) && !contains)
            {
                contains = true;
                Symbol next = *(++it);
                numbers[(int)last] = func[((int)current) - ADD_OFFSET](numbers[(int)last], numbers[(int)next]);
                continue;
            }
            lastlast = last;
            last = current;
            if(lastlast != Symbol::INV)
                symbolsB.push_back(lastlast);
        }
        if(contains)
        {
            symbolsB.push_back(last);
            symbols = symbolsB;
        }

    }while(contains);

    calcAddSub(symbols, numbers);
}

float calcBoBc(std::vector<Symbol> symbols, std::array<float, 4> numbers)
{
    bool contains = false;
    std::vector<Symbol> symbolsB;

    Symbol last = Symbol::INV;
    for(auto& s : symbols)
    {
        if(s == Symbol::BO)
            contains = true;
        else if(s == Symbol::BC)
        {
            if(last == Symbol::BO)
                contains = false;
            break;
        }
        else if(contains)
            symbolsB.push_back(s);

        last = s;
    }

    if(contains)
        calcMulDiv(symbolsB, numbers);
    else
    {
        calcMulDiv(symbols, numbers);
        return numbers[0];
    }

    contains = false;
    std::vector<Symbol> symbolsC;
    for(auto it = symbols.begin(); it != symbols.end(); ++it)
    {
        Symbol s = *it;

        if(contains)
        {
            if(s == Symbol::BC)
            {
                symbolsC.push_back(symbolsB.front());
                contains = false;
            }
        }
        else
        {
            if(s == Symbol::BO)
                contains = true;
            else
                symbolsC.push_back(s);
        }
    }

    symbols = symbolsC;
    calcMulDiv(symbols, numbers);
    return numbers[0];
}

float calc(float* nums, int* symbols)
{
    float ab = func[symbols[0]](nums[0], nums[1]);
    float abc = func[symbols[1]](ab, nums[2]);
    float abcd = func[symbols[2]](abc, nums[3]);
    return abcd;
}

void getPermutation(std::array<float,4>& nums, int permutationIndex, std::array<float,4>& numsOut)
{
    int a = (permutationIndex / 6) % 4;
    int b = (permutationIndex / 2) % 3;
    int c = (permutationIndex % 2);
    int d = 0;

    auto& dataA = nums;
    float dataB[3];
    float dataC[2];
    float dataD[1];

    int i = 0;
    int l = 0;
    for(i = 0; i < a; ++i, ++l)
        dataB[l] = dataA[i];
    for(i = a+1; i < 4; ++i, ++l)
        dataB[l] = dataA[i];

    i = 0;
    l = 0;
    for(i = 0; i < b; ++i, ++l)
        dataC[l] = dataB[i];
    for(i = b+1; i < 3; ++i, ++l)
        dataC[l] = dataB[i];

    
    i = 0;
    l = 0;
    for(i = 0; i < c; ++i, ++l)
        dataD[l] = dataC[i];
    for(i = c+1; i < 2; ++i, ++l)
        dataD[l] = dataC[i];


    numsOut[0] = dataA[a];
    numsOut[1] = dataB[b];
    numsOut[2] = dataC[c];
    numsOut[3] = dataD[d];
}

int main(int argc, char **argv)
{
    std::array<float, 4> nums;
    std::array<float, 4> numsBuffer;
    int symbolsss[3];
    char chars[6] = {'+','-','*','/','(',')'};

    std::vector<Symbol> symbols(4+3+2);
    std::array<int, 2> b00 = {0,1};
    std::array<int, 2> b0 = {4,8};
    std::array<int, 2> b1 = {2,6};
    std::array<int, 2> b2 = {0,4};
    std::array<int, 2> b3 = {2,8};
    std::array<int, 2> b4 = {0,6};
    std::array<std::array<int, 2>, 6> brackets = {b00, b0, b1, b2, b3, b4};

    if(argc == 5)
    {
        try
        {
            nums[0] = std::atoi(argv[1]);
            nums[1] = std::atoi(argv[2]);
            nums[2] = std::atoi(argv[3]);
            nums[3] = std::atoi(argv[4]);
        }
        catch(const std::exception& e)
        {
            std::cout << "Invalid input";
            return 0;
        }
    }

    getPermutation(nums, 0, numsBuffer);

    float res = 0;
    int it = 0;
    int it2 = 0;
    int it3 = 0;
    while(true)
    {
        int index = 0;
        Symbol sO[3];

        for(auto& s : symbols)
            s = Symbol::INV;

        sO[0] = (Symbol)((it / 16) % 4 + ADD_OFFSET);
        sO[1] = (Symbol)((it /  4) % 4 + ADD_OFFSET);
        sO[2] = (Symbol)((it /  1) % 4 + ADD_OFFSET);

        symbols[brackets[it3][0]] = Symbol::BO;
        symbols[brackets[it3][1]] = Symbol::BC;
        for(auto& s : symbols)
        {
            if(s == Symbol::BO || s == Symbol::BC)
                continue;
            
            if((index % 2) == 0)
                s = (Symbol)(index++/2);
            else
                s = sO[index++/2];
        }

        res = calcBoBc(symbols, numsBuffer);
        if(res > 9.95f && res < 10.05f)
            break;  

        ++it;
        it2 = it / 64;
        it3 = it / (64 * 24);

        if((it % 64) == 0)
            getPermutation(nums, it2, numsBuffer);

        if(it3 == brackets.size())
            break;
    }

    if(res == 10)
        std::cout << "Found: ";
    else
        std::cout << "Not Found: ";


    for(auto& s : symbols)
        switch (s)
        {
        case Symbol::ZERO:
        case Symbol::ONE:
        case Symbol::TWO:
        case Symbol::THREE:
            std::cout << numsBuffer[(int)s];
            break;
        case Symbol::ADD:
        case Symbol::SUB:
        case Symbol::MUL:
        case Symbol::DIV:
            std::cout << chars[(int)s - ADD_OFFSET];
            break;
        case Symbol::BO:
        case Symbol::BC:
            if(symbols[0] == Symbol::BO && symbols[1] == Symbol::BC)
                continue;
            std::cout << chars[(int)s - ADD_OFFSET];
        default:
            break;
        }

    std::cout << std::endl;

    return 0;
}