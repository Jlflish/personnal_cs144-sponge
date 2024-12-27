#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity) 
    , _ackno(0) {}

uint64_t TCPSender::bytes_in_flight() const { return {}; }

void TCPSender::fill_window() {
    TCPSegment output;
    auto header = output.header();
    auto payload = output.payload();
    size_t output_size = {};
    if (!_SYN_flag) {
        output_size += 1;
        header.syn = true;
    }
    const size_t buffer_size = _stream.buffer_size();
    size_t _size;
    if (TCPConfig::MAX_PAYLOAD_SIZE > buffer_size + output_size) {
        _size = TCPConfig::MAX_PAYLOAD_SIZE - output_size;
    } else if (TCPConfig::MAX_PAYLOAD_SIZE == buffer_size + output_size) {
        _size = buffer_size - 1;
    } else {
        _size = buffer_size;
    }

    if (_size == buffer_size) {
        header.fin = true;
    }
    header.seqno = wrap(_next_seqno, _isn);
    _next_seqno += _size;

    payload = Buffer(_stream.read(_size));
    _segments_out.push(output);
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) { DUMMY_CODE(ackno, window_size); }

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) { DUMMY_CODE(ms_since_last_tick); }

unsigned int TCPSender::consecutive_retransmissions() const { return {}; }

void TCPSender::send_empty_segment() {}
