#include "wrapper.h"

Wrapper::Wrapper(Value const &value) : v{value} {};
Wrapper::Wrapper(Value &value) : v{value} {};

Wrapper::operator Value &() { return v; };
Wrapper::operator const Value &() const { return v; };

Value & Wrapper::value() {
    return v;
}

Value const & Wrapper::value() const {
    return v;
}
