/*!
 * minter_tx.
 * utils.cpp
 *
 * \date 2019
 * \author Eduard Maximovich (edward.vstock@gmail.com)
 * \link   https://github.com/edwardstock
 */
#include <sstream>
#include <algorithm>
#include <minter/crypto/sha3.h>
#include <minter/tx/utils.h>

dev::bytes minter::utils::to_bytes(std::string &&input) {
    if(!input.size()) {
        return dev::bytes(0);
    }

    dev::bytes output;
    output.resize(input.size());
    std::move(input.begin(), input.end(), output.begin());

    return output;
}
dev::bytes minter::utils::to_bytes(const std::string &input) {
    if (!input.size()) {
        return dev::bytes(0);
    }

    dev::bytes output;
    output.resize(input.length());
    for (auto i = 0; i < input.size(); i++) {
        output[i] = (uint8_t) input[i];
    }

    return output;
}

dev::bytes minter::utils::to_bytes_fixed(const std::string &input, size_t fixed_size) {
    dev::bytes output;
    output.resize(fixed_size);
    size_t left = fixed_size;
    size_t fill_from = 0;
    for (auto i = 0; i < input.size() && i < fixed_size; i++, left--, fill_from++) {
        output[i] = (uint8_t) input[i];
    }

    for (auto i = fill_from; i < fixed_size; i++) {
        output[i] = (uint8_t) 0x00;
    }

    return output;
}

dev::bytes minter::utils::to_bytes(const dev::bigint &num) {
    dev::bytes v;
    boost::multiprecision::export_bits(num, std::back_inserter(v), 8);

    return v;
}

dev::bytes minter::utils::to_bytes(const dev::u256 &num) {
    dev::bytes v;
    boost::multiprecision::export_bits(num, std::back_inserter(v), 8);

    return v;
}

dev::bytes minter::utils::to_bytes(const dev::bigdec18 &num) {
    auto out = num * dev::bigdec18("1000000000000000000");
    auto nt = dev::bigint(out);

    return to_bytes(nt);
}

dev::bytes minter::utils::sha3k(const minter::Data &message) {
    return sha3k(message.get());
}

dev::bytes minter::utils::sha3k(const dev::bytes &message) {
    SHA3_CTX hash_ctx;
    dev::bytes output(32);
    keccak_256_Init(&hash_ctx);
    keccak_Update(&hash_ctx, message.data(), message.size());
    keccak_Final(&hash_ctx, &output[0]);

    return output;
}

dev::bigint minter::utils::to_bigint(const dev::bytes &bytes) {
    dev::bigint val;
    boost::multiprecision::import_bits(val, bytes.begin(), bytes.end());
    return val;
}

dev::bigint minter::utils::to_bigint(const uint8_t *bytes, size_t len) {
    return to_bigint(dev::bytes(bytes, bytes + len));
}

std::string minter::utils::to_string(const dev::bytes &src) {
    std::stringstream ss;
    for(const auto& item: src) {
        ss << item;
    }

    return ss.str();
}

std::string minter::utils::to_string(const dev::bigdec18 &src) {
    std::stringstream ss;
    ss << std::setprecision(std::numeric_limits<boost::multiprecision::cpp_dec_float<18>>::max_digits10);
    ss << src;

    return toolboxpp::numbers::decimal_formatter()(ss.str());
}

std::string minter::utils::to_string_lp(const dev::bigdec18 &src) {
    std::stringstream ss;
    ss << std::setprecision(std::numeric_limits<boost::multiprecision::cpp_dec_float<4>>::max_digits10);
    ss << src;

    return toolboxpp::numbers::decimal_formatter(ss.str()).set_min_fractions(4).set_max_fractions(4).format();
}

std::string minter::utils::to_string(const dev::bigint &src) {
    std::stringstream ss;
    ss << src;

    return ss.str();
}

std::string minter::utils::to_string(uint64_t src) {
    std::stringstream ss;
    ss << src;
    return ss.str();
}

std::string minter::utils::to_string(const std::vector<char> &data) {
    return std::string(data.begin(), data.end());
}

std::string minter::utils::to_string_clear(const dev::bytes &src) {
    const std::string tmp(to_string(src));
    const char* tmp2 = tmp.c_str();
    return strip_null_bytes(tmp2);
}


std::string minter::utils::strip_null_bytes(const char* input) {
    if(input == nullptr || strlen(input) == 0) {
        return std::string();
    }

    std::stringstream ss;
    for(size_t i = 0; i < strlen(input); i++) {
        if(input[i] == 0x00) {
            continue;
        }

        ss << input[i];
    }

    return ss.str();
}

dev::bigint minter::utils::normalize_value(const char* input) {
    return dev::bigint(dev::bigdec18(input) * minter::utils::normalized_value_dec);
}

dev::bigint minter::utils::normalize_value(const std::string &input) {
    return minter::utils::normalize_value(input.c_str());
}

dev::bigint minter::utils::normalize_value(const dev::bigdec18 &value) {
    return dev::bigint(value * minter::utils::normalized_value_dec);
}

dev::bigdec18 minter::utils::humanize_value(const dev::bigint &value) {
    return dev::bigdec18(value) / minter::utils::normalized_value_dec;
}

std::ostream &operator << (std::ostream &out, const minter::Data &d) {
    out << d.toHex();
    return out;
}

std::ostream &operator << (std::ostream &out, const dev::bytes &d) {
    minter::Data tmp(d);
    out << tmp.toHex();
    return out;
}

std::ostream &operator << (std::ostream &out, const dev::RLPStream &rlp) {
    minter::Data tmp(rlp.out());
    out << tmp.toHex();
    return out;
}