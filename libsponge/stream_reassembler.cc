#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _next(capacity + 1), _output(capacity), _capacity(capacity) {
    iota(_next.begin(), _next.end(), 0);
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.

size_t StreamReassembler::find(size_t pos) {
    if (_next[pos] == pos) {
        return pos;
    }
    return _next[pos] = find(_next[pos]);
}

void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    
    DUMMY_CODE(data, index, eof);
}

size_t StreamReassembler::unassembled_bytes() const { return {}; }

bool StreamReassembler::empty() const { return {}; }
