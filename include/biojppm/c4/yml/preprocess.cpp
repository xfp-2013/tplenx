#include "c4/yml/preprocess.hpp"
#include "c4/yml/detail/parser_dbg.hpp"
#include <cstdio>

/** @file preprocess.hpp Functions for preprocessing YAML prior to parsing. */

namespace c4 {
namespace yml {


namespace {
struct _SubstrWriter
{
    substr buf;
    size_t pos;
    _SubstrWriter(substr buf_, size_t pos_=0) : buf(buf_), pos(pos_) {}
    void operator()(csubstr s)
    {
        C4_ASSERT(!s.overlaps(buf));
        if(pos + s.len <= buf.len)
        {
            memcpy(buf.str + pos, s.str, s.len);
        }
        pos += s.len;
    }
    void operator()(char c)
    {
        if(pos < buf.len)
        {
            buf.str[pos] = c;
        }
        ++pos;
    }
    size_t excess() const { return pos > buf.len ? pos - buf.len : 0; }
    //! get the part written so far
    csubstr curr() const { return pos <= buf.len ? buf.first(pos) : buf; }
    //! get the part that is still free to write to
    substr rem() { return pos <= buf.len ? buf.sub(pos) : substr(buf.end(), size_t(0u)); }

    size_t advance(size_t more) { pos += more; return pos; }
};

bool _is_idchar(char c)
{
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || (c == '_' || c == '-' || c == '~' || c == '$');
}

typedef enum { kReadPending = 0, kKeyPending = 1, kValPending = 2 } _ppstate;
_ppstate _next(_ppstate s)
{
    int n = (int)s + 1;
    return (_ppstate)(n <= (int)kValPending ? n : 0);
}
} // empty namespace


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

size_t preprocess_json(csubstr s, substr buf)
{
    _SubstrWriter _append(buf);
    size_t last = 0; // the index of the last character in s that was copied to buf

    // append everything that was not written yet
    #define _apfromlast() { csubstr _s_ = (s.range(last, i)); _append(_s_); last += _s_.len; i += _s_.len; }
    #define _adv(n) { size_t _n_ = (n); _append.advance(_n_); last += _n_; i += _n_; }
    #define _apelm(c) { _append(c); ++last; ++i; }   // append element from the buffer

    for(size_t i = 0; i < s.len; ++i)
    {
        const char curr = s[i];
        const char next = i+1 < s.len ? s[i+1] : '\0';
        if(curr == ':')  // if it was missing, add a space after semicolon
        {
            if(next == '"' || next == '\'' || next == '{' || next == '['
               || (next >= '0' && next <= '9'))
            {
                _apfromlast();
                _apelm(':');
                _append(' ');
            }
        }
        else if((curr == '{' || curr == '[') && next != '\0') // recurse into substructures
        {
            const char close = static_cast<char>(curr + 2); // in ascii: {=123,}=125 and [=91,]=93. So add 2!
            // get the contents inside the brackets
            csubstr ss = s.sub(i).pair_range_nested(curr, close);
            C4_ASSERT(ss.size() >= 2);
            C4_ASSERT(ss.ends_with(close));
            ss = ss.offs(1, 1); // skip the open-close bracket characters
            _apfromlast();
            _apelm(curr);
            if(!ss.empty())  // recurse into the substring
            {
                size_t ret = preprocess_json(ss, _append.rem());
                _adv(ret);
            }
            _apelm(close);
        }
        else if(curr == '\'' || curr == '"')  // consume quoted strings at once
        {
            csubstr ss = s.sub(i).pair_range_esc(curr, '\\');
            C4_ASSERT(ss.end() >= (s.str + i));
            i += ss.end() - (s.str + i);
            C4_ASSERT(i > 0);
            i -= 1; //
        }
    }

    if(last + 1 < s.len)
    {
        _append(s.sub(last));
    }

    #undef _apfromlast
    #undef _apchar
    #undef _apelm
    #undef _adv

    return _append.pos;
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

size_t preprocess_rxmap(csubstr s, substr buf)
{
    _SubstrWriter _append(buf);
    _ppstate state = kReadPending;
    size_t last = 0;

    if(s.begins_with('{'))
    {
        RYML_CHECK(s.ends_with('}'));
        s = s.offs(1, 1);
    }

    _append('{');

    for(size_t i = 0; i < s.len; ++i)
    {
        const char curr = s[i];
        const char next = i+1 < s.len ? s[i+1] : '\0';

        if(curr == '\'' || curr == '"')
        {
            csubstr ss = s.sub(i).pair_range_esc(curr, '\\');
            i += ss.end() - (s.str + i);
            state = _next(state);
        }
        else if(state == kReadPending && _is_idchar(curr))
        {
            state = _next(state);
        }

        switch(state)
        {
        case kKeyPending:
        {
            if(curr == ':' && next == ' ')
            {
                state = _next(state);
            }
            else if(curr == ',' && next == ' ')
            {
                _append(s.range(last, i));
                _append(": 1, ");
                last = i + 2;
            }
            break;
        }
        case kValPending:
        {
            if(curr == '[' || curr == '{' || curr == '(')
            {
                csubstr ss = s.sub(i).pair_range_nested(curr, '\\');
                i += ss.end() - (s.str + i);
                state = _next(state);
            }
            else if(curr == ',' && next == ' ')
            {
                state = _next(state);
            }
            break;
        }
        default:
            // nothing to do
            break;
        }
    }

    _append(s.sub(last));
    if(state == kKeyPending) _append(": 1");
    _append('}');

    return _append.pos;
}


} // namespace yml
} // namespace c4
