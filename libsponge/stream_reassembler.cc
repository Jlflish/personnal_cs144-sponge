#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _next(),  _filled_stream(capacity, {' ', false}), _output(capacity), _capacity(capacity) {
    _next.set();
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.

size_t StreamReassembler::find(size_t pos) {
    if (_next[pos]) {
        return pos;
    }
    return _next._Find_next(pos);
}

void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if (eof) _end_index = index + data.size(); 
    for (size_t i = find(index); i != _capacity && i - index < data.size(); i = find(i)) {
        _filled_stream[i] = make_pair(data[i - index], true);
        _unassembled_byte_cnt++;
        _next.reset(i);
    }

    string _add;
    while (_current_pos < _end_index && _filled_stream[_current_pos].second) { 
        _add += _filled_stream[_current_pos].first;
        _filled_stream[_current_pos].second = false;
        _next.set(_current_pos);
        _unassembled_byte_cnt--;
        _current_pos++;
    }
    _output.write(_add);

    if (_current_pos == _end_index) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_byte_cnt; }

bool StreamReassembler::empty() const { return _unassembled_byte_cnt == 0; }
