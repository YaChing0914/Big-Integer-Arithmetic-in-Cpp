#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class HugeInt {
private:
    static const int SIZE = 100;
    int integer[SIZE]; //用陣列來儲存每一個數字
    bool isNegative; //判斷是否為負數

public:
    HugeInt() : isNegative(false) 
    { //預設不是負數
        for (int i = 0; i < SIZE; ++i)
            integer[i] = 0; //初始化所有數字為0
    }

    HugeInt(const string &num) 
    {
        for (int i = 0; i < SIZE; i++)
            integer[i] = 0;
        isNegative = (num[0] == '-'); //若第一個字元是 '-'，表示是負數

        int start = isNegative ? 1 : 0; //若有負號，跳過開頭的 '-' 字元
        int len = num.length() - start; //除掉 '-' 之後的數字長度

        for (int i = 0; i < len; i++) //len是輸入字串num的長度
            integer[SIZE - len + i] = num[start + i] - '0'; //把每一位數儲存在 integer[SIZE] 裡，靠右對齊
                                                            //將字元轉成數字
    }

    bool isZero() const 
    {
        for (int i = 0; i < SIZE; ++i)
            if (integer[i] != 0) return false;
        return true;
    }

    HugeInt operator-() const //可以對 HugeInt 使用負號
    {
        HugeInt temp = *this;
        temp.isNegative = !isNegative;
        return temp;
    }

    bool operator<(const HugeInt &num) const //可以直接使用 < 比大小
    {
        if (isNegative != num.isNegative) return isNegative; //如果兩個數字的正負號不同，可以直接用符號判斷大小
        bool absLess = absLessThan(*this, num); //如果兩個都是正數，或兩個都是負數，就比較絕對值大小
        return isNegative ? !absLess : absLess; //如果是負數 → 結果要反過來
    }

    static bool absLessThan(const HugeInt &a, const HugeInt &b) 
    {
        for (int i = 0; i < SIZE; ++i) 
        {
            if (a.integer[i] < b.integer[i]) return true;
            if (a.integer[i] > b.integer[i]) return false;
        }
        return false;
    }


//大數加法
    HugeInt operator+(const HugeInt &num) const 
    {
        if (isNegative == num.isNegative) { //同號相加
            HugeInt temp; //用來儲存結果的變數
            int carry = 0; //一開始是沒有進位狀態
            for (int i = SIZE - 1; i >= 0; i--) //從陣列的最後一格（個位數）開始往前加
            {
                temp.integer[i] = integer[i] + num.integer[i] + carry; 
                carry = temp.integer[i] / 10; //如果加起來 ≥ 10，就進位 1
                temp.integer[i] %= 10; //去掉十位數，只保留個位數
            }
            temp.isNegative = isNegative; //因為是「同號相加」，所以結果的符號和原來一樣
            return temp;
        }
        return isNegative ? (num - (-*this)) : (*this - (-num)); //異號相加
        // 如果 this 是負的，就把它變正，再用 num 減它 // 否則把 num 變正，再用 this 減它
    }

//大數減法
    HugeInt operator-(const HugeInt &num) const 
    {
        if (isNegative != num.isNegative) //異號相減，把減法轉成加法來處理
        {
            return *this + (-num);
        }
        if (absLessThan(*this, num)) //同號相減，先判斷*this的絕對值是否比num小
        {
            HugeInt temp = num - *this;
            temp.isNegative = !isNegative;
            return temp;
        }
        HugeInt temp; //同號相減，絕對值較大的減絕對值較小
        int borrow = 0; //減法中向前一位借1的意思
        for (int i = SIZE - 1; i >= 0; i--) 
        {
            temp.integer[i] = integer[i] - num.integer[i] - borrow;
            if (temp.integer[i] < 0) //變成負數，需要借位
            {
                temp.integer[i] += 10; //從前一為借1，相當於+10
                borrow = 1; //下一輪（左邊那一位）借出1
            } 
            else //沒有小於0，不需借位
            {
                borrow = 0;
            }
        }
        temp.isNegative = isNegative; //因為是this比num大的情況，結果的正負跟this一樣
        return temp;
    }

