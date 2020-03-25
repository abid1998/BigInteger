#include <iostream>
#include <vector>
#include <string>

class big_integer
{
public:
    big_integer()
      : digits(1, 0)
    {}

    big_integer(unsigned int x) {
        do {
            digits.push_back(x % 10);
            x /= 10;
        } while (x > 0);
    }

    big_integer(const std::string &s) {
        for(int i=0; s[i] != '\0'; i++)
        {
            digits.push_back(static_cast<int>(s[i]) - 48);
        }
    }

    big_integer(const std::vector<int> &v) {
        digits = v;
     }

    big_integer& operator++() {
        digits[0]++;
        handle_carry();
        return *this;
    }

    big_integer operator++(int) {
        big_integer copy(*this);
        ++copy;
        return copy;
    }

    big_integer operator/(const big_integer& o) {
        big_integer res;
        big_integer reminder(*this);
        int power = reminder.digits.size() - o.digits.size();
        if (power <= 0) {
            return 0;
        }
        res.digits.resize(power + 1, 0);
        while (reminder > o) {
            for (int d = 1; d <= 10; ++d) {
                big_integer temp;
                temp.digits.resize(power,  0);
                temp.digits.push_back(d);
                if (temp * o > reminder) {
                    res.digits[power] = d - 1;
                    temp.digits[power] = d - 1;
                    reminder = reminder - temp * o;
                    power--;
                    break;
                }
            }
        }
        res.remove_leader_zeroes();
        return res;
    }

    big_integer operator+(const big_integer& o) {
        big_integer res;
        res.digits.resize(std::max(digits.size(), o.digits.size()));
        for (int i = 0; i < res.digits.size(); ++i) {
            res.digits[i] = get_digit(i) + o.get_digit(i);
        }
        res.handle_carry();
        return res;
    }

    big_integer operator-(const big_integer& o) {
        big_integer res;
        res.digits.resize(std::max(digits.size(), o.digits.size()));
        for (int i = 0; i < res.digits.size(); ++i) {
            res.digits[i] = get_digit(i) - o.get_digit(i);
        }
        res.handle_borrow();
        res.remove_leader_zeroes();
        return res;
    }

    big_integer operator*(const big_integer& o) {
        big_integer res;
        res.digits.resize(digits.size() + o.digits.size(), 0);
        for (int i = 0; i < digits.size(); ++i) {
            for (int j = 0; j < o.digits.size(); ++j) {
                res.digits[i + j] += digits[i] * o.digits[j];
            }
        }
        res.handle_carry();
        res.remove_leader_zeroes();
        return res;
    }

    bool operator==(const big_integer& o) const{
        if (o.digits.size() != digits.size())
            return false;
        for (int i = 0; i < digits.size(); ++i) {
            if (o.digits[i] != digits[i])
                return false;
        }
        return true;
    }

    bool operator!=(const big_integer& o) const {
        return !(*this == o);
    }

    bool operator<(const big_integer& o) const {
        if (digits.size() < o.digits.size())
            return true;
        if (digits.size() > o.digits.size())
            return false;

        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] < o.digits[i])
                return true;
            if (digits[i] > o.digits[i])
                return false;
        }
        return false;
    }

    bool operator<=(const big_integer& o) const{
        return *this < o || *this == o;
    }

    bool operator>(const big_integer& o) const{
        return o < *this;
    }

    bool operator>=(const big_integer& o) const{
        return o <= *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const big_integer& num);
private:

    std::vector<int> digits;
    void handle_carry() {
        for (int i = 0; i < digits.size(); ++i) {
            int carry = digits[i] / 10;
            digits[i] %= 10;
            if (i == digits.size() - 1 && carry != 0) {
                digits.push_back(carry);
            } else {
                digits[i + 1] += carry;
            }
        }
    }

    void handle_borrow() {
    
        for (int i = 0; i < digits.size(); ++i) {
            if (digits[i] < 0) {
                digits[i] += 10;
                digits[i + 1]--;
            }
        }
    }

    void remove_leader_zeroes() {
        while (digits.size() > 1 && digits[digits.size() - 1] == 0)
            digits.pop_back();
    }

    int get_digit(int idx) const {
        if (idx < 0)
            throw std::exception(); 
        if (idx > digits.size() - 1)
            return 0;
        return digits[idx];
    }

};

std::ostream& operator<<(std::ostream& out, const big_integer& num){
   for (int i = num.digits.size() - 1; i >= 0; --i) {
       out << num.digits[i];
    }
   return out;
}

int main()
{
    std::string s1 = "21010101011";
    big_integer x(s1);
    std::vector<int> s2(10, 9);
    big_integer y(s2);
    
    std::cout << x << " " << y << std::endl;
    big_integer z = x + y;
    std::cout << z << std::endl;
    big_integer a = z + x + y;
    std::cout << a << std::endl;
    std::cout << (z == 42) << std::endl;
    std::cout << "NON-EQ test" << std::endl;
    std::cout << (z != 42) << std::endl;
    std::cout << "NON-EQ test ended" << std::endl;
    std::cout << (z < 42) << std::endl;
    std::cout << "TEST" << std::endl;
    std::cout << (z + z + 1 < z + z + 2) << std::endl;
    std::cout << "SUBTRACTION TEST" << std::endl;
    big_integer q = 1700;
    std::cout << q - 18 << std::endl;
    std::cout << "DIVISION TEST" << std::endl;
    big_integer d = 1280;
    std::cout << d / 14 << std::endl;

    return 0;
}