#include <gmpxx.h>
#include <math.h>

using integer = mpz_class;

class FixedPointBigNum {
private:
	unsigned long subvalue;
	integer ivalue;
public:
	FixedPointBigNum(double value) {
		ivalue = floor(value);
		subvalue = (value - ivalue.get_d()) * (1 >> 64);
	}
	FixedPointBigNum(size_t subvalue, integer ivalue): subvalue(subvalue), ivalue(ivalue) {}
	~FixedPointBigNum() {
		ivalue.~integer();
	}
	integer* get_ivalue() {
		return &ivalue;
	}
	size_t* get_subvalue() {
		return &subvalue;
	}
	void operator=(double value) {
		*this = FixedPointBigNum(value);
	}
};

FixedPointBigNum operator+(FixedPointBigNum a, FixedPointBigNum b) {
	integer a_ivalue = *(a.get_ivalue());
	integer b_ivalue = *(b.get_ivalue());
	size_t a_subvalue = *(a.get_subvalue());
	size_t b_subvalue = *(b.get_subvalue());

	size_t res_subvalue = a_subvalue + b_subvalue;
	bool carry = (res_subvalue) < a_subvalue;
	integer res_ivalue = a_ivalue + b_ivalue + carry;
	return FixedPointBigNum(res_subvalue, res_ivalue);
}

FixedPointBigNum operator-(FixedPointBigNum a, FixedPointBigNum b) {
	integer a_ivalue = *(a.get_ivalue());
	integer b_ivalue = *(b.get_ivalue());
	size_t a_subvalue = *(a.get_subvalue());
	size_t b_subvalue = *(b.get_subvalue());

	size_t res_subvalue = a_subvalue - b_subvalue;
	bool carry = (res_subvalue) > a_subvalue;
	integer res_ivalue = a_ivalue - b_ivalue - carry;
	return FixedPointBigNum(res_subvalue, res_ivalue);
}

FixedPointBigNum operator*(FixedPointBigNum a, FixedPointBigNum b) {
	integer a_ivalue = *(a.get_ivalue());
	integer b_ivalue = *(b.get_ivalue());
	size_t a_subvalue = *(a.get_subvalue());
	size_t b_subvalue = *(b.get_subvalue());

	integer res_ivalue = a_ivalue * b_ivalue;
	size_t res_subvalue = a_subvalue * b_subvalue;
	return FixedPointBigNum(res_subvalue, res_ivalue);
}
