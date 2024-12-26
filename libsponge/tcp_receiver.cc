#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    if (seg.header().syn) {
        _SYN_recieved = true;
        _SYN_NO = WrappingInt32(seg.header().seqno);
    }
    
    if (!_SYN_recieved) {
        return;
    }

    uint64_t abs_ackno = _reassembler.stream_out().bytes_written() + 1;
    uint64_t curr_abs_seqno = unwrap(seg.header().seqno, _SYN_NO, abs_ackno);
    uint64_t stream_index = curr_abs_seqno - 1 + (seg.header().syn);
    _reassembler.push_substring(string{seg.payload().str()}, stream_index, seg.header().fin);
}

optional<WrappingInt32> TCPReceiver::ackno() const { 
    if (!_SYN_recieved) {
        return {};
    }
    
    WrappingInt32 result = _SYN_NO + _reassembler.stream_out().bytes_written() + 1;
    if (_reassembler.stream_out().input_ended()) {
        result = result + 1;
    }
    return result;
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }
