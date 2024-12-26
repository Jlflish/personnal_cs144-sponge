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
    _reassembler.push_substring(seg.payload().str(), seg.header().seqno - (seg.header().syn));
}

optional<WrappingInt32> TCPReceiver::ackno() const { 
    if (_SYN_recieved) {
        return _SYN_NO + _reassembler.
    }
    return {};
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.unassembled_bytes(); }