//大數乘法
    HugeInt operator*(const HugeInt &num) const 
    {
        HugeInt temp;
        for (int i = SIZE - 1; i >= 0; i--) //用直式的乘法下去想
        {
            for (int j = SIZE - 1; j >= 0; j--) 
            {
                int pos = i + j - (SIZE - 1); //乘完的結果要放的位置
                if (pos < 0) continue;
                temp.integer[pos] += integer[i] * num.integer[j]; //把乘積加到正確的位置上
            }
        }
        int carry = 0; //處理進位
        for (int i = SIZE - 1; i >= 0; i--) //處理進位
        {
            temp.integer[i] += carry;
            carry = temp.integer[i] / 10;
            temp.integer[i] %= 10;
        }
        temp.isNegative = (isNegative != num.isNegative); //如果兩個數符號不同則結果為負
        return temp;
    }

    HugeInt operator/(const HugeInt &divisor) const 
    {
        if (divisor.isZero()) {
            HugeInt error;
            error.integer[SIZE - 1] = 9; // dummy error value
            return error;
        }
    
        HugeInt result, remainder;
        for (int i = 0; i < SIZE; i++) {
            result.integer[i] = 0;
            remainder.integer[i] = 0;
        }
    
        //  先取絕對值再做除法
        HugeInt absDividend = *this;
        HugeInt absDivisor = divisor;
        absDividend.isNegative = false;
        absDivisor.isNegative = false;
    
        for (int i = 0; i < SIZE; i++) { //用直式除法下去想，被除數從最高位（最左邊）一路掃到最右邊（個位數）
            remainder = remainder * HugeInt("10"); //要拉下一位下來之前，先補個 0 的概念
            remainder.integer[SIZE - 1] = integer[i];  ////把被除數 A 的第 i 位數字，加到目前的餘數後面（個位）
            result.integer[i] = 0;
    
            for (int d = 9; d >= 0; d--) { ////哪一個數字乘以除數，不會超過目前的餘數，然後把它當作這一位的商數（找最大的商數）
                HugeInt trial = absDivisor * HugeInt(to_string(d)); ///把 d（int）轉成字串 → 再變成 HugeInt
                if (!(remainder < trial)) {
                    result.integer[i] = d;
                    remainder = remainder - trial;
                    break;
                }
            }
        }
    
        //  設定正負號（如果結果是 0，不要設負）
        bool isAllZero = true;
        for (int i = 0; i < SIZE; i++) {
            if (result.integer[i] != 0) {
                isAllZero = false;
                break;
            }
        }
    
        result.isNegative = isAllZero ? false : (isNegative != divisor.isNegative);
        return result;
    }
    

    
//把一個 HugeInt 的內容轉換成字串
    string toString() const  
    {
        string result;
        bool leadingZero = true; //不印出前面的0
        for (int i = 0; i < SIZE; ++i) 
        {
            if (leadingZero && integer[i] == 0) continue;
            leadingZero = false;
            result += (char)(integer[i] + '0'); //把數字轉成字元，加進字串
        }
        if (result.empty()) result = "0"; 
        if (isNegative && result != "0") result = "-" + result;
        return result;
    }
};

int main() 
{
    ifstream fin("testbench.txt"); //輸入 
    ofstream fout("ans.txt"); //輸出
    string x, y; //用字串 x、y 接收
    while (fin >> x >> y) 
    {
        HugeInt X(x), Y(y); //把字串轉成 HugeInt
        HugeInt add = X + Y;
        HugeInt sub = X - Y;
        HugeInt mul = X * Y;

        fout << add.toString() << endl; 
        fout << sub.toString() << endl;
        fout << mul.toString() << endl;

        // 只在 Y 不是 0 的時候才做除法
        if (Y.isZero()) {
            fout << "錯誤: 除數不可為0" << endl;
        } else {
            HugeInt div = X / Y;
            fout << div.toString() << endl;
        }
        fout << endl;
    }
    

    fin.close();
    fout.close();
    return 0;
}
