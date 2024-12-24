#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _buffer_stream(capacity), _total_capacity(capacity) {

}
size_t ByteStream::write(const string &data) {
    const size_t put_size = min(remaining_capacity(), data.size());
    for (size_t i = 0; i < put_size; i++) {
        _buffer_stream[_tail] = data[i];
        _tail = _tail + 1 == _total_capacity ? 0 : _tail + 1; 
    }
    _current_size += put_size;
    _written_cnt += put_size;
    return put_size;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    const size_t copy_size = min(_current_size, len);
    string copy_result;
    for (size_t i = 0, j = _head; i < copy_size; i++) {
        copy_result += _buffer_stream[j];
        j = j + 1 == _total_capacity ? 0 : j + 1;
    }
    return copy_result;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    const size_t pop_size = min(_current_size, len);
    _head = (_head + pop_size) % _total_capacity;
    _current_size -= pop_size;
    _read_cnt += pop_size;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    const string read_result = peek_output(len);
    pop_output(len);
    return read_result;
}

void ByteStream::end_input() { _input_ended_flag = true; }

bool ByteStream::input_ended() const { return _input_ended_flag; }

size_t ByteStream::buffer_size() const { return _current_size; }

bool ByteStream::buffer_empty() const { return _current_size == 0; }

bool ByteStream::eof() const { return _input_ended_flag && _current_size == 0; }

size_t ByteStream::bytes_written() const { return _written_cnt; }

size_t ByteStream::bytes_read() const { return _read_cnt; }

size_t ByteStream::remaining_capacity() const { return _total_capacity - _current_size; }