#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : buffer_stream(capacity), total_capacity(capacity) {

}

size_t ByteStream::write(const string &data) {
    const size_t put_size = min(total_capacity - current_size, data.size());
    for (int i = 0; i < put_size; i++) {
        buffer_stream[tail] = data[i];
        tail = tail == total_capacity ? 0 : tail + 1; 
    }
    current_size += put_size;
    written_cnt += put_size;
    return put_size;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    const size_t copy_size = min(current_size, len);
    string copy_result;
    for (int i = 0, j = head; i < copy_size; i++) {
        copy_result += buffer_stream[j];
        j = j == total_capacity ? 0 : j + 1;
    }
    return copy_result;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    const size_t pop_size = min(current_size, len);
    head += pop_size;
    if (head >= total_capacity) {
        head -= total_capacity;
    }
    current_size -= pop_size;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    const size_t read_size = min(read_size, len);
    string read_result;
    for (int i = 0, j = head; i < read_size; i++) {
        read_result += buffer_stream[j];
        j = j == total_capacity ? 0 : j + 1;
    }
    pop_output(read_size);
    read_cnt += read_size;
    return read_result;
}

void ByteStream::end_input() { input_ended_flag = true; }

bool ByteStream::input_ended() const { return input_ended_flag; }

size_t ByteStream::buffer_size() const { return total_capacity; }

bool ByteStream::buffer_empty() const { return current_size == 0; }

bool ByteStream::eof() const { return input_ended_flag && current_size == 0; }

size_t ByteStream::bytes_written() const { return written_cnt; }

size_t ByteStream::bytes_read() const { return read_cnt; }

size_t ByteStream::remaining_capacity() const { return total_capacity - current_size; }
