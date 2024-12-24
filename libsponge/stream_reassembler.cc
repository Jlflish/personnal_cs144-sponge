#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _filled_stream(capacity, {' ', false}), _output(capacity), _capacity(capacity) {

}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.

void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    auto _start_pos = max(index, _current_pos);
    auto _end_pos = min(index + data.size(), min(_current_pos + _capacity - _output.buffer_size(), _end_index));
    if (eof) _end_index = index + data.size();
    for (size_t i = _start_pos, j = _start_pos - index; i < _end_pos; i++, j++) {
        if (!_filled_stream[i % _capacity].second) {
            _filled_stream[i % _capacity] = make_pair(data[j], true);
            _unassembled_byte_cnt++;
        }
    }

    string _add;
    while (_current_pos < _end_index && _filled_stream[_current_pos % _capacity].second) { 
        _add += _filled_stream[_current_pos % _capacity].first;
        _filled_stream[_current_pos % _capacity].second = false;
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
